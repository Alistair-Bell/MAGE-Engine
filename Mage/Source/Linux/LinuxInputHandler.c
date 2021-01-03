#include "../Includes.h"
#include "../InputHandler.h"

static U8 MageInputHandleResize(MageInputHandler *handler, MageApplicationWindow *window, const I16 width, const I16 height)
{
    printf("Inform: New window size [%d:%d]\n", width, height);
    return MageTrue;
}

/* 
            mapKeys[0x61] = Key::A; mapKeys[0x62] = Key::B; mapKeys[0x63] = Key::C; mapKeys[0x64] = Key::D; mapKeys[0x65] = Key::E;
			mapKeys[0x66] = Key::F; mapKeys[0x67] = Key::G; mapKeys[0x68] = Key::H; mapKeys[0x69] = Key::I; mapKeys[0x6A] = Key::J;
			mapKeys[0x6B] = Key::K; mapKeys[0x6C] = Key::L; mapKeys[0x6D] = Key::M; mapKeys[0x6E] = Key::N; mapKeys[0x6F] = Key::O;
			mapKeys[0x70] = Key::P; mapKeys[0x71] = Key::Q; mapKeys[0x72] = Key::R; mapKeys[0x73] = Key::S; mapKeys[0x74] = Key::T;
			mapKeys[0x75] = Key::U; mapKeys[0x76] = Key::V; mapKeys[0x77] = Key::W; mapKeys[0x78] = Key::X; mapKeys[0x79] = Key::Y;
			mapKeys[0x7A] = Key::Z;



*/

MageKeyboardKey MageInputHandlerTranslateKeyCodes(const U64 code)
{
    switch (code)
    {
        case 0x61: return MAGE_KEYBOARD_KEY_A;
        case 0x62: return MAGE_KEYBOARD_KEY_B;
        case 0x63: return MAGE_KEYBOARD_KEY_C;
        case 0x64: return MAGE_KEYBOARD_KEY_D;
        case 0x65: return MAGE_KEYBOARD_KEY_E;
        case 0x66: return MAGE_KEYBOARD_KEY_F;
        case 0x67: return MAGE_KEYBOARD_KEY_G;
        case 0x68: return MAGE_KEYBOARD_KEY_H;
        case 0x69: return MAGE_KEYBOARD_KEY_I;
        case 0x6A: return MAGE_KEYBOARD_KEY_J;
        case 0x6B: return MAGE_KEYBOARD_KEY_K;
        case 0x6C: return MAGE_KEYBOARD_KEY_L;
        case 0x6D: return MAGE_KEYBOARD_KEY_M;
        case 0x6E: return MAGE_KEYBOARD_KEY_N;
        case 0x6F: return MAGE_KEYBOARD_KEY_O;
        case 0x70: return MAGE_KEYBOARD_KEY_P;
        case 0x71: return MAGE_KEYBOARD_KEY_Q;
        case 0x72: return MAGE_KEYBOARD_KEY_R;
        case 0x73: return MAGE_KEYBOARD_KEY_S;
        case 0x74: return MAGE_KEYBOARD_KEY_T;
        case 0x75: return MAGE_KEYBOARD_KEY_U;
        case 0x76: return MAGE_KEYBOARD_KEY_V;
        case 0x77: return MAGE_KEYBOARD_KEY_W;
        case 0x78: return MAGE_KEYBOARD_KEY_X;
        case 0x79: return MAGE_KEYBOARD_KEY_Y;
        case 0x7A: return MAGE_KEYBOARD_KEY_Z;

        case XK_Up:    return MAGE_KEYBOARD_KEY_DIRECTIONAL_UP;
        case XK_Left:  return MAGE_KEYBOARD_KEY_DIRECTIONAL_LEFT;
        case XK_Right: return MAGE_KEYBOARD_KEY_DIRECTIONAL_RIGHT;
        case XK_Down:  return MAGE_KEYBOARD_KEY_DIRECTIONAL_DOWN;

        case XK_BackSpace: return MAGE_KEYBOARD_KEY_BACKSPACE;
        case XK_Escape:    return MAGE_KEYBOARD_KEY_ESCAPE;
        case XK_Linefeed:  return MAGE_KEYBOARD_KEY_ENTER;


        case XK_F1:  return MAGE_KEYBOARD_KEY_F1;
        case XK_F2:  return MAGE_KEYBOARD_KEY_F2;
        case XK_F3:  return MAGE_KEYBOARD_KEY_F3;
        case XK_F4:  return MAGE_KEYBOARD_KEY_F4;
        case XK_F5:  return MAGE_KEYBOARD_KEY_F5;
        case XK_F6:  return MAGE_KEYBOARD_KEY_F6;
        case XK_F7:  return MAGE_KEYBOARD_KEY_F7;
        case XK_F8:  return MAGE_KEYBOARD_KEY_F8;
        case XK_F9:  return MAGE_KEYBOARD_KEY_F9;
        case XK_F10: return MAGE_KEYBOARD_KEY_F10;
        case XK_F11: return MAGE_KEYBOARD_KEY_F11;
        case XK_F12: return MAGE_KEYBOARD_KEY_F12;

    }
    return MAGE_KEYBOARD_KEY_UNDEFINED;
}


