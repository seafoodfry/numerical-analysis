import numpy as np
import numpy.typing as npt
from typing import Any, Tuple

from .exceptions import PivotIsZero


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
