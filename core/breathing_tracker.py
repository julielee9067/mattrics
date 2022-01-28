import re
from typing import List

import matplotlib.pyplot as plt


def read_file(file_path: str) -> List[List[int]]:
    result = []
    with open(file_path, "r+") as f:
        contents = f.readlines()
        for content in contents:
            content = re.sub(r"\t", " ", content).split()
            int_list = list(map(int, content))
            result.append(int_list)

    return result


def get_total_pressure_list(data: List[List[int]]) -> List[int]:
    return [sum(stream) for stream in data]


def plot_respiratory_pattern(pressure_list: List[int]) -> None:
    x_data = range(len(pressure_list))
    y_data = pressure_list
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.plot(x_data, y_data)
    ax.set_xlabel("Time (s)")
    ax.set_ylabel("Total Pressure")

    plt.show()


if __name__ == "__main__":
    result = read_file("pressure_data/Matrix_Air_B2.txt")
    pressure_list = get_total_pressure_list(data=result)
    plot_respiratory_pattern(pressure_list=pressure_list)
