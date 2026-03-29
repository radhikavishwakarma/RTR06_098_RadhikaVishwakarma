#import <Foundation/Foundation.h>	// Similar to stdio.h
#import <Cocoa/Cocoa.h> 			// Similar to Windows.h

// OpenGL related Header files
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>

// Sphere related Header Files
#include "Sphere.h"

// Header file for Matrix and Transformation related functions
#include "vmath.h"
using namespace vmath;

// Global function declarations
CVReturn myDisplayLinkCallback(CVDisplayLinkRef,
							   const CVTimeStamp *,
							   const CVTimeStamp *,
							   CVOptionFlags,
							   CVOptionFlags *,
							   void *);

// Global Variable declarations
FILE *gpFile = NULL;

enum
{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_NORMAL,
};

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

@interface GLView : NSOpenGLView
-(void)uninitialize;
@end

// Entry-point Function
int main(int argc, char *argv[])
{
	// Code
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];

	NSApp = [NSApplication sharedApplication];

	[NSApp setDelegate:[[AppDelegate alloc]init]];

	[NSApp run];

	[pool release];
	
	return(0);
}

@implementation AppDelegate
{
@private
	NSWindow *window;
	GLView *glView;
}

-(void)applicationDidFinishLaunching:(NSNotification *)notification
{
	// Code
	// Create Log file
	NSBundle *appBundle = [NSBundle mainBundle];
	NSString *appDirPath = [appBundle bundlePath];
	NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];
	NSString *logFileNameWithPath = [NSString stringWithFormat:@"%@/Log.txt",parentDirPath];
	const char *pszLogFileNameWithPath = [logFileNameWithPath cStringUsingEncoding:NSASCIIStringEncoding];

	gpFile = fopen(pszLogFileNameWithPath,"w");
	if(gpFile == NULL)
	{
		// Message Box
		NSAlert *alert = [[NSAlert alloc]init];
		[alert setAlertStyle:NSAlertStyleCritical];
		[alert setMessageText:@"Log File Creation Failed!"];
		[alert addButtonWithTitle:@"Exit"];
		[alert runModal];
		[alert release];

		// Exit
		[self release];
		[NSApp terminate:self];
	}
	else
	{
		fprintf(gpFile, "Program Started Successfully!\n");
	}

	// Window
	NSRect winRect = NSMakeRect(0.0, 0.0, 800.0, 600.0);
	window = [[NSWindow alloc]initWithContentRect:winRect
										styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable
										  backing:NSBackingStoreBuffered
										  	defer:NO];

	[window setTitle:@"Radhika Vishwakarma"];
	[window center];

	glView = [[GLView alloc]initWithFrame:winRect];

	[window setContentView:glView];
	[window setDelegate:self];
	[window makeKeyAndOrderFront:self];
}

