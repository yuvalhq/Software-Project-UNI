COMPILER := gcc
FLAGS := -ansi -Wall -Wextra -Werror -pedantic-errors
LINKER_FLAGS := -lm
DEBUG_FLAGS := -g -DDEBUG=1

compile: spkmeans.c strutils.c matrix.c jacobi.c
	$(COMPILER) $(FLAGS) spkmeans.c strutils.c matrix.c jacobi.c $(LINKER_FLAGS) -o spkmeans

compile_no_flags: spkmeans.c strutils.c matrix.c jacobi.c
	$(COMPILER) spkmeans.c strutils.c matrix.c jacobi.c $(LINKER_FLAGS) -o spkmeans

debug: spkmeans.c strutils.c matrix.c jacobi.c
	$(COMPILER) $(DEBUG_FLAGS) spkmeans.c strutils.c matrix.c jacobi.c $(LINKER_FLAGS) -o spkmeans