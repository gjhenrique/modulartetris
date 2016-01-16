#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "board.h"
#include "block_list.h"
#include "matrix_file.h"
#include "boilerplate.h"
#include "block.h"

#define BEGIN_CURRENT_BOARD_LOOP(board, x, y)                             \
for (int i = y, k = board->current_block->row_size - 1; i > y - board->current_block->row_size; i--, k--) { \
    for (int j = x, m = 0; j < x + board->current_block->col_size; j++, m++) {

#define END_CURRENT_BOARD_LOOP \
  }                            \
}

void set_default_values(struct Board *board)
{
    board->next_block = get_random_block(board->block_list);

    board->current_block_x = (board->width / 2) - (board->current_block->col_size / 2);

    board->current_block_y = -1;
}

struct Board *create_board(int width, int height, struct BlockList *list)
{
    srand(time(NULL));

    struct Board *board = (struct Board *) malloc(sizeof(struct Board));

    board->width = width;
    board->height = height;

    board->block_list = list;

    if(board->block_list->elements_number == 0)
    {
        fprintf(stderr, "Number of tetrominos is empty");
        exit(-1);
    }

    for (int i = 0; i < list->elements_number; i++)
    {
        struct Block *block = get(list, i);

        if(block->col_size > board->width)
        {
            fprintf(stderr, "Blocks are not supposed to be wider than the width of the board");
            exit(-1);
        }

        if(block->row_size > board->height)
        {
            fprintf(stderr, "Blocks are not supposed to be taller than the height of the board");
            exit(-1);
        }
    }

    board->current_block = get_random_block(board->block_list);
    set_default_values(board);

    board->board_values = malloc_collor_matrix(width, height);
    board->is_game_over = false;

    board->score = 0;
    board->rows = 0;

    return board;
}

struct Board *create_board_file(int width, int height, char *file_name)
{
    struct BlockList *block_list = read_from_file(file_name);
    return create_board(width, height, block_list);
}

struct Board *create_board_string(int width, int height, char *board_string)
{
    struct BlockList *block_list = read_from_string(board_string);
    return create_board(width, height, block_list);
}

bool is_empty_space(enum Color color)
{
  return color == NONE || color == GHOST;
}

void replace_lines(struct Board *board, int row)
{
    for (int i = 0; i < board->width; i++)
    {
        for (int j = row - 1; j >= 0; j--)
        {
            enum Color tmp = board->board_values[j][i];
            board->board_values[j][i] = NONE;
            board->board_values[j+1][i] = tmp;
        }
    }
}

bool is_empty_line(struct Board *board, int line)
{
    for (int i = 0; i < board->width; i++)
    {
        if (is_empty_space(board->board_values[line][i]))
        {
            return true;
        }
    }
    return false;
}

void check_lines(struct Board *board)
{
    int rows = 0;
    for (int i = 0; i < board->height; i++)
    {
        if (!is_empty_line(board, i))
        {
            replace_lines(board, i);
            rows++;
        }
    }
    // Inspired in javascript-tetris
    if (rows != 0)
    {
        board->rows += rows;
        board->score += 100 * pow(2, rows - 1);
    }
}

void prepare_next_block(struct Board * board)
{
    check_lines(board);
    free_block(board->current_block);
    board->current_block = board->next_block;
    set_default_values(board);
}

void erase_current_block(struct Board *board, int index)
{
    BEGIN_CURRENT_BOARD_LOOP(board, board->current_block_x, index)
        if (i < board->height && i >= 0)
        {
            if(board->current_block->values[k][m])
                board->board_values[i][j] = NONE;
        }
    END_CURRENT_BOARD_LOOP
}

void insert_current_block(struct Board *board, int index, enum Color color)
{

    BEGIN_CURRENT_BOARD_LOOP(board, board->current_block_x, index)
        if (i < board->height && i >= 0)
        {
            if(board->current_block->values[k][m])
            {
                board->board_values[i][j] = color;
            }
        }
    END_CURRENT_BOARD_LOOP
}

