#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

struct Matrix
{
    bool **values;
    int col_size;
    int row_size;
};

enum Color
{
    NONE, RED, BLUE, GREEN, ORANGE, PINK, BROWN, PURPLE
};

struct Block
{
    struct Matrix *matrix;
    enum Color color;
};

struct Board
{
    int height;
    int width;

    int current_block_x;
    int current_block_y;

    // Pointer to Block
    struct Block *current_block;

    struct Block *next_block;

    struct BlockList *default_blocks;

    enum Color **visited;

    bool is_game_over;
};


#endif /* MODEL_H */

