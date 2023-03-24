#ifndef KMEANS_H
#define KMEANS_H

#include "vector.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define DEFAULT_ITERATIONS_COUNT 300
#define DEFAULT_EPSILON 0.001

typedef struct Cluster {
  Vector centroid;
} Cluster;

/**
 * Receive an array of initial clusters, an array of vectors to
 * cluster, the number of vectors, the size of each vector, the value k, number
 * of iterations and an epsilon value, and partition the vectors to clusters,
 * using the K-means algorithm. The return value is void, and the new clusters
 * are set in the passed clusters array.
 */
void fit(Cluster *clusters, Vector *vectors, size_t vectors_count,
         size_t vector_size, size_t k, size_t iter, double epsilon);

#endif
