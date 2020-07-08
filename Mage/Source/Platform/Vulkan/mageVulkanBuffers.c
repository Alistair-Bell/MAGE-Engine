#include <mageAPI.h>

#if defined (MAGE_VULKAN)

void mageVertexBufferInitialise(struct mageVertexBuffer *buffer, struct vector2 vertex, struct vector3 color)
{
    buffer->Vertex = vertex;
    buffer->Color = color; 
}
void mageBufferAllocate(struct mageBuffer *buffer, void *data, uint32_t dataSize, const VkBufferUsageFlags bufferUsage, struct mageRenderer *renderer)
{
    VkMemoryRequirements requirements;
    memset(&requirements, 0, sizeof(VkMemoryRequirements));

    VkBufferCreateInfo bufferInfo;
    memset(&bufferInfo, 0, sizeof(VkBufferCreateInfo));

    bufferInfo.sType        = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size         = dataSize;
    bufferInfo.usage        = bufferUsage;
    bufferInfo.sharingMode  = VK_SHARING_MODE_EXCLUSIVE;

    MAGE_CHECK_VULKAN(vkCreateBuffer(renderer->Device, &bufferInfo, NULL, &buffer->Buffer));

    vkGetBufferMemoryRequirements(renderer->Device, buffer->Buffer, &requirements);

    VkMemoryAllocateInfo allocateInfo;
    memset(&allocateInfo, 0, sizeof(VkMemoryAllocateInfo));
    allocateInfo.sType              = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.memoryTypeIndex    = mageFindMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, renderer);
    allocateInfo.allocationSize     = requirements.size;
    MAGE_CHECK_VULKAN(vkAllocateMemory(renderer->Device, &allocateInfo, NULL, &buffer->AllocatedMemory));

    MAGE_CHECK_VULKAN(vkBindBufferMemory(renderer->Device, buffer->Buffer, buffer->AllocatedMemory, 0));
    
    void *mappedData;
    vkMapMemory(renderer->Device, buffer->AllocatedMemory, 0, bufferInfo.size, 0, &mappedData);
    memcpy(mappedData, data, dataSize);
    vkUnmapMemory(renderer->Device, buffer->AllocatedMemory);
}
void mageBufferDestroy(struct mageBuffer *buffer, struct mageRenderer *renderer)
{
    vkDestroyBuffer(renderer->Device, buffer->Buffer, NULL);
    vkFreeMemory(renderer->Device, buffer->AllocatedMemory, NULL);
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
VkVertexInputAttributeDescription *mageVertexGetAttributeDescriptions(uint32_t *count)
{
    VkVertexInputAttributeDescription *attributeDescriptions = calloc(2, sizeof(VkVertexInputAttributeDescription));
    *count = 2;
    memset(attributeDescriptions, 0, sizeof(VkVertexInputAttributeDescription) * 2);

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(struct mageVertexBuffer, Vertex);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(struct mageVertexBuffer, Color);
    return attributeDescriptions;
}

#endif