#include <stddef.h>

Matrix build_matrix(size_t n);
Matrix build_identity_mat(size_t n);
void free_matrix(Matrix mat, size_t n);
void print_matrix(Matrix mat, size_t n, size_t m);
void print_vector(Vector vector, size_t m);
Matrix get_mat_from_file(char *filename, size_t *n, size_t *m);