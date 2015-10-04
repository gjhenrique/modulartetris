package br.com.gjhenrique.modulartetris;

import android.app.Activity;
import android.content.Context;
import android.graphics.Canvas;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.io.IOException;
import java.io.InputStream;

public class TetrisView extends SurfaceView implements SurfaceHolder.Callback {

    public static final String TAG = "LogTetrisView";

    private volatile boolean paused = false;
    private volatile boolean stopped = false;

    private Board board;

    private Thread gameThread;

    private TetrisPanel tetrisPanel;

    private Activity activity;

    public TetrisView(Activity activity) {
        super(activity);

        this.activity = activity;
        this.gameThread = new Thread(new TetrisGameLoop());

        this.setupBoard(activity);
        getHolder().addCallback(this);
        this.setOnTouchListener(new TetrisGesture(activity, this.tetrisPanel));
    }

    public void drawBoard() {
        synchronized (this) {
            Canvas canvas = getHolder().lockCanvas();
            if (canvas == null)
                return;
            tetrisPanel.drawBoard(canvas);
            getHolder().unlockCanvasAndPost(canvas);
        }
    }

    private void setupBoard(Activity activity) {
        String boardSrt = fromAssetToString();
        int boardWidth = getResources().getInteger(R.integer.board_width);
        int boardHeight = getResources().getInteger(R.integer.board_height);

        this.board = BoardSwig.createBoard(boardWidth, boardHeight, boardSrt);
        this.tetrisPanel = new TetrisPanel(activity, board);
    }

    private String fromAssetToString() {
        String boardStr = null;
        try {
            String fileName = getResources().getString(R.string.filename);
            InputStream inputStream = activity.getAssets().open(fileName);
            int size = inputStream.available();
            byte[] buffer = new byte[size];
            inputStream.read(buffer);
            inputStream.close();

            boardStr = new String(buffer);
        } catch (IOException e) {
            Log.e(TAG, "Error at reading file");
            e.printStackTrace();
        }

        return boardStr;
    }

    private void checkGameOver() {
        if (board.isGameOver()) {
            setupBoard(activity);
        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        stopped = false;

        if (this.gameThread.getState() == Thread.State.TERMINATED)
		{
			this.gameThread = new Thread(new TetrisGameLoop());
		}

        this.drawBoard();
        this.gameThread.start();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        stopped = true;

        boolean retry = true;
        while (retry) {
            try {
                gameThread.join();
                retry = false;
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    private class TetrisGameLoop implements Runnable {
        @Override
        public void run() {
            while (!stopped) {

                if (!paused) {
                    synchronized (TetrisView.this) {
                        checkGameOver();
                        board.nextMove();
                        drawBoard();
                    }
                }

                // TODO: We should not be sleeping that much here
                // Improve the game loop
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private class TetrisGesture extends TetrisGestureListener {

        public TetrisGesture(Context ctx, TetrisPanel tetrisPanel) {
            super(ctx, tetrisPanel);
        }

        @Override
        public void moveBlockLeft() {
            if(!paused) {
                board.moveToLeft();
                drawBoard();
            }
        }

        @Override
        public void moveBlockRight() {
            if(!paused) {
                board.moveToRight();
                drawBoard();
            }
        }

        @Override
        public void rotateBlock() {
            if(!paused) {
                board.rotateClockwise();
                drawBoard();
            }
        }

        @Override
        public void moveToBottom() {
            if(!paused) {
                checkGameOver();
                board.moveToBottom();
                drawBoard();
            }
        }

        @Override
        public void pauseGame() {
            paused = !paused;
            tetrisPanel.setPaused(paused);
            drawBoard();
        }
    }
}
