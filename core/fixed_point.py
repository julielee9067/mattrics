import csv
from datetime import datetime
from typing import List

import matplotlib.pyplot as plt
import numpy as np
import numpy.linalg
import seaborn

from core.constants import TOTAL_NUM_NODES
from core.util_functions import (
    get_conductance_from_adc,
    get_data_from_csv,
    get_node_average,
    write_to_csv,
)
from utils import logger

# TODO: ii1 is referred here as a ground, but we don't have one.

NUM_COL = 16
NUM_ROW = 57


def generate_node_equations(cm, ii1):
    """
    It generates the matrix of node equations taking row ii1 as ground
    Inputs:
        cm matrix of conductances between nodes (inverse of resistances)
        ii1 row whose voltage is considered as the reference (ground)
    Output:
        If NxM is the dimension of cm, the function returns a (N+M-1) x (N+M-1) matrix
        The order of the variables is first row voltages, then column voltages. ii1 is
        excluded since it is 0.
        It can be used to solve A V = b where b is a column vector of intensities
        coming from current sources.
    """
    N, M = cm.shape
    # N+M-1 nodes (ii1 is ground)
    # First N-1 are row wires, next M are column wires
    A = np.zeros((N + M - 1, N + M - 1))
    row = 0  # Refers to A

    # Equations of horizontal wires ...
    for ii in range(0, N):
        if ii == ii1:
            continue
        A[row, row] = cm[ii, :].sum()
        for column in range(N - 1, N - 1 + M):
            A[row, column] = -cm[ii, column - (N - 1)]
        row = row + 1

    # Equations of vertical wires
    for jj in range(0, M):
        column = 0
        for ii in range(0, N):
            if ii == ii1:
                continue
            A[row, column] = -cm[ii, jj]
            column = column + 1
        A[row, row] = cm[:, jj].sum()
        row = row + 1
    return A


##
def get_ceq(cm):
    """
    It generates the equivalent conductance matrix, that is the matrix
    of conductances between two nodes (one row and one column).
    Input:
        cm is the matrix of conductances obtained from the resistances that
        link directly a pair row column
    Output:
        It returns a matrix of the same dimension as cm. It is the matrix of
        equivalent conductances
    """
    A = generate_node_equations(cm, 0)  # Row 0 is ground
    N, M = cm.shape
    # B will contain all the right part of the equations
    # that will allow to get Req between nodes
    # B is a set of column vectors, each one N+M-1
    # The first N-1 refer to row nodes (row 0 is ground so excluded)
    # and the next M elements to column nodes
    # There are N*M columns in B, corresponding to N*M pairs
    # The solution can be obtained in a single call to linalg.solve !!!
    B = np.zeros((N + M - 1, N * M))
    bcol = 0
    for jj in range(0, M):
        # To obtain Req between row node 0 and column node jj
        # the source intensity goes from jj to 0
        # Since row node 0 is not present only a
        # 1 in the vector is present
        b = np.zeros(N + M - 1)
        b[N - 1 + jj] = 1.0
        B[:, bcol] = b[:]
        bcol = bcol + 1
    # Now the rest of pairs
    for ii in range(1, N):
        for jj in range(0, M):
            # Req between row node ii and column node jj
            # The source goes from node column jj (+1)
            # and returns from node row ii (-1)
            # Note that row nodes are placed first in b vector
            b = np.zeros(N + M - 1)
            b[ii - 1] = -1
            b[N - 1 + jj] = 1
            B[:, bcol] = b[:]
            bcol = bcol + 1
    try:
        # X = np.linalg.lstsq(A, B, rcond=None)[0]
        X = np.linalg.solve(A, B)  # X is the voltage solution for each case
        sol2 = np.zeros((N, M))
        for jj in range(0, M):
            # Req between row node 0 and column node jj
            sol2[0, jj] = X[N - 1 + jj, jj]
        bcol = M
        for ii in range(1, N):
            for jj in range(0, M):
                # Req between column node jj and row node ii
                sol2[ii, jj] = X[N - 1 + jj, bcol] - X[ii - 1, bcol]
                bcol = bcol + 1
        return 1.0 / sol2  # return equivalent conductance
    except Exception as e:
        logger.error(e)
        logger.error(f"A: {A}\nB: {B}")
        pass


