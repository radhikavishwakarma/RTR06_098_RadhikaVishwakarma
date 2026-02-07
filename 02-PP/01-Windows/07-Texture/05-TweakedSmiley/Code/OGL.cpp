// Win32 Header files
#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>

// OpenGL related Header files
#include <GL/glew.h> // This header file must be included befor gl/GL.h
#include<gl/GL.h>

// Custom Header file
#include "OGL.h"
#include "vmath.h"

using namespace vmath;

// OpenGL related libraries
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declaration
// variables related to fullscreen
BOOL gbFullScreen = FALSE;
HWND ghwnd = NULL;
DWORD dwStyle;
WINDOWPLACEMENT wpPrev;

// Active window related variables
BOOL gbActiveWindow = FALSE;

// Exit key press related
BOOL gbEscapeKeyIsPressed = FALSE; 

// variables related to FILE IO
char gszLogFileName[] = "Log.txt";
FILE *gpFile = NULL;

// OpenGL related global variables
HDC ghdc = NULL; // Handle device context
HGLRC ghrc = NULL; // Handle to graphics rendering context 

// Shader related global variables
GLuint shaderProgramObject = 0;

enum{
	AMC_ATTRIBUTE_POSITION = 0,
	AMC_ATTRIBUTE_COLOR,
	AMC_ATTRIBUTE_TEXCOORD,
};

GLuint vao_rectangle = 0;
GLuint vbo_position_rectangle;
GLuint vbo_texcoord_rectangle = 0;

GLuint mvpMatrixUniform = 0;

mat4 perspectiveProjectionMatrix; 

// taxture related global variables
GLuint texture_smiley;
GLuint textureSamplerUniform;
GLuint64 keyPressUniform = 0;

static int KeyPressed = 0;

// Entry Point Function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// function declaration
	int initialize(void);
	void display(void);
	void update(void);
	void uninitialize(void);

	// variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("RTR6");
	BOOL bDone = FALSE;

	int smX = GetSystemMetrics(SM_CXSCREEN);
	int smY = GetSystemMetrics(SM_CYSCREEN);

	int x = smX / 2 - WIN_WIDTH/2;
	int y = smY / 2 - WIN_HEIGHT/2;

	// code
	// Create log file
	gpFile = fopen(gszLogFileName, "w");
	if (gpFile == NULL)
	{
		MessageBox(hwnd, TEXT("Log file creation failed"), TEXT("FILE IO ERROR"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Program started successfully\n");
	}
	
	// Window Class Initialization
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// Registration of Window Class
	RegisterClassEx(&wndclass);

	// Create Window
	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		szAppName,
		TEXT("Radhika Vishwakarma"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		x,  // update as homework
		y,  // update as homework
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	
	ghwnd = hwnd;

	// Show Window
	ShowWindow(hwnd, iCmdShow);

	// Point Background & Window
	UpdateWindow(hwnd);
	
	// initialize
	int result = initialize();
	
	if (result != 0)
	{
		fprintf(gpFile, "Initialized function failed\n");
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
	else
	{
		fprintf(gpFile, "Initialized function completed successfully\n");
	}

	// Set this window as foreground and active window
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	

	// Game Loop
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);	
			}
		}
		else
		{
			if (gbActiveWindow == TRUE)
			{
				if (gbEscapeKeyIsPressed == TRUE)
				{
					bDone = TRUE;
				}
				// render
				display();

				// update
				update();
			}
		}
	}

	// uninitialize
	uninitialize();

	return((int)msg.wParam);
}

// Callback Function
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declarations
	void toggleFullScreen(void);
	void resize(int, int);
	void uninitialize(void);

	// code
	switch (iMsg)
	{
		case WM_CREATE:
			ZeroMemory((void*) &wpPrev, sizeof(WINDOWPLACEMENT));
			break;
		case WM_SETFOCUS:
			gbActiveWindow = TRUE;
			break;
		case WM_KILLFOCUS:
			gbActiveWindow = FALSE;
			break;
		case WM_ERASEBKGND:
			return(0);
		case WM_SIZE:
			resize(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_ESCAPE:
					gbEscapeKeyIsPressed = TRUE;
					break;
				
				default:
					break;
			}
			break;
		case WM_CHAR:
			switch (wParam)
			{
				case 'F':
				case 'f':
					if (gbFullScreen == FALSE)
					{
						toggleFullScreen();
						gbFullScreen = TRUE;
					}
					else
					{
						toggleFullScreen();
						gbFullScreen = FALSE;
					}
					break;
				case '1':
				case VK_NUMPAD1:
					KeyPressed = 1;
					glEnable(GL_TEXTURE_2D);
					break;
				case '2':
				case VK_NUMPAD2:
					KeyPressed = 2;
					glEnable(GL_TEXTURE_2D);
					break;
				case '3':
				case VK_NUMPAD3:
					KeyPressed = 3;
					glEnable(GL_TEXTURE_2D);
					break;
				case '4':
				case VK_NUMPAD4:
					KeyPressed = 4;
					glEnable(GL_TEXTURE_2D);
					break;
				default:
					KeyPressed = 5;
					glDisable(GL_TEXTURE_2D);
					break;
			}
			break;
		case WM_CLOSE:
			uninitialize();
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			break;
	}

	return(DefWindowProc(hWnd, iMsg, wParam, lParam));
}

