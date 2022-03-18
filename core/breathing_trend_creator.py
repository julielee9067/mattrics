import statistics
from datetime import datetime
from typing import List, Tuple

import matplotlib.pyplot as plt

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


def create_breathing_trend(csv_path: str) -> Tuple[str, float]:
    now = datetime.now().strftime("%Y_%m_%d_%H_%M_%S")
    save_path = f"breathing_data/breathing_graph/breathing_trend/{now}.png"
    # pressure_list = get_breathing_from_csv(csv_path=csv_path)
    # breathing_data = get_breathing_data(pressure_list=pressure_list)
    # averaged_result = get_average_per_hour(minute_list=pressure_list)
    average_result = [18, 19, 18, 20, 18, 21, 19, 20, 21]
    x = [
        "22:00",
        "23:00",
        "00:00",
        "01:00",
        "02:00",
        "03:00",
        "04:00",
        "05:00",
        "06:00",
    ]
    ax = plt.gca()
    ax.set_ylim([0, 40])
    plt.title(f"Overnight Breathing Trend")
    plt.xlabel("Time")
    plt.ylabel("Average Breathing Rate [breaths/minute]")
    default_x_ticks = range(len(x))
    plt.plot(default_x_ticks, average_result)
    plt.xticks(default_x_ticks, x)
    plt.savefig(save_path)
    plt.clf()
    plt.cla()
    plt.close()
    logger.info(f"Successfully created breathing trend: {save_path}")
    return save_path, round(statistics.mean(average_result), 2)


if __name__ == "__main__":
    create_breathing_trend(csv_path="pressure_data/03-11-22.csv")
