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


struct Block *populate_block(char **saved_ptr, int row_size, int col_size, int color_index)
{
    char *line = NULL;

    struct Block *block = create_block(row_size, col_size);
    block->color = (enum Color) color_index;

    for (int i = 0; i < block->row_size; ++i) {
        line = strtok_r(NULL, "\n", saved_ptr);
        for (int j = 0, k = 0; k < block->col_size; j += 2, k++) {
          // If the line is 0, fill with NONE
          block->values[i][k] = to_digit(line[j]) ? (enum Color) color_index : NONE;
        }
    }

    return block;
}

struct Block *read_block(char **saved_ptr, char *line, int color_index)
{
    int i, j, k, row_size;

    if (strlen(line) != 1) {
        fprintf(stderr, "Line %s is malformed", line);
        exit(EXIT_FAILURE);
    }

    row_size = to_digit(line[0]);

    // Blocks should be a square matrix
    return populate_block(saved_ptr, row_size, row_size, color_index);
}

struct BlockList *read_from_string(char blocks_string[])
{
    char *line = NULL;
    char *saved_ptr;
    size_t len = 0;
    ssize_t read;

    int color_index = 0;
    struct BlockList *block_list = create_list();
    for (line = strtok_r(blocks_string, "\n", &saved_ptr);
         line != NULL; line = strtok_r(NULL, "\n", &saved_ptr)) {
        if (!(strlen(line) == 0 || line[0] == '#' || !isdigit(line[0]))) {

            // Jumping color NONE
            while (color_index == NONE || color_index == GHOST)
                color_index++;

            struct Block *block = read_block(&saved_ptr, line, color_index);
            color_index++;

            add(block_list, block);
        }
    }

    return block_list;
}

// ** Array of pointers
struct BlockList *read_from_file(char *file_name)
{
    FILE *file = fopen(file_name, "r");

    if (file == NULL) {
        fprintf(stderr, "Cannot read file %s", file_name);
    }

    int fd = fileno(file);
    struct stat stat_file;
    fstat(fd, &stat_file);

    int size = stat_file.st_size;
    char blocks_string[size + 1];
    fread(blocks_string, sizeof(char), size, file);

    struct BlockList *block_list = read_from_string(blocks_string);
    fclose(file);

    return block_list;
}

