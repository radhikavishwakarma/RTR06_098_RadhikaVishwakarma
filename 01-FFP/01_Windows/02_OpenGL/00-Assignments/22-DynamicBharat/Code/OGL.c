// Win32 Header files
#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#define _USE_MATH_DEFINES 1
#include<math.h>

// OpenGL related Header files
#include<gl/GL.h>

// Custom Header file
#include "OGL.h"

// OpenGL related libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib,"GLU32.lib")
#pragma comment(lib,"winmm.lib")

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define GREY 150, 150, 150
#define WHITE 255, 255, 255
#define SAFFRON 255, 103, 31
#define GREEN 4, 106, 56

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

float tBx = -8.0f, tBy = 0.0f;
float tHx = -8.0f, tHy = 6.3f;
float tA1x = -8.0f, tA1y = -7.5f;
float tRx = 8.0f, tRy = 7.3f;
float tA2x = 8.0f, tA2y = -6.1f;
float tTx = 8.0f, tTy = 0.0f;

float blendLetterB = 0.0f;
float blendLetterH = 0.0f;
float blendLetterA1 = 0.0f;
float blendLetterR = 0.0f;
float blendLetterA2 = 0.0f;
float blendLetterT = 0.0f;

float tJet1x = -8.0f, tJet1y = 0.0f;
float tJet2x = -8.0f, tJet2y = -8.0f;
float tJet3x = -8.0f, tJet3y = 8.0f;
float rJet1 = 0.0f;
float rJet2 = 90.0f;
float rJet3 = -90.0f;

float speed = 0.0005;
float offset = 0.5f;

BOOL bReached = FALSE;
BOOL hReached = FALSE;
BOOL a1Reached = FALSE;
BOOL rReached = FALSE;
BOOL a2Reached = FALSE;
BOOL tReached = FALSE;

int alphaLetters = 255;
int alphaFlagB = 0;
int alphaFlagH = 0;
int alphaFlagA1 = 0;
int alphaFlagR = 0;
int alphaFlagA2 = 0;
int alphaFlagT = 0;

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
            wpPrev.length = sizeof(WINDOWPLACEMENT);
            PlaySound(TEXT("DulhanChali_Clip.wav"), NULL, SND_ASYNC | SND_FILENAME);
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
	
	// Enable blendLettering
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	void drawB(float, float, int);
	void drawH(float, float, int);
	void drawA1(float, float, int);
	void drawR(float, float, int);
	void drawA2(float, float, int);
	void drawT(float, float, int);
	void drawAirplane1(float, float, float);
	void drawAirplane2(float, float, float);
	void drawAirplane3(float, float, float);
	void drawBFlag(float, float, int);
	void drawHFlag(float, float, int);
	void drawA1Flag(float, float, int);
	void drawRFlag(float, float, int);
	void drawA2Flag(float, float, int);
	void drawTFlag(float, float, int);

    // code
	// clear OpenGL buffers
	glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glMatrixMode(GL_MODELVIEW);
	
	// Set to identity matrix
   	glLoadIdentity();

	// Draw here
	drawB(tBx,tBy, alphaLetters);
	drawH(tHx, tHy, alphaLetters);
	drawA1(tA1x, tA1y, alphaLetters);
	drawR(tRx, tRy, alphaLetters);
	drawA2(tA2x, tA2y, alphaLetters);
	drawT(tTx, tTy, alphaLetters);
    drawAirplane1(tJet1x, tJet1y, rJet1);
    drawAirplane2(tJet2x, tJet2y, rJet2);
    drawAirplane3(tJet3x, tJet3y, rJet3);
	drawBFlag(tBx,tBy, alphaFlagB);
	drawHFlag(tHx,tHy, alphaFlagH);
	drawA1Flag(tA1x,tA1y, alphaFlagA1);
	drawRFlag(tRx,tRy, alphaFlagR);
	drawA2Flag(tA2x,tA2y, alphaFlagA2);
	drawTFlag(tTx,tTy, alphaFlagT);

    glDisable(GL_BLEND);

	SwapBuffers(ghdc);
}

