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

// Sphere related header file
#import "Sphere.h"

// Macros related to FBO
#define FBO_WIDTH 512
#define FBO_HEIGHT 512

enum
{
    AMC_ATTRIBUTE_POSITION = 0,
    AMC_ATTRIBUTE_COLOR,
    AMC_ATTRIBUTE_TEXCOORD,
    AMC_ATTRIBUTE_NORMAL,
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
    GLuint shaderProgramObject_Cube;
    GLuint shaderProgramObject_Sphere;

    // Global variables related to Cube objects
    GLuint vao_cube;            // Vertex Array Object
    GLuint vbo_position_cube;   // Vertex Buffer Object
    GLuint vbo_texcoord_cube;

    // Texture related global variables
    GLuint textureSamplerUniform_Cube;

    GLuint mvpMatrixUniform_Cube;

    mat4 perspectiveProjectionMatrix_Cube;

    // Global variables related to FBO
    int viewWidth;
    int viewHeight;

    GLuint fbo;     // Frame Buffer Object
    GLuint rbo;     // Render Buffer Object
    GLuint texture_FBO;
    int fboResult;

    // Global variables related to Sphere
    // Global variables related to Sphere objects
    GLuint vao_sphere;          // Vertex Array Object
    GLuint vbo_position_sphere; // Vertex Buffer Object
    GLuint vbo_normal_sphere;
    GLuint vbo_element_sphere;

    // Global variables related to Sphere
    GLuint gNumVertices_Sphere;
    GLuint gNumElements_Sphere;

    GLfloat sphere_vertices[1146];
    GLfloat sphere_normals[1146];
    GLfloat sphere_textures[764];
    unsigned short sphere_elements[2280];

    // Uniform related to matrix
    GLuint modelMatrixUniform_Sphere;
    GLuint viewMatrixUniform_Sphere;
    GLuint projectionMatrixUniform_Sphere;

    // Uniform related to lights
    GLuint laUniform_Sphere;
    GLuint ldUniform_Sphere;
    GLuint lsUniform_Sphere;
    GLuint lightPositionUniform_Sphere;

    // Uniform related to material
    GLuint kaUniform_Sphere;
    GLuint kdUniform_Sphere;
    GLuint ksUniform_Sphere;
    GLuint materialShininessUniform_Sphere;

    // Uniform related to DoubleTap
    int lDoubleTapUniform_Sphere;

    // Global Variables related to lights
    GLfloat lightAmbient_Sphere[4];
    GLfloat lightDiffuse_Sphere[4];
    GLfloat lightSpecular_Sphere[4];
    GLfloat lightPosition_Sphere[4];

    GLfloat materialAmbient_Sphere[4];
    GLfloat materialDiffuse_Sphere[4];
    GLfloat materialSpecular_Sphere[4];
    GLfloat materialShininess_Sphere;

    BOOL bLight_Sphere;

    int iDoubleTap;

    mat4 perspectiveProjectionMatrix_Sphere;

