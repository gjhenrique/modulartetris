#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "matrix_file.h"
#include "model.h"
#include "block_list.h"
#include "boilerplate.h"

int main()
{
    int i, j;

    struct BlockList *block_list = read_from_file("default_blocks"); 
        
    for (i = 0; i < block_list->elements_number; ++i)
    {
        struct Block *block = get(block_list, i);
        print_matrix(block->matrix);
        struct Block *new_block = block;

        for (j = 0; j < 1; j++) {
            struct Block *tmp_block = rotate_block_anticlockwise(new_block);
            new_block = tmp_block;
            tmp_block = NULL;
        }
        print_matrix(new_block->matrix);
    }

    return 0;
}
