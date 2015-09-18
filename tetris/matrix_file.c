#include "matrix_file.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include "boilerplate.h"
#include "block_list.h"


struct Matrix *populate_matrix(char **saved_ptr, int row_size, int col_size)
{
    char *line = NULL;
    size_t len = 0;

    struct Matrix *matrix = create_matrix(row_size, col_size);
    for (int i = 0; i < matrix->row_size; ++i)
    {
        line = strtok_r(NULL, "\n", saved_ptr);
        for(int j = 0, k = 0; k < matrix->col_size; j+=2, k++)
        {
            matrix->values[i][k] = to_digit(line[j]);
        }
    }

    return matrix;
}

struct Matrix *read_matrix(char **saved_ptr, char *line)
{
    int i, j, k, row_size;

    if (strlen(line) != 1)
    {
      printf("%d\n", strlen(line));
        fprintf(stderr, "Line %s is malformed", line);
        exit(EXIT_FAILURE);
    }

    row_size = to_digit(line[0]);

    // Blocks should be a square matrix
    return populate_matrix(saved_ptr, row_size, row_size);
}

struct BlockList *read_from_string(char blocks_string[])
{
    char *line = NULL;
    char *saved_ptr;
    size_t len = 0;
    ssize_t read;

    int color_index = 0;
    struct BlockList *block_list = create_list();
    for(line = strtok_r(blocks_string, "\n", &saved_ptr); line != NULL; line = strtok_r(NULL, "\n", &saved_ptr))
    {
        if(!(strlen(line) == 0 || line[0] == '#' || !isdigit(line[0])))
        {
            struct Block *block = malloc(sizeof(struct Block));

            block->matrix = read_matrix(&saved_ptr, line);

            // Jumping color NONE
            if(color_index == NONE)
                color_index++;

            block->color = color_index++;

            add(block_list, block);
        }
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

    int fd = fileno(file);
    struct stat stat_file;
    fstat(fd, &stat_file);

    int size = stat_file.st_size;
    char blocks_string[size +1];
    fread(blocks_string, sizeof(char), size, file);

    struct BlockList *block_list = read_from_string(blocks_string);
    fclose(file);

    return block_list;
}

