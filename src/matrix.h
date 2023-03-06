#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>
#include "vector.h"

typedef Vector *Matrix;

typedef struct Coordinate {
    size_t i;
    size_t j;
} Coordinate;

Matrix build_matrix(size_t n);
Matrix build_identity_matrix(size_t n);
Matrix build_matrix_from_file(char *filename, size_t *n, size_t *m);
Matrix matrix_sub(Matrix left, Matrix right, size_t n);
Vector matrix_diagonal_values(Matrix mat, size_t n);
void print_matrix(Matrix mat, size_t n, size_t m);
void free_matrix(Matrix mat, size_t n);

#endif