from datetime import datetime
from typing import List

import matplotlib.pyplot as plt

from core.breathing_tracker import get_breathing_from_csv
from utils import logger


def get_average_per_hour(minute_list: List[int]) -> List[float]:
    temp = 0
    res = list()
    for i, num in enumerate(minute_list):
        if i % 60 == 0:
            res.append(temp / 60)
            temp = 0
        temp += num

    return res[1:]


def create_breathing_trend(csv_path: str) -> str:
    now = datetime.now().strftime("%Y_%m_%d_%H_%M_%S")
    save_path = f"breathing_data/breathing_graph/breathing_trend/{now}.png"
    breathing_data = get_breathing_from_csv(csv_path=csv_path)
    averaged_result = get_average_per_hour(minute_list=breathing_data)
    plt.plot(averaged_result)
    plt.savefig(save_path)
    plt.clf()
    plt.cla()
    plt.close()
    logger.info(f"Successfully created breathing trend: {save_path}")
    return save_path


if __name__ == "__main__":
    create_breathing_trend(csv_path="pressure_data/03-11-22.csv")
