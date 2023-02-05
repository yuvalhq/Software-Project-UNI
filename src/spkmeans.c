#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef DEBUG
#define DEBUG false
#endif

#define NUM_OF_ARGS 3

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "jacobi.h"
#include "spkmeans.h"


int main(int argc, char *argv[]) {
    size_t n = 0, m = 0;
    Matrix input, ww, dd, ll;
    JacobiResult *rr;
    
    handle_args(argc, argv);

    input = get_mat_from_file(argv[2], &n, &m);
    ww = wam(input, n, m);
    dd = ddg(ww, n);
    ll = gl(dd, ww, n);
    rr = jacobi(ll, n);

    printf("\n");
    print_matrix(rr -> eigenvectors, n, n);
    printf("\n");
    print_vector(rr -> eigenvalues, n);
    printf("\n");

    free_matrix(input, n);
    free_matrix(ww, n);
    free_matrix(dd, n);
    free_matrix(ll, n);
    free_matrix(rr -> eigenvectors, n);
    free(rr -> eigenvalues);
    free(rr);
    return EXIT_SUCCESS;
}


static void handle_args(int argc, char *argv[]) {
    size_t i;
    bool is_goal_valid = true;
    char *goals[] = {"wam", "ddg", "gl", "jacobi"};

    if (argc != NUM_OF_ARGS) {
        FATAL_ERROR();
    }

    for (i = 0; i < 4; i++) {
        if (strcmp(argv[1], goals[i]) == 0) {
            is_goal_valid = false;
            break;
        }
    }
    if (is_goal_valid) {
        FATAL_ERROR();
    }

    if (access(argv[2], R_OK) != 0) {
        FATAL_ERROR()
    }
}


Matrix gl(Matrix d, Matrix w, size_t n) {
    size_t i, j;
    Matrix l = build_matrix(n);

    for (i = 0; i < n; i++) {
        for (j=0; j<n; j++){
            l[i][j] = d[i][j] - w[i][j];
        }
    }
    return l;
}


Matrix ddg(Matrix w, size_t n) {
    size_t i, j;
    Matrix d = build_matrix(n);

    for (i = 0; i < n; i++) {
        for (j=0; j<n; j++){
            d[i][i] += w[i][j];
        }
    }
    return d;
}


Matrix wam(Matrix mat, size_t n, size_t m) {
    size_t i, j;
    double dist;
    Matrix w = build_matrix(n);

    for (i = 0; i < n; i++){
        for (j = i; j < n; j++){
            if (i == j) {
                w[i][j] = 0;
                continue;
            }

            dist = sq_euclidean_distance(mat[i], mat[j], m);
            w[i][j] = exp(-dist / 2);
        }
    }

    for (i = 0; i < n; i++){
        for (j = 0; j < i; j++){
            w[i][j] = w[j][i];
        }
    }
    return w;
}


double sq_euclidean_distance(Vector p, Vector q, size_t m) {
    double distance = 0;
    size_t i;

    for (i = 0; i < m; i++) {
        distance += pow(p[i] - q[i], 2);
    }
    return distance;
}
