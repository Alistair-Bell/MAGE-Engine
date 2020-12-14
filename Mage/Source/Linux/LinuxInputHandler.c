#include "../Includes.h"
#include "../InputHandler.h"

U8 MageInputHandlerCreate(MageInputHandlerCreateInfo *info, MageInputHandler *handler)
{
    MAGE_HANDLE_ERROR_MESSAGE(info == NULL || handler == NULL, printf("Error: Any passed parameters are NULL!\n"));
    memset(handler, 0, sizeof(MageInputHandler));
    MAGE_HANDLE_ERROR_MESSAGE(info->ApplicationWindow == NULL, printf("Error: Passed application window is NULL!\n"));

    /* See flags https://tronche.com/gui/x/xlib/events/mask.html */

    I64 xflags = 0;
    if (info->InputFlags & MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_KEYBOARD)
        xflags |= (KeyPressMask | KeyReleaseMask);
    if (info->InputFlags & MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_MOUSE)
        xflags |= (ButtonPressMask | ButtonReleaseMask | PointerMotionHintMask);
    if (info->InputFlags & MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_WINDOW_EVENTS)
        xflags |= (EnterWindowMask | LeaveWindowMask | VisibilityChangeMask | ResizeRedirectMask | SubstructureNotifyMask);
    XSelectInput(info->ApplicationWindow->WindowDisplay, info->ApplicationWindow->ContextWindow, xflags);
    return MageTrue;
}
U8 MageInputHandlerPollEvents(MageInputHandler *handler)
{
    
    return MageTrue;
}
U8 MageInputHandlerInterrogateKeyState(MageInputHandler *handler, const MageKeyboardKey key, const MageButtonState state)
{

    return MageTrue;
}
U8 MageInputHandlerInterrogateMouseButtonState(MageInputHandler *handler, const MageMouseButton button, const MageButtonState state)
{

    return MageTrue;
}
U8 MageInputHandlerDestroy(MageInputHandler *handler)
{

    return MageTrue;
}