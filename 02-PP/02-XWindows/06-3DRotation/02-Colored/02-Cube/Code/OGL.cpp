// Standard Header files
#include <stdio.h> 
#include <stdlib.h> 
#include <memory.h> // for memset

// XWindow header files
#include <X11/Xlib.h>
#include <X11/Xutil.h>  // Visual info related API
#include <X11/XKBlib.h>  // Keyboard related Xlib API

// OpenGL related Header files
#include <GL/glew.h> // This header file must be included befor gl/GL.h
#include <GL/gl.h>
#include <GL/glx.h>

// Custom Header File
#include "vmath.h"

using namespace vmath;

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global variables
Display *gpDisplay = NULL; // 77 members structure which a mediator between Xserver and XClient.
XVisualInfo *visualInfo = NULL; // similar to device context
Window window; // gives window object
Colormap colormap; //

Bool bFullScreen = False;
Bool bActiveWindow = False;

// variables related to FILE IO
char gszLogFileName[] = "Log.txt";
FILE *gpFile = NULL;

typedef GLXContext (*glXCreateContextAttribsARBProc) (Display*, GLXFBConfig, GLXContext, Bool, const int*);

glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;

GLXFBConfig gLXFBConfig;

// OpenGL related variables
GLXContext glxContext = NULL;

// Shader related global variables
GLuint shaderProgramObject = 0;

enum{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
};

GLuint vao_cube = 0;
GLuint vbo_position_cube;
GLuint vbo_color_cube = 0;

GLuint mvpMatrixUniform = 0;

mat4 perspectiveProjectionMatrix;

// rotation angles
float angleCube = 0.0f;

