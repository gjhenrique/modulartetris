#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "block_list.h"
#include "boilerplate.h"

struct Block *transpose_block(struct Block *block)
{
    struct Block *transposed_block = create_block(block->col_size, block->row_size);

    for (int i = 0; i < transposed_block->row_size; i++)
    {
        for (int j = 0; j < transposed_block->col_size; j++)
        {
            transposed_block->values[i][j] = block->values[j][i];
        }
    }

    return transposed_block;
}

struct Block *rotate_block_clockwise(struct Block *block)
{
    struct Block *rotated_block = create_block(block->row_size, block->col_size);

    for (int i = 0 ; i < block->row_size; i++)
    {
        for (int j = 0, k = block->col_size - 1; j < block->col_size; j++, k--)
        {
            rotated_block->values[i][j] = block->values[i][k];
        }
    }

    return rotated_block;
}

struct Block *rotate_block_anticlockwise(struct Block *block)
{
    struct Block *rotated_block = create_block(block->row_size, block->col_size);

    for (int i = 0, k = block->row_size - 1; i < block->row_size; i++, k--)
    {
        for (int j = 0; j < block->col_size; j++)
        {
            rotated_block->values[i][j] = block->values[k][j];
        }
    }

    return rotated_block;
}

// Algorithm taken from http://stackoverflow.com/questions/42519/how-do-you-rotate-a-two-dimensional-array
struct Block *rotate_block(struct Block *block, bool clockwise)
{
    struct Block *transposed_block = transpose_block(block);

    struct Block *rotated_block = (clockwise) ? rotate_block_clockwise(transposed_block) : rotate_block_anticlockwise(transposed_block);

    free_block(transposed_block);
    rotated_block->color = block->color;

    return rotated_block;
}

struct Block *get_random_block(struct BlockList *block_list)
{
    int random_block = rand() % block_list->elements_number;

    struct Block *block = get(block_list, random_block);

    return clone_block(block);
}
