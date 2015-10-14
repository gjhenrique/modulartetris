#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "board.h"
#include "block_list.h"
#include "matrix_file.h"
#include "boilerplate.h"
#include "block.h"

void set_default_values(struct Board *board)
{
    board->next_block = get_random_block(board->default_blocks);

    board->current_block_x = (board->width / 2) - (board->current_block->col_size / 2);

    board->current_block_y = -1;
}

struct Board *create_board(int width, int height, struct BlockList *blockList)
{
    srand(time(NULL));

    struct Board *board = malloc(sizeof(struct Board));

    board->width = width;
    board->height = height;

    board->default_blocks = blockList;

    if(board->default_blocks->elements_number == 0)
    {
        fprintf(stderr, "Number of tetrominos is empty");
        exit(0);
    }

    board->current_block = get_random_block(board->default_blocks);
    set_default_values(board);

    board->visited = malloc_collor_matrix(width, height);
    board->is_game_over = false;

    return board;
}


void replace_lines(struct Board *board, int row)
{
    for (int i = 0; i < board->width; i++)
    {
        for (int j = row - 1; j >= 0; j--)
        {
            enum Color tmp = board->visited[j][i];
            board->visited[j][i] = NONE;
            board->visited[j+1][i] = tmp;
        }
    }
}

bool is_empty_line(struct Board *board, int line)
{
    for (int i = 0; i < board->width; i++)
    {
        if(board->visited[line][i] == NONE)
        {
            return true;
        }
    }
    return false;
}

void check_lines(struct Board *board)
{
    for (int i = 0; i < board->height; i++)
    {
        if(!is_empty_line(board, i))
        {
            replace_lines(board, i);
        }
    }
}

void prepare_next_block(struct Board * board)
{
    check_lines(board);
    free_block(board->current_block);
    board->current_block = board->next_block;
    set_default_values(board);
}

void erase_current_block(struct Board *board)
{
    for (int i = board->current_block_y, k = board->current_block->row_size - 1; i > board->current_block_y - board->current_block->row_size; i--, k--)
    {
        for (int j = board->current_block_x, m = 0; j < board->current_block_x + board->current_block->col_size; j++, m++)
        {
            if (i < board->height && i >= 0)
            {
                if(board->current_block->values[k][m])
                    board->visited[i][j] = NONE;
            }
        }
    }
}

void print_current_block(struct Board *board, int index)
{
    for (int i = index, k = board->current_block->row_size - 1; i > index - board->current_block->row_size; i--, k--)
    {
        for (int j = board->current_block_x, m = 0; j < board->current_block_x + board->current_block->col_size; j++, m++)
        {
            if (i < board->height && i >= 0)
            {
                if(board->current_block->values[k][m])
                {
                    board->visited[i][j] = board->current_block->color;
                }
            }
        }
    }
}

bool block_fits(struct Board *board, int new_x, int new_y)
{
    for (int i = new_y, k = board->current_block->row_size - 1; i > new_y - board->current_block->row_size; i--, k--)
    {
        for (int j = new_x, m = 0; j < board->current_block->col_size + new_x; j++, m++)
        {
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
                if (board->visited[i][j] != NONE && board->current_block->values[k][m] != NONE)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void move_block(struct Board *board, bool left)
{
    int new_x = (left) ? board->current_block_x - 1 : board->current_block_x + 1;

    erase_current_block(board);

    bool fits = block_fits(board, new_x, board->current_block_y);

    if(fits)
    {
        board->current_block_x = new_x;
    }

    print_current_block(board, board->current_block_y);
}

bool game_over(struct Board *board, bool fits)
{
    for (int i = board->current_block_y, k = board->current_block->row_size - 1; i > board->current_block_y - board->current_block->row_size; i--, k--)
    {
        for (int j = board->current_block_x, m = 0; j < board->current_block->col_size + board->current_block_x; j++, m++)
        {
            if(i < 0 && !fits && board->current_block->values[k][m] != NONE)
            {
                return true;
            }
        }
    }

    return false;
}

bool block_fits_default(struct Board *board)
{
    return block_fits(board, board->current_block_x, board->current_block_y);
}

bool rotate(struct Board *board, bool clockwise)
{
    struct Block *old_block = board->current_block;

    erase_current_block(board);
    board->current_block = rotate_block(board->current_block, clockwise);

    bool fits = block_fits_default(board);

    if (fits)
    {
        free_block(old_block);
    }
    else
    {
        free_block(board->current_block);
        board->current_block = old_block;
    }

    print_current_block(board, board->current_block_y);

    return fits;
}

bool next_move(struct Board *board)
{
    int new_y = board->current_block_y + 1;

    erase_current_block(board);

    bool fits = block_fits(board, board->current_block_x, new_y);

    if (game_over(board, fits))
    {
        print_current_block(board, board->current_block_y);
        board->is_game_over = true;
        return false;
    }

    if(!fits)
    {
        print_current_block(board, board->current_block_y);
        prepare_next_block(board);
        return false;
    }
    print_current_block(board, new_y);
    board->current_block_y = new_y;
    return true;
}

