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

enum
{
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_COLOR,
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

    GLuint vao_pyramid; // Vertex Array Object
    GLuint vao_cube;    // Vertex Array Object

    GLuint vbo_position_pyramid;    // Vertex Buffer Object
    GLuint vbo_position_cube;       // Vertex Buffer Object

    GLuint vbo_color_pyramid;
    GLuint vbo_color_cube;

    GLuint mvpMatrixUniform;

    mat4 perspectiveProjectionMatrix;

    // Rotation angles
    GLfloat anglePyramid;
    GLfloat angleCube;
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
}

-(void)onDoubleTap:(UITapGestureRecognizer *)gestureRecognizer
{
    // Code
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
        "in vec4 aColor;\n" \
        "uniform mat4 uMVPMatrix;\n" \
        "out vec4 out_Color;\n" \
        "void main(void)\n" \
        "{\n" \
            "gl_Position = uMVPMatrix * aPosition;\n" \
            "out_Color = aColor;\n" \
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
        "in vec4 out_Color;\n" \
        "out vec4 FragColor;\n" \
        "void main(void)\n" \
        "{\n" \
            "FragColor = out_Color;\n" \
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
    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_COLOR, "aColor");

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

    /*************************      PYRAMID     *************************/
    // Provide Vertex position, Color, Normals, Texco-ords etc..
    // Position
    const GLfloat pyramid_Position[] =
    {
        // front
        0.0f,  1.0f,  0.0f,     // front-top
       -1.0f, -1.0f,  1.0f,     // front-left
        1.0f, -1.0f,  1.0f,     // front-right

        // right
        0.0f,  1.0f,  0.0f,     // right-top
        1.0f, -1.0f,  1.0f,     // right-left
        1.0f, -1.0f, -1.0f,     // right-right

        // back
        0.0f,  1.0f,  0.0f,     // back-top
        1.0f, -1.0f, -1.0f,     // back-left
       -1.0f, -1.0f, -1.0f,     // back-right
       
       // left
       0.0f,  1.0f,  0.0f,      // left-top
      -1.0f, -1.0f, -1.0f,      // left-left
      -1.0f, -1.0f,  1.0f,      // left-right
    };

    // Color
    const GLfloat pyramid_Color[] =
    {
        // front
        1.0f, 0.0f, 0.0f,   // front-top
        0.0f, 1.0f, 0.0f,   // front-left
        0.0f, 0.0f, 1.0f,   // front-right

        // right
        1.0f, 0.0f, 0.0f,   // right-top
        0.0f, 0.0f, 1.0f,   // right-left
        0.0f, 1.0f, 0.0f,   // right-right

        // back
        1.0f, 0.0f, 0.0f,   // back-top
        0.0f, 1.0f, 0.0f,   // back-left
        0.0f, 0.0f, 1.0f,   // back-right

        // left
        1.0f, 0.0f, 0.0f,   // left-top
        0.0f, 0.0f, 1.0f,   // left-left
        0.0f, 1.0f, 0.0f,   // left-right
    };

    /*************************      CUBE    *************************/
    // Provide Vertex position, Color, Normals, Texco-ords etc..
    // Position
    const GLfloat cube_Position[] =
    {
        // front
        1.0f,  1.0f,  1.0f,     // top-right of front
       -1.0f,  1.0f,  1.0f,     // top-left of front
       -1.0f, -1.0f,  1.0f,     // bottom-left of front
        1.0f, -1.0f,  1.0f,     // bottom-right of front

        // right
        1.0f,  1.0f, -1.0f,     // top-right of right
        1.0f,  1.0f,  1.0f,     // top-left of right
        1.0f, -1.0f,  1.0f,     // bottom-left of right
        1.0f, -1.0f, -1.0f,     // bottom-right of right

        // back
        1.0f,  1.0f, -1.0f,     // top-right of back
       -1.0f,  1.0f, -1.0f,     // top-left of back
       -1.0f, -1.0f, -1.0f,     // bottom-left of back
        1.0f, -1.0f, -1.0f,     // bottom-right of back
        
        // left
        -1.0f,  1.0f,  1.0f,    // top-right of left
        -1.0f,  1.0f, -1.0f,    // top-left of left
        -1.0f, -1.0f, -1.0f,    // bottom-left of left
        -1.0f, -1.0f,  1.0f,    // bottom-right of left
        
        // top
        1.0f,  1.0f, -1.0f,     // top-right of top
       -1.0f,  1.0f, -1.0f,     // top-left of top
       -1.0f,  1.0f,  1.0f,     // bottom-left of top
        1.0f,  1.0f,  1.0f,     // bottom-right of top
        
        // bottom
        1.0f, -1.0f,  1.0f,     // top-right of bottom
       -1.0f, -1.0f,  1.0f,     // top-left of bottom
       -1.0f, -1.0f, -1.0f,     // bottom-left of bottom
        1.0f, -1.0f, -1.0f,     // bottom-right of bottom
    };

    // Color
    const GLfloat cube_Color[] =
    {
        // front
        1.0f, 0.0f, 0.0f,   // top-right of front
        1.0f, 0.0f, 0.0f,   // top-left of front
        1.0f, 0.0f, 0.0f,   // bottom-left of front
        1.0f, 0.0f, 0.0f,   // bottom-right of front

        // right
        0.0f, 0.0f, 1.0f,   // top-right of right
        0.0f, 0.0f, 1.0f,   // top-left of right
        0.0f, 0.0f, 1.0f,   // bottom-left of right
        0.0f, 0.0f, 1.0f,   // bottom-right of right

        // back
        1.0f, 1.0f, 0.0f,   // top-right of back
        1.0f, 1.0f, 0.0f,   // top-left of back
        1.0f, 1.0f, 0.0f,   // bottom-left of back
        1.0f, 1.0f, 0.0f,   // bottom-right of back

        // left
        1.0f, 0.0f, 1.0f,   // top-right of left
        1.0f, 0.0f, 1.0f,   // top-left of left
        1.0f, 0.0f, 1.0f,   // bottom-left of left
        1.0f, 0.0f, 1.0f,   // bottom-right of left

        // top
        0.0f, 1.0f, 0.0f,   // top-right of top
        0.0f, 1.0f, 0.0f,   // top-left of top
        0.0f, 1.0f, 0.0f,   // bottom-left of top
        0.0f, 1.0f, 0.0f,   // bottom-right of top

        // bottom
        1.0f, 0.5f, 0.0f,   // top-right of bottom
        1.0f, 0.5f, 0.0f,   // top-left of bottom
        1.0f, 0.5f, 0.0f,   // bottom-left of bottom
        1.0f, 0.5f, 0.0f,   // bottom-right of bottom
    };

    /*************************      PYRAMID     *************************/
    // Vertex array object for arrays of vertex attributes
    glGenVertexArrays(1, &vao_pyramid);

    glBindVertexArray(vao_pyramid);

    // Position
    glGenBuffers(1, &vbo_position_pyramid);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_position_pyramid);

    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_Position), pyramid_Position, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Color
    glGenBuffers(1, &vbo_color_pyramid);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_color_pyramid);

    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_Color), pyramid_Color, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /*************************      CUBE    *************************/
    // Vertex array object for arrays of vertex attributes
    glGenVertexArrays(1, &vao_cube);

    glBindVertexArray(vao_cube);

    // Position
    glGenBuffers(1, &vbo_position_cube);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_position_cube);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_Position), cube_Position, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Color
    glGenBuffers(1, &vbo_color_cube);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_color_cube);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_Color), cube_Color, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);

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

    /*************************      PYRAMID     *************************/
    // Transformations
    // This line is analogous to glLoadIdentity() in display() for MODELVIEW matrix
    mat4 modelViewMatrix = mat4::identity();

    mat4 translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(-1.5f, 0.0f, -6.0f);

    mat4 rotationMatrix = mat4::identity();

    rotationMatrix = vmath::rotate(anglePyramid, 0.0f, 1.0f, 0.0f);

    modelViewMatrix = translationMatrix * rotationMatrix;   // Order is important (Matrix multiplication is non-commutative)

    // This line is analogous to glMatrixMode(GL_MODELVIEW); in display()
    mat4 modelViewProjectionMatrix = mat4::identity();

    modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;  // Order is important

    // Send above matrix to the vertex shader in uniform
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    // Bind with vao_pyramid
    glBindVertexArray(vao_pyramid);

    // Draw the vertex arrays
    glDrawArrays(GL_TRIANGLES, 0, 12);

    // UnBind with vao_pyramid
    glBindVertexArray(0);

    /*************************      CUBE    *************************/
    // Transformations
    // This line is analogous to glLoadIdentity() in display() for MODELVIEW matrix
    modelViewMatrix = mat4::identity();

    translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(1.5f, 0.0f, -6.0f);

    mat4 scaleMatrix = mat4::identity();

    scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

    rotationMatrix = mat4::identity();

    mat4 rotationMatrixX = mat4::identity();
    rotationMatrixX = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);
    
    mat4 rotationMatrixY = mat4::identity();
    rotationMatrixY = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);
    
    mat4 rotationMatrixZ = mat4::identity();
    rotationMatrixZ = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);

    rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;

    modelViewMatrix = translationMatrix * scaleMatrix * rotationMatrix; // Order is important (Matrix multiplication is non-commutative)

    // This line is analogous to glMatrixMode(GL_MODELVIEW); in display()
    modelViewProjectionMatrix = mat4::identity();

    modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix;  // Order is important

    // Send above matrix to the vertex shader in uniform
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

    // Bind with vao_cube
    glBindVertexArray(vao_cube);

    // Draw the vertex arrays
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

    // UnBind with vao_cube
    glBindVertexArray(0);

    // Unuse Shader Program Object
    glUseProgram(0);
}