    // Rotation angles
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
    
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Step 3: Create Color Renderbuffer
    //glGenRenderbuffers(1, &colorRenderbuffer);
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
    iDoubleTap++;
    if(iDoubleTap > 1)
    {
        iDoubleTap = 0;
        bLight_Sphere = NO;
    }
    else
    {
        bLight_Sphere = YES;
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
        "precision highp int;\n" \
        "precision highp float;\n" \
        "in vec4 aPosition;\n" \
        "in vec2 aTexCoord;\n" \
        "uniform mat4 uMVPMatrix;\n" \
        "out vec2 out_TexCoord;\n" \
        "void main(void)\n" \
        "{\n" \
            "gl_Position = uMVPMatrix * aPosition;\n" \
            "out_TexCoord = aTexCoord;\n" \
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
        "precision highp int;\n" \
        "precision highp float;\n" \
        "in vec2 out_TexCoord;\n" \
        "uniform sampler2D uTextureSampler;\n" \
        "out vec4 FragColor;\n" \
        "void main(void)\n" \
        "{\n" \
            "FragColor = texture(uTextureSampler, out_TexCoord);\n" \
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
    shaderProgramObject_Cube = glCreateProgram();

    glAttachShader(shaderProgramObject_Cube, vertexShaderObject);
    glAttachShader(shaderProgramObject_Cube, fragmentShaderObject);

    // Bind Shader Attributes at a certain index in Shader to same index in Host Program
    glBindAttribLocation(shaderProgramObject_Cube, AMC_ATTRIBUTE_POSITION, "aPosition");
    glBindAttribLocation(shaderProgramObject_Cube, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");

    glLinkProgram(shaderProgramObject_Cube);

    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetProgramiv(shaderProgramObject_Cube, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject_Cube, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar));
            if (szInfoLog != NULL)
            {
                glGetProgramInfoLog(shaderProgramObject_Cube, infoLogLength, NULL, szInfoLog);
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
    mvpMatrixUniform_Cube = glGetUniformLocation(shaderProgramObject_Cube, "uMVPMatrix");
    textureSamplerUniform_Cube = glGetUniformLocation(shaderProgramObject_Cube, "uTextureSampler");

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

    // TexCo-ords
    const GLfloat cube_TexCoord[] =
    {
        // front
        1.0f, 1.0f,     // top-right of front
        0.0f, 1.0f,     // top-left of front
        0.0f, 0.0f,     // bottom-left of front
        1.0f, 0.0f,     // bottom-right of front

        // right
        1.0f, 1.0f,     // top-right of right
        0.0f, 1.0f,     // top-left of right
        0.0f, 0.0f,     // bottom-left of right
        1.0f, 0.0f,     // bottom-right of right

        // back
        1.0f, 1.0f,     // top-right of back
        0.0f, 1.0f,     // top-left of back
        0.0f, 0.0f,     // bottom-left of back
        1.0f, 0.0f,     // bottom-right of back

        // left
        1.0f, 1.0f,     // top-right of left
        0.0f, 1.0f,     // top-left of left
        0.0f, 0.0f,     // bottom-left of left
        1.0f, 0.0f,     // bottom-right of left

        // top
        1.0f, 1.0f,     // top-right of top
        0.0f, 1.0f,     // top-left of top
        0.0f, 0.0f,     // bottom-left of top
        1.0f, 0.0f,     // bottom-right of top

        // bottom
        1.0f, 1.0f,     // top-right of bottom
        0.0f, 1.0f,     // top-left of bottom
        0.0f, 0.0f,     // bottom-left of bottom
        1.0f, 0.0f,     // bottom-right of bottom
    };

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

    // Texture
    glGenBuffers(1, &vbo_texcoord_cube);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoord_cube);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_TexCoord), cube_TexCoord, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Depth related function calls..
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // From here onwards, OpenGL code starts...
    // Tell OpenGL to choose the color to clear the screen
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // Screen color is set to White for beautification (Not syntactically compulsory for FBO)

    // Enable texturing
    //glEnable(GL_TEXTURE_2D);

    // This line is analogous to glLoadIdentity in resize() for GL_PROJECTION in FFP
    perspectiveProjectionMatrix_Cube = mat4::identity();

    // Create FBO and If successful, call initialise_Sphere
    if ([self createAndPrepareFBOforDrawing:FBO_WIDTH :FBO_HEIGHT] == YES)
    {
        printf("createAndPrepareFBOforDrawing() Succeeded!\n");
        fboResult = [self initialize_Sphere];
        if (fboResult != 0)
        {
            printf("initialize_Sphere() Failed!\n");
            return(-1);
        }
        else
        {
            printf("initialize_Sphere() Succeeded!\n");
        }
    }
    else
    {
        printf("createAndPrepareFBOforDrawing() Failed!\n");
        return(-2);
    }   

    return(0);
}

-(BOOL)createAndPrepareFBOforDrawing:(GLint)textureWidth :(GLint)textureHeight
{
    // Variable declarations
    // Step 1: Check whether textureWidth and textureHeight parameters are compatible.
    GLint maxRenderBufferSize;
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderBufferSize);
    if ((maxRenderBufferSize <= textureWidth) || (maxRenderBufferSize <= textureHeight))
    {
        printf("FBO_WIDTH or FBO_HEIGHT are exceeding maxRenderBufferSize!\n");
        return(NO);
    }

    // Code
    // Step 2: Create new Framebuffer for our Sphere
    glGenFramebuffers(1, &fbo);

    // Step 3: Bind with the newly created Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Step 4: Create Renderbuffer as placeholder for Depthbuffer of our Framebuffer
    glGenRenderbuffers(1, &rbo);

    // Step 5: Bind with the newly created Renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);

    // Step 6: Give specific storage to this Renderbuffer, so Depth needed for Sphere will reside here
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, textureWidth, textureHeight);

    // Step 7: Create an empty but fully capable and compatible texture object for our Sphere
    glGenTextures(1, &texture_FBO);
    glBindTexture(GL_TEXTURE_2D, texture_FBO);

    // Step 8: To create full featured texture, give appropriate parameters to the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Step 9: Now create this texture for our FBO
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);   // NULL indicates that this is empty texture

    // Step 10: Give this empty texture to the Framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_FBO, 0);

    // Step 11: Attach previously created Depth Renderbuffer to our Framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Step 12: Check whether status of our Framebuffer is complete or not
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("FBO Creation is incomplete!\n");
        return(NO);
    }

    // Step 13: UnBind with the newly created Framebuffer for now, we will use it when needed
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return(YES);
}

