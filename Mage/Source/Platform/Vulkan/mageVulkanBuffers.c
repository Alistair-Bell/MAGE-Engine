#include <mageAPI.h>

#if defined (MAGE_VULKAN)

void mageVertexBufferInitialise(struct mageVertexBuffer *buffer, struct vector2 *verticies, uint32_t count, VkDevice device)
{
    buffer->Vertecies = calloc(count, sizeof(struct vector2));
    memcpy(buffer->Vertecies, verticies, sizeof(struct vector2) * count);
    buffer->PositionCount = count;

    VkBufferCreateInfo bufferInfo;
    memset(&bufferInfo, 0, sizeof(VkBufferCreateInfo));
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(struct vector2) * count;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    MAGE_CHECK_VULKAN(vkCreateBuffer(device, &bufferInfo, NULL, &buffer->Buffer));
}
void mageVertexBufferDestroy(struct mageVertexBuffer *buffer, VkDevice device)
{
    free(buffer->Vertecies);
    vkDestroyBuffer(device, buffer->Buffer, NULL);
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