#include "../Window.h"

U8 MageApplicationWindowCreateVulkanSurface(MageApplicationWindow *window, VkInstance instance, VkSurfaceKHR *surface)
{
    PFN_vkCreateWin32SurfaceKHR surfaceCreate = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");
    MAGE_HANDLE_ERROR_MESSAGE(surfaceCreate == NULL, printf("Error: Unable to load function vkCreateWin32SurfaceKHR - check driver!\n"));

    VkWin32SurfaceCreateInfoKHR surfaceInfo;
    memset(&surfaceInfo, 0, sizeof(VkWin32SurfaceCreateInfoKHR));
    surfaceInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.flags     = 0;
    surfaceInfo.hinstance = window->Instance;
    surfaceInfo.hwnd      = window->NativeWindow;
    VkResult result = surfaceCreate(instance, &surfaceInfo, NULL, surface);
    MAGE_HANDLE_ERROR_MESSAGE(result != VK_SUCCESS, printf("Error: Unable to create vulkan Win32 surface\n"));
    return MageTrue;
}