void update(void)
{
	// code
	tBx += speed;
	if(tBx >= -2.9f)
	{
		tBx = -2.9f;
		bReached = TRUE;
	}

    if (bReached == TRUE)
    {
        tHx += speed;
        tHy -= speed;
        if(tHx >= -1.7f && tHy <= 0.0f)
        {
            tHx = -1.7f;
            tHy = 0.0f;
            hReached = TRUE;
        }
    }

    if (hReached == TRUE)
    {
        tA1x += speed;
        tA1y += speed;
        if(tA1x >= -0.5f && tA1y >= 0.0f)
        {
            tA1x = -0.5f;
            tA1y = 0.0f;
            a1Reached = TRUE;
        }
    }

    if (a1Reached == TRUE)
    {
        tRx -= speed;
        tRy -= speed;
        if(tRx <= 0.7f && tRy <= 0.0f)
        {
            tRx = 0.7f;
            tRy = 0.0f;
            rReached = TRUE;
        }
    }

    if (rReached == TRUE)
    {
        tA2x -= speed;
        tA2y += speed;
        if(tA2x <= 1.9f && tA2y >= 0.0f)
        {
            tA2x = 1.9f;
            tA2y = 0.0f;
            a2Reached = TRUE;
        }
    }

    if (a2Reached == TRUE)
    {
        tTx -= speed;
        if(tTx <= 3.0f && tTy <= 0.0f)
        {
            tTx = 3.0f;
            tTy = 0.0f;
            tReached = TRUE;
        }
    }

    if (tReached == TRUE)
    {
        tJet1x += speed;
        if(tJet1x >= -2.9f + offset)  // Jet reached B
        {
            alphaFlagB += 1;
            if(alphaFlagB >= 255)
                alphaFlagB = 255;
        }

        if(tJet1x >= -1.7f + offset)  // Jet reached H
        {
            alphaFlagH += 1;
            if(alphaFlagH >= 255)
                alphaFlagH = 255;
        }

        if(tJet1x >= -0.5f + offset)  // Jet reached A1
        {
            alphaFlagA1 += 1;
            if(alphaFlagA1 >= 255)
                alphaFlagA1 = 255;
        }

        if(tJet1x >= 0.7f + offset)  // Jet reached R
        {
            alphaFlagR += 1;
            if(alphaFlagR >= 255)
                alphaFlagR = 255;
        }

        if(tJet1x >= 1.9f + offset)  // Jet reached A2
        {
            alphaFlagA2 += 1;
            if(alphaFlagA2 >= 255)
                alphaFlagA2 = 255;
        }

        if(tJet1x >= 3.0f + offset)  // Jet reached T
        {
            alphaFlagT += 1;
            if(alphaFlagT >= 255)
                alphaFlagT = 255;
        }

        if(tJet1x >= 8.0f)
            tJet1x = 8.0f;

        tJet2x += speed;        
        if(tJet2x >= 8.0f)
            tJet2x = 8.0f;

        tJet2y += speed;
        if(tJet2y >= 0.0f)
            tJet2y = 0.0f;

        rJet2 -= 1.0f;        
        if(rJet2 <= 0.0)
            rJet2 = 0.0f;

        tJet3x += speed;
        if(tJet3x >= 8.0f)
            tJet3x = 8.0f;
        
        tJet3y -= speed;
        if(tJet3y <= 0.0f)
            tJet3y = 0.0f;
       
        rJet3 += 0.5f;
        if(rJet3 >= 0.0f)
            rJet3 = 0.0f;
    }
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

void drawB(float tBx, float tBy, int alphaLetters) 
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(tBx, tBy, 0.0f);
    glBegin(GL_POLYGON);
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5, 0.85f, 0.0f);
    glVertex3f(0.35, 1.0f, 0.0f);
    glVertex3f(-0.5, 1.0f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5, 0.7f, 0.0f);
    glVertex3f(0.5f, 0.7f, 0.0f);
    glEnd();

    glBegin(GL_QUADS);
    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.2f, 0.7f, 0.0f);
    glVertex3f(-0.5f, 0.7f, 0.0f);
   
    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5, 0.15f, 0.0f);
    glVertex3f(-0.2, 0.15f, 0.0f);

    //Upper Right Vertical Strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5f, 0.7f, 0.0f);
    glVertex3f(0.2f, 0.7f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.2f, 0.15f, 0.0f);
    glVertex3f(0.5f, 0.15f, 0.0f);

    //Lower Left Vertical Strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.2f, -0.15f, 0.0f);
    glVertex3f(-0.5f, -0.15f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, -0.7f, 0.0f);
    glVertex3f(-0.2f, -0.7f, 0.0f);

    //Lower Right Vertical Strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5f, -0.15f, 0.0f);
    glVertex3f(0.2f, -0.15f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.2f, -0.7f, 0.0f);
    glVertex3f(0.5f, -0.7f, 0.0f);

    //Middle horizontal strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5f, 0.15f, 0.0f);
    glVertex3f(-0.5f, 0.15f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.35f, 0.0f, 0.0f);

    glVertex3f(0.35f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, -0.15f, 0.0f);
    glVertex3f(0.5f, -0.15f, 0.0f);

    glEnd();

    glBegin(GL_POLYGON);
    //Lower Horizontal strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5f, -0.7f, 0.0f);
    glVertex3f(-0.5f, -0.7f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, -1.0f, 0.0f);
    glVertex3f(0.35f, -1.0f, 0.0f);
    glVertex3f(0.5f, -0.85f, 0.0f);

    glEnd();
}

