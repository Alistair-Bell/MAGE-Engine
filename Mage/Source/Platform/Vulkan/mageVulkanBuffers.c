#include <mageAPI.h>

#if defined (MAGE_VULKAN)


void mageVertexBufferCreate(struct mageVertexBuffer *buffer, struct mageVertex *vertexes, uint32_t vertexCount, struct mageRenderer *renderer)
{
    buffer->Vertexes = calloc(vertexCount, sizeof(struct mageVertex));
    buffer->Count = vertexCount;
    memcpy(buffer->Vertexes, vertexes, sizeof(struct mageVertex) * vertexCount);
    mageBufferAllocate(&buffer->MemoryBuffer, buffer->Vertexes, sizeof(struct mageVertex) * vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, renderer);

    void *mappedData;
    vkMapMemory(renderer->Device, buffer->MemoryBuffer.AllocatedMemory, 0, sizeof(struct mageVertex) * vertexCount, 0, &mappedData);
    memcpy(mappedData, vertexes, sizeof(struct mageVertex) * vertexCount);
    vkUnmapMemory(renderer->Device, buffer->MemoryBuffer.AllocatedMemory);
}
void mageVertexBufferDestroy(struct mageVertexBuffer *buffer, struct mageRenderer *renderer)
{
    mageBufferDestroy(&buffer->MemoryBuffer, renderer);
    free(buffer->Vertexes);
}
void mageIndexBufferCreate(struct mageIndexBuffer *buffer, uint16_t *indexes, uint32_t indexCount, struct mageRenderer *renderer)
{
    buffer->Indexes = calloc(indexCount, sizeof(uint16_t));
    buffer->Count = indexCount;
    memcpy(buffer->Indexes, indexes, sizeof(uint16_t) * indexCount);
    mageBufferAllocate(&buffer->MemoryBuffer, buffer->Indexes,  sizeof(uint16_t) * indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, renderer);

    void *mappedData;
    vkMapMemory(renderer->Device, buffer->MemoryBuffer.AllocatedMemory, 0,  sizeof(uint16_t) * indexCount, 0, &mappedData);
    memcpy(mappedData, indexes,  sizeof(uint16_t) * indexCount);
    vkUnmapMemory(renderer->Device, buffer->MemoryBuffer.AllocatedMemory);
}
void mageIndexBufferDestroy(struct mageIndexBuffer *buffer, struct mageRenderer *renderer)
{
    mageBufferDestroy(&buffer->MemoryBuffer, renderer);
    free(buffer->Indexes);
}

void mageVertexInitialise(struct mageVertex *vertexInstance, struct vector2 vertex, struct vector3 color)
{
    vertexInstance->Vertex = vertex;
    vertexInstance->Color = color; 
}
void mageBufferAllocate(struct mageBuffer *buffer, void *data, uint32_t dataSize, const VkBufferUsageFlags bufferUsage, VkMemoryPropertyFlags flags, struct mageRenderer *renderer)
{
    VkMemoryRequirements requirements;
    
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
    allocateInfo.memoryTypeIndex    = mageFindMemoryType(requirements.memoryTypeBits, flags, renderer);
    allocateInfo.allocationSize     = requirements.size;
    MAGE_CHECK_VULKAN(vkAllocateMemory(renderer->Device, &allocateInfo, NULL, &buffer->AllocatedMemory));

    MAGE_CHECK_VULKAN(vkBindBufferMemory(renderer->Device, buffer->Buffer, buffer->AllocatedMemory, 0));
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
    bindingDescription.stride       = sizeof(struct mageVertex);
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
    attributeDescriptions[0].offset = offsetof(struct mageVertex, Vertex);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(struct mageVertex, Color);
    return attributeDescriptions;
}

#endif