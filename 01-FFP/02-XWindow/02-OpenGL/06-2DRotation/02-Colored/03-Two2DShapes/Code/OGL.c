// Standard Header files
#include <stdio.h> // for printf()
#include <stdlib.h> // for exit()
#include <memory.h> // for memset

// XWindow header files
#include <X11/Xlib.h>
#include <X11/Xutil.h>  // Visual info related API
#include <X11/XKBlib.h>  // Keyboard related Xlib API

// OpenGL related header files
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

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

// OpenGL related varibles
GLXContext glxContext = NULL;

// rotation angles
float angleTriangle = 0.0f;
float angleRectangle = 0.0f;

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

    int framBufferAttributes[] = {
        GLX_DOUBLEBUFFER,
        GLX_RGBA,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
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
	
	// Get Visual info
    visualInfo = glXChooseVisual(gpDisplay, defaultScreen, framBufferAttributes);

    if (visualInfo == NULL)
    {
        fprintf(gpFile, "glXchooseVisual Failed!!!\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }
	
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
        fprintf(gpFile, "Initialize function failed!!\n");
        uninitialize();
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(gpFile, "Initialize function completed sucessfully!!!\n");
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
    // Function declaration
    void printGLInfo(void);

    // code
    glxContext = glXCreateContext(gpDisplay, visualInfo, NULL, True);

    if (glxContext == NULL)
    {
        fprintf(gpFile, "glCreateContext() failed! to create OpenGL context");
        return -1;
    }

    // PrintGl info
    printGLInfo();

    glXMakeCurrent(gpDisplay, window, glxContext);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    return(0);
}

void printGLInfo(void)
{
    // code
        //print OpenGL information
        fprintf(gpFile, "OPENGL INFORMATON");
        fprintf(gpFile, "-----------------\n"); 
        fprintf(gpFile, "OpenGL Vendor : %s\n", glGetString(GL_VENDOR));
        fprintf(gpFile, "OpenGL Rendorer : %s\n", glGetString(GL_RENDERER));
        fprintf(gpFile, "OpenGL Version : %s/n", glGetString(GL_VERSION));
        fprintf(gpFile, "-----------------\n"); 

}

void resize(int width, int height)
{
    // code
    if (height <= 0)        
    {
        height = 1;
    }
    
    glViewport(0,0,(GLsizei)width,(GLsizei)height);

    // set matrix projection mode
	glMatrixMode(GL_PROJECTION);

	// set to identity matrix
	glLoadIdentity();

    // do perpective projection
	gluPerspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 100.0f);
}

void display(void)
{
    // code
    glClear(GL_COLOR_BUFFER_BIT);

    // Set Matrix to Model View mode
	glMatrixMode(GL_MODELVIEW);

	// Set it to identity matrix
	glLoadIdentity();

	// Translate Triangle backword
	glTranslatef(-1.5f, 0.0f, -6.0f);
    glRotatef(angleTriangle, 0.0f, 1.0f, 0.0f);

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

	// Set it to identity matrix
	glLoadIdentity();

	// Translate Rectangle backword
	glTranslatef(1.5f, 0.0f, -6.0f);
    glRotatef(angleRectangle, 1.0f, 0.0f, 0.0f);

	// Draw rectangle here
	glBegin(GL_QUADS);
	// top right
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
	// top left
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
	// left bottom
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
	// right bottom
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glXSwapBuffers(gpDisplay, window);
}

void update(void)
{
    angleTriangle = angleTriangle + 0.1f;
	if(angleTriangle >= 360.0f)
	{
		angleTriangle = angleTriangle - 360.0f;
	}

    angleRectangle = angleRectangle - 0.1f;
	if(angleRectangle <= 0.0f)
	{
		angleRectangle = angleRectangle + 360.0f;
	}
}

void uninitialize(void)
{
	// code
    GLXContext currentContext = glXGetCurrentContext();

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

    // close the file
    if (gpFile)
    {
        fprintf(gpFile, "Program terminated successfully\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}