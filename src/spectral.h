#ifndef SPECTRAL_H
#define SPECTRAL_H

#include "matrix.h"

Matrix weighted_adjacency_matrix(Matrix mat, size_t n, size_t m);
Matrix diagonal_degree_matrix(Matrix w, size_t n);
Matrix graph_laplacian(Matrix d, Matrix w, size_t n);

#endif