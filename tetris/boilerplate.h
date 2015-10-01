#ifndef BOILERPLATE_H
#define BOILERPLATE_H

#include <stdbool.h>

#include "model.h"

int to_digit(char c);

void print_block(struct Block *block);

void print_board(struct Board *board);

struct Block *create_block(int row_size, int col_size);

void free_block(struct Block *block);

void free_board(struct Board *board);

void free_list(struct BlockList* block_list);

struct Block *clone_block(struct Block *block);

struct Block *clone_block(struct Block *block);

enum Color **malloc_collor_matrix(int width, int height);

#endif /* BOILERPLATE_H */
