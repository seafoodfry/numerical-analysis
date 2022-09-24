import pytest
import numpy as np
from numa.equation_systems import simple_gaussian_elimination, back_substitution, lu_factorization
from numa.exceptions import PivotIsZero


def test_simple_gaussian_one():
    a = np.array([[1., 1.], [3., -4.]])
    b = np.array([[3], [2]])
    a, b = simple_gaussian_elimination(a, b)

    sol_a = np.array([[1., 1.], [3., -7.]])
    sol_b = np.array([[3], [-7]])
    assert np.allclose(a, sol_a)
    assert np.allclose(b, sol_b)

def test_back_substitution_one():
    sol_a = np.array([[1., 1.], [3., -7.]])
    sol_b = np.array([[3], [-7]])
    c = back_substitution(sol_a, sol_b)

    solution = np.array([2., 1.])
    assert np.allclose(c, solution)

def test_simple_gaussian_two():
    a = np.array([[1, 2, -1], [2, 1, -2], [-3, 1, 1]])
    b = np.array([[3], [3], [-6]])
    a, b = simple_gaussian_elimination(a, b)

    sol_a = np.array([[1.,  2., -1.], [2., -3.,  0.], [-3.,  7., -2.]])
    sol_b = np.array([[3.], [-3.], [-4.]])
    assert np.allclose(a, sol_a)
    assert np.allclose(b, sol_b)

def test_back_substitution_two():
    sol_a = np.array([[1.,  2., -1.], [2., -3.,  0.], [-3.,  7., -2.]])
    sol_b = np.array([[3.], [-3.], [-4.]])
    c = back_substitution(sol_a, sol_b)

    solution = np.array([3., 1., 2.])
    assert np.allclose(c, solution)

def test_lu_gaussian_factorization_one():
    a = np.array([[1., 1.], [3., -4.]])
    L, U = lu_factorization(a)

    sol_L = np.array([[1., 0.], [3., 1.]])
    sol_U = np.array([[1., 1.], [0., -7.]])
    assert np.allclose(L, sol_L)
    assert np.allclose(U, sol_U)

def test_lu_gaussian_factorization_two():
    a = np.array([[1, 2, -1], [2, 1, -2], [-3, 1, 1]])
    L, U = lu_factorization(a)

    sol_L = np.array([[1., 0., 0.], [2., 1., 0.], [-3., -7/3, 1.]])
    sol_U = np.array([[1., 2., -1.], [0., -3., 0.], [ 0., 0., -2.]])
    assert np.allclose(L, sol_L)
    assert np.allclose(U, sol_U)
