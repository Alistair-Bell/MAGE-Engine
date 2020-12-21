#include "VulkanRenderer.h"
U8 MageVulkanRendererSurfacePickCorrectFormats(MageRenderer *renderer, MageRendererSurfaceSwapchainSupport *info, VkSurfaceFormatKHR *format, VkPresentModeKHR *present)
{
    U32 i;
    
    *format = info->Formats[0];
    for (i = 0; i < info->FormatCount; i++)
    {
        if (info->Formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && info->Formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            format = &info->Formats[i];
    }

    *present = VK_PRESENT_MODE_FIFO_KHR;
    for (i = 0; i < info->PresentCount; i++)
    {
        if (info->PresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
            present = &info->PresentModes[i];
    }

    return MageTrue;
}
U8 MageVulkanRendererSurfaceFindSwapchainSupport(MageRenderer *renderer, MageRendererSurfaceSwapchainSupport *info)
{
    memset(info, 0, sizeof(MageRendererSurfaceSwapchainSupport));
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(renderer->Device.GPU, renderer->Surface.Surface, &info->Capabilities);
    
    vkGetPhysicalDeviceSurfaceFormatsKHR(renderer->Device.GPU, renderer->Surface.Surface, &info->FormatCount, NULL);
    MAGE_HANDLE_ERROR_MESSAGE(info->FormatCount <= 0, printf("Error: Unable to find any suface formats with GPU %s\n", renderer->Device.Properties.deviceName));

    info->Formats = calloc(info->FormatCount, sizeof(VkSurfaceFormatKHR));
    vkGetPhysicalDeviceSurfaceFormatsKHR(renderer->Device.GPU, renderer->Surface.Surface, &info->FormatCount, info->Formats);

    vkGetPhysicalDeviceSurfacePresentModesKHR(renderer->Device.GPU, renderer->Surface.Surface, &info->PresentCount, NULL);
    MAGE_HANDLE_ERROR_MESSAGE(info->PresentCount <= 0, printf("Error: Unable to find any surface present modes with GPU %s\n", renderer->Device.Properties.deviceName); free(info->Formats))

    info->PresentModes = calloc(info->PresentCount, sizeof(VkPresentModeKHR));
    vkGetPhysicalDeviceSurfacePresentModesKHR(renderer->Device.GPU, renderer->Surface.Surface, &info->PresentCount, info->PresentModes);

    return (info->FormatCount != 0 || info->PresentCount != 0);
}
U8 MageVulkanRendererCreateSwapChain(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    MageRendererSurfaceSwapchainSupport sss;
    U8 currentResult = MageVulkanRendererSurfaceFindSwapchainSupport(renderer, &sss);
    MAGE_HANDLE_ERROR_MESSAGE(!currentResult, printf("Error: Unable to create renderer swapchain\n"));

    VkSurfaceFormatKHR chosenFormat;
    VkPresentModeKHR   chosePresentMode;
    VkExtent2D         chosenExtent = MageVulkanRendererSurfaceHandleExtent(info, &sss, renderer);
    currentResult = MageVulkanRendererSurfacePickCorrectFormats(renderer, &sss, &chosenFormat, &chosePresentMode);

    U32 minCount = sss.Capabilities.minImageCount + 1;
    /* 0 means there is no max image count */
    if (sss.Capabilities.maxImageCount > 0 && minCount > sss.Capabilities.maxImageCount)
        minCount = sss.Capabilities.maxImageCount;

    U32 graphicIndex = renderer->Device.QueueFamilies.GraphicsFamilyIndex;
    U32 presentIndex = renderer->Device.QueueFamilies.PresentFamilyIndex;

    VkSwapchainCreateInfoKHR swapchainInfo;
    memset(&swapchainInfo, 0, sizeof(VkSwapchainCreateInfoKHR));
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.surface           = renderer->Surface.Surface;
    swapchainInfo.minImageCount     = minCount;
    swapchainInfo.imageFormat       = chosenFormat.format;
    swapchainInfo.imageExtent       = chosenExtent;
    swapchainInfo.imageArrayLayers  = 1;
    swapchainInfo.imageUsage        = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainInfo.oldSwapchain      = VK_NULL_HANDLE;
    swapchainInfo.preTransform      = sss.Capabilities.currentTransform;
    swapchainInfo.compositeAlpha    = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.clipped           = MageTrue;
    swapchainInfo.presentMode       = chosePresentMode;

    if (graphicIndex != presentIndex)
    {
        swapchainInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        swapchainInfo.queueFamilyIndexCount = 2;
        swapchainInfo.pQueueFamilyIndices   = (U32[]) { graphicIndex, presentIndex };
    }
    else
    {
        swapchainInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        swapchainInfo.queueFamilyIndexCount = 0;
        swapchainInfo.pQueueFamilyIndices   = NULL;
    }
    
    VkResult swapResult = vkCreateSwapchainKHR(renderer->Device.LogicalDevice, &swapchainInfo, NULL, &renderer->SwapChain.PrimarySwapchain) == VK_TRUE;
    MageVulkanRendererSurfaceSwapchainSupportDestroy(&sss);
    return swapResult == VK_SUCCESS;
}
U8 MageVulkanRendererSurfaceSwapchainSupportDestroy(MageRendererSurfaceSwapchainSupport *info)
{
    free(info->Formats);
    free(info->PresentModes);
    return MageTrue;
}