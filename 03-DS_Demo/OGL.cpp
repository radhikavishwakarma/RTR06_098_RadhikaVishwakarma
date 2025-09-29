// Win32 Header files
#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <mmsystem.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// OpenGL related Header files
#include<GL/glew.h>
#include<gl/GL.h>
#include<gl/GLU.h>

// Custom Header file
#include "OGL.h"

// OpenGL related libraries
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GLU32.lib")

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

GLuint texture_sky = 0;
GLuint texture_scene1 = 0;
GLuint texture_bird_pink = 0;
GLuint texture_bird_red = 0;
GLuint texture_pearl1 = 0;
GLuint texture_stick = 0;
GLuint texture_grasses = 0;
GLuint texture_bird_blue = 0;
GLuint texture_CreditsRoll = 0;
GLuint texture_PostCreditsRoll = 0;
GLuint texture_PreIntroRoll = 0;
GLuint texture_TechStackRoll = 0;
GLuint texture_TitleMain = 0;

// Sun Position
float sunPosY = -7.0f;
float skyPosY = -5.5f;
float ScenePosY = -9.5f;
BOOL isSunReached = FALSE;
float birdPinkPosX = 11.8f;
float birdPinkPosY = 6.0f;
float birdRedPosX = -7.5f;
float birdRedPosY = 6.0f;
BOOL areBirdsReachedGround = FALSE;
BOOL areBirdsReachedSky = FALSE;
float pearl1PosX = 11.8f;
float pearl1PosY = 6.0f;
float stickPosX = -7.5f;
float stickPosY = 6.0f;
float grassesPosX = 11.8f;
float grassesPosY = 6.0f;
float birdBluePosX = -7.5f;
float birdBluePosY = 6.0f;
float creditsRollY = -5.0f;
float postCreditsRollY = -5.0f;
float preIntroRollY = -5.0f;
float techStackRollY = -5.0f;
float titleMainY = -8.0f;

UINT iTimeElapsed = 0;
UINT_PTR timerId;

