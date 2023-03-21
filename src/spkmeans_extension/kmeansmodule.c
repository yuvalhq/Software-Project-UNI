#define PY_SSIZE_T_CLEAN
#include <stdio.h>
#include <Python.h>
#include "kmeans.h"

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

static PyObject* kmeans_fit_wrapper(PyObject *self, PyObject *args) {
    PyObject *centroids_lst, *vectors_lst, *res;
    Py_ssize_t iter = DEFAULT_ITERATIONS_COUNT, vectors_count, vector_size, k, i;
    double epsilon = DEFAULT_EPSILON;
    Cluster *clusters;
    Vector *vectors;

    if (!PyArg_ParseTuple(args, "OOn|n|d", &centroids_lst, &vectors_lst, &k, &iter, &epsilon)) {
        return NULL;
    }

    res = PyList_New(k);
    vectors_count = PyList_Size(vectors_lst);
    vector_size = PyList_Size(PyList_GetItem(vectors_lst, 0));

    vectors = (Vector *) malloc(sizeof(Vector) * vectors_count);
    clusters = (Cluster *) malloc(sizeof(Cluster) * k);
    
    for (i = 0; i < vectors_count; i++) {
        vectors[i] = (Vector) malloc(sizeof(double) * vector_size);
        PyObject *vector_lst = PyList_GetItem(vectors_lst, i);
        convert_python_float_list_to_double_array(vector_lst, vectors[i]);
    }

    for (i = 0; i < k; i++) {
        clusters[i].centroid = (Vector) malloc(sizeof(double) * vector_size);
        PyObject *cluster_lst = PyList_GetItem(centroids_lst, i);
        convert_python_float_list_to_double_array(cluster_lst, clusters[i].centroid);
    }

    fit(&clusters, vectors_count, vectors, vector_size, k, iter, epsilon);

    for (i = 0; i < vectors_count; i++) {
        if (i < k) {
            PyObject *lst = PyList_New(vector_size);
            convert_double_array_to_python_float_list(lst, clusters[i].centroid, vector_size);
            PyList_SetItem(res, i, lst);
            free(clusters[i].centroid);
            clusters[i].centroid = NULL;
        }
        free(vectors[i]);
        vectors[i] = NULL;
    }

    free(clusters);
    free(vectors);
    clusters = NULL;
    vectors = NULL;

    return res;
}

static PyMethodDef kmeans_methods[] = {
    {
        .ml_name = "fit",
        .ml_meth = (PyCFunction) kmeans_fit_wrapper,
        .ml_flags = METH_VARARGS,
        .ml_doc = PyDoc_STR(
            "fit(centroids_lst, vectors_lst, k, iter=300, epsilon=0.001)\n"
            "--\n"
            "\n"
            "Fits the Kmeans model. The centroids and vectors are represented as a list vectors, "
            "each represented as a list of floats.\n\n"
            "Parameters\n"
            "----------\n"
            "centroids_lst:\n"
            "    The list of initial centroids.\n"
            "vectors_lst:\n"
            "    The list vectors to partition to different clusters.\n"
            "k:\n"
            "    The number of clusters to partition.\n"
            "iter:\n"
            "    The number of iterations of the algorithm to run.\n"
            "epsilon:\n"
            "    Epsilon value used for convergence."
        )
    },
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef kmeans_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "kmeans_c",
    .m_doc = NULL,
    .m_size = -1,
    .m_methods = kmeans_methods
};

PyMODINIT_FUNC PyInit_kmeans_c(void) {
    PyObject *module = PyModule_Create(&kmeans_module);
    PyObject *default_epsilon = PyFloat_FromDouble(DEFAULT_EPSILON);

    if (!module) {
        return NULL;
    }

    PyModule_AddIntConstant(module, "DEFAULT_ITERATIONS_COUNT", DEFAULT_ITERATIONS_COUNT);
    PyModule_AddObject(module, "DEFAULT_EPSILON", default_epsilon);

    return module;
}