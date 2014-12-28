#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define TOTAL_TETRIMINOES 7

enum Color {
    RED, BLUE, GREEN, ORANGE, PINK, BROWN, PURPLE
};

struct Block {
    bool **format_bool;
    int row_size;
    int col_size;
    enum Color color;
};

struct Board {
    int height;
    int width;

    // Pointer to Block
    struct Block *current_block;

    struct Block *next_block;

    // Vector of pointers of blocks
    struct Block **available_block;

    // Matrix of a pointers of blocks
    enum Color **visited;
};

// Col 2 for compiler stop complaining when create_default_block is called
const bool STRAIGHT_POLYOMINO[4][2] = {1, 1, 1, 1}; 

const bool SQUARE_POLYOMINO[2][2] = {{1, 1}, {1, 1}};

const bool T_POLYOMINO[3][2] = {{1, 0}, {1, 1}, {1, 0}};

const bool S_POLYOMINO[3][2] = {{1, 0}, {1, 1}, {0, 1}};

const bool Z_POLYOMINO[3][2] = {{0, 1}, {1, 1}, {1, 0}};

const bool L_POLYOMINO[3][2] = {{1, 0}, {1, 0}, {1, 1}};

const bool J_POLYOMINO[3][2] = {{1, 1}, {1, 0}, {1, 0}};

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
/*
 * Block instatiation
 */
struct Block *create_block(const bool **matrix, int row_size, int col_size, enum Color color)
{
    bool **new_matrix
    copy_m(&matrix, row_size, col_size); 

    print_matrix(new_matrix, row_size, col_size);

    struct Block *block = malloc(sizeof(struct Block));

    block->format_bool = block;
    block->row_size = row_size;
    block->col_size = col_size;
    block->color = color;

    return block;
}

void free_block(struct Block *block) 
{
    int i;

    for(i = 0; i < block->row_size; i++) {
        free(block->format_bool[i]);
    }

    free(block->format_bool);
    free(block);
}

void free_board(struct Board *board)
{    
    int i;
    for (i = 0; i < TOTAL_TETRIMINOES; i++)
    {
        free_block(board->available_block[i]);
    }

    free(board->available_block);
    free(board);
}

int main()
{
    const bool **A = {1, 1, 1, 1}; 

    struct Board *board = (struct Board *) malloc(sizeof(struct Board));
    board->available_block = (struct Block **) malloc(sizeof(struct Block *) * TOTAL_TETRIMINOES);

    board->available_block[0] = create_block(STRAIGHT_POLYOMINO, 4, 1, RED);

    board->available_block[1] = create_block(SQUARE_POLYOMINO, 2, 2, BLUE);
    board->available_block[2] = create_block(T_POLYOMINO, 3, 2, GREEN);
    board->available_block[3] = create_block(S_POLYOMINO, 3, 2, ORANGE);
    board->available_block[4] = create_block(Z_POLYOMINO, 3, 2, PINK);
    board->available_block[5] = create_block(L_POLYOMINO, 3, 2, BROWN);
    board->available_block[6] = create_block(J_POLYOMINO, 3, 2, PURPLE);
    
    
    int next_piece = rand() % TOTAL_TETRIMINOES;
    struct Block *block = board->available_block[next_piece]; 

    // Disable for now
    //free_board(board);

    return 0;
}