-(void)applicationWillTerminate:(NSNotification *)notification
{
	// Code
	if(gpFile)
	{
		fprintf(gpFile, "Program Terminated Successfully!\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}

-(BOOL)windowShouldClose:(NSWindow *)aWindow
{
    // code
    [glView uninitialize];
    return(YES);
}

-(void)windowWillClose:(NSNotification *)notification
{
	// Code
	[NSApp terminate:self];
}

-(void)dealloc
{
	// Code
	[glView release];

	[window release];

	[super dealloc];
}

@end

@implementation GLView
{
	CVDisplayLinkRef displayLink;
	int winWidth;
	int winHeight;

	// Shader related global variables
	GLuint shaderProgramObject_PerVertex;
	GLuint shaderProgramObject_PerFragment;

	// Sphere related
	GLuint vao_sphere;		// Vertex Array Object
	GLuint vbo_position_sphere;	// Vertex Buffer Object
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
	// Per-Vertex
	GLuint modelMatrixUniform_PV;
	GLuint viewMatrixUniform_PV;
	GLuint projectionMatrixUniform_PV;
	GLuint LKeyIsPressedUniform_PV;

	// Per-Fragment
	GLuint modelMatrixUniform_PF;
	GLuint viewMatrixUniform_PF;
	GLuint projectionMatrixUniform_PF;
	GLuint LKeyIsPressedUniform_PF;

	// Uniforms related to Per Vertex Light
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

	// Uniforms related to Per Fragment Light
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

	// Uniform related to keypress
	GLuint LKeyIsPressedUniform;

	// Global Variables related to lights
	GLfloat lightAmbient[4];
	GLfloat lightDiffuse[4];
	GLfloat lightSpecular[4];
	GLfloat lightPosition[4];

	GLfloat materialAmbient[4];
	GLfloat materialDiffuse[4];
	GLfloat materialSpecular[4];
	GLfloat materialShininess;

	struct Light
	{
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		vec4 position;
	};
	struct Light light[3];

	GLfloat lightAngleZero;
	GLfloat lightAngleOne;
	GLfloat lightAngleTwo;

	BOOL bLight;
	BOOL bPerVertexLight;
	BOOL bPerFragmentLight;
}

-(id)initWithFrame:(NSRect)frame
{
	// Code
	self = [super initWithFrame:frame];

	if(self)
	{
		[[self window]setContentView:self];
		
		[self setWantsLayer:YES];
		NSColor *backgroundColor = [NSColor blackColor];
		struct CGColor *bgColor = [backgroundColor CGColor];
		[[self layer]setBackgroundColor:bgColor];

		// Step 1: NSOpenGLPixelFormatAttribute Initialization
		NSOpenGLPixelFormatAttribute attributes[] = 
		{
			NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
			NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
			NSOpenGLPFANoRecovery,
			NSOpenGLPFAAccelerated,
			NSOpenGLPFAColorSize, 24,
			NSOpenGLPFADepthSize, 24,
			NSOpenGLPFAAlphaSize, 8,
			NSOpenGLPFADoubleBuffer,
			0
		};

		// Step 2: Create NSOpenGLPixelFormat
		NSOpenGLPixelFormat *pixelFormat = [[[NSOpenGLPixelFormat alloc]initWithAttributes:attributes]autorelease];
		
		if(pixelFormat == nil)
		{
			fprintf(gpFile,"NSOpenGLPixelFormat Creation Failed!\n");
			[self uninitialize];
			[self release];
			[NSApp terminate:self];
		}

		// Step 3: Create NSOpenGLContext
		NSOpenGLContext *glContext = [[[NSOpenGLContext alloc]initWithFormat:pixelFormat shareContext:nil]autorelease];

		if(glContext == nil)
		{
			fprintf(gpFile,"NSOpenGLContext Creation Failed!\n");
			[self uninitialize];
			[self release];
			[NSApp terminate:self];
		}

		// Step 4: Set NSOpenGLView's PixelFormat with our created PixelFormat
		[self setPixelFormat:pixelFormat];

		// Step 4: Set NSOpenGLView's OpenGLContext with our created Context
		[self setOpenGLContext:glContext];
	}

	return(self);
}

-(CVReturn)getFrameForTime:(const CVTimeStamp *)outputTime
{
	// Code
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];

	[self drawView];

	[pool release];

	return(kCVReturnSuccess);
}

-(void)prepareOpenGL
{
	// Code
	// Step 1: Call super prepareOpenGL
	[super prepareOpenGL];

	// Step 2: Make the OpenGL Context as Current Context
	[[self openGLContext]makeCurrentContext];

	// Step 3: Set Swap Interval to 1 to Synchronize Arrival of Buffers in Double Buffering, to Avoid Screen tearing
	GLint swapInterval = 1;
	[[self openGLContext]setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];

	// Step 4: Now call, our initialize function here
	int iResult = [self initialize];

	// Step 5: Start the DisplayLink to Create the Separate Rendering Thread which will Render our View
	// Step A: Create the DisplayLink
	CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);

	// Step B: Set the Callback Function of this DisplayLink
	CVDisplayLinkSetOutputCallback(displayLink, &myDisplayLinkCallback, self);

	// Step C: Convert NSOpenGLPixelFormat into CGLPixelFormatObj
	CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat]CGLPixelFormatObj];

	// Step D: Convert NSOpenGLContext into CGLContext
	CGLContextObj cglContext = (CGLContextObj)[[self openGLContext]CGLContextObj];

	// Step E: Set CGLPixelFormat and CGLContext as Current Display's CGLPixelFormat and CGLContext for this DisplayLink
	CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);

	// Step F: Start the DisplayLink (In other words, Start the new Rendering Thread)
	CVDisplayLinkStart(displayLink);
}

-(void)reshape
{
	// Code
	[super reshape];

	// Make the OpenGL Context as Current Context
	[[self openGLContext]makeCurrentContext];

	// Lock the Current CGLContext
	CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);

	// Variable declarations
	NSRect viewRect = [self bounds];
	winWidth = (int)viewRect.size.width;
	winHeight = (int)viewRect.size.height;

	[self resize:winWidth :winHeight];
	
	// Unlock the Current CGLContext
	CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
}

-(void)drawView
{
	// Code
	// Make the OpenGL Context as Current Context
	[[self openGLContext]makeCurrentContext];

	// Lock the Current CGLContext
	CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);

	// Render
	[self display];

	// Update
	[self myUpdate];

	// Swap buffers or Double buffering
	CGLFlushDrawable((CGLContextObj)[[self openGLContext]CGLContextObj]);

	// Unlock the Current CGLContext
	CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
}

-(void)drawRect:(NSRect)dirtyRect
{
	// Code
	[self drawView];	// To avoid the possibility of flickering
}

