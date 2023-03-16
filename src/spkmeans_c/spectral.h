#ifndef SPECTRAL_H
#define SPECTRAL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "jacobi.h"
#include "matrix.h"
#include "vector.h"

typedef struct SpectralResult {
    int k;
    Matrix u;
} SpectralResult;

Matrix weighted_adjacency_matrix(Matrix mat, size_t n, size_t m);
Matrix diagonal_degree_matrix(Matrix w, size_t n);
Matrix graph_laplacian(Matrix d, Matrix w, size_t n);
SpectralResult *spectral_clustering(Matrix mat, size_t k, size_t n, size_t m);
size_t eigengap_heuristic(Vector vec, size_t n);
int compare_doubles(const void *a, const void *b);
Matrix get_first_k_eigenvectors(JacobiResult *jacobi_result, size_t k, size_t n);
int compare_vectors(const void* a, const void* b);

#endif
