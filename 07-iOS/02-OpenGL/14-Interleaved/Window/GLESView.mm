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
    AMC_ATTRIBUTE_NORMAL,
    AMC_ATTRIBUTE_TEXCOORD,
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

    GLuint vao_cube;            // Vertex Array Object
    GLuint vbo_cube;            // Vertex Buffer Object

    GLuint mvpMatrixUniform;

    mat4 perspectiveProjectionMatrix;

    // Uniform related to matrix
    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint projectionMatrixUniform;

    // Uniform related to lights
    GLuint laUniform;
    GLuint ldUniform;
    GLuint lsUniform;
    GLuint lightPositionUniform;

    // Uniform related to material
    GLuint kaUniform;
    GLuint kdUniform;
    GLuint ksUniform;
    GLuint materialShininessUniform;

    // Uniform related to DoubleTap
    GLuint lDoubleTapUniform;

    // Global Variables related to lights
    GLfloat lightAmbient[4];
    GLfloat lightDiffuse[4];
    GLfloat lightSpecular[4];
    GLfloat lightPosition[4];

    GLfloat materialAmbient[4];
    GLfloat materialDiffuse[4];
    GLfloat materialSpecular[4];
    GLfloat materialShininess;

    BOOL bLight;
    BOOL bAnimation;

    int iSingleTap;
    int iDoubleTap;

    // Texture related global variables
    GLuint texture_Marble;
    GLuint textureSamplerUniform;

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
    
    // Step 1: Bind Color Renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)[self layer]];

    // Step 2: Create Depth Renderbuffer
    GLint width;
    GLint height;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

    // Step 3: Check Whether Framebuffer is Complete or not
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("layoutSubviews(): Framebuffer Creation is Not Complete!\n");
        return;
    }

    // Step 4: Call resize()
    [self resize:width :height];

    // Step 5: Call drawView()
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
    if (bAnimation == YES)
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
    if (iSingleTap > 1)
    {
        iSingleTap = 0;
        bAnimation = NO;
    }
    else
    {
        bAnimation = YES;
    }
}

