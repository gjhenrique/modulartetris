#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "matrix_file.h"
#include "model.h"
#include "block_list.h"

/*
struct Block *create_default_block(const bool matrix[][2], int row_size, int col_size, enum Color color)
{
    int i, j;
    bool **double_pointer =  malloc_double_pointer(row_size, col_size);

    for (i = 0; i < row_size; i++)
    {
        for (j = 0; j < col_size; j++)
        {
            double_pointer[i][j] = matrix[i][j];
        }
    }

    struct Block *block = build_block(double_pointer, row_size, col_size, color);

    free(double_pointer);

    return block;
}



bool **build_format_map(bool **format_map, int row_size, int col_size)
{
    int i, j;

    bool **new_format_map = malloc_double_pointer(row_size, col_size);
   
    for (i = 0; i < row_size; ++i)
    {
        for (j = 0; j < col_size; ++j)
        {
            new_format_map[i][j] = format_map[i][j];
        }
    }

    return new_format_map;
}

struct Block *build_block(bool **format_map, int row_size, int col_size, enum Color color) 
{
    int i, j;
    struct Block *block;
    
    block = (struct Block *) malloc(sizeof(struct Block));

    block->format_bool = build_format_map(format_map, row_size, col_size);
    block->row_size = row_size;
    block->col_size = col_size;

    return block; 
}

struct Block *clone_block(struct Block *block) 
{
    return build_block(block->format_bool, block->row_size, block->col_size, block->color);
}

struct Block *rotate_block_clockwise(struct Block *format) 
{
    int col_size, row_size;
    struct Block *new_format = clone_block(format);

    // TODO
    return new_format;
}

*/

/*
int counter = 0;

void copy_matrix(void *dest, void *src, int row_size, int col_size)
{
    memcpy(dest, src, row_size * col_size * sizeof(bool));
}

// Temp: Depois deletar
void print_matrix(bool **new_matrix, int row_size, int col_size)
{
    int i, j;

    for (i = 0; i < col_size; ++i)
    {
        for (j = 0; j < row_size; ++j)
        {
           printf("%d\t", new_matrix[i][j]); 
        }
        printf("\n");
    }

    printf("\n\n");
}

struct Block *create_block(const bool **matrix, int row_size, int col_size, enum Color color)
{
    copy_matrix(&matrix, row_size, col_size); 

    print_matrix(new_matrix, row_size, col_size);

    struct Block *block = malloc(sizeof(struct Block));

    block->color = color;

    return block;
}
*/

int main()
{
    struct BlockList *block_list = read_from_file("default_blocks");    
        
    struct Block *block = get(block_list, 1); 
    print_matrix(block->matrix);

    return 0;
}

