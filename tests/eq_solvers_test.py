import pytest
from math import cos
from numa.equation_solvers import bisection_eq_solver, secant_eq_solver
from numa.exceptions import NoSolution


def test_bisection_one():
    func = lambda x: x**3 + x - 1
    approx = bisection_eq_solver(func, 0, 1, 0.0005)
    assert 0.6816 < approx < 0.6836

def test_bisection_two():
    func = lambda x: cos(x) - x
    approx = bisection_eq_solver(func, 0, 1, 0.0000005)
    assert 0.739084 <= approx <= 0.739086

def test_bisection_exception():
    func = lambda x: x**3 + x - 1
    with pytest.raises(NoSolution) as e_info:
        approx = bisection_eq_solver(func, 0, 0.5, 0.0005)

def test_secant_one():
    func = lambda x: x**3 + x - 1
    approx = secant_eq_solver(func, 0, 1, 9)
    assert 0.68232780382801 <= approx <= 0.68232780382803
