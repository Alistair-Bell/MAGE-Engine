#include <mageAPI.h>

#if defined (MAGE_VULKAN)

VkResult mageHandleVulkanResult(const char *functionName, VkResult functionResult)
{
    switch (functionResult)
    {
    case VK_SUCCESS:
        break;
    case VK_NOT_READY:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, vulkan is not ready\n", functionName);
        break;
    case VK_TIMEOUT:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, vulkan timeout\n", functionName);
        break;
    case VK_EVENT_SET:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, event has been set\n", functionName);
        break;
    case VK_EVENT_RESET:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, event has been reset\n", functionName);
        break;
    case VK_INCOMPLETE:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, incomplete error\n", functionName);
        break;
    case VK_ERROR_OUT_OF_HOST_MEMORY:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, host out of memory\n", functionName);
        break;
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, device has run out of memory\n", functionName);
        break;
    case VK_ERROR_INITIALIZATION_FAILED:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, initialization failed\n", functionName);
        break;
    case VK_ERROR_DEVICE_LOST:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, vulkan device was lost\n", functionName);
        break;
    case VK_ERROR_MEMORY_MAP_FAILED:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, memory map has failed\n", functionName);
        break;
    case VK_ERROR_LAYER_NOT_PRESENT:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, layers not present\n", functionName);
        break;
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, requested extensions not present\n", functionName);
        break;
    case VK_ERROR_FEATURE_NOT_PRESENT:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, requested features not present\n", functionName);
        break;
    case VK_ERROR_INCOMPATIBLE_DRIVER:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, host drivers incompatable, check vulkan install\n", functionName);
        break;
    case VK_ERROR_TOO_MANY_OBJECTS:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, too many objects\n", functionName);
        break;
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, format requested was not present\n", functionName);
        break;
    case VK_ERROR_FRAGMENTED_POOL:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, pool has fragmented\n", functionName);
        break;
    case VK_ERROR_OUT_OF_POOL_MEMORY:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, out of pool memory\n", functionName);
        break;
    case VK_ERROR_INVALID_EXTERNAL_HANDLE:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, invalid external handle\n", functionName);
        break;
    case VK_ERROR_FRAGMENTATION:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, fragmentation has occured\n", functionName);
        break;
    case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, invalid opaque capture adress\n", functionName);
        break;
    case VK_ERROR_SURFACE_LOST_KHR:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, surface has been lost\n", functionName);
        break;
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, native window already in use\n", functionName);
        break;
    case VK_SUBOPTIMAL_KHR:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, conditions suboptimal\n", functionName);
        break;
    case VK_ERROR_OUT_OF_DATE_KHR:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, error out of date\n", functionName);
        break;
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, incompatable display\n", functionName);
        break;
    case VK_ERROR_VALIDATION_FAILED_EXT:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, validation has failed\n", functionName);
        break;
    case VK_ERROR_INVALID_SHADER_NV:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, invalid Nividia shaders\n", functionName);
        break;
    case VK_ERROR_INCOMPATIBLE_VERSION_KHR:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, incompatable version\n", functionName);
        break;
    case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, DRM format plane layout modifier in use\n", functionName);
        break;
    case VK_ERROR_NOT_PERMITTED_EXT:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, operation not permited\n", functionName);
        break;
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, full screen exclusive mode has been lost\n", functionName);
        break;
    case VK_THREAD_IDLE_KHR:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, thread is idle\n", functionName);
        break;
    case VK_THREAD_DONE_KHR:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, thread is done\n", functionName);
        break;
    case VK_OPERATION_DEFERRED_KHR:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, operation defered\n", functionName);
        break;
    case VK_OPERATION_NOT_DEFERRED_KHR:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, operation not defered\n", functionName);
        break;
    case VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT:
        MAGE_LOG_CORE_FATAL_ERROR("%s has failed, pipeline compile required\n", functionName);
        break;
    default:
        MAGE_LOG_CORE_FATAL_ERROR("Undefined vulkan error\n", NULL);
    }
    assert(functionResult == VK_SUCCESS);
    return functionResult;
}

