CC=gcc
CFLAGS=-std=gnu99 -Wall
CFLAGS=-g -std=gnu99
LIBFLAGS=-fPIC -shared -Wl,-soname,$(TETRIS_LIB_NAME)

TETRIS_LIB=tetris
TETRIS_LIB_NAME=lib$(TETRIS_LIB).so
TETRIS_SRC=$(TETRIS_FOLDER)/block_list.c $(TETRIS_FOLDER)/matrix_file.c $(TETRIS_FOLDER)/board.c $(TETRIS_FOLDER)/boilerplate.c $(TETRIS_FOLDER)/block.c
TETRIS_FOLDER=tetris

TEST_SRC=test/test.c

NCURSES_FLAGS=-lncurses -lpanel -Itetris
NCURSES_SRC=ncurses/ncurses.c

all: $(TETRIS_LIB_NAME) tetris_c test

build_lib $(TETRIS_LIB_NAME): $(TETRIS_SRC)
	$(CC) $(LDFLAGS) $(LIBFLAGS) -o $(TETRIS_LIB_NAME) $(TETRIS_SRC) $(CFLAGS)

tetris_c: $(TETRIS_LIB_NAME) $(NCURSES_SRC)
	$(CC) $(NCURSES_SRC) $(CFLAGS) $(NCURSES_FLAGS) -o tetris_c -l$(TETRIS_LIB) -L.

run_test: $(TETRIS_LIB_NAME)
	$(CC) $(TEST_SRC) -l$(TETRIS_LIB) -L. $(CFLAGS) -Itetris -o run_test

clean:
	rm -f $(TETRIS_LIB_NAME) run_test tetris_c

.PHONY:
	build_lib
