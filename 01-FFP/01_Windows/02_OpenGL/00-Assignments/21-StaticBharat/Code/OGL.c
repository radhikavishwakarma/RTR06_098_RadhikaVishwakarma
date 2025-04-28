// Win32 Header files
#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

// OpenGL related Header files
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
	// local variable declaration
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

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
	
	// From here OpenGL Code starts
	// Tell OpenGL to choose the color to clear the screen
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

	// Set the viewport
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
}

void display(void)
{
	// function declaration
	void drawB(void);
	void drawH(void);
	void drawA1(void);
	void drawR(void);
	void drawA2(void);
	void drawT(void);

    // code
	// clear OpenGL buffers
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	
	// Set to identity matrix
   	glLoadIdentity();

	// Draw here
	drawB();
	drawH();
	drawA1();
	drawR();
	drawA2();
	drawT();

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

void drawB(void) 
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(-2.9f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glColor4f(1.0f, 0.404f, 0.122f, 1.0f);
    glVertex3f(0.5, 0.85f, 0.0f);
    glVertex3f(0.35, 1.0f, 0.0f);
    glVertex3f(-0.5, 1.0f, 0.0f);

    glColor4f(1.0f, 0.583f, 0.385f, 1.0f);
    glVertex3f(-0.5, 0.7f, 0.0f);
    glVertex3f(0.5f, 0.7f, 0.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor4f(1.0f, 0.583f, 0.385f, 1.0f);
    glVertex3f(-0.2f, 0.7f, 0.0f);
    glVertex3f(-0.5f, 0.7f, 0.0f);
   
    glColor4f(1.0f, 0.911f, 0.868f, 1.0f);
    glVertex3f(-0.5, 0.15f, 0.0f);
    glVertex3f(-0.2, 0.15f, 0.0f);

    //Upper Right Vertical Strip
    glColor4f(1.0f, 0.583f, 0.385f, 1.0f);
    glVertex3f(0.5f, 0.7f, 0.0f);
    glVertex3f(0.2f, 0.7f, 0.0f);

    glColor4f(1.0f, 0.911f, 0.868f, 1.0f);
    glVertex3f(0.2f, 0.15f, 0.0f);
    glVertex3f(0.5f, 0.15f, 0.0f);

    //Lower Left Vertical Strip
    glColor4f(0.852, 0.912f, 0.867f, 1.0f);
    glVertex3f(-0.2f, -0.15f, 0.0f);
    glVertex3f(-0.5f, -0.15f, 0.0f);

    glColor4f(0.311f, 0.591f, 0.454f, 1.0f);
    glVertex3f(-0.5f, -0.7f, 0.0f);
    glVertex3f(-0.2f, -0.7f, 0.0f);

    //Lower Right Vertical Strip
    glColor4f(0.852f, 0.912f, 0.887f, 1.0f);
    glVertex3f(0.5f, -0.15f, 0.0f);
    glVertex3f(0.2f, -0.15f, 0.0f);

    glColor4f(0.311f, 0.591f, 0.454f, 1.0f);
    glVertex3f(0.2f, -0.7f, 0.0f);
    glVertex3f(0.5f, -0.7f, 0.0f);

    //Middle horizontal strip
    glColor4f(1.0f, 0.911f, 0.868f, 1.0f);
    glVertex3f(0.5f, 0.15f, 0.0f);
    glVertex3f(-0.5f, 0.15f, 0.0f);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.35f, 0.0f, 0.0f);

    glVertex3f(0.35f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);

    glColor4f(0.852f, 0.912f, 0.887f, 1.0f);
    glVertex3f(-0.5f, -0.15f, 0.0f);
    glVertex3f(0.5f, -0.15f, 0.0f);

    glEnd();

    glBegin(GL_POLYGON);
    //Lower Horizontal strip
    glColor4f(0.311f, 0.591f, 0.454f, 1.0f);
    glVertex3f(0.5f, -0.7f, 0.0f);
    glVertex3f(-0.5f, -0.7f, 0.0f);

    glColor4f(0.016f, 0.416f, 0.220f, 1.0f);
    glVertex3f(-0.5f, -1.0f, 0.0f);
    glVertex3f(0.35f, -1.0f, 0.0f);
    glVertex3f(0.5f, -0.85f, 0.0f);

    glEnd();
}

void drawH(void) 
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(-1.7f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    //Upper Left Vertical Strip
    glColor4f(1.0f, 0.404f, 0.122f, 1.0f);
    glVertex3f(-0.2f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 1.0f, 0.0f);

    glColor4f(1.0f, 0.911f, 0.868f, 1.0f);
    glVertex3f(-0.5f, 0.15f, 0.0f);
    glVertex3f(-0.2f, 0.15f, 0.0f);

    //Upper Right Vertical Strip
    glColor4f(1.0f, 0.404f, 0.122f, 1.0f);
    glVertex3f(0.5f, 1.0f, 0.0f);
    glVertex3f(0.2f, 1.0f, 0.0f);

    glColor4f(1.0f, 0.911f, 0.868f, 1.0f);
    glVertex3f(0.2f, 0.15f, 0.0f);
    glVertex3f(0.5f, 0.15f, 0.0f);

    //Lower left Vertical Strip
    glColor4f(0.852f, 0.912f, 0.887f, 1.0f);
    glVertex3f(-0.2f, -0.15f, 0.0f);
    glVertex3f(-0.5f, -0.15f, 0.0f);

    glColor4f(0.016f, 0.416f, 0.220f, 1.0f);
    glVertex3f(-0.5f, -1.0f, 0.0f);
    glVertex3f(-0.2f, -1.0f, 0.0f);  

    //Lower Right vertical strip
    glColor4f(0.852, 0.912f, 0.887f, 1.0f);
    glVertex3f(0.5f, -0.15f, 0.0f);
    glVertex3f(0.2f, -0.15f, 0.0f);

    glColor4f(0.016f, 0.416f, 0.220f, 1.0f);
    glVertex3f(0.2f, -1.0f, 0.0f);
    glVertex3f(0.5f, -1.0f, 0.0f);

    //Middle Upper Horizontal strip
    glColor4f(1.0f, 0.911f, 0.868f, 1.0f);
    glVertex3f(0.5f, 0.15f, 0.0f);
    glVertex3f(-0.5f, 0.15f, 0.0f);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);

    //Middle lower horizontal strip
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);

    glColor4f(0.852f, 0.912f, 0.887f, 1.0f);
    glVertex3f(-0.5f, -0.15f, 0.0f);
    glVertex3f(0.5f, -0.15f, 0.0f);

    glEnd();
}