void drawH(float tHx, float tHy, int alphaLetters) 
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(tHx, tHy, 0.0f);

    glBegin(GL_QUADS);
    //Upper Left Vertical Strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.2f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 1.0f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, 0.15f, 0.0f);
    glVertex3f(-0.2f, 0.15f, 0.0f);

    //Upper Right Vertical Strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5f, 1.0f, 0.0f);
    glVertex3f(0.2f, 1.0f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.2f, 0.15f, 0.0f);
    glVertex3f(0.5f, 0.15f, 0.0f);

    //Lower left Vertical Strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.2f, -0.15f, 0.0f);
    glVertex3f(-0.5f, -0.15f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, -1.0f, 0.0f);
    glVertex3f(-0.2f, -1.0f, 0.0f);  

    //Lower Right vertical strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5f, -0.15f, 0.0f);
    glVertex3f(0.2f, -0.15f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.2f, -1.0f, 0.0f);
    glVertex3f(0.5f, -1.0f, 0.0f);

    //Middle Upper Horizontal strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5f, 0.15f, 0.0f);
    glVertex3f(-0.5f, 0.15f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);

    //Middle lower horizontal strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, -0.15f, 0.0f);
    glVertex3f(0.5f, -0.15f, 0.0f);

    glEnd();
}

void drawA1(float tA1x, float tA1y, int alphaLetters)
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(tA1x, tA1y, 0.0f);
    glBegin(GL_POLYGON);
    //Upper Horizontal strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5f, 0.85f, 0.0f);
    glVertex3f(0.35f, 1.0f, 0.0f);
    glVertex3f(-0.35f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 0.85f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, 0.7f, 0.0f);
    glVertex3f(0.5f, 0.7f, 0.0f);

    glEnd();

    glBegin(GL_QUADS);
    {
        //Upper Left Vertical Strips
        glColor4ub(GREY, alphaLetters);
        glVertex3f(-0.2f, 0.7f, 0.0f);
        glVertex3f(-0.5f, 0.7f, 0.0f);

        glColor4ub(GREY, alphaLetters);
        glVertex3f(-0.5f, 0.15f, 0.0f);
        glVertex3f(-0.2f, 0.15f, 0.0f);

        //Upper Right Vertical Strips
        glColor4ub(GREY, alphaLetters);
        glVertex3f(0.5f, 0.7f, 0.0f);
        glVertex3f(0.2f, 0.7f, 0.0f);

        glColor4ub(GREY, alphaLetters);
        glVertex3f(0.2f, 0.15f, 0.0f);
        glVertex3f(0.5f, 0.15f, 0.0f);

        //Lower Left Vertical Strip
        glColor4ub(GREY, alphaLetters);
        glVertex3f(-0.2f, -0.15f, 0.0f);
        glVertex3f(-0.5f, -0.15f, 0.0f);

        glColor4ub(GREY, alphaLetters);
        glVertex3f(-0.5f, -1.0f, 0.0f);
        glVertex3f(-0.2f, -1.0f, 0.0f);

        //Lower Right Vertical Strip
        glColor4ub(GREY, alphaLetters);
        glVertex3f(0.5f, -0.15f, 0.0f);
        glVertex3f(0.2f, -0.15f, 0.0f);

        glColor4ub(GREY, alphaLetters);
        glVertex3f(0.2f, -1.0f, 0.0f);
        glVertex3f(0.5f, -1.0f, 0.0f);

        //Middle (upper) horizontal strip
        glColor4ub(GREY, alphaLetters);
        glVertex3f(0.5f, 0.15f, 0.0f);
        glVertex3f(-0.5f, 0.15f, 0.0f);

        glColor4ub(GREY, alphaLetters);
        glVertex3f(-0.5f, 0.0f, 0.0f);
        glVertex3f(0.5f, 0.0f, 0.0f);

        //Middle (lower) horizontal strip
        glColor4ub(GREY, alphaLetters);
        glVertex3f(0.5f, 0.0f, 0.0f);
        glVertex3f(-0.5f, 0.0f, 0.0f);

        glColor4ub(GREY, alphaLetters);
        glVertex3f(-0.5f, -0.15f, 0.0f);
        glVertex3f(0.5f, -0.15f, 0.0f);

        glEnd();

    }

}

