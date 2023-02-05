PROJECT_NAME  := spkmeans

OS            := $(shell uname -s)
SHELL         := bash
COLOR_PREFIX  := e

ifeq ($(OS),Darwin)
	COLOR_PREFIX := 033
endif

BROWN         :=\$(COLOR_PREFIX)[0;33m
BLUE          :=\$(COLOR_PREFIX)[1;34m
END_COLOR     :=\$(COLOR_PREFIX)[0m

CC            := gcc
CFLAGS        := -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS          := -lm
DEBUG_FLAGS   := -g -DDEBUG=1

SRCDIR        := src
BINDIR        := bin
LOGDIR        := log
SRCEXT        := c

MAIN          := $(BINDIR)/$(PROJECT_NAME)

NAMES         := $(notdir $(basename $(wildcard $(SRCDIR)/*.$(SRCEXT))))
HEADERS       := $(shell find $(SRCDIR) -name '*.h')
OBJECTS       :=$(patsubst %,$(BINDIR)/%.o,$(NAMES))

.PHONY: compile debug valgrind clean

compile: $(OBJECTS)
	@echo -en "$(BROWN)LD $(END_COLOR)";
	$(CC) -o $(MAIN) $+ $(CFLAGS) $(LIBS)
	@echo -en "\n--\nBinary file placed at" \
			  "$(BROWN)$(MAIN)$(END_COLOR)\n";

debug: $(OBJECTS)
	@echo -en "$(BROWN)LD $(END_COLOR)";
	$(CC) -o $(MAIN) $+ $(DEBUG_FLAGS) $(CFLAGS) $(LIBS)
	@echo -en "\n--\nBinary file placed at" \
			  "$(BROWN)$(MAIN)$(END_COLOR)\n";

$(BINDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(HEADERS)
	@echo -en "$(BROWN)CC $(END_COLOR)";
	$(CC) -c $(firstword $^) -o $@ $(DEBUG) $(CFLAGS) $(LIBS)

valgrind: debug
ifndef args
	@echo -en "$(BROWN)You can pass program args with args=\"...\"$(END_COLOR)\n\n--\n"
endif
	valgrind \
		--track-origins=yes \
		--leak-check=full \
		--leak-resolution=high \
		--log-file=$(LOGDIR)/$@.log \
		$(MAIN) $(args)
	@echo -en "\nCheck the log file: $(LOGDIR)/$@.log\n"

clean:
	@rm -rvf $(BINDIR)/*