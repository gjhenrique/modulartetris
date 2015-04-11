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
    
    board->current_block_x = (board->width / 2) - (board->current_block->matrix->col_size / 2);

    board->current_block_y = -1;
}

struct Board *create_board(int width, int height)
{
    int i, j;

    srand(time(NULL));

    struct Board *board = malloc(sizeof(struct Board));

    board->width = width;
    board->height = height; 

    board->default_blocks = read_from_file("default_blocks");

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
    for(int i = 0; i < board->width; i++)
    {
        for(int j = row - 1; j >=0; j--)
        {
            if(board->visited[j+1][i] != NONE)
            {
                int tmp = board->visited[j][i];
                board->visited[j][i] = 0;
                board->visited[j+1][i] = tmp;               
            }
        }
    }
}

bool check_line(struct Board *board, int line)
{
    for (int i = 0; i < board->width; i++)
    {
        if(board->visited[line][i] == NONE)
        {
            return false;
        }
    }

    return true;
}

void check_lines(struct Board *board)
{
    for (int i = 0; i < board->height; i++)
    {
        if(check_line(board, i))
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
    for (int i = board->current_block_y, k = board->current_block->matrix->row_size - 1; i > board->current_block_y - board->current_block->matrix->row_size; i--, k--)
    {
        for (int j = board->current_block_x, m = 0; j < board->current_block_x + board->current_block->matrix->col_size; j++, m++)
        {
            if(i < board->height && i >= 0)
            {
                if(board->current_block->matrix->values[k][m])
                    board->visited[i][j] = NONE;
            }
        }
    }
}

void print_current_block(struct Board *board, int index)
{
    for (int i = index, k = board->current_block->matrix->row_size - 1; i > index - board->current_block->matrix->row_size; i--, k--)
    {
        for (int j = board->current_block_x, m = 0; j < board->current_block_x + board->current_block->matrix->col_size; j++, m++)
        {
            if (i < board->height && i >= 0)
            {
                if(board->current_block->matrix->values[k][m])
                {
                    board->visited[i][j] = board->current_block->color;
                }
            }
        }
    }
}

bool block_fits(struct Board *board, int new_y, int new_x)
{
    for (int i = new_y, k = board->current_block->matrix->row_size - 1; i > new_y - board->current_block->matrix->row_size; i--, k--)
    {
        for (int j = board->current_block_x, m = 0; j < board->current_block->matrix->col_size + board->current_block_x; j++, m++)
        {
            if(i >= board-> height && board->current_block->matrix->values[k][m] == true)
            {
                return false;
            }

            if((j < 0 || j >= board->width) && board->current_block->matrix->values[k][m] == true)
            {
                return false;
            }

            if (i < board->height && i >= 0)
            {
                if (board->visited[i][j] != NONE && board->current_block->matrix->values[k][m] == true)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void move(struct Board *board, bool left)
{
    int current_x = board->current_block_x;

    int new_x = (left) ? current_x -1 : current_x + 1;

    erase_current_block(board);
    
    bool fits = block_fits(board, board->current_block_y, new_x);

    if(fits)
    {
        board->current_block_x = new_x;
    }

    print_current_block(board, board->current_block_y);
}

bool game_over(struct Board *board, bool fits)
{
    int i;
    int new_y = board->current_block_y; 

    for (i = new_y; i > new_y - board->current_block->matrix->row_size; i--)
    {
        if (i == -1 && !fits)
        {
            return true;
        }
    }
    return false;
}

bool block_fits_default(struct Board *board)
{
    block_fits(board, board->current_block_y, board->current_block_x);
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
    
    int i, j; 
   
    erase_current_block(board); 
    
    bool fits = block_fits(board, new_y, board->current_block_x); 

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

