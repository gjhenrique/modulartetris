LDFLAGS=-g -std=gnu99

all: main.c
	gcc block_list.c matrix_file.c board.c boilerplate.c block.c main.c $(LDFLAGS) -o main

ncurses:
	gcc block_list.c matrix_file.c board.c boilerplate.c block.c ncurses.c $(LDFLAGS) -lncurses -lpanel -o ncurses

test:
	gcc block_list.c matrix_file.c board.c boilerplate.c block.c test/test.c test/greatest.h $(LDFLAGS) -o run_test

.PHONY: all test ncurses
