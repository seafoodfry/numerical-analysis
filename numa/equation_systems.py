import numpy as np
import numpy.typing as npt
from typing import Any, Tuple

from .exceptions import PivotIsZero, SingularMatrix


def simple_gaussian_elimination(a: npt.NDArray, b: npt.NDArray) -> npt.NDArray[Any]:
    a = a.astype(np.float64)
    b = b.astype(np.float64)

    machine_epsilon = np.finfo(float).eps
    nrows, ncols = a.shape
    n = nrows
    for j in range(0, n-1, 1): # We only need to elminate n-1 columns.
        if abs(a[j,j]) < machine_epsilon:
            raise PivotIsZero(a[j])

        # We need to look at the next row (we always subtract the row above).
        # We want to look below the diagonal.
        for i in range(j+1, n, 1):
            mult = a[i,j] / a[j,j]

            # Next, we want to iterate through the row, subtracting the row product of the multiplier
            # and the row above.
            # We begin at j+1 becaue the first entry we can assume that it will be 0 (we want to
            # look to the right of the diagonal).
            b[i] = b[i] - (mult*b[j])
            for k in range(j+1, n, 1):
                a[i,k] = a[i,k] - (mult * a[j,k])
    return a, b

def lu_factorization(a: npt.NDArray) -> Tuple[npt.NDArray[Any], npt.NDArray[Any]]:
    a = a.astype(np.float64)
    machine_epsilon = np.finfo(float).eps

    nrows, ncols = a.shape
    n = nrows
    L = np.zeros((n, n), np.float64)
    np.fill_diagonal(L, 1)

    for j in range(0, n-1, 1): # We only need to elminate n-1 columns.
        if abs(a[j,j]) < machine_epsilon:
            raise PivotIsZero(a[j])

        # We need to look at the next row (we always subtract the row above).
        # We want to look below the diagonal.
        for i in range(j+1, n, 1):
            mult = a[i,j] / a[j,j]
            L[i,j] = mult

            # Next, we want to iterate through the row, subtracting the row product of the multiplier
            # and the row above.
            for k in range(j, n, 1):
                a[i,k] = a[i,k] - (mult * a[j,k])
    return L, a

def pa_lu_factorization(a: npt.NDArray) -> Tuple[npt.NDArray[Any], npt.NDArray[Any], npt.NDArray[Any]]:
    a = a.astype(np.float64)
    machine_epsilon = np.finfo(float).eps

    nrows, ncols = a.shape
    n = nrows
    U = a.copy()
    L = np.zeros((n, n), np.float64)
    # np.fill_diagonal(L, 1) - we will fill the diagonal at the end as we may be potentially pivoting rows around.
    P = np.zeros((n, n), np.float64)
    np.fill_diagonal(P, 1)

    for j in range(0, n-1, 1): # We only need to elminate n-1 columns.
        # To get the pivot, find the row with the element that has the greatest absolute value
        # in the same column we are already looking at.
        print(f"--> looking for the max abs value of {U[j:, j]}")
        index_of_max_abs = np.absolute(U[j:, j]).argmax() # get the j-th column starting at the j-th entry.
        index_of_max_abs += j # Because when we look after the j-th entry of a column we are decreasing the value of the index by j.
        if j != index_of_max_abs and index_of_max_abs > j:
            print(f"current index {j}. New pivot index: {index_of_max_abs}")
            # The X[[i, j]] notation gets rows i and j and packges them into a [sub-]matrix.
            P[[j, index_of_max_abs]] = P[[index_of_max_abs, j]]
            L[[j, index_of_max_abs]] = L[[index_of_max_abs, j]]
            U[[j, index_of_max_abs]] = U[[index_of_max_abs, j]]
            print(f"current P:\n{P}")
            print(f"A after pivot:\n{U}")

        if abs(U[j,j]) < machine_epsilon:
            raise SingularMatrix()

        # We need to look at the next row (we always subtract the row above).
        # We want to look below the diagonal.
        for i in range(j+1, n, 1):
            mult = U[i,j] / U[j,j]
            L[i,j] = mult

            # Next, we want to iterate through the row, subtracting the row product of the multiplier
            # and the row above.
            for k in range(j, n, 1):
                U[i,k] = U[i,k] - (mult * U[j,k])
        print(f"new A:\n{U}")
        print(f"current L:\n{L}")

    # Fill in the diagonal of the lower triangular matrix. We didn't do it before because of the possibility
    # that we may have to swap rows.
    np.fill_diagonal(L, 1)
    return P, L, U


def back_substitution(a: npt.NDArray, b: npt.NDArray) -> npt.ArrayLike:
    a = a.astype(np.float64)
    b = b.astype(np.float64)

    nrows, ncols = a.shape
    n = nrows
    x = np.zeros(n)
    # We will count backwards and stop at 0.
    for i in range(n-1, -1, -1):
        # Then, proceed to subtract terms from the left of the equation.
        # We'll be left with a[i,0]*x = b[i], which we will divide afterwards.
        for j in range(i+1, n, 1):
            b[i] = b[i] - (a[i,j] * x[j])
        x[i] = b[i]/a[i,i]
    return x.astype(np.float64)

    

if __name__=="__main__":
    print("Gaussian elimination...")
    a = np.array([[1, 1], [3, -4]])
    b = np.array([[3], [2]])
    a, b = simple_gaussian_elimination(a, b)
    print(a)
    print(b)
    solution = back_substitution(a, b)
    print(f"solution: {solution}")

    print("Gaussian elimination...")
    a = np.array([[1, 2, -1], [2, 1, -2], [-3, 1, 1]])
    b = np.array([[3], [3], [-6]])
    a, b = simple_gaussian_elimination(a, b)
    print(a)
    print(b)
    solution = back_substitution(a, b)
    print(f"solution: {solution}")

    print("LU factorization...")
    a = np.array([[1, 1], [3, -4]])
    L, U = lu_factorization(a)
    print(L)
    print(U)

    print("LU factorization...")
    a = np.array([[1, 2, -1], [2, 1, -2], [-3, 1, 1]])
    L, U = lu_factorization(a)
    print(L)
    print(U)

    print("PA = GLU factorization...")
    a = np.array([[2, 1, 5], [4, 4, -4], [1, 3, 1]])
    P, L, U = pa_lu_factorization(a)
    print(P)
    print(L)
    print(U)
