package br.com.gjhenrique.modulartetris;


import android.graphics.Color;
import android.graphics.Rect;

public class Board {

    // WARNING: This enum must reflect the values os C Color Enum from the model.h file
    public enum Color {
        NONE(android.graphics.Color.WHITE), RED(android.graphics.Color.RED), BLUE(android.graphics.Color.BLUE), GREEN(android.graphics.Color.GREEN),
        YELLOW(android.graphics.Color.YELLOW), PINK(android.graphics.Color.parseColor("#FFC0CB")),
        PURPLE(android.graphics.Color.parseColor("#800080")), ORANGE(android.graphics.Color.parseColor("#FFA500"));

        private int color;

        private Color(int color) {
            this.color = color;
        }

        public int getColor() {
            return this.color;
        }
    }

    public int height;
    public int width;

    public boolean isGameOver;

    public Color[][] currentBlock;

    public Color[][] boardValues;

    public Board(int width, int height) {
        this.width = width;
        this.height = height;
        this.boardValues = new Color[this.height][this.width];
    }

    public native void nextMove();

    public native void rotateClockwise();

    public native void moveToLeft();

    public native void moveToRight();

    public native void moveToBottom();
}
