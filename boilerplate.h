#ifndef BOILERPLATE_H
#define BOILERPLATE_H

#include <stdbool.h>

#include "model.h"

int to_digit(char c);

void print_matrix(struct Matrix *matrix);

bool **malloc_matrix(int row_size, int col_size);

void free_matrix(struct Matrix *matrix);

void free_block(struct Block *block);

void free_board(struct Board *board);

void free_list(struct BlockList* blockList);

#endif /* BOILERPLATE_H */
