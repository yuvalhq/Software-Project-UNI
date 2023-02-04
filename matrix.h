#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

typedef double *Vector;
typedef Vector *Matrix;

typedef struct Coordinate {
    size_t i;
    size_t j;
} Coordinate;

Matrix build_matrix(size_t n);
Matrix build_identity_mat(size_t n);
void free_matrix(Matrix mat, size_t n);
Vector get_diagonal_values_from_matrix(Matrix mat, size_t n);
void print_matrix(Matrix mat, size_t n, size_t m);
void print_vector(Vector vector, size_t m);
Matrix get_mat_from_file(char *filename, size_t *n, size_t *m);

#endif