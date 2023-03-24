PROJECT_NAME            := spkmeans

OS                      := $(shell uname -s)
SHELL                   := bash
COLOR_PREFIX            := e

ifeq ($(OS),Darwin)
	COLOR_PREFIX        := 033
endif

BROWN                   :=\$(COLOR_PREFIX)[0;33m
BLUE                    :=\$(COLOR_PREFIX)[1;34m
END_COLOR               :=\$(COLOR_PREFIX)[0m

CC                      := gcc
CFLAGS                  := -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS                    := -lm
DEBUG_FLAGS             := -g -DDEBUG=1

PYTHON_EXTENSION_DIR    := src/spkmeans_extension
PYTHON_EXTENSION_H      := $(shell find $(PYTHON_EXTENSION_DIR) -name '*.h')
PYTHON_EXTENSION_C      := $(shell find $(PYTHON_EXTENSION_DIR) -name '*.c')
PYTHON_EXTENSION_SETUP  := $(PYTHON_EXTENSION_DIR)/setup.py

PYTHON_SRC_DIR          := src/spkmeans_python

SRCDIR                  := src/spkmeans_c
BINDIR                  := bin
LIBSDIR                 := libs
TESTSDIR                := tests
SRCEXT                  := c

MAIN                    := $(BINDIR)/$(PROJECT_NAME)
SRC_NAMES               := $(notdir $(basename $(wildcard $(SRCDIR)/*.$(SRCEXT))))
SRC_HEADERS             := $(shell find $(SRCDIR) -name '*.h')
SRC_OBJECTS             := $(patsubst %,$(BINDIR)/%.o,$(SRC_NAMES))

TESTS_NAME              := $(PROJECT_NAME)-tests
TESTS_MAIN              := $(BINDIR)/$(TESTS_NAME)
TESTS_MAIN_C            := $(TESTSDIR)/$(PROJECT_NAME)_tests.c
TESTS_FLAGS             := -std=c99
TESTS_LIB_C             := $(LIBSDIR)/munit.c
TESTS_LIB_H             := $(LIBSDIR)/munit.h
TESTS_LIB_OBJECT        := $(BINDIR)/munit.o
TESTS_MAIN_OBJECT       := $(BINDIR)/$(PROJECT_NAME)_tests.o

DEBUG_OBJECTS           := $(patsubst %.o,%-debug.o,$(SRC_OBJECTS))
DEBUG_MAIN              := $(MAIN)-debug

.PHONY: build build-python-extension build-tests \
	build-debug build-devel-image \
	valgrind clean generate-requirements \
	run-tests run-pytest \
	run-clang-format run-black run-isort \
	pre-commit

build: $(SRC_OBJECTS)
	@echo -en "$(BROWN)LD $(END_COLOR)";
	$(CC) -o $(MAIN) $+ $(CFLAGS) $(LIBS)
	@echo -en "\n--\nBinary file placed at" \
			  "$(BROWN)$(MAIN)$(END_COLOR)\n";

build-python-extension: $(PYTHON_EXTENSION_C) $(PYTHON_EXTENSION_H) $(PYTHON_EXTENSION_SETUP)
	@echo -en "$(BROWN)Building setup.py $(END_COLOR)";
	python3 $(PYTHON_EXTENSION_SETUP) build_ext --inplace

build-tests: $(TESTS_MAIN_OBJECT) $(TESTS_LIB_OBJECT) $(filter-out $(MAIN).o,$(SRC_OBJECTS))
	@echo -en "$(BROWN)LD $(END_COLOR)";
	$(CC) -o $(TESTS_MAIN) $+ $(CFLAGS) $(LIBS)
	@echo -en "\n--\nBinary file placed at" \
			  "$(BROWN)$(TESTS_MAIN)$(END_COLOR)\n";

build-debug: $(DEBUG_OBJECTS)
	@echo -en "$(BROWN)LD $(END_COLOR)";
	$(CC) -o $(DEBUG_MAIN) $+ $(DEBUG_FLAGS) $(CFLAGS) $(LIBS)
	@echo -en "\n--\nBinary file placed at" \
			  "$(BROWN)$(DEBUG_MAIN)$(END_COLOR)\n";

build-devel-image: Dockerfile generate-requirements
	@echo -e "$(BROWN)Building Docker image $(END_COLOR)";
	docker build -t mykmeanssp-devel:1.0.0 .

$(BINDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(SRC_HEADERS)
	@echo -en "$(BROWN)CC $(END_COLOR)";
	$(CC) -c $(firstword $^) -o $@ $(CFLAGS) $(LIBS)

$(TESTS_MAIN_OBJECT): $(TESTS_MAIN_C) $(TESTS_LIB_C) $(TESTS_LIB_H)
	@echo -en "$(BROWN)CC $(END_COLOR)";
	$(CC) -c $(firstword $^) -I $(LIBSDIR) -I $(SRCDIR) -o $@ $(CFLAGS) $(LIBS) $(TESTS_FLAGS)

$(TESTS_LIB_OBJECT): $(TESTS_LIB_C) $(TESTS_LIB_H)
	@echo -en "$(BROWN)CC $(END_COLOR)";
	$(CC) -c $(firstword $^) -I $(LIBSDIR) -o $@ $(CFLAGS) $(LIBS) $(TESTS_FLAGS)

$(BINDIR)/%-debug.o: $(SRCDIR)/%.$(SRCEXT) $(SRC_HEADERS)
	@echo -en "$(BROWN)CC $(END_COLOR)";
	$(CC) -c $(firstword $^) -o $@ $(DEBUG) $(CFLAGS) $(LIBS)

valgrind: build-debug
ifndef args
	@echo -en "$(BROWN)You can pass program args with args=\"...\"$(END_COLOR)\n\n--\n"
endif
	valgrind \
		--track-origins=yes \
		--leak-check=full \
		--show-leak-kinds=all \
		--leak-resolution=high \
		$(DEBUG_MAIN) $(args)

clean:
	@find -name __pycache__ \
		-o -name .mypy_cache \
		-o -name build \
		-o -name .pytest_cache \
		-o -name *.egg-info \
		-o -name *.so \
		| xargs rm -rfv

generate-requirements: Pipfile
	@echo -e "$(BROWN)Generating requirements $(END_COLOR)";
	@pipenv requirements > requirements.txt
	@pipenv requirements --dev > requirements-dev.txt

run-mypy:
	python3 -m mypy .

run-tests: build-tests
	./$(TESTS_MAIN)

run-pytest: build-python-extension
	python3 -m pytest -sv .

run-clang-format:
	find src tests -name *.c -o -name *.h -exec clang-format -i {} \+

run-black:
	python3 -m black .

run-isort:
	python3 -m isort .

pre-commit:
	@which pre-commit &>/dev/null || (echo "ERROR: Please install pre-commit first" ; exit 1)
	pre-commit install
