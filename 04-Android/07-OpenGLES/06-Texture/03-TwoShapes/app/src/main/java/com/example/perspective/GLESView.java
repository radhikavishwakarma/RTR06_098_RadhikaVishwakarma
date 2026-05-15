package com.example.perspective;

import android.content.Context;
// OpenGLES related packages
import android.opengl.GLES32;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;

//Image related packages
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

// event packages
import android.opengl.GLUtils;
import android.opengl.Matrix;
import android.view.MotionEvent;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.GestureDetector.OnDoubleTapListener;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;


public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener{

    private Context context;
    private GestureDetector gestureDetector;

    // Shader related global variables
    private int shaderProgramObject = 0;

    private int vao_pyramid[] = new int[1];
    private int vbo_position_pyramid[] = new int[1];
    private int vbo_texcoord_pyramid[] = new int[1];

    private int vao_cube[] = new int[1];
    private int vbo_position_cube[] = new int[1];
    private int vbo_texcoord_cube[] = new int[1];

    // rotation angles
    float anglePyramid = 0.0f;
    float angleCube = 0.0f;

    private int texture_stone[] = new int[1];
    private int texture_vijay_kundali[] = new int[1];
    private int textureSamplerUniform = 0;

    private int mvpMatrixUniform = 0;
    private float perspectiveProjectionMatrix[] = new float[16];

    public GLESView(Context _context)
    {
        super(_context);
        context = _context;

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

        // VERTEX SHADER
        // 1. Write shader source code
	    final String vertexShaderSourceCode = String.format
                (
                    "#version 310 es\n" +
                    "in vec4 aPosition;\n" +
                    "in vec2 aTexCoord;\n" +
                    "uniform mat4 uMVPMatrix;\n" +
                    "out vec2 out_texCoord;\n" +
                    "void main(void)\n" +
                    "{\n" +
                        "gl_Position = uMVPMatrix * aPosition;\n" +
                        "out_texCoord = aTexCoord;\n" +
                    "}\n"
                );

        // 2. Create the shader object
        int vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

        // 3. Give the shader source code to the shader object
        GLES32.glShaderSource(vertexShaderObject, vertexShaderSourceCode);

        // 4. Compile the shader programmatically
        GLES32.glCompileShader(vertexShaderObject);

        // 5. Do shader compilation error checking
        int status[] = new int[1];
        int infoLogLength[] = new int[1];
        String szInfoLog = null;

        GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_COMPILE_STATUS, status,0);
        if (status[0] == GLES32.GL_FALSE)
        {
            GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0)
            {
                szInfoLog = GLES32.glGetShaderInfoLog(vertexShaderObject);
                if (szInfoLog != null)
                {
                    GLES32.glGetShaderInfoLog(vertexShaderObject);
                    System.out.println("AMC: VERTEX SHADER COMPILATION LOG = " + szInfoLog);
                }
            }
            uninitialize();
            System.exit(0);
        }

        // FRAGMENT SHADER
        // 1. Write shader source code
        final String fragmentShaderSourceCode = String.format
                (
                    "#version 310 es\n" +
                    "precision highp float;\n" +
                    "in vec2 out_texCoord;\n" +
                    "uniform sampler2D uTextureSampler;\n" +
                    "out vec4 fragColor;\n" +
                    "void main(void)\n" +
                    "{\n" +
                            "fragColor = texture(uTextureSampler, out_texCoord);\n" +
                    "}\n"
                );

        // 2. Create the shader object
        int fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

        // 3. Give the shader source code to the shader object
        GLES32.glShaderSource(fragmentShaderObject, fragmentShaderSourceCode);

        // 4. Compile the shader programmatically
        GLES32.glCompileShader(fragmentShaderObject);

        // 5. Do shader compilation error checking
        status[0] = 0;
        infoLogLength[0] = 0;
        szInfoLog = null;

        GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_COMPILE_STATUS, status,0);
        if (status[0] == GLES32.GL_FALSE)
        {
            GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0)
            {
                szInfoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject);
                if (szInfoLog != null)
                {
                    GLES32.glGetShaderInfoLog(fragmentShaderObject);
                    System.out.println("AMC: FRAGMENT SHADER COMPILATION LOG = " + szInfoLog);
                }
            }
            uninitialize();
            System.exit(0);
        }

        // Create, attach, link shader program object
        shaderProgramObject = GLES32.glCreateProgram();
        GLES32.glAttachShader(shaderProgramObject, vertexShaderObject);
        GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);

        // Bind shader attribute at a certain index in shader to save index in host program
        GLES32.glBindAttribLocation(shaderProgramObject, MyAttributes.AMC_ATTRIBUTE_POSITION, "aPosition");
        GLES32.glBindAttribLocation(shaderProgramObject, MyAttributes.AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");
        GLES32.glLinkProgram(shaderProgramObject);

        // Link error checking
        status[0] = 0;
        infoLogLength[0] = 0;
        szInfoLog = null;

        GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_LINK_STATUS, status, 0);
        if (status[0] == GLES32.GL_FALSE)
        {
            GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0)
            {
                szInfoLog = GLES32.glGetProgramInfoLog(shaderProgramObject);
                if (szInfoLog != null)
                {
                    GLES32.glGetProgramInfoLog(shaderProgramObject);
                    System.out.println("AMC: SHADER PROGRAM LINK LOG = " + szInfoLog);
                }
            }
            uninitialize();
            System.exit(0);
        }

        // Get the required uniform location from the shader
        mvpMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uMVPMatrix");
        textureSamplerUniform = GLES32.glGetUniformLocation(shaderProgramObject, "uTextureSampler");

        // Provide vertex position, color, normal, texCoord etc.
        final float pyramid_position[] = new float[]
            {
                // front
                0.0f,  1.0f,  0.0f, // front-top
                -1.0f, -1.0f,  1.0f, // front-left
                1.0f, -1.0f,  1.0f, // front-right

                // right
                0.0f,  1.0f,  0.0f, // right-top
                1.0f, -1.0f,  1.0f, // right-left
                1.0f, -1.0f, -1.0f, // right-right

                // back
                0.0f,  1.0f,  0.0f, // back-top
                1.0f, -1.0f, -1.0f, // back-left
                -1.0f, -1.0f, -1.0f, // back-right

                // left
                0.0f,  1.0f,  0.0f, // left-top
                -1.0f, -1.0f, -1.0f, // left-left
                -1.0f, -1.0f,  1.0f, // left-right
            };

        final float pyramid_texcoord[] = new float[]
            {
                0.5f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f,

                0.5f, 1.0f,
                1.0f, 0.0f,
                0.0f, 0.0f,

                0.5f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f,

                0.5f, 1.0f,
                1.0f, 0.0f,
                0.0f, 0.0f,
            };

        final float cube_position[] = new float[]
        {
            // front
            1.0f,  1.0f,  1.0f, // top-right of front
            -1.0f,  1.0f,  1.0f, // top-left of front
            -1.0f, -1.0f,  1.0f, // bottom-left of front
            1.0f, -1.0f,  1.0f, // bottom-right of front

            // right
            1.0f,  1.0f, -1.0f, // top-right of right
            1.0f,  1.0f,  1.0f, // top-left of right
            1.0f, -1.0f,  1.0f, // bottom-left of right
            1.0f, -1.0f, -1.0f, // bottom-right of right

            // back
            1.0f,  1.0f, -1.0f, // top-right of back
            -1.0f,  1.0f, -1.0f, // top-left of back
            -1.0f, -1.0f, -1.0f, // bottom-left of back
            1.0f, -1.0f, -1.0f, // bottom-right of back

            // left
            -1.0f,  1.0f,  1.0f, // top-right of left
            -1.0f,  1.0f, -1.0f, // top-left of left
            -1.0f, -1.0f, -1.0f, // bottom-left of left
            -1.0f, -1.0f,  1.0f, // bottom-right of left

            // top
            1.0f,  1.0f, -1.0f, // top-right of top
            -1.0f,  1.0f, -1.0f, // top-left of top
            -1.0f,  1.0f,  1.0f, // bottom-left of top
            1.0f,  1.0f,  1.0f, // bottom-right of top

            // bottom
            1.0f, -1.0f,  1.0f, // top-right of bottom
            -1.0f, -1.0f,  1.0f, // top-left of bottom
            -1.0f, -1.0f, -1.0f, // bottom-left of bottom
            1.0f, -1.0f, -1.0f, // bottom-right of bottom
        };

        final float cube_texcoord[] = new float[]
            {
                // front
                1.0f, 1.0f, // top-right of front
                0.0f, 1.0f, // top-left of front
                0.0f, 0.0f, // bottom-left of front
                1.0f, 0.0f, // bottom-right of front

                // right
                1.0f, 1.0f, // top-right of right
                0.0f, 1.0f, // top-left of right
                0.0f, 0.0f, // bottom-left of right
                1.0f, 0.0f, // bottom-right of right

                // back
                1.0f, 1.0f, // top-right of back
                0.0f, 1.0f, // top-left of back
                0.0f, 0.0f, // bottom-left of back
                1.0f, 0.0f, // bottom-right of back

                // left
                1.0f, 1.0f, // top-right of left
                0.0f, 1.0f, // top-left of left
                0.0f, 0.0f, // bottom-left of left
                1.0f, 0.0f, // bottom-right of left

                // top
                1.0f, 1.0f, // top-right of top
                0.0f, 1.0f, // top-left of top
                0.0f, 0.0f, // bottom-left of top
                1.0f, 0.0f, // bottom-right of top

                // bottom
                1.0f, 1.0f, // top-right of bottom
                0.0f, 1.0f, // top-left of bottom
                0.0f, 0.0f, // bottom-left of bottom
                1.0f, 0.0f, // bottom-right of bottom
            };

        //PYRAMID
        // VERTEX ARRAY OBJECT FOR ARRAYS OF VERTEX OBJECT
        GLES32.glGenVertexArrays(1, vao_pyramid, 0);
        // Bind vertex array object
        GLES32.glBindVertexArray(vao_pyramid[0]);
        // POSITION
        GLES32.glGenBuffers(1, vbo_position_pyramid, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position_pyramid[0]);
        // Create a native buffer suitable for native IO but for Java
        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(pyramid_position.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer pyramidPositionBuffer = byteBuffer.asFloatBuffer();
        pyramidPositionBuffer.put(pyramid_position);
        pyramidPositionBuffer.position(0);
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, pyramid_position.length * 4, pyramidPositionBuffer, GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyAttributes.AMC_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyAttributes.AMC_ATTRIBUTE_POSITION);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        // TEXCOORD
        GLES32.glGenBuffers(1, vbo_texcoord_pyramid, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_texcoord_pyramid[0]);
        // Create a native buffer suitable for native IO but for Java
        byteBuffer = ByteBuffer.allocateDirect(pyramid_position.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer pyramidTexCoordBuffer = byteBuffer.asFloatBuffer();
        pyramidTexCoordBuffer.put(pyramid_texcoord);
        pyramidTexCoordBuffer.position(0);
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, pyramid_texcoord.length * 4, pyramidTexCoordBuffer, GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyAttributes.AMC_ATTRIBUTE_TEXCOORD, 2, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyAttributes.AMC_ATTRIBUTE_TEXCOORD);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        // unbind VAO
        GLES32.glBindVertexArray(0);

        // CUBE
        // VERTEX ARRAY OBJECT FOR ARRAYS OF VERTEX OBJECT
        GLES32.glGenVertexArrays(1, vao_cube, 0);
        // Bind vertex array object
        GLES32.glBindVertexArray(vao_cube[0]);
        // POSITION
        GLES32.glGenBuffers(1, vbo_position_cube, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_position_cube[0]);
        // Create a native buffer suitable for native IO but for Java
        byteBuffer = ByteBuffer.allocateDirect(cube_position.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer cubePositionBuffer = byteBuffer.asFloatBuffer();
        cubePositionBuffer.put(cube_position);
        cubePositionBuffer.position(0);
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cube_position.length * 4, cubePositionBuffer, GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyAttributes.AMC_ATTRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyAttributes.AMC_ATTRIBUTE_POSITION);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        // TEXCOORD
        GLES32.glGenBuffers(1, vbo_texcoord_cube, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_texcoord_cube[0]);
        // Create a native buffer suitable for native IO but for Java
        byteBuffer = ByteBuffer.allocateDirect(cube_position.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer cubeTexCoordBuffer = byteBuffer.asFloatBuffer();
        cubeTexCoordBuffer.put(cube_texcoord);
        cubeTexCoordBuffer.position(0);
        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cube_texcoord.length * 4, cubeTexCoordBuffer, GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyAttributes.AMC_ATTRIBUTE_TEXCOORD, 2, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyAttributes.AMC_ATTRIBUTE_TEXCOORD);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        // unbind VAO
        GLES32.glBindVertexArray(0);

        // Depth initialization
        GLES32.glClearDepthf(1.0f);
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

        // Set the clear color
        GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Load texture
        texture_stone[0] = loadGLTexture(R.drawable.stone);
        texture_vijay_kundali[0] = loadGLTexture(R.drawable.vijay_kundali);

        Matrix.setIdentityM(perspectiveProjectionMatrix, 0);

        return 0;
    }

    private int loadGLTexture(int imageFileResourceID)
    {
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inScaled = false;

        Bitmap bitmap = BitmapFactory.decodeResource(context.getResources(), imageFileResourceID, options);
        if (bitmap == null)
        {
            return 0;
        }

        int texture[] = new int[1];
        GLES32.glGenTextures(1, texture, 0);
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture[0]);

        GLES32.glPixelStorei(GLES32.GL_UNPACK_ALIGNMENT, 1);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_LINEAR);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER, GLES32.GL_LINEAR_MIPMAP_LINEAR);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_WRAP_S, GLES32.GL_REPEAT);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_WRAP_T, GLES32.GL_REPEAT);

        GLUtils.texImage2D(GLES32.GL_TEXTURE_2D, 0, bitmap, 0);
        GLES32.glGenerateMipmap(GLES32.GL_TEXTURE_2D);

        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);
        bitmap.recycle();

        return texture[0];
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

        // do perspective projection
        Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float)width/(float)height, 0.1f, 100.0f);
    }

    private void display()
    {
        // Code
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

        // use shader program object
        GLES32.glUseProgram(shaderProgramObject);

        //PYRAMID
        // Transformations
        float modelViewMatrix[] = new float[16]; // this is similar to glLoadIdentity() in display for model view matrix
        Matrix.setIdentityM(modelViewMatrix, 0);
        float translationMatrix[] = new float[16];
        Matrix.setIdentityM(translationMatrix, 0);
        Matrix.translateM(translationMatrix, 0, -1.5f, 0.0f, -10.0f);
        float rotationMatrix[] = new float[16];
        Matrix.setIdentityM(rotationMatrix, 0);
        Matrix.rotateM(rotationMatrix, 0, anglePyramid, 0.0f, 1.0f, 0.0f);
        Matrix.multiplyMM(modelViewMatrix, 0, translationMatrix, 0, rotationMatrix, 0);
        float modelViewProjectionMatrix[] = new float[16];
        Matrix.setIdentityM(modelViewProjectionMatrix, 0);
        Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

        // send above matrix to the shader in "uniform"
        GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix, 0);

        GLES32.glActiveTexture(GLES32.GL_TEXTURE0);
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture_stone[0]);
        GLES32.glUniform1i(textureSamplerUniform, 0);

        // Bind with VAO
        GLES32.glBindVertexArray(vao_pyramid[0]);

        // Draw the vertex arrays
        GLES32.glDrawArrays(GLES32.GL_TRIANGLES, 0, 12);

        // Unbind with VAO
        GLES32.glBindVertexArray(0);

        // CUBE
        // Transformations
        Matrix.setIdentityM(modelViewMatrix, 0);
        Matrix.setIdentityM(translationMatrix, 0);
        Matrix.translateM(translationMatrix, 0, 1.5f, 0.0f, -10.0f);
        Matrix.setIdentityM(rotationMatrix, 0);
        Matrix.rotateM(rotationMatrix, 0, angleCube, 1.0f, 1.0f, 1.0f);
        Matrix.multiplyMM(modelViewMatrix, 0, translationMatrix, 0, rotationMatrix, 0);
        Matrix.setIdentityM(modelViewProjectionMatrix, 0);
        Matrix.multiplyMM(modelViewProjectionMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

        // send above matrix to the shader in "uniform"
        GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectionMatrix, 0);

        GLES32.glActiveTexture(GLES32.GL_TEXTURE0);
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture_vijay_kundali[0]);
        GLES32.glUniform1i(textureSamplerUniform, 0);

        // Bind with VAO
        GLES32.glBindVertexArray(vao_cube[0]);

        // Draw the vertex arrays
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 4, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 8, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 12, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 16, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 20, 4);

        // Unbind with VAO
        GLES32.glBindVertexArray(0);

        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);

        // unuse shader program object
        GLES32.glUseProgram(0);

        // Similar to swap buffer
        requestRender();
    }

    private void update()
    {
        // code
        anglePyramid = anglePyramid + 0.2f;
        if(anglePyramid >= 360.0f)
        {
            anglePyramid = anglePyramid - 360.0f;
        }

        angleCube = angleCube + 0.2f;
        if(angleCube >= 360.0f)
        {
            angleCube = angleCube - 360.0f;
        }
    }

    private void uninitialize()
    {
        // Code
        // Free vbo_position_cube
        if (vbo_position_cube[0] > 0)
        {
            GLES32.glDeleteBuffers(1, vbo_position_cube,0);
            vbo_position_cube[0] = 0;
        }

        // Free VAO
        if (vao_cube[0] > 0)
        {
            GLES32.glDeleteVertexArrays(1, vao_cube, 0);
            vao_cube[0] = 0;
        }

        // Free vbo_position_pyramid
        if (vbo_position_pyramid[0] > 0)
        {
            GLES32.glDeleteBuffers(1, vbo_position_pyramid,0);
            vbo_position_pyramid[0] = 0;
        }

        // Free VAO
        if (vao_pyramid[0] > 0)
        {
            GLES32.glDeleteVertexArrays(1, vao_pyramid, 0);
            vao_pyramid[0] = 0;
        }

        // Detach, delete shader objects and delete shader program object
        if (shaderProgramObject > 0)
        {
            GLES32.glUseProgram(shaderProgramObject);
            int retVal[] = new int[1];
            GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_ATTACHED_SHADERS, retVal, 0);
            int numAttachedShaders = retVal[0];
            if (numAttachedShaders > 0)
            {
                int shaderObjects[] = new int[numAttachedShaders];
                GLES32.glGetAttachedShaders(shaderProgramObject, numAttachedShaders, retVal, 0, shaderObjects, 0);
                for (int i = 0; i < numAttachedShaders; i++)
                {
                    GLES32.glDetachShader(shaderProgramObject, shaderObjects[i]);
                    GLES32.glDeleteShader(shaderObjects[i]);
                    shaderObjects[i] = 0;
                }
            }
            GLES32.glUseProgram(0);
            GLES32.glDeleteProgram(shaderProgramObject);
            shaderProgramObject = 0;
        }
    }
}