//	time to update the update() function
UINT timeUpdate = 100; //	10 * timeUpdate = 1 sec

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
		case WM_TIMER:
			iTimeElapsed++;
			fprintf(gpFile, "%d seconds\n", iTimeElapsed);
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
	// function declaration
	void resize(int, int);
	BOOL LoadPNGTexture(GLuint*, char*);

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
	
	// Depth related code
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// From here OpenGL Code starts
	// Tell OpenGL to choose the color to clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// checkout https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-settimer
	timerId = SetTimer(ghwnd, 1, timeUpdate, (TIMERPROC)NULL);

	if (timerId == 0)
	{
		// Handle timer creation error
		fprintf(gpFile, "SetTimer failed\n");
		return -1;
	}

	// Loading transparent texture_scene1
	stbi_set_flip_vertically_on_load(TRUE);
	
	if (LoadPNGTexture(&texture_sky, "Sky.png") == FALSE)
	{
		fprintf(gpFile, "LoadPNGTexture() for texture_sky failed\n");
		return(-6);
	}

	if (LoadPNGTexture(&texture_scene1, "Scene1.png") == FALSE)
	{
		fprintf(gpFile, "LoadPNGTexture() for texture_scene1 failed\n");
		return(-7);
	}

	if (LoadPNGTexture(&texture_bird_pink, "bird_pink.png") == FALSE)
	{
		fprintf(gpFile, "LoadPNGTexture() for texture_bird_pink failed\n");
		return(-8);
	}

	if (LoadPNGTexture(&texture_bird_red, "bird_red.png") == FALSE)
	{
		fprintf(gpFile, "LoadPNGTexture() for texture_bird_red failed\n");
		return(-9);
	}

	if (LoadPNGTexture(&texture_pearl1, "pearl1.png") == FALSE)
	{
		fprintf(gpFile, "LoadPNGTexture() for texture_pearl1 failed\n");
		return(-10);
	}

	if (LoadPNGTexture(&texture_stick, "stick.png") == FALSE)
	{
		fprintf(gpFile, "LoadPNGTexture() for texture_stick failed\n");
		return(-11);
	}

	if (LoadPNGTexture(&texture_grasses, "grasses.png") == FALSE)
	{
		fprintf(gpFile, "LoadPNGTexture() for texture_stick failed\n");
		return(-12);
	}

	if (LoadPNGTexture(&texture_bird_blue, "bird_blue.png") == FALSE)
	{
		fprintf(gpFile, "LoadPNGTexture() for texture_bird_blue failed\n");
		return(-13);
	}

	if (LoadPNGTexture(&texture_CreditsRoll, "CreditsRoll.png") == FALSE)
	{
		fprintf(gpFile, "LoadPNGTexture() for texture_CreditsRoll failed\n");
		return(-14);
	}

	if (LoadPNGTexture(&texture_PostCreditsRoll, "PostCreditsRoll.png") == FALSE)
	{
		fprintf(gpFile, "LoadPNGTexture() for texture_PostCreditsRoll failed\n");
		return(-15);
	}

	if (LoadPNGTexture(&texture_PreIntroRoll, "PreIntroRoll.png") == FALSE)
	{
		fprintf(gpFile, "LoadPNGTexture() for texture_PreIntroRoll failed\n");
		return(-16);
	}

	if (LoadPNGTexture(&texture_TechStackRoll, "TechStackRoll.png") == FALSE)
	{
		fprintf(gpFile, "LoadPNGTexture() for texture_TechStackRoll failed\n");
		return(-17);
	}

	if (LoadPNGTexture(&texture_TitleMain, "TitleMain.png") == FALSE)
	{
		fprintf(gpFile, "LoadPNGTexture() for texture_TitleMain failed\n");
		return(-18);
	}

	FILE *fp = fopen("ChivChivChimni.wav", "rb");
	int readyPlay = 0;
    if (fp != NULL)
    {
		readyPlay = 1;
        fclose(fp); // close the file
    }
	if (readyPlay == 1)
	{
		PlaySound("ChivChivChimni.wav", NULL, SND_ASYNC | SND_FILENAME);
	}
    
	// enabling depth
	glEnable(GL_TEXTURE_2D);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


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

	// set matrix projection mode
	glMatrixMode(GL_PROJECTION);

	// set to identity matrix
	glLoadIdentity();

	// do perspective projection
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void display(void)
{
	// function declaration
	void drawCircle(float, float, float);

    // code
	// clear OpenGL buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set Matrix to Model View mode
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ----------------------Sky texture here-------------------
	// Set it to identity matrix
	glLoadIdentity();

	glPushMatrix();
	// translate triangle backwards
	glTranslatef(0.0f, skyPosY, -13.0f);
	glScalef(1.2f, 1.2f, 0.0f);

	// glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_sky);
	
	// Draw quad here
	glBegin(GL_QUADS);
	// Front face
	// Top Right
	glTexCoord2f(1.0f, 1.0f);
    glVertex3f(8.0f, 9.0f, 0.0f);

	// Top Left
	glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-8.0f, 9.0f, 0.0f);

	// Bottom Left
	glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-8.0f, -9.0f, 0.0f);

	// Bottom Right
	glTexCoord2f(1.0f, 0.0f);
    glVertex3f(8.0f, -9.0f, 0.0f);

    glEnd();

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();

	// ----------------------PreIntro texture here-------------------
	// Set it to identity matrix
	glLoadIdentity();

	glPushMatrix();
	// translate triangle backwards
	glTranslatef(0.0f, preIntroRollY, -13.0f);
	glScalef(1.2f, 1.2f, 0.0f);

	// glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_PreIntroRoll);
	
	// Draw quad here
	glBegin(GL_QUADS);
	// Front face
	// Top Right
	glTexCoord2f(1.0f, 1.0f);
    glVertex3f(2.5f, 1.5f, 0.0f);

	// Top Left
	glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2.5f, 1.5f, 0.0f);

	// Bottom Left
	glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-2.5f, -1.5f, 0.0f);

	// Bottom Right
	glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2.5f, -1.5f, 0.0f);

    glEnd();

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();

	// ----------------------Title Main texture here-------------------
	// Set it to identity matrix
	glLoadIdentity();

	glPushMatrix();
	// translate triangle backwards
	glTranslatef(0.0f, titleMainY, -13.0f);
	glScalef(1.2f, 1.2f, 0.0f);

	// glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_TitleMain);
	
	// Draw quad here
	glBegin(GL_QUADS);
	// Front face
	// Top Right
	glTexCoord2f(1.0f, 1.0f);
    glVertex3f(4.5f, 1.0f, 0.0f);

	// Top Left
	glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-4.5f, 1.0f, 0.0f);

	// Bottom Left
	glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-4.5f, -1.0f, 0.0f);

	// Bottom Right
	glTexCoord2f(1.0f, 0.0f);
    glVertex3f(4.5f, -1.0f, 0.0f);

    glEnd();

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();

	// ----------------------Draw sun here-------------------
	// Set it to identity matrix
	glLoadIdentity();

	glPushMatrix();
	// translate triangle backwards
	glTranslatef(3.8f, sunPosY, -13.0f);
	
	drawCircle(0.0f, 0.0f, 0.0f);

	glPopMatrix();

	// ----------------------Scene texture here-------------------
	// Set it to identity matrix
	glLoadIdentity();
	glPushMatrix();

	// translate triangle backwards
	glTranslatef(0.0f, ScenePosY, -11.0f);
	// glScalef(0.2f, 0.2f, 0.0f);
	
	// glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_scene1);

	// Draw quad here
	glBegin(GL_QUADS);
	// Front face
	// Top Right
	glTexCoord2f(1.0f, 1.0f);
    glVertex3f(8.0f, 4.5f, 0.0f);

	// Top Left
	glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-8.0f, 4.5f, 0.0f);

	// Bottom Left
	glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-8.0f, -4.5f, 0.0f);

	// Bottom Right
	glTexCoord2f(1.0f, 0.0f);
    glVertex3f(8.0f, -4.5f, 0.0f);

    glEnd();

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	// ----------------------Bird_pink texture here-------------------
	// Set it to identity matrix
	glLoadIdentity();
	glPushMatrix();

	// translate pink_bird 
	glTranslatef(birdPinkPosX, birdPinkPosY, -11.0f);
	glScalef(-0.65f, 0.65f, 0.0f);
	
	// glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_bird_pink);

	// Draw quad here
	glBegin(GL_QUADS);
	// Front face
	// Top Right
	glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);

	// Top Left
	glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);

	// Bottom Left
	glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);

	// Bottom Right
	glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glEnd();

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	// ----------------------Bird_red texture here-------------------
	// Set it to identity matrix
	glLoadIdentity();
	glPushMatrix();

	// translate bird_red 
	glTranslatef(birdRedPosX, birdRedPosY, -11.0f);
	glScalef(0.55f, 0.55f, 0.0f);
	
	// glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_bird_red);

	// Draw quad here
	glBegin(GL_QUADS);
	// Front face
	// Top Right
	glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);

	// Top Left
	glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);

	// Bottom Left
	glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);

	// Bottom Right
	glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glEnd();

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	// ----------------------Pearl1 texture here-------------------
	// Set it to identity matrix
	glLoadIdentity();
	glPushMatrix();

	// translate pearl1
	glTranslatef(pearl1PosX, pearl1PosY, -11.0f);
	glScalef(0.07f, 0.07f, 0.0f);
	
	// glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_pearl1);

	// Draw quad here
	glBegin(GL_QUADS);
	// Front face
	// Top Right
	glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);

	// Top Left
	glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);

	// Bottom Left
	glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);

	// Bottom Right
	glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glEnd();

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	// ----------------------stick texture here-------------------
	// Set it to identity matrix
	glLoadIdentity();
	glPushMatrix();

	// translate stick
	glTranslatef(stickPosX, stickPosY, -11.0f);
	glScalef(0.30f, 0.30f, 0.0f);
	
	// glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_stick);

	// Draw quad here
	glBegin(GL_QUADS);
	// Front face
	// Top Right
	glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);

	// Top Left
	glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);

	// Bottom Left
	glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);

	// Bottom Right
	glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glEnd();

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	// ----------------------Grasses texture here-------------------
	// Set it to identity matrix
	glLoadIdentity();
	glPushMatrix();

	// translate pearl1
	glTranslatef(grassesPosX, grassesPosY, -11.0f);
	glScalef(0.35f, 0.35f, 0.0f);
	
	// glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_grasses);

	// Draw quad here
	glBegin(GL_QUADS);
	// Front face
	// Top Right
	glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);

	// Top Left
	glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);

	// Bottom Left
	glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);

	// Bottom Right
	glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glEnd();

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	// ----------------------Bird_blue texture here-------------------
	// Set it to identity matrix
	glLoadIdentity();
	glPushMatrix();

	// translate bird_red 
	glTranslatef(birdBluePosX, birdBluePosY, -11.0f);
	glScalef(0.22f, 0.22f, 0.0f);
	
	// glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_bird_blue);

	// Draw quad here
	glBegin(GL_QUADS);
	// Front face
	// Top Right
	glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);

	// Top Left
	glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);

	// Bottom Left
	glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);

	// Bottom Right
	glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);

    glEnd();

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	SwapBuffers(ghdc);
}

