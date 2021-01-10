#include "VulkanRenderer.h"

U8 MageVulkanRendererSurfaceCreate(MageRendererCreateInfo *info,  MageRenderer *renderer)
{
    VkResult result;

#if MAGE_BUILD_PLATFORM_WINDOWS

    VkWin32SurfaceCreateInfoKHR win32SurfaceInfo;
    memset(&win32SurfaceInfo, 0, sizeof(VkWin32SurfaceCreateInfoKHR));
    win32SurfaceInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    win32SurfaceInfo.flags     = 0;
    win32SurfaceInfo.hinstance = info->Window->Instance;
    win32SurfaceInfo.hwnd      = info->Window->NativeWindow;
    result = vkCreateWin32SurfaceKHR(renderer->Overseer.Instance, &win32SurfaceInfo, NULL, &renderer->Surface.Surface);

#elif MAGE_BUILD_PLATFORM_LINUX
    
    VkXlibSurfaceCreateInfoKHR xlibSurfaceInfo;
    memset(&xlibSurfaceInfo, 0, sizeof(VkXlibSurfaceCreateInfoKHR));
    xlibSurfaceInfo.sType   = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    xlibSurfaceInfo.flags   = 0;
    xlibSurfaceInfo.window  = info->Window->ContextWindow;
    xlibSurfaceInfo.dpy     = info->Window->WindowDisplay;
    result = vkCreateXlibSurfaceKHR(renderer->Overseer.Instance, &xlibSurfaceInfo, NULL, &renderer->Surface.Surface);

#endif

    return result == VK_SUCCESS;
}
VkExtent2D MageVulkanRendererSurfaceHandleExtent(MageRendererCreateInfo *info, MageRendererSurfaceSwapchainSupport *surfaceSupport, MageRenderer *renderer)
{
    VkExtent2D extent;
    memset(&extent, 0, sizeof(VkExtent2D));

    if (surfaceSupport->Capabilities.currentExtent.width == 0xFFFFFFFF)
        return surfaceSupport->Capabilities.currentExtent;

    MageApplicationWindowDimensions d;
    MageApplicationWindowGetFramebufferDimensions(info->Window, &d);
    U32 w = d.Width;
    U32 h = d.Height;
    extent.width  = max(surfaceSupport->Capabilities.minImageExtent.width,  min(surfaceSupport->Capabilities.maxImageExtent.width, w));
    extent.height = max(surfaceSupport->Capabilities.minImageExtent.height, min(surfaceSupport->Capabilities.maxImageExtent.height, h));

    return extent;
}
