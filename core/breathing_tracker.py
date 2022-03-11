import csv
import re
from datetime import datetime
from pathlib import Path
from statistics import mean
from typing import List, Tuple

import matplotlib.pyplot as plt
import numpy as np
from scipy.fft import irfft, rfft
from scipy.signal import argrelextrema

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


def get_local_minima_and_maxima_indexes(
    data, threshold=0
) -> Tuple[np.ndarray, np.ndarray]:
    # indexes = find_peaks(data, threshold=threshold, distance=3)[0]
    maxima_index = argrelextrema(data, np.greater)[0]
    minima_index = argrelextrema(data, np.less)[0]
    logger.info(f"Maxima index: {maxima_index}, minima index: {minima_index}")
    return minima_index, maxima_index


def apply_fft(data: List[int], fft_threshold=350) -> np.ndarray:
    yf = rfft(data)
    yf_abs = np.abs(yf)
    indices = yf_abs > fft_threshold  # filter out those value under threshold
    yf_clean = indices * yf  # noise frequency will be set to 0

    return irfft(yf_clean)


def get_time_intervals(minima_index: np.ndarray, maxima_index: np.ndarray):
    minima_interval = []
    maxima_interval = []
    for i in range(0, len(minima_index)):
        minima_interval.append((minima_index[i] - minima_index[i - 1]) / 2)
        maxima_interval.append((maxima_index[i] - maxima_index[i - 1]) / 2)

    return minima_interval, maxima_interval


def plot_respiratory_pattern(pressure_list: List[int], save_path: Path) -> None:
    peak_threshold = 0
    clean_data = apply_fft(data=pressure_list, fft_threshold=peak_threshold)
    t = np.arange(start=0, stop=int(len(pressure_list) / 2), step=0.5)

    # get local minima and maxima
    minima_index, maxima_index = get_local_minima_and_maxima_indexes(data=clean_data)
    minima = clean_data[minima_index]
    maxima = clean_data[maxima_index]
    logger.info(f"Number of peaks: {len(maxima)} with threshold: {peak_threshold}")

    # get intervals
    minima_interval, maxima_interval = get_time_intervals(minima_index, maxima_index)
    logger.info(
        f"Minima interval: {minima_interval}, average: {round(mean(minima_interval), 2)}"
    )
    logger.info(
        f"Maxima interval: {maxima_interval}, average: {round(mean(maxima_interval), 2)}"
    )

    # plot graph
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.set_xlabel("Time (s)")
    ax.set_ylabel("Total Pressure")
    ax.plot(t, clean_data)
    ax.plot(minima_index / 2, minima, "x")
    ax.plot(maxima_index / 2, maxima, "x")

    fig.savefig(save_path)
    logger.info(f"Successfully created respiratory graph: {save_path}")


def create_breathing_pattern(patient_name: str) -> str:
    now = datetime.now().strftime("%Y_%m_%d_%H_%M_%S")
    file_name = f"breathing_data/{patient_name}/{patient_name}_{now}.png"
    result = []

    file_path = "pressure_data/03-11-22.csv"
    result = read_csv_file(file_path=file_path)

    pressure_list = get_total_pressure_list(data=result)
    plot_respiratory_pattern(
        pressure_list=pressure_list, save_path=Path(file_name).with_suffix(".png")
    )
    return file_name


if __name__ == "__main__":
    create_breathing_pattern(patient_name="adam_johnson")
