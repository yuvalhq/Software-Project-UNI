#include "pyutils.h"

Vector from_python_vector(PyObject *python_vector) {
    Py_ssize_t i;
    Py_ssize_t n = PyList_Size(python_vector);
    Vector result = (Vector) malloc(sizeof(double) * n);

    for (i = 0; i < n; i++) {
        PyObject *item = PyList_GetItem(python_vector, i);
        result[i] = PyFloat_AsDouble(item);
    }

    return result;
}

PyObject *to_python_vector(Vector vector, Py_ssize_t n) {
    Py_ssize_t i;
    PyObject *res = PyList_New(n);

    for (i = 0; i < n; i++) {
        PyList_SetItem(res, i, PyFloat_FromDouble(vector[i]));
    }

    return res;
}

Matrix from_python_matrix(PyObject *python_matrix) {
    Py_ssize_t n = PyList_Size(python_matrix);
    Py_ssize_t i;
    PyObject *vector_lst = NULL;
    Matrix result = (Matrix) malloc(sizeof(Vector) * n);

    for (i = 0; i < n; i++) {
        vector_lst = PyList_GetItem(python_matrix, i);
        result[i] = from_python_vector(vector_lst);
    }

    return result;
}

PyObject *to_python_matrix(Matrix matrix, Py_ssize_t n, Py_ssize_t m) {
    PyObject *result = PyList_New(n);
    PyObject *vector_lst = NULL;
    Py_ssize_t i;

    for (i = 0; i < n; i++) {
        vector_lst = to_python_vector(matrix[i], m);
        PyList_SetItem(result, i, vector_lst);
    }

    return result;
}

void convert_python_float_list_to_double_array(PyObject *float_list, double arr[]) {
    Py_ssize_t i;
    Py_ssize_t n = PyList_Size(float_list);

    for (i = 0; i < n; i++) {
        PyObject *item = PyList_GetItem(float_list, i);
        arr[i] = PyFloat_AsDouble(item);
    }
}

void convert_double_array_to_python_float_list(PyObject *float_list, double arr[], Py_ssize_t n) {
    Py_ssize_t i;

    for (i = 0; i < n; i++) {
        PyList_SetItem(float_list, i, PyFloat_FromDouble(arr[i]));
    }
}
