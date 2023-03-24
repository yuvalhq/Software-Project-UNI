#ifndef PYUTILS_H
#define PYUTILS_H

#include "matrix.h"
#include "vector.h"
#include <Python.h>

Vector from_python_vector(PyObject *python_vector);
PyObject *to_python_vector(Vector vector, Py_ssize_t n);

Matrix from_python_matrix(PyObject *python_matrix);
PyObject *to_python_matrix(Matrix matrix, Py_ssize_t n, Py_ssize_t m);

#endif
