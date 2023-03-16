#include <stdio.h>
#include "common.h"
#include "vector.h"

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
