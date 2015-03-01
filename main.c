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

    struct Board *board = create_board(10, 10); 
    
    print_matrix(board->current_block->matrix);
    for (i = 0; i < 5; ++i)
    {
        next_move(board);
    }

    print_board(board);
    
    return 0;
}