-(void)myUpdate
{
    // Code
    // PYRAMID
    anglePyramid = anglePyramid + 0.5f;
    if (anglePyramid >= 360.0f)
    {
        anglePyramid = anglePyramid - 360.0f;
    }

    // CUBE
    angleCube = angleCube - 0.5f;
    if (angleCube <= 0.0f)
    {
        angleCube = angleCube + 360.0f;
    }
}

-(void)uninitialize
{
    // Code
    // Free vbo_color_cube
    if (vbo_color_cube)
    {
        glDeleteBuffers(1, &vbo_color_cube);
        vbo_color_cube = 0;
    }

    // Free vbo_position_cube
    if (vbo_position_cube)
    {
        glDeleteBuffers(1, &vbo_position_cube);
        vbo_position_cube = 0;
    }

    // Free vbo_color_pyramid
    if (vbo_color_pyramid)
    {
        glDeleteBuffers(1, &vbo_color_pyramid);
        vbo_color_pyramid = 0;
    }

    // Free vbo_position_pyramid
    if (vbo_position_pyramid)
    {
        glDeleteBuffers(1, &vbo_position_pyramid);
        vbo_position_pyramid = 0;
    }

    // Free vao_cube
    if (vao_cube)
    {
        glDeleteVertexArrays(1, &vao_cube);
        vao_cube = 0;
    }

    // Free vao_pyramid
    if (vao_pyramid)
    {
        glDeleteVertexArrays(1, &vao_pyramid);
        vao_pyramid = 0;
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