void drawR(float tRx, float tRy, int alphaLetters) 
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(tRx, tRy, 0.0f);
    glBegin(GL_POLYGON);
    //Upper Horizontal Strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5f, 0.85f, 0.0f);
    glVertex3f(0.35f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 1.0f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, 0.7f, 0.0f);
    glVertex3f(0.5f, 0.7f, 0.0f);
    glEnd();

    glBegin(GL_QUADS);
    //Upper Left Vertical Strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.2f, 0.7f, 0.0f);
    glVertex3f(-0.5f, 0.7f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, 0.15f, 0.0f);
    glVertex3f(-0.2f, 0.15f, 0.0f);

    //Upper Right Vertical Strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5f, 0.7f, 0.0f);
    glVertex3f(0.2f, 0.7f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.2f, 0.15f, 0.0f);
    glVertex3f(0.5f, 0.15f, 0.0f);

    //Middle (upper) horizontal strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5f, 0.15f, 0.0f);
    glVertex3f(-0.5f, 0.15f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);

    //Middle (Lower) horizontal strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, -0.15f, 0.0f);
    glVertex3f(0.35f, -0.15f, 0.0f);

    //Lower Left Vertical Strips
    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.2f, -0.15f, 0.0f);
    glVertex3f(-0.5f, -0.15f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, -1.0f, 0.0f);
    glVertex3f(-0.2f, -1.0f, 0.0f);

    //Diagonal Strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.1f, -0.15f, 0.0f);
    glVertex3f(-0.2f, -0.15f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.2f, -1.0f, 0.0f);
    glVertex3f(0.5f, -1.0f, 0.0f);
    glEnd();
}

