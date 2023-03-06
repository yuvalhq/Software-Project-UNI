#ifndef SPKMEANS_H
#define SPKMEANS_H

#include <stddef.h>
#include "matrix.h"

typedef enum Goal {
    WAM, DDG, GL, JACOBI, UNKNOWN
} Goal;

typedef struct CommandLineArguments {
    enum Goal goal;
    char *input_file_path;
} CommandLineArguments;

static char *goal_names[] = {"wam", "ddg", "gl", "jacobi", "unknown", NULL};

static Goal create_goal_from_name(char *goal_name);
static CommandLineArguments* handle_args(int argc, char *argv[]);

#endif