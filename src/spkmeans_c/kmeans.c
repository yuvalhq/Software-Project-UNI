#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kmeans.h"

#define COMMA ','
#define NEWLINE '\n'

#ifndef DEBUG
#define DEBUG false
#endif
#define FATAL_ERROR() {\
    printf("An Error Has Occurred\n");\
    exit(EXIT_FAILURE);\
}
#define FATAL_ERROR_CUSTOM_MSG(MSG) {\
    printf(MSG);\
    exit(EXIT_FAILURE);\
}

size_t strcount(char *str, char c);
void handle_args(int argc, char *argv[], size_t *k, size_t *iter);
size_t build_vectors_from_input(size_t k, Vector **vectors, size_t *vector_size);
size_t assign_vector_to_cluster(Cluster *clusters, Vector vector, size_t vector_size, size_t k);
bool update_centroid_of_cluster(Cluster *clusters, Vector *vectors, size_t *cluster_mapping, size_t cluster_idx, size_t vectors_count, size_t vector_size, double epsilon);
void print_vector(Vector vector, size_t vector_size);
void print_output(Cluster *clusters, size_t vector_size, size_t k);

int main(int argc, char *argv[]) {
    Cluster *clusters;
    Vector *vectors = (Vector *) malloc(sizeof(double));
    size_t vectors_count = 0, k = 0, iter = 0, vector_size = 0, i = 0;
    handle_args(argc, argv, &k, &iter);

    clusters = (Cluster *) malloc(sizeof(Cluster) * k);
    
    if (DEBUG) printf("k: %lu, iter: %lu\n", (unsigned long) k, (unsigned long) iter);

    vectors_count = build_vectors_from_input(k, &vectors, &vector_size);
    if (DEBUG) printf("Got %lu vectors\n", (unsigned long) vectors_count);
    
    init_clusters(&clusters, vectors, vector_size, k);
    fit(&clusters, vectors_count, vectors, vector_size, k, iter, DEFAULT_EPSILON);
    print_output(clusters, vector_size, k);

    for (i = 0; i < vectors_count; i++) {
        if (i < k) {
            free(clusters[i].centroid);
        }
        free(vectors[i]);
        vectors[i] = NULL;
    }
    free(vectors);
    free(clusters);
    vectors = NULL;
    clusters = NULL;
    return EXIT_SUCCESS;
}

size_t strcount(char *str, char c) {
    size_t length = strlen(str), count = 0, i;
    for (i = 0; i < length; i++) {
        if (str[i] == c) {
            count++;
        }
    }
    return count;
}

void handle_args(int argc, char *argv[], size_t *k, size_t *iter) {
    if (argc == 2) {
        *k = atoi(argv[1]);
        *iter = DEFAULT_ITERATIONS_COUNT;
    } else if (argc == 3) {
        *k = atoi(argv[1]);
        *iter = atoi(argv[2]);
    } else {
        FATAL_ERROR();
    }

    if (*k <= 1) {
        FATAL_ERROR_CUSTOM_MSG("Invalid number of clusters!\n");
    }

    if (*iter <= 1 || *iter >= 1000) {
        FATAL_ERROR_CUSTOM_MSG("Invalid maximum iteration!\n");
    }
}

size_t build_vectors_from_input(size_t k, Vector **vectors, size_t *vector_size) {
    char *line = NULL;
    size_t line_len;
    ssize_t read_len;
    size_t vector_len;
    Vector vector;
    char *vector_idx;
    size_t vectors_count = 0;
    size_t i = 0;

    while ((read_len = getline(&line, &line_len, stdin)) != -1) {
        vector_len = strcount(line, COMMA) + 1;
        if (*vector_size == 0) {
            *vector_size = vector_len;
        } else if (*vector_size != vector_len) {
            FATAL_ERROR();
        }

        vector = (Vector) malloc(sizeof(double) * vector_len);
        vector_idx = line;

        for (i = 0; i < vector_len; i++) {
            vector[i] = strtod(vector_idx, &vector_idx);
            if (*vector_idx != COMMA && *vector_idx != NEWLINE) {
                FATAL_ERROR();
            }
            vector_idx++;
        }

        *vectors = (Vector *) realloc(*vectors, ++vectors_count * sizeof(Vector));
        (*vectors)[vectors_count - 1] = vector;
    }

    if (k >= vectors_count) {
        FATAL_ERROR_CUSTOM_MSG("Invalid number of clusters!\n");
    }

    free(line);
    line = NULL;
    return vectors_count;
}

Distance euclidean_distance(Vector p, Vector q, size_t size) {
    double distance = 0;
    size_t i;

    for (i = 0; i < size; i++) {
        distance += pow(p[i] - q[i], 2);
    }

    return sqrt(distance);
}

size_t assign_vector_to_cluster(Cluster *clusters, Vector vector, size_t vector_size, size_t k) {
    size_t i = 0, assigned_cluster = 0;
    Distance min_distance = INFINITY;

    for (i = 0; i < k; i++) {
        Vector centroid = clusters[i].centroid;
        Distance distance = euclidean_distance(centroid, vector, vector_size);

        if (distance < min_distance) {
            min_distance = distance;
            assigned_cluster = i;
        }
    }

    return assigned_cluster;
}

bool update_centroid_of_cluster(Cluster *cluster, Vector *vectors, size_t *cluster_mapping,
    size_t cluster_idx, size_t vectors_count, size_t vector_size, double epsilon) {

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
    old_centroid = NULL;
    return done;
}

void init_clusters(Cluster **clusters, Vector *vectors, size_t vector_size, size_t k) {
    size_t i;
    for (i = 0; i < k; i++) {
        Cluster cluster;
        cluster.centroid = calloc(vector_size, sizeof(double));
        memcpy(cluster.centroid, vectors[i], vector_size * sizeof(double));
        (*clusters)[i] = cluster;
    }
}

void fit(Cluster **clusters, size_t vectors_count, Vector *vectors, size_t vector_size, size_t k, size_t iter, double epsilon) {
    size_t *cluster_mapping = (size_t *) calloc(vectors_count, sizeof(size_t));
    size_t i, j;
    bool done = false;

    for (i = 0; i < iter && !done; i++) {
        for (j = 0; j < vectors_count; j++) {
            cluster_mapping[j] = assign_vector_to_cluster(*clusters, vectors[j], vector_size, k);
        }

        done = true;
        for (j = 0; j < k; j++) {
            done &= update_centroid_of_cluster(&((*clusters)[j]), vectors, cluster_mapping, j, vectors_count, vector_size, epsilon);
        }
    }

    free(cluster_mapping);
    cluster_mapping = NULL;
}

void print_vector(Vector vector, size_t vector_size) {
    size_t i;
    for (i = 0; i < vector_size; i++) {
        printf("%.4f", vector[i]);
        if (i < vector_size - 1) {
            printf(",");
        }
    }
}

void print_output(Cluster *clusters, size_t vector_size, size_t k) {
    size_t i;
    for (i = 0; i < k; i++) {
        print_vector(clusters[i].centroid, vector_size);
        printf("\n");
    }
}