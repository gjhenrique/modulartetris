#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

#define rotate_clockwise(board) rotate(board, true);
#define rotate_anticlockwise(board) rotate(board, false);

#define move_to_left(board) move(board, true);
#define move_to_right(board) move(board, false);

#define move_to_bottom(board) while(next_move(board)) { print_board(board); }

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

bool next_move (struct Board *board);
struct Board *create_board(int width, int height);

struct Block *rotate_block(struct Block *block, bool clockwise);

bool rotate(struct Board *, bool);

#endif /* MODEL_H */
