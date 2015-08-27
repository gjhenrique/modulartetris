#ifndef BOILERPLATE_H
#define BOILERPLATE_H

#include <stdbool.h>

#include "model.h"

int to_digit(char c);

void print_matrix(struct Matrix *matrix);

void print_board(struct Board *board);

bool **malloc_matrix(int row_size, int col_size);

struct Matrix *create_matrix(int row_size, int col_size);

void free_matrix(struct Matrix *matrix);

void free_block(struct Block *block);

void free_board(struct Board *board);

void free_list(struct BlockList* block_list);

struct Block *clone_block(struct Block *matrix);

struct Matrix *clone_matrix(struct Matrix *matrix);

enum Color **malloc_collor_matrix(int width, int height);

#endif /* BOILERPLATE_H */
