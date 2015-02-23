#include <stdio.h>
#include <stdlib.h>

#include "model.h"
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
