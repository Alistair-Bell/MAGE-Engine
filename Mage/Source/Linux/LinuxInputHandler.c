#include "../Includes.h"
#include "../InputHandler.h"

static U8 MageInputHandleResize(MageInputHandler *handler, MageApplicationWindow *window, const I16 width, const I16 height)
{
    printf("Inform: New window size [%d:%d]\n", width, height);
    return MageTrue;
}

U8 MageInputHandlerCreate(MageInputHandlerCreateInfo *info, MageInputHandler *handler)
{
    MAGE_HANDLE_ERROR_MESSAGE(info == NULL || handler == NULL, printf("Error: Any passed parameters are NULL!\n"));
    memset(handler, 0, sizeof(MageInputHandler));
    MAGE_HANDLE_ERROR_MESSAGE(info->ApplicationWindow == NULL, printf("Error: Passed application window is NULL!\n"));

    /* See flags https://tronche.com/gui/x/xlib/events/mask.html */

    I64 eventMask = 0;
    if (info->InputFlags & MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_KEYBOARD)
        eventMask |= (KeyPressMask | KeyReleaseMask);
    if (info->InputFlags & MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_MOUSE)
        eventMask |= (ButtonPressMask | ButtonReleaseMask | PointerMotionHintMask | PointerMotionMask);
    if (info->InputFlags & MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_WINDOW_EVENTS)
        eventMask |= (EnterWindowMask | LeaveWindowMask | VisibilityChangeMask | ResizeRedirectMask | SubstructureNotifyMask | ExposureMask | FocusChangeMask);
    
    XSelectInput(info->ApplicationWindow->WindowDisplay, info->ApplicationWindow->ContextWindow, eventMask);
    return MageTrue;
}
U8 MageInputHandlerPollEvents(MageInputHandler *handler, MageApplicationWindow *window)
{
    XEvent *e = &window->PollingEvent;
    while (XPending(window->WindowDisplay))
    {
        XNextEvent(window->WindowDisplay, e);
        switch (e->type)
        {
            case Expose:
            {
                XWindowAttributes a;
                XGetWindowAttributes(window->WindowDisplay, window->ContextWindow, &a);
                MageInputHandleResize(handler, window, a.width, a.height);
                break;
            }
            case KeyPress:
            {
                break;
            }
            case KeyRelease:
            {
                break;
            }
            case ButtonPress:
            {
                break;
            }
            case ButtonRelease:
            {
                break;
            }
            case MotionNotify:
            {
                break;
            }
            case ClientMessage:
            {
                printf("Inform: Terminating window\n");
                return MageFalse;
            }

        }
    }    
    return MageTrue;
}

U8 MageInputHandlerDestroy(MageInputHandler *handler)
{
    return MageTrue;
}