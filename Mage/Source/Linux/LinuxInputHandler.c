#include "../Includes.h"
#include "../InputHandler.h"

U8 MageInputHandlerCreate(MageInputHandlerCreateInfo *info, MageInputHandler *handler)
{
    MAGE_HANDLE_ERROR_MESSAGE(info == NULL || handler == NULL, printf("Error: Any passed parameters are NULL!\n"));
    memset(handler, 0, sizeof(MageInputHandler));
    MAGE_HANDLE_ERROR_MESSAGE(info->ApplicationWindow == NULL, printf("Error: Passed application window is NULL!\n"));

    /* See flags https://tronche.com/gui/x/xlib/events/mask.html */

    I64 eventMask = 0;
    if (info->InputFlags & MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_KEYBOARD)
        eventMask |= (KeyPressMask | KeyReleaseMask); printf("Here1\n");
    if (info->InputFlags & MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_MOUSE)
        eventMask |= (ButtonPressMask | ButtonReleaseMask | PointerMotionHintMask | PointerMotionMask);
    if (info->InputFlags & MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_WINDOW_EVENTS)
        eventMask |= (EnterWindowMask | LeaveWindowMask | VisibilityChangeMask | ResizeRedirectMask | SubstructureNotifyMask);
    
    XSelectInput(info->ApplicationWindow->WindowDisplay, info->ApplicationWindow->ContextWindow, eventMask);
    return MageTrue;
}
U8 MageInputHandlerPollEvents(MageInputHandler *handler)
{
    
    return MageTrue;
}

U8 MageInputHandlerDestroy(MageInputHandler *handler)
{

    return MageTrue;
}