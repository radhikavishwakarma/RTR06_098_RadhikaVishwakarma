#include <AppKit/AppKit.h>
#include <CoreGraphics/CoreGraphics.h>
#include <CoreVideo/CoreVideo.h>
#import <Foundation/Foundation.h> // like stdio.h
#include <OpenGL/OpenGL.h>
#import <Cocoa/Cocoa.h>  // like Windows.h
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>

// global functions declarations
CVReturn myDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp *, const CVTimeStamp *, CVOptionFlags, CVOptionFlags *, void *);

// global variables 
FILE *gpFile = NULL;

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
@end

@interface GLView : NSOpenGLView
-(void)uninitialize;
@end

int main(int argc, char *argv[])
{
    // code
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];

    NSApp = [NSApplication sharedApplication];

    [NSApp setDelegate:[[AppDelegate alloc]init]];

    [NSApp run];

    [pool release];

    return(0);
}

@implementation AppDelegate
{
    @private
    NSWindow *window;
    GLView *glView;
}

-(void)applicationDidFinishLaunching:(NSNotification *)notification
{
    // code
    // Create Log File
    NSBundle *appBundle = [NSBundle mainBundle]; // NSBundle is logical representation of your application directory [Window.app]
    NSString *appDirPath = [appBundle bundlePath]; // Gives the base path from Root directory.
    NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];
    NSString *logFileNameWithPath = [NSString stringWithFormat:@"%@/Log.txt", parentDirPath]; // %@ is format specifier for NSString
    const char *pszLogFileNameWithPath = [logFileNameWithPath cStringUsingEncoding:NSASCIIStringEncoding];

    gpFile = fopen(pszLogFileNameWithPath, "w");

    if (gpFile == NULL) 
    {
        // Message Box
        NSAlert *alert = [[NSAlert alloc]init];
        [alert setAlertStyle:NSAlertStyleCritical];
        [alert setMessageText:@"Failed to create log file!!"];
        [alert addButtonWithTitle:@"Exit"];
        [alert runModal];
        [alert release];

        [self release];
        [NSApp terminate:self];
    }
    else 
    {
        fprintf(gpFile, "Program Started Succesfully!\n");
    }

    NSRect winRect = NSMakeRect(0.0, 0.0, 800.0, 600.0);
    window = [[NSWindow alloc]initWithContentRect:winRect
                                        styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable| NSWindowStyleMaskClosable | NSWindowStyleMaskResizable
                                          backing:NSBackingStoreBuffered
                                            defer:NO];
    [window setTitle:@"Radhika Vishwakarma"];
    [window center];
    
    glView = [[GLView alloc]initWithFrame:winRect];
    
    [window setContentView:glView];
    [window setDelegate:self];
    [window makeKeyAndOrderFront:self];
}

-(void)applicationWillTerminate:(NSNotification *)notification
{
    // code
    if (gpFile)
    {
        fprintf(gpFile, "Program Terminated Succesfully!\n");
        fclose(gpFile);
        gpFile = NULL;
    }
}

-(BOOL)windowShouldClose:(NSWindow *)aWindow
{
    // code
    [glView uninitialize];
    return(YES);
}

-(void)windowWillClose:(NSNotification *)notification 
{
    // code
    [NSApp terminate:self];
}

-(void)dealloc 
{
    [glView release];
    [window release];
    [super dealloc];
}

@end

@implementation GLView
{
    CVDisplayLinkRef displayLink;
    @private
    
}

-(id)initWithFrame:(NSRect)frame
{
    // code
    self = [super initWithFrame:frame];
    if (self) 
    {
        [[self window]setContentView:self];

        // NSOpenGLPixelFormat attributes
        NSOpenGLPixelFormatAttribute attributes[] =
        {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core, 
            NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
            NSOpenGLPFANoRecovery,
            NSOpenGLPFAAccelerated,
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAAlphaSize, 8,
            NSOpenGLPFADoubleBuffer,
            0
        };

        // Create NSOpenGLPixelFormat
        NSOpenGLPixelFormat *pixelFormat = [[[NSOpenGLPixelFormat alloc]initWithAttributes: attributes] autorelease];
        if (pixelFormat == nil) 
        {
            fprintf(gpFile, "NSOpenGLPixelFormat cannot be created.\n");
            [self uninitialize];
            [self release];
            [NSApp terminate: self];
        }

        // Create NSOpenGL context
        NSOpenGLContext *glContext = [[[NSOpenGLContext alloc]initWithFormat:pixelFormat shareContext:nil]autorelease];

        if (glContext == nil) 
        {
            fprintf(gpFile, "NSOpenGLContext cannot be created.\n");
            [self uninitialize];
            [self release];
            [NSApp terminate: self];
        }

        // set view's pixelFormat with our created pixel format
        [self setPixelFormat: pixelFormat];

        // set view's context with our created context
        [self setOpenGLContext: glContext];

        // [self setWantsLayer:YES];
        // NSColor *backgroundColor = [NSColor blackColor];
        // struct CGColor *bgColor = [backgroundColor CGColor];
        // [[self layer]setBackgroundColor:bgColor];
    }
    
    return(self);
}

