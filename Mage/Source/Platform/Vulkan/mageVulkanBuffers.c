#include "mageVulkanAPI.h"

#if defined (MAGE_VULKAN)


VkBufferUsageFlagBits mageBufferTypeToFlagBits(const mageBufferType type)
{
    switch (type)
    {
        case MAGE_BUFFER_TYPE_INDEX:
            return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        case MAGE_BUFFER_TYPE_VERTEX:
            return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        default:
            MAGE_LOG_CORE_WARNING("Invalid buffer type %d\n", type);
    }
}
void mageBufferCreate(struct mageBuffer *buffer, mageBufferType bufferType, void *data, uint32_t dataByteSize, struct mageRenderer *renderer)
{
    buffer->StructureType = MAGE_STRUCTURE_TYPE_BUFFER;
    mageBufferWrapperAllocate(&buffer->Wrapper, data, dataByteSize, mageBufferTypeToFlagBits(bufferType), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, renderer);
}
void mageBufferDestroy(struct mageBuffer *buffer, struct mageRenderer *renderer)
{
    mageBufferWrapperDestroy(&buffer->Wrapper, renderer);
}
void mageVertexInitialise(struct mageVertex *vertexInstance, struct vector2 vertex, struct vector3 color)
{
    vertexInstance->StructureType = MAGE_STRUCTURE_TYPE_VERTEX;
    vertexInstance->Vertex = vertex;
    vertexInstance->Color = color; 
}
void mageBufferWrapperAllocate(struct mageBufferWrapper *buffer, void *data, uint32_t dataSize, const VkBufferUsageFlags bufferUsage, VkMemoryPropertyFlags flags, struct mageRenderer *renderer)
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

    void *memory;
    vkMapMemory(renderer->Device, buffer->AllocatedMemory, 0, dataSize, 0, &memory);
        memcpy(memory, data, dataSize);
    vkUnmapMemory(renderer->Device, buffer->AllocatedMemory);
}
void mageBufferWrapperDestroy(struct mageBufferWrapper *buffer, struct mageRenderer *renderer)
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