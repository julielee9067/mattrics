import csv
import statistics
from typing import Any, List

import numpy as np

from core.constants import TOTAL_NUM_NODES
from utils import logger


def subtract(a, b) -> Any:
    logger.info(f"A: {a}")
    logger.info(f"B: {b}")
    return np.subtract(a, b)


def get_data_from_csv(csv_path: str) -> List[List[str]]:
    with open(csv_path, newline="") as f:
        reader = csv.reader(f)
        total_list = [row for row in reader]

    return total_list


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
            new_r = [int(point) for point in row[1:]]
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
    vout = adc * 0.0008056641
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
