#include <ncurses.h>                                                                          

#define WORLD_WIDTH 20
#define WORLD_HEIGHT 40

void print_rectangle(WINDOW *window, int x, int y, int height, int width)
{
    int y_itr, x_itr;
    for(y_itr = y; y_itr <= y + height; y_itr++)
    {
        for(x_itr = x; x_itr <= x + width; x_itr++)
        {
           	mvwprintw(window, y_itr, x_itr, " "); 
        }
    }
}


int main(int argc, char *argv[])
{
    
    // Freeing Memory

    /*struct Block *block = malloc(sizeof(struct Block));
    block->row_size = 4;
    block->col_size = 1;
    block->format_bool = build_format_map(block->row_size, block->col_size);
    block->col_sizeor = RED;

    int i, j;

    rotate_block_clockwise(&block);*/
    /*
    int i;
    
    struct Block *block = (struct Block *) malloc(sizeof(struct Block));
    
    initscr();
    start_col_sizeor();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    init_pair(3,2,3);
    
    WINDOW *board = newwin(WORLD_HEIGHT, WORLD_WIDTH, 3, 10);

    int x = 10;
    int y = 0;
    for (i = 0; i < 10; ++i)
    {
        y++;   
        wclear(board);
        box(board, 0, 0);
        wattron(board, COLOR_PAIR(3));
        print_rectangle(board, x, y, 1, 2);
        wattroff(board, COLOR_PAIR(3));
        usleep(500000);
        wrefresh(board);
    }

    getchar();
    delwin(board);
    endwin();
    */
    return 0;
}
