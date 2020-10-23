#include "mageAPI.h"

mageResult mageWindowCreate(struct mageWindow *window, struct mageApplicationCreateInfo *info)
{
	if (info->WindowIcon == NULL)
    {
        info->WindowIcon = "Mage/Resources/Textures/MTEC/Logo.png";
    }
	
	window->Height  = info->Height;
	window->Width   = info->Width;
	window->Title   = info->Name;
	window->Running = 1;
	
	MAGE_LOG_CORE_INFORM("Using GLFW as window mode\n", NULL);
		
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);


	GLFWmonitor *monitor = NULL;
	if (info->FixedResolution) { glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); }
	if (info->Fullscreen) { monitor = glfwGetPrimaryMonitor(); }

	window->Context = glfwCreateWindow(window->Width, window->Height, window->Title, monitor, NULL);

	GLFWimage localIcon;
	uint8_t data;
	int32_t width, height, channels;
    uint8_t *image = stbi_load(info->WindowIcon, &width, &height, &channels, STBI_rgb_alpha);


	localIcon.width = width;
	localIcon.height = height;
	localIcon.pixels = image;
	glfwSetWindowIcon(window->Context, 1, &localIcon);
	
	if (window->Context == NULL)
	{
		glfwTerminate();
		MAGE_LOG_CORE_FATAL_ERROR("GLFW context has failed to create\n", NULL);
		return MAGE_RESULT_CONTEXT_CREATION_FAILED;
	}
	
	MAGE_LOG_CORE_INFORM("GLFW context created\n", NULL);
	
	MAGE_MEMORY_FREE(image);
	return MAGE_RESULT_SUCCESS;
}
void mageWindowDestroy(struct mageWindow *window)
{	
	glfwTerminate();	
	MAGE_LOG_CORE_INFORM("Window has been terminated\n", NULL);
}
