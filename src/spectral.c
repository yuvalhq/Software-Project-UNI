#include "spectral.h"
#include "vector.h"

Matrix weighted_adjacency_matrix(Matrix mat, size_t n, size_t m) {
    size_t i, j;
    double dist;
    Matrix w = build_matrix(n);

    for (i = 0; i < n; i++){
        for (j = i; j < n; j++){
            if (i == j) {
                w[i][j] = 0;
                continue;
            }

            dist = squared_euclidean_distance(mat[i], mat[j], m);
            w[i][j] = exp(-dist / 2);
        }
    }

    for (i = 0; i < n; i++){
        for (j = 0; j < i; j++){
            w[i][j] = w[j][i];
        }
    }
    return w;
}

Matrix diagonal_degree_matrix(Matrix w, size_t n) {
    size_t i, j;
    Matrix d = build_matrix(n);

    for (i = 0; i < n; i++) {
        for (j=0; j < n; j++){
            d[i][i] += w[i][j];
        }
    }
    return d;
}

Matrix graph_laplacian(Matrix d, Matrix w, size_t n) {
    return matrix_sub(d, w, n);
}