-(void)onDoubleTap:(UITapGestureRecognizer *)gestureRecognizer
{
    // Code
    iDoubleTap++;
    if(iDoubleTap > 1)
    {
        iDoubleTap = 0;
        bLight = NO;
    }
    else
    {
        bLight = YES;
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
        "precision highp int;\n" \
        "in vec4 aPosition;\n" \
        "in vec4 aColor;\n" \
        "in vec3 aNormal;\n" \
        "in vec2 aTexCoord;\n" \
        "uniform mat4 uModelMatrix;\n" \
        "uniform mat4 uViewMatrix;\n" \
        "uniform mat4 uProjectionMatrix;\n" \
        "uniform vec4 uLightPosition;\n" \
        "uniform int uDoubleTap;\n" \
        "out vec3 out_TransformedNormals;\n" \
        "out vec3 out_LightDirection;\n" \
        "out vec3 out_ViewerVector;\n" \
        "out vec4 out_Color;\n" \
        "out vec2 out_TexCoord;\n" \
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
            "out_Color = aColor;\n" \
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
        "precision highp float;\n" \
        "precision highp int;\n" \
        "in vec4 out_Color;\n" \
        "in vec2 out_TexCoord;\n" \
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
        "uniform sampler2D uTextureSampler;\n" \
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
                "vec4 phong_ads_LightFinal = vec4(phong_ads_Light, 1.0f);\n" \
                "FragColor = out_Color * phong_ads_LightFinal * texture(uTextureSampler, out_TexCoord);\n" \
            "}\n" \
            "else\n" \
            "{\n" \
                "FragColor = out_Color * texture(uTextureSampler, out_TexCoord);\n" \
            "}\n" \
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
    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_NORMAL, "aNormal");
    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");

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
    modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "uModelMatrix");
    viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "uViewMatrix");
    projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix");
    textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "uTextureSampler");

    laUniform = glGetUniformLocation(shaderProgramObject, "uLa");
    ldUniform = glGetUniformLocation(shaderProgramObject, "uLd");
    lsUniform = glGetUniformLocation(shaderProgramObject, "uLs");
    lightPositionUniform = glGetUniformLocation(shaderProgramObject, "uLightPosition");

    kaUniform = glGetUniformLocation(shaderProgramObject, "uKa");
    kdUniform = glGetUniformLocation(shaderProgramObject, "uKd");
    ksUniform = glGetUniformLocation(shaderProgramObject, "uKs");
    materialShininessUniform = glGetUniformLocation(shaderProgramObject, "uMaterialShininess");

    lDoubleTapUniform = glGetUniformLocation(shaderProgramObject, "uDoubleTap");

    /*************************      CUBE    *************************/
    // Provide Vertex position, Color, Normals, Texco-ords etc..
    const GLfloat cube_PCNT[] =
    {
        // Front
        // Position             // Color             // Normals             // Texco-ords
        1.0f,  1.0f,  1.0f,     1.0f, 0.0f, 0.0f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
       -1.0f,  1.0f,  1.0f,     1.0f, 0.0f, 0.0f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
       -1.0f, -1.0f,  1.0f,     1.0f, 0.0f, 0.0f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
        1.0f, -1.0f,  1.0f,     1.0f, 0.0f, 0.0f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
        
        // Right             
        // Position             // Color             // Normals             // Texco-ords
        1.0f,  1.0f, -1.0f,     0.0f, 0.0f, 1.0f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
        1.0f,  1.0f,  1.0f,     0.0f, 0.0f, 1.0f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        1.0f, -1.0f,  1.0f,     0.0f, 0.0f, 1.0f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,     0.0f, 0.0f, 1.0f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
        
        // Back              
        // Position             // Color             // Normals             // Texco-ords
        1.0f,  1.0f, -1.0f,     1.0f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
       -1.0f,  1.0f, -1.0f,     1.0f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
       -1.0f, -1.0f, -1.0f,     1.0f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,     1.0f, 1.0f, 0.0f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
        
        // Left              
        // Position             // Color             // Normals             // Texco-ords
        -1.0f,  1.0f,  1.0f,    1.0f, 0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,    1.0f, 0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,    1.0f, 0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
        
        // Top               
        // Position             // Color             // Normals             // Texco-ords
        1.0f,  1.0f, -1.0f,     0.0f, 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
       -1.0f,  1.0f, -1.0f,     0.0f, 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
       -1.0f,  1.0f,  1.0f,     0.0f, 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
        1.0f,  1.0f,  1.0f,     0.0f, 1.0f, 0.0f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
        
        // Bottom            
        // Position             // Color             // Normals             // texcoords
        1.0f, -1.0f,  1.0f,     1.0f, 0.5f, 0.0f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
       -1.0f, -1.0f,  1.0f,     1.0f, 0.5f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
       -1.0f, -1.0f, -1.0f,     1.0f, 0.5f, 0.0f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,     1.0f, 0.5f, 0.0f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
    };

    /*************************      CUBE    *************************/
    // Vertex array object for arrays of vertex attributes
    glGenVertexArrays(1, &vao_cube);

    glBindVertexArray(vao_cube);

    // Create a common vbo for Position, Color, Normals and Texco-ords
    glGenBuffers(1, &vbo_cube);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);

    // For Android and WebGL, size for ByteBuffer is 24 * 11 * 4 (i.e.sizeof(float))
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_PCNT), cube_PCNT, GL_STATIC_DRAW);

    // For Position
    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

    // For Color
    glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);

    // For Normals
    glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);

    // For Texco-ords
    glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // Depth related function calls..
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // From here onwards, OpenGL code starts...
    // Tell OpenGL to choose the color to clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Lights Initialization
    lightAmbient[0] = 0.0f;
    lightAmbient[1] = 0.0f;
    lightAmbient[2] = 0.0f;
    lightAmbient[3] = 1.0f;

    lightDiffuse[0] = 1.0f;
    lightDiffuse[1] = 1.0f;
    lightDiffuse[2] = 1.0f;
    lightDiffuse[3] = 1.0f;

    lightSpecular[0] = 1.0f;
    lightSpecular[1] = 1.0f;
    lightSpecular[2] = 1.0f;
    lightSpecular[3] = 1.0f;

    lightPosition[0] = 100.0f;
    lightPosition[1] = 100.0f;
    lightPosition[2] = 100.0f;
    lightPosition[3] = 1.0f;

    materialAmbient[0] = 0.25f;
    materialAmbient[1] = 0.25f;
    materialAmbient[2] = 0.25f;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 1.0f;
    materialDiffuse[1] = 1.0f;
    materialDiffuse[2] = 1.0f;
    materialDiffuse[3] = 1.0f;

    materialSpecular[0] = 1.0f;
    materialSpecular[1] = 1.0f;
    materialSpecular[2] = 1.0f;
    materialSpecular[3] = 1.0f;

    materialShininess = 128.0f;

    bLight = NO;
    bAnimation = NO;

    iSingleTap = 0;
    iDoubleTap = 0;

    // Load Marble texture
    texture_Marble = [self loadGLTexture:@"Marble" :@"bmp"];
    if (texture_Marble == 0)
    {
        printf("loadGLTexture() for Marble failed!\n");
        
        [self uninitialize];
        [self release];
        exit(0);
    }

    // Enable texturing
    glEnable(GL_TEXTURE_2D);

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

