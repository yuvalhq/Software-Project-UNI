from typing import List

import mykmeanssp
import numpy as np
import pytest
from testutils import *

Matrix = List[List[float]]

param = [
    "inputs_folder, others_answer_folder",
    [
        (
            "testfiles/test",
            "testfiles/ofek_s/test", # name is a place holder
        ),
    ],
]

@pytest.mark.parametrize(param[0], param[1])
def test_wam(
    inputs_folder: str, others_answer_folder: str
):
    for i in range(1, 11):
        our_mat = read_matrix_from_file(inputs_folder + str(i) + ".txt")
        
        our_wam = mykmeanssp.wam(our_mat)
        other_wam = read_matrix_from_file(others_answer_folder + str(i) + "_wam.txt")
        #other_wam = mykmeanssp.wam(our_mat)
        np.testing.assert_almost_equal(our_wam, other_wam, decimal=3)


@pytest.mark.parametrize(param[0], param[1])
def test_ddg(
    inputs_folder: str, others_answer_folder: str
):
    for i in range(1, 11):
        our_mat = read_matrix_from_file(inputs_folder + str(i) + ".txt")
        
        our_ddg = mykmeanssp.ddg(our_mat)
        other_ddg = read_matrix_from_file(others_answer_folder + str(i) + "_ddg.txt")
        #other_ddg = mykmeanssp.ddg(our_mat)
        np.testing.assert_almost_equal(our_ddg, other_ddg, decimal=3)
        

@pytest.mark.parametrize(param[0], param[1])       
def test_gl(
    inputs_folder: str, others_answer_folder: str
):
    for i in range(1, 11):
        our_mat = read_matrix_from_file(inputs_folder + str(i) + ".txt")
        
        our_gl = mykmeanssp.gl(our_mat)
        other_gl = read_matrix_from_file(others_answer_folder + str(i) + "_gl.txt")
        #other_gl = mykmeanssp.gl(our_mat)
        np.testing.assert_almost_equal(our_gl, other_gl, decimal=3)
        

@pytest.mark.parametrize(param[0], param[1])      
def test_jacobi(
    inputs_folder: str, others_answer_folder: str
):
    for i in range(1, 11):
        our_mat = read_matrix_from_file(inputs_folder + str(i) + "_j.txt")
        
        our_eigenvectors, our_eigenvalues = mykmeanssp.jacobi(our_mat)
        other_eigenvalues, other_eigenvectors = read_vector_and_matrix_from_file(others_answer_folder + str(i) + "_j_ans.txt")
        #other_eigenvectors, other_eigenvalues = mykmeanssp.jacobi(our_mat)
        np.testing.assert_almost_equal(our_eigenvectors, other_eigenvectors, decimal=3)
        np.testing.assert_almost_equal(our_eigenvalues, other_eigenvalues, decimal=3)

