#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "spkmeans.h"
#include "jacobi.h"
#include "matutils.h"


JacobiResult *jacobi(Matrix mat, size_t n) {
    size_t i, j, iter;
    bool pivoted = false;
    double convergence = 1.0;
    Matrix a = mat, a_new;
    Matrix v = build_identity_mat(n), v_new;
    Coordinate *pivot = NULL;
    JacobiParameters *jp = NULL;
    JacobiResult *res = NULL;

    for(iter = 0; convergence > epsilon && iter < MAX_ROTATIONS; iter++) {
        pivot = get_pivot_coord(a, n);
        jp = get_jacobi_parameters(a, pivot);
        a_new = transform(a, pivot, jp -> c, jp -> s, n);
        v_new = jacobi_left_matrix_mul(v, pivot, jp -> c, jp -> s, n);
        convergence = off_sq_diff(a, a_new, n);
    
        free(pivot);
        free(jp);
        free_matrix(v, n);
        if (pivoted) {
            free_matrix(a, n);
        }

        pivot = NULL;
        jp = NULL;
        a = a_new;
        v = v_new;

        pivoted = true;
    }

    res = (JacobiResult *) malloc(sizeof(JacobiResult));
    res -> eigenvectors = v;
    res -> eigenvalues = get_diagonal_values_from_matrix(a, n);
    for (i = 0; i < n; i++) {
        if (res -> eigenvalues[i] == 0 && signbit(res -> eigenvalues[i]) != 0) {
            res -> eigenvalues[i] = 0;
            for (j = 0; j < n; i++) {
               v[i][j] = -v[i][j];
            }
        }
    }

    if (pivoted) {
        free_matrix(a, n);
    }
    return res;
}


Coordinate *get_pivot_coord(Matrix mat, size_t n) {
    size_t i, j;
    Coordinate *res = (Coordinate *) malloc(sizeof(Coordinate));
    double val, max_val = -1.0;

    for (i = 0; i < n; i++) {
        for (j= i+1; j < n; j++) {
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


JacobiParameters *get_jacobi_parameters(Matrix a, Coordinate *pivot){
    JacobiParameters *jp = (JacobiParameters *) malloc(sizeof(JacobiParameters));
    jp -> theta = (a[pivot -> j][pivot -> j] - a[pivot -> i][pivot -> i]) / (2 * a[pivot -> i][pivot -> j]);
    jp -> t = ((jp -> theta) / fabs(jp -> theta)) / (fabs(jp -> theta) + sqrt(pow(jp -> theta, 2) + 1));
    jp -> c = 1.0 / sqrt(pow((jp -> t), 2) + 1);
    jp -> s = (jp -> t) * (jp -> c);
    return jp;
}


Matrix transform(Matrix mat, Coordinate *pivot, double c, double s, size_t n) {
    size_t i, j;
    Matrix mat_new = build_matrix(n);

    for (i = 0; i < n; i++) {
        for (j = i; j < n; j++) {
            if (i == pivot -> i){
                if (j == pivot -> j) {
                    mat_new[i][j] = 0.0;
                } else if (i == j){
                    mat_new[i][j] = (pow(c, 2) * mat[i][i]) + (pow(s, 2) * mat[j][j]) - (2 * s * c * mat[i][j]);
                } else {
                    mat_new[i][j] = (c * mat[i][j]) + (s * mat[i][pivot -> j]);
                }
            } else if (j == pivot -> j) {
                if (i == j) {
                    mat_new[i][j] = (pow(s, 2) * mat[i][i]) + (pow(c, 2) * mat[j][j]) + (2 * s * c * mat[i][j]);
                } else {
                    mat_new[i][j] = (c * mat[i][j]) + (s * mat[i][pivot -> i]);
                }
            } else {
                mat_new[i][j] = mat[i][j];
            }
        }
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < i; j++) {
            mat_new[i][j] = mat_new[j][i];
        }
    }
    return mat_new;
}


Matrix build_rotation_matrix(Coordinate *pivot, JacobiParameters *jp, size_t n) {
    Matrix p = build_identity_mat(n);
    p[pivot -> i][pivot -> i] = jp -> c;
    p[pivot -> i][pivot -> j] = jp -> s;
    p[pivot -> j][pivot -> i] = -(jp -> s);
    p[pivot -> j][pivot -> j] = jp -> c;
    return p;
}


Matrix mat_mul(Matrix mat1, Matrix mat2, size_t n){
    size_t i, j, k;
    Matrix res;

    res = build_matrix(n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) {
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    return res;
}


Matrix jacobi_left_matrix_mul(Matrix mat, Coordinate *pivot, double c, double s, size_t n) {
    size_t i, j;
    Matrix res = build_matrix(n);

    for (i = 0; i < n; i++) {
        for (j = i; j < n; j++) {
            if ((i == pivot -> i || j == pivot -> j) && i == j){
                res[i][j] = (c * mat[i][j]);
            } else if (i == pivot -> i) {
                res[i][j] = (c * mat[i][j]) - (s * mat[pivot -> j][j]);
            } else if (j == pivot -> j) {
                res[i][j] = (s * mat[i][pivot -> i]) - (c * mat[i][j]);
            } else {
                res[i][j] = mat[i][j];
            }
        }
    } 
    for (i = 0; i < n; i++) {
        for (j = 0; j < i; j++) {
            res[i][j] = res[j][i];
        }
    } 
    return res;
}


double off_sq_diff(Matrix mat1, Matrix mat2, size_t n) {
    double off1 = 0.0, off2 = 0.0;
    off1 = off_sq(mat1, n);
    off2 = off_sq(mat2, n);
    return (off1 - off2);
}


double off_sq(Matrix mat, size_t n) {
    size_t i, j;
    double res = 0.0;

    for (i = 0; i < n; i++) {
        for (j = i+1; j < n; j++) {
            res += (2 * pow(mat[i][j], 2));
        }
    }
    return res;
}
