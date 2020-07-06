#include <mageAPI.h>

#if defined (MAGE_VULKAN)

void mageVertexBufferInitialise(struct mageVertexBuffer *buffer, struct vector2 *verticies, uint32_t count)
{
    buffer->Vertecies = calloc(count, sizeof(struct vector2));
    memcpy(buffer->Vertecies, verticies, sizeof(struct vector2) * count);
    buffer->PositionCount = count;
}
void mageVertexBufferDestroy(struct mageVertexBuffer *buffer)
{
    free(buffer->Vertecies);
}
VkVertexInputBindingDescription mageVertexBindingDescription()
{
    VkVertexInputBindingDescription bindingDescription;
    memset(&bindingDescription, 0, sizeof(VkVertexInputBindingDescription));
    bindingDescription.binding      = 0;
    bindingDescription.stride       = sizeof(struct mageVertexBuffer);
    bindingDescription.inputRate    = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
}
VkVertexInputAttributeDescription mageVertexGetAttributeDescriptions()
{
    VkVertexInputAttributeDescription attributeDescriptions;
    memset(&attributeDescriptions, 0, sizeof(VkVertexInputAttributeDescription));

    attributeDescriptions.binding    = 0;
    attributeDescriptions.location   = 0;
    attributeDescriptions.format     = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions.offset     = offsetof(struct mageVertexBuffer, Vertecies);
    return attributeDescriptions;
}

#endif