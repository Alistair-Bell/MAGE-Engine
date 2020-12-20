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
	switch (code)
	{



	}
}
U8 MageInputHandlerDestroy(MageInputHandler* handler)
{
	return MageTrue;
}