-(BOOL)acceptsFirstResponder
{
	// Code
	[[self window]makeFirstResponder:self];

	return(YES);
}

-(void)keyDown:(NSEvent *)event
{
	// Code
	int key = (int)[[event characters]characterAtIndex:0];

	switch(key)
	{
	case 27:
		[self uninitialize];
		[self release];
		[NSApp terminate:self];
		break;

	case 'L':
	case 'l':
		if (bLight == NO)
		{
			bLight = YES;
		}
		else
		{
			bLight = NO;
		}
		break;

	case 'F':
	case 'f':
		bPerFragmentLight = YES;
		bPerVertexLight = NO;
		break;

	case 'V':
	case 'v':
		bPerVertexLight = YES;
		bPerFragmentLight = NO;
		break;

	default:
		break;
	}
}

-(void)mouseDown:(NSEvent *)event
{
	// Code
}

-(int)initialize
{
	// Code
	// Print GL Information
	[self printGLInfo];

	// ******************************	PER VERTEX LIGHT	 ******************************

	// VERTEX SHADER
	// Step 1 : Write the Shader Source Code
	const GLchar* vertexShaderSourceCode_PerVertex =
		"#version 410 core\n" \
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
		"uniform int uLKeyIsPressed;\n" \
		"out vec3 out_phong_ads_Light;\n" \
		"void main(void)\n" \
		"{\n" \
			"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n" \
			"if(uLKeyIsPressed == 1)\n" \
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
				fprintf(gpFile, "Vertex Shader Compilation Log : %s \n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
			}
		}

		[self release];
		[self uninitialize];
		[NSApp terminate:self];
	}

	// FRAGMENT SHADER
	// Step 1 : Write the Shader Source Code
	const GLchar* fragmentShaderSourceCode_PerVertex =
		"#version 410 core\n" \
		"in vec3 out_phong_ads_Light;\n" \
		"out vec4 FragColor;\n" \
		"uniform int uLKeyIsPressed;\n" \
		"void main(void)\n" \
		"{\n" \
			"if(uLKeyIsPressed == 1)\n" \
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
				fprintf(gpFile, "Fragment Shader Compilation Log : %s \n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
			}
		}

		[self release];
		[self uninitialize];
		[NSApp terminate:self];
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
				fprintf(gpFile, "Shader Program Link Log : %s \n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
			}
		}

		[self release];
		[self uninitialize];
		[NSApp terminate:self];
	}

	// Get the required uniform location from the shader
	modelMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PerVertex, "uModelMatrix");
	viewMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PerVertex, "uViewMatrix");
	projectionMatrixUniform_PV = glGetUniformLocation(shaderProgramObject_PerVertex, "uProjectionMatrix");
	
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
	
	LKeyIsPressedUniform_PV = glGetUniformLocation(shaderProgramObject_PerVertex, "uLKeyIsPressed");

	// ******************************	PER FRAGMENT LIGHT	 ******************************

	// VERTEX SHADER
	// Step 1 : Write the Shader Source Code
	const GLchar* vertexShaderSourceCode_PerFragment =
		"#version 410 core\n" \
		"in vec4 aPosition;\n" \
		"in vec3 aNormal;\n" \
		"uniform mat4 uModelMatrix;\n" \
		"uniform mat4 uViewMatrix;\n" \
		"uniform mat4 uProjectionMatrix;\n" \
		"uniform vec4 uLightPosition[3];\n" \
		"uniform int uLKeyIsPressed;\n" \
		"out vec3 out_TransformedNormals;\n" \
		"out vec3 out_LightDirection[3];\n" \
		"out vec3 out_ViewerVector;\n" \
		"void main(void)\n" \
		"{\n" \
			"gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n" \
			"if(uLKeyIsPressed == 1)\n" \
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
				fprintf(gpFile, "Vertex Shader Compilation Log : %s \n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
			}
		}

		[self release];
		[self uninitialize];
		[NSApp terminate:self];
	}

	// FRAGMENT SHADER
	// Step 1 : Write the Shader Source Code	
	const GLchar* fragmentShaderSourceCode_PerFragment =
		"#version 410 core\n" \
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
		"uniform int uLKeyIsPressed;\n" \
		"out vec4 FragColor;\n" \
		"void main(void)\n" \
		"{\n" \
			"vec3 ambientLight[3];\n" \
			"vec3 diffuseLight[3];\n" \
			"vec3 specularLight[3];\n" \
			"vec3 reflectionVector[3];\n" \
			"vec3 phong_ads_Light = vec3(0.0f, 0.0f, 0.0f);\n" \
			"if(uLKeyIsPressed == 1)\n" \
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
				fprintf(gpFile, "Fragment Shader Compilation Log : %s \n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
			}
		}

		[self release];
		[self uninitialize];
		[NSApp terminate:self];
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
				fprintf(gpFile, "Shader Program Link Log : %s \n", szInfoLog);

				free(szInfoLog);
				szInfoLog = NULL;
			}
		}

		[self release];
		[self uninitialize];
		[NSApp terminate:self];
	}

	// Get the required uniform location from the shader
	modelMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PerFragment, "uModelMatrix");
	viewMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PerFragment, "uViewMatrix");
	projectionMatrixUniform_PF = glGetUniformLocation(shaderProgramObject_PerFragment, "uProjectionMatrix");

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

	LKeyIsPressedUniform_PF = glGetUniformLocation(shaderProgramObject_PerFragment, "uLKeyIsPressed");

	/*************************		SPHERE		*************************/
	// Provide Vertex position, Color, Normals, Texco-ords etc..
	Sphere *sphere = [[Sphere alloc]init];
	[sphere getSphereVertexData :sphere_vertices :sphere_normals :sphere_textures :sphere_elements];
	gNumVertices = [sphere getNumberOfSphereVertices];
	gNumElements = [sphere getNumberOfSphereElements];

	// getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	// gNumVertices = getNumberOfSphereVertices();
	// gNumElements = getNumberOfSphereElements();

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
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// From here onwards, OpenGL code starts...
	// Tell OpenGL to choose the color to clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

	materialShininess = 50.0f;

	bLight = NO;
	bPerVertexLight = YES;
	bPerFragmentLight = NO;


	// This line is analogous to glLoadIdentity in resize() for GL_PROJECTION in FFP
	perspectiveProjectionMatrix = mat4::identity();

	// Warm-up resize
	[self resize:winWidth :winHeight];

	return(0);
}

