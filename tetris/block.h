#ifndef BLOCK_H
#define BLOCK_H

#include "model.h"

struct Block *rotate_block(struct Block *block, bool clockwise);

struct Block *get_random_block(struct BlockList *block_list);

#endif /* BLOCK_H */
