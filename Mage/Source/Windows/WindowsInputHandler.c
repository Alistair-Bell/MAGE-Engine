#include "../Includes.h"
#include "../InputHandler.h"

static MageInputHandler *StaticHandler;

U8 MageInputHandlerCreate(MageInputHandlerCreateInfo *info, MageInputHandler *handler)
{ 
	memset(handler, 0, sizeof(MageInputHandler));
	StaticHandler = handler;
	return MageTrue;
}
LRESULT CALLBACK MageInputHandlerWindowsEventListener(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(MageFalse);
			return MageFalse;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(MageFalse);
			return MageFalse;
		}
		case WM_MOUSEWHEEL:
		{
			StaticHandler->MouseWheelPosition = (I32)GET_WHEEL_DELTA_WPARAM(wParam);
			break;
		}
		case WM_MOUSELEAVE:
		{
			StaticHandler->MouseFocused = 0;
			break;
		}
		case WM_MOUSEMOVE:
		{
			StaticHandler->MousePositionX = (I32)GET_X_LPARAM(lParam);
			StaticHandler->MousePositionY = (I32)GET_Y_LPARAM(lParam);
			break;
		}
		case WM_KEYDOWN:
		{
			printf("Inform: Keydown %d\n", (I32)wParam);
			break;
		}

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
U8 MageInputHandlerPollEvents(MageInputHandler *handler, MageApplicationWindow *window)
{
	MSG message;
	while (MageTrue)
	{
		while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);

			DispatchMessage(&message);
		}
		if (message.message == WM_QUIT) break;
	}
	return MageFalse;
}
MageKeyboardKey MageInputHandlerTranslateKeyCodes(MageInputHandler *handler, const U64 code)
{
	/* Stole from https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/olcPixelGameEngine.h, thanks Javid*/
	switch (code)
	{
		case 0x41:       return MAGE_KEYBOARD_KEY_A;
		case 0x42:       return MAGE_KEYBOARD_KEY_B;
		case 0x43:       return MAGE_KEYBOARD_KEY_C;
		case 0x44:       return MAGE_KEYBOARD_KEY_D;
		case 0x45:       return MAGE_KEYBOARD_KEY_E;
		case 0x46:       return MAGE_KEYBOARD_KEY_F;
		case 0x47:       return MAGE_KEYBOARD_KEY_G;
		case 0x48:       return MAGE_KEYBOARD_KEY_H;
		case 0x49:       return MAGE_KEYBOARD_KEY_I;
		case 0x4A:       return MAGE_KEYBOARD_KEY_J;
		case 0x4B:       return MAGE_KEYBOARD_KEY_K;
		case 0x4C:       return MAGE_KEYBOARD_KEY_L;
		case 0x4D:       return MAGE_KEYBOARD_KEY_M;
		case 0x4E:       return MAGE_KEYBOARD_KEY_N;
		case 0x4F:       return MAGE_KEYBOARD_KEY_O;
		case 0x50:       return MAGE_KEYBOARD_KEY_P;
		case 0x51:       return MAGE_KEYBOARD_KEY_Q;
		case 0x52:       return MAGE_KEYBOARD_KEY_R;
		case 0x53:       return MAGE_KEYBOARD_KEY_S;
		case 0x54:       return MAGE_KEYBOARD_KEY_T;
		case 0x55:       return MAGE_KEYBOARD_KEY_U;
		case 0x56:       return MAGE_KEYBOARD_KEY_V;
		case 0x57:       return MAGE_KEYBOARD_KEY_W;
		case 0x58:       return MAGE_KEYBOARD_KEY_X;
		case 0x59:       return MAGE_KEYBOARD_KEY_Y;
		case 0x5A:       return MAGE_KEYBOARD_KEY_Z;
		
		case 0x30:       return MAGE_KEYBOARD_KEY_0;
		case 0x31:       return MAGE_KEYBOARD_KEY_1;
		case 0x32:       return MAGE_KEYBOARD_KEY_2;
		case 0x33:       return MAGE_KEYBOARD_KEY_3;
		case 0x34:       return MAGE_KEYBOARD_KEY_4;
		case 0x35:       return MAGE_KEYBOARD_KEY_5;
		case 0x36:       return MAGE_KEYBOARD_KEY_6;
		case 0x37:       return MAGE_KEYBOARD_KEY_7;
		case 0x38:       return MAGE_KEYBOARD_KEY_8;
		case 0x39:       return MAGE_KEYBOARD_KEY_9;
		
		case VK_F1:      return MAGE_KEYBOARD_KEY_F1;
		case VK_F2:      return MAGE_KEYBOARD_KEY_F2;
		case VK_F3:      return MAGE_KEYBOARD_KEY_F3;
		case VK_F4:      return MAGE_KEYBOARD_KEY_F4;
		case VK_F5:      return MAGE_KEYBOARD_KEY_F5;
		case VK_F6:      return MAGE_KEYBOARD_KEY_F6;
		case VK_F7:      return MAGE_KEYBOARD_KEY_F7;
		case VK_F8:      return MAGE_KEYBOARD_KEY_F8;
		case VK_F9:      return MAGE_KEYBOARD_KEY_F9;
		case VK_F10:     return MAGE_KEYBOARD_KEY_F10;
		case VK_F11:     return MAGE_KEYBOARD_KEY_F11;
		case VK_F12:     return MAGE_KEYBOARD_KEY_F12;

		case VK_DOWN:    return MAGE_KEYBOARD_KEY_DIRECTIONAL_DOWN;
		case VK_LEFT:    return MAGE_KEYBOARD_KEY_DIRECTIONAL_LEFT;
		case VK_RIGHT:   return MAGE_KEYBOARD_KEY_DIRECTIONAL_RIGHT;
		case VK_UP:      return MAGE_KEYBOARD_KEY_DIRECTIONAL_UP;
		case VK_RETURN:  return MAGE_KEYBOARD_KEY_ENTER;
						 
		case VK_BACK:    return MAGE_KEYBOARD_KEY_BACKSPACE;
		case VK_ESCAPE:  return MAGE_KEYBOARD_KEY_ESCAPE;
		case VK_PAUSE:   return MAGE_KEYBOARD_KEY_PAUSE;
		case VK_SCROLL:  return MAGE_KEYBOARD_KEY_SCROLL_LOCK;
		case VK_TAB:     return MAGE_KEYBOARD_KEY_TAB;
		case VK_DELETE:  return MAGE_KEYBOARD_KEY_DELETE;
		case VK_HOME:    return MAGE_KEYBOARD_KEY_HOME;
		case VK_END:     return MAGE_KEYBOARD_KEY_END;
		case VK_PRIOR:   return MAGE_KEYBOARD_KEY_PAGE_UP;
		case VK_NEXT:    return MAGE_KEYBOARD_KEY_PAGE_DOWN;
		case VK_INSERT:  return MAGE_KEYBOARD_KEY_INSERT;
		case VK_SHIFT:   return MAGE_KEYBOARD_KEY_LEFT_SHIFT;
		case VK_CONTROL: return MAGE_KEYBOARD_KEY_LEFT_CONTROL;
		case VK_SPACE:   return MAGE_KEYBOARD_KEY_SPACE;

		case VK_NUMPAD0:   return MAGE_KEYBOARD_KEY_KEYPAD_0;
		case VK_NUMPAD1:   return MAGE_KEYBOARD_KEY_KEYPAD_1;
		case VK_NUMPAD2:   return MAGE_KEYBOARD_KEY_KEYPAD_2;
		case VK_NUMPAD3:   return MAGE_KEYBOARD_KEY_KEYPAD_3;
		case VK_NUMPAD4:   return MAGE_KEYBOARD_KEY_KEYPAD_4;
		case VK_NUMPAD5:   return MAGE_KEYBOARD_KEY_KEYPAD_5;
		case VK_NUMPAD6:   return MAGE_KEYBOARD_KEY_KEYPAD_6;
		case VK_NUMPAD7:   return MAGE_KEYBOARD_KEY_KEYPAD_7;
		case VK_NUMPAD8:   return MAGE_KEYBOARD_KEY_KEYPAD_8;
		case VK_NUMPAD9:   return MAGE_KEYBOARD_KEY_KEYPAD_9;
		case VK_MULTIPLY:  return MAGE_KEYBOARD_KEY_MULTIPLY;
		case VK_ADD:       return MAGE_KEYBOARD_KEY_ADD;
		case VK_DIVIDE:    return MAGE_KEYBOARD_KEY_DIVIDE;
		case VK_SUBTRACT:  return MAGE_KEYBOARD_KEY_SUBTRACT;
		case VK_DECIMAL:   return MAGE_KEYBOARD_KEY_PERIOD;

		case VK_OEM_1:     return MAGE_KEYBOARD_KEY_SEMI_COLON;
		case VK_OEM_2:     return MAGE_KEYBOARD_KEY_SLASH;
		case VK_OEM_5:     return MAGE_KEYBOARD_KEY_BACKSLASH;
		default: return MAGE_KEYBOARD_KEY_UNDEFINED;
	}
}
MageMouseButton MageInputHandlerTranslateMouseCode(MageInputHandler *handler, const U64 code)
{
	switch (code)
	{
		case VK_LBUTTON: return MAGE_MOUSE_BUTTON_LEFT;
		case VK_RBUTTON: return MAGE_MOUSE_BUTTON_RIGHT;
		case VK_MBUTTON: return MAGE_MOUSE_BUTTON_MIDDLE;
		default: return MAGE_MOUSE_BUTTON_UNDEFINED;
	}
}
U8 MageInputHandlerDestroy(MageInputHandler* handler)
{
	return MageTrue;
}
