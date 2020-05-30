#include "mageAPI.h"

double mageGetMousePositionX(mageWindow *window)
{
	#if defined(MAGE_SDL2)
		return window->Events.motion.x;
	#elif defined(MAGE_GLFW)
		double x;
		glfwGetCursorPos(window->Context, &x, NULL);
		return x;
	#endif
	return 0.0;
}
double mageGetMousePositionY(mageWindow *window)
{
	#if defined(MAGE_SDL2)
		return window->Events.motion.y;
	#elif defined(MAGE_GLFW)
		double y;
		glfwGetCursorPos(window->Context, NULL, &y);
		return y;
	#endif
	return 0.0;
}
uint8_t mageGetMouseButtonLeftClick(mageWindow *window)
{

	#if defined(MAGE_SDL2)
		
	#elif defined(MAGE_GLFW)
		return (glfwGetMouseButton(window->Context, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
	#endif
	return 0;
}
uint8_t mageGetMouseButtonRightClick(mageWindow *window)
{
	#if defined(MAGE_SDL2)
		
	#elif defined(MAGE_GLFW)
		return (glfwGetMouseButton(window->Context, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
	#endif
	return 0;
}
uint8_t mageGetMouseInsideContext(mageWindow *window)
{
	#if defined(MAGE_SDL2)
	
	#elif defined(MAGE_GLFW)
		return glfwGetWindowAttrib(window->Context, GLFW_HOVERED);
	#endif
	return 0;
}
void mageSetMousePosition(mageWindow *window, const double x, const double y)
{
	#if defined(MAGE_SDL2)

	#elif defined(MAGE_GLFW)
		glfwSetCursorPos(window->Context, x, y);
	#endif
}
uint8_t mageGetKeyDown(mageWindow *window, const int32_t key)
{
	
	#if defined(MAGE_SDL2)
	
	#elif defined(MAGE_GLFW)
		uint8_t value = glfwGetKey(window->Context, key);
		return value == GLFW_PRESS || value == GLFW_REPEAT;
	#endif
	return 0;
}
uint8_t mageGetKeyNotDown(mageWindow *window, const int32_t key)
{
#if defined(MAGE_SDL2)
	
	#elif defined(MAGE_GLFW)
		uint8_t value = glfwGetKey(window->Context, key);
		return !(value == GLFW_PRESS || value == GLFW_REPEAT);
	#endif
	return 0;
}





