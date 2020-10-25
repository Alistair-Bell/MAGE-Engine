#include "mageAPI.h"

mageResult mageWindowCreate(struct mageWindow *window, struct mageWindowCreateInfo *info)
{
	if (info->Icon == NULL)
        info->Icon = "Mage/Resources/Textures/Vulkan/Vulkan_LogoBug_48px_Nov17.png";
		
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWmonitor *monitor = NULL;
	if (info->Fullscreen) { monitor = glfwGetPrimaryMonitor(); }

	window->Context = glfwCreateWindow(info->Width, info->Height, info->Title, monitor, NULL);
	MAGE_ASSERT_MESSAGE(window->Context != NULL, "Failed to create glfw window\n", NULL);

	uint8_t data;
	int32_t width, height, channels;
    uint8_t *image = stbi_load(info->Icon, &width, &height, &channels, STBI_rgb_alpha);

	GLFWimage icon;
	icon.width 		= width;
	icon.height 	= height;
	icon.pixels 	= image;
	glfwSetWindowIcon(window->Context, 1, &icon);
	
	MAGE_LOG_CORE_INFORM("GLFW context created\n", NULL);
	
	MAGE_MEMORY_FREE(image);
	return MAGE_RESULT_SUCCESS;
}
void mageWindowDestroy(struct mageWindow *window)
{	
	glfwTerminate();	
	MAGE_LOG_CORE_INFORM("Window has been terminated\n", NULL);
}
