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

        ColorSwigWrap colorSwigWrap = modular_tetris.getColorValue(boardPtr.getVisited(), col, row);
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

        ColorSwigWrap colorSwigWrap = modular_tetris.getColorValue(boardPtr.getNext_block().getValues(), col, row);
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

    public Color convertColor(ColorSwigWrap colorSwigWrap) {
        return Color.valueOf(colorSwigWrap.toString());
    }

    public static Board createBoard(int width, int height, String boardLayout) {
        SWIGTYPE_p_BlockList p = modular_tetris.read_from_string(boardLayout);
        BoardSwigWrap boardSwigWrap = modular_tetris.create_board(width, height, p);
        return new BoardSwig(boardSwigWrap);
    }
}
