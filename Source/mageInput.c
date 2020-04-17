#include "mageAPI.h"

void mageGetMousePosition(mageWindow *window, double *x, double *y)
{
	glfwGetCursorPos(window->Context, x, y);
}
void mageGetMousePositionX(mageWindow *window, double *x)
{
	glfwGetCursorPos(window->Context, x, NULL);
}
void mageGetMousePositionY(mageWindow *window, double *y)
{
	glfwGetCursorPos(window->Context, NULL, y);
}
void mageGetMouseButtonStates(mageWindow *window, sint32 *left, sint32 *right)
{
	sint32 foo = glfwGetMouseButton(window->Context, GLFW_MOUSE_BUTTON_LEFT);
	sint32 bar = glfwGetMouseButton(window->Context, GLFW_MOUSE_BUTTON_RIGHT); 
	
	if (left != NULL)
		*left = foo;

	if (right != NULL)
		*right = bar;
}
void mageGetMouseButtonLeftClick(mageWindow *window, uint8 *state)
{
	int foo;
	mageGetMouseButtonStates(window, &foo, NULL);
	*state = (foo == GLFW_PRESS);
}
void mageGetMouseButtonRightClick(mageWindow *window, uint8 *state)
{
	sint32 foo;
	mageGetMouseButtonStates(window, NULL, &foo);
	*state = (foo == GLFW_PRESS);
}
void mageGetMouseInsideContext(mageWindow *window, uint8 *state)
{
	sint32 foo = glfwGetWindowAttrib(window->Context, GLFW_HOVERED);
	*state = foo;
}
void mageSetMousePosition(mageWindow *window, const double x, const double y)
{
	glfwSetCursorPos(window->Context, x, y);
}
void mageGetKeyDown(mageWindow *window, const sint32 key, uint8 *state)
{
	uint32 value = glfwGetKey(window->Context, key);
	*state = (value == GLFW_PRESS || value == GLFW_REPEAT);
}
void mageGetKeyNotDown(mageWindow *window, const sint32 key, uint8 *state)
{
	uint32 value = glfwGetKey(window->Context, key);
	*state = !(value == GLFW_PRESS || value == GLFW_REPEAT);
}





