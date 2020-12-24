#include "VulkanRenderer.h"

static const char *MageRequiredDeviceExtensions[] = 
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

U8 MageVulkanRendererFindPhysicalDeviceExtensions(VkPhysicalDevice device, const char *deviceExtensions[], const U32 count)
{
    U32 extensionCount;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);
    if (extensionCount < count)
        return MageFalse;
    VkExtensionProperties *extensions = calloc(extensionCount, sizeof(VkExtensionProperties));
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, extensions);
 
    U32 i, j;
    U32 foundCount = 0;
    for (i = 0; i < count; i++)
    {
        const char *current = deviceExtensions[i];
        for (j = 0; j < extensionCount; i++)
        {
            if (strcmp(current, extensions[j].extensionName))
            {
                foundCount++;
                break;
            }
        }
    }


    free(extensions);
    return (foundCount == count);
}
U64 MageVulkanRendererRatePhysicalDevice(MageRendererCreateInfo *info, MageRenderer *renderer, VkPhysicalDevice device)
{
    U64 score = 0;

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

    /* TODO: Allow the client to pick desired features */

    score += (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) * 500;
    score += (features.geometryShader == MageTrue)   * 1000;
    score += (properties.limits.maxImageDimension2D) * 10;
    score += (memoryProperties.memoryHeapCount)      * 50;

    MageRendererPhysicalDeviceQueueFamilies queues;
    score *= (MageVulkanRendererFindPhysicalDeviceQueueFamiles(renderer, device, &queues));

    return score;
}
U8 MageVulkanRendererFindPhysicalDeviceQueueFamiles(MageRenderer *renderer, VkPhysicalDevice device, MageRendererPhysicalDeviceQueueFamilies *families)
{
    memset(families, 0, sizeof(MageRendererPhysicalDeviceQueueFamilies));
    U32 familyCount  = 0;
    U32 i            = 0;
    U8 graphicsFound = MageFalse;
    U8 presentFound  = MageFalse;
    
    vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, NULL);
    VkQueueFamilyProperties *properties = calloc(familyCount, sizeof(VkQueueFamilyProperties));
    MAGE_HANDLE_ERROR_MESSAGE(familyCount <= 0, goto evaluate);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, properties);


    for (i = 0; i < familyCount; i++)
    {
        VkQueueFamilyProperties *current = &properties[i];
        if (current->queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            families->GraphicsFamilyIndex = i;
            graphicsFound = MageTrue;
        }
        
        U32 suported = MageFalse;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, renderer->Surface.Surface, &suported);
        if (suported)
        {
            families->PresentFamilyIndex = i;
            presentFound = MageTrue;
        }

        if (graphicsFound && presentFound)
        {
            goto evaluate;
        }
    }

    evaluate:
    {
        free(properties);
        return (graphicsFound) && (presentFound);
    }
}
U8 MageVulkanRendererCreatePhysicalDevice(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    MageRendererPhysicalDevice *rd = &renderer->Device;
    U32 count, i;
    U64 leadingIndex = 0;
    vkEnumeratePhysicalDevices(renderer->Overseer.Instance, &count, NULL);
    
    U64 *scores = calloc(count, sizeof(U64));
    memset(scores, 0, sizeof(U64) * count);
    
    VkPhysicalDevice *devices = calloc(count, sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(renderer->Overseer.Instance, &count, devices);
   
    switch (count)
    {
        case 0:
        {
            printf("Error: Unable to find any physical devices!\n");
            free(devices);
            free(scores);
            return MageFalse;
        }
        case 1:
        {
            scores[0] = MageVulkanRendererRatePhysicalDevice(info, renderer, devices[0]);
            goto settingDesiredDevice;
            break;
        }
    }

    for (i = 0; i < count; i++)
    {
        scores[i] = MageVulkanRendererRatePhysicalDevice(info, renderer, devices[i]);
        if (scores[i] > scores[leadingIndex])
            leadingIndex = i;
    }

    settingDesiredDevice:
    {
        MAGE_HANDLE_ERROR_MESSAGE(scores[leadingIndex] == 0, printf("Error: Unable to find suitable vulkan device with desired features!\n"));
        rd->GPU = devices[leadingIndex];
        vkGetPhysicalDeviceProperties(rd->GPU, &rd->Properties);
        vkGetPhysicalDeviceMemoryProperties(rd->GPU, &rd->MemoryProperties);
        MageVulkanRendererFindPhysicalDeviceQueueFamiles(renderer, renderer->Device.GPU, &renderer->Device.QueueFamilies);
        printf("Inform: Selected %s as rendering GPU, ranked most suitable of %d GPU's\n", rd->Properties.deviceName, count);
    }
    
    free(devices);
    free(scores);
    
    
    VkDeviceQueueCreateInfo queueCreateInfo;
    memset(&queueCreateInfo, 0, sizeof(VkDeviceQueueCreateInfo));
    queueCreateInfo.sType               = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex    = renderer->Device.QueueFamilies.GraphicsFamilyIndex;
    queueCreateInfo.queueCount          = 1;
    queueCreateInfo.pQueuePriorities    = (F32[]) { 1.0f };
    
    U32 reqCount = sizeof(MageRequiredDeviceExtensions) / sizeof(const char *);

    U8 result = MageVulkanRendererFindPhysicalDeviceExtensions(rd->GPU, MageRequiredDeviceExtensions, reqCount);
    MAGE_HANDLE_ERROR_MESSAGE(!result, printf("Error: Unable to find required device extensions\n"));

    VkDeviceCreateInfo deviceCreateInfo;
    memset(&deviceCreateInfo, 0, sizeof(VkDeviceCreateInfo));
    deviceCreateInfo.sType                       = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos           = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount        = 1;
    deviceCreateInfo.ppEnabledExtensionNames     = MageRequiredDeviceExtensions;
    deviceCreateInfo.enabledExtensionCount       = reqCount;
    
    VkResult createResult = vkCreateDevice(renderer->Device.GPU, &deviceCreateInfo, NULL, &renderer->Device.LogicalDevice);

    vkGetDeviceQueue(rd->LogicalDevice, renderer->Device.QueueFamilies.GraphicsFamilyIndex, 0, &renderer->QueueHandles.GraphicsQueue);
    return createResult == VK_SUCCESS;
}
