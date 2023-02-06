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
    Matrix input = NULL, wam = NULL, ddg = NULL, gl = NULL;
    JacobiResult *jacobi_result = NULL;
    
    CommandLineArguments *args = handle_args(argc, argv);
    input = build_matrix_from_file(args -> input_file_path, &n, &m);

    if (args -> goal == JACOBI) {
        jacobi_result = jacobi(input, n);

        print_matrix(jacobi_result -> eigenvectors, n, n);
        print_vector(jacobi_result -> eigenvalues, n);

        free_matrix(jacobi_result -> eigenvectors, n);
        free(jacobi_result -> eigenvalues);
        free(jacobi_result);

        return EXIT_SUCCESS;
    }

    wam = weighted_adjacency_matrix(input, n, m);
    if (args -> goal == WAM) {
        print_matrix(wam, n, n);
        goto free_wam;
    }

    ddg = diagonal_degree_matrix(wam, n);
    if (args -> goal == DDG) {
        print_matrix(ddg, n, n);
        goto free_ddg;
    }

    gl = graph_laplacian(ddg, wam, n);
    if (args -> goal == GL) {
        print_matrix(gl, n, n);
        goto free_gl;
    }

free_gl:
    free_matrix(gl, n);
free_ddg:
    free_matrix(ddg, n);
free_wam:
    free_matrix(wam, n);
    free(args);
    free_matrix(input, n);

    return EXIT_SUCCESS;
}

static Goal create_goal_from_name(char *goal_name) {
    size_t i;

    for (i = 0; goal_names[i] != NULL; i++) {
        if (strcmp(goal_names[i], goal_name) == 0) {
            return (Goal) i;
        }
    }
    
    return UNKNOWN;
}

static CommandLineArguments* handle_args(int argc, char *argv[]) {
    CommandLineArguments* args = NULL;

    if (argc != NUM_OF_ARGS) {
        FATAL_ERROR();
    }

    args = (CommandLineArguments *) malloc(sizeof(CommandLineArguments));
    args -> goal = create_goal_from_name(argv[1]);
    args -> input_file_path = argv[2];

    if (args -> goal == UNKNOWN ||
            access(args -> input_file_path, R_OK) != 0) {
        free(args);
        FATAL_ERROR();
    }

    return args;
}