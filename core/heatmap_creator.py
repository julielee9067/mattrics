import datetime
from typing import List

import matplotlib.pyplot as plt
import numpy as np
import seaborn

from core.constants import NUM_COL, NUM_ROW
from core.util_functions import (
    convert_list_to_np_array,
    filter_garbage,
    get_data_from_csv,
    get_node_average,
    subtract,
)
from utils import logger


def get_pressure_data_from_csv(csv_path: str, is_calibrated: bool) -> List[int]:
    total_list = get_data_from_csv(csv_path=csv_path)

    if is_calibrated:
        calibration_data = [int(node) for node in total_list[-1]]
        total_list = total_list[:-1]
        filtered_data = filter_garbage(total_list=total_list)
        avg_data = get_node_average(data=np.array(filtered_data))
        result = subtract(a=avg_data, b=calibration_data)
        logger.info(f"calibrated data: {result}")

        return result

    filtered_data = filter_garbage(total_list=total_list)
    result = get_node_average(data=np.array(filtered_data))

    return result


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


def get_diff_between_two_data(
    first_csv_path: str,
    second_csv_path: str,
    f_calibrated: bool = True,
    s_calibrated: bool = True,
):
    f_data = get_pressure_data_from_csv(
        csv_path=first_csv_path, is_calibrated=f_calibrated
    )
    s_data = get_pressure_data_from_csv(
        csv_path=second_csv_path, is_calibrated=s_calibrated
    )
    difference = subtract(a=f_data, b=s_data)
    return convert_list_to_np_array(original_list=difference, num_col=NUM_COL)


def get_final_pressure_data(csv_path: str, is_calibrated: bool = True) -> np.array:
    data = get_pressure_data_from_csv(csv_path=csv_path, is_calibrated=is_calibrated)
    return convert_list_to_np_array(original_list=data, num_col=NUM_COL)


def create_pressure_heatmap(data: np.array) -> str:
    now = datetime.datetime.now().strftime("%Y_%m_%d_%H_%M_%S")
    file_name = f"pressure_data/heatmap/{now}.png"
    plot_heatmap(data=data, num_col=NUM_COL, num_row=NUM_ROW, save_path=file_name)

    return file_name


if __name__ == "__main__":
    csv_path = "pressure_data/testC.csv"
    data = get_final_pressure_data(csv_path=csv_path)
    create_pressure_heatmap(data=data)

    # data = get_diff_between_two_data(first_csv_path=csv_path, second_csv_path=csv_path)
    # create_pressure_heatmap(data=data)
