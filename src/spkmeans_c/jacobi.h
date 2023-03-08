#ifndef JACOBI_H
#define JACOBI_H

#include <stddef.h>
#include "matrix.h"

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

JacobiResult *jacobi(Matrix mat, size_t n);
Coordinate *get_pivot_coord(Matrix mat, size_t n);
JacobiParameters *get_jacobi_parameters(Matrix mat, Coordinate *pivot);
Matrix transform(Matrix mat, Coordinate *pivot, JacobiParameters *jp, size_t n);
Matrix mat_mul_left_jacobi(Matrix mat, Coordinate *pivot, JacobiParameters *jp, size_t n);
double off_diagonal_square_diff(Matrix mat1, Matrix mat2, size_t n);
double off_diagonal_square(Matrix mat, size_t n);

#endif
