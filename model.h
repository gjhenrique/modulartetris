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
    RED, BLUE, GREEN, ORANGE, PINK, BROWN, PURPLE
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
    
    // Pointer to Block
    struct Block *current_block;

    struct Block *next_block;

    struct BlockList *default_blocks;

    // Matrix of a pointers of blocks
    enum Color **visited;
};

#endif /* MODEL_H */
