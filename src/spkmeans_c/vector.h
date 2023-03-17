#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <stddef.h>

typedef double *Vector;

Vector copy_vector(Vector vector, size_t n);
double squared_euclidean_distance(Vector p, Vector q, size_t m);
void print_vector(Vector vector, size_t m);

#endif
