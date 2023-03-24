#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
#include <stddef.h>

typedef Vector *Matrix;

typedef struct Coordinate {
    size_t i;
    size_t j;
} Coordinate;

/**
 * Build a zero matrix from the given order.
 * The function allocates memory for the matrix, so it's the caller's
 * responsibility to free it.
 */
Matrix build_matrix(size_t n, size_t m);

/**
 * Build an identity matrix from the given order.
 * The function allocates memory for the matrix, so it's the caller's
 * responsibility to free it.
 */
Matrix build_identity_matrix(size_t n);

/**
 * Build a matrix from the given file path, and set its order in the given
 * pointers. The function allocates memory for the matrix, so it's the caller's
 * responsibility to free it.
 */
Matrix build_matrix_from_file(char *filename, size_t *n, size_t *m);

/**
 * Receive a matrix and its order, and return a pointer to a new copied matrix.
 * The function allocates memory for the copied matrix, so it's the caller's
 * responsibility to free it.
 */
Matrix copy_matrix(Matrix mat, size_t n, size_t m);

/**
 * Receive two matrices, and their order, and return a new matrix defined by
 * their subtraction. The function allocates memory for the new matrix, so it's
 * the caller's responsibility to free it.
 */
Matrix matrix_sub(Matrix left, Matrix right, size_t n, size_t m);

/**
 * Receive two square matrices, and their order, and return a new matrix defined
 * by their multiplication. The function allocates memory for the new matrix, so
 * it's the caller's responsibility to free it.
 */
Matrix matrix_mul(Matrix left, Matrix right, size_t n);

/**
 * Receive a matrix and its order, and return a pointer to a new transposed
 * matrix. The function allocates memory for the new matrix, so it's the
 * caller's responsibility to free it.
 */
Matrix transpose(Matrix mat, size_t n, size_t m);

/**
 * Receive a squared matrix and its order, and return a pointer to a vector of
 * the diagonal vectors. An example usage of this function is to extract the
 * eigenvalues of a diagonal matrix. The function allocates memory for the new
 * vector, so it's the caller's responsibility to free it.
 */
Vector matrix_diagonal_values(Matrix mat, size_t n);

/**
 * Receive a matrix and its order and print it to stdout.
 */
void print_matrix(Matrix mat, size_t n, size_t m);

/**
 * Receive a matrix and its order and print its transpose to stdout.
 */
void print_transposed_matrix(Matrix mat, size_t n, size_t m);

/**
 * Receive a square matrix and its order and free it.
 */
void free_matrix(Matrix mat, size_t n);

#endif
