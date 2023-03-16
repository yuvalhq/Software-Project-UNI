from typing import List

import numpy as np
import pytest

import mykmeanssp

Matrix = List[List[float]]


@pytest.mark.parametrize(
    "matrix, expected_eigenvalues, expected_eigenvectors",
    [
        (
            [[1.0, 2.0, 3.0], [2.0, 3.0, 11.0], [3.0, 11.0, 5.0]],
            [0.1864, -7.0851, 15.8986],
            [
                [0.9698, -0.2109, -0.1224],
                [0.0725, 0.7286, -0.6811],
                [0.2328, 0.6517, 0.7219],
            ],
        ),
        (
            [[3.0, 2.0, 4.0], [2.0, 0.0, 2.0], [4.0, 2.0, 3.0]],
            [-1.0, -1.0, 8.0],
            [
                [0.7071, 0.0000, -0.7071],
                [-0.2357, 0.9428, -0.2357],
                [0.6667, 0.3333, 0.6667],
            ],
        ),
    ],
)
def test_jacobi_algorithm(
    matrix: Matrix, expected_eigenvalues: List[float], expected_eigenvectors: Matrix
):
    actual_eigenvectors, actual_eigenvalues = mykmeanssp.jacobi(matrix)
    np.testing.assert_almost_equal(expected_eigenvalues, actual_eigenvalues, decimal=4)
    np.testing.assert_almost_equal(
        expected_eigenvectors, actual_eigenvectors, decimal=4
    )
