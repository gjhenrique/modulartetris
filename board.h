#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

#include "model.h"

#define rotate_clockwise(board) rotate(board, true);
#define rotate_anticlockwise(board) rotate(board, false);

#define move_to_left(board) move(board, true);
#define move_to_right(board) move(board, false);

#define move_to_bottom(board) while(next_move(board)) { }

bool next_move (struct Board *board);

struct Board *create_board(int width, int height, struct BlockList *blockList);

bool rotate(struct Board *, bool);

#endif
