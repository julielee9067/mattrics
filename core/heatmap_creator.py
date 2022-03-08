import csv
import datetime
from typing import List

import matplotlib.pyplot as plt
import numpy as np
import seaborn

from utils import logger


def get_first_row_data(csv_path: str) -> List:
    with open(csv_path, newline="") as f:
        reader = csv.reader(f)
        row = next(reader)

    result = [int(point) for point in row[1:]]
    return result


def convert_list_to_np_array(original_list: List, num_col: int) -> np.array:
    matrix = [
        original_list[i : i + num_col] for i in range(0, len(original_list), num_col)
    ]
    np_array = np.asarray(matrix)
    return np_array


def plot_heatmap(data: np.array, num_col: int, num_row: int, save_path: str):
    plt.style.use("seaborn")
    plt.figure(figsize=(num_col, num_row))
    plt.title("Pressure Heat Map")
    seaborn.heatmap(data, linewidth=0.30, annot=False, cmap="Blues")
    plt.savefig(save_path)
    logger.info(f"Successfully created heatmap: {save_path}")


def create_pressure_heatmap(patient_name: str) -> str:
    row = []  # TODO: GET IT FROM DB LATER ON
    data = convert_list_to_np_array(original_list=row, num_col=32)  # TODO: FIX NUM_COL?
    now = datetime.datetime.now().strftime("%Y/%m/%d/%H:%M:%S")
    file_name = f"pressure_data/{patient_name}/{patient_name}_{now}.png"
    plot_heatmap(data=data, num_col=6, num_row=6, save_path=file_name)
    return file_name


if __name__ == "__main__":
    row = get_first_row_data("pressure_data/Jan13_drift_test.csv")
    data = convert_list_to_np_array(original_list=row, num_col=6)
    plot_heatmap(data=data, num_col=6, num_row=6, save_path="pressure_data/1.png")
