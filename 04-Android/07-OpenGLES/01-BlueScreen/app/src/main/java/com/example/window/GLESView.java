package com.example.window;

import android.content.Context;
// OpenGLES related packages
import android.opengl.GLES32;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;


// event packages
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;


public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener{

    private Context context;
    private GestureDetector gestureDetector;

    public GLESView(Context _context)
    {
        super(_context);

        // Initialize of OpenGLES
        setEGLContextClientVersion(3);
        setRenderer(this);
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        // Get gesture Detector
        gestureDetector = new GestureDetector(_context, this, null, false);

        // set this class as double tap listener
        gestureDetector.setOnDoubleTapListener(this);
    }

    // 3 implementable methods of GLsurfaceView.Renderer interface
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        // Code - similar to initialize
        int iResult = initialize(gl);
        if (iResult != 0)
        {
            System.out.println("AMC: initialize failed");
            System.exit(0);
        }
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        // Code - similar to resize
        resize(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        // Code - similar to display
        display();
        update();
    }

    // Implement one method from voew
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
    }

    @Override
    public boolean onScroll(MotionEvent el, MotionEvent e2, float distanceX, float distanceY)
    {
        uninitialize();
        System.exit(0);
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

    // Our custom OpenGLES method
    private int initialize(GL10 gl)
    {
        printGLESInfo(gl);

        // Depth initialization
        GLES32.glClearDepthf(1.0f);
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

        // Set the clear color
        GLES32.glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        return 0;
    }

    private void printGLESInfo(GL10 gl)
    {
        // code
        String gles_vendor = gl.glGetString(GL10.GL_VENDOR);
        String gles_renderer = gl.glGetString(GL10.GL_RENDERER);
        String gles_version = gl.glGetString(GL10.GL_VERSION);

        System.out.println("AMC: GLES Vendor - " + gles_vendor);
        System.out.println("AMC: GLES renderer - " + gles_renderer);
        System.out.println("AMC: GLES Version - " + gles_version);

    }

    private void resize(int width, int height)
    {
        // Code
        if (height <= 0)
        {
            height = 1;
        }

        // Set the viewport
        GLES32.glViewport(0, 0, width, height);
    }

    private void display()
    {
        // Code
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

        // Similar to swap buffer
        requestRender();
    }

    private void update()
    {
        // code
    }

    private void uninitialize()
    {
        // Code
    }





}

