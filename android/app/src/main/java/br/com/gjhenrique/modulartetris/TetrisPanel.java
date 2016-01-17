package br.com.gjhenrique.modulartetris;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.RectF;
import android.util.DisplayMetrics;
import android.util.Log;

public class TetrisPanel {

    private String MAX_SCORE = "999999";
    private Board board;

    private float screenHeight;
    private float screenWidth;

    private float blockWidth;
    private float blockHeight;

    private Bitmap playBitmap;
    private Bitmap pauseBitmap;

    private RectF boardBox;
    private RectF nextBlockBox;
    private RectF scoreBox;
    private RectF playPauseBox;

    private boolean isPaused;

    public TetrisPanel(Activity activity, Board board) {
        this.board = board;

        DisplayMetrics displaymetrics = new DisplayMetrics();
        activity.getWindowManager().getDefaultDisplay().getMetrics(displaymetrics);
        this.screenHeight = displaymetrics.heightPixels;
        this.screenWidth = displaymetrics.widthPixels;

        float paddingY = screenHeight * 0.04f;
        float paddingX = screenWidth * 0.04f;

        boardBox = createBox(paddingX, paddingY, 0.7f, 0.9f);
        float sideBar = boardBox.right + paddingX;
        nextBlockBox = createBox(sideBar, paddingY, 0.2f, 0.14f);
        scoreBox = createBox(sideBar, nextBlockBox.bottom + paddingY * 0.5f, 0.2f, 0.05f);
        playPauseBox = createBox(sideBar, scoreBox.bottom + paddingY * 0.5f, 0.2f, 0.14f);

        this.playBitmap = BitmapFactory.decodeResource(activity.getResources(), android.R.drawable.ic_media_play);
        this.pauseBitmap = BitmapFactory.decodeResource(activity.getResources(), android.R.drawable.ic_media_pause);

        this.blockHeight = boardBox.height() / board.getBoardHeight();
        this.blockWidth = boardBox.width() / board.getBoardWidth();
    }

    private RectF createBox(float initialX, float initialY, float width, float height) {
        float right = initialX + screenWidth * width;
        float bottom = initialY + screenHeight * height;
        return new RectF(initialX, initialY, right, bottom);
    }

    public void drawBoard(Canvas canvas) {
        canvas.drawColor(android.graphics.Color.BLACK);
        this.drawTetrisBoard(canvas);
        this.drawNextBlock(canvas);
        this.drawPlayPauseButton(canvas);
        this.drawScore(canvas);
    }

    private void drawTetrisBoard(Canvas canvas) {

        //float tetrisBoardHeight = (float) screenHeight * 0.9f;
        //this.tetrisBoardWidth = (float) screenWidth * 0.75f;

        // this.blockWidth = tetrisBoardWidth / board.getBoardWidth();
        // this.blockHeight = tetrisBoardHeight / board.getBoardHeight();
        for (int i = 0; i < board.getBoardHeight(); i++) {
            for (int j = 0; j < board.getBoardWidth(); j++) {
                Color color = board.getBoardValuesIndex(i, j);

                float left = j * blockWidth + boardBox.left;
                float right = j * blockWidth + blockWidth + boardBox.left;
                float top = i * blockHeight + boardBox.top;
                float bottom = i * blockHeight + blockHeight + boardBox.top;

                RectF rect = new RectF(left, top, right, bottom);
                this.paintRect(canvas, rect, color);
            }
        }
    }

    private void drawNextBlock(Canvas canvas) {

        float heightFactor = nextBlockBox.height() / board.getNextBlockHeight();
        float widthFactor = nextBlockBox.width() / board.getNextBlockWidth();

        for (int i = 0; i < board.getNextBlockHeight(); i++) {
            for (int j = 0; j < board.getNextBlockWidth(); j++) {
                Color color = board.getNextBlockIndex(i, j);

                float left = j * widthFactor + nextBlockBox.left;
                float right = j * widthFactor + widthFactor + nextBlockBox.left;
                float top = i * heightFactor + nextBlockBox.top;
                float bottom = i * heightFactor + heightFactor + nextBlockBox.top;
                RectF rect = new RectF(left, top, right, bottom);

                this.paintRect(canvas, rect, color);
            }
        }
    }

    private void paintRect(Canvas canvas, RectF rect, Color color) {

        Paint paint = new Paint();
        int colorRgb = color.getColorRgb();
        paint.setColor(colorRgb);
        canvas.drawRect(rect, paint);

        if (color != Color.NONE) {
            paint.setStyle(Paint.Style.STROKE);
            paint.setAntiAlias(true);
            paint.setStrokeWidth(2f);
            paint.setColor(android.graphics.Color.BLACK);
            canvas.drawRect(rect, paint);
        }
    }

    private void drawPlayPauseButton(Canvas canvas) {
        Paint paint = new Paint();

        paint.setColor(android.graphics.Color.GRAY);
        paint.setAntiAlias(true);
        if (isPaused)
            canvas.drawBitmap(playBitmap, null, playPauseBox, paint);
        else
            canvas.drawBitmap(pauseBitmap, null, playPauseBox, paint);
    }

    private void drawScore(Canvas canvas) {
        Paint paint = new Paint();
        paint.setColor(android.graphics.Color.WHITE);
        paint.setTextAlign(Paint.Align.CENTER);
        paint.setAntiAlias(true);
        float x = scoreBox.width() / 2;

        Rect rect = new Rect();
        int size = 10;
        do {
            paint.getTextBounds(MAX_SCORE, 0, MAX_SCORE.length(), rect);
            paint.setTextSize(size++);
        } while (rect.height() < scoreBox.height() && rect.width() < scoreBox.width());

        canvas.drawText(board.getScore() + "", scoreBox.left + x, scoreBox.bottom,  paint);
    }

    public RectF getPlayPauseBox() {
        return playPauseBox;
    }

    public float getBlockWidth() {
        return blockWidth;
    }

    public float getBlockHeight() {
        return blockHeight;
    }

    public void setPaused(boolean paused) {
        isPaused = paused;
    }
}