U8 MageInputHandlerCreate(MageInputHandlerCreateInfo *info, MageInputHandler *handler)
{
    MAGE_HANDLE_ERROR_MESSAGE(info == NULL || handler == NULL, printf("Error: Any passed parameters are NULL!\n"));
    memset(handler, 0, sizeof(MageInputHandler));
    MAGE_HANDLE_ERROR_MESSAGE(info->ApplicationWindow == NULL, printf("Error: Passed application window is NULL!\n"));

    /* See flags https://tronche.com/gui/x/xlib/events/mask.html */

    MageApplicationWindow *w = info->ApplicationWindow;

    I64 eventMask = 0;
    if (info->InputFlags & MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_KEYBOARD)
        eventMask |= (KeyPressMask | KeyReleaseMask);
    if (info->InputFlags & MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_MOUSE)
        eventMask |= (ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
    if (info->InputFlags & MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_WINDOW_EVENTS)
    {
        eventMask |= (SubstructureNotifyMask | FocusChangeMask | ExposureMask);
        Atom windowDelete = XInternAtom(w->WindowDisplay, "WM_DELETE_WINDOW", MageTrue);
        XSetWMProtocols(w->WindowDisplay, w->ContextWindow, &windowDelete, MageTrue);
    }

    XSelectInput(info->ApplicationWindow->WindowDisplay, info->ApplicationWindow->ContextWindow, eventMask);
    return MageTrue;
}
U8 MageInputHandlerPollEvents(MageInputHandler *handler, MageApplicationWindow *window)
{

    /* The inevitable thing in life is native frameworks being a clusterfuck */

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
                KeySym sym = XLookupKeysym(&e->xkey, 0);
                handler->KeyboardStates[MageInputHandlerTranslateKeyCodes((U64)sym)] |= MAGE_BUTTON_STATE_PRESS; 
                break;
            }
            case KeyRelease:
            {
                KeySym sym = XLookupKeysym(&e->xkey, 0);
                handler->KeyboardStates[MageInputHandlerTranslateKeyCodes((U64)sym)] |= MAGE_BUTTON_STATE_RELEASE; 
                break;
            }
            case ButtonPress:
            {
               handler->MouseStates[e->xbutton.button] |= MAGE_BUTTON_STATE_PRESS;
               break;
            }
            case ButtonRelease:
            {
                handler->MouseStates[e->xbutton.button] |= MAGE_BUTTON_STATE_RELEASE; 
                break;
            }
            case MotionNotify:
            {
                handler->MousePositionX = e->xmotion.x;
                handler->MousePositionY = e->xmotion.y;
                printf("Inform: [%d:%d]\n", (I32)handler->MousePositionX, (I32)handler->MousePositionY);
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
U8 MageApplicationWindowGetDimensions(MageApplicationWindow *window, MageApplicationWindowDimensions *dimensions)
{
    XWindowAttributes a;
    XGetWindowAttributes(window->WindowDisplay, window->ContextWindow, &a);
    dimensions->Width     = a.width;
    dimensions->Height    = a.height;
    dimensions->PositionX = a.x;
    dimensions->PositionY = a.y;
    return MageTrue;
}
U8 MageApplicationWindowGetFramebufferDimensions(MageApplicationWindow *window, MageApplicationWindowDimensions *dimensions)
{
    return MageApplicationWindowGetDimensions(window, dimensions);
}

U8 MageInputHandlerDestroy(MageInputHandler *handler)
{
    return MageTrue;
}
