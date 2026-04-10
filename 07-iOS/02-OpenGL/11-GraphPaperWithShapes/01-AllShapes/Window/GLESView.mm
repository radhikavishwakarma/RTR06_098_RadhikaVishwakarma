//
//  GLESView.mm
//  Window
//
//  Created by ADMIN on 21/12/25.
//

// OpenGL related Header files
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#import <QuartzCore/CADisplayLink.h>

#import "GLESView.h"

// Header file for Matrix and Transformation related functions
#import "vmath.h"
using namespace vmath;

// Constants related to Graph paper
const int numberOfLinesEachSide = 20;
const int numberOfTotalLines = numberOfLinesEachSide * 2;
const int numberOfPointsToDrawCircle = 1000;

enum
{
    AMC_ATTRIBUTE_POSITION = 0,
};

@implementation GLESView
{
    EAGLContext *eaglContext;

    GLuint framebuffer;
    GLuint colorRenderbuffer;
    GLuint depthRenderbuffer;

    CADisplayLink *displayLink;
    CAFrameRateRange frameRateRange;
    BOOL isDisplayLink;

    // Shader related global variables
    GLuint shaderProgramObject;

    // Triangle
    GLuint vao_triangle;            // Vertex Array Object
    GLuint vbo_position_triangle;   // Vertex Buffer Object

    // Rectangle
    GLuint vao_rectangle;           // Vertex Array Object
    GLuint vbo_position_rectangle;  // Vertex Buffer Object

    // Circle
    GLuint vao_circle;              // Vertex Array Object
    GLuint vbo_position_circle;     // Vertex Buffer Object

    // Graph
    // X-Axis
    GLuint vao_XAxis;
    GLuint vbo_position_XAxis;

    // Y-Axis
    GLuint vao_YAxis;
    GLuint vbo_position_YAxis;

    // Horizontal Lines
    GLuint vao_horizontalLines;
    GLuint vbo_position_horizontalLines;

    // Vertical Lines
    GLuint vao_verticalLines;
    GLuint vbo_position_verticalLines;

    GLuint mvpMatrixUniform;
    GLuint colorUniform;

    mat4 perspectiveProjectionMatrix;

    // Tap related variables
    int iSingleTap;
    int iDoubleTap;
}

-(void)awakeFromNib
{
    // Code
    [super awakeFromNib];

    // Set Background Color to Black Color
    [self setBackgroundColor:[UIColor blackColor]];

    // Step 1: Get drawable layer
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)[super layer];

    // Step 2: Set this layer's properties
    [eaglLayer setOpaque:YES];
    NSDictionary *dictionary = [NSDictionary dictionaryWithObjectsAndKeys:
                                kEAGLDrawablePropertyRetainedBacking, [NSNumber numberWithBool:NO],
                                kEAGLDrawablePropertyColorFormat, kEAGLColorFormatRGBA8,
                                nil];

    [eaglLayer setDrawableProperties:dictionary];

    // Step 3: Create OpenGL-ES Context
    eaglContext = [[EAGLContext alloc]initWithAPI:kEAGLRenderingAPIOpenGLES3];
    if(eaglContext == nil)
    {
        printf("OpenGL-ES Context Creation Failed!\n");
        return;
    }

    // Step 4: Set this OpenGL-ES Context as Current Context
    [EAGLContext setCurrentContext:eaglContext];

    // Step 5: Create Framebuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Step 6: Create Color Renderbuffer
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);

    // Step 7: Create Depth Renderbuffer
    GLint width;
    GLint height;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

    // Step 8: Check Whether Framebuffer is Complete or not
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("initWithFrame(): Framebuffer Creation is Not Complete!\n");
        [self uninitialize];
        return;
    }

    // Step 9: Initialize other Class Variables
    displayLink = nil;
    frameRateRange.minimum = 30.0f;
    frameRateRange.maximum = 60.0f;
    frameRateRange.preferred = 60.0f;
    isDisplayLink = NO;

    // Step 10: Call initialize() here
    int result = [self initialize];
    if(result != 0)
    {
        printf("initialize() Failed!\n");
    }
    else
    {
        printf("initialize() Completed Successfully!\n");
    }
    
    // Event Handling
    // Single Tap
    UITapGestureRecognizer *singleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onSingleTap:)];
    [singleTapGestureRecognizer setNumberOfTapsRequired:1];
    [singleTapGestureRecognizer setNumberOfTouchesRequired:1];
    [singleTapGestureRecognizer setDelegate:self];
    [self addGestureRecognizer:singleTapGestureRecognizer];

    // Double Tap
    UITapGestureRecognizer *doubleTapGestureRecognizer = [[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(onDoubleTap:)];
    [doubleTapGestureRecognizer setNumberOfTapsRequired:2];
    [doubleTapGestureRecognizer setNumberOfTouchesRequired:1];
    [doubleTapGestureRecognizer setDelegate:self];
    [self addGestureRecognizer:doubleTapGestureRecognizer];

    // Tell single tap recognizer to fail when there is double tap
    [singleTapGestureRecognizer requireGestureRecognizerToFail:doubleTapGestureRecognizer];

    // Swipe
    UISwipeGestureRecognizer *swipeGestureRecognizer = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(onSwipe:)];
    [swipeGestureRecognizer setDelegate:self];
    [self addGestureRecognizer:swipeGestureRecognizer];

    // Long press
    UILongPressGestureRecognizer *longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(onLongPress:)];
    [longPressGestureRecognizer setDelegate:self];
    [self addGestureRecognizer:longPressGestureRecognizer];
}

