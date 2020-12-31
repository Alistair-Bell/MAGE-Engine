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
    
    if (info->Flags & MAGE_APPLICATION_WINDOW_CREATE_FLAGS_FULLSCREEN)
    {
        Atom windowState, fullscreen;
        windowState = XInternAtom(window->WindowDisplay, "_NET_WM_STATE", MageFalse);
        fullscreen  = XInternAtom(window->WindowDisplay, "_NET_WM_STATE_FULLSCREEN", MageFalse);
        XEvent staging;
        memset(&staging, 0, sizeof(XEvent));
        staging.type                 = ClientMessage;
        staging.xclient.window       = window->ContextWindow;
        staging.xclient.message_type = windowState;
        staging.xclient.format       = 32;
        staging.xclient.data.l[0]    = ((info->Flags & MAGE_APPLICATION_WINDOW_CREATE_FLAGS_FULLSCREEN) ? 1 : 0);
        staging.xclient.data.l[1]    = fullscreen;
        staging.xclient.data.l[2]    = 0;
        staging.xclient.data.l[3]    = 0;
        XMapWindow(window->WindowDisplay, window->ContextWindow);
        XSendEvent(window->WindowDisplay, DefaultRootWindow(window->WindowDisplay), MageFalse, SubstructureRedirectMask | SubstructureNotifyMask, &staging);
        XFlush(window->WindowDisplay);
    }

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
    XDestroyWindow(window->WindowDisplay, window->ContextWindow);
    XCloseDisplay(window->WindowDisplay);
    return MageTrue;
}
