import pytest
import numpy as np
from numa.equation_systems import simple_gaussian_elimination, back_substitution, lu_factorization, pa_lu_factorization
from numa.exceptions import PivotIsZero, SingularMatrix


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

def test_pa_lu_gaussian_factorization_one():
    a = np.array([[2, 1, 5], [4, 4, -4], [1, 3, 1]])
    P, L, U = pa_lu_factorization(a)

    sol_P = np.array([[0, 1, 0], [0, 0, 1], [1, 0, 0]])
    sol_L = np.array([[1, 0, 0], [1/4, 1, 0], [1/2, -1/2, 1]])
    sol_U = np.array([[4, 4, -4], [0, 2, 2], [0, 0, 8]])
    assert np.allclose(P, sol_P)
    assert np.allclose(L, sol_L)
    assert np.allclose(U, sol_U)

def test_pa_lu_gaussian_factorization_two():
    a = np.array([[2, 3], [3, 2]])
    P, L, U = pa_lu_factorization(a)

    sol_P = np.array([[0, 1], [1, 0]])
    sol_L = np.array([[1, 0], [2/3, 1]])
    sol_U = np.array([[3, 2], [0, 5/3]])
    assert np.allclose(P, sol_P)
    assert np.allclose(L, sol_L)
    assert np.allclose(U, sol_U)

def test_lu_gaussian_factorization_exception():
    a = np.array([[0, 3], [3, 2]])
    with pytest.raises(PivotIsZero) as e_info:
        lu_factorization(a)

def test_pa_lu_gaussian_factorization_exception():
    a = np.array([[0, 3], [0, 2]])
    with pytest.raises(SingularMatrix) as e_info:
        pa_lu_factorization(a)
