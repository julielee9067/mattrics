import csv
from datetime import datetime
from typing import List

import matplotlib.pyplot as plt

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
            result.append(list(map(int, row)))

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


def create_breathing_trend(patient_name: str) -> str:
    now = datetime.now().strftime("%Y_%m_%d_%H_%M_%S")
    save_path = (
        f"breathing_data/{patient_name}/breathing_trend/{patient_name}_{now}.png"
    )
    file_path = "breathing_data/zero_breathing_trend.csv"
    result = read_csv_file(file_path=file_path)
    averaged_result = average_per_hour(minute_list=result)
    plt.plot(averaged_result)
    plt.savefig(save_path)
    logger.info(f"Successfully created heatmap: {save_path}")
    return save_path


if __name__ == "__main__":
    # create_mock_data()
    create_breathing_trend(patient_name="adam_johnson")