void drawA2(float tA2x, float tA2y, int alphaLetters)
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(tA2x, tA2y, 0.0f);
    glBegin(GL_POLYGON);
    //Upper Horizontal strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5f, 0.85f, 0.0f);
    glVertex3f(0.35f, 1.0f, 0.0f);
    glVertex3f(-0.35f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 0.85f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, 0.7f, 0.0f);
    glVertex3f(0.5f, 0.7f, 0.0f);

    glEnd();


    glBegin(GL_QUADS);
 
        //Upper Left Vertical Strips
        glColor4ub(GREY, alphaLetters);
        glVertex3f(-0.2f, 0.7f, 0.0f);
        glVertex3f(-0.5f, 0.7f, 0.0f);

        glColor4ub(GREY, alphaLetters);
        glVertex3f(-0.5f, 0.15f, 0.0f);
        glVertex3f(-0.2f, 0.15f, 0.0f);

        //Upper Right Vertical Strips
        glColor4ub(GREY, alphaLetters);
        glVertex3f(0.5f, 0.7f, 0.0f);
        glVertex3f(0.2f, 0.7f, 0.0f);

        glColor4ub(GREY, alphaLetters);
        glVertex3f(0.2f, 0.15f, 0.0f);
        glVertex3f(0.5f, 0.15f, 0.0f);

        //Lower Left Vertical Strip
        glColor4ub(GREY, alphaLetters);
        glVertex3f(-0.2f, -0.15f, 0.0f);
        glVertex3f(-0.5f, -0.15f, 0.0f);

        glColor4ub(GREY, alphaLetters);
        glVertex3f(-0.5f, -1.0f, 0.0f);
        glVertex3f(-0.2f, -1.0f, 0.0f);

        //Lower Right Vertical Strip
        glColor4ub(GREY, alphaLetters);
        glVertex3f(0.5f, -0.15f, 0.0f);
        glVertex3f(0.2f, -0.15f, 0.0f);

        glColor4ub(GREY, alphaLetters);
        glVertex3f(0.2f, -1.0f, 0.0f);
        glVertex3f(0.5f, -1.0f, 0.0f);

        //Middle (upper) horizontal strip
        glColor4ub(GREY, alphaLetters);
        glVertex3f(0.5f, 0.15f, 0.0f);
        glVertex3f(-0.5f, 0.15f, 0.0f);

        glColor4ub(GREY, alphaLetters);
        glVertex3f(-0.5f, 0.0f, 0.0f);
        glVertex3f(0.5f, 0.0f, 0.0f);

        //Middle (lower) horizontal strip
        glColor4ub(GREY, alphaLetters);
        glVertex3f(0.5f, 0.0f, 0.0f);
        glVertex3f(-0.5f, 0.0f, 0.0f);

        glColor4ub(GREY, alphaLetters);
        glVertex3f(-0.5f, -0.15f, 0.0f);
        glVertex3f(0.5f, -0.15f, 0.0f);

        glEnd();

 }

void drawT(float tTx, float tTy, int alphaLetters) 
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(tTx, tTy, 0.0f);
    glBegin(GL_QUADS);
    //horizontal Strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.5f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 1.0f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.5f, 0.7f, 0.0f);
    glVertex3f(0.5f, 0.7f, 0.0f);

    //Upper vertical Strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.15f, 0.7f, 0.0f);
    glVertex3f(-0.15f, 0.7f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.15f, 0.0f, 0.0f);
    glVertex3f(0.15f, 0.0f, 0.0f);

    //Lower Vertical strip
    glColor4ub(GREY, alphaLetters);
    glVertex3f(0.15f, 0.0f, 0.0f);
    glVertex3f(-0.15f, 0.0f, 0.0f);

    glColor4ub(GREY, alphaLetters);
    glVertex3f(-0.15f, -1.0f, 0.0f);
    glVertex3f(0.15f, -1.0f, 0.0f);
    glEnd();
}

