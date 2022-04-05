import re
from datetime import datetime
from pathlib import Path
from typing import List

import matplotlib.pyplot as plt
import numpy as np

from core.constants import FIT_CURVE_DEGREE, NUM_COL, WEIGHTS
from core.heatmap_creator import get_final_pressure_data
from core.util_functions import (
    create_2d_array_from_1d,
    get_data_from_csv,
    get_fit_curve_coefficients,
    write_1d_list_to_csv,
)
from utils import logger


def get_listed_val_from_csv(csv_path: str) -> List[List]:
    data = get_final_pressure_data(csv_path=csv_path, is_calibrated=True)
    listed_val = []
    for row in data:
        listed_val.append([round(float(val), 2) for val in row])

    return listed_val


def iterate_through_calibration_folder():
    path_list = Path("pressure_data/panel_1").rglob("*.csv")
    for path in path_list:
        path_in_str = str(path)
        logger.info(f"Parsing {path_in_str}")

        listed_val = get_listed_val_from_csv(csv_path=path_in_str)
        weight = int(re.findall(r"\d+", path_in_str)[-1])
        logger.info(f"WEIGHT: {weight}")
        concatenated_data = list(np.concatenate(listed_val).flat)
        concatenated_data.insert(0, weight)
        write_1d_list_to_csv(
            csv_path="coeff_data/drained_calibrated.csv", data=concatenated_data
        )


def stack_vout_vals(vout_vals: np.array) -> np.array:
    res = vout_vals[0]
    for vals in vout_vals[1:]:
        res = np.dstack((res, vals))
    return res


def plot_from_coeff(item, title):
    plt.figure()
    plt.style.use("seaborn")

    trend = np.polyfit(WEIGHTS, item, FIT_CURVE_DEGREE)
    x_line = np.linspace(0, 2500)
    fit = np.polyval(trend, x_line)
    plt.plot(x_line, fit)
    plt.scatter(WEIGHTS, item)
    plt.title(f"Voltage vs. Weight for coordinate {title}")
    plt.xlabel("Weight [grams]")
    plt.ylabel("Voltage [mV]")

    now = datetime.now().strftime("%Y_%m_%d_%H_%M_%S")
    save_path = f"coeff_data/{title}_{now}.png"
    plt.savefig(save_path)
    plt.clf()
    plt.cla()
    plt.close()
    logger.info(f"Successfully created coeff plot: {save_path}")


if __name__ == "__main__":
    # iterate_through_calibration_folder()

    total_list = get_data_from_csv(csv_path="coeff_data/drained_calibrated.csv")
    vout_per_weight = dict()

    for values_per_weight in total_list:
        weight = int(float(values_per_weight[0]))
        print(f"Weight converted: {weight}")
        float_converted = [float(val) for val in values_per_weight[1:]]
        vout_per_weight[weight] = create_2d_array_from_1d(
            original_array=float_converted, num_col=NUM_COL
        )

    sorted_list = []
    for weight in WEIGHTS:
        sorted_list.append(vout_per_weight[weight])

    stacked = stack_vout_vals(vout_vals=sorted_list)
    item_list = []
    # print(list(stacked[13][31]))  # ROW AND THEN COLUMN
    for r, row in enumerate(stacked):
        for c, node in enumerate(row):
            item_list.append(list(node))
            coeff = get_fit_curve_coefficients(vout_data=node)
            # location = zip(*np.where(stacked == list(item)))
            plot_from_coeff(item=node, title=f"({c}, {r})")