void drawCircle(float x, float y, float z)
{
    // local variables declaration
    double radius = 1.0f;
    double radian_angle;
	const double PI = 3.1459;
 
    glBegin(GL_POLYGON);
    for (double angle = 0.0; angle <= 360.0; angle += 2.0)
    {
        radian_angle = angle * PI/180;
        x = radius * cos(radian_angle);
        y = radius * sin(radian_angle);
		//set the foreground color
		glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(x, y);
    }
     
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	
}

void update(void)
{
	// code
	if (iTimeElapsed > 0 && iTimeElapsed < 90)
	{
		// PreIntro animated here
		preIntroRollY = preIntroRollY + 0.01f;
		if (preIntroRollY >= 7.5f)
		{
			preIntroRollY = 7.5f;
		}	
	}

	// Title Main animated here
	if (iTimeElapsed > 70 && iTimeElapsed < 175)
	{
		titleMainY = titleMainY + 0.01f;
		if (titleMainY >= 6.5f)
		{
			titleMainY = 6.5f;
		}
	}
		
	if (iTimeElapsed > 160 && iTimeElapsed < 250)
	{
		// Sky animated here
		skyPosY = skyPosY + 0.01f;
		if (skyPosY >= 5.5f)
		{
			skyPosY = 5.5f;
		}

		// Sun animated here
		sunPosY = sunPosY + 0.01f;
		if (sunPosY >= 3.8f)
		{
			sunPosY = 3.8f;
			isSunReached = TRUE;
		}

		// Scene animated here
		ScenePosY = ScenePosY + 0.01f;
		if (ScenePosY >= 0.0f)
		{
			ScenePosY = 0.0f;
		}
	}

	if (iTimeElapsed > 200 && iTimeElapsed < 350)
	{
		if (isSunReached == TRUE)
		{
			birdPinkPosX = birdPinkPosX - 0.01f;
			birdPinkPosY = birdPinkPosY - 0.01f;
			if (birdPinkPosX <= 2.8f || birdPinkPosY <= -3.0f)
			{
				birdPinkPosX = 2.8f;
				birdPinkPosY = -3.0f;
				areBirdsReachedGround = TRUE;
			}
		}
	}

	if (iTimeElapsed > 200 && iTimeElapsed < 350)
	{
		if (isSunReached == TRUE)
		{
			birdRedPosX = birdRedPosX + 0.01f;
			birdRedPosY = birdRedPosY - 0.01f;
			if (birdRedPosX >= 1.5f || birdRedPosY <= -3.0f)
			{
				birdRedPosX = 1.5f;
				birdRedPosY = -3.0f;
				areBirdsReachedGround = TRUE;
			}
		}
	}

	if (iTimeElapsed > 300 && iTimeElapsed < 420)
	{
		if (areBirdsReachedGround == TRUE)
		{
			birdPinkPosX = birdPinkPosX + 0.01f;
			birdPinkPosY = birdPinkPosY + 0.01f;
			if (birdPinkPosX >= 11.8f || birdPinkPosY >= 6.0f)
			{
				birdPinkPosX = 11.8f;
				birdPinkPosY = 6.0f;
				areBirdsReachedGround = FALSE;
				areBirdsReachedSky = TRUE;
			}
		}
	}

	if (iTimeElapsed > 300 && iTimeElapsed < 420)
	{
		if (areBirdsReachedGround == TRUE)
		{
			birdRedPosX = birdRedPosX - 0.01f;
			birdRedPosY = birdRedPosY + 0.01f;
			if (birdRedPosX >= 7.5f || birdRedPosY >= 6.0f)
			{
				birdRedPosX = -7.5f;
				birdRedPosY = 6.0f;
				areBirdsReachedGround = FALSE;
				areBirdsReachedSky = TRUE;
			}
		}
	}

	if (iTimeElapsed > 420 && iTimeElapsed < 600)
	{
		if (isSunReached == TRUE)
		{
			birdPinkPosX = birdPinkPosX - 0.01f;
			birdPinkPosY = birdPinkPosY - 0.01f;
			if (birdPinkPosX <= 2.8f || birdPinkPosY <= -3.0f)
			{
				birdPinkPosX = 2.8f;
				birdPinkPosY = -3.0f;
			}
		}
		pearl1PosX = pearl1PosX - 0.01f;
		pearl1PosY = pearl1PosY - 0.01f;
		if (pearl1PosX <= 2.8f || pearl1PosY <= -3.5f)
		{
			pearl1PosX = 2.8f;
			pearl1PosY = -3.5f;	
		}
	}

	if (iTimeElapsed > 420 && iTimeElapsed < 600)
	{
		if (isSunReached == TRUE)
		{
			birdRedPosX = birdRedPosX + 0.01f;
			birdRedPosY = birdRedPosY - 0.01f;
			if (birdRedPosX >= 1.5f || birdRedPosY <= -3.0f)
			{
				birdRedPosX = 1.5f;
				birdRedPosY = -3.0f;
			}
		}
	}

	if (iTimeElapsed > 600 && iTimeElapsed < 720)
	{
		birdPinkPosX = birdPinkPosX + 0.01f;
		birdPinkPosY = birdPinkPosY + 0.01f;
		if (birdPinkPosX >= 11.8f || birdPinkPosY >= 6.0f)
		{
			birdPinkPosX = 11.8f;
			birdPinkPosY = 6.0f;
			areBirdsReachedGround = FALSE;
			areBirdsReachedSky = TRUE;
		}

		birdRedPosX = birdRedPosX - 0.01f;
		birdRedPosY = birdRedPosY + 0.01f;
		if (birdRedPosX >= 7.5f || birdRedPosY >= 6.0f)
		{
			birdRedPosX = -7.5f;
			birdRedPosY = 6.0f;
			areBirdsReachedGround = FALSE;
			areBirdsReachedSky = TRUE;
		}
	}

	if (iTimeElapsed > 720 && iTimeElapsed < 900)
	{
		birdPinkPosX = birdPinkPosX - 0.01f;
		birdPinkPosY = birdPinkPosY - 0.01f;
		if (birdPinkPosX <= 3.5f || birdPinkPosY <= -3.0f)
		{
			birdPinkPosX = 3.5f;
			birdPinkPosY = -3.0f;
		}
		grassesPosX = grassesPosX - 0.01f;
		grassesPosY = grassesPosY - 0.01f;
		if (grassesPosX <= 3.5f || grassesPosY <= -3.3f)
		{
			grassesPosX = 3.5f;
			grassesPosY = -3.3f;	
		}
	}

	if (iTimeElapsed > 720 && iTimeElapsed < 900)
	{
		birdRedPosX = birdRedPosX + 0.01f;
		birdRedPosY = birdRedPosY - 0.01f;
		if (birdRedPosX >= 1.5f || birdRedPosY <= -3.0f)
		{
			birdRedPosX = 1.5f;
			birdRedPosY = -3.0f;
		}
		stickPosX = stickPosX + 0.01f;
		stickPosY = stickPosY - 0.01f;
		if (stickPosX >= 1.5f || stickPosY <= -3.0f)
		{
			stickPosX = 2.0f;
			stickPosY = -3.5f;	
		}
	}

	if (iTimeElapsed > 830 && iTimeElapsed < 1000)
	{
		birdBluePosX = birdBluePosX + 0.01f;
		birdBluePosY = birdBluePosY - 0.01f;
		if (birdBluePosX >= -4.4f || birdBluePosY <= -1.3f)
		{
			birdBluePosX = -4.4f;
			birdBluePosY = -1.3f;
		}
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

	if (texture_scene1)
	{
		glDeleteTextures(1, &texture_scene1);
		texture_scene1 = 0;
	}

	if (texture_bird_pink)
	{
		glDeleteTextures(1, &texture_bird_pink);
		texture_bird_pink = 0;
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

BOOL LoadPNGTexture(GLuint *texture, char *file)
{
	// Variable declarations
	int width, height, numberOfChannels;
	unsigned char * image;
	BOOL bResult = FALSE;

	// code
	image = stbi_load(file, &width, &height, &numberOfChannels, STBI_rgb_alpha);
	if(image)
	{
		bResult = TRUE;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		if(numberOfChannels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		}
		if(numberOfChannels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		}

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(image);
		
	}
	else
	{
		fprintf(gpFile, "stbi_load failed!! %s - %s\n", file, stbi_failure_reason());
		return (-7);
	}
	return (bResult);
}