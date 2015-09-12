#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <panel.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "model.h"
#include "matrix_file.h"
#include "boilerplate.h"
#include "board.h"

#define STEP 1000
#define BOARD_WIDTH 20
#define BOARD_HEIGHT 10

struct NcursesGame
{
    WINDOW *board_window;
    WINDOW *next_block_window;
    PANEL *board_panel;
    PANEL *next_block_panel;

    struct Board *board;
};

void init_ncurses()
{
    initscr();
    noecho();
    cbreak();
    halfdelay(5);
    keypad(stdscr, true);
    curs_set(false);
    srand(time(NULL));

    if(has_colors())
    {
        start_color();
        init_pair(RED, COLOR_RED, COLOR_RED);
        init_pair(BLUE, COLOR_BLUE, COLOR_BLUE);
        init_pair(GREEN, COLOR_GREEN, COLOR_GREEN);
        init_pair(YELLOW, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(PINK, COLOR_CYAN, COLOR_CYAN);
        init_pair(PURPLE, COLOR_MAGENTA, COLOR_MAGENTA);
        init_pair(ORANGE, COLOR_YELLOW, COLOR_YELLOW);
    }
    else
    {
        fprintf(stderr, "[Your terminal does not support colors");
        endwin();
        exit(1);
    }
}

struct NcursesGame *init_game()
{

    struct NcursesGame *game = malloc(sizeof(struct NcursesGame));
    struct BlockList *blockList = read_from_file("default_blocks");
    struct Board *board = create_board(10, 20, blockList);

    game->board_window = newwin(board->height + 2, board->width + 2, 10, 10);
    game->board_panel = new_panel(game->board_window);

    game->board = board;

    wclear(stdscr);
    wclear(game->board_window);

    return game;
}

void draw_board(struct NcursesGame *game)
{
    wclear(game->board_window);
    box(game->board_window, false, false);

    for(int i = 0; i < game->board->height; i++)
    {
        for(int j = 0; j < game->board->width; j++)
        {
            if (game->board->visited[i][j] > 0)
            {
                int ch = ' ' | COLOR_PAIR(game->board->visited[i][j]);
                mvwaddch(game->board_window, i + 1, j + 1, ch);
            }
        }
    }
}

void free_game(struct NcursesGame *game)
{
    wclear(stdscr);
    endwin();
    free_board(game->board);
    free(game);
}

bool route(struct NcursesGame *game, int ch)
{
    if (ch == 'q')
    {
        free_game(game);
        endwin();
        return false;
    }

    switch (ch)
    {
    case 'k': case KEY_UP:    rotate_anticlockwise(game->board); break;
    case 'j': case KEY_DOWN:  next_move(game->board); break;
    case 'h': case KEY_LEFT:  move_to_left(game->board) ; break;
    case 'l': case KEY_RIGHT: move_to_right(game->board); break;
    case ' ':                 move_to_bottom(game->board); break;
    default: break;
    }

  return true;
}

int main(int argc, char *argv[])
{
    init_ncurses();
    struct NcursesGame *game = init_game();
    for(int i = 0; i < 10; i++)
    {
        next_move(game->board);
    }

    struct timeval prev, now;
    int diff;
    gettimeofday(&prev, NULL);

    // Game loop "stolen" from https://github.com/theabraham/terminal-tetris
    while (route(game, getch())) {

      gettimeofday(&now, NULL);
      diff = ((now.tv_sec-prev.tv_sec)*1000)+((now.tv_usec-prev.tv_usec)/1000);
      //if (diff >= game->step && (!game.ended && !game.paused)) {
      if (diff >= STEP)
      {
          next_move(game->board);
          gettimeofday(&prev, NULL);
      }

      // draw end screen if the game's over, otherwise draw our board (unless paused)
      /*if ((game.ended = istopped())) {
        drawend();
      } else 
        if (!game.paused) {
        addscore(clrcols())*/

      draw_board(game);

      // reflect changes in our panels onscreen
      update_panels();
      doupdate();
    }

    return 0;
}
