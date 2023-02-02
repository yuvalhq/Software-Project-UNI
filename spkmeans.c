#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMA ','
#define NEWLINE '\n'

#ifndef DEBUG
#define DEBUG false
#endif

#define FATAL_ERROR() {\
    printf("An Error Has Occurred");\
    exit(EXIT_FAILURE);\
}

typedef double *Vector;
typedef Vector *Matrix;
typedef struct JacobiResult {
    Vector eigenvalues;
    Matrix eigenvectors;
} JacobiResult;
typedef struct Coordinate {
    size_t i;
    size_t j;
} Coordinate;

static const int NUM_OF_ARGS = 3;
static const size_t MAX_ROTATIONS = 100;
static const double epsilon = 0.00001;

void handle_args(int argc, char *argv[]);
Matrix build_mat(size_t n, int init_zeros);
Vector build_vector(size_t n, int init_zeros);
Matrix build_identity_mat(size_t n);
void free_mat(Matrix mat, size_t n);
void print_matrix(Matrix mat, size_t n, size_t m);
void print_vector(Vector vector, size_t m);
JacobiResult jacobi(Matrix mat, size_t n);
Coordinate get_pivot_coord(Matrix mat, size_t n);
Matrix transform(Matrix mat, Coordinate pivot, double c, double s, size_t n);
Matrix mat_mul(Matrix mat1, Matrix mat2, size_t n);
double off_sq_diff(Matrix mat1, Matrix mat2, size_t n);
double off_sq(Matrix mat, size_t n);
Vector get_eigenvalues(Matrix mat, size_t n);
Matrix gl(Matrix w, Matrix d, size_t n);
Matrix ddg(Matrix w, size_t n);
Matrix wam(Matrix mat, size_t n, size_t m);
double sq_euclidean_distance(Vector p, Vector q, size_t m);
Matrix get_mat_from_file(char *filename, size_t *n, size_t *m);
size_t strcount(char *str, char c);


int main(int argc, char *argv[]) {
    size_t n = 0, m = 0;
    Matrix input, ww, dd, ll;
    JacobiResult rr;
    
    handle_args(argc, argv);

    input = get_mat_from_file(argv[2], &n, &m);
    ww = wam(input, n, m);
    dd = ddg(ww, n);
    ll = gl(ww, dd, n);
    rr = jacobi(ll, n);

    printf("\n");
    print_matrix(rr.eigenvectors, n, n);
    printf("\n");
    print_vector(rr.eigenvalues, n);
    printf("\n");

    free_mat(input, n);
    free_mat(ww, n);
    free_mat(dd, n);
    free_mat(ll, n);
    free_mat(rr.eigenvectors, n);
    free(rr.eigenvalues);
    return EXIT_SUCCESS;
}


void handle_args(int argc, char *argv[]) {
    size_t i;
    int not_found = 1;
    char *goals[] = {"wam", "ddg", "gl", "jacobi"};
    FILE *file;

    if (argc != NUM_OF_ARGS) {
        FATAL_ERROR();
    }

    for (i=0; i<4; i++) {
        if (strcmp(argv[1], goals[i]) == 0) {
            not_found = 0;
        }
    }
    if (not_found) {
        FATAL_ERROR();
    }

    file = fopen(argv[2], "r");
    if (file == NULL) {
        FATAL_ERROR()
    }
    fclose(file);
}


Matrix build_mat(size_t n, int init_zeros) {
    size_t i;
    Matrix mat = (Matrix) malloc(n * sizeof(Vector));
    for (i=0; i<n; i++) {
        if (init_zeros) {
            mat[i] = (Vector) calloc(n, sizeof(double));
            } else {
            mat[i] = (Vector) malloc(n * sizeof(double)); 
        }
    }
    return mat;
}


Vector build_vector(size_t n, int init_zeros) {
    Vector vector;
    if (init_zeros) {
        vector = (Vector) calloc(n, sizeof(double));
    } else {
        vector = (Vector) malloc(n * sizeof(double)); 
    }
    return vector;
}


Matrix build_identity_mat(size_t n) {
    size_t i;
    Matrix mat = build_mat(n, 1);

    for (i=0; i<n; i++) {
        mat[i][i] = 1;
    }
    return mat;
}    


void free_mat(Matrix mat, size_t n) {
    size_t i;
    for (i=0; i<n; i++) {
        free(mat[i]);
    }
    free(mat);
}


void print_matrix(Matrix mat, size_t n, size_t m) {
    size_t i;
    for (i = 0; i < n; i++) {
        print_vector(mat[i], m);
        printf("\n");
    }
}


void print_vector(Vector vector, size_t m) {
    size_t i;
    for (i = 0; i < m; i++) {
        printf("%.4f", vector[i]);
        if (i < m - 1) {
            printf(",");
        }
    }
}


