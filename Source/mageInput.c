#include "mageAPI.h"

void mageGetMousePosition(mageWindow *window, double *x, double *y)
{
	
	#if defined(MAGE_SDL2)
	
	#elif defined(MAGE_GLFW)
		glfwGetCursorPos(window->Context, x, y);
	#endif
}
void mageGetMousePositionX(mageWindow *window, double *x)
{
	#if defined(MAGE_SDL2)
		
	#elif defined(MAGE_GLFW)
		glfwGetCursorPos(window->Context, x, NULL);
	#endif
}
void mageGetMousePositionY(mageWindow *window, double *y)
{
	#if defined(MAGE_SDL2)
	
	#elif defined(MAGE_GLFW)
		glfwGetCursorPos(window->Context, NULL, y);
	#endif
}
void mageGetMouseButtonStates(mageWindow *window, sint32 *left, sint32 *right)
{
	sint32 foo, bar;
	
	#if defined(MAGE_SDL2)
	
	#elif defined(MAGE_GLFW)
		foo = glfwGetMouseButton(window->Context, GLFW_MOUSE_BUTTON_LEFT);
		bar = glfwGetMouseButton(window->Context, GLFW_MOUSE_BUTTON_RIGHT); 
	#endif
	
	mageTryDumpSuccess(foo, left);
	mageTryDumpSuccess(bar, right);
	
}
void mageGetMouseButtonLeftClick(mageWindow *window, uint8 *state)
{
	sint32 value;
	uint8 is;
	#if defined(MAGE_SDL2)
	
	#elif defined(MAGE_GLFW)
		mageGetMouseButtonStates(window, &value, NULL);
		is = (value == GLFW_PRESS);
	#endif
	
	mageTryDumpSuccess(is, state);
}
void mageGetMouseButtonRightClick(mageWindow *window, uint8 *state)
{
	sint32 value;
	uint8 is;
	#if defined(MAGE_SDL2)
	
	#elif defined(MAGE_GLFW)
		mageGetMouseButtonStates(window, NULL, &value);
		is = (value == GLFW_PRESS);
	#endif

	mageTryDumpSuccess(is, state);
}
void mageGetMouseInsideContext(mageWindow *window, uint8 *state)
{
	sint32 is;
	#if defined(MAGE_SDL2)
	
	#elif defined(MAGE_GLFW)
		is = glfwGetWindowAttrib(window->Context, GLFW_HOVERED);
	#endif
	
	mageTryDumpSuccess(is, state);
}
void mageSetMousePosition(mageWindow *window, const double x, const double y)
{
	#if defined(MAGE_SDL2)

	#elif defined(MAGE_GLFW)
		glfwSetCursorPos(window->Context, x, y);
	#endif
}
void mageGetKeyDown(mageWindow *window, const sint32 key, uint8 *state)
{
	uint32 value;
	uint8 is;
	
	#if defined(MAGE_SDL2)
	
	#elif defined(MAGE_GLFW)
		value = glfwGetKey(window->Context, key);
		is = (value == GLFW_PRESS || value == GLFW_REPEAT);
	#endif
	
	mageTryDumpSuccess(is, state);
}
void mageGetKeyNotDown(mageWindow *window, const sint32 key, uint8 *state)
{
	uint32 value;
	uint8 is;
	
	#if defined(MAGE_SDL2)
	
	#elif defined(MAGE_GLFW)
		value = glfwGetKey(window->Context, key);
		is = !(value == GLFW_PRESS || value == GLFW_REPEAT);
	#endif
	
	mageTryDumpSuccess(is, state);
}





