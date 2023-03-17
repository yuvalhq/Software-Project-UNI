#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include "pyutils.h"
#include "spectral.h"
#include "jacobi.h"

static PyObject* wam_wrapper(PyObject *self, PyObject *args) {
    Py_ssize_t m, n;
    PyObject *data_points = NULL;
    PyObject *res = NULL;
    Matrix mat = NULL;
    Matrix wam = NULL;

    if (!PyArg_ParseTuple(args, "O", &data_points)) {
        return NULL;
    }

    n = PyList_Size(data_points);
    m = PyList_Size(PyList_GetItem(data_points, 0));
    mat = from_python_matrix(data_points);

    wam = weighted_adjacency_matrix(mat, n, m);
    res = to_python_matrix(wam, n, n);

    free_matrix(mat, n);
    free_matrix(wam, n);

    return res;
}

static PyObject* ddg_wrapper(PyObject *self, PyObject *args) {
    Py_ssize_t m, n;
    PyObject *data_points = NULL;
    PyObject *res = NULL;
    Matrix mat = NULL;
    Matrix wam = NULL;
    Matrix ddg = NULL;

    if (!PyArg_ParseTuple(args, "O", &data_points)) {
        return NULL;
    }

    n = PyList_Size(data_points);
    m = PyList_Size(PyList_GetItem(data_points, 0));
    mat = from_python_matrix(data_points);

    wam = weighted_adjacency_matrix(mat, n, m);
    ddg = diagonal_degree_matrix(wam, n);

    res = to_python_matrix(ddg, n, n);

    free_matrix(mat, n);
    free_matrix(wam, n);
    free_matrix(ddg, n);

    return res;
}

static PyObject* gl_wrapper(PyObject *self, PyObject *args) {
    Py_ssize_t m, n;
    PyObject *data_points = NULL;
    PyObject *res = NULL;
    Matrix mat = NULL;
    Matrix wam = NULL;
    Matrix ddg = NULL;
    Matrix gl = NULL;

    if (!PyArg_ParseTuple(args, "O", &data_points)) {
        return NULL;
    }

    n = PyList_Size(data_points);
    m = PyList_Size(PyList_GetItem(data_points, 0));
    mat = from_python_matrix(data_points);

    wam = weighted_adjacency_matrix(mat, n, m);
    ddg = diagonal_degree_matrix(wam, n);
    gl = graph_laplacian(ddg, wam, n);

    res = to_python_matrix(gl, n, n);

    free_matrix(mat, n);
    free_matrix(wam, n);
    free_matrix(ddg, n);
    free_matrix(gl, n);

    return res;
}

static PyObject* jacobi_wrapper(PyObject *self, PyObject *args) {
    Py_ssize_t n;
    PyObject *data_points = NULL;
    Matrix mat = NULL;
    JacobiResult *jacobi_result = NULL;

    PyObject *res = NULL;
    PyObject *eigenvectors = NULL;
    PyObject *eigenvalues = NULL;

    if (!PyArg_ParseTuple(args, "O", &data_points)) {
        return NULL;
    }

    n = PyList_Size(data_points);
    mat = from_python_matrix(data_points);
    jacobi_result = jacobi(mat, n);

    eigenvectors = to_python_matrix(jacobi_result -> eigenvectors, n, n);
    eigenvalues = to_python_vector(jacobi_result -> eigenvalues, n);
    res = PyTuple_New(2);
    PyTuple_SetItem(res, 0, eigenvectors);
    PyTuple_SetItem(res, 1, eigenvalues);

    free_matrix(mat, n);
    free_matrix(jacobi_result -> eigenvectors, n);
    free(jacobi_result -> eigenvalues);
    free(jacobi_result);

    return res;
}

