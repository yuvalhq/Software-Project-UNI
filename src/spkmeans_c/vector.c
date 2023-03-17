#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

Vector copy_vector(Vector vector, size_t n) {
    size_t i;
    Vector copy = (Vector) calloc(n, sizeof(double));

    for (i = 0; i < n; i++) {
        copy[i] = vector[i];
    }
    return copy;
}

double squared_euclidean_distance(Vector p, Vector q, size_t m) {
    double distance = 0.0;
    size_t i;

    for (i = 0; i < m; i++) {
        distance += pow(p[i] - q[i], 2);
    }
    return distance;
}

void print_vector(Vector vector, size_t m) {
    size_t i;
    for (i = 0; i < m; i++) {
        printf("%.4f", vector[i]);
        if (i < m - 1) {
            printf(",");
        }
    }
    printf("\n");
}
