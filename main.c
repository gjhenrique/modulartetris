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
    
    for (i = 0; i < 1000; ++i)
    {
        for (j = 0; j < 10; j++)
        {
            if(i % 2 == 0)
                move_to_right(board);
            else
               move_to_left(board);
        }
        
        move_to_bottom(board);

        if(board->is_game_over)
        {
            print_matrix(board->current_block->matrix);
                printf("%s", (i % 2 == 0) ? "RIGHT" : "LEFT");
            return 0;
        }
        
        print_board(board);
    }

    return 0;
}

