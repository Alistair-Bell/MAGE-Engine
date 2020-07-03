#include <mageAPI.h>

#if defined (MAGE_VULKAN)

VkVertexInputBindingDescription mageVertexBindingDescription()
{
    VkVertexInputBindingDescription description;
    memset(&description, 0, sizeof(VkVertexInputBindingDescription));

    description.binding = 0;
    description.stride = sizeof(struct mageVertex);
    description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return description;
}   

#endif