void drawAirplane1(float tJet1x, float tJet1y, float rJet1)
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(tJet1x, tJet1y, 0.0f);
    glRotatef(rJet1, 0.0f, 0.0f, 1.0f);
    // Body
    glColor3f(0.3f, 0.3f, 0.8f); // Blue-gray color
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f, 0.1f);
        glVertex2f(1.0f, 0.1f);
        glVertex2f(1.2f, 0.0f);
        glVertex2f(1.0f, -0.1f);
        glVertex2f(-1.0f, -0.1f);
        glVertex2f(-1.2f, 0.0f);
    glEnd();

    // Wings
    glColor3f(0.4f, 0.4f, 0.9f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, 0.1f);
        glVertex2f(0.5f, 0.1f);
        glVertex2f(0.0f, 0.6f);

        glVertex2f(-0.5f, -0.1f);
        glVertex2f(0.5f, -0.1f);
        glVertex2f(0.0f, -0.6f);
    glEnd();

    // Tail
    glColor3f(0.3f, 0.3f, 0.7f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-1.0f, 0.1f);
        glVertex2f(-1.0f, 0.4f);
        glVertex2f(-0.8f, 0.1f);
    glEnd();

    // Indian Air Force roundel (simplified)
    // Outer orange circle
    glColor3f(1.0f, 0.5f, 0.0f);
    float x = 0.5f, y = 0.0f, r = 0.08f;
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= 360; i += 10)
            glVertex2f(x + r * cos(i * 3.14159f / 180.0f), y + r * sin(i * 3.14159f / 180.0f));
    glEnd();

    // Middle white circle
    glColor3f(1.0f, 1.0f, 1.0f);
    r = 0.05f;
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= 360; i += 10)
            glVertex2f(x + r * cos(i * 3.14159f / 180.0f), y + r * sin(i * 3.14159f / 180.0f));
    glEnd();

    // Inner green circle
    glColor3f(0.0f, 1.0f, 0.0f);
    r = 0.025f;
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= 360; i += 10)
            glVertex2f(x + r * cos(i * 3.14159f / 180.0f), y + r * sin(i * 3.14159f / 180.0f));
    glEnd();
}

void drawAirplane2(float tJet2x, float tJet2y, float rJet2)
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(tJet2x, tJet2y, 0.0f);
    glRotatef(rJet2, 0.0f, 0.0f, 1.0f);
    // Body
    glColor3f(0.3f, 0.3f, 0.8f); // Blue-gray color
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f, 0.1f);
        glVertex2f(1.0f, 0.1f);
        glVertex2f(1.2f, 0.0f);
        glVertex2f(1.0f, -0.1f);
        glVertex2f(-1.0f, -0.1f);
        glVertex2f(-1.2f, 0.0f);
    glEnd();

    // Wings
    glColor3f(0.4f, 0.4f, 0.9f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, 0.1f);
        glVertex2f(0.5f, 0.1f);
        glVertex2f(0.0f, 0.6f);

        glVertex2f(-0.5f, -0.1f);
        glVertex2f(0.5f, -0.1f);
        glVertex2f(0.0f, -0.6f);
    glEnd();

    // Tail
    glColor3f(0.3f, 0.3f, 0.7f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-1.0f, 0.1f);
        glVertex2f(-1.0f, 0.4f);
        glVertex2f(-0.8f, 0.1f);
    glEnd();

    // Indian Air Force roundel (simplified)
    // Outer orange circle
    glColor3f(1.0f, 0.5f, 0.0f);
    float x = 0.5f, y = 0.0f, r = 0.08f;
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= 360; i += 10)
            glVertex2f(x + r * cos(i * 3.14159f / 180.0f), y + r * sin(i * 3.14159f / 180.0f));
    glEnd();

    // Middle white circle
    glColor3f(1.0f, 1.0f, 1.0f);
    r = 0.05f;
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= 360; i += 10)
            glVertex2f(x + r * cos(i * 3.14159f / 180.0f), y + r * sin(i * 3.14159f / 180.0f));
    glEnd();

    // Inner green circle
    glColor3f(0.0f, 1.0f, 0.0f);
    r = 0.025f;
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= 360; i += 10)
            glVertex2f(x + r * cos(i * 3.14159f / 180.0f), y + r * sin(i * 3.14159f / 180.0f));
    glEnd();
}

