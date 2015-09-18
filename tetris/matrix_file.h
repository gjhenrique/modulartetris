#ifndef MATRIX_FILE_H
#define MATRIX_FILE_H

struct BlockList *read_from_string(char *blocks_string);

struct BlockList *read_from_file(char *file_name);

#endif
