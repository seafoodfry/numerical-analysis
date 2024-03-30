from numa.horner import horner_simple_polynomial_evaluation
from numa.interpolation import newton_divided_difference


def test_newton_divided_difference():
    # Example from Sauer page 140, example 3.1.
    x = [0, 2, 3]
    y = [1, 2, 4]
    coeff = newton_divided_difference(x, y)
    # The actual polynomial is 1/2 x^2 - 1/2 x + 1.

    # P_2 (0) = 1.
    assert horner_simple_polynomial_evaluation(coeff, 0, b=x) == 1
    # P_2 (2) = 2.
    assert horner_simple_polynomial_evaluation(coeff, 2, b=x) == 2
    # P_2 (3) = 4.
    assert horner_simple_polynomial_evaluation(coeff, 3, b=x) == 4


    # Example from Sauer page 140, example 3.2.
    x = [0, 1, 2]
    y = [2, 1, 0]
    coeff = newton_divided_difference(x, y)
    # The actual polynomial is -x + 2.

    # P_2 (0) = 2.
    assert horner_simple_polynomial_evaluation(coeff, 0, b=x) == 2
    # P_2 (1) = 1.
    assert horner_simple_polynomial_evaluation(coeff, 1, b=x) == 1
    # P_2 (2) = 0.
    assert horner_simple_polynomial_evaluation(coeff, 2, b=x) == 0


    # Example 3.4 from page 143.
    x = [0, 2, 3, 1]
    y = [1, 2, 4, 0]
    coeff = newton_divided_difference(x, y)
    # The actual polynomial is -x + 2.

    # P_3 (1) = 0.
    assert horner_simple_polynomial_evaluation(coeff, 1, b=x) == 0


    # Example 3.4 from page 143.
    x = [0, 1, 2, 3]
    y = [2, 1, 0, -1]
    coeff = newton_divided_difference(x, y)
    # The actual polynomial is 2 - x.

    # P_3 (2) = 0.
    assert horner_simple_polynomial_evaluation(coeff, 2, b=x) == 0