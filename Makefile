.SUFFIXES: .c .h .o

CC	= gcc
INCLDIR	= include/
BINDIR	= bin/
SRCDIR	= src/

_BIN	= a.out
BIN	= $(addprefix $(BINDIR), $(_BIN))

SRC	= $(wildcard src/*.c)

CFLAGS = -Wall -pedantic -g -I$(INCLDIR) -lm -lpthread

$(BINDIR)/$(_BIN): $(SRC)
	@$(CC) $(SRC) $(CFLAGS) -o $@

gendir: 
	@mkdir include bin src

clean: 
	@rm bin/a.out

.PHONY: gendir
.PHONY: clean
