#include <stdio.h>
#include <stdlib.h>

#include "block_list.h"

struct BlockList *create_list()
{
    struct BlockList *block_list = malloc(sizeof(struct BlockList));

    block_list->HEAD = NULL;
    block_list->elements_number = 0;

    return block_list;
}

void add(struct BlockList *list, struct Block *block)
{
    struct BlockNode *block_node = malloc(sizeof(struct BlockNode));
    block_node->block = block;
    block_node->next = NULL;

    if (list->HEAD == NULL)
    {
        list->HEAD = block_node;
    }
    else
    {
        struct BlockNode *tmp = list->HEAD;
        // Going to the final of the list
        for(int i = 0; i < list->elements_number - 1; i++)
        {
            tmp = tmp->next;
        }

        tmp->next = block_node;
        block_node->next = NULL;
    }

    list->elements_number++;
}

struct Block *get(struct BlockList *block_list, int index)
{
    int i;

    if (index > block_list->elements_number)
    {
        fprintf(stderr, "%d is bigger than the current number of elements in the list: %d", index, block_list->elements_number);
        return NULL;
    }

    if (block_list->HEAD == NULL)
    {
        fprintf(stderr, "HEAD is empty");
    }

    struct BlockNode *block_node = block_list->HEAD;

    for (i = 0; i < index; ++i)
    {
        block_node = block_node->next;
    }

    return block_node->block;
}
