import numpy.typing as npt


class NoSolution(Exception):
    def __init__(self, message: str):
        self.message = message
        super().__init__(self.message)

class PivotIsZero(Exception):
    def __init__(self, bad_row: npt.ArrayLike):
        self.message = f"Bad pivot in row: {bad_row}"
        super().__init__(self.message)

class SingularMatrix(Exception):
    def __init__(self):
        self.message = "A is a singular matrix. Gauss elimination cannot proceed"
        super().__init__(self.message)
