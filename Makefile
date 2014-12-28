LDFLAGS=-g -lncurses 

all: main.c	
	gcc main.c $(LDFLAGS) -o main