bool block_fits(struct Board *board, int new_x, int new_y)
{
    BEGIN_CURRENT_BOARD_LOOP(board, new_x, new_y)
        if(i >= board->height && board->current_block->values[k][m] != NONE)
        {
            return false;
        }

        if((j < 0 || j >= board->width) && board->current_block->values[k][m] != NONE)
        {
            return false;
        }

        if (i < board->height && i >= 0)
        {
            if (!is_empty_space(board->board_values[i][j]) && board->current_block->values[k][m] != NONE)
            {
                return false;
            }
        }
    END_CURRENT_BOARD_LOOP

    return true;
}

int get_ghost_position(struct Board *board, int y_position)
{
    int ghost_y = y_position;

    while(block_fits(board, board->current_block_x, ghost_y))
    {
      ghost_y++;
    }
    return ghost_y - 1;
}

void move_block(struct Board *board, bool left)
{
    int new_x = (left) ? board->current_block_x - 1 : board->current_block_x + 1;

    erase_current_block(board, board->current_block_y);
    erase_current_block(board, board->ghost_block_y);

    bool fits = block_fits(board, new_x, board->current_block_y);

    if(fits)
    {
        board->current_block_x = new_x;
    }

    board->ghost_block_y = get_ghost_position(board, board->current_block_y);
    insert_current_block(board, board->ghost_block_y, GHOST);
    insert_current_block(board, board->current_block_y, board->current_block->color);
}

void move_to_left(struct Board *board)
{
    move_block(board, true);
}

void move_to_right(struct Board *board)
{
    move_block(board, false);
}

bool game_over(struct Board *board, bool fits)
{
    BEGIN_CURRENT_BOARD_LOOP(board, board->current_block_x, board->current_block_y)
        if(i < 0 && !fits && !is_empty_space(board->current_block->values[k][m]))
        {
            return true;
        }
    END_CURRENT_BOARD_LOOP

    return false;
}

bool rotate(struct Board *board, bool clockwise)
{
    struct Block *old_block = board->current_block;

    erase_current_block(board, board->current_block_y);
    erase_current_block(board, board->ghost_block_y);

    board->current_block = rotate_block(board->current_block, clockwise);

    bool fits = block_fits(board, board->current_block_x, board->current_block_y);

    if (fits)
    {
        free_block(old_block);
    }
    else
    {
        free_block(board->current_block);
        board->current_block = old_block;
    }

    board->ghost_block_y = get_ghost_position(board, board->current_block_y);
    insert_current_block(board, board->ghost_block_y, GHOST);
    insert_current_block(board, board->current_block_y, board->current_block->color);

    return fits;
}

void rotate_clockwise(struct Board *board)
{
    rotate(board, true);
}

void rotate_anticlockwise(struct Board *board)
{
    rotate(board, false);
}

bool next_move(struct Board *board)
{
    int new_y = board->current_block_y + 1;

    erase_current_block(board, board->current_block_y);
    erase_current_block(board, board->ghost_block_y);

    bool fits = block_fits(board, board->current_block_x, new_y);

    if (game_over(board, fits))
    {
        insert_current_block(board, board->current_block_y, board->current_block->color);
        board->is_game_over = true;
        return false;
    }

    if(!fits)
    {
        board->score += 10;
        insert_current_block(board, board->current_block_y, board->current_block->color);
        prepare_next_block(board);
        board->ghost_block_y = board->current_block_y;

        return false;
    }

    board->current_block_y = new_y;

    board->ghost_block_y = get_ghost_position(board, board->current_block_y);
    insert_current_block(board, board->ghost_block_y, GHOST);
    insert_current_block(board, new_y, board->current_block->color);

    return true;
}

void move_to_bottom(struct Board *board)
{
    while(next_move(board)) { }
}