void drawA1(void)
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(-0.5f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    //Upper Horizontal strip
    glColor4f(1.0f, 0.404f, 0.122f, 1.0f);
    glVertex3f(0.5f, 0.85f, 0.0f);
    glVertex3f(0.35f, 1.0f, 0.0f);
    glVertex3f(-0.35f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 0.85f, 0.0f);

    glColor4f(1.0f, 0.583f, 0.385f, 1.0f);
    glVertex3f(-0.5f, 0.7f, 0.0f);
    glVertex3f(0.5f, 0.7f, 0.0f);

    glEnd();

    glBegin(GL_QUADS);
    {
        //Upper Left Vertical Strips
        glColor4f(1.0f, 0.583f, 0.385f, 1.0f);
        glVertex3f(-0.2f, 0.7f, 0.0f);
        glVertex3f(-0.5f, 0.7f, 0.0f);

        glColor4f(1.0f, 0.911f, 0.868f, 1.0f);
        glVertex3f(-0.5f, 0.15f, 0.0f);
        glVertex3f(-0.2f, 0.15f, 0.0f);

        //Upper Right Vertical Strips
        glColor4f(1.0f, 0.583f, 0.385f, 1.0f);
        glVertex3f(0.5f, 0.7f, 0.0f);
        glVertex3f(0.2f, 0.7f, 0.0f);

        glColor4f(1.0f, 0.911f, 0.868f, 1.0f);
        glVertex3f(0.2f, 0.15f, 0.0f);
        glVertex3f(0.5f, 0.15f, 0.0f);

        //Lower Left Vertical Strip
        glColor4f(0.852f, 0.912f, 0.887f, 1.0f);
        glVertex3f(-0.2f, -0.15f, 0.0f);
        glVertex3f(-0.5f, -0.15f, 0.0f);

        glColor4f(0.016f, 0.416f, 0.220f, 1.0f);
        glVertex3f(-0.5f, -1.0f, 0.0f);
        glVertex3f(-0.2f, -1.0f, 0.0f);

        //Lower Right Vertical Strip
        glColor4f(0.852f, 0.912f, 0.887f, 1.0f);
        glVertex3f(0.5f, -0.15f, 0.0f);
        glVertex3f(0.2f, -0.15f, 0.0f);

        glColor4f(0.016f, 0.416f, 0.220f, 1.0f);
        glVertex3f(0.2f, -1.0f, 0.0f);
        glVertex3f(0.5f, -1.0f, 0.0f);

        //Middle (upper) horizontal strip
        glColor4f(1.0f, 0.911f, 0.868f, 1.0f);
        glVertex3f(0.5f, 0.15f, 0.0f);
        glVertex3f(-0.5f, 0.15f, 0.0f);

        glColor4f(1.0f, 1.0f , 1.0f, 1.0f);
        glVertex3f(-0.5f, 0.0f, 0.0f);
        glVertex3f(0.5f, 0.0f, 0.0f);

        //Middle (lower) horizontal strip
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glVertex3f(0.5f, 0.0f, 0.0f);
        glVertex3f(-0.5f, 0.0f, 0.0f);

        glColor4f(0.852f, 0.912f, 0.887f, 1.0f);
        glVertex3f(-0.5f, -0.15f, 0.0f);
        glVertex3f(0.5f, -0.15f, 0.0f);

        glEnd();

    }

}

