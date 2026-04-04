package com.example.perspective;

import android.graphics.Color;
import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

// packages for full screen
import androidx.core.view.WindowCompat;
import androidx.core.view.WindowInsetsControllerCompat;
import androidx.core.view.WindowInsetsCompat;

// package for forced landscape
import android.content.pm.ActivityInfo;

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
        GLESView glesView = new GLESView(this);
        setContentView(glesView);
    }
}

