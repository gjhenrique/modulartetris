#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

#include "model.h"

void rotate_clockwise(struct Board *board);
void rotate_anticlockwise(struct Board *board);

void move_to_left(struct Board *board);
void move_to_right(struct Board *board);


bool next_move(struct Board *board);
void move_to_bottom(struct Board *board);

struct Board *create_board_file(int width, int height, char *file_path);
struct Board *create_board_string(int width, int height, char *board_string);


bool rotate(struct Board *board, bool clockwise);

#endif
