from typing import Callable

from .exceptions import NoSolution


def bisection_eq_solver(func: Callable[[float], float], a: float, b: float, tolerance: float) -> float:
    """ Use the bisection method to find the roots of a function

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
    while (b-a)/2.0 > tolerance:
        print(f"Iteration number {iter}")
        iter += 1
        c = (a+b)/2.0 # Midway point.
        f_c = func(c)

        approximation = c
        if f_c == 0:
            break
        
        # If f(a) and f(c) differ in sign then it means we must chose a "b" closer to a.
        # Else, let's get "a" closer to b.
        if func(a) * f_c < 0: # There is a solution in [a,c].
            b = c
        else: # f(c) * f(b) < 0.
            a = c

    print(f"final approximation: {approximation}")
    return approximation

if __name__=="__main__":
    func = lambda x: x**3 + x - 1
    bisection_eq_solver(func, 0, 1, 0.0005)
