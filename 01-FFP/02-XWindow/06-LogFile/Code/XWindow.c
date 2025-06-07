// Standard Header files
#include <stdio.h> // for printf()
#include <stdlib.h> // for exit()
#include <memory.h> // for memset

// XWindow header files
#include <X11/Xlib.h>
#include <X11/Xutil.h>  // Visual info related API
#include <X11/XKBlib.h>  // Keyboard related Xlib API

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global variables
Display *gpDisplay = NULL; // 77 members structure which a mediator between Xserver and XClient.
XVisualInfo visualInfo; // similar to device context
Window window; // gives window object
Colormap colormap; //

Bool bFullScreen = False;

// variables related to FILE IO
char gszLogFileName[] = "Log.txt";
FILE *gpFile = NULL;

int main(void)
{
	// function declarations
	void toggleFullScreen(void);
	void uninitialize(void);

	// local variables
	int defaultScreen;
	int defaultDepth;
	Status status;
	XSetWindowAttributes windowAttributes;
	Atom windowManagerDeleteAtom;
	XEvent event;
	Screen *screen = NULL;
	int screenWidth, screenHeight;
	KeySym keySym;
	char keys[26]; // Although we need only 0th index, conventionally the array size is equal to number of alphabets, either capital or small or both

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
	memset((void*) &visualInfo, 0, sizeof(XVisualInfo));
	status = XMatchVisualInfo(gpDisplay, defaultScreen, defaultDepth, TrueColor, &visualInfo);

	if (status == 0)
	{
		fprintf(gpFile, "XMatchVisualIinfo failed!!!\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}
	
	// Set window attributes
	memset((void*) &windowAttributes, 0, sizeof(XSetWindowAttributes));
	windowAttributes.border_pixel = 0;
	windowAttributes.background_pixmap = 0;
	windowAttributes.background_pixel = XBlackPixel(gpDisplay, visualInfo.screen);
	windowAttributes.colormap = XCreateColormap(gpDisplay, 
												XRootWindow(gpDisplay, visualInfo.screen),
												visualInfo.visual,
												AllocNone
											);
	colormap = windowAttributes.colormap;
	windowAttributes.event_mask = KeyPressMask | ButtonPressMask | FocusChangeMask | StructureNotifyMask | ExposureMask;
	// PointerMotionMask is used to get mouse move.
	// VisibilityChangeMask is used to hide and show window.

	// Create the window
	window = XCreateWindow(gpDisplay,
							XRootWindow(gpDisplay, visualInfo.screen),
							0, 0, WIN_WIDTH, WIN_HEIGHT, 0, 
							visualInfo.depth,
							InputOutput, visualInfo.visual,
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
	screen = XScreenOfDisplay(gpDisplay, visualInfo.screen);
	screenWidth = XWidthOfScreen(screen);
	screenHeight = XHeightOfScreen(screen);

	// The initial position of the window is governed by window manager, so let the window manager decide the position. 
	// Later we will center it by XMoveWindow() xlib API
	XMoveWindow(gpDisplay, window, (screenWidth/2 - WIN_WIDTH/2), (screenHeight/2 - WIN_HEIGHT/2));
	

	// Message Loop
	while (1)
	{
		XNextEvent(gpDisplay, &event);
		switch (event.type)
		{
			case MapNotify:
				fprintf(gpFile, "MapNotify event is received\n");
				break;
			case FocusIn:
				fprintf(gpFile, "Window got focus\n");
				break;
			case FocusOut:
				fprintf(gpFile, "Window lost focus\n");
				break;
			case ConfigureNotify:  // similar to WM_SIZE
				fprintf(gpFile, "Window is resized\n");
				break;
			case KeyPress:
				fprintf(gpFile, "Keypress event received\n");
				// For Escape key
				keySym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0); // 3rd parameter for key combination and 4th parameter for "Shift" level
				switch (keySym)
				{
					case XK_Escape:
						uninitialize();
						exit(EXIT_SUCCESS);
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
				fprintf(gpFile, "Mouse is clicked\n");
				switch (event.xbutton.button)
				{
					case 1:
						fprintf(gpFile, "Left mouse is clicked\n");
						break;
					case 2:
						fprintf(gpFile, "Middle mouse is clicked\n");
						break;
					case 3:
						fprintf(gpFile, "Right mouse is clicked\n");
						break;
					case 4:
						fprintf(gpFile, "Mouse wheel up\n");
						break;
					case 5:
						fprintf(gpFile, "Mouse wheel down\n");
						break;
					default:
						break;
				}
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

	uninitialize();
	
	// Close the file
	if (gpFile)
	{
		fprintf(gpFile, "Program terminated successfully\n");
		fclose(gpFile);
		gpFile = NULL;
	}

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
	XSendEvent(gpDisplay, XRootWindow(gpDisplay, visualInfo.screen), False, SubstructureNotifyMask, &event);

}

void uninitialize(void)
{
	// code
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
}