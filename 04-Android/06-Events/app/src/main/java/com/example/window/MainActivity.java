package com.example.window;

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.graphics.Color;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.view.WindowCompat;
import androidx.core.view.WindowInsetsCompat;
import androidx.core.view.WindowInsetsControllerCompat;

public class MainActivity extends AppCompatActivity{
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        // Force Landscape orientation
//        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        // Full Screen
        getSupportActionBar().hide();
        // Tell the android system to make your window edge to edge
        WindowCompat.setDecorFitsSystemWindows(getWindow(), false);
        // get Window insets controller
        WindowInsetsControllerCompat windowInsetsControllerCompat = WindowCompat.getInsetsController(getWindow(), getWindow().getDecorView());
        // Now tell the insets controllers to hide the remaining bars and insets
        windowInsetsControllerCompat.hide(WindowInsetsCompat.Type.systemBars() | WindowInsetsCompat.Type.ime());
        getWindow().getDecorView().setBackgroundColor(Color.BLACK);
        MyTextView myTextView = new MyTextView(this);
        setContentView(myTextView);
    }
}

