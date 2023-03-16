import sys
from dataclasses import dataclass
from enum import Enum, auto
from pathlib import Path
from typing import List, Optional

import numpy as np

import mykmeanssp

from kmeanspp import kmeanspp

Vector = List[float]
Matrix = List[Vector]


class Goal(Enum):
    SPK = auto()
    WAM = auto()
    DDG = auto()
    GL = auto()
    JACOBI = auto()

    @classmethod
    def from_lowercase_string(cls, goal_str: str) -> "Goal":
        return cls[goal_str.upper()]


@dataclass
class CommandLineArguments:
    k: Optional[int]
    goal: Goal
    file_path: Path


def handle_args() -> CommandLineArguments:
    if len(sys.argv) == 3:
        return CommandLineArguments(
            k=None,
            goal=Goal.from_lowercase_string(sys.argv[1]),
            file_path=Path(sys.argv[2]),
        )
    elif len(sys.argv) == 4:
        return CommandLineArguments(
            k=int(sys.argv[1]),
            goal=Goal.from_lowercase_string(sys.argv[2]),
            file_path=Path(sys.argv[3]),
        )
    sys.exit("An Error Has Ocurred")

       
def read_matrix_from_file(file_path: Path) -> Matrix:
    result = []
    with open(file_path, "r") as fd:
        for line in fd.readlines():
            row = [float(x) for x in line.split(",")]
            result.append(row)
    return result


def print_matrix(matrix: Matrix) -> None:
    for row in matrix:
        print_vector(row)


def print_vector(vector: Vector) -> None:
    print(",".join(f"{x:.4f}" for x in vector))


def print_int_list(integers: List[int]) -> None:
    print(",".join(str(x) for x in integers))


def main():
    goal_map = {
        Goal.WAM: mykmeanssp.wam,
        Goal.DDG: mykmeanssp.ddg,
        Goal.GL: mykmeanssp.gl,
        Goal.JACOBI: mykmeanssp.jacobi,
    }
    np.random.seed(0)

    cmd_args = handle_args()
    input_matrix = read_matrix_from_file(cmd_args.file_path)
    if cmd_args.goal == Goal.SPK:
        u = mykmeanssp.spk(input_matrix, cmd_args.k)
        result, centroids_idxs = kmeanspp(u, cmd_args.k)
        output = result.tolist()
        print_int_list(centroids_idxs)

    elif cmd_args.goal == Goal.JACOBI:
        eigenvectors, eigenvalues = mykmeanssp.jacobi(input_matrix)
        output = np.array(eigenvectors).T.tolist()
        print_vector(eigenvalues)

    else:
        output = goal_map[cmd_args.goal](input_matrix)
    print_matrix(output)


if __name__ == "__main__":
    main()