+(Class)layerClass
{
    // Code
    return([CAEAGLLayer class]);
}

/*
-(void)drawRect:(CGRect)rect
{
    // Code
}
*/

-(void)layoutSubviews
{
    // Code
    [super layoutSubviews];

    // Step 1: Delete all the Old Buffers
    // Delete Old depthRenderbuffer
    if(depthRenderbuffer)
    {
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }

    // Delete Old colorRenderbuffer
    if(colorRenderbuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }

    // Delete Old framebuffer
    if(framebuffer)
    {
        glDeleteFramebuffers(1, &framebuffer);
        framebuffer = 0;
    }
    
    // Step 2: Create Framebuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Step 3: Create Color Renderbuffer
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)[self layer]];
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);

    // Step 4: Create Depth Renderbuffer
    GLint width;
    GLint height;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

    // Step 5: Check Whether Framebuffer is Complete or not
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("layoutSubviews(): Framebuffer Creation is Not Complete!\n");
        return;
    }

    // Step 6: Call resize()
    [self resize:width :height];

    // Step 7: Call drawView()
    [self drawView:self];
}

-(void)drawView:(id)sender
{
    // Code
    // Set this OpenGL-ES Context as Current Context
    [EAGLContext setCurrentContext:eaglContext];

    // Bind Framebuffer 
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Render
    [self display];

    // Update
    [self myUpdate];

    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [eaglContext presentRenderbuffer:colorRenderbuffer];
}

-(void)startDisplayLink
{
    // Code
    // Start the DisplayLink to Create the Separate Rendering Thread which will Render our View
    if(isDisplayLink == NO)
    {
        // Create the DisplayLink and Set the Callback Function of this DisplayLink
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
        [displayLink setPreferredFrameRateRange:frameRateRange];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
        isDisplayLink = YES;
    }
}

-(void)stopDisplayLink
{
    // Code
    if(isDisplayLink == YES)
    {
        [displayLink invalidate];
        isDisplayLink = NO;
    }
}

-(BOOL)becomeFirstResponder
{
    // Code
    return (YES);
}

-(void)touchesBegan:(UITouch *)touches withEvent:(UIEvent *)event
{
    // Code
}

-(void)onSingleTap:(UITapGestureRecognizer *)gestureRecognizer
{
    // Code
    iSingleTap++;
    if(iSingleTap > 3)
    {
        iSingleTap = 0;
    }
}

-(void)onDoubleTap:(UITapGestureRecognizer *)gestureRecognizer
{
    // Code
    iDoubleTap++;
    if(iDoubleTap > 2)
    {
        iDoubleTap = 0;
    }
}

-(void)onSwipe:(UISwipeGestureRecognizer *)gestureRecognizer
{
    // Code
    [self uninitialize];
    [self release];
    exit(0);
}