// functions which don't have body are called as stub functions
void toggleFullScreen(void)
{
	// Variable declarations
	MONITORINFO mi;
	if(gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if(dwStyle & WS_OVERLAPPEDWINDOW)
		{
			ZeroMemory((void*) &mi, sizeof(MONITORINFO));
			mi.cbSize = sizeof(MONITORINFO);

			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				int monitorWidth = mi.rcMonitor.right - mi.rcMonitor.left;
				int monitorHeight = mi.rcMonitor.bottom - mi.rcMonitor.top;
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, monitorWidth, monitorHeight, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
	}
	else
	{
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
	ShowCursor(TRUE);
}

int initialize(void)
{
	// function declaration
	void resize(int, int);
	void printGLInfo(void);
	void uninitialize(void);
	BOOL loadGLTexture(GLuint*, TCHAR[]);

	// local variable declaration
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	GLenum glewResult;

    // code
	// PixelFormatDescriptor initialization
	ZeroMemory((void*) &pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	// Get DC
	ghdc = GetDC(ghwnd);

	if (ghdc == NULL)
	{
		fprintf(gpFile, "Get DC function failed!\n");
		return(-1);
	}

	// Get matching pixel format index using hdc and pfd
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	
	if (iPixelFormatIndex == 0)
	{
		fprintf(gpFile, "Choose pixel format failed!\n");
		return(-2);
	}
	
	// Select the pixel format of found index
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFile, "Set pixel format function failed!\n");
		return(-3);
	}
	
	// Create rendering context using hdc, pfd and chosen pixel format index
	ghrc = wglCreateContext(ghdc);	// function from Bridging API
	if (ghdc == NULL)
	{
		fprintf(gpFile, "wglCreateContext function failed!\n");
		return(-4);
	}

	// Make this rendering context as current context
	if (wglMakeCurrent(ghdc,ghrc) == FALSE)
	{
		fprintf(gpFile, "wglMakeCurrent function failed!\n");
		return(-5);
	}

	// Initialize GLEW
	glewResult = glewInit();
	
	if (glewResult != GLEW_OK)
	{
		fprintf(gpFile, "glewInit function failed!\n");
		return(-6);
	}

	printGLInfo();

	// VERTEX SHADER
	// 1. Write shader source code
	const GLchar *vertexShaderSourceCode = 
	"#version 460 core\n" \
	"in vec4 aPosition;\n" \
	"in vec2 aTexCoord;\n" \
	"uniform mat4 uMVPMatrix;\n" \
	"out vec2 out_texCoord;\n" \
	"void main(void)\n" \
	"{\n" \
	"gl_Position = uMVPMatrix * aPosition;\n" \
	"out_texCoord = aTexCoord;\n" \
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
	"in vec2 out_texCoord;\n" \
	"uniform int uKeyPress;\n" \
	"uniform sampler2D uTextureSampler;\n" \
	"out vec4 fragColor;\n" \
	"void main(void)\n" \
	"{\n" \
		"if((uKeyPress != 1) && (uKeyPress != 2) && (uKeyPress != 3) && (uKeyPress != 4))" \
		"{\n" \
			"fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n" \
		"}\n" \
		"else\n" \
		"{\n" \
			"fragColor = texture(uTextureSampler, out_texCoord);\n" \
		"}\n" \
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
	glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");
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
	textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "uTextureSampler");
	keyPressUniform = glGetUniformLocation(shaderProgramObject, "uKeyPress");

	// Provide vertex position, color, normal, texCoord etc.
	const GLfloat rectangle_position[] = {
		1.0f, 1.0f, 0.0f, // Top right
		-1.0f, 1.0f, 0.0f, // Top left
		-1.0f, -1.0f, 0.0f, // Bottom left
		1.0f, -1.0f, 0.0f // Bottom right
	};

	// RECTANGLE
	// VERTEX ARRAY OBJECT FOR ARRAYS OF VERTEX OBJECT
	glGenVertexArrays(1, &vao_rectangle);
	// Bind vertex array object
	glBindVertexArray(vao_rectangle);
	// POSITION
	glGenBuffers(1, &vbo_position_rectangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position_rectangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_position), rectangle_position, GL_STATIC_DRAW);
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// TEXCOORDS
	glGenBuffers(1, &vbo_texcoord_rectangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoord_rectangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 4 * 2, NULL, GL_DYNAMIC_DRAW);  // Dynamic drawing
	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);
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

	// load textures
	if (loadGLTexture (& texture_smiley, MAKEINTRESOURCE (IDBITMAP_SMILEY)) == FALSE)
	{
		fprintf(gpFile, "loadGLTexture smiley texture failed \n");
		return (-7);
	}

	perspectiveProjectionMatrix = mat4::identity(); // this is similar to glLoadIdentity() in resize
	
	// warm up resize
	resize(WIN_WIDTH, WIN_HEIGHT);

    return(0);
}

