import datetime
from typing import List, Tuple

import matplotlib.pyplot as plt
import numpy as np
import seaborn

from core.util_functions import (
    convert_list_to_np_array,
    filter_garbage,
    get_data_from_csv,
    get_node_average,
    subtract,
)
from utils import logger


def get_pressure_data_from_csv(
    csv_path: str, is_calibrated: bool
) -> Tuple[List[int], List[int]]:
    total_list = get_data_from_csv(csv_path=csv_path)
    calibration_data = None

    if is_calibrated:
        calibration_data = [int(node) for node in total_list[-1]]
        total_list = total_list[:-1]

    result = filter_garbage(total_list=total_list)
    result = get_node_average(data=np.array(result))

    return calibration_data, result


def plot_heatmap(data: np.array, num_col: int, num_row: int, save_path: str):
    plt.style.use("seaborn")
    plt.figure(figsize=(num_col, num_row))
    plt.title("Pressure Heat Map", fontsize=50)
    hm = seaborn.heatmap(data, linewidth=0.30, annot=False, cmap="Blues")
    cbar = hm.collections[0].colorbar
    cbar.ax.tick_params(labelsize=50)
    plt.tick_params(axis="y", labelsize=30)
    plt.tick_params(axis="x", labelsize=30)
    plt.savefig(save_path)
    plt.clf()
    plt.cla()
    plt.close()
    logger.info(f"Successfully created heatmap: {save_path}")


def create_pressure_heatmap(
    csv_path: str, num_col: int = 32, num_row: int = 57, is_calibrated: bool = True
) -> str:
    calibration, data = get_pressure_data_from_csv(
        csv_path=csv_path, is_calibrated=is_calibrated
    )
    if is_calibrated:
        data = subtract(a=data, b=calibration)
        logger.info(f"calibrated data: {data}")

    data = convert_list_to_np_array(original_list=data, num_col=num_col)
    now = datetime.datetime.now().strftime("%Y_%m_%d_%H_%M_%S")
    file_name = f"pressure_data/heatmap/{now}.png"
    plot_heatmap(data=data, num_col=num_col, num_row=num_row, save_path=file_name)
    return file_name


if __name__ == "__main__":
    create_pressure_heatmap(csv_path="pressure_data/testC.csv")
