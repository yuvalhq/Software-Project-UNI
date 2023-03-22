#ifndef PYUTILS_H
#define PYUTILS_H

#include <Python.h>
#include "vector.h"
#include "matrix.h"

Vector from_python_vector(PyObject *python_vector);
PyObject *to_python_vector(Vector vector, Py_ssize_t n);

Matrix from_python_matrix(PyObject *python_matrix);
PyObject *to_python_matrix(Matrix matrix, Py_ssize_t n, Py_ssize_t m);

void convert_python_float_list_to_double_array(PyObject *float_list, double arr[]);
void convert_double_array_to_python_float_list(PyObject *float_list, double arr[], Py_ssize_t n);

#endif
