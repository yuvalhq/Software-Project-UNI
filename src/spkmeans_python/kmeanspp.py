from typing import List, Tuple

import numpy as np

import mykmeanssp

EPSILON = 0.00001


def compute_dx(
    points: np.ndarray,
    centroids_idxs: List[int],
    N: int,
) -> np.ndarray:
    dx_arr = np.full(N, -1.0)

    for j, point in enumerate(points):
        for centroid_idx in centroids_idxs:
            centroid = points[centroid_idx]
            dist = np.linalg.norm(point - centroid)  # Euclidean Distance

            if dx_arr[j] == -1 or dist < dx_arr[j]:
                dx_arr[j] = dist

    return dx_arr


def compute_probabilities(dx_arr: np.ndarray, N: int) -> np.ndarray:
    probabilities = np.zeros(N)
    denominator = np.sum(dx_arr)

    for j in range(N):
        probabilities[j] = dx_arr[j] / denominator

    return probabilities


def kmeanspp(points: np.ndarray, k: int) -> Tuple[np.ndarray, List[int]]:
    np.random.seed(0)
    N, vec_len = points.shape

    centroids = np.empty((k, vec_len))
    centroids_idxs = []

    # step 1
    first_center_idx = np.random.choice(N)
    centroids_idxs.append(first_center_idx)
    centroids[0] = points[first_center_idx]

    # step 4
    for i in range(1, k):
        # step 2
        dx_arr = compute_dx(points, centroids_idxs, N)

        # step 3
        probabilities = compute_probabilities(dx_arr, N)
        new_center = np.random.choice(N, p=probabilities)
        centroids_idxs.append(new_center)
        centroids[i] = points[new_center]

    # step 5
    result = mykmeanssp.fit(
        centroids.tolist(), points.tolist(), k, mykmeanssp.DEFAULT_ITERATIONS_COUNT, mykmeanssp.DEFAULT_EPSILON
    )
    return result, centroids_idxs
