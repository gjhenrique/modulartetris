package br.com.gjhenrique.modulartetris;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;

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
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(new TetrisView(this));
    }

}
