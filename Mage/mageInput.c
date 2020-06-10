#include "mageAPI.h"

#if defined(MAGE_GLFW)
	#define MAGE_KEY_COUNT 350
	#define MAGE_MOUSE_BUTTON_COUNT 7
#elif #defined(MAGE_SDL2)
	#define MAGE_KEY_COUNT 1024
	#define MAGE_MOUSE_BUTTON_COUNT 10
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


typedef struct MAGE_EVENT_SYSTEM
{
	#if defined(MAGE_GLFW)
		GLFWwindow *WindowContext;
	#else
		SDLEvent *EventContext;
	#endif



	mageKeyHandle 	Keys[MAGE_KEY_COUNT];
	uint8_t 		MouseButtons[MAGE_MOUSE_BUTTON_COUNT];

	double 			MouseXPosition;
	double 			MouseYPosition;
	uint8_t 		WindowFocused;

} mageEvent;

static mageEvent EventHandle;


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
				EventHandle.Keys[key] = mageCreateKeyHandle(1, 0, 0);
				break;

			case GLFW_REPEAT:
				EventHandle.Keys[key] = mageCreateKeyHandle(1, 0, 1);
				break;

			case GLFW_RELEASE:
				EventHandle.Keys[key] = mageCreateKeyHandle(0, 1, 0);
				break;
		}
	}
	static void mageGLFWCursorCallback(GLFWwindow *window, double x, double y)
	{
		EventHandle.MouseXPosition = x;
		EventHandle.MouseYPosition = y;
	}
	static void mageGLFWWindowFocusCallback(GLFWwindow *window, int32_t focused)
	{
		EventHandle.WindowFocused = focused;
	}
	static void mageGLFWMouseButtonCallbacks(GLFWwindow* window, int button, int action, int modes)
	{
		EventHandle.MouseButtons[button] = action;
	}
	

#endif

void mageInputIntialise(mageWindow *window)
{
	memset(EventHandle.Keys, 0, MAGE_KEY_COUNT * sizeof(mageKeyHandle));
	memset(EventHandle.MouseButtons, 0, sizeof(uint8_t) * MAGE_MOUSE_BUTTON_COUNT);
	EventHandle.MouseXPosition = 0.0;
	EventHandle.MouseYPosition = 0.0;

	#if defined(MAGE_GLFW)
		EventHandle.WindowContext = window->Context;
		glfwSetKeyCallback(EventHandle.WindowContext, mageGLFWKeyCallback);
		glfwSetCursorPosCallback(EventHandle.WindowContext, mageGLFWCursorCallback);
		glfwSetWindowFocusCallback(EventHandle.WindowContext, mageGLFWWindowFocusCallback);
		glfwSetMouseButtonCallback(EventHandle.WindowContext, mageGLFWMouseButtonCallbacks);
	#endif
}
void mageInputFlush(mageWindow *window)
{

}


double mageGetMousePositionX()
{
	return EventHandle.MouseXPosition;
}
double mageGetMousePositionY()
{
	return EventHandle.MouseYPosition;
}
uint8_t mageGetMouseButtonLeftClick(window)
{

	#if defined(MAGE_SDL2)
		
	#elif defined(MAGE_GLFW)
		
	#endif
	return 0;
}
uint8_t mageGetMouseButtonRightClick(mageWindow *window)
{
	#if defined(MAGE_SDL2)
		
	#elif defined(MAGE_GLFW)
		
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
uint8_t mageGetKeyDown(const int32_t key)
{	
	return ((EventHandle.Keys[key] >> 0) & 1) || ((EventHandle.Keys[key] >> 1) & 1);
}
uint8_t mageGetKeyNotDown(const int32_t key)
{
#if defined(MAGE_SDL2)
	
	#elif defined(MAGE_GLFW)
		return (!((EventHandle.Keys[key] >> 0) & 1) || ((EventHandle.Keys[key] >> 1) & 1));
	#endif
	return 0;
}





