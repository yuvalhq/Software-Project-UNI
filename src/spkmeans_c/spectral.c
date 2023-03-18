#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "spectral.h"

static int _compare_doubles(const void *a, const void *b);
static int _compare_vectors_by_first_column(const void* a, const void* b);

/**
 * Get the eigenvectors corresponding to the k smallest eigenvalues.
 */
static Matrix _get_first_k_eigenvectors(JacobiResult *jacobi_result, size_t k, size_t n);

Matrix weighted_adjacency_matrix(Matrix data_points, size_t n, size_t m) {
    size_t i, j;
    double dist;
    Matrix w = build_matrix(n, n);

    for (i = 0; i < n; i++){
        for (j = i; j < n; j++){
            if (i == j) {
                w[i][j] = 0;
                continue;
            }

            dist = squared_euclidean_distance(data_points[i], data_points[j], m);
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
    Matrix d = build_matrix(n, n);

    for (i = 0; i < n; i++) {
        for (j=0; j < n; j++){
            d[i][i] += w[i][j];
        }
    }
    return d;
}

Matrix graph_laplacian(Matrix d, Matrix w, size_t n) {
    return matrix_sub(d, w, n, n);
}

SpectralResult *spectral_clustering(Matrix data_points, size_t k, size_t n, size_t m) {
    Matrix wam = NULL;
    Matrix ddg = NULL;
    Matrix gl = NULL;
    JacobiResult *jacobi_result = NULL;
    SpectralResult *spectral_result = NULL;

    if (k > n) {
        return NULL;
    }

    spectral_result = (SpectralResult*) malloc(sizeof(SpectralResult));
    wam = weighted_adjacency_matrix(data_points, n, m);
    ddg = diagonal_degree_matrix(wam, n);
    gl = graph_laplacian(ddg, wam, n);
    jacobi_result = jacobi(gl, n);

    if (k == 0) {
        k = eigengap_heuristic(jacobi_result -> eigenvalues, n);
    }

    spectral_result -> k = k;
    spectral_result -> new_points = _get_first_k_eigenvectors(jacobi_result, k, n);

    free_matrix(wam, n);
    free_matrix(ddg, n);
    free_matrix(gl, n);
    free_matrix(jacobi_result -> eigenvectors, n);
    free(jacobi_result -> eigenvalues);
    free(jacobi_result);

    return spectral_result;
}

size_t eigengap_heuristic(Vector eigenvalues, size_t n) {
    size_t i;
    size_t max_index = 0;
    double max_delta = 0.0;
    Vector deltas = calloc(n - 1, sizeof(double));
    Vector sorted_eigenvalues = copy_vector(eigenvalues, n);

    qsort(sorted_eigenvalues, n, sizeof(double), _compare_doubles);

    for (i = 0; i < n - 1; i++) {
        deltas[i] = fabs(sorted_eigenvalues[i + 1] - sorted_eigenvalues[i]);
    }
    for (i = 0; i < n / 2; i++) {
        if (deltas[i] > max_delta) {
            max_delta = deltas[i];
            max_index = i;
        }
    }
    free(deltas);
    free(sorted_eigenvalues);
    return max_index + 1;
}

static int _compare_doubles(const void *a, const void *b) {
    double x = *(double *) a;
    double y = *(double *) b;
    if (x < y) {
        return -1;
    } else if (x > y) {
        return 1;
    } else {
        return 0;
    }
}

static int _compare_vectors_by_first_column(const void* a, const void* b) {
    const Vector row_a = *(const Vector*) a;
    const Vector row_b = *(const Vector*) b;
    return (row_a[0] > row_b[0]) - (row_b[0] > row_a[0]);
}

static Matrix _get_first_k_eigenvectors(JacobiResult *jacobi_result, size_t k, size_t n) {
    size_t i, j;
    Matrix extended_eigenvectors_mat = build_matrix(n, n + 1);
    Matrix u = build_matrix(k, n);

    for (i = 0; i < n; i++) {
        extended_eigenvectors_mat[i][0] = jacobi_result -> eigenvalues[i];
        for (j = 0; j < n; j++) {
            extended_eigenvectors_mat[i][j + 1] = jacobi_result -> eigenvectors[i][j];
        }
    }

    qsort(extended_eigenvectors_mat, n, sizeof(Vector), _compare_vectors_by_first_column);

    for (i = 0; i < k; i++) {
        for (j = 0; j < n; j++) {
            u[i][j] = extended_eigenvectors_mat[i][j + 1];
        }
    }
    free_matrix(extended_eigenvectors_mat, n);
    return u;
}
