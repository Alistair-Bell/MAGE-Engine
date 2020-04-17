#include "mageAPI.h"

void mageWindowAllocate(mageWindow *window)
{
	window = malloc(sizeof(*window));
}
uint8 mageWindowInitialise(mageWindow *window, const sint32 xResolution, const sint32 yResolution, const char *title)
{
	if (!glfwInit())
	{
		return -1;
	}	

	window->Height = yResolution;
	window->Width = xResolution;
	window->Title = title;
	window->Running = 1;

	window->Context = glfwCreateWindow(window->Width, window->Height, window->Title, NULL, NULL);


	if (window->Context == NULL)
	{
		glfwTerminate();
		printf("GLFW window context failed to initialise!\n");
		return 0;
	}
	glfwMakeContextCurrent(window->Context);
	printf("GLFW context has been created!\n");

#ifdef __MAGE_OPENGL__

	// Opengl rendering mode requires glew to initialise to be used
	// http://glew.sourceforge.net/

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW has failed to initialse!\n");
		return 0;
	}
	printf("GLEW has succesfully initialise!\n");
#endif

	printf("MAGE window has succesfully initialsed\n");
	
	return 1;
}
void mageWindowSwapBuffers(mageWindow *window)
{
	glfwSwapBuffers(window->Context);
}

void mageWindowDestroy(mageWindow *window)
{
	glfwTerminate();
	free(window);
}
