#include "mageAPI.h"
#include <GLFW/glfw3.h>

void *mageWindowAllocate()
{
	return malloc(sizeof(struct MAGE_WINDOW_STRUCT));
}
void mageWindowInitialise(mageWindow *window, const sint32 xResolution, const sint32 yResolution, const char *title, uint8 *success)
{
	if (!glfwInit())
	{
		mageTryDumpSuccess(0, success);
	}	

	window->Height = yResolution;
	window->Width = xResolution;
	window->Title = title;
	window->Running = 1;

	window->Context = glfwCreateWindow(window->Width, window->Height, window->Title, NULL, NULL);


	if (window->Context == NULL)
	{
		glfwTerminate();
		mageTryDumpSuccess(0, success);
	}
	glfwMakeContextCurrent(window->Context);
	glfwSetWindowSizeCallback(window->Context, mageWindowResizeCallback);
	
#ifdef MAGE_OPENGL_
	glewExperimental = GL_TRUE
	if (glewInit() != GLEW_OK)
		mageTryDumpSuccess(0, success);
#endif
	mageTryDumpSuccess(1, success);
}
void mageWindowSwapBuffers(mageWindow *window)
{
	glfwSwapBuffers(window->Context);
}
void mageWindowResizeCallback(GLFWwindow *window, sint32 xResolution, sint32 yResolution)
{
	#ifdef MAGE_OPENGL_
		GLCall(glViewport(0, 0, xResolution, yResolution));
	#endif
}
void mageWindowDestroy(mageWindow *window)
{
	glfwTerminate();
	mageFreeMethod(window);
}
