import csv
import re
import statistics
from pathlib import Path
from typing import List

import matplotlib.pyplot as plt

from utils import logger


def read_csv_file(file_path: str) -> List[List[int]]:
    result = []

    with open(file_path, newline="") as f:
        reader = csv.reader(f)
        for row in reader:
            result.append(list(map(int, row[1:])))

    return result


def read_txt_file(file_path: str) -> List[List[int]]:
    result = []
    with open(file_path, "r+") as f:
        contents = f.readlines()

        for content in contents:
            content = re.sub(r"\t", " ", content).split()
            result.append(list(map(int, content)))

    return result


def get_total_pressure_list(data: List[List[int]]) -> List[int]:
    # Get average
    # return [statistics.mean(stream) for stream in data]

    # Get total
    return [sum(stream) for stream in data]


def plot_respiratory_pattern(pressure_list: List[int], save_path: Path) -> None:
    x_data = [
        value / 2 for value in range(len(pressure_list))
    ]  # Considering we are sampling twice per second
    y_data = pressure_list
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.plot(x_data, y_data)
    ax.set_xlabel("Time (s)")
    ax.set_ylabel("Total Pressure")

    plt.savefig(save_path)
    logger.info(f"Successfully created respiratory graph: {save_path}")


if __name__ == "__main__":
    file_path = "pressure_data/Feb1_breathing.csv"
    # result = read_txt_file("pressure_data/Matrix_Air_B2.txt")
    result = read_csv_file(file_path=file_path)
    pressure_list = get_total_pressure_list(data=result)
    plot_respiratory_pattern(
        pressure_list=pressure_list, save_path=Path(file_path).with_suffix(".png")
    )
