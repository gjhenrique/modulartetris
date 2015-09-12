#include <stdbool.h>

#include "greatest.h"
#include "../block.h"
#include "../block_list.h"
#include "../matrix_file.h"
#include "../boilerplate.h"
#include "../model.h"
#include "../board.h"

int check_matrix_equals(struct Matrix *matrix, struct Matrix *tested_matrix)
{
    for (int i = 0; i < matrix->row_size; ++i)
    {
        for (int j = 0; j < matrix->col_size; ++j)
        {
            if (matrix->values[i][j] != tested_matrix->values[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

// God bless VLA
int check_board_equals(struct Board *board, bool tested_board[board->width][board->height])
{
    for(int i = 0; i < board->width; i++)
    {
        for(int j = 0; j < board->height; j++)
        {
          if((bool) board->visited[i][j] != tested_board[i][j])
              return false;
        }
    }
    return true;
}

int assert_blocks_rotation(struct Block *block, struct BlockList *block_list)
{
    ASSERTm("Normal", check_matrix_equals(block->matrix, get(block_list, 0)->matrix));

    block = rotate_block(block, 1);
    ASSERTm("1 clock", check_matrix_equals(block->matrix, get(block_list, 1)->matrix));

    block = rotate_block(block, 1);
    ASSERTm("2 clock", check_matrix_equals(block->matrix, get(block_list, 3)->matrix));

    block = rotate_block(block, 1);
    ASSERTm("3 clock", check_matrix_equals(block->matrix, get(block_list, 2)->matrix));

    block = rotate_block(block, 1);
    ASSERTm("4 clock", check_matrix_equals(block->matrix, get(block_list, 0)->matrix));

    block = rotate_block(block, 0);
    ASSERTm("1 anticlock", check_matrix_equals(block->matrix, get(block_list, 2)->matrix));
    PASS();
}

/*
 * This test works like this:
 * The function assert_blocks_rotation performs several rotations over target_block and compare them * with the rotation_blocks.
 * The convention of the positions of rotation_blocks is the following:
 * [0] -> The "normal" block
 * [1] -> The normal block rotated clockwise
 * [2] -> The normal block rotated anticlockwise
 * [3] -> The normal block rotated twice clockwise or anticlockwise
 */
int test_blocks_rotation(char target_block_str[], char rotation_blocks_str[])
{
    struct BlockList *target_block_list = read_from_string(target_block_str);
    struct Block *target_block = get(target_block_list, 0);

    struct BlockList *rotation_blocks = read_from_string(rotation_blocks_str);

    CHECK_CALL(assert_blocks_rotation(target_block, rotation_blocks));

    free_list(target_block_list);
    free_list(rotation_blocks);
    PASS();

}

TEST test_straight(void)
{
    char rotation_blocks[] = "4\n0 0 0 0\n1 1 1 1\n0 0 0 0\n0 0 0 0\n"
                          "4\n0 0 1 0\n0 0 1 0\n0 0 1 0\n0 0 1 0\n"
                          "4\n0 1 0 0\n0 1 0 0\n0 1 0 0\n0 1 0 0\n"
                          "4\n0 0 0 0\n0 0 0 0\n1 1 1 1\n0 0 0 0";

    char target_block[] = "4\n0 0 0 0\n1 1 1 1\n0 0 0 0\n0 0 0 0";

    CHECK_CALL(test_blocks_rotation(target_block, rotation_blocks));

    PASS();
}

TEST test_square(void)
{
    char rotation_blocks[] = "2\n1 1\n1 1\n"
                             "2\n1 1\n1 1\n"
                             "2\n1 1\n1 1\n"
                             "2\n1 1\n1 1\n";
    char target_block[] = "2\n1 1\n1 1";

    CHECK_CALL(test_blocks_rotation(target_block, rotation_blocks));
    PASS();
}

TEST test_T(void)
{
    char rotation_blocks[] = "3\n1 1 1\n0 1 0\n0 0 0\n"
                             "3\n0 0 1\n0 1 1\n0 0 1\n"
                             "3\n1 0 0\n1 1 0\n1 0 0\n"
                             "3\n0 0 0\n0 1 0\n1 1 1";
    char target_block[] = "3\n1 1 1\n0 1 0\n0 0 0";

    CHECK_CALL(test_blocks_rotation(target_block, rotation_blocks));
    PASS();
}

TEST test_S(void)
{
    char rotation_blocks[] = "3\n1 1 0\n0 1 1\n0 0 0\n"
                             "3\n0 0 1\n0 1 1\n0 1 0\n"
                             "3\n0 1 0\n1 1 0\n1 0 0\n"
                             "3\n0 0 0\n1 1 0\n0 1 1";

    char target_block[] = "3\n1 1 0\n0 1 1\n0 0 0";

    CHECK_CALL(test_blocks_rotation(target_block, rotation_blocks));
    PASS();
}

TEST test_Z(void)
{
    char rotation_blocks[] = "3\n0 1 1\n1 1 0\n0 0 0\n"
                             "3\n0 1 0\n0 1 1\n0 0 1\n"
                             "3\n1 0 0\n1 1 0\n0 1 0\n"
                             "3\n0 0 0\n0 1 1\n1 1 0";

    char target_block[] = "3\n0 1 1\n1 1 0\n0 0 0";

    CHECK_CALL(test_blocks_rotation(target_block, rotation_blocks));
    PASS();
}

TEST test_L(void)
{
    char rotation_blocks[] = "3\n1 1 1\n0 0 1\n0 0 0\n"
                             "3\n0 0 1\n0 0 1\n0 1 1\n"
                             "3\n1 1 0\n1 0 0\n1 0 0\n"
                             "3\n0 0 0\n1 0 0\n1 1 1";

    char target_block[] = "3\n1 1 1\n0 0 1\n0 0 0";

    CHECK_CALL(test_blocks_rotation(target_block, rotation_blocks));
    PASS();
}

TEST test_J(void)
{
    char rotation_blocks[] = "3\n1 1 1\n1 0 0\n0 0 0\n"
                             "3\n0 1 1\n0 0 1\n0 0 1\n"
                             "3\n1 0 0\n1 0 0\n1 1 0\n"
                             "3\n0 0 0\n0 0 1\n1 1 1";

    char target_block[] = "3\n1 1 1\n1 0 0\n0 0 0";

    CHECK_CALL(test_blocks_rotation(target_block, rotation_blocks));
    PASS();
}

SUITE(block)
{
    RUN_TEST(test_straight);
    RUN_TEST(test_square);
    RUN_TEST(test_T);
    RUN_TEST(test_S);
    RUN_TEST(test_Z);
    RUN_TEST(test_L);
    RUN_TEST(test_J);
}

TEST test_clear_line(void)
{
    bool board_test[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}};
    bool board_empty[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

    char straight_block[] = "4\n0 0 0 0\n1 1 1 1\n0 0 0 0\n0 0 0 0";
    struct BlockList *block_list = read_from_string(straight_block);

    struct Board *board = create_board(4, 4, block_list);

    move_to_bottom(board);
    ASSERT(check_board_equals(board, board_empty));

    for (int i = 0; i < 6; i++)
    {
        next_move(board);
    }
    ASSERT(check_board_equals(board, board_test));

    next_move(board);
    ASSERT(check_board_equals(board, board_empty));

    free_board(board);
    PASS();
}

TEST test_blocks_direction(void)
{
    bool left_board[5][5] = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {1, 1, 1, 1, 0}, {0, 0, 0, 0, 0}};
    bool right_board[5][5] = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 1, 1, 1, 1}, {0, 0, 0, 0, 0}};

    char square_block[] = "4\n0 0 0 0\n1 1 1 1\n0 0 0 0\n0 0 0 0";
    struct BlockList *block_list = read_from_string(square_block);

    struct Board *board = create_board(5, 5, block_list);

    for(int i = 0; i < 6; i++)
    {
        next_move(board);
    }

    for(int i = 0; i < 50; i++)
    {
        move_to_left(board);
    }
    ASSERT(check_board_equals(board, left_board));

    for(int i = 0; i < 50; i++)
    {
        move_to_right(board);
    }
    ASSERT(check_board_equals(board, right_board));

    free_board(board);
    PASS();
}

TEST test_game_over(void)
{
    char square_block[] = "4\n0 0 0 0\n1 1 1 1\n0 0 0 0\n0 0 0 0";
    struct BlockList *block_list = read_from_string(square_block);

    struct Board *board = create_board(5, 5, block_list);

    move_to_bottom(board);
    move_to_bottom(board);
    move_to_bottom(board);
    move_to_bottom(board);
    move_to_bottom(board);

    ASSERT_FALSEm("Board should not have ended", board->is_game_over);

    // 3 moves because of the two down "0 0 0 0"
    next_move(board);
    next_move(board);
    next_move(board);

    ASSERTm("Game should have ended", board->is_game_over);

    free_board(board);
    PASS();
}

TEST test_board_rotation(void)
{

    bool anticlock_rotation_board[5][5] = {{0, 1, 0, 0, 0}, {0, 1, 0, 0, 0}, {0, 1, 0, 0, 0}, {0, 1, 0, 0, 0}, {0, 0, 0, 0, 0}};
    bool clock_rotation_board[5][5] = {{0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 1, 0, 0}, {0, 0, 0, 0, 0}};
    bool left_rotation_board[5][5] = {{1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {1, 0, 0, 0, 0}, {0, 0, 0, 0, 0}};
    bool two_rotation_board[5][5] = {{0, 1, 0, 0, 0}, {1, 1, 0, 0, 0}, {1, 1, 0, 0, 0}, {1, 1, 0, 0, 0}, {1, 0, 0, 0, 0}};

    char square_block[] = "4\n0 0 0 0\n1 1 1 1\n0 0 0 0\n0 0 0 0";
    struct BlockList *block_list = read_from_string(square_block);

    struct Board *board = create_board(5, 5, block_list);

    for(int i = 0; i < 4; i++)
    {
        next_move(board);
    }

    rotate_clockwise(board);
    ASSERT(check_board_equals(board, clock_rotation_board));

    rotate_clockwise(board);
    rotate_clockwise(board);
    ASSERT(check_board_equals(board, anticlock_rotation_board));

    move_to_left(board);
    // It should not be possible to rotate because the block extrapolates the left border
    rotate_anticlockwise(board);
    ASSERT(check_board_equals(board, left_rotation_board));
    move_to_bottom(board);

    rotate_anticlockwise(board);
    for(int i = 0; i < 4; i++)
    {
        next_move(board);
    }

    rotate_clockwise(board);
    ASSERT(check_board_equals(board, two_rotation_board));

    PASS();
}

TEST test_fit_blocks(void)
{
    bool board1[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 1, 1}};
    bool board2[4][4] = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};

    char blocks[] = "3\n0 0 0\n0 0 1\n1 1 1\n2\n0 1 \n0 1\n2\n1 1\n0 0";
    struct BlockList *block_list = read_from_string(blocks);
    struct Board *board = create_board(4, 4, block_list);

    board->current_block = clone_block(get(block_list,0));
    move_to_left(board);
    move_to_bottom(board);
    board->current_block = clone_block(get(block_list,1));
    move_to_right(board);
    move_to_bottom(board);
    ASSERT(check_board_equals(board, board1));

    board->current_block = clone_block(get(block_list,2));
    move_to_left(board);
    move_to_bottom(board);
    ASSERT(check_board_equals(board, board2));
    PASS();
}

SUITE(board)
{
    RUN_TEST(test_clear_line);
    RUN_TEST(test_blocks_direction);
    RUN_TEST(test_game_over);
    RUN_TEST(test_board_rotation);
   RUN_TEST(test_fit_blocks);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv)
{
    GREATEST_MAIN_BEGIN();
    RUN_SUITE(block);
    RUN_SUITE(board);
    GREATEST_MAIN_END();
}
