#ifndef KMEANS_H
#define KMEANS_H

#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "strutils.h"
#include "vector.h"

#define DEFAULT_ITERATIONS_COUNT 300
#define DEFAULT_EPSILON 0.00001

typedef double Distance;

typedef struct Cluster {
    Vector centroid;
} Cluster;

Distance euclidean_distance(Vector p, Vector q, size_t size);
size_t assign_vector_to_cluster(Cluster *clusters, Vector vector, size_t vector_size, size_t k);
bool update_centroid_of_cluster(Cluster *clusters, Vector *vectors, size_t *cluster_mapping, size_t cluster_idx, size_t vectors_count, size_t vector_size, double epsilon);
void fit(Cluster **clusters, size_t vectors_count, Vector *vectors, size_t vector_size, size_t k, size_t iter, double epsilon);

#endif
