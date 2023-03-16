#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "spectral.h"


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

Matrix spectral_clustering(Matrix mat, int k, size_t n, size_t m) {
    Matrix wam, ddg, gl, u;
    JacobiResult *jacobi_result = NULL;

    wam = weighted_adjacency_matrix(mat, n, m);
    ddg = diagonal_degree_matrix(wam, n);
    gl = graph_laplacian(ddg, wam, n);
    jacobi_result = jacobi(gl, n);
    
    if (k == -1) {
        k = eigengap_heuristic(jacobi_result -> eigenvalues, n);
    }

    u = get_first_k_eigenvectors(jacobi_result, k, n);
    
    free_matrix(wam, n);
    free_matrix(ddg, n); 
    free_matrix(gl, n);
    free_matrix(jacobi_result -> eigenvectors, n);
    free(jacobi_result -> eigenvalues);
    free(jacobi_result);

    return u;
}

int eigengap_heuristic(Vector vector, size_t n) {
    size_t i;
    size_t half = n / 2;
    size_t max_index = 0;
    double max_delta = 0.0;
    Vector deltas = malloc((n - 1) * sizeof(double));
    Vector vec = copy_vector(vector, n);

    qsort(vec, n, sizeof(double), compare_doubles);

    for (i = 0; i < n - 1; i++) {
        deltas[i] = fabs(vec[i + 1] - vec[i]);
    }
    for (i = 0; i < half; i++) {
        if (deltas[i] > max_delta) {
            max_delta = deltas[i];
            max_index = i;
        }
    }
    free(deltas);
    free(vec);
    return (int) (max_index + 1);
}

int compare_doubles(const void *a, const void *b) {
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

Matrix get_first_k_eigenvectors(JacobiResult *jacobi_result, int k, size_t n) {
    size_t i, j;
    Matrix jacobi_res_t = build_nonsquare_matrix(n, n + 1);
    Matrix u = build_matrix(n);

    for (i = 0; i < n; i++) {
        jacobi_res_t[i][0] = jacobi_result -> eigenvalues[i];
        for (j = 0; j < n; j++) {
            jacobi_res_t[i][j+1] = jacobi_result -> eigenvectors[i][j];
        }
    }

    qsort(jacobi_res_t, n, sizeof(Vector), compare_vectors);

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            u[i][j] = jacobi_res_t[i][j+1];
        }
    }
    free_matrix(jacobi_res_t, n);
    return u;
}

int compare_vectors(const void* a, const void* b) {
    const Vector row_a = *(const Vector*)a;
    const Vector row_b = *(const Vector*)b;
    return (row_b[0] > row_a[0]) - (row_a[0] > row_b[0]);
}
