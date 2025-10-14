#include <Windows.h>
#include <stdio.h>

#include <stdlib.h>

#include <glew.h>
#include <gl/GL.h>

#include "OGL.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

LRESULT CALLBACK WNDPROC(HWND, UINT, WPARAM, LPARAM);

BOOL gbFullScreen = FALSE;
HWND ghwnd = NULL;
DWORD dwStyle;
WINDOWPLACEMENT wpPrev;

BOOL gbActiveWindow = FALSE;

BOOL gbEscapeKeyIsPressed = FALSE;

char gszLogFileName[] = "Log.txt";
FILE *gpFile = NULL;

HDC ghdc = NULL;
HGLRC ghrc = NULL;

GLuint shaderProgramObject = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    int initilize(void);
    void display(void);
    void update(void);
    void uninitialize(void);

    WND
}

int windowStartX = (screenWidth / 2) - (WIN_WIDTH / 2);
int windowStartX = (screenHeight / 2) - (WIN_HEIGHT / 2);

// window initializ
wndClass.cbSize = sizeof(WNDCLASSEX);
wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
wndClass.cbClsExtra = 0;
wndClass.cbClsExtra = 0;
wndClass.lpfnWndProc = WndProc;
wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
wnd.Class.lpszClassName = szAppName;
wndClas.menuName = NULL;
wndClass.hIconSm = LoadIcoKn(hInstance, MAKEINTRESOURCEA)