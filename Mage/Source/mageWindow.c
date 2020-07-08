#include "mageAPI.h"

mageResult mageWindowInitialise(struct mageWindow *window, struct mageApplicationProps *props)
{

	window->Height  = props->Height;
	window->Width   = props->Width;
	window->Title   = props->Name;
	window->Running = 1;
	
	MAGE_LOG_CORE_INFORM("Using GLFW as window mode\n", NULL);
		
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	GLFWmonitor *monitor = NULL;


	if (props->FixedResolution) { glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); }
	if (props->Fullscreen) { monitor = glfwGetPrimaryMonitor(); }

	window->Context = glfwCreateWindow(window->Width, window->Height, window->Title, monitor, NULL);

	GLFWimage localIcon;
	uint8_t data;
	uint32_t width, height;
	uint8_t *image;
	uint32_t error = lodepng_decode32_file(&image, &width, &height, props->WindowIcon);

	if (error) 
	{
    	MAGE_LOG_CORE_ERROR("Image decoder error %u: %s\n", error, lodepng_error_text(error));
  	}
	else
	{
		localIcon.width = width;
		localIcon.height = height;
		localIcon.pixels = image;
		glfwSetWindowIcon(window->Context, 1, &localIcon);
	}
	
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
