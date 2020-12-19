#include "../Includes.h"
#include "../Window.h"
#include "../InputHandler.h"


U8 MageApplicationWindowCreate(MageApplicationWindowCreateInfo* info, MageApplicationWindow* window)
{
	MAGE_HANDLE_ERROR_MESSAGE(info == NULL || window == NULL, printf("Error: Any passed parameters are NULL!\n"));
	
	ZeroMemory(window, sizeof(MageApplicationWindow));

	/* Using primary screen */

	if (info->SpawnOffsetX == 0)
		info->SpawnOffsetX = (GetSystemMetrics(SM_CXSCREEN) - info->Width) / 2;
	if (info->SpawnOffsetY == 0)
		info->SpawnOffsetY = (GetSystemMetrics(SM_CYSCREEN) - info->Height) / 2;

	/* Todo, allow user customisation */
	DWORD windowStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle     = WS_CAPTION | WS_SYSMENU | WS_VISIBLE;


	ZeroMemory(&window->NativeWindowClass, sizeof(WNDCLASSEX));
	window->NativeWindowClass.cbSize        = sizeof(window->NativeWindowClass);
	window->NativeWindowClass.style         = CS_HREDRAW | CS_VREDRAW;
	window->NativeWindowClass.lpfnWndProc   = MageInputHandlerWindowsEventListener;
	window->NativeWindowClass.hInstance	    = window->Instance;
	window->NativeWindowClass.hIcon		    = LoadIcon(NULL, IDI_APPLICATION);
	window->NativeWindowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	window->NativeWindowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	window->NativeWindowClass.lpszClassName = TEXT("MainWindow");
	window->NativeWindowClass.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
	
	ATOM result = RegisterClassEx(&window->NativeWindowClass);
	MAGE_HANDLE_ERROR_MESSAGE(!result, printf("Error: Failed to register win32 class!\n"));

	if (info->Resisable)
		dwStyle |= WS_THICKFRAME;
	
	window->NativeWindow = CreateWindowEx(
		windowStyle, 
		TEXT("MainWindow"),
		TEXT(info->Title), 
		dwStyle, 
		info->SpawnOffsetX, 
		info->SpawnOffsetY, 
		info->Width,
		info->Height, 
		NULL, 
		NULL,
		window->Instance,
		NULL
	);

	printf("Inform: Created Win32 windows\n");
	return MageTrue;
}
U8 MageApplicationWindowDestroy(MageApplicationWindow* window)
{
	DestroyWindow(window->NativeWindow);
	return MageTrue;
}