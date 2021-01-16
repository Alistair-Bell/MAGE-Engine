#include "../Window.h"

U8 MageApplicationWindowCreateVulkanSurface(MageApplicationWindow *window, VkInstance instance, VkSurfaceKHR *surface)
{
    VkWin32SurfaceCreateInfoKHR surfaceInfo;
    memset(&win32SurfaceInfo, 0, sizeof(VkWin32SurfaceCreateInfoKHR));
    surfaceInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.flags     = 0;
    surfaceInfo.hinstance = info->Window->Instance;
    surfaceInfo.hwnd      = info->Window->NativeWindow;
    VkResult result = vkCreateWin32SurfaceKHR(renderer->Overseer.Instance, &surfaceInfo, NULL, &renderer->Surface.Surface);
    MAGE_HANDLE_ERROR_MESSAGE(result != VK_SUCCESS, printf("Error: Unable to create vulkan Win32 surface\n"));
    return MageTrue;
}