int main(void)
{
	// function declarations
	int initialize(void);
	void resize(int, int);
	void display(void);
	void update(void);
	void uninitialize(void);
	void toggleFullScreen(void);

	// local variables
	int defaultScreen;
	int defaultDepth;
	XSetWindowAttributes windowAttributes;
	Atom windowManagerDeleteAtom;
	XEvent event;
	Screen *screen = NULL;
	int screenWidth, screenHeight;
	KeySym keySym;
	char keys[26]; // Although we need only 0th index, conventionally the array size is equal to number of alphabets, either capital or small or both

	GLXFBConfig *pGLXFBConfigs = NULL;
	GLXFBConfig bestGLXFBConfig;
	XVisualInfo *pXVisualInfo = NULL;
	int iNumFBConfigs = 0;

	
	int frameBufferAttributes[] = {
		GLX_X_RENDERABLE, True,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
		GLX_DOUBLEBUFFER, True,
		// GLX_RGBA,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24, // MESA recommends 24 bit depth
		GLX_STENCIL_SIZE, 8,
		None
	};

	Bool bDone = False;

	// code 
	// Create log file
	gpFile = fopen(gszLogFileName, "w");
	if (gpFile == NULL)
	{
		printf("Log file creation failed!!\n");
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Program started successfully!!\n");
	}

	// Creates connection with XServer
	gpDisplay = XOpenDisplay(NULL);

	if (gpDisplay == NULL)
	{
		fprintf(gpFile, "XOpendisplay failed to connect with XServer\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}

	// Create the default screen object
	defaultScreen = XDefaultScreen(gpDisplay); // asks the opened display to return the default screen

	// Get default depth
	defaultDepth = XDefaultDepth(gpDisplay, defaultScreen);

	pGLXFBConfigs = glXChooseFBConfig(gpDisplay, defaultScreen, frameBufferAttributes, &iNumFBConfigs);

	if (pGLXFBConfigs == NULL)
	{
		fprintf(gpFile, "Choose GLXFBConfig failed!!\n");
		uninitialize();
	}
	else
	{
		fprintf(gpFile, "Found number of GLXFBConfigs = %d\n", iNumFBConfigs);
	}

	int indexOfBestGLXFBConfig = -1;
	int indexOfWorstGLXFBConfig = -1;
	int bestNumberOfSamples = -1;
	int worstNumberOfSamples = 999;

	for (int i = 0; i < iNumFBConfigs; i++)
	{
		pXVisualInfo = glXGetVisualFromFBConfig(gpDisplay, pGLXFBConfigs[i]);

		if (pXVisualInfo)
		{
			int sampleBuffer, samples;
			glXGetFBConfigAttrib(gpDisplay, pGLXFBConfigs[i], GLX_SAMPLE_BUFFERS, &sampleBuffer);
			glXGetFBConfigAttrib(gpDisplay, pGLXFBConfigs[i], GLX_SAMPLES, &samples);
			
			if (indexOfBestGLXFBConfig < 0 || sampleBuffer && samples > bestNumberOfSamples)
			{
				indexOfBestGLXFBConfig = i;
				bestNumberOfSamples = samples;
			}
			if (indexOfWorstGLXFBConfig < 0 || !sampleBuffer || samples < worstNumberOfSamples)
			{
				indexOfWorstGLXFBConfig = i;
				worstNumberOfSamples = samples;
			}
			XFree(pXVisualInfo);
		}
	}

	// fprintf(gpFile, "LOKI = %d\n", indexOfBestGLXFBConfig);

	bestGLXFBConfig = pGLXFBConfigs[indexOfBestGLXFBConfig]; // set the global glxFBConfig;
	gLXFBConfig = bestGLXFBConfig;
	XFree(pGLXFBConfigs); // free array

	// Create the Visual Info
	// As we already go the best FBConfig from the loop there is no need to check again for visual info
	visualInfo = glXGetVisualFromFBConfig(gpDisplay, gLXFBConfig);
	
	// Set window attributes
	memset((void*) &windowAttributes, 0, sizeof(XSetWindowAttributes));
	windowAttributes.border_pixel = 0;
	windowAttributes.background_pixmap = 0;
	windowAttributes.background_pixel = XBlackPixel(gpDisplay, visualInfo->screen);
	windowAttributes.colormap = XCreateColormap(gpDisplay, 
												XRootWindow(gpDisplay, visualInfo->screen),
												visualInfo->visual,
												AllocNone
											);
	colormap = windowAttributes.colormap;
	windowAttributes.event_mask = KeyPressMask | ButtonPressMask | FocusChangeMask | StructureNotifyMask | ExposureMask;
	// PointerMotionMask is used to get mouse move.
	// VisibilityChangeMask is used to hide and show window.

	// Create the window
	window = XCreateWindow(gpDisplay,
							XRootWindow(gpDisplay, visualInfo->screen),
							0, 0, WIN_WIDTH, WIN_HEIGHT, 0, 
							visualInfo->depth,
							InputOutput, visualInfo->visual,
							CWBorderPixel | CWBackPixel | CWEventMask | CWColormap,
							&windowAttributes
						);

	if (!window)
	{
		fprintf(gpFile, "Window creation failed\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}

	// Create Atom for Window manager to destroy the window
	windowManagerDeleteAtom = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(gpDisplay, window, &windowManagerDeleteAtom, 1);

	// Set window title
	XStoreName(gpDisplay, window, "Radhika Vishwakarma");

	// Map the window to show
	XMapWindow(gpDisplay, window);

	// Centering of Window
	screen = XScreenOfDisplay(gpDisplay, visualInfo->screen);
	screenWidth = XWidthOfScreen(screen);
	screenHeight = XHeightOfScreen(screen);

	// The initial position of the window is governed by window manager, so let the window manager decide the position. 
	// Later we will center it by XMoveWindow() xlib API
	XMoveWindow(gpDisplay, window, (screenWidth/2 - WIN_WIDTH/2), (screenHeight/2 - WIN_HEIGHT/2));
	
	// initialize
	int iResult = initialize();
	if (iResult != 0)
	{
		fprintf(gpFile, "Initialized function failed\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}
	else
	{
		fprintf(gpFile, "Initialized function completed successfully\n");
	}
	

	// Game Loop
	while (bDone == False)
	{
		while (XPending(gpDisplay))
		{
			XNextEvent(gpDisplay, &event);
			switch (event.type)
			{
				case MapNotify:
					break;
				case FocusIn:
					bActiveWindow = True;
					break;
				case FocusOut:
					bActiveWindow = True;
					break;
				case ConfigureNotify:  // similar to WM_SIZE
					resize(event.xconfigure.width, event.xconfigure.height);
					break;
				case KeyPress:
					// For Escape key
					keySym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0); // 3rd parameter for key combination and 4th parameter for "Shift" level
					switch (keySym)
					{
						case XK_Escape:
							bDone = True;
							break;
						default:
							break;
					}

					// For Alphabetic keypress
					XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL);
					switch (keys[0])
					{
						case 'F':
						case 'f':
							if (bFullScreen == False)
							{
								toggleFullScreen();
								bFullScreen = True;
							}
							else
							{
								toggleFullScreen();
								bFullScreen = False;
							}
							break;
						default:
							break;
					}

					break;
				case ButtonPress:
					break;
				case Expose: // similar to WM_PAINT
					break;
				case 33:  // similar to WM_DESTROY on Win32 SDK
					uninitialize();
					exit(EXIT_SUCCESS);
					break;
				default:
					break;
			}
		}

		// Rendering
		if (bActiveWindow == True)
		{	
			// render
			display();
			// update
			update();
		}
	}

	uninitialize();

	return 0;
}

void toggleFullScreen(void)
{
	// code
	Atom windowManagerNormalStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE", False);
	Atom windowManagerFullScreenStateAtom = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);
	XEvent event;
	memset((void*) &event, 0, sizeof(XEvent));
	event.type = ClientMessage;
	event.xclient.window = window;
	event.xclient.message_type = windowManagerNormalStateAtom;
	event.xclient.format = 32;
	event.xclient.data.l[0] = bFullScreen ? 0 : 1;
	event.xclient.data.l[1] = windowManagerFullScreenStateAtom;
	
	// send above event to XServer
	XSendEvent(gpDisplay, XRootWindow(gpDisplay, visualInfo->screen), False, SubstructureNotifyMask, &event);

}

int initialize(void)
{
	// function declaration
	void printGLInfo(void);
	void resize(int, int);
	void uninitialize(void);
	
	// Local variable declaration
	GLenum glewResult;

	// code
	// Get Function Pointer
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB((GLubyte*) "glXCreateContextAttribsARB");
	// declare attributes array
	GLint attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
		GLX_CONTEXT_MINOR_VERSION_ARB, 6,
		GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
		None
	};

	glxContext = glXCreateContextAttribsARB(gpDisplay, gLXFBConfig, 0, True, attribs);

	if (!glxContext)
	{
		// Fail safe mechanism
		GLint attribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
			GLX_CONTEXT_MINOR_VERSION_ARB, 0,
			None
		};

		glxContext = glXCreateContextAttribsARB(gpDisplay, gLXFBConfig, 0, True, attribs);

		fprintf(gpFile, "Cannot get OpenGL Context 4.6 but received lesser context.");
	}
	else
	{
		fprintf(gpFile, "Receieved OpenGL Context 4.6");
	}

	glXMakeCurrent(gpDisplay, window, glxContext);

	// Initialize GLEW
	glewResult = glewInit();
	
	if (glewResult != GLEW_OK)
	{
		fprintf(gpFile, "glewInit function failed!\n");
		return(-6);
	}

	// Print GL Info
	// printGLInfo();

	
	// VERTEX SHADER
	// 1. Write shader source code
	const GLchar *vertexShaderSourceCode = 
	"#version 460 core\n" \
	"in vec4 aPosition;\n" \
	"in vec4 aColor;\n" \
	"uniform mat4 uMVPMatrix;\n" \
	"out vec4 out_color;\n" \
	"void main(void)\n" \
	"{\n" \
	"gl_Position = uMVPMatrix * aPosition;\n" \
	"out_color = aColor;\n" \
	"}\n";

	// 2. Create the shader object
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// 3. Give the shader source code to the shader object
	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

	// 4. Compile the shader programmatically
	glCompileShader(vertexShaderObject);

	// 5. Do shader compilation error checking
	GLint status = 0;
	GLint infoLogLength = 0;
	GLchar* szInfoLog = NULL;

	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*) malloc(infoLogLength * sizeof(GLchar));
			if (szInfoLog != NULL)
			{
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, szInfoLog);
				fprintf(gpFile, "VERTEX SHADER COMPILATION LOG = %s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
			}
		}
		uninitialize();
	}
	
	// FRAGMENT SHADER
	// 1. Write shader source code
	const GLchar* fragmentShaderSourceCode = 
	"#version 460 core\n" \
	"in vec4 out_color;\n" \
	"out vec4 fragColor;\n" \
	"void main(void)\n" \
	"{\n" \
	"fragColor = out_color;\n" \
	"}\n";

	// 2. Create the shader object
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	// 3. Give the shader source code to the shader object
	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	// 4. Compile the shader programmatically
	glCompileShader(fragmentShaderObject);

	// 5. Do shader compilation error checking
	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*) malloc(infoLogLength * sizeof(GLchar));
			if (szInfoLog != NULL)
			{
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL, szInfoLog);
				fprintf(gpFile, "FRAGMENT SHADER COMPILATION LOG = %s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
			}
		}
		uninitialize();
	}

	// Create, attach, link shader program object
	shaderProgramObject = glCreateProgram();
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);\

	// Bind shader attribute at a certain index in shader to save index in host program
	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");
	glLinkProgram(shaderProgramObject);

	// Link error checking
	status = 0;
	infoLogLength = 0;
	szInfoLog = NULL;

	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			szInfoLog = (GLchar*) malloc(infoLogLength * sizeof(GLchar));
			if (szInfoLog != NULL)
			{
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, NULL, szInfoLog);
				fprintf(gpFile, "SHADER PROGRAM LINK LOG = %s\n", szInfoLog);
				free(szInfoLog);
				szInfoLog = NULL;
			}
		}
		uninitialize();
	}

	// Get the required uniform location from the shader
	mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix");

	// Provide vertex position, color, normal, texCoord etc.
	const GLfloat cube_position[] = {
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

	const GLfloat cube_color[] = {
		// front
		1.0f, 0.0f, 0.0f, // top-right of front
		1.0f, 0.0f, 0.0f, // top-left of front
		1.0f, 0.0f, 0.0f, // bottom-left of front
		1.0f, 0.0f, 0.0f, // bottom-right of front

		// right
		0.0f, 0.0f, 1.0f, // top-right of right
		0.0f, 0.0f, 1.0f, // top-left of right
		0.0f, 0.0f, 1.0f, // bottom-left of right
		0.0f, 0.0f, 1.0f, // bottom-right of right

		// back
		1.0f, 1.0f, 0.0f, // top-right of back
		1.0f, 1.0f, 0.0f, // top-left of back
		1.0f, 1.0f, 0.0f, // bottom-left of back
		1.0f, 1.0f, 0.0f, // bottom-right of back

		// left
		1.0f, 0.0f, 1.0f, // top-right of left
		1.0f, 0.0f, 1.0f, // top-left of left
		1.0f, 0.0f, 1.0f, // bottom-left of left
		1.0f, 0.0f, 1.0f, // bottom-right of left

		// top
		0.0f, 1.0f, 0.0f, // top-right of top
		0.0f, 1.0f, 0.0f, // top-left of top
		0.0f, 1.0f, 0.0f, // bottom-left of top
		0.0f, 1.0f, 0.0f, // bottom-right of top

		// bottom
		1.0f, 0.5f, 0.0f, // top-right of bottom
		1.0f, 0.5f, 0.0f, // top-left of bottom
		1.0f, 0.5f, 0.0f, // bottom-left of bottom
		1.0f, 0.5f, 0.0f, // bottom-right of bottom
	};

	// CUBE
	// VERTEX ARRAY OBJECT FOR ARRAYS OF VERTEX OBJECT
	glGenVertexArrays(1, &vao_cube);
	// Bind vertex array object
	glBindVertexArray(vao_cube);
	// POSITION
	glGenBuffers(1, &vbo_position_cube);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_position), cube_position, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// COLOR
	glGenBuffers(1, &vbo_color_cube);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// unbind VAO
	glBindVertexArray(0);

	// Depth related code
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// From here OpenGL Code starts
	// Tell OpenGL to choose the color to clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	perspectiveProjectionMatrix = mat4::identity(); // this is similar to glLoadIdentity() in resize

	// warm up resize
	resize(WIN_WIDTH, WIN_HEIGHT);

	return(0);
}

