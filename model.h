#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

#define rotate_block_clockwise(block) rotate_block(block, true);
#define rotate_block_anticlockwise(block) rotate_block(block, false);

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
};

void next_move (struct Board *board);
struct Board *create_board(int width, int height);

struct Block *rotate_block(struct Block *block, bool clockwise);

#endif /* MODEL_H */
