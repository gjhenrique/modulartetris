#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

enum Color
{
    NONE, GHOST, RED, BLUE, GREEN, YELLOW, PINK, PURPLE, ORANGE
};

struct Block
{
    int col_size;
    int row_size;
    enum Color color;
    enum Color **values;
};

struct Board
{
    int height;
    int width;

    int current_block_x;
    int current_block_y;

    int ghost_block_y;

    // Pointer to Block
    struct Block *current_block;

    struct Block *next_block;

    struct BlockList *block_list;

    enum Color **board_values;

    bool is_game_over;

    int score;
    int rows;
};


#endif /* MODEL_H */

