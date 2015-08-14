LDFLAGS=-g -lncurses -std=gnu99

all: main.c
	gcc block_list.c matrix_file.c board.c boilerplate.c main.c block.c $(LDFLAGS) -o main

test:
	gcc block_list.c matrix_file.c board.c boilerplate.c block.c test/test.c $(LDFLAGS) -o run_test

.PHONY: all test
