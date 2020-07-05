#include <mageAPI.h>

#if defined (MAGE_VULKAN)

VkVertexInputBindingDescription mageVertexBindingDescription()
{
    VkVertexInputBindingDescription description;
    memset(&description, 0, sizeof(VkVertexInputBindingDescription));

    description.binding     = 0;
    description.stride      = sizeof(struct mageVertex);
    description.inputRate   = VK_VERTEX_INPUT_RATE_VERTEX;

    return description;
}   
VkVertexInputAttributeDescription *mageVertexGetAttributeDescriptions(uint32_t *count)
{
    VkVertexInputAttributeDescription *description = calloc(2, sizeof(VkVertexInputAttributeDescription));
    memset(description, 0, sizeof(VkVertexInputAttributeDescription) * 2);
    description[0].binding      = 0;
    description[0].location     = 0;
    description[0].format       = VK_FORMAT_R32G32_SFLOAT;
    description[0].offset       = offsetof(struct mageVertex, Position);

    description[1].binding      = 0;
    description[1].location     = 1;
    description[1].format       = VK_FORMAT_R32G32B32_SFLOAT;
    description[1].offset       = offsetof(struct mageVertex, Color);

    *count = 2;
    return description;
}

#endif