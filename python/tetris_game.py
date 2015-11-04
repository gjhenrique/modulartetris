import sys
import pygame
import modular_tetris

WINDOW_WIDTH = 300
WINDOW_HEIGHT = 400
BOARD_WIDTH = 228
BOARD_HEIGHT = 400
NEXTBLOCK_WIDTH = 48
NEXTBLOCK_HEIGHT = 48
TETRIS_WIDTH = 12
TETRIS_HEIGHT = 20
TICKS_PER_SECOND = 1
SKIP_TICKS = 1000 / TICKS_PER_SECOND
MAX_FRAMESKIP = 10
# Warning: Width and height factors can't be integers, because pygame.Rect typecasts float to int
BOARD_WIDTH_FACTOR = BOARD_WIDTH / TETRIS_WIDTH
BOARD_HEIGHT_FACTOR = BOARD_HEIGHT / TETRIS_HEIGHT

color_dict = {
    modular_tetris.NONE: (255, 255, 255),
    modular_tetris.GHOST: (128, 128, 128),
    modular_tetris.RED: (255, 51, 51),
    modular_tetris.BLUE: (0, 0, 204),
    modular_tetris.GREEN: (0, 102, 0),
    modular_tetris.YELLOW: (255, 255, 0),
    modular_tetris.PINK: (204, 0, 204),
    modular_tetris.PURPLE: (76, 0, 153),
    modular_tetris.ORANGE: (255, 128, 0)
}

pressed_key_functions = {
    pygame.K_SPACE: modular_tetris.move_to_bottom,
    pygame.K_LEFT: modular_tetris.move_to_left,
    pygame.K_RIGHT: modular_tetris.move_to_right,
    pygame.K_UP: modular_tetris.rotate_clockwise
}


def draw_block(block, width, height, paddingX, paddingY, row_size, col_size):
    for i in range(0, height):
        for j in range(0, width):
            value = modular_tetris.get_color_value(block, i, j)
            y = col_size * i + paddingY
            x = row_size * j + paddingX

            color = color_dict[value]
            pygame.draw.rect(screen, color, pygame.Rect(x, y, row_size, col_size), 0)


def draw_board(board):
    draw_block(board.board_values, board.width, board.height, 0, 0, BOARD_WIDTH_FACTOR, BOARD_HEIGHT_FACTOR)

    next_block_width_factor = NEXTBLOCK_WIDTH / board.next_block.row_size
    next_block_height_factor = NEXTBLOCK_HEIGHT / board.next_block.col_size

    draw_block(board.next_block.values, board.next_block.col_size, board.next_block.row_size,
               BOARD_WIDTH + 20, 20, next_block_width_factor, next_block_height_factor)

if __name__ == "__main__":

    pygame.init()
    screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
    clock = pygame.time.Clock()

    file_name = sys.argv[1] if (len(sys.argv) == 2) else "default_blocks"

    board = modular_tetris.create_board_file(TETRIS_WIDTH, TETRIS_HEIGHT, file_name)

    next_game_tick = pygame.time.get_ticks()
    done = False

    while not done:

        if pygame.QUIT in [e.type for e in pygame.event.get()]:
            done = True

        loops = 0
        # Game loop from  http://www.koonsolo.com/news/dewitters-gameloop/
        while pygame.time.get_ticks() > next_game_tick and loops < MAX_FRAMESKIP:

            modular_tetris.next_move(board)
            screen.fill((0, 0, 0))
            draw_board(board)
            pygame.display.flip()

            next_game_tick += SKIP_TICKS
            loops += 1

        pressed = pygame.key.get_pressed()

        for p in (p for p in pressed_key_functions if pressed[p]):
            pressed_key_functions[p](board)
            draw_board(board)
            pygame.display.flip()

        if board.is_game_over:
            modular_tetris.free_board(board)
            board = modular_tetris.create_board_file(TETRIS_WIDTH, TETRIS_HEIGHT, "default_blocks")

        pygame.time.Clock().tick(8)