-(void)printGLInfo
{
	// Code
	fprintf(gpFile, "\n OPENGL INFORMATION \n");
	fprintf(gpFile, "*************************************************\n");
	fprintf(gpFile, "OpenGL Vender   : %s \n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer : %s \n", glGetString(GL_RENDERER));
	fprintf(gpFile, "OpenGL Version  : %s \n", glGetString(GL_VERSION));
	fprintf(gpFile, "GLSL   Version  : %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	fprintf(gpFile, "*************************************************\n\n");
}

-(void)resize:(int)width :(int)height
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
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

-(void)display
{
	// Code
	// Clear OpenGL buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*************************		SPHERE		*************************/
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
		glUniformMatrix4fv(modelMatrixUniform_PV, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform_PV, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform_PV, 1, GL_FALSE, perspectiveProjectionMatrix);
	}
	else if (bPerFragmentLight)
	{
		// Use Shader Program Object
		glUseProgram(shaderProgramObject_PerFragment);

		// Send above matrix to the vertex shader in uniform
		glUniformMatrix4fv(modelMatrixUniform_PF, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(viewMatrixUniform_PF, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionMatrixUniform_PF, 1, GL_FALSE, perspectiveProjectionMatrix);
	}

	if (bLight == TRUE)
	{
		if (bPerVertexLight == TRUE)
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

			glUniform1i(LKeyIsPressedUniform_PV, 1);
		}
		else if (bPerFragmentLight == TRUE)
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

			glUniform1i(LKeyIsPressedUniform_PF, 1);
		}
	}
	else
	{
		if (bPerVertexLight)
			glUniform1i(LKeyIsPressedUniform_PV, 0);
		else
			glUniform1i(LKeyIsPressedUniform_PF, 0);
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
	lightAngleZero = lightAngleZero + 0.02f;
	if (lightAngleZero >= 360.0f)
	{
		lightAngleZero = lightAngleZero - 360.0f;
	}

	// Update GREEN Light Angle
	lightAngleOne = lightAngleOne + 0.02f;
	if (lightAngleOne >= 360.0f)
	{
		lightAngleOne = lightAngleOne - 360.0f;
	}

	// Update BLUE Light Angle
	lightAngleTwo = lightAngleTwo + 0.02f;
	if (lightAngleTwo >= 360.0f)
	{
		lightAngleTwo = lightAngleTwo - 360.0f;
	}
}

-(void)uninitialize
{
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

	// Close the log file
	if (gpFile)
	{
		fprintf(gpFile, "Program Terminated Successfully\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}

-(void)dealloc
{
	// Code
	CVDisplayLinkStop(displayLink);
	CVDisplayLinkRelease(displayLink);
	[super dealloc];
}

@end

CVReturn myDisplayLinkCallback(CVDisplayLinkRef displayLinkRef,
							   const CVTimeStamp *current,
							   const CVTimeStamp *output,
							   CVOptionFlags inputFlags,
							   CVOptionFlags *outputFlags,
							   void *view)
{
	// Code
	CVReturn result = [(GLView *)view getFrameForTime:output];
	return(result);
}
