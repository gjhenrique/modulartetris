package br.com.gjhenrique.modulartetris;

import android.app.Activity;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Window;

import java.io.IOException;
import java.io.InputStream;


public class MainActivity extends Activity
{

    public static final String TAG = "ModularTetris";

    static {
        System.loadLibrary("modular-tetris");
    }

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {

        // TODO: Read file name from xml, width and height
        String boardSrt = fromAssetToString("default_blocks");
        Board board = createBoard(boardSrt, 12, 20);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(new TetrisView(this, board));

        super.onCreate(savedInstanceState);
    }

    private String fromAssetToString(String fileName) {
        String boardStr = null;
        try {

            InputStream inputStream = getAssets().open("default_blocks");
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

    public native Board createBoard(String boardStr, int width, int height);
}
