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
    GLuint shaderProgramObject_PerVertex;
    GLuint shaderProgramObject_PerFragment;

    GLuint vao_sphere;              // Vertex Array Object
    GLuint vbo_position_sphere;     // Vertex Buffer Object
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

    // Uniforms related to Per Vertex Light
    // Uniform related to matrix
    GLuint modelMatrixUniform_perVertex;
    GLuint viewMatrixUniform_perVertex;
    GLuint projectionMatrixUniform_perVertex;

    // Uniform related to lights
    GLuint laUniform_perVertex[3];
    GLuint ldUniform_perVertex[3];
    GLuint lsUniform_perVertex[3];
    GLuint lightPositionUniform_perVertex[3];

    // Uniform related to material
    GLuint kaUniform_perVertex;
    GLuint kdUniform_perVertex;
    GLuint ksUniform_perVertex;
    GLuint materialShininessUniform_perVertex;

    // Uniform related to DoubleTap
    GLuint lDoubleTapUniform_perVertex;

    // Uniforms related to Per Fragment Light
    // Uniform related to matrix
    GLuint modelMatrixUniform_perFragment;
    GLuint viewMatrixUniform_perFragment;
    GLuint projectionMatrixUniform_perFragment;

    // Uniform related to lights
    GLuint laUniform_perFragment[3];
    GLuint ldUniform_perFragment[3];
    GLuint lsUniform_perFragment[3];
    GLuint lightPositionUniform_perFragment[3];

    // Uniform related to material
    GLuint kaUniform_perFragment;
    GLuint kdUniform_perFragment;
    GLuint ksUniform_perFragment;
    GLuint materialShininessUniform_perFragment;

    // Uniform related to DoubleTap
    GLuint lDoubleTapUniform_perFragment;

    struct Light
    {
        vec4 ambient;
        vec4 diffuse;
        vec4 specular;
        vec4 position;
    };
    struct Light light[3];

    // Global Variables related to lights
    GLfloat materialAmbient[4];
    GLfloat materialDiffuse[4];
    GLfloat materialSpecular[4];
    GLfloat materialShininess;

    BOOL bLight;
    BOOL bPerVertexLight;
    BOOL bPerFragmentLight;

    int iSingleTap;
    int iDoubleTap;

    // Rotation angles
    GLfloat lightAngleZero;
    GLfloat lightAngleOne;
    GLfloat lightAngleTwo;
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
    if(iSingleTap > 1)
    {
        iSingleTap = 0;
        bPerVertexLight = YES;
        bPerFragmentLight = NO;
    }
    else
    {
        bPerFragmentLight = YES;
        bPerVertexLight = NO;
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

    // ******************************   PER VERTEX LIGHT     ******************************

    // VERTEX SHADER
    // Step 1 : Write the Shader Source Code
    const GLchar* vertexShaderSourceCode_PerVertex =
        "#version 300 es\n" \
        "precision highp int;\n" \
        "precision highp float;\n" \
        "in vec4 aPosition;\n" \
        "in vec3 aNormal;\n" \
        "uniform mat4 uModelMatrix;\n" \
        "uniform mat4 uViewMatrix;\n" \
        "uniform mat4 uProjectionMatrix;\n" \
        "uniform vec3 uLa[3];\n" \
        "uniform vec3 uLd[3];\n" \
        "uniform vec3 uLs[3];\n" \
        "uniform vec4 uLightPosition[3];\n" \
        "uniform vec3 uKa;\n" \
        "uniform vec3 uKd;\n" \
        "uniform vec3 uKs;\n" \
        "uniform float uMaterialShininess;\n" \
        "uniform int uDoubleTap;\n" \
        "out vec3 out_phong_ads_Light;\n" \
        "void main(void)\n" \
        "{\n" \
            "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n" \
            "if(uDoubleTap == 1)\n" \
            "{\n" \
                "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n" \
                "mat3 normalMatrix = mat3(uViewMatrix * uModelMatrix);\n" \
                "vec3 transformedNormals = normalize(normalMatrix * aNormal);\n" \
                "vec3 viewerVector = normalize(-eyeCoordinates.xyz);\n" \
                "vec3 lightDirection[3];\n" \
                "vec3 ambientLight[3];\n" \
                "vec3 diffuseLight[3];\n" \
                "vec3 reflectionVector[3];\n" \
                "vec3 specularLight[3];\n" \
                "out_phong_ads_Light = vec3(0.0f, 0.0f, 0.0f);\n" \
                "for(int i = 0; i < 3; i++)\n" \
                "{\n" \
                    "lightDirection[i] = normalize(vec3(uLightPosition[i] - eyeCoordinates));\n" \
                    "ambientLight[i] = uLa[i] * uKa;\n" \
                    "diffuseLight[i] = uLd[i] * uKd * max(dot(lightDirection[i], transformedNormals), 0.0f);\n" \
                    "reflectionVector[i] = reflect(-lightDirection[i], transformedNormals);\n" \
                    "specularLight[i] = uLs[i] * uKs * pow(max(dot(reflectionVector[i], viewerVector), 0.0f), uMaterialShininess);\n" \
                    "out_phong_ads_Light = out_phong_ads_Light + ambientLight[i] + diffuseLight[i] + specularLight[i];\n" \
                "}\n" \
            "}\n" \
            "else\n" \
            "{\n" \
                "out_phong_ads_Light = vec3(1.0f, 1.0f, 1.0f);\n" \
            "}\n" \
        "}\n";

    // Step 2 : Create the Shader Object
    GLuint vertexShaderObject_PerVertex = glCreateShader(GL_VERTEX_SHADER);

    // Step 3 : Give the Shader Source Code to the Shader Object
    glShaderSource(vertexShaderObject_PerVertex, 1, (const GLchar**)&vertexShaderSourceCode_PerVertex, NULL);

    // Step 4 : Compile the Shader Programmatically
    glCompileShader(vertexShaderObject_PerVertex);

    // Step 5 : Shader Compilation Error Checking
    GLint status = 0;
    GLint infoLogLength = 0;
    GLchar* szInfoLog = NULL;

    glGetShaderiv(vertexShaderObject_PerVertex, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObject_PerVertex, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar));
            if (szInfoLog != NULL)
            {
                glGetShaderInfoLog(vertexShaderObject_PerVertex, infoLogLength, NULL, szInfoLog);
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
    const GLchar* fragmentShaderSourceCode_PerVertex =
        "#version 300 es\n" \
        "precision highp int;\n" \
        "precision highp float;\n" \
        "in vec3 out_phong_ads_Light;\n" \
        "out vec4 FragColor;\n" \
        "uniform int uDoubleTap;\n" \
        "void main(void)\n" \
        "{\n" \
            "if(uDoubleTap == 1)\n" \
            "{\n" \
                "FragColor = vec4(out_phong_ads_Light, 1.0f);\n" \
            "}\n" \
            "else\n" \
            "{\n" \
                "FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n" \
            "}\n" \
        "}\n";

    // Step 2 : Create the Shader Object
    GLuint fragmentShaderObject_PerVertex = glCreateShader(GL_FRAGMENT_SHADER);

    // Step 3 : Give the Shader Source Code to the Shader Object
    glShaderSource(fragmentShaderObject_PerVertex, 1, (const GLchar**)&fragmentShaderSourceCode_PerVertex, NULL);

    // Step 4 : Compile the Shader Programmatically
    glCompileShader(fragmentShaderObject_PerVertex);

    // Step 5 : Shader Compilation Error Checking
    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetShaderiv(fragmentShaderObject_PerVertex, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderObject_PerVertex, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar));
            if (szInfoLog != NULL)
            {
                glGetShaderInfoLog(fragmentShaderObject_PerVertex, infoLogLength, NULL, szInfoLog);
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
    shaderProgramObject_PerVertex = glCreateProgram();

    glAttachShader(shaderProgramObject_PerVertex, vertexShaderObject_PerVertex);
    glAttachShader(shaderProgramObject_PerVertex, fragmentShaderObject_PerVertex);

    // Bind Shader Attributes at a certain index in Shader to same index in Host Program
    glBindAttribLocation(shaderProgramObject_PerVertex, AMC_ATTRIBUTE_POSITION, "aPosition");
    glBindAttribLocation(shaderProgramObject_PerVertex, AMC_ATTRIBUTE_NORMAL, "aNormal");

    glLinkProgram(shaderProgramObject_PerVertex);

    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetProgramiv(shaderProgramObject_PerVertex, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject_PerVertex, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar));
            if (szInfoLog != NULL)
            {
                glGetProgramInfoLog(shaderProgramObject_PerVertex, infoLogLength, NULL, szInfoLog);
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
    modelMatrixUniform_perVertex = glGetUniformLocation(shaderProgramObject_PerVertex, "uModelMatrix");
    viewMatrixUniform_perVertex = glGetUniformLocation(shaderProgramObject_PerVertex, "uViewMatrix");
    projectionMatrixUniform_perVertex = glGetUniformLocation(shaderProgramObject_PerVertex, "uProjectionMatrix");
    
    laUniform_perVertex[0] = glGetUniformLocation(shaderProgramObject_PerVertex, "uLa[0]");
    ldUniform_perVertex[0] = glGetUniformLocation(shaderProgramObject_PerVertex, "uLd[0]");
    lsUniform_perVertex[0] = glGetUniformLocation(shaderProgramObject_PerVertex, "uLs[0]");
    lightPositionUniform_perVertex[0] = glGetUniformLocation(shaderProgramObject_PerVertex, "uLightPosition[0]");

    laUniform_perVertex[1] = glGetUniformLocation(shaderProgramObject_PerVertex, "uLa[1]");
    ldUniform_perVertex[1] = glGetUniformLocation(shaderProgramObject_PerVertex, "uLd[1]");
    lsUniform_perVertex[1] = glGetUniformLocation(shaderProgramObject_PerVertex, "uLs[1]");
    lightPositionUniform_perVertex[1] = glGetUniformLocation(shaderProgramObject_PerVertex, "uLightPosition[1]");

    laUniform_perVertex[2] = glGetUniformLocation(shaderProgramObject_PerVertex, "uLa[2]");
    ldUniform_perVertex[2] = glGetUniformLocation(shaderProgramObject_PerVertex, "uLd[2]");
    lsUniform_perVertex[2] = glGetUniformLocation(shaderProgramObject_PerVertex, "uLs[2]");
    lightPositionUniform_perVertex[2] = glGetUniformLocation(shaderProgramObject_PerVertex, "uLightPosition[2]");
    
    kaUniform_perVertex = glGetUniformLocation(shaderProgramObject_PerVertex, "uKa");
    kdUniform_perVertex = glGetUniformLocation(shaderProgramObject_PerVertex, "uKd");
    ksUniform_perVertex = glGetUniformLocation(shaderProgramObject_PerVertex, "uKs");

    materialShininessUniform_perVertex = glGetUniformLocation(shaderProgramObject_PerVertex, "uMaterialShininess");
    
    lDoubleTapUniform_perVertex = glGetUniformLocation(shaderProgramObject_PerVertex, "uDoubleTap");


    // ******************************   PER FRAGMENT LIGHT   ******************************

    // VERTEX SHADER
    // Step 1 : Write the Shader Source Code
    const GLchar* vertexShaderSourceCode_PerFragment =
        "#version 300 es\n" \
        "precision highp int;\n" \
        "precision highp float;\n" \
        "in vec4 aPosition;\n" \
        "in vec3 aNormal;\n" \
        "uniform mat4 uModelMatrix;\n" \
        "uniform mat4 uViewMatrix;\n" \
        "uniform mat4 uProjectionMatrix;\n" \
        "uniform vec4 uLightPosition[3];\n" \
        "uniform int uDoubleTap;\n" \
        "out vec3 out_TransformedNormals;\n" \
        "out vec3 out_LightDirection[3];\n" \
        "out vec3 out_ViewerVector;\n" \
        "void main(void)\n" \
        "{\n" \
            "gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n" \
            "if(uDoubleTap == 1)\n" \
            "{\n" \
                "vec4 eyeCoordinates = uViewMatrix * uModelMatrix * aPosition;\n" \
                "mat3 normalMatrix = mat3(uViewMatrix * uModelMatrix);\n" \
                "out_TransformedNormals = normalMatrix * aNormal;\n" \
                "out_ViewerVector = -eyeCoordinates.xyz;\n" \
                "for(int i = 0; i < 3; i++)\n" \
                "{\n" \
                    "out_LightDirection[i] = vec3(uLightPosition[i] - eyeCoordinates);\n" \
                "}\n" \
            "}\n" \
        "}\n";

    // Step 2 : Create the Shader Object
    GLuint vertexShaderObject_PerFragment = glCreateShader(GL_VERTEX_SHADER);

    // Step 3 : Give the Shader Source Code to the Shader Object
    glShaderSource(vertexShaderObject_PerFragment, 1, (const GLchar**)&vertexShaderSourceCode_PerFragment, NULL);

    // Step 4 : Compile the Shader Programmatically
    glCompileShader(vertexShaderObject_PerFragment);

    // Step 5 : Shader Compilation Error Checking
    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetShaderiv(vertexShaderObject_PerFragment, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObject_PerFragment, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar));
            if (szInfoLog != NULL)
            {
                glGetShaderInfoLog(vertexShaderObject_PerFragment, infoLogLength, NULL, szInfoLog);
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
    const GLchar* fragmentShaderSourceCode_PerFragment =
        "#version 300 es\n" \
        "precision highp int;\n" \
        "precision highp float;\n" \
        "in vec3 out_TransformedNormals;\n" \
        "in vec3 out_LightDirection[3];\n" \
        "in vec3 out_ViewerVector;\n" \
        "uniform vec3 uLa[3];\n" \
        "uniform vec3 uLd[3];\n" \
        "uniform vec3 uLs[3];\n" \
        "uniform vec3 uKa;\n" \
        "uniform vec3 uKd;\n" \
        "uniform vec3 uKs;\n" \
        "uniform float uMaterialShininess;\n" \
        "uniform int uDoubleTap;\n" \
        "out vec4 FragColor;\n" \
        "void main(void)\n" \
        "{\n" \
            "vec3 ambientLight[3];\n" \
            "vec3 diffuseLight[3];\n" \
            "vec3 specularLight[3];\n" \
            "vec3 reflectionVector[3];\n" \
            "vec3 phong_ads_Light = vec3(0.0f, 0.0f, 0.0f);\n" \
            "if(uDoubleTap == 1)\n" \
            "{\n" \
                "vec3 normalizedTransformedNormals = normalize(out_TransformedNormals);\n" \
                "vec3 normalizedViewerVector = normalize(out_ViewerVector);\n" \
                "for(int i = 0; i < 3; i++)\n" \
                "{\n" \
                    "vec3 normalizedLightDirection = normalize(out_LightDirection[i]);\n" \
                    "ambientLight[i] = uLa[i] * uKa;\n" \
                    "diffuseLight[i] = uLd[i] * uKd * max(dot(normalizedLightDirection, normalizedTransformedNormals), 0.0f);\n" \
                    "reflectionVector[i] = reflect(-normalizedLightDirection, normalizedTransformedNormals);\n" \
                    "specularLight[i] = uLs[i] * uKs * pow(max(dot(reflectionVector[i], normalizedViewerVector), 0.0f), uMaterialShininess);\n" \
                    "phong_ads_Light = phong_ads_Light + ambientLight[i] + diffuseLight[i] + specularLight[i];\n" \
                "}\n" \
            "}\n" \
            "else\n" \
            "{\n" \
                "phong_ads_Light = vec3(1.0f, 1.0f, 1.0f);\n" \
            "}\n" \
            "FragColor = vec4(phong_ads_Light, 1.0f);\n" \
        "}\n";

    // Step 2 : Create the Shader Object
    GLuint fragmentShaderObject_PerFragment = glCreateShader(GL_FRAGMENT_SHADER);

    // Step 3 : Give the Shader Source Code to the Shader Object
    glShaderSource(fragmentShaderObject_PerFragment, 1, (const GLchar**)&fragmentShaderSourceCode_PerFragment, NULL);

    // Step 4 : Compile the Shader Programmatically
    glCompileShader(fragmentShaderObject_PerFragment);

    // Step 5 : Shader Compilation Error Checking
    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetShaderiv(fragmentShaderObject_PerFragment, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderiv(fragmentShaderObject_PerFragment, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar));
            if (szInfoLog != NULL)
            {
                glGetShaderInfoLog(fragmentShaderObject_PerFragment, infoLogLength, NULL, szInfoLog);
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
    shaderProgramObject_PerFragment = glCreateProgram();

    glAttachShader(shaderProgramObject_PerFragment, vertexShaderObject_PerFragment);
    glAttachShader(shaderProgramObject_PerFragment, fragmentShaderObject_PerFragment);

    // Bind Shader Attributes at a certain index in Shader to same index in Host Program
    glBindAttribLocation(shaderProgramObject_PerFragment, AMC_ATTRIBUTE_POSITION, "aPosition");
    glBindAttribLocation(shaderProgramObject_PerFragment, AMC_ATTRIBUTE_NORMAL, "aNormal");

    glLinkProgram(shaderProgramObject_PerFragment);

    status = 0;
    infoLogLength = 0;
    szInfoLog = NULL;

    glGetProgramiv(shaderProgramObject_PerFragment, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObject_PerFragment, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar));
            if (szInfoLog != NULL)
            {
                glGetProgramInfoLog(shaderProgramObject_PerFragment, infoLogLength, NULL, szInfoLog);
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
    modelMatrixUniform_perFragment = glGetUniformLocation(shaderProgramObject_PerFragment, "uModelMatrix");
    viewMatrixUniform_perFragment = glGetUniformLocation(shaderProgramObject_PerFragment, "uViewMatrix");
    projectionMatrixUniform_perFragment = glGetUniformLocation(shaderProgramObject_PerFragment, "uProjectionMatrix");

    laUniform_perFragment[0] = glGetUniformLocation(shaderProgramObject_PerFragment, "uLa[0]");
    ldUniform_perFragment[0] = glGetUniformLocation(shaderProgramObject_PerFragment, "uLd[0]");
    lsUniform_perFragment[0] = glGetUniformLocation(shaderProgramObject_PerFragment, "uLs[0]");
    lightPositionUniform_perFragment[0] = glGetUniformLocation(shaderProgramObject_PerFragment, "uLightPosition[0]");

    laUniform_perFragment[1] = glGetUniformLocation(shaderProgramObject_PerFragment, "uLa[1]");
    ldUniform_perFragment[1] = glGetUniformLocation(shaderProgramObject_PerFragment, "uLd[1]");
    lsUniform_perFragment[1] = glGetUniformLocation(shaderProgramObject_PerFragment, "uLs[1]");
    lightPositionUniform_perFragment[1] = glGetUniformLocation(shaderProgramObject_PerFragment, "uLightPosition[1]");

    laUniform_perFragment[2] = glGetUniformLocation(shaderProgramObject_PerFragment, "uLa[2]");
    ldUniform_perFragment[2] = glGetUniformLocation(shaderProgramObject_PerFragment, "uLd[2]");
    lsUniform_perFragment[2] = glGetUniformLocation(shaderProgramObject_PerFragment, "uLs[2]");
    lightPositionUniform_perFragment[2] = glGetUniformLocation(shaderProgramObject_PerFragment, "uLightPosition[2]");

    kaUniform_perFragment = glGetUniformLocation(shaderProgramObject_PerFragment, "uKa");
    kdUniform_perFragment = glGetUniformLocation(shaderProgramObject_PerFragment, "uKd");
    ksUniform_perFragment = glGetUniformLocation(shaderProgramObject_PerFragment, "uKs");

    materialShininessUniform_perFragment = glGetUniformLocation(shaderProgramObject_PerFragment, "uMaterialShininess");

    lDoubleTapUniform_perFragment = glGetUniformLocation(shaderProgramObject_PerFragment, "uDoubleTap");

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

    materialShininess = 50.0f;

    // LIGHT0 Configuration
    light[0].ambient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    light[0].diffuse = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    light[0].specular = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    light[0].position = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    // LIGHT1 Configuration
    light[1].ambient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    light[1].diffuse = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    light[1].specular = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    light[1].position = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    // LIGHT2 Configuration
    light[2].ambient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    light[2].diffuse = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    light[2].specular = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    light[2].position = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    bLight = NO;
    bPerVertexLight = YES;
    bPerFragmentLight = NO;

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

    /*************************      SPHERE      *************************/
    // Transformations
    // This line is analogous to glLoadIdentity() in display() for MODELVIEW matrix
    mat4 modelMatrix = mat4::identity();

    mat4 viewMatrix = mat4::identity();

    mat4 translationMatrix = mat4::identity();

    // Translate Sphere backwards by Z
    translationMatrix = vmath::translate(0.0f, 0.0f, -2.5f);

    modelMatrix = translationMatrix;

    if (bPerVertexLight)
    {
        // Use Shader Program Object
        glUseProgram(shaderProgramObject_PerVertex);

        // Send above matrix to the vertex shader in uniform
        glUniformMatrix4fv(modelMatrixUniform_perVertex, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewMatrixUniform_perVertex, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projectionMatrixUniform_perVertex, 1, GL_FALSE, perspectiveProjectionMatrix);
    }
    else if (bPerFragmentLight)
    {
        // Use Shader Program Object
        glUseProgram(shaderProgramObject_PerFragment);

        // Send above matrix to the vertex shader in uniform
        glUniformMatrix4fv(modelMatrixUniform_perFragment, 1, GL_FALSE, modelMatrix);
        glUniformMatrix4fv(viewMatrixUniform_perFragment, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projectionMatrixUniform_perFragment, 1, GL_FALSE, perspectiveProjectionMatrix);
    }

    if (bLight == YES)
    {
        if (bPerVertexLight == YES)
        {
            // RED Light
            glUniform3fv(laUniform_perVertex[0], 1, light[0].ambient);
            glUniform3fv(ldUniform_perVertex[0], 1, light[0].diffuse);
            glUniform3fv(lsUniform_perVertex[0], 1, light[0].specular);

            glUniform4f(lightPositionUniform_perVertex[0], sinf(lightAngleZero) * 3, cosf(lightAngleZero) * 3, 0.0f, 1.0f);

            // GREEN Light
            glUniform3fv(laUniform_perVertex[1], 1, light[1].ambient);
            glUniform3fv(ldUniform_perVertex[1], 1, light[1].diffuse);
            glUniform3fv(lsUniform_perVertex[1], 1, light[1].specular);

            glUniform4f(lightPositionUniform_perVertex[1], 0.0f, sinf(lightAngleOne) * 3, cosf(lightAngleOne) * 3, 1.0f);

            // BLUE Light
            glUniform3fv(laUniform_perVertex[2], 1, light[2].ambient);
            glUniform3fv(ldUniform_perVertex[2], 1, light[2].diffuse);
            glUniform3fv(lsUniform_perVertex[2], 1, light[2].specular);

            glUniform4f(lightPositionUniform_perVertex[2], sinf(lightAngleTwo) * 3, 0.0f, cosf(lightAngleTwo) * 3, 1.0f);
            
            // Material
            glUniform3fv(kaUniform_perVertex, 1, materialAmbient);
            glUniform3fv(kdUniform_perVertex, 1, materialDiffuse);
            glUniform3fv(ksUniform_perVertex, 1, materialSpecular);
            glUniform1f(materialShininessUniform_perVertex, materialShininess);

            glUniform1i(lDoubleTapUniform_perVertex, 1);
        }
        else if (bPerFragmentLight == YES)
        {
            // RED Light
            glUniform3fv(laUniform_perFragment[0], 1, light[0].ambient);
            glUniform3fv(ldUniform_perFragment[0], 1, light[0].diffuse);
            glUniform3fv(lsUniform_perFragment[0], 1, light[0].specular);

            glUniform4f(lightPositionUniform_perFragment[0], sinf(lightAngleZero) * 3, cosf(lightAngleZero) * 3, 0.0f, 1.0f);

            // GREEN Light
            glUniform3fv(laUniform_perFragment[1], 1, light[1].ambient);
            glUniform3fv(ldUniform_perFragment[1], 1, light[1].diffuse);
            glUniform3fv(lsUniform_perFragment[1], 1, light[1].specular);

            glUniform4f(lightPositionUniform_perFragment[1], 0.0f, sinf(lightAngleOne) * 3, cosf(lightAngleOne) * 3, 1.0f);

            // BLUE Light
            glUniform3fv(laUniform_perFragment[2], 1, light[2].ambient);
            glUniform3fv(ldUniform_perFragment[2], 1, light[2].diffuse);
            glUniform3fv(lsUniform_perFragment[2], 1, light[2].specular);

            glUniform4f(lightPositionUniform_perFragment[2], sinf(lightAngleTwo) * 3, 0.0f, cosf(lightAngleTwo) * 3, 1.0f);

            // Material
            glUniform3fv(kaUniform_perFragment, 1, materialAmbient);
            glUniform3fv(kdUniform_perFragment, 1, materialDiffuse);
            glUniform3fv(ksUniform_perFragment, 1, materialSpecular);
            glUniform1f(materialShininessUniform_perFragment, materialShininess);

            glUniform1i(lDoubleTapUniform_perFragment, 1);
        }
    }
    else
    {
        if (bPerVertexLight)
            glUniform1i(lDoubleTapUniform_perVertex, 0);
        else
            glUniform1i(lDoubleTapUniform_perFragment, 0);
    }

    // Bind with vao_sphere
    glBindVertexArray(vao_sphere);

    // draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element_sphere);
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

    // UnBind with vao_sphere
    glBindVertexArray(0);

    if (shaderProgramObject_PerVertex)
    {
        // Unuse Shader Program Object
        glUseProgram(0);
    }

    if (shaderProgramObject_PerFragment)
    {
        // Unuse Shader Program Object
        glUseProgram(0);
    }
}

-(void)myUpdate
{
    // Code
    // Update RED Light Angle
    lightAngleZero = lightAngleZero + 0.05f;
    if (lightAngleZero >= 360.0f)
    {
        lightAngleZero = lightAngleZero - 360.0f;
    }

    // Update GREEN Light Angle
    lightAngleOne = lightAngleOne + 0.05f;
    if (lightAngleOne >= 360.0f)
    {
        lightAngleOne = lightAngleOne - 360.0f;
    }

    // Update BLUE Light Angle
    lightAngleTwo = lightAngleTwo + 0.05f;
    if (lightAngleTwo >= 360.0f)
    {
        lightAngleTwo = lightAngleTwo - 360.0f;
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
    if (shaderProgramObject_PerFragment)
    {
        glUseProgram(shaderProgramObject_PerFragment);
        GLint numShaders;

        // Step 2 : Get Number of Attached Shaders and Continue if Number Of Shaders is Greater than 0 
        glGetProgramiv(shaderProgramObject_PerFragment, GL_ATTACHED_SHADERS, &numShaders);
        if (numShaders > 0)
        {
            // Step 3 : Create a Buffer / Array to hold Attached Shader Object of Obtained Number of Shaders and Continue Only if malloc is Succeeded
            GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));
            if (pShaders != NULL)
            {
                // Step 4 : Get Shader Objects into this Buffer / Array
                glGetAttachedShaders(shaderProgramObject_PerFragment, numShaders, NULL, pShaders);

                // Step 5 : Iterate through Obtained Number of Shaders and inside this loop Detach and Delete Every Shader from the Buffer / Array
                for (GLint i = 0; i < numShaders; i++)
                {
                    glDetachShader(shaderProgramObject_PerFragment, pShaders[i]);
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
        glDeleteProgram(shaderProgramObject_PerFragment);
        shaderProgramObject_PerFragment = 0;
    }
    
    // Step 1: Check whether Shader Program Object is still there
    if (shaderProgramObject_PerVertex)
    {
        glUseProgram(shaderProgramObject_PerVertex);
        GLint numShaders;

        // Step 2 : Get Number of Attached Shaders and Continue if Number Of Shaders is Greater than 0 
        glGetProgramiv(shaderProgramObject_PerVertex, GL_ATTACHED_SHADERS, &numShaders);
        if (numShaders > 0)
        {
            // Step 3 : Create a Buffer / Array to hold Attached Shader Object of Obtained Number of Shaders and Continue Only if malloc is Succeeded
            GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint));
            if (pShaders != NULL)
            {
                // Step 4 : Get Shader Objects into this Buffer / Array
                glGetAttachedShaders(shaderProgramObject_PerVertex, numShaders, NULL, pShaders);

                // Step 5 : Iterate through Obtained Number of Shaders and inside this loop Detach and Delete Every Shader from the Buffer / Array
                for (GLint i = 0; i < numShaders; i++)
                {
                    glDetachShader(shaderProgramObject_PerVertex, pShaders[i]);
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
        glDeleteProgram(shaderProgramObject_PerVertex);
        shaderProgramObject_PerVertex = 0;
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
