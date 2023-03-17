#ifndef SPECTRAL_H
#define SPECTRAL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jacobi.h"
#include "matrix.h"
#include "vector.h"

typedef struct SpectralResult {
    size_t k;
    Matrix new_points;
} SpectralResult;

/**
 * Receive an array of datapoints and its dimensions and calculate the weighted adjacency matrix of it.
 * The function allocates memory for the new matrix, so it's the caller's responsibility to free it.
 */
Matrix weighted_adjacency_matrix(Matrix data_points, size_t n, size_t m);

/**
 * Receive a square matrix and its order and calculate the diagonal degree matrix of it.
 * The function allocates memory for the new matrix, so it's the caller's responsibility to free it.
 */
Matrix diagonal_degree_matrix(Matrix w, size_t n);

/**
 * Receive the diagonal degree matrix and the weighted adjacency matrix, and calculate the graph Laplacian,
 * by subtracting between them.
 * The function allocates memory for the new matrix, so it's the caller's responsibility to free it.
 */
Matrix graph_laplacian(Matrix d, Matrix w, size_t n);

/**
 * Receive an array of datapoints, its dimensions and the value k.
 * Runs the spectral clustering algorithm and returns an array of new datapoints and the effective k value used.
 * If k == 0, the function will use the eigengap heuristic to determine the best k value.
 */
SpectralResult *spectral_clustering(Matrix data_points, size_t k, size_t n, size_t m);

/**
 * Receive a vector of eigenvalues and determine the best k value to use with these eigenvalues.
 */
size_t eigengap_heuristic(Vector eigenvalues, size_t n);

#endif
