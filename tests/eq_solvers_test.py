import pytest
from numa.equation_solvers import bisection_eq_solver
from numa.exceptions import NoSolution


def test_bisection_one():
    func = lambda x: x**3 + x - 1
    approx = bisection_eq_solver(func, 0, 1, 0.0005)
    assert 0.6816 < approx < 0.6836

def test_bisection_exception():
    func = lambda x: x**3 + x - 1
    with pytest.raises(NoSolution) as e_info:
        approx = bisection_eq_solver(func, 0, 0.5, 0.0005)
