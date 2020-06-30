#include "mageAPI.h"

mageResult mageWindowInitialise(struct mageWindow *window, const int32_t xResolution, const int32_t yResolution, const char *title)
{

	window->Height  = yResolution;
	window->Width   = xResolution;
	window->Title   = title;
	window->Running = 1;
	
	MAGE_LOG_CORE_INFORM("Using GLFW as window mode\n", NULL);
		
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


	window->Context = glfwCreateWindow(window->Width, window->Height, window->Title, NULL, NULL);

	if (window->Context == NULL)
	{
		glfwTerminate();
		MAGE_LOG_CORE_FATAL_ERROR("GLFW context has failed to create\n", NULL);
		return MAGE_CONTEXT_CREATION_FAILED;
	}
	
	MAGE_LOG_CORE_INFORM("GLFW context created\n", NULL);
	
	glfwSetInputMode(window->Context, GLFW_STICKY_KEYS, GLFW_TRUE);

	glfwSetInputMode(window->Context, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);


	return MAGE_SUCCESS;

}
void mageWindowTerminate(struct mageWindow *window)
{
	glfwDestroyWindow(window->Context);
	glfwTerminate();	
	MAGE_LOG_CORE_INFORM("Window has been terminated\n", NULL);
}
