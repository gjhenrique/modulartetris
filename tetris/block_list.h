#ifndef BLOCK_LIST_H
#define BLOCK_LIST_H

#include "model.h"

struct BlockList
{
    struct BlockNode *HEAD;
    int elements_number;
};

struct BlockNode
{
    struct Block *block;
    struct BlockNode *next;
};

struct BlockList *create_list();

void add(struct BlockList *list, struct Block *block);

struct Block *get(struct BlockList *list, int index);

#endif /* BLOCK_LIST_H */
