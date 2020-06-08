#include "mageAPI.h"

void *mageRendererAllocate()
{
    return malloc(sizeof(struct MAGE_RENDERER_STRUCT));
}
void *mageVulkanHandlerAllocate()
{
    return malloc(sizeof(struct MAGE_VULKAN_HANDLER_STRUCT));
}