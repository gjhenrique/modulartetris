package br.com.gjhenrique.modulartetris;

public class BoardSwig implements Board {

    private BoardSwigWrap boardPtr;

    public BoardSwig(BoardSwigWrap boardSwigWrap) {
        this.boardPtr = boardSwigWrap;
    }

    @Override
    public boolean isGameOver() {
        return boardPtr.getIs_game_over();
    }

    @Override
    public void nextMove() {
        modular_tetris.next_move(boardPtr);
    }

    @Override
    public void moveToLeft() {
        modular_tetris.move_to_left(boardPtr);
    }

    @Override
    public void moveToRight() {
        modular_tetris.move_to_right(boardPtr);
    }

    @Override
    public void rotateClockwise() {
        modular_tetris.rotate(boardPtr, true);
    }

    @Override
    public void moveToBottom() {
        modular_tetris.move_to_bottom(boardPtr);
    }

    @Override
    public Color getBoardValuesIndex(int col, int row) {
        if (col >= getBoardHeight())
            throw new IllegalArgumentException(col + " is bigger than the board height: " + getBoardHeight());

        if (row >= getBoardWidth())
            throw new IllegalArgumentException(col + " is bigger than the board height: " + getBoardWidth());

        ColorSwigWrap colorSwigWrap = modular_tetris.get_color_value(boardPtr.getBoard_values(), col, row);
        return convertColor(colorSwigWrap);
    }

    @Override
    public int getBoardHeight() {
        return boardPtr.getHeight();
    }

    @Override
    public int getBoardWidth() {
        return boardPtr.getWidth();
    }

    @Override
    public Color getNextBlockIndex(int col, int row) {
        if (col >= getNextBlockHeight())
            throw new IllegalArgumentException(col + " is bigger than the next block Height: " + getNextBlockHeight());

        if (row >= getNextBlockWidth())
            throw new IllegalArgumentException(col + " is bigger than the next block height: " + getNextBlockWidth());

        ColorSwigWrap colorSwigWrap = modular_tetris.get_color_value(boardPtr.getNext_block().getValues(), col, row);
        return convertColor(colorSwigWrap);
    }

    @Override
    public int getNextBlockHeight() {
        return boardPtr.getNext_block().getCol_size();
    }

    @Override
    public int getNextBlockWidth() {
        return boardPtr.getNext_block().getRow_size();
    }

    @Override
    public int getScore() {
        return boardPtr.getScore();
    }

    public Color convertColor(ColorSwigWrap colorSwigWrap) {
        return Color.valueOf(colorSwigWrap.toString());
    }

    public static Board createBoard(int width, int height, String boardLayout) {
        BoardSwigWrap boardSwigWrap = modular_tetris.create_board_string(width, height, boardLayout);
        return new BoardSwig(boardSwigWrap);
    }
}
