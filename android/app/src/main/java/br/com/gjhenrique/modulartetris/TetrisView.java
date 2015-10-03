package br.com.gjhenrique.modulartetris;

import android.annotation.TargetApi;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.os.Build;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

public class TetrisView extends SurfaceView implements SurfaceHolder.Callback {

    public static final String TAG = "TetrisView";
    private final float heightFactor;
    private final float widthFactor;

    private boolean paused = false;

    private Board board;

    private Bitmap boardBitmap;

    private Thread gameThread;

    private int screenHeight;
    private int screenWidth;

    public TetrisView(Activity activity, Board board) {
        super(activity);

        this.board = board;

        getHolder().addCallback(this);
        this.boardBitmap = BitmapFactory.decodeResource(activity.getResources(), R.drawable.block_tetris);

        DisplayMetrics displaymetrics = new DisplayMetrics();
        activity.getWindowManager().getDefaultDisplay().getMetrics(displaymetrics);

        this.screenHeight = displaymetrics.heightPixels;
        this.screenWidth = displaymetrics.widthPixels;

        widthFactor = (float) screenWidth / board.width;
        heightFactor = (float) screenHeight / board.height;

        this.setOnTouchListener(new SwipeListener(activity, this, board, widthFactor, heightFactor) {
        });

        this.gameThread = new Thread(new Runnable() {
            @Override
            public void run() {

                while (!paused && !TetrisView.this.board.isGameOver) {
                    TetrisView.this.board.nextMove();
                    TetrisView.this.drawBoard();
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        });
    }

    public void drawBoard() {

        synchronized (this) {

            Canvas canvas = getHolder().lockCanvas();

            for (int i = 0; i < board.height; i++) {
                for (int j = 0; j < board.width; j++) {
                    int color = board.boardValues[i][j].getColor();
                    RectF rect = new RectF(j * widthFactor, i * heightFactor, j * widthFactor + widthFactor,
                            i * heightFactor + heightFactor);

                    Paint paint = new Paint();
                    paint.setColor(color);
                    canvas.drawRect(rect, paint);
                }
            }

            getHolder().unlockCanvasAndPost(canvas);
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Log.v(TAG, "Surface Created");
        if (gameThread.getState() == Thread.State.NEW)
            this.gameThread.start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Log.v(TAG, "Surface Changed");
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.v(TAG, "Surface Destroyed");
        boolean retry = true;
        paused = true;
        while (retry) {
            try {
                gameThread.join();
                retry = false;
            } catch (InterruptedException e) {
                Log.e("Tile Game Example", e.getMessage());
            }
        }
    }
}
