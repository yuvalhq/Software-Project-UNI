#include <stdio.h>
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
        if (vector[i] > -0.0001 && vector[i] <= 0){
            printf("0.0000");
        }
        else {
          printf("%.4f", vector[i]);  
        }
        if (i < m - 1) {
            printf(",");
        }
    }
    printf("\n");
}
