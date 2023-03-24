#ifndef JACOBI_H
#define JACOBI_H

#include "matrix.h"
#include <stddef.h>

typedef struct JacobiParameters {
    double theta;
    double c;
    double s;
    double t;
} JacobiParameters;

typedef struct JacobiResult {
    Vector eigenvalues;
    Matrix eigenvectors;
} JacobiResult;

/**
 * Receive a symmetric matrix and run the Jacobi algorithm to return the
 * eigenvectors and eigenvalues of the matrix. The function allocates memory for
 * the JacobiResult instance, so it's the caller's responsibility to free it.
 * Notice that the JacobiResult holds the eigenvectors, and mathematically,
 * Jacobi returns a matrix that its columns are the eigenvectors, meaning the
 * return value is transposed. If the caller wants to use the matrix returned
 * from the Jacobi algorithm, it's his responsibility to transpose the returned
 * matrix. If the passed matrix is a 1x1 matrix, then it has a single eigenvalue
 * which is the singleton of the matrix, and all vectors are eigenvectors. In
 * this case we define the returned eigenvector as the singleton of 1.0.
 */
JacobiResult *jacobi(Matrix sym_mat, size_t n);

/**
 * Receive a symmetric matrix, and return the pivot coordinate for it, which is
 * the off-diagonal values with the largest absolute value.
 */
Coordinate *get_pivot_coord(Matrix mat, size_t n);

/**
 * Receive a symmetric matrix and a pivot, and return theta, c, s, t for it.
 */
JacobiParameters *get_jacobi_parameters(Matrix mat, Coordinate *pivot);

/**
 * Receive a symmetric matrix, its found pivot and Jacobi parameters, and run
 * the Jacobi iteration on it.
 */
Matrix jacobi_transform_matrix(Matrix mat, Coordinate *pivot,
                               JacobiParameters *jp, size_t n);

/**
 * Receive a symmetric matrix, its found pivot and Jacobi parameters, and run
 * the Jacobi iteration for finding its eigenvalues. This is an efficient
 * implementation taken from a paper we found at:
 * http://phys.uri.edu/nigh/NumRec/bookfpdf/f11-1.pdf
 * in page 459.
 */
Matrix jacobi_calc_eigenvectors_iteration(Matrix mat, Coordinate *pivot,
                                          JacobiParameters *jp, size_t n);

/**
 * Receive two square matrices, and calculate their off diagonal square
 * difference. Assumes that the two matrices are from the same order.
 */
double off_diagonal_square_diff(Matrix mat1, Matrix mat2, size_t n);

/**
 * Receive a square matrix and calculate its off diagonal square.
 */
double off_diagonal_square(Matrix mat, size_t n);

#endif
