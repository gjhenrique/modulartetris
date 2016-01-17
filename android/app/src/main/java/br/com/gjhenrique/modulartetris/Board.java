package br.com.gjhenrique.modulartetris;

// Board.java already exists in swig generation
public interface Board {

    public boolean isGameOver();

    public void nextMove();

    public void moveToLeft();

    public void moveToRight();

    public void rotateClockwise();

    public void moveToBottom();

    public Color getBoardValuesIndex(int col, int row);

    public int getBoardHeight();

    public int getBoardWidth();

    public Color getNextBlockIndex(int col, int row);

    public int getNextBlockHeight();

    public int getNextBlockWidth();

    public int getScore();
}