void drawR(void) 
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(0.7f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    //Upper Horizontal Strip
    glColor4f(1.0f, 0.404f, 0.122f, 1.0f);
    glVertex3f(0.5f, 0.85f, 0.0f);
    glVertex3f(0.35f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 1.0f, 0.0f);

    glColor4f(1.0f, 0.583f, 0.385f, 1.0f);
    glVertex3f(-0.5f, 0.7f, 0.0f);
    glVertex3f(0.5f, 0.7f, 0.0f);
    glEnd();

    glBegin(GL_QUADS);
    //Upper Left Vertical Strip
    glColor4f(1.0f, 0.583f, 0.385f, 1.0f);
    glVertex3f(-0.2f, 0.7f, 0.0f);
    glVertex3f(-0.5f, 0.7f, 0.0f);

    glColor4f(1.0f, 0.911f, 0.868f, 1.0f);
    glVertex3f(-0.5f, 0.15f, 0.0f);
    glVertex3f(-0.2f, 0.15f, 0.0f);

    //Upper Right Vertical Strip
    glColor4f(1.0f, 0.583f, 0.385f, 1.0f);
    glVertex3f(0.5f, 0.7f, 0.0f);
    glVertex3f(0.2f, 0.7f, 0.0f);

    glColor4f(1.0f, 0.911f, 0.868f, 1.0f);
    glVertex3f(0.2f, 0.15f, 0.0f);
    glVertex3f(0.5f, 0.15f, 0.0f);

    //Middle (upper) horizontal strip
    glColor4f(1.0f, 0.911f, 0.868f, 1.0f);
    glVertex3f(0.5f, 0.15f, 0.0f);
    glVertex3f(-0.5f, 0.15f, 0.0f);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);

    //Middle (Lower) horizontal strip
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);

    glColor4f(0.852f, 0.912f, 0.887f, 1.0f);
    glVertex3f(-0.5f, -0.15f, 0.0f);
    glVertex3f(0.35f, -0.15f, 0.0f);

    //Lower Left Vertical Strips
    glColor4f(0.852f, 0.912f, 0.887f, 1.0f);
    glVertex3f(-0.2f, -0.15f, 0.0f);
    glVertex3f(-0.5f, -0.15f, 0.0f);

    glColor4f(0.016f, 0.416f, 0.220f, 1.0f);
    glVertex3f(-0.5f, -1.0f, 0.0f);
    glVertex3f(-0.2f, -1.0f, 0.0f);

    //Diagonal Strip
    glColor4f(0.852f, 0.912f, 0.887f, 1.0f);
    glVertex3f(0.1f, -0.15f, 0.0f);
    glVertex3f(-0.2f, -0.15f, 0.0f);

    glColor4f(0.016f, 0.416f, 0.220f, 1.0f);
    glVertex3f(0.2f, -1.0f, 0.0f);
    glVertex3f(0.5f, -1.0f, 0.0f);
    glEnd();
}

