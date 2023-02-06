#include <stddef.h>

#define COMMA ','
#define NEWLINE '\n'
#define NUM_OF_ARGS 3
#define MAX_ROTATIONS 100
#define epsilon 0.00001

#define FATAL_ERROR() {\
    printf("An Error Has Occurred");\
    exit(EXIT_FAILURE);\
}

typedef double *Vector;
typedef Vector *Matrix;
typedef struct Coordinate {
    size_t i;
    size_t j;
} Coordinate;
typedef struct JacobiParameters {
    double theta;
    double c;
    double s;
    double t;
} JacobiParameters;
typedef struct JacobiResult {
    Vector eigenvalues;
    Matrix eigenvectors;
} JacobiResult;