-(int)initialize_Sphere
{
    // Code
    // VERTEX SHADER
    // Step 1 : Write the Shader Source Code
    const GLchar* vertexShaderSourceCode =
        "#version 300 es\n" \
        "precision highp int;\n" \
        "precision highp float;\n" \
        "in vec4 aPosition;\n" \
        "in vec3 aNormal;\n" \
        "uniform mat4 uModelMatrix;\n" \
        "uniform mat4 uViewMatrix;\n" \
        "uniform mat4 uProjectionMatrix;\n" \
        "uniform vec4 uLightPosition;\n" \
        "uniform int uDoubleTap;\n" \
        "out vec3 out_TransformedNormals;\n" \
        "out vec3 out_LightDirection;\n" \
        "out vec3 out_ViewerVector;\n" \
        "void main(void)\n" \
        "{\n" \
            "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n" \
            "if(uDoubleTap == 1)\n" \
            "{\n" \
                "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n" \
                "mat3 normalMatrix = mat3(uViewMatrix * uModelMatrix);\n" \
                "out_TransformedNormals = normalMatrix * aNormal;\n" \
                "out_LightDirection = vec3(uLightPosition - eyeCoordinates);\n" \
                "out_ViewerVector = -eyeCoordinates.xyz;\n" \
            "}\n" \
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
        "precision highp int;\n" \
        "precision highp float;\n" \
        "in vec3 out_TransformedNormals;\n" \
        "in vec3 out_LightDirection;\n" \
        "in vec3 out_ViewerVector;\n" \
        "uniform vec3 uLa;\n" \
        "uniform vec3 uLd;\n" \
        "uniform vec3 uLs;\n" \
        "uniform vec3 uKa;\n" \
        "uniform vec3 uKd;\n" \
        "uniform vec3 uKs;\n" \
        "uniform float uMaterialShininess;\n" \
        "uniform int uDoubleTap;\n" \
        "out vec4 FragColor;\n" \
        "void main(void)\n" \
        "{\n" \
            "vec3 phong_ads_Light;\n" \
            "if(uDoubleTap == 1)\n" \
            "{\n" \
                "vec3 normalizedTransformedNormals = normalize(out_TransformedNormals);\n" \
                "vec3 normalizedLightDirection = normalize(out_LightDirection);\n" \
                "vec3 normalizedViewerVector = normalize(out_ViewerVector);\n" \
                "vec3 ambientLight = uLa * uKa;\n" \
                "vec3 diffuseLight = uLd * uKd * max(dot(normalizedLightDirection, normalizedTransformedNormals), 0.0f);\n" \
                "vec3 reflectionVector = reflect(-normalizedLightDirection, normalizedTransformedNormals);\n" \
                "vec3 specularLight = uLs * uKs * pow(max(dot(reflectionVector, normalizedViewerVector), 0.0f), uMaterialShininess);\n" \
                "phong_ads_Light = ambientLight + diffuseLight + specularLight;\n" \
            "}\n" \
            "else\n" \
            "{\n" \
                "phong_ads_Light = vec3(1.0f, 1.0f, 1.0f);\n" \
            "}\n" \
            "FragColor = vec4(phong_ads_Light, 1.0f);\n" \
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
    shaderProgramObject_Sphere = glCreateProgram();

    glAttachShader(shaderProgramObject_Sphere, vertexShaderObject);
    glAttachShader(shaderProgramObject_Sphere, fragmentShaderObject);

    // Bind Shader Attributes at a certain index in Shader to same index in Host Program
    glBindAttribLocation(shaderProgramObject_Sphere, AMC_ATTRIBUTE_POSITION, "aPosition");
    glBindAttribLocation(shaderProgramObject_Sphere, AMC_ATTRIBUTE_NORMAL, "aNormal");

    glLinkProgram(shaderProgramObject_Sphere);

    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetProgramiv(shaderProgramObject_Sphere, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject_Sphere, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar));
            if (szInfoLog != NULL)
            {
                glGetProgramInfoLog(shaderProgramObject_Sphere, infoLogLength, NULL, szInfoLog);
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
    modelMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uModelMatrix");
    viewMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uViewMatrix");
    projectionMatrixUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uProjectionMatrix");
    
    laUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uLa");
    ldUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uLd");
    lsUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uLs");
    lightPositionUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uLightPosition");
    
    kaUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uKa");
    kdUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uKd");
    ksUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uKs");
    materialShininessUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uMaterialShininess");
    
    lDoubleTapUniform_Sphere = glGetUniformLocation(shaderProgramObject_Sphere, "uDoubleTap");

    /*************************      SPHERE      *************************/
    // Provide Vertex position, Color, Normals, Texco-ords etc..
    Sphere *sphere = [[Sphere alloc]init];
    [sphere getSphereVertexData:sphere_vertices :sphere_normals :sphere_textures :sphere_elements];
    
    gNumVertices_Sphere = [sphere getNumberOfSphereVertices];
    gNumElements_Sphere = [sphere getNumberOfSphereElements];

    // Vertex array object for arrays of vertex attributes
    glGenVertexArrays(1, &vao_sphere);

    glBindVertexArray(vao_sphere);

    // Position
    glGenBuffers(1, &vbo_position_sphere);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_position_sphere);

    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Normal
    glGenBuffers(1, &vbo_normal_sphere);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_normal_sphere);

    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

    glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Element
    glGenBuffers(1, &vbo_element_sphere);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element_sphere);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Unbind vao
    glBindVertexArray(0);

    // Depth related function calls..
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // From here onwards, OpenGL code starts...
    // Tell OpenGL to choose the color to clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Lights Initialization
    lightAmbient_Sphere[0] = 0.1f;
    lightAmbient_Sphere[1] = 0.1f;
    lightAmbient_Sphere[2] = 0.1f;
    lightAmbient_Sphere[3] = 1.0f;

    lightDiffuse_Sphere[0] = 1.0f;
    lightDiffuse_Sphere[1] = 1.0f;
    lightDiffuse_Sphere[2] = 1.0f;
    lightDiffuse_Sphere[3] = 1.0f;

    lightSpecular_Sphere[0] = 1.0f;
    lightSpecular_Sphere[1] = 1.0f;
    lightSpecular_Sphere[2] = 1.0f;
    lightSpecular_Sphere[3] = 1.0f;

    lightPosition_Sphere[0] = 100.0f;
    lightPosition_Sphere[1] = 100.0f;
    lightPosition_Sphere[2] = 100.0f;
    lightPosition_Sphere[3] = 1.0f;

    materialAmbient_Sphere[0] = 0.0f;
    materialAmbient_Sphere[1] = 0.0f;
    materialAmbient_Sphere[2] = 0.0f;
    materialAmbient_Sphere[3] = 1.0f;

    materialDiffuse_Sphere[0] = 0.5f;
    materialDiffuse_Sphere[1] = 0.2f;
    materialDiffuse_Sphere[2] = 0.7f;
    materialDiffuse_Sphere[3] = 1.0f;

    materialSpecular_Sphere[0] = 0.7f;
    materialSpecular_Sphere[1] = 0.7f;
    materialSpecular_Sphere[2] = 0.7f;
    materialSpecular_Sphere[3] = 1.0f;

    materialShininess_Sphere = 128.0f;

    bLight_Sphere = NO;

    iDoubleTap = 0;

    // This line is analogous to glLoadIdentity in resize() for GL_PROJECTION in FFP
    perspectiveProjectionMatrix_Sphere = mat4::identity();

    // Warm-up resize
    [self resize_Sphere:FBO_WIDTH :FBO_HEIGHT];

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

    viewWidth = width;
    viewHeight = height;

    // Set the ViewPort
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // This line is analogous to glMatrixMode(GL_PROJECTION); from resize() in FFP
    // Set Perspective Projection
    perspectiveProjectionMatrix_Cube = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

