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

    XSetWindowAttributes attributes;
    memset(&attributes, 0, sizeof(XSetWindowAttributes));

    if (!(info->Flags & MAGE_APPLICATION_WINDOW_CREATE_FLAGS_ALLOW_RESIZING))
        attributes.override_redirect = MageTrue; 

    if (info->Flags & MAGE_APPLICATION_WINDOW_CREATE_FLAGS_AUTO_CENTRE)
    {
        XWindowAttributes dimensions;
        XGetWindowAttributes(window->WindowDisplay, window->RootWindow, &dimensions);
        info->SpawnOffsetX = (dimensions.width- info->Width)  / 2;
        info->SpawnOffsetY = (dimensions.height- info->Height) / 2;
    }

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
        CWOverrideRedirect,
        &attributes
    );

    XMapWindow(window->WindowDisplay, window->ContextWindow);
    XStoreName(window->WindowDisplay, window->ContextWindow, info->Title);

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
U8 MageApplicationWindowSetTitle(MageApplicationWindow *window, const char *newName)
{
    return XStoreName(window->WindowDisplay, window->ContextWindow, newName);
}
U8 MageApplicationWindowDestroy(MageApplicationWindow *window)
{
    XCloseDisplay(window->WindowDisplay);
    XDestroyWindow(window->WindowDisplay, window->ContextWindow);
    return MageTrue;
}