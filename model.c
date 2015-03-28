#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "model.h"
#include "block_list.h"
#include "matrix_file.h"
#include "boilerplate.h"

//TODO:
// Verificar se completou blocos
// Rotation of block in board

struct Matrix *transpose_matrix(struct Matrix *matrix)
{
    int i, j;
    struct Matrix *transposed_matrix = create_matrix(matrix->col_size, matrix->row_size);

    for (i = 0; i < transposed_matrix->row_size; i++)
    {
        for (j = 0; j < transposed_matrix->col_size; j++)
        {
            transposed_matrix->values[i][j] = matrix->values[j][i];
        }
    }

    return transposed_matrix;
}

// Push to boilerplate
struct Matrix *clone_matrix(struct Matrix *matrix)
{
    int i, j;
    struct Matrix *new_matrix = create_matrix(matrix->row_size, matrix->col_size);

    for (i = 0 ; i < matrix->row_size; i++)
    {
        for (j = 0; j < matrix->col_size; j++)
        {
            new_matrix->values[i][j] = matrix->values[i][j];
        }
    }
    return new_matrix;
}

struct Matrix *rotate_matrix_clockwise(struct Matrix *matrix)
{
    int i, j, k;
    struct Matrix *rotated_matrix = create_matrix(matrix->row_size, matrix->col_size);

    for (i = 0 ; i < matrix->row_size; i++)
    {
        for (j = 0, k = matrix->col_size - 1; j < matrix->col_size; j++, k--)
        {
            rotated_matrix->values[i][j] = matrix->values[i][k];
        }
    }

    return rotated_matrix;
}

struct Matrix *rotate_matrix_anticlockwise(struct Matrix *matrix)
{        
    int i, j, k;
    struct Matrix *rotated_matrix = create_matrix(matrix->row_size, matrix->col_size);

    for (i = 0, k = matrix->row_size - 1; i < matrix->row_size; i++, k--)
    {
        for (j = 0; j < matrix->col_size; j++)
        {
            rotated_matrix->values[i][j] = matrix->values[k][j];
        }
    }

    return rotated_matrix;
}

// Algorithm taken from http://stackoverflow.com/questions/42519/how-do-you-rotate-a-two-dimensional-array
struct Block *rotate_block(struct Block *block, bool clockwise)
{
    struct Matrix *transposed_matrix = transpose_matrix(block->matrix);

    struct Matrix *rotated_matrix = (clockwise) ? rotate_matrix_clockwise(transposed_matrix) : rotate_matrix_anticlockwise(transposed_matrix);

    free_matrix(transposed_matrix);

    struct Block *new_block = malloc(sizeof(struct Block));
    new_block->color = block->color;
    new_block->matrix = rotated_matrix;

    return new_block;
}

struct Block *get_random_block(struct BlockList *block_list)
{
    int i;

    int random_block = rand() % block_list->elements_number;

    struct Block *block = get(block_list, random_block);
    
    struct Block *new_block = malloc(sizeof(struct Block));
    new_block->color = block->color;
    new_block->matrix = clone_matrix(block->matrix); 

   return new_block;
}

enum Color **malloc_collor_matrix(int width, int height)
{
    int i, j;
    enum Color **colors = malloc(height * sizeof(enum Color *));

    for ( i = 0; i < height; i++)
        colors[i] = malloc(width * sizeof(enum Color));

    for (i = 0; i < height; ++i)
    {
        for (j = 0; j < width; j++)
        {
            colors[i][j] = NONE;        
        }
    }

    return colors;
}

void set_default_values(struct Board *board)
{
    board->next_block = get_random_block(board->default_blocks);
    // TODO: Calculate the right initial position of x
    board->current_block_x = 0;
    board->current_block_y = -1;

}

struct Board *create_board(int width, int height)
{
    int i, j;

    srand(time(NULL));

    struct Board *board = malloc(sizeof(struct Board));

    board->width = width;
    board->height = height; 

    board->default_blocks = read_from_file("temp");

    board->current_block = get_random_block(board->default_blocks);
    set_default_values(board);

    board->visited = malloc_collor_matrix(width, height);
    board->is_game_over = false;

    return board;
}


void replace_lines(struct Board *board, int row)
{
    int i, j, k, tmp;

    for(i = 0; i < board->width; i++)
    {
        for(j = row - 1; j >=0; j--)
        {
            if(board->visited[j+1][i] != NONE)
            {
                tmp = board->visited[j][i];
                board->visited[j][i] = 0;
                board->visited[j+1][i] = tmp;               
            }
        }
    }
}

bool check_line(struct Board *board, int line)
{
    int i, j;
    for (i = 0; i < board->width; i++)
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
    int i;

    for (i = 0; i < board->height; i++)
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
    int i, j, k, m;
    for (i = board->current_block_y, k = board->current_block->matrix->row_size - 1; i > board->current_block_y - board->current_block->matrix->row_size; i--, k--)
    {
        for (j = board->current_block_x, m = 0; j < board->current_block_x + board->current_block->matrix->col_size; j++, m++)
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
    int i , k, j, m;

    for (i = index, k = board->current_block->matrix->row_size - 1; i > index - board->current_block->matrix->row_size; i--, k--)
    {
        for (j = board->current_block_x, m = 0; j < board->current_block_x + board->current_block->matrix->col_size; j++, m++)
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

bool block_fits(struct Board *, int, int);

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

bool block_fits(struct Board *board, int new_y, int new_x)
{
    int i, j, k, m;
    for (i = new_y, k = board->current_block->matrix->row_size - 1; i > new_y - board->current_block->matrix->row_size; i--, k--)
    {
        for (j = board->current_block_x, m = 0; j < board->current_block->matrix->col_size + board->current_block_x; j++, m++)
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

