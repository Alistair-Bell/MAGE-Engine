#include "../Window.h"

U8 MageApplicationWindowCreateVulkanSurface(MageApplicationWindow *window, VkInstance instance, VkSurfaceKHR *surface)
{
    PFN_vkCreateWaylandSurfaceKHR surfaceFunction = (PFN_vkCreateWaylandSurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateWaylandSurfaceKHR");
    MAGE_HANDLE_ERROR_MESSAGE(surfaceFunction == NULL, printf("Error: Unable to load function vkCreateWaylandSurfaceKHR - check driver\n"));
    
    VkWaylandSurfaceCreateInfoKHR surfaceInfo;
    memset(&surfaceInfo, 0, sizeof(VkWaylandSurfaceCreateInfoKHR));
    surfaceInfo.sType        = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.display      = window->Display;
    surfaceInfo.surface      = window->Surface;
    VkResult r = surfaceFunction(instance, &surfaceInfo, NULL, surface);
    MAGE_HANDLE_ERROR_MESSAGE(r != VK_SUCCESS, printf("Error: Unable to create vulkan Wayland surface\n"));
    return MageTrue;
}