JacobiResult jacobi(Matrix mat, size_t n) {
    size_t i, j, iter;
    int pivoted;
    Coordinate pivot;
    double theta, t, c, s, convergence; 
    Matrix a;
    Matrix a_new;
    Matrix p;
    Matrix v;
    Matrix v_new;
    JacobiResult res;

    iter = 0;
    pivoted = 0;
    convergence = 1;
    a = mat;
    v = build_identity_mat(n);
    while(convergence > epsilon && iter < MAX_ROTATIONS) {
        pivot = get_pivot_coord(a, n);

        theta = (a[pivot.j][pivot.j] - a[pivot.i][pivot.i]) / (2 * a[pivot.i][pivot.j]);
        t = 1.0 / (fabs(theta) + sqrt(pow(theta, 2) + 1));
        if (theta < 0) {
            t = -t;
        }
        c = 1.0 / sqrt(pow(t, 2) + 1);
        s = t * c;

        a_new = transform(a, pivot, c, s, n);

        convergence = off_sq_diff(a, a_new, n);
        if (pivoted) {
            free_mat(a, n);
        }
        a = a_new;

        p = build_identity_mat(n);
        p[pivot.i][pivot.i] = c;
        p[pivot.i][pivot.j] = s;
        p[pivot.j][pivot.i] = -s;
        p[pivot.j][pivot.j] = c;
        v_new = mat_mul(v, p, n);
        free_mat(p, n);
        free_mat(v, n);
        v = v_new;

        pivoted = 1;
        iter++;
    }

    res.eigenvalues = get_eigenvalues(a, n);
    for (i=0; i<n; i++) {
        if (res.eigenvalues[i] == 0 && signbit(res.eigenvalues[i]) != 0) {
            res.eigenvalues[i] = 0;
            for (j=0; j<n; i++) {
               v[i][j] = -v[i][j];
            }
        }
    }
    res.eigenvectors = v;

    if (pivoted) {
        free_mat(a, n);
    }
    return res;
}


Coordinate get_pivot_coord(Matrix mat, size_t n) {
    size_t i, j;
    Coordinate res;
    double val, max_val = fabs(mat[0][1]);

    for (i=0; i<n; i++) {
        for (j=i+1; j<n; j++) {
            val = fabs(mat[i][j]);
            if (val > max_val) {
                res.i = i;
                res.j = j;
                max_val = val;
            }
        }
    }
    return res;
}


Matrix transform(Matrix mat, Coordinate pivot, double c, double s, size_t n) {
    size_t i, j;
    Matrix mat_new = build_mat(n, 0);

    for (i=0; i<n; i++) {
        for (j=i; j<n; j++) {
            if (i == pivot.i){
                if (j == pivot.j) {
                    mat_new[i][j] = 0;
                } else if (i == j){
                    mat_new[i][j] = (pow(c, 2)*mat[i][i])+(pow(s, 2)*mat[j][j])-(2*s*c*(mat[i][j]));
                } else {
                    mat_new[i][j] = (c*mat[i][j])+(s*mat[i][pivot.j]);
                }
            } else if (j == pivot.j) {
                if (i == j) {
                    mat_new[i][j] = (pow(s, 2)*mat[i][i])+(pow(c, 2)*mat[j][j])+(2*s*c*mat[i][j]);
                } else {
                    mat_new[i][j] = (c*mat[i][j])+(s*mat[i][pivot.i]);
                }
            } else {
                mat_new[i][j] = mat[i][j];
            }
        }
    }
    for (i=0; i<n; i++) {
        for (j=0; j<i; j++) {
            mat_new[i][j] = mat_new[j][i];
        }
    }
    return mat_new;
}


Matrix mat_mul(Matrix mat1, Matrix mat2, size_t n){
    size_t i, j, k;
    Matrix res;

    res = build_mat(n, 1);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) {
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    return res;
}


double off_sq_diff(Matrix mat1, Matrix mat2, size_t n) {
    double off1, off2;
    off1 = off_sq(mat1, n);
    off2 = off_sq(mat2, n);
    return (off1 - off2);
}


double off_sq(Matrix mat, size_t n) {
    size_t i, j;
    double res;

    for (i=0; i<n; i++) {
        for (j=i+1; j<n; j++) {
            res += (2 * pow(mat[i][j], 2));
        }
    }
    return res;
}


Vector get_eigenvalues(Matrix mat, size_t n){
    size_t i;
    Vector eigenvalues = build_vector(n, 0);

    for (i=0; i<n; i++) {
        eigenvalues[i] = mat[i][i];
    }
    return eigenvalues;
}


Matrix gl(Matrix w, Matrix d, size_t n) {
    size_t i, j;
    Matrix l = build_mat(n, 0);

    for (i=0; i<n; i++) {
        for (j=0; j<n; j++){
            l[i][j] = d[i][j] - w[i][j];
        }
    }
    return l;
}


Matrix ddg(Matrix w, size_t n) {
    size_t i, j;
    Matrix d = build_mat(n, 1);

    for (i=0; i<n; i++) {
        for (j=0; j<n; j++){
            d[i][i] += w[i][j];
        }
    }
    return d;
}


Matrix wam(Matrix mat, size_t n, size_t m) {
    size_t i, j;
    double dist;
    Matrix w = build_mat(n, 0);

    for (i=0; i<n; i++){
        for (j=i; j<n; j++){
            if (i == j) {
                w[i][j] = 0;
                continue;
            }

            dist = sq_euclidean_distance(mat[i], mat[j], m);
            w[i][j] = exp(-dist / 2);
        }
    }

    for (i=0; i<n; i++){
        for (j=0; j<i; j++){
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


Matrix get_mat_from_file(char *filename, size_t *n, size_t *m) {
    size_t i, line_len;
    char *line = NULL;
    char *line_idx;
    Vector vector;
    Matrix mat;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        FATAL_ERROR()
    }

    mat = (Matrix) malloc(sizeof(Vector));

    while (getline(&line, &line_len, file) != -1) {
        if (*m == 0) {
           *m = strcount(line, COMMA) + 1; 
        }

        vector = build_vector(*m, 0);
        line_idx = line;

        for (i = 0; i < (*m); i++) {
            vector[i] = strtod(line_idx, &line_idx);
            if (*line_idx != COMMA && *line_idx != NEWLINE) {
                FATAL_ERROR();
            }
            line_idx++;
        }

        mat = (Matrix) realloc(mat, (++(*n)) * sizeof(Vector));
        mat[(*n) - 1] = vector;
    }

    free(line);
    fclose(file);
    return mat;
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
