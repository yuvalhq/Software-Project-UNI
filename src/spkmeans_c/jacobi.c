#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#define MAX_ROTATIONS 100
#define EPSILON 0.00001
#define SIGN(x) (((x) < 0) ? (-1) : (1))

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "jacobi.h"

static void _unsign_zero_in_jacobi_result(JacobiResult *jr, size_t n);

JacobiResult *jacobi(Matrix mat, size_t n) {
    size_t iter;
    double convergence = 1.0;
    Matrix a = mat, a_new = NULL;
    Matrix v = build_identity_matrix(n), v_new = NULL;
    Matrix rotation_matrix = NULL;
    Coordinate *pivot = NULL;
    JacobiParameters *jp = NULL;
    JacobiResult *res = NULL;

    for(iter = 0; convergence > EPSILON && iter < MAX_ROTATIONS; iter++) {
        pivot = get_pivot_coord(a, n);
        jp = get_jacobi_parameters(a, pivot);
        rotation_matrix = build_rotation_matrix(pivot, jp, n);
        a_new = transform(a, pivot, jp, n);
        v_new = matrix_mul(v, rotation_matrix, n);
        convergence = off_diagonal_square_diff(a, a_new, n);

        free(pivot);
        free(jp);
        free_matrix(rotation_matrix, n);
        free_matrix(v, n);
        if (iter > 0) {
            free_matrix(a, n);
        }

        pivot = NULL;
        jp = NULL;
        a = a_new;
        v = v_new;
    }

    res = (JacobiResult *) malloc(sizeof(JacobiResult));
    res -> eigenvectors = transpose(v, n);
    res -> eigenvalues = matrix_diagonal_values(a, n);

    _unsign_zero_in_jacobi_result(res, n);

    free_matrix(v, n);
    if (iter > 0) {
        free_matrix(a, n);
    }
    return res;
}

Coordinate *get_pivot_coord(Matrix mat, size_t n) {
    size_t i, j;
    Coordinate *res = (Coordinate *) malloc(sizeof(Coordinate));
    double val, max_val = -1.0;

    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            val = fabs(mat[i][j]);
            if (val > max_val) {
                res -> i = i;
                res -> j = j;
                max_val = val;
            }
        }
    }
    return res;
}

JacobiParameters *get_jacobi_parameters(Matrix mat, Coordinate *pivot){
    JacobiParameters *jp = (JacobiParameters *) malloc(sizeof(JacobiParameters));
    jp -> theta = (mat[pivot -> j][pivot -> j] - mat[pivot -> i][pivot -> i]) / (2 * mat[pivot -> i][pivot -> j]);
    jp -> t = SIGN(jp -> theta) / (fabs(jp -> theta) + sqrt(pow(jp -> theta, 2) + 1));
    jp -> c = 1.0 / sqrt(pow(jp -> t, 2) + 1);
    jp -> s = (jp -> t) * (jp -> c);
    return jp;
}

Matrix transform(Matrix mat, Coordinate *pivot, JacobiParameters *jp, size_t n) {
    size_t r;
    size_t i = pivot -> i;
    size_t j = pivot -> j;
    double c = jp -> c;
    double s = jp -> s;

    Matrix mat_new = copy_matrix(mat, n);

    for (r = 0; r < n; r++) {
        if (r != i && r != j) {
            mat_new[r][i] = c * mat[r][i] - s * mat[r][j];
            mat_new[i][r] = mat_new[r][i];
            mat_new[r][j] = c * mat[r][j] + s * mat[r][i];
            mat_new[j][r] = mat_new[r][j];
        }
    }

    mat_new[i][i] = pow(c, 2) * mat[i][i] + pow(s, 2) * mat[j][j] - 2 * s * c * mat[i][j];
    mat_new[j][j] = pow(s, 2) * mat[i][i] + pow(c, 2) * mat[j][j] + 2 * s * c * mat[i][j];
    mat_new[i][j] = 0.0;
    mat_new[j][i] = 0.0;

    return mat_new;
}

Matrix build_rotation_matrix(Coordinate *pivot, JacobiParameters *jp, size_t n) {
    Matrix res = build_identity_matrix(n);

    res[pivot -> i][pivot -> i] = jp -> c;
    res[pivot -> j][pivot -> j] = jp -> c;

    if (pivot -> i > pivot -> j) {
        res[pivot -> i][pivot -> j] = -(jp -> s);
        res[pivot -> j][pivot -> i] = jp -> s;
    } else {
        res[pivot -> i][pivot -> j] = jp -> s;
        res[pivot -> j][pivot -> i] = -(jp -> s);
    }

    return res;
}

double off_diagonal_square_diff(Matrix mat1, Matrix mat2, size_t n) {
    double off1 = 0.0, off2 = 0.0;
    off1 = off_diagonal_square(mat1, n);
    off2 = off_diagonal_square(mat2, n);
    return off1 - off2;
}

double off_diagonal_square(Matrix mat, size_t n) {
    size_t i, j;
    double res = 0.0;

    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            res += 2 * pow(mat[i][j], 2);
        }
    }
    return res;
}

static void _unsign_zero_in_jacobi_result(JacobiResult *jr, size_t n) {
    size_t i, j;
    for (i = 0; i < n; i++) {
        if (jr -> eigenvalues[i] == 0 && signbit(jr -> eigenvalues[i]) != 0) {
            jr -> eigenvalues[i] = 0.0;
            for (j = 0; j < n; i++) {
               jr -> eigenvectors[j][i] = -jr -> eigenvectors[j][i];
            }
        }
    }
}