void drawAirplane3(float tJet3x, float tJet3y, float rJet3)
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(tJet3x, tJet3y, 0.0f);
    glRotatef(rJet3, 0.0f, 0.0f, 1.0f);
    // Body
    glColor3f(0.3f, 0.3f, 0.8f); // Blue-gray color
    glBegin(GL_POLYGON);
        glVertex2f(-1.0f, 0.1f);
        glVertex2f(1.0f, 0.1f);
        glVertex2f(1.2f, 0.0f);
        glVertex2f(1.0f, -0.1f);
        glVertex2f(-1.0f, -0.1f);
        glVertex2f(-1.2f, 0.0f);
    glEnd();

    // Wings
    glColor3f(0.4f, 0.4f, 0.9f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, 0.1f);
        glVertex2f(0.5f, 0.1f);
        glVertex2f(0.0f, 0.6f);

        glVertex2f(-0.5f, -0.1f);
        glVertex2f(0.5f, -0.1f);
        glVertex2f(0.0f, -0.6f);
    glEnd();

    // Tail
    glColor3f(0.3f, 0.3f, 0.7f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-1.0f, 0.1f);
        glVertex2f(-1.0f, 0.4f);
        glVertex2f(-0.8f, 0.1f);
    glEnd();

    // Indian Air Force roundel (simplified)
    // Outer orange circle
    glColor3f(1.0f, 0.5f, 0.0f);
    float x = 0.5f, y = 0.0f, r = 0.08f;
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= 360; i += 10)
            glVertex2f(x + r * cos(i * 3.14159f / 180.0f), y + r * sin(i * 3.14159f / 180.0f));
    glEnd();

    // Middle white circle
    glColor3f(1.0f, 1.0f, 1.0f);
    r = 0.05f;
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= 360; i += 10)
            glVertex2f(x + r * cos(i * 3.14159f / 180.0f), y + r * sin(i * 3.14159f / 180.0f));
    glEnd();

    // Inner green circle
    glColor3f(0.0f, 1.0f, 0.0f);
    r = 0.025f;
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= 360; i += 10)
            glVertex2f(x + r * cos(i * 3.14159f / 180.0f), y + r * sin(i * 3.14159f / 180.0f));
    glEnd();
}

void drawBFlag(float tBx, float tBy, int alphaFlagB) 
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(tBx, tBy, 0.0f);
    
    glBegin(GL_POLYGON);
    //Middle horizontal strip - Upper Half
    glColor4ub(SAFFRON, alphaFlagB);
    glVertex3f(0.5f, 0.15f, 0.0f);
    glVertex3f(-0.5f, 0.15f, 0.0f);
    
    glColor4ub(WHITE, alphaFlagB);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.35f, 0.0f, 0.0f);
    glEnd();

    glBegin(GL_POLYGON);
    //Middle horizontal strip - Bottom Half
    glColor4ub(WHITE, alphaFlagB);
    glVertex3f(0.35f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);

    glColor4ub(GREEN, alphaFlagB);
    glVertex3f(-0.5f, -0.15f, 0.0f);
    glVertex3f(0.5f, -0.15f, 0.0f);
    glEnd();
}

void drawHFlag(float tHx, float tHy, int alphaFlagH) 
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(tHx, tHy, 0.0f);

    glBegin(GL_POLYGON);
    //Middle Upper Horizontal strip
    glColor4ub(SAFFRON, alphaFlagH);
    glVertex3f(0.5f, 0.15f, 0.0f);
    glVertex3f(-0.5f, 0.15f, 0.0f);

    glColor4ub(WHITE, alphaFlagH);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glEnd();
    
    glBegin(GL_POLYGON);
    //Middle lower horizontal strip
    glColor4ub(WHITE, alphaFlagH);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);

    glColor4ub(GREEN, alphaFlagH);
    glVertex3f(-0.5f, -0.15f, 0.0f);
    glVertex3f(0.5f, -0.15f, 0.0f);
    glEnd();
}

