#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "boilerplate.h"
#include "block_list.h"

// Warning: Only works with numbers
int to_digit(char c)
{
    return c - '0';
}

void print_matrix(struct Matrix *matrix)
{
    int i, j;

    for (i = 0; i < matrix->row_size; ++i)
    {
        for (j = 0; j < matrix->col_size; ++j)
        {
           printf("%d\t", matrix->values[i][j]); 
        }
        printf("\n");
    }

    printf("\n\n");
}

void print_board(struct Board *board)
{
    int i, j;

    for (i = 0; i < board->height; i++)
    {
        for (j = 0; j < board->width; j++)
        {
            printf("%d", board->visited[i][j]);
            printf ("\t");
        }
        printf("\n");
    } 
    printf("\n\n");
}

struct Matrix *create_matrix(int row_size, int col_size) {

    struct Matrix* matrix = malloc(sizeof(struct Matrix));

    matrix->col_size = col_size;
    matrix->row_size = row_size;
    matrix->values = malloc_matrix(row_size, col_size);

    return  matrix;
}

bool **malloc_matrix(int row_size, int col_size)
{
    int i, j;

    bool **matrix = malloc(row_size * (sizeof(bool*))); 

    for (i = 0; i < row_size; i++) 
        matrix[i] = malloc(col_size * sizeof(bool));

    for (i = 0; i < row_size; i++)
    {
        for (j = 0; j < col_size; j++)
        {
            matrix[i][j] = 0; 
        }
    }

    return matrix;
}

void free_matrix(struct Matrix *matrix)
{
    int i;
    for(i = 0; i < matrix->row_size; i++) {
        free(matrix->values[i]);
    }

    free(matrix->values);
    free(matrix);
}

void free_block(struct Block *block) 
{
    free_matrix(block->matrix);
    free(block);
}

void free_board(struct Board *board)
{    
    int i;

    free_list(board->default_blocks);

    if(board->current_block)
        free_block(board->current_block);

    if(board->next_block)
        free_block(board->next_block);

    for (i = 0; i < board->height; i++)
    {
        free(board->visited[i]);
    }
    
    free(board->visited);

    free(board);
}

void free_list(struct BlockList* block_list)
{
    int i = 0;
    
    struct BlockNode *node = block_list->HEAD;
    struct BlockNode *tmp = NULL; 

    for (tmp = node; tmp != NULL; tmp = node)
    {
        node = node->next;
        free_block(tmp->block); 
        free(tmp);
    }

    free(block_list);
}

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
