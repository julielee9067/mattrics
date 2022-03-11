import csv
from datetime import datetime
from typing import List

import matplotlib.pyplot as plt

from core.breathing_tracker import get_breathing_tracker_data
from utils import logger


def create_mock_data():
    original = "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "
    a = ""
    for i in range(47):
        a += original

    a += "0, 0, 0, 0, 0, 0, 0, 0, 0, 0"
    print(a)


def read_csv_file(file_path: str) -> List[int]:
    result = []

    with open(file_path, newline="") as f:
        reader = csv.reader(f)
        for row in reader:
            result.append(list(map(int, row[1:])))

    return result[0]


def average_per_hour(minute_list: List[int]) -> List[float]:
    temp = 0
    res = list()
    for i, num in enumerate(minute_list):
        if i % 60 == 0:
            res.append(temp / 60)
            temp = 0
        temp += num

    return res[1:]


def create_breathing_trend(csv_file_name: str) -> str:
    now = datetime.now().strftime("%Y_%m_%d_%H_%M_%S")
    save_path = f"breathing_data/adam_johnson/breathing_trend/adam_johnson_{now}.png"
    breathing_data = get_breathing_tracker_data(csv_file_name=csv_file_name)
    averaged_result = average_per_hour(minute_list=breathing_data)
    plt.plot(averaged_result)
    plt.savefig(save_path)
    logger.info(f"Successfully created heatmap: {save_path}")
    return save_path


if __name__ == "__main__":
    # create_mock_data()
    create_breathing_trend(csv_file_name="pressure_data/03-11-22.csv")
