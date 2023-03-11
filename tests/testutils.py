from typing import List

Vector = List[float]
Matrix = List[List[float]]


def read_matrix_from_file(file_path: str):
    result = []
    with open(file_path, "r") as fd:
        for line in fd.readlines():
            row = [float(x) for x in line.split(",")]
            result.append(row)
    return result


def read_vector_and_matrix_from_file(file_path: str):
    mat = read_matrix_from_file(file_path)
    return mat[0], mat[1:]
