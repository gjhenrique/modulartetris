CC=gcc
CFLAGS=-g -std=gnu99
LIBFLAGS=-fPIC -shared -Wl,-soname,$(TETRIS_LIB_NAME)

TETRIS_LIB=tetris
TETRIS_LIB_NAME=lib$(TETRIS_LIB).so
TETRIS_SRC=$(TETRIS_FOLDER)/block_list.c $(TETRIS_FOLDER)/matrix_file.c $(TETRIS_FOLDER)/board.c $(TETRIS_FOLDER)/boilerplate.c $(TETRIS_FOLDER)/block.c
TETRIS_FOLDER=tetris

TEST_SRC=test/test.c

NCURSES_FLAGS=-lncurses -lpanel -Itetris
NCURSES_SRC=ncurses/ncurses.c

# Put your Python 3 Python.h location here
PYTHON_INCLUDE=/usr/include/python3.5m

all: $(TETRIS_LIB_NAME) tetris_c test android_swig python_swig

build_lib $(TETRIS_LIB_NAME): $(TETRIS_SRC)
	$(CC) $(LDFLAGS) $(LIBFLAGS) -o $(TETRIS_LIB_NAME) $(TETRIS_SRC) $(CFLAGS)

tetris_c: $(TETRIS_LIB_NAME) $(NCURSES_SRC)
	$(CC) $(NCURSES_SRC) $(CFLAGS) $(NCURSES_FLAGS) -o tetris_c -l$(TETRIS_LIB) -L.

run_test: $(TETRIS_LIB_NAME) $(TEST_SRC)
	$(CC) $(TEST_SRC) -l$(TETRIS_LIB) -L. $(CFLAGS) -Itetris -o run_test

android_swig: $(TETRIS_LIB_NAME)
	swig  -java -verbose -package br.com.gjhenrique.modulartetris -o android/app/src/main/swig/modulartetris_wrapper.c -outdir android/app/src/main/swig/br/com/gjhenrique/modulartetris tetris/modular_tetris.i

python_swig: $(TETRIS_LIB_NAME)
	swig -python -py3 -o python/modulartetris_wrap.c tetris/modular_tetris.i
	gcc -shared -fPIC $(CFLAGS) python/modulartetris_wrap.c $(TETRIS_SRC) -Itetris -I$(PYTHON_INCLUDE) -o python/_modular_tetris.so

clean:
	rm -f $(TETRIS_LIB_NAME) run_test tetris_c

.PHONY:
	build_lib android_swig python_swig
