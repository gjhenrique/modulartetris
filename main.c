#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "matrix_file.h"
#include "model.h"
#include "block_list.h"
#include "boilerplate.h"
#include "board.h"

int main()
{

    struct Board *board = create_board(5, 100);

    for (int i = 0; i < 90; ++i)
    {
        /*for (j = 0; j < 10; j++)
        {
            if(i % 2 == 0)
                move_to_right(board);
            else
               move_to_left(board);
        }*/

        //move_to_bottom(board);
        next_move(board);
        if(board->is_game_over)
        {

            free_board(board);
            return 0;
        }

        //print_board(board);
    }

    free_board(board);
    return 0;
}

