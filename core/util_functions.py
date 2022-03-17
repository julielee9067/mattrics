import csv
import statistics
from typing import Any, List

import numpy as np

from core.constants import (
    FIT_COEFFICIENTS,
    FIT_CURVE_DEGREE,
    TOTAL_NUM_NODES,
    VOUT_CONSTANT,
    WEIGHTS,
)
from utils import logger


def subtract(a, b) -> Any:
    logger.info(f"A: {a}")
    logger.info(f"B: {b}")
    logger.info(f"A-B: {np.subtract(a, b)}")
    return np.subtract(a, b)


def get_data_from_csv(csv_path: str) -> List[List[str]]:
    with open(csv_path, newline="") as f:
        reader = csv.reader(f)
        total_list = [row for row in reader]

    return total_list


def write_to_csv(csv_path: str, data: List[List]):
    new_data = []
    for row in data:
        new_data.append([float(node) for node in row])

    with open(csv_path, "a", encoding="UTF8", newline="") as f:
        writer = csv.writer(f)
        writer.writerows(new_data)
        writer.writerows([])

    logger.info(f"Successfully written: {csv_path}")


def get_square_matrices(data: np.array) -> List:
    res = list()
    for i in range(0, 32, 16):
        for j in range(0, 48, 16):
            res.append(data[j : j + 16, i : i + 16])

    for i in range(0, 32, 8):
        for j in range(48, 56, 8):
            res.append(data[j : j + 8, i : i + 8])

    return res


def parse_combined_squares(data: np.array) -> np.array:
    stack_list = []
    for i in range(0, 96, 32):
        start = np.asarray(data[i : i + 16])
        end = np.asarray(data[i + 16 : i + 32])
        stack = np.hstack((start, end))
        stack_list.append(stack)

    start = np.asarray(data[96:104])
    for i in range(104, 128, 8):
        end = np.asarray(data[i : i + 8])
        start = np.hstack((start, end))

    stack_list.append(start)
    start = stack_list[0]
    for stack in stack_list[1:]:
        start = np.vstack((start, stack))

    return start


def get_node_average(data: np.ndarray) -> List:
    return np.average(data, axis=0)


def get_node_sum(data: np.ndarray) -> List:
    return data.sum(axis=0)


def get_total_sum(data: List[List[int]]) -> List[int]:
    return [sum(stream) for stream in data]


def get_total_average(data: List[List[int]]) -> List[int]:
    return [statistics.mean(stream) for stream in data]


def filter_garbage(total_list: List) -> List:
    res = list()

    for i, row in enumerate(total_list):
        try:
            new_r = [int(point) for point in row]
        except ValueError as e:
            logger.warning(f"Ignoring row: {e}")
            continue

        if len(new_r) == TOTAL_NUM_NODES:
            res.append(new_r)

    logger.info(f"{len(res)} row found")

    return res


def convert_list_to_np_array(original_list: List, num_row: int) -> np.array:
    matrix = [
        original_list[i : i + num_row] for i in range(0, len(original_list), num_row)
    ]
    np_array = np.asarray(matrix)
    np_array = np.transpose(np_array)
    return np_array


def get_vout(adc: np.array) -> np.array:
    vout = adc * VOUT_CONSTANT * 1000
    logger.info(f"vout: {vout}")
    return vout


def get_resistance(vout: np.array) -> np.array:
    resistance = (3.3 * 470 / vout) - 470 - 2
    logger.info(f"resistance: {resistance}")
    return resistance


def get_conductance(resistance: np.array) -> np.array:
    conductance = np.power(resistance, -1)
    logger.info(f"conductance: {conductance}")
    return conductance


def apply_conductance(data: np.array) -> np.array:
    vout = get_vout(adc=data)
    resistance = get_resistance(vout=vout)
    conductance = get_conductance(resistance=resistance)

    return conductance


def get_fit_curve(vout_data: np.array):
    return np.polyfit(x=WEIGHTS, y=vout_data, deg=FIT_CURVE_DEGREE)


def get_adjusted_value(row_num: int, col_num: int, value: Any[int, float]):
    coeff = FIT_COEFFICIENTS[row_num][col_num]
    return np.polyval(p=coeff, x=value)
