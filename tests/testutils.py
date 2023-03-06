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
    vec = []
    mat = []
    got_vec = False
    with open(file_path, "r") as fd:
        for line in fd.readlines():
            row = [float(x) for x in line.split(",")]
            if not got_vec:
                vec = row
                got_vec = True
            else:
                mat.append(row)
    return vec, mat


def print_matrix(matrix: Matrix) -> None:
    for row in matrix:
        print(",".join(f"{x:4f}" for x in row))
    print()