###
def fixed_point_solution(cmeq, cm0=None, beta=0.05, NITER=25, bounds=False, ftol=6e-6):
    """
    It finds the direct conductance of the link between each pair of
    nodes (row-column) given the equivalent conductance of each pair.

    It uses a fixed point approach.

    Inputs:
        cmeq: equivalenteconductance matrix (measured values)
        cm0: initial guess for the solution. If None, cmeq is taken.
        beta: update parameter for iterations
        NITER: maximum number of iterations
        bounds: whether to apply bounds (force conductance to be positive)
        ftol: kind of tolerance similar toscipy.optimize.newton_krylov: f_tol : float, optional. Absolute tolerance (in max-norm) for the residual. If omitted, default is 6e-6.
    Output:
        It returns the value of the cell conductances (same dimension as cmeq)

    """
    if cm0 is None:
        c0 = cmeq
    else:
        c0 = cm0
    for nn in range(NITER):
        ceq0 = get_ceq(c0)
        # Termination condition
        # Residual of the initial nonlinear problem
        if ceq0 is not None:
            residual = np.fabs(ceq0 - cmeq).max()
            # print('residual', residual)
            if residual < ftol:
                # print(nn)
                break
            c1 = c0 - beta * (ceq0 - cmeq)
            if bounds:
                c1[c1 < 0] = 0.0
            c0 = c1
        else:
            continue
    # print(residual)
    return c1


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


def get_first_row_data(csv_path: str) -> List[int]:
    with open(csv_path, newline="") as f:
        reader = csv.reader(f)
        row = next(reader)

    result = [int(point) for point in row[1:]]
    return result


def filter_garbage(total_list: List) -> List:
    res = list()

    for i, row in enumerate(total_list):
        try:
            new_r = [float(point) for point in row]
        except ValueError as e:
            logger.warning(f"Ignoring row: {e}")
            continue
        res.append(new_r)
    logger.info(f"{len(res)} row found")

    return res


def filter_full_mat_garbage(total_list: List) -> List:
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


def convert_list_to_np_array(original_list: List, num_col: int) -> np.array:
    matrix = [
        original_list[i : i + num_col] for i in range(0, len(original_list), num_col)
    ]

    new = [matrix[i][:num_col] for i in range(num_col)]

    np_array = np.asarray(new)
    return np_array


def convert_list_to_np_array_full_mat(original_list: List, num_row: int) -> np.array:
    matrix = [
        original_list[i : i + num_row] for i in range(0, len(original_list), num_row)
    ]
    np_array = np.asarray(matrix)
    np_array = np.transpose(np_array)
    return np_array


def solutions(original_data: np.array) -> np.array:
    cm = original_data
    cmeq = get_ceq(cm)
    cm2 = fixed_point_solution(cmeq, beta=0.1, NITER=1000, ftol=1e-12, bounds=True)
    print("Fixed point: Mean abs relative error ", abs((cm2 - cm) / cm).mean())

    return (cm2 - cm) / cm
    # return cm2


def get_original_data(csv_path: str) -> np.array:
    result = get_data_from_csv(csv_path=csv_path)
    data = filter_garbage(total_list=result)
    avg_data = get_node_average(data=np.array(data))
    return convert_list_to_np_array(original_list=avg_data, num_col=NUM_COL)


def get_full_mat_data(csv_path: str) -> np.array:
    result = get_data_from_csv(csv_path=csv_path)
    data = filter_full_mat_garbage(total_list=result)
    avg_data = get_node_average(data=np.array(data))
    full_mat_data = convert_list_to_np_array_full_mat(
        original_list=avg_data, num_row=NUM_ROW
    )
    squared_data = get_square_matrices(data=full_mat_data)
    return squared_data


def plot_heatmap(
    data: np.array, num_col: int, num_row: int, save_path: str, title: str
):
    plt.style.use("seaborn")
    plt.figure(figsize=(num_col, num_row))
    plt.title(f"{title} Pressure Heat Map")
    seaborn.heatmap(data, linewidth=0.30, annot=False, cmap="Blues")
    plt.savefig(save_path)
    plt.clf()
    plt.cla()
    plt.close()
    logger.info(f"Successfully created heatmap: {save_path}")


if __name__ == "__main__":
    squared_data = get_full_mat_data(csv_path="pressure_data/feet_test.csv")
    # original_data = get_original_data(csv_path="fixed_point_test_data/new2.csv")
    f = open("fixed_point_test_data/test.csv", "w+")
    for data in squared_data:
        conductance = get_conductance_from_adc(data=data)
        data = solutions(original_data=data)
        write_to_csv(csv_path="fixed_point_test_data/test.csv", data=data)

    combined_data = get_data_from_csv(csv_path="fixed_point_test_data/test.csv")
    parsed_data = parse_combined_squares(data=combined_data)

    new_data = filter_garbage(total_list=parsed_data)

    now = datetime.now().strftime("%Y_%m_%d_%H_%M_%S")
    original_file_name = f"fixed_point_test_data/result/original/{now}.png"
    after_file_name = f"fixed_point_test_data/result/after/{now}.png"

    plot_heatmap(
        data=new_data,
        num_col=32,
        num_row=56,
        save_path=after_file_name,
        title="after conductance",
    )
