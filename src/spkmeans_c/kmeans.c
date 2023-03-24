#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <math.h>
#include "kmeans.h"

static size_t assign_vector_to_cluster(Vector vector, Cluster *clusters,
                                       size_t vector_size, size_t k);
static bool update_centroid_of_cluster(Cluster *cluster, Vector *vectors,
                                       size_t *cluster_mapping,
                                       size_t cluster_idx, size_t vectors_count,
                                       size_t vector_size, double epsilon);

static size_t assign_vector_to_cluster(Vector vector, Cluster *clusters,
                                       size_t vector_size, size_t k) {
    size_t i = 0, assigned_cluster_idx = 0;
    double min_distance = INFINITY;
    Vector centroid = NULL;
    double distance;

    for (i = 0; i < k; i++) {
        centroid = clusters[i].centroid;
        distance = euclidean_distance(centroid, vector, vector_size);

        if (distance < min_distance) {
            min_distance = distance;
            assigned_cluster_idx = i;
        }
    }

    return assigned_cluster_idx;
}

static bool update_centroid_of_cluster(Cluster *cluster, Vector *vectors,
                                       size_t *cluster_mapping,
                                       size_t cluster_idx, size_t vectors_count,
                                       size_t vector_size, double epsilon) {
    size_t i, j, cluster_size = 0;
    bool done = false;
    double distance = INFINITY;

    Vector old_centroid = (*cluster).centroid;
    Vector new_centroid = calloc(vector_size, sizeof(double));

    for (i = 0; i < vectors_count; i++) {
        if (cluster_mapping[i] != cluster_idx) {
            continue;
        }

        ++cluster_size;
        for (j = 0; j < vector_size; j++) {
            new_centroid[j] += vectors[i][j];
        }
    }

    for (i = 0; i < vector_size; i++) {
        new_centroid[i] /= cluster_size;
    }

    distance = euclidean_distance(new_centroid, old_centroid, vector_size);
    done = distance < epsilon;
    (*cluster).centroid = new_centroid;
    free(old_centroid);
    return done;
}

void fit(Cluster *clusters, Vector *vectors, size_t vectors_count,
         size_t vector_size, size_t k, size_t iter, double epsilon) {
    size_t *cluster_mapping = (size_t *) calloc(vectors_count, sizeof(size_t));
    size_t i, j;
    bool done = false;

    for (i = 0; i < iter && !done; i++) {
        for (j = 0; j < vectors_count; j++) {
            cluster_mapping[j] = assign_vector_to_cluster(vectors[j], clusters, vector_size, k);
        }

        done = true;
        for (j = 0; j < k; j++) {
            done &= update_centroid_of_cluster(&clusters[j], vectors, cluster_mapping, j, vectors_count, vector_size, epsilon);
        }
    }

    free(cluster_mapping);
}