void resize(int width, int height)
{
    // code
	// if height by accident becomes 0 then make height 1
	if (height <= 0)
	{
		height = 1;
	}

	// Set the viewport
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);

	// do perspective projection
	perspectiveProjectionMatrix = vmath::perspective(45.0, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
}

void display(void)
{
	// code
	// clear OpenGL buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use shader program object
	glUseProgram(shaderProgramObject);

	// Cube
	// Transformations
	mat4 modelViewMatrix = mat4::identity(); // this is similar to glLoadIdentity() in display for model view matrix
	mat4 translationMatrix = mat4::identity();
	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f); //translate cube backwards
	mat4 rotationMatrix = mat4::identity();
	rotationMatrix = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);
	modelViewMatrix = translationMatrix * rotationMatrix;
	mat4 modelViewProjectionMatrix = mat4::identity();
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix; // order is important in matrix multiplication

	// send above matrix to the shader in "uniform"
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	// Bind with VAO
	glBindVertexArray(vao_cube);

	// Draw the vertex arrays
	glDrawArrays(GL_TRIANGLES, 0, 12);

	// Draw the vertex arrays
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	// Unbind with VAO
	glBindVertexArray(0);

	// unuse shader program object
	glUseProgram(0);

	glXSwapBuffers(gpDisplay, window);
}

