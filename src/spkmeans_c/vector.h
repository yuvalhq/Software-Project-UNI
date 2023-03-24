#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef double *Vector;

/**
 * Receive a vector and its size, and copy it to a new vector.
 * The function allocates memory for the new vector, so it's the caller's
 * responsibility to free it.
 */
Vector copy_vector(Vector vector, size_t n);

/**
 * Receive a vector and its size and print it to stdout.
 */
void print_vector(Vector vector, size_t m);

/**
 * Receive two vectors and their order, while assuming the vectors are equal in
 * length, and return the square of the euclidean distance between them.
 */
double squared_euclidean_distance(Vector p, Vector q, size_t m);

/**
 * Receive two vectors and their order, while assuming the vectors are equal in
 * length, and return the euclidean distance between them.
 */
double euclidean_distance(Vector p, Vector q, size_t m);

#endif
