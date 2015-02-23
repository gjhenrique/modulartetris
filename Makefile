LDFLAGS=-g -lncurses

all: main.c	
	gcc block_list.c matrix_file.c model.c boilerplate.c main.c $(LDFLAGS) -o main
