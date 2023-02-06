#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef DEBUG
#define DEBUG false
#endif

#define NUM_OF_ARGS 3

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"
#include "jacobi.h"
#include "matrix.h"
#include "spectral.h"
#include "spkmeans.h"

int main(int argc, char *argv[]) {
    size_t n = 0, m = 0;
    Matrix input, ww, dd, ll;
    JacobiResult *rr;
    
    handle_args(argc, argv);

    input = build_matrix_from_file(argv[2], &n, &m);
    ww = weighted_adjacency_matrix(input, n, m);
    dd = diagonal_degree_matrix(ww, n);
    ll = graph_laplacian(dd, ww, n);
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