-(void)onLongPress:(UILongPressGestureRecognizer *)gestureRecognizer
{
    // Code
}

-(int)initialize
{
    // Code
    // Print OpenGL-ES Information
    [self printGLESInfo];

    // VERTEX SHADER
    // Step 1 : Write the Shader Source Code
    const GLchar* vertexShaderSourceCode =
        "#version 300 es\n" \
        "precision highp float;\n" \
        "in vec4 aPosition;\n" \
        "uniform mat4 uMVPMatrix;\n" \
        "void main(void)\n" \
        "{\n" \
            "gl_Position = uMVPMatrix * aPosition;\n" \
        "}\n";

    // Step 2 : Create the Shader Object
    GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

    // Step 3 : Give the Shader Source Code to the Shader Object
    glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

    // Step 4 : Compile the Shader Programmatically
    glCompileShader(vertexShaderObject);

    // Step 5 : Shader Compilation Error Checking
    GLint status = 0;
    GLint infoLogLength = 0;
    GLchar* szInfoLog = NULL;

    glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar));
            if (szInfoLog != NULL)
            {
                glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, szInfoLog);
                printf("Vertex Shader Compilation Log : %s \n", szInfoLog);

                free(szInfoLog);
                szInfoLog = NULL;
            }
        }

        [self uninitialize];
        [self release];
        exit(0);
    }

    // FRAGMENT SHADER
    // Step 1 : Write the Shader Source Code
    const GLchar* fragmentShaderSourceCode =
        "#version 300 es\n" \
        "precision highp float;\n" \
        "uniform vec4 uColor;\n" \
        "out vec4 FragColor;\n" \
        "void main(void)\n" \
        "{\n" \
            "FragColor = uColor;\n" \
        "}\n";

    // Step 2 : Create the Shader Object
    GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

    // Step 3 : Give the Shader Source Code to the Shader Object
    glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

    // Step 4 : Compile the Shader Programmatically
    glCompileShader(fragmentShaderObject);

    // Step 5 : Shader Compilation Error Checking
    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar));
            if (szInfoLog != NULL)
            {
                glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL, szInfoLog);
                printf("Fragment Shader Compilation Log : %s \n", szInfoLog);

                free(szInfoLog);
                szInfoLog = NULL;
            }
        }

        [self uninitialize];
        [self release];
        exit(0);
    }

    // Create, Attach and Link Shader Program Object
    shaderProgramObject = glCreateProgram();

    glAttachShader(shaderProgramObject, vertexShaderObject);
    glAttachShader(shaderProgramObject, fragmentShaderObject);

    // Bind Shader Attributes at a certain index in Shader to same index in Host Program
    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");

    glLinkProgram(shaderProgramObject);

    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar));
            if (szInfoLog != NULL)
            {
                glGetProgramInfoLog(shaderProgramObject, infoLogLength, NULL, szInfoLog);
                printf("Shader Program Link Log : %s \n", szInfoLog);

                free(szInfoLog);
                szInfoLog = NULL;
            }
        }

        [self uninitialize];
        [self release];
        exit(0);
    }

    // Get the required uniform location from the shader
    mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");
    colorUniform = glGetUniformLocation(shaderProgramObject, "uColor");

    /*************************      GRAPH     *************************/
    /*************************      X-Axis    *************************/
    const GLfloat XAxis_Position[] =
    {
        1.0f, 0.0f, 0.0f,
       -1.0f, 0.0f, 0.0f,
    };

    // Vertex array object for arrays of vertex attributes
    glGenVertexArrays(1, &vao_XAxis);

    glBindVertexArray(vao_XAxis);

    // Position
    glGenBuffers(1, &vbo_position_XAxis);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_position_XAxis);

    glBufferData(GL_ARRAY_BUFFER, sizeof(XAxis_Position), XAxis_Position, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /*************************      Y-Axis    *************************/
    const GLfloat YAxis_Position[] =
    {
        0.0f,  1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
    };

    // Vertex array object for arrays of vertex attributes
    glGenVertexArrays(1, &vao_YAxis);

    glBindVertexArray(vao_YAxis);

    // Position
    glGenBuffers(1, &vbo_position_YAxis);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_position_YAxis);

    glBufferData(GL_ARRAY_BUFFER, sizeof(YAxis_Position), YAxis_Position, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /*************************      Horizontal Lines    *************************/
    GLfloat horizontalLines_Position[numberOfTotalLines * 2][3];
    float hX = 1.0f;
    float hY = 1.0f;

    for (int i = 0; i < numberOfLinesEachSide; i++)
    {
        horizontalLines_Position[i * 2][0] = hX;
        horizontalLines_Position[i * 2][1] = hY;
        horizontalLines_Position[i * 2][2] = 0.0f;

        horizontalLines_Position[i * 2 + 1][0] = -hX;
        horizontalLines_Position[i * 2 + 1][1] = hY;
        horizontalLines_Position[i * 2 + 1][2] = 0.0f;

        hY = hY - 0.05f;
    }

    hY = -0.05f;

    for (int i = 0; i < numberOfLinesEachSide; i++)
    {
        int base = (numberOfLinesEachSide + i) * 2;

        horizontalLines_Position[base][0] = hX;
        horizontalLines_Position[base][1] = hY;
        horizontalLines_Position[base][2] = 0.0f;

        horizontalLines_Position[base + 1][0] = -hX;
        horizontalLines_Position[base + 1][1] = hY;
        horizontalLines_Position[base + 1][2] = 0.0f;

        hY = hY - 0.05f;
    }

    // Vertex array object for arrays of vertex attributes
    glGenVertexArrays(1, &vao_horizontalLines);

    glBindVertexArray(vao_horizontalLines);

    // Position
    glGenBuffers(1, &vbo_position_horizontalLines);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_position_horizontalLines);

    glBufferData(GL_ARRAY_BUFFER, sizeof(horizontalLines_Position), horizontalLines_Position, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /*************************      Vertical Lines      *************************/
    GLfloat verticalLines_Position[numberOfTotalLines * 2][3];
    hX = 1.0f;
    hY = 1.0f;

    for (int i = 0; i < numberOfLinesEachSide; i++)
    {
        verticalLines_Position[i * 2][0] = hX;
        verticalLines_Position[i * 2][1] = hY;
        verticalLines_Position[i * 2][2] = 0.0f;

        verticalLines_Position[i * 2 + 1][0] = hX;
        verticalLines_Position[i * 2 + 1][1] = -hY;
        verticalLines_Position[i * 2 + 1][2] = 0.0f;

        hX = hX - 0.05f;
    }

    hX = -0.05f;

    for (int i = 0; i < numberOfLinesEachSide; i++)
    {
        int base = (numberOfLinesEachSide + i) * 2;

        verticalLines_Position[base][0] = hX;
        verticalLines_Position[base][1] = hY;
        verticalLines_Position[base][2] = 0.0f;

        verticalLines_Position[base + 1][0] = hX;
        verticalLines_Position[base + 1][1] = -hY;
        verticalLines_Position[base + 1][2] = 0.0f;

        hX = hX - 0.05f;
    }

    // Vertex array object for arrays of vertex attributes
    glGenVertexArrays(1, &vao_verticalLines);

    glBindVertexArray(vao_verticalLines);

    // Position
    glGenBuffers(1, &vbo_position_verticalLines);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_position_verticalLines);

    glBufferData(GL_ARRAY_BUFFER, sizeof(verticalLines_Position), verticalLines_Position, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /*************************      TRIANGLE    *************************/
    // Provide Vertex position, Color, Normals, Texco-ords etc..
    // Position
    const GLfloat triangle_Position[] =
    {
        0.0f,  0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f,
    };

    // Vertex array object for arrays of vertex attributes
    glGenVertexArrays(1, &vao_triangle);

    glBindVertexArray(vao_triangle);

    // Position
    glGenBuffers(1, &vbo_position_triangle);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_position_triangle);

    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_Position), triangle_Position, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /*************************      SQUARE      *************************/
    // Provide Vertex position, Color, Normals, Texco-ords etc..
    // Position
    const GLfloat rectangle_Position[] =
    {
        0.5f,  0.5f, 0.0f,
       -0.5f,  0.5f, 0.0f,
       -0.5f,  0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f,  0.5f, 0.0f
    };

    // Vertex array object for arrays of vertex attributes
    glGenVertexArrays(1, &vao_rectangle);

    glBindVertexArray(vao_rectangle);

    // Position
    glGenBuffers(1, &vbo_position_rectangle);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_position_rectangle);

    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_Position), rectangle_Position, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /*************************      CIRCLE      *************************/
    GLfloat circle_Position[numberOfPointsToDrawCircle][3];
    GLfloat angle;

    for (int i = 0; i < numberOfPointsToDrawCircle; i++)
    {
        angle = (GLfloat)(2 * M_PI * i) / numberOfPointsToDrawCircle;
        circle_Position[i][0] = (GLfloat)cos(angle) * 0.7;
        circle_Position[i][1] = (GLfloat)sin(angle) * 0.7;
        circle_Position[i][2] = 0.0f;
    }

    // Vertex array object for arrays of vertex attributes
    glGenVertexArrays(1, &vao_circle);

    glBindVertexArray(vao_circle);

    // Position
    glGenBuffers(1, &vbo_position_circle);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_position_circle);

    glBufferData(GL_ARRAY_BUFFER, sizeof(circle_Position), circle_Position, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Depth related function calls..
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // From here onwards, OpenGL code starts...
    // Tell OpenGL to choose the color to clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // This line is analogous to glLoadIdentity in resize() for GL_PROJECTION in FFP
    perspectiveProjectionMatrix = mat4::identity();

    return(0);
}