void drawA1Flag(float tA1x, float tA1y, int alphaFlagA1) 
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(tA1x, tA1y, 0.0f);

    glBegin(GL_POLYGON);
    //Middle Upper Horizontal strip
    glColor4ub(SAFFRON, alphaFlagA1);
    glVertex3f(0.5f, 0.15f, 0.0f);
    glVertex3f(-0.5f, 0.15f, 0.0f);

    glColor4ub(WHITE, alphaFlagA1);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glEnd();
    
    glBegin(GL_POLYGON);
    //Middle lower horizontal strip
    glColor4ub(WHITE, alphaFlagA1);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);

    glColor4ub(GREEN, alphaFlagA1);
    glVertex3f(-0.5f, -0.15f, 0.0f);
    glVertex3f(0.5f, -0.15f, 0.0f);
    glEnd();
}

void drawRFlag(float tRx, float tRy, int alphaFlagR) 
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(tRx, tRy, 0.0f);

    glBegin(GL_QUADS);
    //Middle (upper) horizontal strip
    glColor4ub(SAFFRON, alphaFlagR);
    glVertex3f(0.5f, 0.15f, 0.0f);
    glVertex3f(-0.5f, 0.15f, 0.0f);

    glColor4ub(WHITE, alphaFlagR);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);

    //Middle (Lower) horizontal strip
    glColor4ub(WHITE, alphaFlagR);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);

    glColor4ub(GREEN, alphaFlagR);
    glVertex3f(-0.5f, -0.15f, 0.0f);
    glVertex3f(0.35f, -0.15f, 0.0f);
    glEnd();
}

void drawA2Flag(float tA2x, float tA2y, int alphaFlagA2) 
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(tA2x, tA2y, 0.0f);

    glBegin(GL_POLYGON);
    //Middle Upper Horizontal strip
    glColor4ub(SAFFRON, alphaFlagA2);
    glVertex3f(0.5f, 0.15f, 0.0f);
    glVertex3f(-0.5f, 0.15f, 0.0f);

    glColor4ub(WHITE, alphaFlagA2);
    glVertex3f(-0.5f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glEnd();
    
    glBegin(GL_POLYGON);
    //Middle lower horizontal strip
    glColor4ub(WHITE, alphaFlagA2);
    glVertex3f(0.5f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.0f, 0.0f);

    glColor4ub(GREEN, alphaFlagA2);
    glVertex3f(-0.5f, -0.15f, 0.0f);
    glVertex3f(0.5f, -0.15f, 0.0f);
    glEnd();
}

void drawTFlag(float tTx, float tTy, int alphaFlagT) 
{
    glLoadIdentity();
    glScalef(0.15f, 0.15f, 0.0f);
    glTranslatef(tTx, tTy, 0.0f);

    glBegin(GL_POLYGON);
    //Middle Upper Horizontal strip
    glColor4ub(SAFFRON, alphaFlagT);
    glVertex3f(0.15f, 0.15f, 0.0f);
    glVertex3f(-0.15f, 0.15f, 0.0f);

    glColor4ub(WHITE, alphaFlagT);
    glVertex3f(-0.15f, 0.0f, 0.0f);
    glVertex3f(0.15f, 0.0f, 0.0f);
    glEnd();
    
    glBegin(GL_POLYGON);
    //Middle lower horizontal strip
    glColor4ub(WHITE, alphaFlagT);
    glVertex3f(0.15f, 0.0f, 0.0f);
    glVertex3f(-0.15f, 0.0f, 0.0f);

    glColor4ub(GREEN, alphaFlagT);
    glVertex3f(-0.15f, -0.15f, 0.0f);
    glVertex3f(0.15f, -0.15f, 0.0f);
    glEnd();
}