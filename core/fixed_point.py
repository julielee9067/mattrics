import scipy as sc
import scipy.linalg


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
    A = sc.zeros((N + M - 1, N + M - 1))
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
    B = sc.zeros((N + M - 1, N * M))
    bcol = 0
    for jj in range(0, M):
        # To obtain Req between row node 0 and column node jj
        # the source intensity goes from jj to 0
        # Since row node 0 is not present only a
        # 1 in the vector is present
        b = sc.zeros(N + M - 1)
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
            b = sc.zeros(N + M - 1)
            b[ii - 1] = -1
            b[N - 1 + jj] = 1
            B[:, bcol] = b[:]
            bcol = bcol + 1
    # import ipdb; ipdb.set_trace()
    X = sc.linalg.solve(A, B)  # X is the voltage solution for each case
    sol2 = sc.zeros((N, M))
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
        residual = sc.fabs(ceq0 - cmeq).max()
        # print('residual', residual)
        if residual < ftol:
            # print(nn)
            break
        c1 = c0 - beta * (ceq0 - cmeq)
        if bounds:
            c1[c1 < 0] = 0.0
        c0 = c1
    # print(residual)
    return c1


###
def test_solutions():
    # Random conductance matrix. Minimum value = 2e-5, max = 2e-2
    cmin = 2e-5
    cmax = 2e-2
    cm = cmin + (cmax - cmin) * sc.rand(16, 16)
    # Get equivalent resistance matrix (the "measured" one)
    cmeq = get_ceq(cm)
    # All the algorithms should find cm as solution
    # Newton-krylov
    cm2 = fixed_point_solution(cmeq, beta=0.1, NITER=1000, ftol=1e-12, bounds=True)
    print("Fixed point: Mean abs relative error ", abs((cm2 - cm) / cm).mean())


###


if __name__ == "__main__":
    test_solutions()
