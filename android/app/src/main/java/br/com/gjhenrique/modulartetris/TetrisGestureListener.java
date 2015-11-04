package br.com.gjhenrique.modulartetris;

import android.content.Context;
import android.util.Log;
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
        private float initialY;
        private final int MAX_VELOCITY = 1000;

        @Override
        public boolean onDown(MotionEvent e) {
            return true;
        }

        @Override
        public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {

            if (!isSrolling) {
                initialY = e1.getY();
                initialX = e1.getX();
                isSrolling = true;
            }

            float scrollDistanceX = Math.abs(e2.getX() - initialX);
            float scrollDistanceY = Math.abs(e2.getY() - initialY);

            if (scrollDistanceY > 2 * tetrisPanel.getBlockHeight()) {
                return false;
            }

            if (scrollDistanceX > tetrisPanel.getBlockWidth()) {
                Log.e(TAG, distanceY+"");
                int times = (int) (scrollDistanceX / tetrisPanel.getBlockWidth());

                for (int i = 0; i < times; i++) {
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

            if (tetrisPanel.getButtonRange().contains(e.getX(), e.getY()))
                pauseGame();
            else
                rotateBlock();

            return false;
        }
    }
}
