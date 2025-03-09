	// Win32 Headers files
	#include<Windows.h>
	#include<stdio.h>
	#include<stdlib.h>

	// open gl related header files
	#include<gl/GL.h>

	// Custom Header file
	#include "OGL.h"

	// OpenGL related libraries
	#pragma comment(lib, "opengl32.lib")

	// Macros
	#define WIN_WIDTH 800
	#define WIN_HEIGHT 600

	// global function declarations
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	// Global variable declaration
	// variables related to fullscreen
	BOOL gbFullScreen = FALSE;
	HWND ghwnd = NULL;
	DWORD dwStyle;
	WINDOWPLACEMENT wpPrev;

	// Active window Related variables
	BOOL gbActiveWindow = FALSE;

	// Exit key press related variable
	BOOL gbEscapeKeyIsPressed = FALSE;  

	// variable related with File I/O
	char gszLogFileName[] = "Log.txt";
	FILE *gpFile = NULL;

	// openGL related global variables
	HDC ghdc = NULL; // Handle device context
	HGLRC ghrc = NULL; // Handle to graphics rendering context

	// Entry Point Function
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
	{
		// local function declaration
		int initialize(void);
		void display(void);
		void update(void);
		void uninitialize(void);

		// Variable declaration 
		WNDCLASSEX wndclass;
		HWND hwnd;
		MSG msg;
		TCHAR szAppName[] = TEXT("RTR6");
		BOOL bDone = FALSE;

		int smX = GetSystemMetrics(SM_CXSCREEN);
		int smY = GetSystemMetrics(SM_CYSCREEN);

		int x = smX / 2 - WIN_WIDTH / 2;
		int y = smY / 2 - WIN_HEIGHT / 2;
		
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
			fprintf(gpFile, "Program started successfull\n");
		}

		// code
		// Window Class Initialization
		wndclass.cbSize = sizeof(WNDCLASSEX);
		wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.lpfnWndProc = WndProc;
		wndclass.hInstance = hInstance;
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
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
			x, // update as homework
			y, // update as homework
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

		// Initialize
		int result = initialize();

		if(result != 0)
		{
			fprintf(gpFile, "Initialize function failed\n");
			DestroyWindow(hwnd);
			hwnd = NULL;	
		}
		else
		{
			fprintf(gpFile, "Initialize function compeleted successfully\n");
		}

		// set this window as for ground and active window
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
					// Render
					display();

					// Update
					update();	
				}	
			}
		}

		// Uninitialize
		uninitialize();

		return((int)msg.wParam);
	}

	// Callback Function
	LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
	{
		// local function declaration
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
					if (gbFullScreen == FALSE){
						toggleFullScreen();
						gbFullScreen = TRUE;
					}
					else{
						toggleFullScreen();
						gbFullScreen = FALSE;
					}
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

	// functions which dont have body are called as stub functions
	void toggleFullScreen(void)
	{
		// Variable Declarations
		MONITORINFO mi;
		if(gbFullScreen == FALSE)
		{
			dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
			if(dwStyle & WS_OVERLAPPEDWINDOW)
			{
				ZeroMemory((void*) &mi, sizeof(MONITORINFO));
				mi.cbSize = sizeof(MONITORINFO);

				if(GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
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
			SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
		}
		ShowCursor(TRUE);
	}

	int initialize(void) 
	{
		// variable declaration
		PIXELFORMATDESCRIPTOR pfd;
		int iPixelFormatIndex = 0;

		// code
		// pixel format descrptior intialization
		ZeroMemory((void*) &pfd, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |  PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cRedBits = 8;
		pfd.cGreenBits = 8;
		pfd.cBlueBits = 8;
		pfd.cAlphaBits = 8;
		

	// get dc
	ghdc = GetDC(ghwnd);

	if (ghdc == NULL)
	{
		fprintf(gpFile, "GetDc function failed!\n");
		return(-1);
	}

	// Get matching pixel format index using hdc and pfd
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

	if (iPixelFormatIndex == 0)
	{
		fprintf(gpFile, "Choose Pixel Format failed!\n");
		return(-2);
	}

	// select the pixel format of the found index
	if (SetPixelFormat(ghdc, iPixelFormatIndex , &pfd) == FALSE)
	{
		fprintf(gpFile, "set Pixel Format failed!\n");
		return(-3);
	}

	// create randring context by using hdc, pfd and chosen pixel format index
	ghrc = wglCreateContext(ghdc); // function from Bridging API
	if (ghdc == NULL)
	{
		fprintf(gpFile, "wglCreateContext function failed!\n");
		return(-4);
	}

	// make this rendering contaxt as current context
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		fprintf(gpFile, "wglMakeCurrent function failed\n");
		return(-5);
	}

	//  from here onwords opengl code starts
	// Tell openGL to choose the color to clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

	// Set )the viewport
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);

	}

	void display(void)
	{
		// code
			// Clear OpenGL buffers
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw triangle here
		glBegin(GL_TRIANGLES);
		// Apex
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		// left bottom
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		// right bottom
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glEnd();

		SwapBuffers(ghdc);
	}

	void update(void)
	{
		// code
	}

	void uninitialize(void)
	{   
		// Function declartion
		void toggleFullScreen(void);
		
		
		// code
			// if user is exiting in fullscreen then return fullscreen back to normal
		if(gbFullScreen == TRUE)
		{
			toggleFullScreen();
			gbFullScreen = FALSE;
		} 

		// make hdc as current context by releasing rendering context as current context
		if (wglGetCurrentContext == ghrc)
		{
			wglMakeCurrent(NULL, NULL);
		}

		// deleted the rendering context
		if (ghrc)
		{
			wglDeleteContext(ghrc);
			ghrc = NULL;
		}

		// release the DC
		if(ghdc)
		{
			ReleaseDC(ghwnd, ghdc);
			
		}

		// Destroy Window
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

