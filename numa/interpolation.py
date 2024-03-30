"""
Interpolation methods

Most algorithms come from Numerical Analysis 2nd Edition by Timothy Sauer, page 148.

In order to properly test this implementation we had to add the "base points" feauture
to horner.py.
"""

import numpy as np


def newton_divided_difference(x: list[float], y: list[float]) -> list[float]:
    """Newton Divided Differences Interpolation Method

    Computes the coefficients of the interpolating polynomial.

    INPUT
    -----
    x: list of x coordinates
    y: list of y coordinates

    OUTPUT
    ------
    c: list of coefficients of the interpolating polynomial in nested form.

    The implementation follows the construction of the Newton triangle where

    x_1 | f[x_1]
        |         f[x_1 x_2]
    x_2 | f[x_2]              f[x_1 x_2 x_3]
        |         f[x_2 x_3]
    x_3 | f[x_3]

    P(x) = f[x_1] + f[x_1 x_2] (x-x_1) + f[x_1 x_2 x_3] (x-x_1)(x-x_2) + ...

    x_1 | f(x_1)
    x_2 | f(x_2)
      . |    .
      . |    .
      . |    .
    x_n | f(x_n)

    The divided differences are defined as follows:

    f[x_k] = f(x_k)

    f[x_k x_{k+1}] = (f[x_{k+1}] - f[x_k]) / (x_{k+1} - x_k)

    f[x_k x_{k+1} x_{k+2}] = (f[x_{k+1} x_{k+2}] - f[x_k x_{k+1}]) / (x_{k+2} - x_k)

    f[x_k x_{k+1} x_{k+2} x_{k+3}] =
        (f[x_{k+1} x_{k+2} x_{k+3}] - f[x_k x_{k+1} x_{k+2}]) / (x_{k+3} - x_k)
    """
    assert len(x) == len(y)

    n = len(x)
    v = np.zeros([n, n])

    # Fill in the y column of the Newton triangle.
    v[:, 0] = y

    for i in range(1, n):
        for j in range(0, n - i):
            x_left = x[j + i]
            x_right = x[j]

            f_left = v[j + 1, i - 1]
            f_right = v[j, i - 1]

            v[j, i] = (f_left - f_right) / (x_left - x_right)

    return v[0, :]