-(CVReturn)getFrameForTime: (const CVTimeStamp *)outputTime
{
    // code
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc]init];
    [self drawView];
    [pool release];

    return(kCVReturnSuccess);
}

-(void)prepareOpenGL 
{
    // code
    [super prepareOpenGL];

    // make the OpenGLContext as current context
    [[self openGLContext]makeCurrentContext];

    // set swap interval to 1 to syncronize arrival of buffers in dobule buffering without screen tearing
    GLint swapInterval = 1;
    [[self openGLContext]setValues: &swapInterval forParameter:NSOpenGLCPSwapInterval];
    [self initialize];

    // Start the display link to create the seperate rendering thread
    // step 1 : Create the display link
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);

    // step 2 : set the callback function of this displayLink
    CVDisplayLinkSetOutputCallback(displayLink, &myDisplayLinkCallback, self);

    // step 3 : 
    CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];

    // step 4 :
    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext]CGLContextObj];
    // step 5 : set above created CGLPixelFormat and CGLContext as current displayed CGLPixelFormate and CGLContext for this display link
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);

    // step 6:
    CVDisplayLinkStart(displayLink);
}

-(void)reshape 
{
    // code
    [super reshape];
    [[self openGLContext]makeCurrentContext];
    CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
    NSRect viewRect = [self bounds];
    int width = (int)viewRect.size.width;
    int height = (int)viewRect.size.height;
    [self resize:width :height];
    CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
}

-(void)drawView
{
    // code
    [[self openGLContext]makeCurrentContext];
    CGLLockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
    [self display];
    [self myUpdate];
    CGLUnlockContext((CGLContextObj)[[self openGLContext]CGLContextObj]);
    CGLFlushDrawable((CGLContextObj)[[self openGLContext]CGLContextObj]);  // similar to swap buffers
}

-(void)drawRect:(NSRect)dirtyRect
{
    // code
    [self drawView]; // to avoid the possibility of flickering
}

-(BOOL)acceptsFirstResponder 
{
    // code
    [[self window]makeFirstResponder:self];

    return(YES);
}

-(void)keyDown:(NSEvent *)event
{
    // code
    int key = (int)[[event characters]characterAtIndex:0];

    switch (key) 
    {
        case 27:
            // Window will close is called which will call applicationWillTerminate
            [self uninitialize];
            [self release];
            [NSApp terminate:self];
            break;
        case 'F':
        case 'f':
            [[self window]toggleFullScreen:self];
            break;
        default:
            break;
    }
}

-(void)mouseDown:(NSEvent *)event
{
    // code
}

-(int)initialize
{
    // code
    [self printGLInfo];

	// Depth related code
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// From here OpenGL Code starts
	// Tell OpenGL to choose the color to clear the screen
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    return 0;
}

-(void)printGLInfo
{
    // code
    // print OpenGL Info
	fprintf(gpFile, "OPENGL INFORMATION\n");
	fprintf(gpFile, "--------------------\n");
	fprintf(gpFile, "OpenGL Vendor : %s\n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer : %s\n", glGetString(GL_RENDERER));
	fprintf(gpFile, "OpenGL Version : %s\n", glGetString(GL_VERSION));
	fprintf(gpFile, "GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	fprintf(gpFile, "--------------------\n");
}

-(void)resize:(int)width : (int)height
{
    // code
    if (height < 0) 
    {
        height = 1;
    }

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

-(void)display 
{
    // code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
-(void)myUpdate
{
    // code
}

-(void)uninitialize
{
    // code
    fprintf(gpFile, "Uninitialize is called!\n");
}

-(void)dealloc 
{
    // code
    CVDisplayLinkStop(displayLink);
    CVDisplayLinkRelease(displayLink);
    [super dealloc];
}
@end

CVReturn myDisplayLinkCallback(CVDisplayLinkRef displayLinkRef, const CVTimeStamp *current, const CVTimeStamp *output, CVOptionFlags inputFlags, CVOptionFlags *outputFlags, void *view)
{
    CVReturn result = [(GLView *)view getFrameForTime: output];
    return(result);
}


