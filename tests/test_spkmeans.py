import numpy as np
import pytest

import spkmeans_c


@pytest.mark.parametrize(
    "matrix, expected_eigenvalues",
    [
        (
            [[1.0, 2.0, 3.0], [2.0, 3.0, 11.0], [3.0, 11.0, 5.0]],
            [0.186, -7.084, 15.898],
        ),
        ([[3.0, 2.0, 4.0], [2.0, 0.0, 2.0], [4.0, 2.0, 3.0]], [-1.0, -1.0, 8.0]),
    ],
)
def test_jacobi_algorithm__eigenvalues(matrix, expected_eigenvalues):
    _, actual_eigenvalues = spkmeans_c.jacobi(matrix)
    np.testing.assert_almost_equal(expected_eigenvalues, actual_eigenvalues, decimal=3)
