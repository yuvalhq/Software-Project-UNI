#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#define MAX_ROTATIONS 1000
#define EPSILON 0.0000001
#define SIGN(x) (((x) < 0) ? (-1) : (1))
#define _is_negative_zero(x) ((x == 0.0 && signbit(x) != 0) || \
                              (x > -0.0001 && x < 0.0))

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "jacobi.h"

static void _unsign_zero_in_jacobi_result(JacobiResult *jr, size_t n);

JacobiResult *jacobi(Matrix sym_mat, size_t n) {
    size_t iter;
    double convergence = 1.0;
    Matrix a = sym_mat, a_new = NULL;
    Matrix v = NULL, v_new = NULL;
    Coordinate *pivot = NULL;
    JacobiParameters *jp = NULL;
    JacobiResult *res = (JacobiResult *) malloc(sizeof(JacobiResult));

    if (n == 1) {
        res -> eigenvectors = build_matrix(1, 1);
        res -> eigenvectors[0][0] = 1.0;
        res -> eigenvalues = (Vector) malloc(sizeof(double));
        res -> eigenvalues[0] = sym_mat[0][0];
        return res;
    }

    v = build_identity_matrix(n);
    for(iter = 0; convergence > EPSILON && iter < MAX_ROTATIONS; iter++) {
        pivot = get_pivot_coord(a, n);
        jp = get_jacobi_parameters(a, pivot);
        a_new = jacobi_transform_matrix(a, pivot, jp, n);
        v_new = jacobi_calc_eigenvectors_iteration(v, pivot, jp, n);
        convergence = off_diagonal_square_diff(a, a_new, n);

        free(pivot);
        free(jp);
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
    res -> eigenvectors = transpose(v, n, n);
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
    jp -> t = SIGN(jp -> theta) / (fabs(jp -> theta) + sqrt(pow(jp -> theta, 2) + 1.0));
    jp -> c = 1.0 / sqrt(pow(jp -> t, 2) + 1.0);
    jp -> s = (jp -> t) * (jp -> c);
    return jp;
}

Matrix jacobi_transform_matrix(Matrix mat, Coordinate *pivot, JacobiParameters *jp, size_t n) {
    size_t r;
    size_t i = pivot -> i;
    size_t j = pivot -> j;
    double c = jp -> c;
    double s = jp -> s;

    Matrix mat_new = copy_matrix(mat, n, n);

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

Matrix jacobi_calc_eigenvectors_iteration(Matrix mat, Coordinate *pivot, JacobiParameters *jp, size_t n) {
    size_t r;
    size_t p = pivot -> i;
    size_t q = pivot -> j;
    double c = jp -> c;
    double s = jp -> s;

    Matrix mat_new = copy_matrix(mat, n, n);

    for (r = 0; r < n; r++) {
        mat_new[r][p] = c * mat[r][p] - s * mat[r][q];
        mat_new[r][q] = c * mat[r][q] + s * mat[r][p];
    }

    return mat_new;
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
        if (_is_negative_zero(jr -> eigenvalues[i])) {
            jr -> eigenvalues[i] = 0.0;
            for (j = 0; j < n; j++) {
               jr -> eigenvectors[i][j] = -(jr -> eigenvectors[i][j]);
            }
        }
    }
}
