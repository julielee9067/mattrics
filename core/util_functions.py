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
    return np.subtract(a, b) * -1


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


def write_1d_list_to_csv(csv_path: str, data: List):
    new_data = [float(item) for item in data]

    with open(csv_path, "a", encoding="UTF8", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(new_data)
        writer.writerows([])

    logger.info(f"Successfully written: {csv_path}")


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
            new_r = [int(point) for point in row[1:] if point != ""]
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


def get_vout_from_adc(adc: np.array) -> np.array:
    vout = adc * VOUT_CONSTANT * 1000
    logger.info(f"vout: {vout}")
    return np.round(vout, 2)


def get_resistance_from_vout(vout: np.array) -> np.array:
    resistance = (3.3 * 470 / vout) - 470 - 2
    logger.info(f"resistance: {resistance}")
    return resistance


def get_conductance_from_r(resistance: np.array) -> np.array:
    conductance = np.power(resistance, -1)
    logger.info(f"conductance: {conductance}")
    return conductance


def get_conductance_from_adc(data: np.array) -> np.array:
    vout = get_vout_from_adc(adc=data)
    resistance = get_resistance_from_vout(vout=vout)
    conductance = get_conductance_from_r(resistance=resistance)
    return conductance


def get_fit_curve_coefficients(vout_data: np.array):
    return np.polyfit(x=WEIGHTS, y=vout_data, deg=FIT_CURVE_DEGREE)


def get_fitted_value(row_num: int, col_num: int, value: Any):
    coeff = FIT_COEFFICIENTS[row_num][col_num]
    return np.polyval(p=coeff, x=value)


# TODO: FIX THIS
def apply_fit_curve(data: np.array) -> np.array:
    get_fit_vec = np.vectorize(get_fit_curve_coefficients)
    return get_fit_vec(data)


def concat_data_at_each_node(total_data) -> np.array:
    start = total_data[0]
    for data in total_data[1:]:
        start = np.dstack((start, data))

    return start


def create_2d_array_from_1d(original_array: List, num_col: int) -> np.array:
    return np.reshape(original_array, (-1, num_col))
