package com.example.window;

import android.graphics.Color;
import android.view.Gravity;
import android.content.Context;
import androidx.appcompat.widget.AppCompatTextView;

// event packages
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;

public class MyTextView extends AppCompatTextView implements OnGestureListener, OnDoubleTapListener{

    private GestureDetector gestureDetector;

    public MyTextView(Context context)
    {
        super(context);
        setTextColor(Color.rgb(0, 255, 0));
        setTextSize(60);
        setGravity(Gravity.CENTER);
        setText("Hello World!!");

        // Get gesture Detector
        gestureDetector = new GestureDetector(context, this, null, false);

        // set this class as double tap listener
        gestureDetector.setOnDoubleTapListener(this);
    }

//    Implement one me
    @Override
    public boolean onTouchEvent(MotionEvent e)
    {
        if (!gestureDetector.onTouchEvent(e))
        {
            super.onTouchEvent(e);
        }
        return true;
    }

//    Three method from on double tap listener
    @Override
    public boolean onDoubleTap(MotionEvent e)
    {
        setText("Double Tap");
        return true;
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e)
    {
        return true;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e)
    {
        setText("Single Tap");
        return true;
    }

//    6 methods from gesture on GestureListener
    @Override
    public boolean onDown(MotionEvent e)
    {
        return true;
    }

    @Override
    public boolean onFling(MotionEvent el, MotionEvent e2, float velocityX, float velocityY)
    {
        return true;
    }

    @Override
    public void onLongPress(MotionEvent e)
    {
        setText("Long Press");
    }

    @Override
    public boolean onScroll(MotionEvent el, MotionEvent e2, float distanceX, float distanceY)
    {
        setText("Scroll");
        return true;
    }

    @Override
    public void onShowPress(MotionEvent e)
    {
        // Code
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e)
    {
        return true;
    }

}

