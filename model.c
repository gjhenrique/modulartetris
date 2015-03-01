#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "model.h"
#include "block_list.h"
#include "matrix_file.h"
#include "boilerplate.h"

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

struct Matrix *rotate_clockwise(struct Matrix *matrix)
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

struct Matrix *rotate_anticlockwise(struct Matrix *matrix)
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

    struct Matrix *rotated_matrix = (clockwise) ? rotate_clockwise(transposed_matrix) : rotate_anticlockwise(transposed_matrix);

    free_matrix(transposed_matrix);

    struct Block *new_block = malloc(sizeof(struct Block));
    new_block->color = block->color;
    new_block->matrix = rotated_matrix;

    return new_block;
}

struct Block *get_random_block(struct BlockList *block_list)
{
    int random_number = rand() % block_list->elements_number;

    struct Block *block = get(block_list, random_number);

    struct Block *new_block = malloc(sizeof(struct Block));

    // Be careful!!! We are doing a shallow copy the block of the list
    // Don't call free_block in the block returned by this function
    memcpy(new_block, block, sizeof(struct Block));

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
    board->current_block_x = (board->width - 3) / 2;
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

    board->current_block = get_random_block(board->default_blocks);
    set_default_values(board);

    board->visited = malloc_collor_matrix(width, height);

    return board;
}

void prepare_next_block(struct Board * board)
{
    free(board->current_block);
    board->current_block = board->next_block;
    set_default_values(board);
}

bool game_over(struct Board *board)
{
    int i, j;
    
    //for(i = 0; i < board->height; i++)

    for (j = board->current_block_x; j < board->current_block_x + board->current_block->matrix->col_size; j++)
    {
        bool over = true;
        //for (j = board->current_block_x; j < board->current_block_x + board->current_block->matrix->col_size; j++)

        for(i = 0; i < board->height; i++)
        {
            if(board->visited[i][j] == NONE)
            {
                over = false;
                break;
            }
        }

        if(over) 
            return true;
    }

    return false;
}

void next_move(struct Board *board)
{
    int i, j, k, m;

    int new_y = board->current_block_y + 1;
    
    // Checking if the game is over
    if (game_over(board))
    {
        exit(-1);
    }

    if (new_y >= board->height)
    {
        prepare_next_block(board);
        return; 
    }

    for (i = board->current_block_x, j = 0; i < board->current_block->matrix->col_size + board->current_block_x; i++, j++)
    {
        if (board->visited[new_y][i] != NONE && board->current_block->matrix->values[board->current_block->matrix->row_size -1][j] == true)
        {
            prepare_next_block(board); 
            return;
        }
    }

    for (i = new_y, k = board->current_block->matrix->row_size - 1; i > new_y - board->current_block->matrix->row_size; i--, k--)
    {
        for (j = board->current_block_x, m = 0; j < board->current_block_x + board->current_block->matrix->col_size; j++, m++)
        {
            if (i < board->height && i >= 0)
            {
                if (board->current_block->matrix->values[k][m])
                {
                    board->visited[i][j] = board->current_block->color;
                }
                else
                {
                    board->visited[i][j] = (new_y == i && board->visited[i][j] !=NONE) ? board->visited[i][j] : NONE;
                }
            }
        }
    }

    // Erasing previous block
    int previous_y = new_y - board->current_block->matrix->row_size;
    for (i = board->current_block_x; i < board->current_block->matrix->col_size + board->current_block_x; i++)
    {
        if (previous_y < board->height && previous_y >= 0)
        {
            board->visited[previous_y][i] = NONE;
        }
    }

    board->current_block_y = new_y;
}