-(GLuint)loadGLTexture:(NSString *)textureFileName :(NSString *)extension
{
    // Code
    // Step 1: Create Bundle Object
    NSBundle *appBundle = [NSBundle mainBundle];

    // Step 2: Get Full textureFileNameWithPath as NSString
    NSString *textureFileNameWithPath = [appBundle pathForResource:textureFileName ofType:extension];

    // Steps related to Texture Code
    // Step 1: Get UIImage of our Texture File
    UIImage *uiImage = [[UIImage alloc]initWithContentsOfFile:textureFileNameWithPath];

    // Error Checking and return(0) if Error
    if(uiImage == nil)
    {
        printf("UIImage Creation Failed!\n");
        return(0);
    }

    // Step 2: Convert UIImage to CGImage
    CGImageRef cgImage = [uiImage CGImage];

    // Step 3: Get Width of the Image
    int imageWidth = (int)CGImageGetWidth(cgImage);

    // Step 4: Get Height of the Image
    int imageHeight = (int)CGImageGetHeight(cgImage);

    // Step 5: Get Image data of the image
    // Step A: Get CGDataProvider
    CGDataProviderRef cgDataProvider = CGImageGetDataProvider(cgImage);

    // Step B: Get CFData Representation of CGData
    CFDataRef cfData = CGDataProviderCopyData(cgDataProvider);

    // Step C: Get CFData in the form of bytes
    void *imageData = (void *)CFDataGetBytePtr(cfData);

    // Usual Texture Code Common in each Platform/Operating System
    GLuint texture;

    // Step 6: Generate OpenGL texture object
    glGenTextures(1, &texture);

    // Step 7: Bind to the newly created empty structure object
    glBindTexture(GL_TEXTURE_2D, texture);

    // Step 8: Unpack the image into memory for faster loading
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Step 9: Release Core Foundation Data i.e cfData
    CFRelease(cfData);

    return(texture);
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

    /*************************      CUBE    *************************/
    // Transformations
    // This line is analogous to glLoadIdentity() in display() for MODELVIEW matrix
    mat4 modelMatrix = mat4::identity();

    mat4 viewMatrix = mat4::identity();

    mat4 translationMatrix = mat4::identity();

    // Translate Cube backwards by Z
    translationMatrix = vmath::translate(0.0f, 0.0f, -7.5f);

    mat4 rotationMatrix = mat4::identity();

    mat4 rotationMatrixX = mat4::identity();
    rotationMatrixX = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);

    mat4 rotationMatrixY = mat4::identity();
    rotationMatrixY = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);

    mat4 rotationMatrixZ = mat4::identity();
    rotationMatrixZ = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);

    rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;

    modelMatrix = translationMatrix * rotationMatrix;   // Order is important (Matrix multiplication is non-commutative)

    // Send above matrix to the vertex shader in uniform
    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    // For Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_Marble);
    glUniform1i(textureSamplerUniform, 0);

    if (bLight == YES)
    {
        glUniform3fv(laUniform, 1, lightAmbient);
        glUniform3fv(ldUniform, 1, lightDiffuse);
        glUniform3fv(lsUniform, 1, lightSpecular);
        glUniform4fv(lightPositionUniform, 1, lightPosition);

        glUniform3fv(kaUniform, 1, materialAmbient);
        glUniform3fv(kdUniform, 1, materialDiffuse);
        glUniform3fv(ksUniform, 1, materialSpecular);
        glUniform1f(materialShininessUniform, materialShininess);

        glUniform1i(lDoubleTapUniform, 1);
    }
    else
    {
        glUniform1i(lDoubleTapUniform, 0);
    }

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
    // Free texture_Marble
    if (texture_Marble)
    {
        glDeleteTextures(1, &texture_Marble);
        texture_Marble = 0;
    }

    // Free vbo_cube
    if (vbo_cube)
    {
        glDeleteBuffers(1, &vbo_cube);
        vbo_cube = 0;
    }

    // Free vao_cube
    if (vao_cube)
    {
        glDeleteVertexArrays(1, &vao_cube);
        vao_cube = 0;
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
