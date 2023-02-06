#ifndef SPKMEANS_H
#define SPKMEANS_H

#include <stddef.h>
#include "matrix.h"

static void handle_args(int argc, char *argv[]);
Matrix gl(Matrix d, Matrix w, size_t n);
Matrix ddg(Matrix w, size_t n);
Matrix wam(Matrix mat, size_t n, size_t m);
double sq_euclidean_distance(Vector p, Vector q, size_t m);

#endif