-(void)resize_Sphere:(int)width :(int)height
{
    // Code
    // If height becomes 0 or less than 0 by accident, then assign height to 1
    if (height <= 0)
    {
        height = 1;
    }

    // Set the ViewPort
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // This line is analogous to glMatrixMode(GL_PROJECTION); from resize() in FFP
    // Set Perspective Projection
    perspectiveProjectionMatrix_Sphere = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

-(void)display
{
    // Code
    // Call Sphere related code
    if (fboResult == 0) // Which means initialize_Sphere() is executed successfully inside initialize()
    {
        [self display_Sphere];
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Call cube's resize to compensate the effect of sphere's resize
    [self resize:viewWidth :viewHeight];

    // Call ClearColor() of cube to compensate the effect of ClearColor() of sphere
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // Screen color is set to White for beautification (Not syntactically compulsory for FBO)

    // Clear OpenGL buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use Shader Program Object
    glUseProgram(shaderProgramObject_Cube);

    /*************************      CUBE    *************************/
    // Transformations
    // This line is analogous to glLoadIdentity() in display() for MODELVIEW matrix
    mat4 modelViewMatrix = mat4::identity();

    mat4 translationMatrix = mat4::identity();

    translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

    mat4 rotationMatrix = mat4::identity();

    mat4 rotationMatrixX = mat4::identity();
    rotationMatrixX = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);

    mat4 rotationMatrixY = mat4::identity();
    rotationMatrixY = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);

    mat4 rotationMatrixZ = mat4::identity();
    rotationMatrixZ = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);

    rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;

    modelViewMatrix = translationMatrix * rotationMatrix;   // Order is important (Matrix multiplication is non-commutative)

    // This line is analogous to glMatrixMode(GL_MODELVIEW); in display()
    mat4 modelViewProjectionMatrix = mat4::identity();

    modelViewProjectionMatrix = perspectiveProjectionMatrix_Cube * modelViewMatrix;  // Order is important

    // Send above matrix to the vertex shader in uniform
    glUniformMatrix4fv(mvpMatrixUniform_Cube, 1, GL_FALSE, modelViewProjectionMatrix);

    // For Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_FBO);
    glUniform1i(textureSamplerUniform_Cube, 0);

    // Bind with vao_cube
    glBindVertexArray(vao_cube);

    // Draw the vertex arrays
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
    glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

    // UnBind Texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // UnBind with vao_cube
    glBindVertexArray(0);

    // Unuse Shader Program Object
    glUseProgram(0);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

