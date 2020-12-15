#include "../Window.h"
#include "../CreateStructures.h"
#include "../Includes.h"


U8 MageApplicationWindowCreate(MageApplicationWindowCreateInfo *info, MageApplicationWindow *window)
{
    MAGE_HANDLE_ERROR_MESSAGE(info == NULL || window == NULL, printf("Error: Any passed parameters are NULL!\n"))
    
    XInitThreads();

    window->WindowDisplay = XOpenDisplay(NULL);
    window->RootWindow = DefaultRootWindow(window->WindowDisplay);
    
    MAGE_HANDLE_ERROR_MESSAGE(window->WindowDisplay == NULL, printf("Error: Failed to open X11 window display\n"));

    window->ContextWindow = XCreateWindow(
        window->WindowDisplay,
        window->RootWindow,
        info->SpawnOffsetX,
        info->SpawnOffsetY,
        (I32)info->Width,
        (I32)info->Height,
        0,
        0,
        InputOutput,
        CopyFromParent,
        0,
        NULL
    );

    XMapWindow(window->WindowDisplay, window->ContextWindow);
    XGrabPointer(
        window->WindowDisplay, 
        window->ContextWindow,
        MageTrue, ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
        GrabModeAsync,
        GrabModeAsync,
        window->ContextWindow,
        window->ContextCursor,
        CurrentTime
    );
    return MageTrue;
}
U8 MageApplicationWindowDestroy(MageApplicationWindow *window)
{
    XCloseDisplay(window->WindowDisplay);
    XDestroyWindow(window->WindowDisplay, window->ContextWindow);
    return MageTrue;
}