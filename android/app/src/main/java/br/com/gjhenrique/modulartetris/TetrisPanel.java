package br.com.gjhenrique.modulartetris;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.util.DisplayMetrics;

public class TetrisPanel {

    private Board board;

    private Context context;

    private float tetrisBoardWidth;

    private float nextBlockHeight;
    private float nextBlockWidth;

    private float paddingY;
    private float paddingX;

    private float blockWidth;
    private float blockHeight;

    private RectF buttonRange;
    private Bitmap playBitmap;
    private Bitmap pauseBitmap;
    private boolean isPaused;


    public TetrisPanel(Activity activity, Board board) {
        this.context = activity;
        this.board = board;

        DisplayMetrics displaymetrics = new DisplayMetrics();
        activity.getWindowManager().getDefaultDisplay().getMetrics(displaymetrics);
        int screenHeight = displaymetrics.heightPixels;
        int screenWidth = displaymetrics.widthPixels;

        float tetrisBoardHeight = (float) screenHeight * 0.9f;
        this.tetrisBoardWidth = (float) screenWidth * 0.75f;

        this.nextBlockHeight = (float) screenHeight * 0.15f;
        this.nextBlockWidth = (float) screenWidth * 0.15f;

        this.paddingY = screenHeight * 0.05f;
        this.paddingX = screenWidth * 0.05f;

        this.blockWidth = tetrisBoardWidth / board.getBoardWidth();
        this.blockHeight = tetrisBoardHeight / board.getBoardHeight();

        float playButtonWidth = paddingX + paddingX + tetrisBoardWidth;
        float playButtonHeigt = paddingY + this.nextBlockHeight + paddingY;
        this.buttonRange = new RectF(playButtonWidth, playButtonHeigt,
                playButtonWidth + ((float) screenWidth * 0.15f), playButtonHeigt + ((float) screenWidth * 0.15f));

        this.playBitmap = BitmapFactory.decodeResource(activity.getResources(), android.R.drawable.ic_media_play);
        this.pauseBitmap = BitmapFactory.decodeResource(activity.getResources(), android.R.drawable.ic_media_pause);
    }

    public void drawBoard(Canvas canvas) {
        this.drawTetrisBoard(canvas);
        this.drawNextBlock(canvas);
        this.drawPlayPauseButton(canvas);
    }

    private void drawTetrisBoard(Canvas canvas) {
        for (int i = 0; i < board.getBoardHeight(); i++) {
            for (int j = 0; j < board.getBoardWidth(); j++) {
                Color color = board.getBoardValuesIndex(i, j);

                float left = j * blockWidth + paddingX;
                float top = i * blockHeight + paddingY;
                float right = j * blockWidth + blockWidth + paddingX;
                float bottom = i * blockHeight + blockHeight + paddingY;

                RectF rect = new RectF(left, top, right, bottom);
                this.paintRect(canvas, rect, color);
            }
        }
    }

    private void drawNextBlock(Canvas canvas) {

        float heightFactor = this.nextBlockHeight / board.getNextBlockHeight();
        float widthFactor = this.nextBlockWidth / board.getNextBlockWidth();

        for (int i = 0; i < board.getNextBlockHeight(); i++) {
            for (int j = 0; j < board.getNextBlockWidth(); j++) {
                Color color = board.getNextBlockIndex(i, j);

                float left = j * widthFactor + paddingX + paddingX + tetrisBoardWidth;
                float top = i * heightFactor + paddingY;
                float right = j * widthFactor + widthFactor + paddingX + paddingX + tetrisBoardWidth;
                float bottom = i * heightFactor + heightFactor + paddingY;

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
        paint.setColor(android.graphics.Color.BLACK);
        canvas.drawRect(this.buttonRange, paint);

        paint.setColor(android.graphics.Color.GRAY);
        paint.setAntiAlias(true);

        if (isPaused)
            canvas.drawBitmap(playBitmap, null, this.buttonRange, paint);
        else
            canvas.drawBitmap(pauseBitmap, null, this.buttonRange, paint);
    }

    public RectF getButtonRange() {
        return this.buttonRange;
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