-(void)display_Sphere
{
    // Code
    // FBO related changes..
    // Bind FBO
    if (fbo)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    // Call sphere's resize to compensate the effect of cube's resize
    [self resize_Sphere:FBO_WIDTH :FBO_HEIGHT];

    // Call ClearColor() of sphere to compensate the effect of ClearColor() of cube
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Clear OpenGL buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use Shader Program Object
    glUseProgram(shaderProgramObject_Sphere);

    /*************************      SPHERE      *************************/
    // Transformations
    // This line is analogous to glLoadIdentity() in display() for MODELVIEW matrix
    mat4 modelMatrix = mat4::identity();

    mat4 viewMatrix = mat4::identity();

    mat4 translationMatrix = mat4::identity();

    // Translate Sphere backwards by Z
    translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

    modelMatrix = translationMatrix;

    // Send above matrix to the vertex shader in uniform
    glUniformMatrix4fv(modelMatrixUniform_Sphere, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform_Sphere, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(projectionMatrixUniform_Sphere, 1, GL_FALSE, perspectiveProjectionMatrix_Sphere);

    if (bLight_Sphere == YES)
    {
        glUniform3fv(laUniform_Sphere, 1, lightAmbient_Sphere);
        glUniform3fv(ldUniform_Sphere, 1, lightDiffuse_Sphere);
        glUniform3fv(lsUniform_Sphere, 1, lightSpecular_Sphere);
        glUniform4fv(lightPositionUniform_Sphere, 1, lightPosition_Sphere);

        glUniform3fv(kaUniform_Sphere, 1, materialAmbient_Sphere);
        glUniform3fv(kdUniform_Sphere, 1, materialDiffuse_Sphere);
        glUniform3fv(ksUniform_Sphere, 1, materialSpecular_Sphere);
        glUniform1f(materialShininessUniform_Sphere, materialShininess_Sphere);

        glUniform1i(lDoubleTapUniform_Sphere, 1);
    }
    else
    {
        glUniform1i(lDoubleTapUniform_Sphere, 0);
    }

    // Bind with vao_sphere
    glBindVertexArray(vao_sphere);

    // draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element_sphere);
    glDrawElements(GL_TRIANGLES, gNumElements_Sphere, GL_UNSIGNED_SHORT, 0);

    // UnBind with vao_sphere
    glBindVertexArray(0);

    // Unuse Shader Program Object
    glUseProgram(0);

    // UnBind FBO
    if (fbo)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

-(void)myUpdate
{
    // Code
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
    if (fboResult == 0)     // Which means initialize_Sphere() is executed successfully inside initialize()
    {
        [self uninitialize_Sphere];
    }

    if (texture_FBO)
    {
        glDeleteTextures(1, &texture_FBO);
        texture_FBO = 0;
    }

    // Free rbo
    if (rbo)
    {
        glDeleteRenderbuffers(1, &rbo);
        rbo = 0;
    }

    // Free fbo
    if (fbo)
    {
        glDeleteFramebuffers(1, &fbo);
        fbo = 0;
    }

    // Free vbo_texcoord_cube
    if (vbo_texcoord_cube)
    {
        glDeleteBuffers(1, &vbo_texcoord_cube);
        vbo_texcoord_cube = 0;
    }

    // Free vbo_position_cube
    if (vbo_position_cube)
    {
        glDeleteBuffers(1, &vbo_position_cube);
        vbo_position_cube = 0;
    }

    // Free vao_cube
    if (vao_cube)
    {
        glDeleteVertexArrays(1, &vao_cube);
        vao_cube = 0;
    }

    // Detach, Delete Shader Objects and Delete Shader Program Object
    // Step 1: Check whether Shader Program Object is still there
    if (shaderProgramObject_Cube)
    {
        glUseProgram(shaderProgramObject_Cube);
        GLint numShaders;

        // Step 2 : Get Number of Attached Shaders and Continue if Number Of Shaders is Greater than 0 
        glGetProgramiv(shaderProgramObject_Cube, GL_ATTACHED_SHADERS, &numShaders);
        if (numShaders > 0)
        {
            // Step 3 : Create a Buffer / Array to hold Attached Shader Object of Obtained Number of Shaders and Continue Only if malloc is Succeeded
            GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));
            if (pShaders != NULL)
            {
                // Step 4 : Get Shader Objects into this Buffer / Array
                glGetAttachedShaders(shaderProgramObject_Cube, numShaders, NULL, pShaders);
                
                // Step 5 : Iterate through Obtained Number of Shaders and inside this loop Detach and Delete Every Shader from the Buffer / Array
                for (GLint i = 0; i < numShaders; i++)
                {
                    glDetachShader(shaderProgramObject_Cube, pShaders[i]);
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
        glDeleteProgram(shaderProgramObject_Cube);
        shaderProgramObject_Cube = 0;
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

-(void)uninitialize_Sphere
{
    // Code
    // Free vbo_element_sphere
    if (vbo_element_sphere)
    {
        glDeleteBuffers(1, &vbo_element_sphere);
        vbo_element_sphere = 0;
    }

    // Free vbo_normal_sphere
    if (vbo_normal_sphere)
    {
        glDeleteBuffers(1, &vbo_normal_sphere);
        vbo_normal_sphere = 0;
    }

    // Free vbo_position_sphere
    if (vbo_position_sphere)
    {
        glDeleteBuffers(1, &vbo_position_sphere);
        vbo_position_sphere = 0;
    }

    // Free vao_sphere
    if (vao_sphere)
    {
        glDeleteVertexArrays(1, &vao_sphere);
        vao_sphere = 0;
    }

    // Detach, Delete Shader Objects and Delete Shader Program Object
    // Step 1: Check whether Shader Program Object is still there
    if (shaderProgramObject_Sphere)
    {
        glUseProgram(shaderProgramObject_Sphere);
        GLint numShaders;

        // Step 2 : Get Number of Attached Shaders and Continue if Number Of Shaders is Greater than 0 
        glGetProgramiv(shaderProgramObject_Sphere, GL_ATTACHED_SHADERS, &numShaders);
        if (numShaders > 0)
        {
            // Step 3 : Create a Buffer / Array to hold Attached Shader Object of Obtained Number of Shaders and Continue Only if malloc is Succeeded
            GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));
            if (pShaders != NULL)
            {
                // Step 4 : Get Shader Objects into this Buffer / Array
                glGetAttachedShaders(shaderProgramObject_Sphere, numShaders, NULL, pShaders);

                // Step 5 : Iterate through Obtained Number of Shaders and inside this loop Detach and Delete Every Shader from the Buffer / Array
                for (GLint i = 0; i < numShaders; i++)
                {
                    glDetachShader(shaderProgramObject_Sphere, pShaders[i]);
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
        glDeleteProgram(shaderProgramObject_Sphere);
        shaderProgramObject_Sphere = 0;
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
