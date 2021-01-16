#include "VulkanRenderer.h"

U8 MageVulkanRendererSurfaceCreate(MageRendererCreateInfo *info, MageRenderer *renderer)
{
   return MageApplicationWindowCreateVulkanSurface(info->Window, renderer->Overseer.Instance, &renderer->Surface.Surface);
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
