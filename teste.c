#include <stdio.h>

#include "block_list.c"

struct Block *create_block(int i)
{
    struct Block *bl = malloc(sizeof(struct Block)); 
    bl->color= i;
}

int main(int argc, char *argv[])
{

    struct BlockList *list = create_list();    
    int i = 0;

    for (i = 0; i < 6; i++)
    {
        add(list, create_block(i));    
    }

    for (i = 0; i < 6; i++)
    {
        struct Block *b = get(list, i);
        printf("%d", b->color);
    }

}


