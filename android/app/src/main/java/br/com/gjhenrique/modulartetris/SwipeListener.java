package br.com.gjhenrique.modulartetris;

import android.content.Context;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;


public class SwipeListener implements View.OnTouchListener {

    private final float height;
    private final float width;
    private boolean isSrolling = false;

    public final static String TAG = "SwipeListener";

    private Board board;
    private TetrisView tetrisView;

    public Context context;
    public GestureDetector gestureDetector;

    public SwipeListener(Context ctx, TetrisView view, Board board, float width, float height) {
        context = ctx;
        this.board = board;
        this.tetrisView = view;
        gestureDetector = new GestureDetector(ctx, new GestureListener());

        this.width = width;
        this.height = height;
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
                initialX = e1.getX();
                isSrolling = true;
            }

            float d = Math.abs(e2.getX() - initialX);

            if (d > width) {

                int times = (int) (d / width);

                for (int i = 0; i <  times; i++) {
                    if (e2.getX() < initialX)
                        board.moveToLeft();
                    else
                        board.moveToRight();
                }

                initialX = e2.getX();
                tetrisView.drawBoard();
            }

            return false;
        }

        @Override
        public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {

            if (e2.getY() > e1.getY() && velocityY > MAX_VELOCITY) {
                board.moveToBottom();
                tetrisView.drawBoard();
            }

            return false;
        }

        @Override
        public boolean onSingleTapUp(MotionEvent e) {
            SwipeListener.this.board.rotateClockwise();
            tetrisView.drawBoard();
            return super.onSingleTapConfirmed(e);
        }
    }
}