void drawA2(void)
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(1.9f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    //Upper Horizontal strip
    glColor4f(1.0f, 0.404f, 0.122f, 1.0f);
    glVertex3f(0.5f, 0.85f, 0.0f);
    glVertex3f(0.35f, 1.0f, 0.0f);
    glVertex3f(-0.35f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 0.85f, 0.0f);

    glColor4f(1.0f, 0.583f, 0.385f, 1.0f);
    glVertex3f(-0.5f, 0.7f, 0.0f);
    glVertex3f(0.5f, 0.7f, 0.0f);

    glEnd();


    glBegin(GL_QUADS);
 
        //Upper Left Vertical Strips
        glColor4f(1.0f, 0.583f, 0.385f, 1.0f);
        glVertex3f(-0.2f, 0.7f, 0.0f);
        glVertex3f(-0.5f, 0.7f, 0.0f);

        glColor4f(1.0f, 0.911f, 0.868f, 1.0f);
        glVertex3f(-0.5f, 0.15f, 0.0f);
        glVertex3f(-0.2f, 0.15f, 0.0f);

        //Upper Right Vertical Strips
        glColor4f(1.0f, 0.583f, 0.385f, 1.0f);
        glVertex3f(0.5f, 0.7f, 0.0f);
        glVertex3f(0.2f, 0.7f, 0.0f);

        glColor4f(1.0f, 0.911f, 0.868f, 1.0f);
        glVertex3f(0.2f, 0.15f, 0.0f);
        glVertex3f(0.5f, 0.15f, 0.0f);

        //Lower Left Vertical Strip
        glColor4f(0.852f, 0.912f, 0.887f, 1.0f);
        glVertex3f(-0.2f, -0.15f, 0.0f);
        glVertex3f(-0.5f, -0.15f, 0.0f);

        glColor4f(0.016f, 0.416f, 0.220f, 1.0f);
        glVertex3f(-0.5f, -1.0f, 0.0f);
        glVertex3f(-0.2f, -1.0f, 0.0f);

        //Lower Right Vertical Strip
        glColor4f(0.852f, 0.912f, 0.887f, 1.0f);
        glVertex3f(0.5f, -0.15f, 0.0f);
        glVertex3f(0.2f, -0.15f, 0.0f);

        glColor4f(0.016f, 0.416f, 0.220f, 1.0f);
        glVertex3f(0.2f, -1.0f, 0.0f);
        glVertex3f(0.5f, -1.0f, 0.0f);

        //Middle (upper) horizontal strip
        glColor4f(1.0f, 0.911f, 0.868f, 1.0f);
        glVertex3f(0.5f, 0.15f, 0.0f);
        glVertex3f(-0.5f, 0.15f, 0.0f);

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glVertex3f(-0.5f, 0.0f, 0.0f);
        glVertex3f(0.5f, 0.0f, 0.0f);

        //Middle (lower) horizontal strip
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glVertex3f(0.5f, 0.0f, 0.0f);
        glVertex3f(-0.5f, 0.0f, 0.0f);

        glColor4f(0.852f, 0.912f, 0.887f, 1.0f);
        glVertex3f(-0.5f, -0.15f, 0.0f);
        glVertex3f(0.5f, -0.15f, 0.0f);

        glEnd();

 }

void drawT(void) 
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(3.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    //horizontal Strip
    glColor4f(1.0f, 0.404f, 0.122f, 1.0f);
    glVertex3f(0.5f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 1.0f, 0.0f);

    glColor4f(1.0f, 0.583f, 0.385f, 1.0f);
    glVertex3f(-0.5f, 0.7f, 0.0f);
    glVertex3f(0.5f, 0.7f, 0.0f);

    //Upper vertical Strip
    glColor4f(1.0f, 0.583f, 0.385f, 1.0f);
    glVertex3f(0.15f, 0.7f, 0.0f);
    glVertex3f(-0.15f, 0.7f, 0.0f);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glVertex3f(-0.15f, 0.0f, 0.0f);
    glVertex3f(0.15f, 0.0f, 0.0f);

    //Lower Vertical strip
    glColor4f(1.0f,1.0f, 1.0f, 1.0f);
    glVertex3f(0.15f, 0.0f, 0.0f);
    glVertex3f(-0.15f, 0.0f, 0.0f);

    glColor4f(0.016f, 0.416f, 0.220f, 1.0f);
    glVertex3f(-0.15f, -1.0f, 0.0f);
    glVertex3f(0.15f, -1.0f, 0.0f);
    glEnd();

}