#include "../../Window.h"

U8 MageApplicationWindowCreateVulkanSurface(MageApplicationWindow *window, VkInstance instance, VkSurfaceKHR *surface)
{
    VkXlibSurfaceCreateInfoKHR surfaceInfo;
    memset(&surfaceInfo, 0, sizeof(VkXlibSurfaceCreateInfoKHR));
    surfaceInfo.sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.window = window->ContextWindow;
    surfaceInfo.dpy    = window->WindowDisplay;
    VkResult result = vkCreateXlibSurfaceKHR(instance, &surfaceInfo, NULL, surface);
    MAGE_HANDLE_ERROR_MESSAGE(result != VK_SUCCESS, printf("Error: Unable to create vulkan XLib surface\n"));
    return MageTrue;
}
