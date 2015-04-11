#include "matrix_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "boilerplate.h"
#include "block_list.h"

struct Matrix *populate_matrix(FILE *file, int row_size, int col_size)
{
    char *line = NULL;
    size_t len = 0;

    struct Matrix *matrix = create_matrix(row_size, col_size);
    for (int i = 0; i < matrix->row_size; ++i)
    {
        getline(&line, &len, file);
        for(int j = 0, k = 0; k < matrix->col_size; j+=2, k++)
        {
            matrix->values[i][k] = to_digit(line[j]);
        }
        free(line);
        line = NULL;
    }

   return matrix;
}

struct Matrix *read_matrix(FILE *file, char *line)
{
    int i, j, k, row_size, col_size;

    if (strlen(line) != 2)
    {
        fprintf(stderr, "Line %s is malformed", line);
        exit(EXIT_FAILURE);
    }

    row_size = to_digit(line[0]);

    return populate_matrix(file, row_size, row_size);
}

struct BlockList *read_lines(FILE *file)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    int color_index = 0;
    struct BlockList *block_list = create_list();

    while ((read = getline(&line, &len, file)) != -1) {

        if(!(strlen(line) == 1 || line[0] == '#'))
        {
            struct Block *block = malloc(sizeof(struct Block));

            block->matrix = read_matrix(file, line);

            // Jumping color NONE
            if(color_index == NONE)
                color_index++;

            block->color = color_index++;

            add(block_list, block);
        }
    }

    if (line)
    {
        free(line);
    }

    return block_list;
}

// ** Array of pointers
struct BlockList *read_from_file(char *file_name)
{

    FILE *file = fopen(file_name, "r");

    if (file == NULL)
    {
        fprintf(stderr, "Cannot read file %s", file_name);
    }

    struct BlockList *block_list = read_lines(file);

    fclose(file);

    return block_list;
}

