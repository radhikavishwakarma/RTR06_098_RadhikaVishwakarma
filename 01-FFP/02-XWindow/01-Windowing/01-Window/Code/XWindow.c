// Standard Header files
#include <stdio.h> // for printf()
#include <stdlib.h> // for exit()
#include <memory.h> // for memset

// XLib header files
#include <X11/Xlib.h>
#include <X11/Xutil.h>

// Macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global variables
Display *gpDisplay = NULL; // 77 members structure which a mediator between Xserver and XClient.
XVisualInfo visualInfo; // similar to device context
Window window; // gives window object
Colormap colormap; //

int main(void)
{
	// function declarations
	void uninitialize(void);

	// local variables
	int defaultScreen;
	int defaultDepth;
	Status status;
	XSetWindowAttributes windowAttributes;
    Atom windowManagerDeleteAtom;
    XEvent event;

	// code
	// Creates connection with XServer
	gpDisplay = XOpenDisplay(NULL);

	if (gpDisplay == NULL)
	{
		printf("XOpendisplay failed to connect with XServer");
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
		printf("XMatchVisualIinfo failed!!!");
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
		printf("Window creation failed");
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

    // Map the window to show
    XMapWindow(gpDisplay, window);

    // Message Loop
    while(1)
    {
        XNextEvent(gpDisplay, &event);
        switch(event.type)
        {
            case 33:
                uninitialize();
                exit(EXIT_SUCCESS);
                break;

            default:
                break;
        }
    }
    uninitialize();
    return (0);

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