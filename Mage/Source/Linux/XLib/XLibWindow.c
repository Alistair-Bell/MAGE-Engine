#include "../../Window.h"

U8 MageApplicationWindowCreate(MageApplicationWindowCreateInfo *info, MageApplicationWindow *window)
{
    MAGE_HANDLE_ERROR_MESSAGE(info == NULL || window == NULL, printf("Error: Any passed parameters are NULL!\n"))
    
    XInitThreads();

    window->WindowDisplay = XOpenDisplay(NULL);
    MAGE_HANDLE_ERROR_MESSAGE(window->WindowDisplay == NULL, printf("Error: Failed to open X11 display\n"));
    window->RootWindow = DefaultRootWindow(window->WindowDisplay);

    XSetWindowAttributes attributes;
    memset(&attributes, 0, sizeof(XSetWindowAttributes));

    if (!(info->Flags & MAGE_APPLICATION_WINDOW_CREATE_FLAGS_ALLOW_RESIZING))
        attributes.override_redirect = MageTrue; 

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
    
    if (info->Flags & MAGE_APPLICATION_WINDOW_CREATE_FLAGS_AUTO_CENTRE)
    {
        U32 w = XDisplayWidth(window->WindowDisplay,  0);
        U32 h = XDisplayHeight(window->WindowDisplay, 0);
        XMoveWindow(window->WindowDisplay, window->ContextWindow, (w - info->Width) / 2, (h - info->Height) / 2);
    }
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
