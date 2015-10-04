package br.com.gjhenrique.modulartetris;

import android.content.Context;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;


public abstract class TetrisGestureListener implements View.OnTouchListener {

    public final static String TAG = "TetrisGestureListener";

    private boolean isSrolling = false;

    private TetrisPanel tetrisPanel;

    public GestureDetector gestureDetector;

    public abstract void moveBlockLeft();

    public abstract void moveBlockRight();

    public abstract void rotateBlock();

    public abstract void moveToBottom();

    public abstract void pauseGame();

    public TetrisGestureListener(Context ctx, TetrisPanel tetrisPanel) {
        gestureDetector = new GestureDetector(ctx, new GestureListener());
        this.tetrisPanel = tetrisPanel;
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_UP) {
            isSrolling = false;
        }

        return gestureDetector.onTouchEvent(event);
    }

    private final class GestureListener extends GestureDetector.SimpleOnGestureListener {

        private float initialX;
        private final int MAX_VELOCITY = 1000;

        @Override
        public boolean onDown(MotionEvent e) {
            return true;
        }

        @Override
        public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {

            if (!isSrolling) {
                initialX = e1.getX();
                isSrolling = true;
            }

            float d = Math.abs(e2.getX() - initialX);

            if (d > tetrisPanel.getBlockWidth()) {

                int times = (int) (d / tetrisPanel.getBlockWidth());

                for (int i = 0; i <  times; i++) {
                    if (e2.getX() < initialX)
                        moveBlockLeft();
                    else
                        moveBlockRight();
                }
                initialX = e2.getX();
            }

            return false;
        }

        @Override
        public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {

            if (e2.getY() > e1.getY() && velocityY > MAX_VELOCITY) {
                moveToBottom();
            }

            return false;
        }

        @Override
        public boolean onSingleTapUp(MotionEvent e) {

            float x = e.getX();
            float y = e.getY();

            if (tetrisPanel.getButtonRange().contains(e.getX(), e.getY()))
                pauseGame();
            else
                rotateBlock();

            return false;
        }
    }
}