-(void)printGLESInfo
{
    // Code
    printf("\n OPENGL-ES INFORMATION \n");
    printf("*************************************************\n");
    printf("OpenGL-ES Vender   : %s \n", glGetString(GL_VENDOR));
    printf("OpenGL-ES Renderer : %s \n", glGetString(GL_RENDERER));
    printf("OpenGL-ES Version  : %s \n", glGetString(GL_VERSION));
    printf("GLSL      Version  : %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("*************************************************\n\n");
}

-(void)resize:(int)width :(int)height
{
    // Code
    // If height becomes 0 or less than 0 by accident, then assign height to 1
    if(height <= 0)
        height = 1;

    // Set the ViewPort
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // This line is analogous to glMatrixMode(GL_PROJECTION); from resize() in FFP
    // Set Perspective Projection
    perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

-(void)display
{
    // Code
    // Clear OpenGL buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use Shader Program Object
    glUseProgram(shaderProgramObject);

    // Transformations
    // This line is analogous to glLoadIdentity() in display() for MODELVIEW matrix
    mat4 modelViewMatrix = mat4::identity();

    mat4 translationMatrix = mat4::identity();

    mat4 scaleMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

    scaleMatrix = vmath::scale(0.8f, 0.8f, 0.0f);

    modelViewMatrix = translationMatrix * scaleMatrix;  // Order is important (Matrix multiplication is non-commutative)

    // This line is analogous to glMatrixMode(GL_MODELVIEW); in display()
    mat4 modelViewProjectionMatrix = mat4::identity();

    modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;  // Order is important

    // Send above matrix to the vertex shader in uniform
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    if (iDoubleTap == 1)
    {
        [self drawGraph];
    }

    if (iSingleTap >= 1)
    {
        glUniform4fv(colorUniform, 1, vmath::vec4(1.0f, 1.0f, 0.0f, 1.0f));

        [self drawHollowTriangle];
    }

    if (iSingleTap >= 2)
    {
        glUniform4fv(colorUniform, 1, vmath::vec4(1.0f, 1.0f, 0.0f, 1.0f));

        [self drawHollowSquare];
    }

    if (iSingleTap >= 3)
    {
        glUniform4fv(colorUniform, 1, vmath::vec4(1.0f, 1.0f, 0.0f, 1.0f));

        [self drawHollowCircle];
    }

    if (iDoubleTap > 1)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // Unuse Shader Program Object
    glUseProgram(0);
}

-(void)drawGraph
{
    [self drawHorizontalLines];
    [self drawVerticalLines];
    [self drawXAxisandYAxis];
}

-(void)drawHorizontalLines
{
    glUniform4fv(colorUniform, 1, vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    // Bind with vao_horizontalLines
    glBindVertexArray(vao_horizontalLines);

    int iCnt;

    // Draw the vertex arrays
    for (int i = 0; i < numberOfTotalLines; i++)
    {
        if (i < numberOfLinesEachSide)
            iCnt = numberOfLinesEachSide - i;
        else
            iCnt = -(i - numberOfLinesEachSide + 1);

        if (iCnt != 0 && (iCnt % 5 == 0))
            glLineWidth(2.5f);
        else
            glLineWidth(1.25f);

        glDrawArrays(GL_LINES, i * 2, 2);
    }

    // UnBind with vao_horizontalLines
    glBindVertexArray(0);
}

-(void)drawVerticalLines
{
    glUniform4fv(colorUniform, 1, vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    // Bind with vao_verticalLines
    glBindVertexArray(vao_verticalLines);

    int iCnt;

    // Draw the vertex arrays
    for (int i = 0; i < numberOfTotalLines; i++)
    {
        if (i < numberOfLinesEachSide)
            iCnt = numberOfLinesEachSide - i;
        else
            iCnt = -(i - numberOfLinesEachSide + 1);

        if (iCnt != 0 && (iCnt % 5 == 0))
            glLineWidth(2.5f);
        else
            glLineWidth(1.25f);

        glDrawArrays(GL_LINES, i * 2, 2);
    }

    // UnBind with vao_verticalLines
    glBindVertexArray(0);
}

-(void)drawXAxisandYAxis
{
    /*************************      X-Axis      *************************/

    glUniform4fv(colorUniform, 1, vmath::vec4(1.0f, 0.0f, 0.0f, 1.0f));

    // Bind with vao_XAxis
    glBindVertexArray(vao_XAxis);

    // Draw the vertex arrays
    glLineWidth(5.0f);
    glDrawArrays(GL_LINES, 0, 2);

    // UnBind with vao_XAxis
    glBindVertexArray(0);

    /*************************      Y-Axis      *************************/

    glUniform4fv(colorUniform, 1, vmath::vec4(0.0f, 1.0f, 0.0f, 1.0f));

    // Bind with vao_YAxis
    glBindVertexArray(vao_YAxis);

    // Draw the vertex arrays
    glLineWidth(5.0f);
    glDrawArrays(GL_LINES, 0, 2);

    // UnBind with vao_YAxis
    glBindVertexArray(0);
}

-(void)drawHollowTriangle
{
    // Bind with vao_triangle
    glBindVertexArray(vao_triangle);

    // Draw the vertex arrays
    glLineWidth(3.0f);
    glDrawArrays(GL_LINES, 0, 6);

    // UnBind with vao_triangle
    glBindVertexArray(0);
}

-(void)drawHollowSquare
{
    // Bind with vao_rectangle
    glBindVertexArray(vao_rectangle);

    // Draw the vertex arrays
    glLineWidth(3.0f);
    glDrawArrays(GL_LINES, 0, 2);
    glLineWidth(3.0f);
    glDrawArrays(GL_LINES, 2, 2);
    glLineWidth(3.0f);
    glDrawArrays(GL_LINES, 4, 2);
    glLineWidth(3.0f);
    glDrawArrays(GL_LINES, 6, 2);

    // UnBind with vao_rectangle
    glBindVertexArray(0);
}

-(void)drawHollowCircle
{
    // Bind with vao_circle
    glBindVertexArray(vao_circle);

    // Draw the vertex arrays
    glLineWidth(3.5f);
    glDrawArrays(GL_LINE_LOOP, 0, numberOfPointsToDrawCircle);

    // UnBind with vao_circle
    glBindVertexArray(0);
}

-(void)myUpdate
{
    // Code
}

-(void)uninitialize
{
    // Code
    // Free vbo_position_verticalLines
    if (vbo_position_verticalLines)
    {
        glDeleteBuffers(1, &vbo_position_verticalLines);
        vbo_position_verticalLines = 0;
    }

    // Free vbo_position_horizontalLines
    if (vbo_position_horizontalLines)
    {
        glDeleteBuffers(1, &vbo_position_horizontalLines);
        vbo_position_horizontalLines = 0;
    }

    // Free vbo_position_YAxis
    if (vbo_position_YAxis)
    {
        glDeleteBuffers(1, &vbo_position_YAxis);
        vbo_position_YAxis = 0;
    }

    // Free vbo_position_XAxis
    if (vbo_position_XAxis)
    {
        glDeleteBuffers(1, &vbo_position_XAxis);
        vbo_position_XAxis = 0;
    }

    // Free vbo_position_circle
    if (vbo_position_circle)
    {
        glDeleteBuffers(1, &vbo_position_circle);
        vbo_position_circle = 0;
    }

    // Free vbo_position_rectangle
    if (vbo_position_rectangle)
    {
        glDeleteBuffers(1, &vbo_position_rectangle);
        vbo_position_rectangle = 0;
    }

    // Free vbo_position_triangle
    if (vbo_position_triangle)
    {
        glDeleteBuffers(1, &vbo_position_triangle);
        vbo_position_triangle = 0;
    }

    // Free vao_verticalLines
    if (vao_verticalLines)
    {
        glDeleteVertexArrays(1, &vao_verticalLines);
        vao_verticalLines = 0;
    }

    // Free vao_horizontalLines
    if (vao_horizontalLines)
    {
        glDeleteVertexArrays(1, &vao_horizontalLines);
        vao_horizontalLines = 0;
    }

    // Free vao_YAxis
    if (vao_YAxis)
    {
        glDeleteVertexArrays(1, &vao_YAxis);
        vao_YAxis = 0;
    }

    // Free vao_XAxis
    if (vao_XAxis)
    {
        glDeleteVertexArrays(1, &vao_XAxis);
        vao_XAxis = 0;
    }

    // Free vao_circle
    if (vao_circle)
    {
        glDeleteVertexArrays(1, &vao_circle);
        vao_circle = 0;
    }

    // Free vao_rectangle
    if (vao_rectangle)
    {
        glDeleteVertexArrays(1, &vao_rectangle);
        vao_rectangle = 0;
    }

    // Free vao_triangle
    if (vao_triangle)
    {
        glDeleteVertexArrays(1, &vao_triangle);
        vao_triangle = 0;
    }

    // Detach, Delete Shader Objects and Delete Shader Program Object
    // Step 1: Check whether Shader Program Object is still there
    if (shaderProgramObject)
    {
        glUseProgram(shaderProgramObject);
        GLint numShaders;

        // Step 2 : Get Number of Attached Shaders and Continue if Number Of Shaders is Greater than 0 
        glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numShaders);
        if (numShaders > 0)
        {
            // Step 3 : Create a Buffer / Array to hold Attached Shader Object of Obtained Number of Shaders and Continue Only if malloc is Succeeded
            GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));
            if (pShaders != NULL)
            {
                // Step 4 : Get Shader Objects into this Buffer / Array
                glGetAttachedShaders(shaderProgramObject, numShaders, NULL, pShaders);
                
                // Step 5 : Iterate through Obtained Number of Shaders and inside this loop Detach and Delete Every Shader from the Buffer / Array
                for (GLint i = 0; i < numShaders; i++)
                {
                    glDetachShader(shaderProgramObject, pShaders[i]);
                    glDeleteShader(pShaders[i]);
                    pShaders[i] = 0;
                }
            }

            // Step 6 : Free the Buffer / Array
            free(pShaders);
            pShaders = 0;
        }

        // Step 7 : Delete the Shader Program Object
        glUseProgram(0);
        glDeleteProgram(shaderProgramObject);
        shaderProgramObject = 0;
    }

    // Free depthRenderbuffer
    if(depthRenderbuffer)
    {
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }

    // Free colorRenderbuffer
    if(colorRenderbuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }

    // Free framebuffer
    if(framebuffer)
    {
        glDeleteFramebuffers(1, &framebuffer);
        framebuffer = 0;
    }

    // Delete the Current EAGLContext
    if([EAGLContext currentContext] == eaglContext)
    {
        [EAGLContext setCurrentContext:nil];
        eaglContext = nil;
    }
}

-(void)dealloc
{
    // Code
    [self uninitialize];
    [self release];

    [super dealloc];
}

@end
