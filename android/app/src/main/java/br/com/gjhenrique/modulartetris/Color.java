package br.com.gjhenrique.modulartetris;

public enum Color {

    NONE(android.graphics.Color.WHITE), GHOST(android.graphics.Color.GRAY),
    RED(android.graphics.Color.RED), BLUE(android.graphics.Color.BLUE), GREEN(android.graphics.Color.GREEN),
    YELLOW(android.graphics.Color.YELLOW), PINK(android.graphics.Color.parseColor("#FFC0CB")),
    PURPLE(android.graphics.Color.parseColor("#800080")), ORANGE(android.graphics.Color.parseColor("#FFA500"));

    private int colorRgb;

    private Color(int colorRgb) {
        this.colorRgb = colorRgb;
    }

    public int getColorRgb() {
        return this.colorRgb;
    }

}
