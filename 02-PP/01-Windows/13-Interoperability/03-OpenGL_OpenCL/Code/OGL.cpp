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

// OpenCL headers
#define CL_TARGET_OPENCL_VERSION 300
#include <CL/opencl.h>

#pragma comment(lib, "opencl.lib")


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
};

GLuint vao = 0;
GLuint vbo_cpu;

GLuint mvpMatrixUniform = 0;

mat4 perspectiveProjectionMatrix; 

// Sine wave related variables
const unsigned int gMeshWidth = 1024;
const unsigned int gMeshHeight = 1024;
const unsigned int gMeshDepth = 4;

#define MESH_ARRAY_SIZE gMeshHeight * gMeshWidth * gMeshDepth

float position[gMeshWidth][gMeshHeight][gMeshDepth]; 
GLuint vbo_gpu;
float animationTime = 0.0f;

// openCL related global variables
cl_int oclResult;
cl_mem oclGraphicsResource = NULL;
cl_device_id oclDeviceID;
cl_context oclContext = NULL;
cl_command_queue oclCommandQueue = NULL;
cl_program oclProgram = NULL;
cl_kernel oclKernel = NULL;

BOOL bOnGPU = FALSE;



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
				case 'C': 
				case 'c':
					bOnGPU = FALSE;
					break;
				case 'G': 
				case 'g':
					bOnGPU = TRUE;
					break;
				default:
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

	// printGLInfo();

	// OpenCL related initiliazation
	cl_platform_id oclPlatformID;
	cl_device_id* oclDeviceIDs = NULL;
	cl_uint devCount = 0;

	// Step 1 : Get platform id
	oclResult = clGetPlatformIDs(1, &oclPlatformID, NULL);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clGetPlatformIDs failed!\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}

	oclResult = clGetDeviceIDs(oclPlatformID, CL_DEVICE_TYPE_GPU, 0, NULL, &devCount);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "First call to clGetDeviceIDs failed!\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}
	else if(devCount == 0)
    {
        fprintf(gpFile, "clGetDeviceIDs succeeded but NO openCL supported device on this system!\n");
		uninitialize();
		exit(EXIT_FAILURE);
    }

	// Step 2: Get openGL supporte
	oclDeviceIDs = (cl_device_id*) malloc(sizeof(cl_device_id) * devCount);

	// Step 3 : Now fill our device array by calling clGetDeviceIDs() function again
	oclResult = clGetDeviceIDs(oclPlatformID, CL_DEVICE_TYPE_GPU, devCount, oclDeviceIDs, NULL);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "Second call to clGetDeviceIDs : %d\n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);
	}

	// step 4 : From the above array select the 0th device
	oclDeviceID = oclDeviceIDs[0];

	// Setp 5 : Free the memory 
	free(oclDeviceIDs);

	// Step 6 : Initiliaze OpenCL context properties arry
	cl_context_properties oclContextProperties[] =
	{
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC,
		CL_CONTEXT_PLATFORM, (cl_context_properties)oclPlatformID, 
	};

	// Step 7 :Create openCL context
	oclContext = clCreateContext(oclContextProperties, 1, &oclDeviceID, NULL, NULL, &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateContext() failed : %d\n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);	
	}
	
	// Step 8: Create OCL Command Queue
	oclCommandQueue = clCreateCommandQueueWithProperties(oclContext, oclDeviceID, 0, &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateCommandQueueWithProperties() failed : %d\n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);
	}

	// Step : 9
	const char* oclKernelSourceCode =
	"__kernel void sineWaveKernel(__global float4* pos, unsigned int width, unsigned int height, float time)" \
	"{" \
		// Code
		"unsigned int i = get_global_id(0);" \
		"unsigned int j = get_global_id(1);"

		"float u = (float)i / (float)width;" \
		"float v = (float)j / (float)height;" \
		
		"u = u * 2.0f - 1.0f;" \
		"v = v * 2.0f - 1.0f;" \

		"float frequency = 4.0f;" \

		"float w = sin( u * frequency + time) * cos(v * frequency + time);" \

		"pos[j * width + i] = (float4)(u, w, v, 1.0f);" \
	"}";

	// Step 10 : Create openCL program avove kernel source code
	oclProgram = clCreateProgramWithSource(oclContext, 1, (const char**)&oclKernelSourceCode, NULL, &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateProgramWithSource() failed : %d\n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);	
	}

	// Step 11 : Build above OCL program
	oclResult = clBuildProgram(oclProgram, 0, NULL, "-cl-fast-relaxed-math", NULL, NULL);
	if (oclResult != CL_SUCCESS)
	{
		cl_int oclBuildResult;
		char szBuffer[512];
		oclBuildResult = clGetProgramBuildInfo(oclProgram, oclDeviceID, CL_PROGRAM_BUILD_LOG, sizeof(szBuffer), szBuffer, NULL);
		if (oclBuildResult != CL_SUCCESS)
		{
			/* code */
			fprintf(gpFile, "clBuildProgram() failed : %d\n", oclResult);
			uninitialize();
			exit(EXIT_FAILURE);	
		}
		fprintf(gpFile, "Program build log : %s\n", szBuffer);
		uninitialize();
		exit(EXIT_FAILURE);		
	}

	// Step 12 : Create OpenCL Kernel
	oclKernel = clCreateKernel(oclProgram, "sineWaveKernel", &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateKernel() failed : %d\n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);
	}
	
	// VERTEX SHADER
	// 1. Write shader source code
	const GLchar *vertexShaderSourceCode = 
	"#version 460 core\n" \
	"in vec4 aPosition;\n" \
	"uniform mat4 uMVPMatrix;\n" \
	"void main(void)\n" \
	"{\n" \
	"gl_Position = uMVPMatrix * aPosition;\n" \
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
	"out vec4 fragColor;\n" \
	"void main(void)\n" \
	"{\n" \
	"fragColor = vec4(0.5f, 0.5f, 0.0f, 1.0f);\n" \
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
	for (int i = 0; i < (int)gMeshWidth; i++)
	{
		for (int j = 0; j < (int)gMeshHeight; j++)
		{
			for (int k = 0; k < (int)gMeshDepth; k++)
			{
				position[i][j][k] = 0.0f;
			}
		}
	}
	

	// VERTEX ARRAY OBJECT FOR ARRAYS OF VERTEX OBJECT
	glGenVertexArrays(1, &vao);
	// Bind vertex array object
	glBindVertexArray(vao);
	// For CPU
	glGenBuffers(1, &vbo_cpu);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cpu);
	glBufferData(GL_ARRAY_BUFFER, MESH_ARRAY_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW); // here we did dynamic draw for multiple resolution sine waves
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// For GPU
	glGenBuffers(1, &vbo_gpu);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_gpu);
	glBufferData(GL_ARRAY_BUFFER, MESH_ARRAY_SIZE * sizeof(float), NULL, GL_DYNAMIC_DRAW); // here we did dynamic draw for multiple resolution sine waves
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

	// Create OpenCL graphics resource
	oclGraphicsResource = clCreateFromGLBuffer(oclContext, CL_MEM_WRITE_ONLY, vbo_gpu, &oclResult);
	if (oclResult != CL_SUCCESS)
	{
		fprintf(gpFile, "clCreateFromGLBuffer() failed : %d\n", oclResult);
		uninitialize();
		exit(EXIT_FAILURE);
	}
	perspectiveProjectionMatrix = mat4::identity(); // this is similar to glLoadIdentity() in resize
	
	// warm up resize
	resize(WIN_WIDTH, WIN_HEIGHT);

    return(0);
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
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void display(void)
{
	// function declarations
	void sineWave(unsigned int, unsigned int, float);
	void uninitialize(void);

    // code
	// clear OpenGL buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use shader program object
	glUseProgram(shaderProgramObject);

	// Transformations
	mat4 modelViewMatrix = mat4::identity(); // this is similar to glLoadIdentity() in display for model view matrix
	// mat4 translationMatrix = mat4::identity();
	// translationMatrix = vmath::translate(0.0f, 0.0f, -5.0f); // translate triangle backwards
	// modelViewMatrix = translationMatrix;
	mat4 modelViewProjectionMatrix = mat4::identity();
	modelViewProjectionMatrix = perspectiveProjectionMatrix * modelViewMatrix; // order is important in matrix multiplication

	// send above matrix to the shader in "uniform"
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, modelViewProjectionMatrix);

	// Bind with VAO
	glBindVertexArray(vao);

	if (bOnGPU == TRUE)
	{
		// Set 0th kernel argument
		oclResult = clSetKernelArg(oclKernel, 0, sizeof(cl_mem), (void*)&oclGraphicsResource);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clSetKernelArg() failed to set 0th argument: %d\n", oclResult);
			uninitialize();
			exit(EXIT_FAILURE);
		}
		// Set 1st kernel argument
		oclResult = clSetKernelArg(oclKernel, 1, sizeof(unsigned int), &gMeshWidth);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clSetKernelArg() failed to set 1st argument: %d\n", oclResult);
			uninitialize();
			exit(EXIT_FAILURE);
		}

		// Set 2nd kernel argument
		oclResult = clSetKernelArg(oclKernel, 2, sizeof(unsigned int), &gMeshHeight);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clSetKernelArg() failed to set 2nd argument: %d\n", oclResult);
			uninitialize();
			exit(EXIT_FAILURE);
		}

		// Set 3rd kernel argument
		oclResult = clSetKernelArg(oclKernel, 3, sizeof(float), &animationTime);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clSetKernelArg() failed to set 3rd argument: %d\n", oclResult);
			uninitialize();
			exit(EXIT_FAILURE);
		}
		// acquire OpenCL Graphic Resource
		oclResult = clEnqueueAcquireGLObjects(oclCommandQueue, 1, &oclGraphicsResource, 0, NULL, NULL);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clEnqueueAcquireGLObjects() failed : %d\n", oclResult);
			uninitialize();
			exit(EXIT_FAILURE);
		}

		// call OpenCL kernel
		size_t globalWorkSize[2];
		globalWorkSize[0] = gMeshWidth;
		globalWorkSize[1] = gMeshHeight;

		size_t localWorkSize[2];
		localWorkSize[0] = 8;
		localWorkSize[1] = 8;

		oclResult = clEnqueueNDRangeKernel(oclCommandQueue, oclKernel, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clEnqueueNDRangeKernel() Failed : %d\n", oclResult);
			uninitialize();
			exit(EXIT_FAILURE);
		}

		// Release OpenCL Graphic Resource
		oclResult = clEnqueueReleaseGLObjects(oclCommandQueue, 1, &oclGraphicsResource, 0, NULL, NULL);
		if (oclResult != CL_SUCCESS)
		{
			fprintf(gpFile, "clEnqueueReleaseGLObjects() failed : %d\n", oclResult);
			uninitialize();
			exit(EXIT_FAILURE);
		}

		// Finish the command queue
		clFinish(oclCommandQueue);
		 
		// Bind buffer
		glBindBuffer(GL_ARRAY_BUFFER, vbo_gpu);
	}
	else
	{
		// Draw sine wave
		sineWave(gMeshWidth, gMeshHeight, animationTime);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_cpu);
		glBufferData(GL_ARRAY_BUFFER, MESH_ARRAY_SIZE * sizeof(float), position, GL_DYNAMIC_DRAW);
	}
	
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

	glDrawArrays(GL_POINTS, 0, gMeshWidth * gMeshHeight);

	// Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO
	glBindVertexArray(0);

	// unuse shader program object
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void sineWave(unsigned int mesh_width, unsigned int mesh_height, float time)
{
	// code
	for (int i = 0; i < (int)gMeshWidth; i++)
	{
		for (int j = 0; j < (int)gMeshHeight; j++)
		{
			for (int k = 0; k < (int)gMeshDepth; k++)
			{
				float u = (float)i / (float)mesh_width;
				float v = (float)j / (float)mesh_height;
				
				u = u * 2.0f - 1.0f;  // normalized the values
				v = v * 2.0f - 1.0f;  // normalized the values

				float frequency = 4.0f; // can experiment with this value

				float w = sin( u * frequency + time) * cos(v * frequency + time); // sine wave equation

				if (k == 0)
				{
					position[i][j][k] = u;
				}
				if (k == 1)
				{
					position[i][j][k] = w;
				}
				if (k == 2)
				{
					position[i][j][k] = v;
				}
				if (k == 3)
				{
					position[i][j][k] = 1.0f;
				}
			}
		}
	}
}

void update(void)
{
	// code
	animationTime += 0.01;
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

	// Free vbo_cpu
	if (vbo_cpu)
	{
		glDeleteBuffers(1, &vbo_cpu);
		vbo_cpu = 0;
	}

	if (vbo_gpu)
	{
		if (oclGraphicsResource)
		{
			clReleaseMemObject(oclGraphicsResource);
			oclGraphicsResource = NULL;
		}
		glDeleteBuffers(1, &vbo_gpu);
		vbo_gpu = 0;
	}
	
	if (oclKernel)
	{
		clReleaseKernel(oclKernel);
		oclKernel = NULL;
	}
	
	if (oclProgram)
	{
		clReleaseProgram(oclProgram);
		oclProgram = NULL;
	}

	if (oclCommandQueue)
	{
		clReleaseCommandQueue(oclCommandQueue);
		oclCommandQueue = NULL;
	}
	
	if (oclContext)
	{
		clReleaseContext(oclContext);
		oclContext = NULL;
	}
		
	// Free VAO
	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
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