void mageIndiciesIndexesInitialise(struct mageIndiciesIndexes *indicies, const uint32_t *graphics, const uint32_t graphicCount, const uint32_t *presents, const uint32_t presentCount)
{
    indicies->GraphicIndexes = calloc(graphicCount, sizeof(uint32_t));
    indicies->PresentIndexes = calloc(presentCount, sizeof(uint32_t));
    memcpy(indicies->GraphicIndexes, graphics, graphicCount * sizeof(uint32_t));
    memcpy(indicies->PresentIndexes, presents, presentCount * sizeof(uint32_t));
    indicies->GraphicIndexesCount = graphicCount;
    indicies->PresentIndexesCount = presentCount;
}
void mageIndiciesIndexesDestroy(struct mageIndiciesIndexes *indicies)
{
    free(indicies->GraphicIndexes);
    free(indicies->PresentIndexes);
}
mageResult mageGetDeviceIndexes(struct mageRenderer *renderer, VkPhysicalDevice device, struct mageIndiciesIndexes *indicies)
{
    uint32_t queueCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, NULL);
    VkQueueFamilyProperties *queueProperties = calloc(queueCount, sizeof(VkPhysicalDeviceProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, queueProperties);


    uint32_t graphicsIndex, presentIndex, i;
    uint32_t graphicsFound = 0;
    uint32_t presentFound = 0;


    for (i = 0; i < queueCount; i++)
    {
        if (queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            graphicsIndex = i;
            graphicsFound = 1;
        }
        VkBool32 supported;

        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, renderer->Surface, &supported);

        if (supported)
        {
            presentIndex = i;
            presentFound = 1;
        }

        if (graphicsFound && presentFound)
        {
            break;
        }
    }
    if (!(presentFound || graphicsFound))
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);
        MAGE_LOG_CORE_ERROR("Device %s, graphics or present index not found\n", properties.deviceName);
        return MAGE_RESULT_HARDWARE_INVALID;
    }
    
    mageIndiciesIndexesInitialise(indicies, (uint32_t []){ graphicsIndex }, 1, (uint32_t []){ presentIndex }, 1);
    free(queueProperties);
    return MAGE_RESULT_SUCCESS;
}
void mageSwapChainSupportInitialise(struct mageSwapChainSupportDetails *swapChainSupport, const VkSurfaceCapabilitiesKHR surfaceCapabilities, VkSurfaceFormatKHR *formats, const uint32_t formatCount, VkPresentModeKHR *presentModes, const uint32_t presentCount, VkExtent2D extent)
{
    swapChainSupport->Formats           = calloc(formatCount, sizeof(VkSurfaceFormatKHR));
    swapChainSupport->PresentModes      = calloc(presentCount, sizeof(VkPresentModeKHR));
    memcpy(swapChainSupport->Formats, formats, sizeof(VkSurfaceFormatKHR) * formatCount);
    memcpy(swapChainSupport->PresentModes, presentModes, sizeof(VkPresentModeKHR) * presentCount);
    
    swapChainSupport->FormatCount       = formatCount;
    swapChainSupport->PresentCount      = presentCount;
    swapChainSupport->Capabilities      = surfaceCapabilities;
    swapChainSupport->Extent            = extent;
}
mageResult mageGetSwapChainSupport(struct mageSwapChainSupportDetails *swapChainSupport, struct mageWindow *window, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    
    uint32_t formatCount, presentCount;
    VkSurfaceCapabilitiesKHR capabilities;
    VkExtent2D extent;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, NULL);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentCount, NULL);

    if (formatCount <= 0 || presentCount <= 0)
    {
        MAGE_LOG_CORE_ERROR("Physical device has no surface formats or present modes supported\n", NULL);
        return MAGE_RESULT_HARDWARE_INVALID;
    }
    
    VkSurfaceFormatKHR *formats = calloc(formatCount, sizeof(VkSurfaceFormatKHR));
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats);
    
    VkPresentModeKHR *presents = calloc(presentCount, sizeof(VkPresentModeKHR));
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentCount, presents);

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &swapChainSupport->Capabilities);
    
    if (capabilities.currentExtent.width != UINT32_MAX) 
    {   
        extent = swapChainSupport->Capabilities.currentExtent;
    }
    else
    {
        extent.width = window->Width;
        extent.height = window->Height;
    }

    mageSwapChainSupportInitialise(swapChainSupport, capabilities, formats, formatCount, presents, presentCount, extent);

    free(presents);
    free(formats);
    return MAGE_RESULT_SUCCESS;
}
VkPresentModeKHR mageSwapChainSupportPickPresentMode(struct mageSwapChainSupportDetails *swapChainSupport)
{
    uint32_t i;
    
    for (i = 0; i < swapChainSupport->PresentCount; i++)
    {
        if (swapChainSupport->PresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return swapChainSupport->PresentModes[i];
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}
VkSurfaceFormatKHR mageSwapChainSupportPickSurfaceFormat(struct mageSwapChainSupportDetails *swapChainDetails)
{
    uint32_t i;
    for (i = 0; i < swapChainDetails->FormatCount; i++) 
    {
        if (swapChainDetails->Formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && swapChainDetails->Formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
        {
            return swapChainDetails->Formats[i];
        }
    }
    return swapChainDetails->Formats[0];
}
void mageSwapChainSupportDestroy(struct mageSwapChainSupportDetails *swapChainSupport)
{
    free(swapChainSupport->Formats);
    free(swapChainSupport->PresentModes);
}
uint32_t mageFindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, struct mageRenderer *renderer)
{

    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(renderer->PhysicalDevice, &memoryProperties);

    uint32_t i;
    for (i= 0; i < memoryProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) 
        {
            return i;
        }
    }
    MAGE_LOG_CORE_FATAL_ERROR("Memory index was not found\n", NULL);
    return UINT32_MAX;
}




#endif