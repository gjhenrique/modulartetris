LDFLAGS=-g -lncurses

all: main.c	
	gcc block_list.c matrix_file.c board.c boilerplate.c main.c block.c $(LDFLAGS) -o main
