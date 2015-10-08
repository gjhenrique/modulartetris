#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "matrix_file.h"
#include "model.h"
#include "block_list.h"
#include "boilerplate.h"
#include "board.h"

int flag = false;
int main()
{
    struct BlockList *blockList = read_from_file("../default_blocks");
    struct Board *board = create_board(5, 10, blockList);

    for (int i = 0; i < 9; ++i)
    {
        next_move(board);
        if(board->is_game_over)
        {
          free_board(board);
          return 0;
        }

    }

    print_board(board);
    printf("%d -- %d\n", board->ghost_block_y, board->current_block_y);
    next_move(board);
    print_board(board);
    printf("%d -- %d\n", board->ghost_block_y, board->current_block_y);
    next_move(board);
    print_board(board);
    printf("%d -- %d\n", board->ghost_block_y, board->current_block_y);
    next_move(board);
    print_board(board);
    printf("%d -- %d\n", board->ghost_block_y, board->current_block_y);
    flag = false;
    next_move(board);
    print_board(board);
    printf("%d -- %d\n", board->ghost_block_y, board->current_block_y);
    free_board(board);
    return 0;
}