void update(void)
{
	// code
	angleCube = angleCube + 0.1f;
	if(angleCube >= 360.0f)
	{
		angleCube = angleCube - 360.0f;
	}
}

void printGLInfo(void)
{
	// variable declaration
	GLint numExtensions, i;
	// code
	// print OpenGL Info
	fprintf(gpFile, "OPENGL INFORMATION\n");
	fprintf(gpFile, "--------------------\n");
	fprintf(gpFile, "OpenGL Vendor : %s\n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer : %s\n", glGetString(GL_RENDERER));
	fprintf(gpFile, "OpenGL Version : %s\n", glGetString(GL_VERSION));
	fprintf(gpFile, "GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	fprintf(gpFile, "--------------------\n");
	
	// get number of extensions
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
	
	// print OpenGL Extensions
	for (int i = 0; i < numExtensions; i++)
	{
		fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i));
	}
	
}

void uninitialize(void)
{
	GLXContext currentContext = glXGetCurrentContext();
	// code

	// Detach, delete shader objects and delete shader program object
	if (shaderProgramObject)
	{
		glUseProgram(shaderProgramObject);
		GLint numShaders;
		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numShaders);
		if (numShaders > 0)
		{
			GLuint* pShaders = (GLuint*) malloc(numShaders * sizeof(GLuint));
			if (pShaders != NULL)
			{
				glGetAttachedShaders(shaderProgramObject, numShaders, NULL, pShaders);
				for (GLint i = 0; i < numShaders; i++)
				{
					glDetachShader(shaderProgramObject, pShaders[i]);
					glDeleteShader(pShaders[i]);
					pShaders[i] = 0;
				}
			}
			free(pShaders);
			pShaders = 0;
		}
		glUseProgram(0);
		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;
	}

	if (currentContext && glxContext == glxContext)
	{
		glXMakeCurrent(gpDisplay, 0, 0);
	}

	if (glxContext)
	{
		glXDestroyContext(gpDisplay, glxContext);
		glxContext = NULL;
	}

	if (visualInfo)
	{
		free(visualInfo);
		visualInfo = NULL;
	}

	if (window)
	{
		XDestroyWindow(gpDisplay, window);
	}
	
	if (colormap)
	{
		XFreeColormap(gpDisplay, colormap);
	}

	if (gpDisplay)
	{
		XCloseDisplay(gpDisplay);
		gpDisplay = NULL;
	}

	// Close the file
	if (gpFile)
	{
		fprintf(gpFile, "Program terminated successfully\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}