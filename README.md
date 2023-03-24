## Project Setup
To develop in this project, it's recommended to have Docker installed, and use the development Docker image we created.
The repository includes a `devcontainer.json` file which instructs VSCode to start the container when launched.
More info about this tool [here](https://code.visualstudio.com/docs/devcontainers/containers).

In any case, if you want to work locally without using the development Docker image, you must have the following prerequisites:
- `Python3` (our image includes version `3.10.6`).
- `make` (our image includes version `4.3`).
- `pipenv` (our image includes version `2023.2.18`).

## External Tools Used
In this project we used some neat external tools:
- `Docker` and `DevContainers` - for easy development workspace setup.
- `pipenv` - for easily Python dependency management and version locking (configuration [here](Pipfile)).
- `pre-commit` - for nice linting and formatting (configuration [here](.pre-commit-config.yaml)). To install the hooks and run them manually, please run:
```bash
pre-commit install --install-hooks
pre-commit run --all-files
```
- `GitHub Actions` - for continuously running the pre-commit hooks (configuration [here](.github/workflows/pre-commit.yaml)).
- `clang-format` - An auto-formatting tool for C. Runs automatically by `pre-commit`, and you can run manually by running:
```bash
make run-clang-format
```
- `black` - An auto-formatting tool for Python. Runs automatically by `pre-commit`, and you can run manually by running:
```bash
make run-black
```
- `isort` - An auto-formatting tool for Python import lines. Doesn't run by `pre-commit` (you can read in the `pre-commit` configuration why we decided to exclude it), so you need to run it manually by running:
```bash
make run-isort
```
- `mypy` - A static type checker for Python. Runs automatically by `pre-commit`, and you can run manually by running:
```bash
make run-mypy
```

## Compiling and running the project
To compile the spkmeans binary and run it, you can execute:
```bash
make build
bin/spkmeans
```

To compile the C extension and run `spkmeans.py`, you can run:
```bash
make build-python-extension
pipenv install
pipenv run python3 src/spkmeans_python/spkmeans.py
```

> **Note**
> The extension's `so` file will be placed in the top level directory, so you must make sure it's importable from your Python code.

To run Valgrind on the compiled binary, you must make sure that it's compiled with the debug symbols. If you already built the binary, it's encouraged to run `make clean`, and then you can run:
```bash
args="jacobi test1.txt" make valgrind
```

To delete all artifacts, you can run:
```bash
make clean
```
