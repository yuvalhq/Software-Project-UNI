#include <stddef.h>

JacobiResult *jacobi(Matrix mat, size_t n);
Coordinate *get_pivot_coord(Matrix mat, size_t n);
JacobiParameters *get_jacobi_parameters(Matrix a, Coordinate pivot);
Matrix transform(Matrix mat, Coordinate pivot, double c, double s, size_t n);
Matrix mat_mul(Matrix mat1, Matrix mat2, size_t n);
double off_sq_diff(Matrix mat1, Matrix mat2, size_t n);
double off_sq(Matrix mat, size_t n);
Vector get_eigenvalues_from_diagonal_matrix(Matrix mat, size_t n);
Matrix build_rotation_matrix(Coordinate *pivot, JacobiParameters *jacobi_params, size_t n);