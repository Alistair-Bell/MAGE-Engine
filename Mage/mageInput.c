#include "mageAPI.h"

#if defined(MAGE_GLFW)
	#define MAGE_KEY_COUNT 350
#elif #defined(MAGE_SDL2)
	#define MAGE_KEY_COUNT 1024
#endif


/*!************************ 
 * BIT INFO
 * 7 - Unused
 * 6 - Unused
 * 5 - Unused
 * 4 - Unused
 * 3 - Unused 
 * 2 - Repeat
 * 1 - Released
 * 0 - Pressed
**************************/
typedef uint8_t mageKeyHandle;

typedef struct MAGE_INPUT_SYSTEM
{
	mageKeyHandle Keys[MAGE_KEY_COUNT];
	double MouseXPosition;
	double MouseYPosition;

} mageInput;

static mageInput InputHandle;


static mageKeyHandle mageCreateKeyHandle(const uint8_t pressed, const uint8_t release, const uint8_t repeat)
{
	mageKeyHandle handle;
	memset(&handle, 0, sizeof(mageKeyHandle));
	handle |= pressed << 0;
	handle |= repeat << 1;
	return handle;
}

#if defined(MAGE_GLFW)

	static void mageGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		switch (action)
		{
			case GLFW_PRESS:
				InputHandle.Keys[key] = mageCreateKeyHandle(1, 0, 0);
				break;

			case GLFW_REPEAT:
				InputHandle.Keys[key] = mageCreateKeyHandle(1, 0, 1);
				break;

			case GLFW_RELEASE:
				InputHandle.Keys[key] = mageCreateKeyHandle(0, 1, 0);
				break;
		}
	}
	static void mageGLFWCursorCallback(GLFWwindow *window, double x, double y)
	{
		InputHandle.MouseXPosition = x;
		InputHandle.MouseYPosition = y;
	}
	

#endif

void mageInputIntialise(mageWindow *window)
{
	memset(InputHandle.Keys, 0, MAGE_KEY_COUNT * sizeof(mageKeyHandle));
	InputHandle.MouseXPosition = 0.0;
	InputHandle.MouseYPosition = 0.0;
	

	#if defined(MAGE_GLFW)
		glfwSetKeyCallback(window->Context, mageGLFWKeyCallback);
		glfwSetCursorPosCallback(window->Context, mageGLFWCursorCallback);
	#endif
}
void mageInputFlush(mageWindow *window)
{

}


double mageGetMousePositionX(mageWindow *window)
{
	return InputHandle.MouseXPosition;
}
double mageGetMousePositionY(mageWindow *window)
{
	return InputHandle.MouseYPosition;
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
	return ((InputHandle.Keys[key] >> 0) & 1) || ((InputHandle.Keys[key] >> 1) & 1);
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





