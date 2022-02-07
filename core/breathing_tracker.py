import csv
import re
import statistics
from pathlib import Path
from typing import List

import matplotlib.pyplot as plt
import numpy as np
from scipy import stats
from scipy.fft import irfft, rfft, rfftfreq
from scipy.signal import argrelextrema, find_peaks

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


def get_peaks(data, threshold=0) -> np.ndarray:
    return data[find_peaks(data, threshold=threshold)[0]]


def apply_fft(data: List[int], fft_threshold=350) -> np.ndarray:
    yf = rfft(data)
    yf_abs = np.abs(yf)
    indices = yf_abs > fft_threshold  # filter out those value under threshold
    yf_clean = indices * yf  # noise frequency will be set to 0

    return irfft(yf_clean)


def plot_respiratory_pattern(pressure_list: List[int], save_path: Path) -> None:
    clean_data = apply_fft(data=pressure_list, fft_threshold=370)
    t = np.arange(start=0, stop=int(len(pressure_list) / 2), step=0.5)

    # get peaks
    peak_threshold = 4
    peaks = get_peaks(data=clean_data, threshold=4)
    logger.info(f"Number of peaks: {len(peaks)} with threshold: {peak_threshold}")

    plt.plot(t, clean_data)

    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.plot(t, clean_data)
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
