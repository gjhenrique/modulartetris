#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <panel.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "model.h"
#include "board.h"
#include "matrix_file.h"
#include "boilerplate.h"

#define STEP 1000
#define BOARD_TOP(board) ((LINES-board->height)/2)
#define BOARD_LEFT(board) ((COLS-board->width)/2)
#define HOLD_HEIGHT  4
#define HOLD_WIDTH   6
#define SIDE_HEIGHT  17
#define SIDE_WIDTH   25
#define PAD          2

struct NcursesGame
{
    WINDOW *board_window;
    WINDOW *next_block_window;
    WINDOW *side_window;
    PANEL *board_panel;
    PANEL *next_block_panel;
    PANEL *side_panel;
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
        init_pair(GHOST, COLOR_WHITE, COLOR_WHITE);
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

struct NcursesGame *init_game(int width, int height, char *file_name)
{
    struct NcursesGame *game = malloc(sizeof(struct NcursesGame));
    struct Board *board = create_board_file(width, height, file_name);

    game->board_window = newwin(board->height + 2, board->width + 2, BOARD_TOP(board), BOARD_LEFT(board));
    game->next_block_window = newwin(HOLD_HEIGHT + 2, HOLD_WIDTH + 2, BOARD_TOP(board), BOARD_LEFT(board) + board->width + 2);
    game->side_window = newwin(SIDE_HEIGHT+PAD, SIDE_WIDTH+PAD, BOARD_TOP(board)+HOLD_HEIGHT+2, BOARD_LEFT(board)+board->width+3);

    game->board_panel = new_panel(game->board_window);
    game->next_block_panel = new_panel(game->next_block_window);
    game->side_panel = new_panel(game->side_window);

    game->board = board;

    wclear(stdscr);
    wclear(game->board_window);

    return game;
}

void draw_board(struct NcursesGame *game)
{
    wclear(game->board_window);
    wclear(game->next_block_window);

    box(game->board_window, false, false);
    box(game->next_block_window, false, false);

    for(int i = 0; i < game->board->height; i++)
    {
        for(int j = 0; j < game->board->width; j++)
        {
            if (game->board->board_values[i][j] > 0)
            {
                int ch = ' ' | COLOR_PAIR(game->board->board_values[i][j]);
                mvwaddch(game->board_window, i + 1, j + 1, ch);
            }
        }
    }

    struct Block *next_block = game->board->next_block;
    for (int i = 0; i < next_block->col_size; i++)
    {
      for(int j = 0; j < next_block->row_size; j++)
      {
          int ch = ' ' | COLOR_PAIR(next_block->values[i][j] ? next_block->color : NONE);
          mvwaddch(game->next_block_window, i + 1, j + 2, ch);
      }
    }
}

void draw_side(struct NcursesGame *game)
{
    wclear(game->side_window);
    wattrset(game->side_window, A_BOLD);
    mvwprintw(game->side_window, 0, 0, "Score:");
    wattrset(game->side_window, A_NORMAL);
    mvwprintw(game->side_window, 1, 0, "%i", game->board->score);
    mvwprintw(game->side_window, 2, 0, "Rows:");
    mvwprintw(game->side_window, 3, 0, "%i", game->board->rows);
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
    if (ch == 'q' || game->board->is_game_over)
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
    int width = 10;
    int height = 15;
    char *file_name = "default_blocks";

    if (argc >= 2)
    {
        file_name = argv[1];
    }

    if (argc == 4)
    {
        width = atoi(argv[2]);
        height = atoi(argv[3]);
    }

    init_ncurses();
    struct NcursesGame *game = init_game(width, height, file_name);

    struct timeval prev, now;
    int diff;
    gettimeofday(&prev, NULL);

    while (route(game, getch())) {

      gettimeofday(&now, NULL);
      diff = ((now.tv_sec-prev.tv_sec)*1000)+((now.tv_usec-prev.tv_usec)/1000);
      if (diff >= STEP)
      {
          next_move(game->board);
          gettimeofday(&prev, NULL);
      }

      draw_board(game);
      draw_side(game);

      update_panels();
      doupdate();
    }

    return 0;
}
