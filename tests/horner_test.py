from numa.horner import horner_simple_polynomial_evaluation

def test_horner_one():
    assert horner_simple_polynomial_evaluation(4, [-1, 5, -3, 3, 2], 0.5) == 1.25