BOOL loadGLTexture(GLuint * texture, TCHAR imageResourceID[])
{
	// variables declartaions
	HBITMAP hBitmap = NULL;
	BITMAP bmp;
	BOOL bResult = FALSE;

	// code
	// load the bitmape as the image
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION); // Create Device Independent Bitmap (DIB Section)
	if (hBitmap)
	{
		bResult = TRUE;

		// get bitmape struture from the loaded bitmape image
		GetObject(hBitmap, sizeof(BITMAP), &bmp);

		// genertae opengl texture objaect
		glGenTextures(1, texture);

		// bind to the newly created emty
		glBindTexture(GL_TEXTURE_2D, *texture);

		// unpack the image in memory for faster loading
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		DeleteObject(hBitmap);
		hBitmap = NULL;
	}
	return bResult;
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

	// CUBE
	// Transformations
	mat4 modelViewMatrix = mat4::identity(); // this is similar to glLoadIdentity() in display for model view matrix
	mat4 translationMatrix = mat4::identity();
	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f); //translate
	mat4 scaleMatrix = mat4::identity();
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	modelViewMatrix = translationMatrix * scaleMatrix;
	mat4 modelViewProjectionMatrix = mat4::identity();
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix; // order is important in matrix multiplication

	// send above matrix to the shader in "uniform"
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	// For Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_smiley);
	glUniform1i(textureSamplerUniform, 0);

	// Bind with VAO
	glBindVertexArray(vao_rectangle);

	GLfloat rectangle_texCoords[8];
	
	if (KeyPressed == 1)
	{
		glUniform1i(keyPressUniform, 1);
		rectangle_texCoords[0] = 0.5f;
		rectangle_texCoords[1] = 0.5f;
		rectangle_texCoords[2] = 0.0f;
		rectangle_texCoords[3] = 0.5f;
		rectangle_texCoords[4] = 0.0f;
		rectangle_texCoords[5] = 0.0f;
		rectangle_texCoords[6] = 0.5f;
		rectangle_texCoords[7] = 0.0f;
	}
	else if(KeyPressed == 2)
	{
		glUniform1i(keyPressUniform, 2);
		rectangle_texCoords[0] = 1.0f;
		rectangle_texCoords[1] = 1.0f;
		rectangle_texCoords[2] = 0.0f;
		rectangle_texCoords[3] = 1.0f;
		rectangle_texCoords[4] = 0.0f;
		rectangle_texCoords[5] = 0.0f;
		rectangle_texCoords[6] = 1.0f;
		rectangle_texCoords[7] = 0.0f;
	}
	else if(KeyPressed == 3)
	{
		glUniform1i(keyPressUniform, 3);
		rectangle_texCoords[0] = 2.0f;
		rectangle_texCoords[1] = 2.0f;
		rectangle_texCoords[2] = 0.0f;
		rectangle_texCoords[3] = 2.0f;
		rectangle_texCoords[4] = 0.0f;
		rectangle_texCoords[5] = 0.0f;
		rectangle_texCoords[6] = 2.0f;
		rectangle_texCoords[7] = 0.0f;
	}
	else if(KeyPressed == 4)
	{
		glUniform1i(keyPressUniform, 4);
		rectangle_texCoords[0] = 0.2f;
		rectangle_texCoords[1] = 0.2f;
		rectangle_texCoords[2] = 0.2f;
		rectangle_texCoords[3] = 0.2f;
		rectangle_texCoords[4] = 0.2f;
		rectangle_texCoords[5] = 0.2f;
		rectangle_texCoords[6] = 0.2f;
		rectangle_texCoords[7] = 0.2f;
	}
	else
	{
		glUniform1i(keyPressUniform, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoord_rectangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_texCoords), rectangle_texCoords, GL_DYNAMIC_DRAW);  // Dynamic drawing
	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Draw the vertex arrays
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	// Unbind Texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// Unbind with VAO
	glBindVertexArray(0);
	
	// unuse shader program object
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void update(void)
{
	// code
}

void uninitialize(void) {
	// function declarations
	void toggleFullScreen(void);

    // code
	// If user is exiting in fullscreen then return fullscreen back to normal
	if(gbFullScreen == TRUE)
	{
		toggleFullScreen();
		gbFullScreen = FALSE;
	}

	// Free vbo_texcoord_rectangle
	if (vbo_texcoord_rectangle)
	{
		glDeleteBuffers(1, &vbo_texcoord_rectangle);
		vbo_position_rectangle = 0;
	}

	// Free vbo_position_rectangle
	if (vbo_position_rectangle)
	{
		glDeleteBuffers(1, &vbo_position_rectangle);
		vbo_position_rectangle = 0;
	}
	
	// Free vao_rectangle
	if (vao_rectangle)
	{
		glDeleteVertexArrays(1, &vao_rectangle);
		vao_rectangle = 0;
	}

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
	

	// Make hdc as current context by releasing rendering context as current context
	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}
	
	// Delete the rendering context
	if(ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	// Release DC
	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
	}

	// Destroy window
	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;
	}
	
	
	// Close the file
	if (gpFile)
	{
		fprintf(gpFile, "Program terminated successfully\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}