static PyObject* spk_wrapper(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject *data_points_py = NULL;
    PyObject *optional_k = Py_None;
    PyObject *res = NULL;

    Py_ssize_t k;
    SpectralResult *spr = NULL;

    static char* kwlist[] = {"data_points", "k", NULL};
    if (!PyArg_ParseTupleAndKeywords(
            args,
            kwargs,
            "O|O",
            kwlist,
            &data_points_py, &optional_k)) {
        return NULL;
    }

    if (optional_k == Py_None) {
        k = 0;
    } else if (PyLong_Check(optional_k)) {
        k = PyLong_AsLong(optional_k);
    } else {
        PyErr_SetString(PyExc_TypeError, "K must be an int or None");
        return NULL;
    }

    Py_ssize_t n = PyList_Size(data_points_py);
    Py_ssize_t m = PyList_Size(PyList_GetItem(data_points_py, 0));
    if (k > n) {
        PyErr_SetString(PyExc_ValueError, "k can't be larger than n");
        return NULL;
    }

    Matrix data_points_c = from_python_matrix(data_points_py);
    spr = spectral_clustering(data_points_c, k, n, m);
    res = PyTuple_New(2);
    PyTuple_SetItem(res, 0, to_python_matrix(spr -> new_points, spr -> k, n));
    PyTuple_SetItem(res, 1, PyLong_FromLong(spr -> k));

    free_matrix(data_points_c, n);
    free(spr);
    return res;
}

static PyMethodDef spkmeans_methods[] = {
    {
        .ml_name = "wam",
        .ml_meth = (PyCFunction) wam_wrapper,
        .ml_flags = METH_VARARGS,
        .ml_doc = PyDoc_STR(
            "wam(data_points)\n"
            "--\n"
            "Receives a list of points and converts them to a weighted adjacency matrix, "
            "which is defined by:\n"
            "w_ij = exp(-||x_i - x_j||^2 / 2) if i != j, and w_ii = 0\n"
            "We denote by ||_||^2 the Squared Euclidean Distance.\n\n"
            "Parameters\n"
            "----------\n"
            "datapoints:\n"
            "    The datapoints to calculate the matrix of."
        )
    },
    {
        .ml_name = "ddg",
        .ml_meth = (PyCFunction) ddg_wrapper,
        .ml_flags = METH_VARARGS,
        .ml_doc = PyDoc_STR(
            "ddg(data_points)\n"
            "--\n"
            "Receives a list of points and converts them to a weighted adjacency matrix, "
            "and then calculates the diagonal degree matrix.\n"
            "The matrix is defined as the diagonal matrix with the degrees d_1,..,d_n on the diagonal "
            "and zero elsewhere. The degree of a vertex x_i \\in X is defined as:\n"
            "d_i = sum_{j=1}^{n}(w_ij)\n\n"
            "Parameters\n"
            "----------\n"
            "datapoints:\n"
            "    The datapoints to calculate the matrix of."
        )
    },
    {
        .ml_name = "gl",
        .ml_meth = (PyCFunction) gl_wrapper,
        .ml_flags = METH_VARARGS,
        .ml_doc = PyDoc_STR(
            "gl(data_points)\n"
            "--\n"
            "Receives a list of points and converts calculates the graph Laplacian of them.\n"
            "The graph L \\in R^{n*n} is defined as L = D - W, while D and W are the DDG and WAM respectively.\n\n"
            "Parameters\n"
            "----------\n"
            "datapoints:\n"
            "    The datapoints to calculate the matrix of."
        )
    },
    {
        .ml_name = "jacobi",
        .ml_meth = (PyCFunction) jacobi_wrapper,
        .ml_flags = METH_VARARGS,
        .ml_doc = PyDoc_STR(
            "jacobi(matrix)\n"
            "--\n"
            "Receives a matrix and runs the Jacobi algorithm on it to calculate the eigenvalues and eigenvectors.\n\n"
            "Parameters\n"
            "----------\n"
            "Matrix:\n"
            "    The matrix to calculate the eigenvalues and eigenvectors of."
        )
    },
    {
        .ml_name = "spk",
        .ml_meth = (PyCFunction) spk_wrapper,
        .ml_flags = METH_VARARGS | METH_KEYWORDS,
        .ml_doc = PyDoc_STR(
            "spk(data_points)\n"
            "--\n"
            "Receives datapoints and k and runs the spectral clustering algorithm on them.\n"
            "The return value of the function is the new points which will be the input of the k-means++ algorithm.\n"
            "If k < 1, the function will use the eigengap heuristic to determine the best k value, and if "
            "k > n, the return value will be None.\n\n"
            "Parameters\n"
            "----------\n"
            "datapoints:\n"
            "    The datapoints to calculate spectral clustering on."
        )
    },
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef spkmeans_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "mykmeanssp",
    .m_doc = NULL,
    .m_size = -1,
    .m_methods = spkmeans_methods
};

PyMODINIT_FUNC PyInit_mykmeanssp(void) {
    PyObject *module = PyModule_Create(&spkmeans_module);

    if (!module) {
        return NULL;
    }

    return module;
}
