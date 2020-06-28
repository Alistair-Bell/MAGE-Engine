#include <mageAPI.h>

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
        return MAGE_HARDWARE_INVALID;
    }
    
    mageIndiciesIndexesInitialise(indicies, (uint32_t []){ graphicsIndex }, 1, (uint32_t []){ presentIndex }, 1);
    free(queueProperties);
    return MAGE_SUCCESS;
}
