#include <stdlib.h>
#include <stdio.h>

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

bool **malloc_matrix(int row_size, int col_size)
{
    int i;

    bool **matrix = malloc(row_size * (sizeof(bool*))); 

    for (i = 0; i < col_size; i++) 
        matrix[i] = malloc(col_size * sizeof(bool));

    return matrix;
}

void free_matrix(struct Matrix *matrix)
{
    int i;
    for(i = 0; i < matrix->row_size; i++) {
        free(matrix->values[i]);
    }

    free(matrix->values);
}

void free_block(struct Block *block) 
{
    free_matrix(block->matrix);
    free(block);
}

void free_board(struct Board *board)
{    
    free_list(board->default_blocks);

    if(board->current_block)
        free(board->current_block);

    if(board->next_block)
        free(board->next_block);

    free(board);
}

void free_list(struct BlockList* blockList)
{
    int i = 0;
    
    struct BlockNode *node = blockList->HEAD;
    struct BlockNode *tmp = NULL; 

    for (tmp = node; tmp != NULL; tmp = node)
    {
        node = node->next;

        free_block(tmp->block); 
        free(tmp);
    }

    free(blockList);
}
