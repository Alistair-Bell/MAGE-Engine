#include <mageAPI.h>

void mageIndiciesIndexesInitialise(struct mageIndiciesIndexes *indicies, const uint32_t *graphics, const uint32_t graphicCount, const uint32_t *presents, const uint32_t presentCount)
{
    indicies->GraphicIndexes = calloc(graphicCount, sizeof(uint32_t));
    indicies->PresentIndexes = calloc(presentCount, sizeof(uint32_t));
    memcpy(indicies->GraphicIndexes, graphics, graphicCount * sizeof(uint32_t));
    memcpy(indicies->PresentIndexes, presents, presentCount * sizeof(uint32_t));
}
void mageIndiciesIndexesDestroy(struct mageIndiciesIndexes *indicies)
{
    free(indicies->GraphicIndexes);
    free(indicies->PresentIndexes);
}
mageResult mageGetDeviceIndexes(struct mageRenderer *renderer, struct mageIndiciesIndexes *indicies)
{
    uint32_t queueCount;
    vkGetPhysicalDeviceQueueFamilyProperties(renderer->Handler.PhysicalDevice, &queueCount, NULL);
    VkQueueFamilyProperties *properties = calloc(queueCount, sizeof(VkPhysicalDeviceProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(renderer->Handler.PhysicalDevice, &queueCount, properties);

    uint32_t graphicsIndex, presentIndex, i;
    uint32_t graphicsFound = 0;
    uint32_t presentFound = 0;
    uint32_t found = 0;

    for (i = 0; i < queueCount; i++)
    {
        if (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            graphicsIndex = i;
            graphicsFound = 1;
        }
        VkBool32 supported;

        vkGetPhysicalDeviceSurfaceSupportKHR(renderer->Handler.PhysicalDevice, i, renderer->Surface, &supported);

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
    if (!found)
    {
        MAGE_LOG_CORE_FATAL_ERROR("Graphics or present index not found\n", NULL);
        return MAGE_HARDWARE_INVALID;
    }
    mageIndiciesIndexesInitialise(indicies, (uint32_t []){ graphicsIndex }, 1, (uint32_t []){ presentIndex }, 1);
    

    free(properties);
    return MAGE_SUCCESS;
}