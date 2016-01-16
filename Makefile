CC=gcc
CFLAGS=-g -std=gnu99 -Wall -lm
LIBFLAGS=-fPIC -shared -Wl,-soname,$(TETRIS_LIB_NAME)
ENVFLAGS=-L. -Wl,-rpath,.

TETRIS_LIB=tetris
TETRIS_LIB_NAME=lib$(TETRIS_LIB).so
TETRIS_SRC=$(TETRIS_FOLDER)/block_list.c $(TETRIS_FOLDER)/matrix_file.c $(TETRIS_FOLDER)/board.c $(TETRIS_FOLDER)/boilerplate.c $(TETRIS_FOLDER)/block.c
TETRIS_FOLDER=tetris

TEST_SRC=test/test.c

NCURSES_FLAGS=-lncurses -lpanel -Itetris
NCURSES_SRC=ncurses/ncurses.c

# Include your Python 3 Python.h location here
PYTHON_INCLUDE=/usr/include/python3.5m

all: $(TETRIS_LIB_NAME) ncurses_game test android_swig python_swig

build_lib $(TETRIS_LIB_NAME): $(TETRIS_SRC)
	$(CC) $(LDFLAGS) $(LIBFLAGS) -o $(TETRIS_LIB_NAME) $(TETRIS_SRC) $(CFLAGS)

libtetrispp.so: $(TETRIS_SRC)
	g++ $(LDFLAGS) $(LIBFLAGS) -o libtetrispp.so $(TETRIS_SRC) $(CFLAGS)

ncurses_game: $(TETRIS_LIB_NAME) $(NCURSES_SRC)
	$(CC) $(NCURSES_SRC) $(ENVFLAGS) $(CFLAGS) $(NCURSES_FLAGS) -l$(TETRIS_LIB) -o ncurses_game

run_test: $(TETRIS_LIB_NAME) $(TEST_SRC)
	$(CC) $(TEST_SRC) $(ENVFLAGS) -l$(TETRIS_LIB) $(CFLAGS) -Itetris -o run_test

android_swig: $(TETRIS_LIB_NAME)
	swig  -java -package br.com.gjhenrique.modulartetris -o android/app/src/main/swig/modulartetris_wrapper.c -outdir android/app/src/main/swig/br/com/gjhenrique/modulartetris tetris/modular_tetris.i

python_swig: $(TETRIS_LIB_NAME)
	swig -python -py3 -o python/modulartetris_wrap.c tetris/modular_tetris.i
	$(CC) -shared -fPIC $(CFLAGS) $(ENVFLAGS) python/modulartetris_wrap.c -l$(TETRIS_LIB) -Itetris -I$(PYTHON_INCLUDE) -o python/_modular_tetris.so

javascript_swig: $(TETRIS_LIB_NAME)
	swig -javascript -node -c++ -DV8_VERSION=$(shell node -e 'console.log(process.versions.v8)' | sed 's/\.//g' | sed 's/^/0x/') -o javascript/modulartetris_wrap.cpp tetris/modular_tetris.i

clean:
	rm -f $(TETRIS_LIB_NAME) run_test ncurses_game

.PHONY:
	build_lib android_swig python_swig javascript_swig
