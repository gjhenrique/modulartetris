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

void print_matrix(struct Block *block)
{
    for (int i = 0; i < block->row_size; ++i)
    {
        for (int j = 0; j < block->col_size; ++j)
        {
         printf("%d\t", block->values[i][j]);
        }
        printf("\n");
    }

    printf("\n\n");
}

void print_board(struct Board *board)
{
    for (int i = 0; i < board->height; i++)
    {
        for (int j = 0; j < board->width; j++)
        {
            printf("%d", board->board_values[i][j]);
            printf ("\t");
        }
        printf("\n");
    }
    printf("\n\n");
}

struct Block *create_block(int row_size, int col_size) {

    struct Block* block =  (struct Block *)malloc(sizeof(struct Block));

    block->col_size = col_size;
    block->row_size = row_size;
    block->values = malloc_collor_matrix(row_size, col_size);

    return  block;
}

void free_block(struct Block *block)
{
    for(int i = 0; i < block->row_size; i++) {
        free(block->values[i]);
    }

    free(block->values);
    free(block);
}

void free_board(struct Board *board)
{
    free_list(board->block_list);

    if(board->current_block)
        free_block(board->current_block);

    if(board->next_block)
        free_block(board->next_block);

    for (int i = 0; i < board->height; i++)
    {
        free(board->board_values[i]);
    }

    free(board->board_values);

    free(board);
}

void free_list(struct BlockList* block_list)
{
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

struct Block *clone_block(struct Block *block)
{
    struct Block *new_block = create_block(block->row_size, block->col_size);

    for (int i = 0 ; i < block->row_size; i++)
    {
        for (int j = 0; j < block->col_size; j++)
        {
            new_block->values[i][j] = block->values[i][j];
        }
    }

    new_block->color = block->color;
    return new_block;
}

enum Color **malloc_collor_matrix(int width, int height)
{
    enum Color **colors = (enum Color **) calloc(height, sizeof(enum Color *));

    for (int i = 0; i < height; i++)
    {
        colors[i] = (enum Color *) calloc(width, sizeof(enum Color));
    }

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; j++)
        {
            colors[i][j] = NONE;
        }
    }

    return colors;
}
