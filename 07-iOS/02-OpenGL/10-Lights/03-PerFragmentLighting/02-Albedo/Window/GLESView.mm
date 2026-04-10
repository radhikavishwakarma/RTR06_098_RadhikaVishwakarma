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

    GLfloat materialAmbient[4];
    GLfloat materialDiffuse[4];
    GLfloat materialSpecular[4];
    GLfloat materialShininess;

    BOOL bLight;
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Lights Initialization
    lightAmbient[0] = 0.1f;
    lightAmbient[1] = 0.1f;
    lightAmbient[2] = 0.1f;
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

    materialAmbient[0] = 0.0f;
    materialAmbient[1] = 0.0f;
    materialAmbient[2] = 0.0f;
    materialAmbient[3] = 1.0f;

    materialDiffuse[0] = 0.5f;
    materialDiffuse[1] = 0.2f;
    materialDiffuse[2] = 0.7f;
    materialDiffuse[3] = 1.0f;

    materialSpecular[0] = 0.7f;
    materialSpecular[1] = 0.7f;
    materialSpecular[2] = 0.7f;
    materialSpecular[3] = 1.0f;

    materialShininess = 128.0f;

    bLight = NO;

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
    translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

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

    // Bind with vao_sphere
    glBindVertexArray(vao_sphere);

    // draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element_sphere);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    // UnBind with vao_sphere
    glBindVertexArray(0);

    // Unuse Shader Program Object
    glUseProgram(0);
}

-(void)myUpdate
{
    // Code
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
