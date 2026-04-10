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

// Variabls for coping height and width in another variable for viewport
int widthX = 0, heightY = 0;

enum
{
    AMC_ATTRIBUTE_POSITION = 0,
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
    GLuint shaderProgramObject;

    GLuint vao_sphere;          // Vertex Array Object
    GLuint vbo_position_sphere; // Vertex Buffer Object
    GLuint vbo_normal_sphere;
    GLuint vbo_element_sphere;

    mat4 perspectiveProjectionMatrix;

    // Global variables related to Sphere
    GLuint gNumVertices;
    GLuint gNumElements;

    GLfloat sphere_vertices[1146];
    GLfloat sphere_normals[1146];
    GLfloat sphere_textures[764];
    unsigned short sphere_elements[2280];

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

    BOOL bLight;

    int iSingleTap;
    int iDoubleTap;

    // Global Variables related to Rotation Angles
    GLfloat angleForXRotation;
    GLfloat angleForYRotation;
    GLfloat angleForZRotation;
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

    widthX = width;
    heightY = height;

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
        iSingleTap = 1;
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
    shaderProgramObject = glCreateProgram();

    glAttachShader(shaderProgramObject, vertexShaderObject);
    glAttachShader(shaderProgramObject, fragmentShaderObject);

    // Bind Shader Attributes at a certain index in Shader to same index in Host Program
    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");
    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_NORMAL, "aNormal");

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
    
    laUniform = glGetUniformLocation(shaderProgramObject, "uLa");
    ldUniform = glGetUniformLocation(shaderProgramObject, "uLd");
    lsUniform = glGetUniformLocation(shaderProgramObject, "uLs");
    lightPositionUniform = glGetUniformLocation(shaderProgramObject, "uLightPosition");
    
    kaUniform = glGetUniformLocation(shaderProgramObject, "uKa");
    kdUniform = glGetUniformLocation(shaderProgramObject, "uKd");
    ksUniform = glGetUniformLocation(shaderProgramObject, "uKs");
    materialShininessUniform = glGetUniformLocation(shaderProgramObject, "uMaterialShininess");
    
    lDoubleTapUniform = glGetUniformLocation(shaderProgramObject, "uDoubleTap");

    /*************************      SPHERE      *************************/
    // Provide Vertex position, Color, Normals, Texco-ords etc..
    Sphere *sphere = [[Sphere alloc]init];
    [sphere getSphereVertexData:sphere_vertices :sphere_normals :sphere_textures :sphere_elements];
    
    gNumVertices = [sphere getNumberOfSphereVertices];
    gNumElements = [sphere getNumberOfSphereElements];

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Unbind vao
    glBindVertexArray(0);

    // Depth related function calls..
    glClearDepthf(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // From here onwards, OpenGL code starts...
    // Tell OpenGL to choose the color to clear the screen
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

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

    lightPosition[0] = 0.0f;
    lightPosition[1] = 0.0f;
    lightPosition[2] = 0.0f;
    lightPosition[3] = 1.0f;

    bLight = NO;

    iSingleTap = 0;
    iDoubleTap = 0;

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

    /*************************      SPHERE      *************************/
    // Transformations
    // This line is analogous to glLoadIdentity() in display() for MODELVIEW matrix
    mat4 modelMatrix = mat4::identity();

    mat4 viewMatrix = mat4::identity();

    mat4 translationMatrix = mat4::identity();

    // Translate Sphere backwards by Z
    translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);

    modelMatrix = translationMatrix;

    // Send above matrix to the vertex shader in uniform
    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

    if (bLight == YES)
    {
        glUniform3fv(laUniform, 1, lightAmbient);
        glUniform3fv(ldUniform, 1, lightDiffuse);
        glUniform3fv(lsUniform, 1, lightSpecular);

        glUniform1i(lDoubleTapUniform, 1);

        if (iSingleTap == 1)
        {
            glUniform4f(lightPositionUniform, 0.0f, sinf(angleForXRotation) * 3, cosf(angleForXRotation) * 3, 1.0f);
        }

        if (iSingleTap == 2)
        {
            glUniform4f(lightPositionUniform, sinf(angleForYRotation) * 3, 0.0f, cosf(angleForYRotation) * 3, 1.0f);
        }

        if (iSingleTap == 3)
        {
            glUniform4f(lightPositionUniform, sinf(angleForZRotation) * 3, cosf(angleForZRotation) * 3, 0.0f, 1.0f);
        }
    }
    else
    {
        glUniform1i(lDoubleTapUniform, 0);
    }

    // Draw 24 Spheres
    // variable declarations
    GLfloat materialAmbient[96];
    GLfloat materialDiffuse[96];
    GLfloat materialSpecular[96];
    GLfloat materialShininess[24];

    // code
    // ***** 1st Sphere of 1st Column - EMERALD *****
    // Ambient material
    materialAmbient[0] = 0.0215f;   // r
    materialAmbient[1] = 0.1745f;   // g
    materialAmbient[2] = 0.0215f;   // b
    materialAmbient[3] = 1.0f;      // a

    // Diffuse material
    materialDiffuse[0] = 0.07568f;  // r
    materialDiffuse[1] = 0.61424f;  // g
    materialDiffuse[2] = 0.07568f;  // b
    materialDiffuse[3] = 1.0f;      // a

    // Specular material
    materialSpecular[0] = 0.633f;       // r
    materialSpecular[1] = 0.727811f;    // g
    materialSpecular[2] = 0.633f;       // b
    materialSpecular[3] = 1.0f;         // a

    // Shininess
    materialShininess[0] = 0.6f * 128.0f;
    // *******************************************************

    // ***** 2nd Sphere of 1st Column - JADE *****
    // Ambient material
    materialAmbient[4] = 0.135f;    // r
    materialAmbient[5] = 0.2225f;   // g
    materialAmbient[6] = 0.1575f;   // b
    materialAmbient[7] = 1.0f;      // a

    // Diffuse material
    materialDiffuse[4] = 0.54f; // r
    materialDiffuse[5] = 0.89f; // g
    materialDiffuse[6] = 0.63f; // b
    materialDiffuse[7] = 1.0f;  // a

    // Specular material
    materialSpecular[4] = 0.316228f;    // r
    materialSpecular[5] = 0.316228f;    // g
    materialSpecular[6] = 0.316228f;    // b
    materialSpecular[7] = 1.0f;         // a

    // Shininess
    materialShininess[1] = 0.1f * 128.0f;
    // *******************************************************

    // ***** 3rd sphere of 1st column - OBSIDIAN *****
    // Ambient material
    materialAmbient[8] = 0.05375f;  // r
    materialAmbient[9] = 0.05f;     // g
    materialAmbient[10] = 0.06625f; // b
    materialAmbient[11] = 1.0f;     // a

    // Diffuse material
    materialDiffuse[8] = 0.18275f;  // r
    materialDiffuse[9] = 0.17f;     // g
    materialDiffuse[10] = 0.22525f; // b
    materialDiffuse[11] = 1.0f;     // a

    // Specular material
    materialSpecular[8] = 0.332741f;    // r
    materialSpecular[9] = 0.328634f;    // g
    materialSpecular[10] = 0.346435f;   // b
    materialSpecular[11] = 1.0f;        // a

    // Shininess
    materialShininess[2] = 0.3f * 128.0f;
    // *******************************************************

    // ***** 4th sphere of 1st column - PEARL *****
    // Ambient material
    materialAmbient[12] = 0.25f;    // r
    materialAmbient[13] = 0.20725f; // g
    materialAmbient[14] = 0.20725f; // b
    materialAmbient[15] = 1.0f;     // a

    // Diffuse material
    materialDiffuse[12] = 1.0f;     // r
    materialDiffuse[13] = 0.829f;   // g
    materialDiffuse[14] = 0.829f;   // b
    materialDiffuse[15] = 1.0f;     // a

    // Specular material
    materialSpecular[12] = 0.296648f;   // r
    materialSpecular[13] = 0.296648f;   // g
    materialSpecular[14] = 0.296648f;   // b
    materialSpecular[15] = 1.0f;        // a

    // Shininess
    materialShininess[3] = 0.088f * 128.0f;
    // *******************************************************

    // ***** 5th sphere on 1st column - RUBY *****
    // Ambient material
    materialAmbient[16] = 0.1745f;  // r
    materialAmbient[17] = 0.01175f; // g
    materialAmbient[18] = 0.01175f; // b
    materialAmbient[19] = 1.0f;     // a

    // Diffuse material
    materialDiffuse[16] = 0.61424f; // r
    materialDiffuse[17] = 0.04136f; // g
    materialDiffuse[18] = 0.04136f; // b
    materialDiffuse[19] = 1.0f;     // a

    // Specular material
    materialSpecular[16] = 0.727811f; // r
    materialSpecular[17] = 0.626959f; // g
    materialSpecular[18] = 0.626959f; // b
    materialSpecular[19] = 1.0f;      // a

    // Shininess
    materialShininess[4] = 0.6f * 128.0f;
    // *******************************************************

    // ***** 6th sphere on 1st column - TURQUOISE *****
    // Ambient material
    materialAmbient[20] = 0.1f;     // r
    materialAmbient[21] = 0.18725f; // g
    materialAmbient[22] = 0.1745f;  // b
    materialAmbient[23] = 1.0f;     // a

    // Diffuse material
    materialDiffuse[20] = 0.396f;   // r
    materialDiffuse[21] = 0.74151f; // g
    materialDiffuse[22] = 0.69102f; // b
    materialDiffuse[23] = 1.0f;     // a

    // Specular material
    materialSpecular[20] = 0.297254f; // r
    materialSpecular[21] = 0.30829f;  // g
    materialSpecular[22] = 0.306678f; // b
    materialSpecular[23] = 1.0f;      // a

    // Shininess
    materialShininess[5] = 0.1f * 128.0f;
    // *******************************************************
    // *******************************************************
    // *******************************************************

    // ***** 1st sphere on 2nd column - BRASS *****
    // Ambient material
    materialAmbient[24] = 0.329412f; // r
    materialAmbient[25] = 0.223529f; // g
    materialAmbient[26] = 0.027451f; // b
    materialAmbient[27] = 1.0f;      // a

    // Diffuse material
    materialDiffuse[24] = 0.780392f; // r
    materialDiffuse[25] = 0.568627f; // g
    materialDiffuse[26] = 0.113725f; // b
    materialDiffuse[27] = 1.0f;      // a

    // Specular material
    materialSpecular[24] = 0.992157f; // r
    materialSpecular[25] = 0.941176f; // g
    materialSpecular[26] = 0.807843f; // b
    materialSpecular[27] = 1.0f;      // a

    // Shininess
    materialShininess[6] = 0.21794872f * 128.0f;
    // *******************************************************

    // ***** 2nd sphere on 2nd column - BRONZE *****
    // Ambient material
    materialAmbient[28] = 0.2125f; // r
    materialAmbient[29] = 0.1275f; // g
    materialAmbient[30] = 0.054f;  // b
    materialAmbient[31] = 1.0f;    // a

    // Diffuse material
    materialDiffuse[28] = 0.714f;   // r
    materialDiffuse[29] = 0.4284f;  // g
    materialDiffuse[30] = 0.18144f; // b
    materialDiffuse[31] = 1.0f;     // a

    // Specular material
    materialSpecular[28] = 0.393548f; // r
    materialSpecular[28] = 0.271906f; // g
    materialSpecular[30] = 0.166721f; // b
    materialSpecular[31] = 1.0f;      // a

    // Shininess
    materialShininess[7] = 0.2f * 128.0f;
    // *******************************************************

    // ***** 3rd sphere on 2nd column - CHROME *****
    // Ambient material
    materialAmbient[32] = 0.25f;    // r
    materialAmbient[33] = 0.25f;    // g
    materialAmbient[34] = 0.25f;    // b
    materialAmbient[35] = 1.0f;     // a

    // Diffuse material
    materialDiffuse[32] = 0.4f; // r
    materialDiffuse[33] = 0.4f; // g
    materialDiffuse[34] = 0.4f; // b
    materialDiffuse[35] = 1.0f; // a

    // Specular material
    materialSpecular[32] = 0.774597f;   // r
    materialSpecular[33] = 0.774597f;   // g
    materialSpecular[34] = 0.774597f;   // b
    materialSpecular[35] = 1.0f;        // a

    // Shininess
    materialShininess[8] = 0.6f * 128.0f;
    // *******************************************************

    // ***** 4th sphere on 2nd column - COPPER *****
    // Ambient material
    materialAmbient[36] = 0.19125f; // r
    materialAmbient[37] = 0.0735f;  // g
    materialAmbient[38] = 0.0225f;  // b
    materialAmbient[39] = 1.0f;     // a

    // Diffuse material
    materialDiffuse[36] = 0.7038f;  // r
    materialDiffuse[37] = 0.27048f; // g
    materialDiffuse[38] = 0.0828f;  // b
    materialDiffuse[39] = 1.0f;     // a

    // Specular material
    materialSpecular[36] = 0.256777f; // r
    materialSpecular[37] = 0.137622f; // g
    materialSpecular[38] = 0.086014f; // b
    materialSpecular[39] = 1.0f;      // a

    // Shininess
    materialShininess[9] = 0.1f * 128.0f;
    // *******************************************************

    // ***** 5th sphere on 2nd column - GOLD *****
    // Ambient material
    materialAmbient[40] = 0.24725f; // r
    materialAmbient[41] = 0.1995f;  // g
    materialAmbient[42] = 0.0745f;  // b
    materialAmbient[43] = 1.0f;     // a

    // Diffuse material
    materialDiffuse[40] = 0.75164f; // r
    materialDiffuse[41] = 0.60648f; // g
    materialDiffuse[42] = 0.22648f; // b
    materialDiffuse[43] = 1.0f;     // a

    // Specular material
    materialSpecular[40] = 0.628281f; // r
    materialSpecular[41] = 0.555802f; // g
    materialSpecular[42] = 0.366065f; // b
    materialSpecular[43] = 1.0f;      // a

    // Shininess
    materialShininess[10] = 0.4f * 128.0f;
    // *******************************************************

    // ***** 6th sphere on 2nd column - SILVER *****
    // Ambient material
    materialAmbient[44] = 0.19225f; // r
    materialAmbient[45] = 0.19225f; // g
    materialAmbient[46] = 0.19225f; // b
    materialAmbient[47] = 1.0f;     // a

    // Diffuse material
    materialDiffuse[44] = 0.50754f; // r
    materialDiffuse[45] = 0.50754f; // g
    materialDiffuse[46] = 0.50754f; // b
    materialDiffuse[47] = 1.0f;     // a

    // Specular material
    materialSpecular[44] = 0.508273f; // r
    materialSpecular[45] = 0.508273f; // g
    materialSpecular[46] = 0.508273f; // b
    materialSpecular[47] = 1.0f;      // a

    // Shininess
    materialShininess[11] = 0.4f * 128.0f;
    // *******************************************************
    // *******************************************************
    // *******************************************************

    // ***** 1st sphere on 3rd column - BLACK *****
    // Ambient material
    materialAmbient[48] = 0.0f;  // r
    materialAmbient[49] = 0.0f;  // g
    materialAmbient[50] = 0.0f;  // b
    materialAmbient[51] = 1.0f;  // a

    // Diffuse material
    materialDiffuse[48] = 0.01f; // r
    materialDiffuse[49] = 0.01f; // g
    materialDiffuse[50] = 0.01f; // b
    materialDiffuse[51] = 1.0f;  // a

    // Specular material
    materialSpecular[48] = 0.50f; // r
    materialSpecular[49] = 0.50f; // g
    materialSpecular[50] = 0.50f; // b
    materialSpecular[51] = 1.0f;  // a

    // Shininess
    materialShininess[12] = 0.25f * 128.0f;
    // *******************************************************

    // ***** 2nd sphere on 3rd column - CYAN *****
    // Ambient material
    materialAmbient[52] = 0.0f;  // r
    materialAmbient[53] = 0.1f;  // g
    materialAmbient[54] = 0.06f; // b
    materialAmbient[55] = 1.0f;  // a

    // Diffuse material
    materialDiffuse[52] = 0.0f;        // r
    materialDiffuse[53] = 0.50980392f; // g
    materialDiffuse[54] = 0.50980392f; // b
    materialDiffuse[55] = 1.0f;        // a

    // Specular material
    materialSpecular[52] = 0.50196078f; // r
    materialSpecular[53] = 0.50196078f; // g
    materialSpecular[54] = 0.50196078f; // b
    materialSpecular[55] = 1.0f;        // a

    // Shininess
    materialShininess[13] = 0.25f * 128.0f;
    // *******************************************************

    // ***** 3rd sphere on 2nd column - GREEN *****
    // Ambient material
    materialAmbient[56] = 0.0f;  // r
    materialAmbient[57] = 0.0f;  // g
    materialAmbient[58] = 0.0f;  // b
    materialAmbient[59] = 1.0f;  // a

    // Diffuse material
    materialDiffuse[56] = 0.1f;   // r
    materialDiffuse[57] = 0.35f;  // g
    materialDiffuse[58] = 0.1f;   // b
    materialDiffuse[59] = 1.0f;   // a

    // Specular material
    materialSpecular[56] = 0.45f; // r
    materialSpecular[57] = 0.55f; // g
    materialSpecular[58] = 0.45f; // b
    materialSpecular[59] = 1.0f;  // a

    // Shininess
    materialShininess[14] = 0.25f * 128.0f;
    // *******************************************************

    // ***** 4th sphere on 3rd column - RED *****
    // Ambient material
    materialAmbient[60] = 0.0f;  // r
    materialAmbient[61] = 0.0f;  // g
    materialAmbient[62] = 0.0f;  // b
    materialAmbient[63] = 1.0f;  // a

    // Diffuse material
    materialDiffuse[60] = 0.5f;  // r
    materialDiffuse[61] = 0.0f;  // g
    materialDiffuse[62] = 0.0f;  // b
    materialDiffuse[63] = 1.0f;  // a

    // Specular material
    materialSpecular[60] = 0.7f;  // r
    materialSpecular[61] = 0.6f;  // g
    materialSpecular[62] = 0.6f;  // b
    materialSpecular[63] = 1.0f;  // a

    // Shininess
    materialShininess[15] = 0.25f * 128.0f;
    // *******************************************************

    // ***** 5th sphere on 3rd column - WHITE *****
    // Ambient material
    materialAmbient[64] = 0.0f;  // r
    materialAmbient[65] = 0.0f;  // g
    materialAmbient[66] = 0.0f;  // b
    materialAmbient[67] = 1.0f;  // a

    // Diffuse material
    materialDiffuse[64] = 0.55f; // r
    materialDiffuse[65] = 0.55f; // g
    materialDiffuse[66] = 0.55f; // b
    materialDiffuse[67] = 1.0f;  // a

    // Specular material
    materialSpecular[64] = 0.70f; // r
    materialSpecular[65] = 0.70f; // g
    materialSpecular[66] = 0.70f; // b
    materialSpecular[67] = 1.0f;  // a

    // Shininess
    materialShininess[16] = 0.25f * 128.0f;
    // *******************************************************

    // ***** 6th sphere on 3rd column - YELLOW PLASTIC *****
    // Ambient material
    materialAmbient[68] = 0.0f;  // r
    materialAmbient[69] = 0.0f;  // g
    materialAmbient[70] = 0.0f;  // b
    materialAmbient[71] = 1.0f;  // a

    // Diffuse material
    materialDiffuse[68] = 0.5f;  // r
    materialDiffuse[69] = 0.5f;  // g
    materialDiffuse[70] = 0.0f;  // b
    materialDiffuse[71] = 1.0f;  // a

    // Specular material
    materialSpecular[68] = 0.60f; // r
    materialSpecular[69] = 0.60f; // g
    materialSpecular[70] = 0.50f; // b
    materialSpecular[71] = 1.0f;  // a

    // Shininess
    materialShininess[17] = 0.25f * 128.0f;
    // *******************************************************
    // *******************************************************
    // *******************************************************

    // ***** 1st sphere on 4th column - BLACK *****
    // Ambient material
    materialAmbient[72] = 0.02f; // r
    materialAmbient[73] = 0.02f; // g
    materialAmbient[74] = 0.02f; // b
    materialAmbient[75] = 1.0f;  // a

    // Diffuse material
    materialDiffuse[72] = 0.01f; // r
    materialDiffuse[73] = 0.01f; // g
    materialDiffuse[74] = 0.01f; // b
    materialDiffuse[75] = 1.0f;  // a

    // Specular material
    materialSpecular[72] = 0.4f;  // r
    materialSpecular[73] = 0.4f;  // g
    materialSpecular[74] = 0.4f;  // b
    materialSpecular[75] = 1.0f;  // a

    // Shininess
    materialShininess[18] = 0.078125f * 128.0f;
    // *******************************************************

    // ***** 2nd sphere on 4th column - CYAN *****
    // Ambient material
    materialAmbient[76] = 0.0f;  // r
    materialAmbient[77] = 0.05f; // g
    materialAmbient[78] = 0.05f; // b
    materialAmbient[79] = 1.0f;  // a

    // Diffuse material
    materialDiffuse[76] = 0.4f;  // r
    materialDiffuse[77] = 0.5f;  // g
    materialDiffuse[78] = 0.5f;  // b
    materialDiffuse[79] = 1.0f;  // a

    // Specular material
    materialSpecular[76] = 0.04f; // r
    materialSpecular[77] = 0.7f;  // g
    materialSpecular[78] = 0.7f;  // b
    materialSpecular[79] = 1.0f;  // a

    // Shininess
    materialShininess[19] = 0.078125f * 128.0f;
    // *******************************************************

    // ***** 3rd sphere on 4th column - GREEN *****
    // Ambient material
    materialAmbient[80] = 0.0f;  // r
    materialAmbient[81] = 0.05f; // g
    materialAmbient[82] = 0.0f;  // b
    materialAmbient[83] = 1.0f;  // a

    // Diffuse material
    materialDiffuse[80] = 0.4f;  // r
    materialDiffuse[81] = 0.5f;  // g
    materialDiffuse[82] = 0.4f;  // b
    materialDiffuse[83] = 1.0f;  // a

    // Specular material
    materialSpecular[80] = 0.04f; // r
    materialSpecular[81] = 0.7f;  // g
    materialSpecular[82] = 0.04f; // b
    materialSpecular[83] = 1.0f;  // a

    // Shininess
    materialShininess[20] = 0.078125f * 128.0f;
    // *******************************************************

    // ***** 4th sphere on 4th column - RED *****
    // Ambient material
    materialAmbient[84] = 0.05f; // r
    materialAmbient[85] = 0.0f;  // g
    materialAmbient[86] = 0.0f;  // b
    materialAmbient[87] = 1.0f;  // a

    // Diffuse material
    materialDiffuse[84] = 0.5f;  // r
    materialDiffuse[85] = 0.4f;  // g
    materialDiffuse[86] = 0.4f;  // b
    materialDiffuse[87] = 1.0f;  // a

    // Specular material
    materialSpecular[84] = 0.7f;  // r
    materialSpecular[85] = 0.04f; // g
    materialSpecular[86] = 0.04f; // b
    materialSpecular[87] = 1.0f;  // a

    // Shininess
    materialShininess[21] = 0.078125f * 128.0f;
    // *******************************************************

    // ***** 5th sphere on 4th column - WHITE *****
    // Ambient material
    materialAmbient[88] = 0.05f; // r
    materialAmbient[89] = 0.05f; // g
    materialAmbient[90] = 0.05f; // b
    materialAmbient[91] = 1.0f;  // a

    // Diffuse material
    materialDiffuse[88] = 0.5f;  // r
    materialDiffuse[89] = 0.5f;  // g
    materialDiffuse[90] = 0.5f;  // b
    materialDiffuse[91] = 1.0f;  // a

    // Specular material
    materialSpecular[88] = 0.7f;  // r
    materialSpecular[89] = 0.7f;  // g
    materialSpecular[90] = 0.7f;  // b
    materialSpecular[91] = 1.0f;  // a

    // Shininess
    materialShininess[22] = 0.078125f * 128.0f;
    // *******************************************************

    // ***** 6th sphere on 4th column - YELLOW RUBBER *****
    // Ambient material
    materialAmbient[92] = 0.05f; // r
    materialAmbient[93] = 0.05f; // g
    materialAmbient[94] = 0.0f;  // b
    materialAmbient[95] = 1.0f;  // a

    // Diffuse material
    materialDiffuse[92] = 0.5f;  // r
    materialDiffuse[93] = 0.5f;  // g
    materialDiffuse[94] = 0.4f;  // b
    materialDiffuse[95] = 1.0f;  // a

    // Specular material
    materialSpecular[92] = 0.7f;  // r
    materialSpecular[93] = 0.7f;  // g
    materialSpecular[94] = 0.04f; // b
    materialSpecular[95] = 1.0f;  // a

    // Shininess
    materialShininess[23] = 0.078125f * 128.0f;
    // *******************************************************
    // *******************************************************
    // *******************************************************

    // Bind with vao_sphere
    glBindVertexArray(vao_sphere);

    // draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element_sphere);

    int width = (GLuint)(widthX / 6);
    int height = (GLuint)(heightY / 4);

    int index = 0;

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            glUniform3f(kaUniform, materialAmbient[index * 4 + 0], materialAmbient[index * 4 + 1], materialAmbient[index * 4 + 2]);

            glUniform3f(kdUniform, materialDiffuse[index * 4 + 0], materialDiffuse[index * 4 + 1], materialDiffuse[index * 4 + 2]);

            glUniform3f(ksUniform, materialSpecular[index * 4 + 0], materialSpecular[index * 4 + 1], materialSpecular[index * 4 + 2]);

            glUniform1f(materialShininessUniform, materialShininess[index]);

            glViewport(width * i, height * (4 - 1 - j), width, height);

            perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

            glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

            index++;
        }
    }

    // UnBind with vao_sphere
    glBindVertexArray(0);

    // Unuse Shader Program Object
    glUseProgram(0);
}

-(void)myUpdate
{
    // Code
    // Update angle for X-Rotation
    angleForXRotation = angleForXRotation + 0.05f;
    if (angleForXRotation >= 360.0f)
    {
        angleForXRotation = angleForXRotation - 360.0f;
    }

    // Update angle for Y-Rotation
    angleForYRotation = angleForYRotation + 0.05f;
    if (angleForYRotation >= 360.0f)
    {
        angleForYRotation = angleForYRotation - 360.0f;
    }

    // Update angle for Z-Rotation
    angleForZRotation = angleForZRotation + 0.05f;
    if (angleForZRotation >= 360.0f)
    {
        angleForZRotation = angleForZRotation - 360.0f;
    }
}

-(void)uninitialize
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
