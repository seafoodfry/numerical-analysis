"""
Polynomial evaluation acording to Horner's method

Implementation based on Numerical ANalysis 2nd ed by Timoty Sauer, page 3.
"""
import numpy as np
from typing import List


def horner_simple_polynomial_evaluation(c: List[float], x: float, b: List[float] = None) -> float:
    """ Use the horner method to evaluate a polynomial of degree d
    at position x.

    Keep in mind that a properly factorized polynomial will look as follows:

    y = c1 + x*( c2 + x*( c3 + x*(...) ) )

    And the coefficients will look like c = [c1, c2, c3, ...]

    INPUT
    -----
    c: list of coefficients of a factorized polynomial
    x: point at which to evaluate polynomial
    b: list of base points

    OUTPUT
    ------
    float: result of the evaluation of the polynomial at x
    """
    if b is None:
        b = np.zeros([len(c)])

    # Degree of the polynomial.
    d = len(c) - 1

    # The way to go is to begin multiplying from inside and to move outwards.
    # So we should begin with the latest coefficient.
    y = c[d]  # Last coefficient.
    print(f"first coefficient: {y}")

    # We only look at the range "d-1" because we have already stored the last coefficient.
    # We stop at "-1" because range() is no inclusive.
    for i in range(d-1, -1, -1):
        print(f"{c[i]} + x*{y}")
        y = (y * (x-b[i])) + c[i]

    return y


if __name__ == "__main__":
    horner_simple_polynomial_evaluation(4, [-1, 5, -3, 3, 2], 0.5)  # y = 1.25
