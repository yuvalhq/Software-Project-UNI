#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <stddef.h>

typedef double *Vector;

double squared_euclidean_distance(Vector p, Vector q, size_t m);

#endif