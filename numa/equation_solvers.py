"""
Implementation based on Numerical ANalysis 2nd ed by Timoty Sauer
"""

from typing import Callable

from .exceptions import NoSolution


def bisection_eq_solver(
    func: Callable[[float], float], a: float, b: float, tolerance: float
) -> float:
    """Use the bisection method to find the roots of a function

    INPUT
    -----
    f: function to solve
    a: start of interval
    b: end of interval
    tolerance:

    OUTPUT
    ------
    Approximate solution
    """
    if func(a) * func(b) >= 0:
        raise NoSolution(f"There is no solution in [{a}, {b}]")

    iter = 1
    approximation = 0
    while (b - a) / 2.0 > tolerance:
        print(f"Iteration number {iter}")
        iter += 1
        c = (a + b) / 2.0  # Midway point.
        f_c = func(c)

        approximation = c
        if f_c == 0:
            break

        # If f(a) and f(c) differ in sign then it means we must chose a "b" closer to a.
        # Else, let's get "a" closer to b.
        if func(a) * f_c < 0:  # There is a solution in [a,c].
            b = c
        else:  # f(c) * f(b) < 0.
            a = c

    print(f"final approximation: {approximation}")
    return approximation


def secant_eq_solver(
    func: Callable[[float], float], x0: float, x1: float, max_iterations: int
) -> float:
    """Secant method for finding roots

    x[i+1] = ( f(x[i]) * (x[i] - x[i-1]) ) / ( f(x[i]) - f([x-1]) )

    INPUT
    -----
    func: function to solve
    x0: initial guess
    x1: second initial guess
    max_iterations: maximum_number of iterations

    OUTPUT
    ------
    Approximate solution
    """

    def iteration(func: Callable[[float], float], x0: float, x1: float) -> float:
        f_x1 = func(x1)
        f_x0 = func(x0)
        diff_x = x1 - x0
        return x1 - ((f_x1 * diff_x) / (f_x1 - f_x0))

    approx = None
    iter_num = 0
    while iter_num < max_iterations:
        try:
            approx = iteration(func, x0, x1)
        except ZeroDivisionError as e:
            print(e)
            return approx
        print(f"x[i+1]: {approx}. x[i]: {x1}. x[i-1]: {x0}")
        x0 = x1
        x1 = approx
        iter_num += 1
    return approx


if __name__ == "__main__":

    def func(x):
        return x**3 + x - 1

    solution = bisection_eq_solver(func, 0, 1, 0.0005)
    print(f"bisection solution: {solution}")
    solution = secant_eq_solver(func, 0, 1, 10)
    print(f"secant solution: {solution}")
