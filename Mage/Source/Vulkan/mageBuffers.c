#include <mageAPI.h>

VkBufferUsageFlags mageBufferTypeToFlag(const mageBufferType type)
{
    switch (type)
    {
        case MAGE_BUFFER_TYPE_INDEX:
            return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        case MAGE_BUFFER_TYPE_VERTEX:
            return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        case MAGE_BUFFER_TYPE_SOURCE:
            return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	    case MAGE_BUFFER_TYPE_TRANSFER:
            return VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        default:
            MAGE_LOG_CORE_WARNING("Invalid buffer type %d\n", type);
    }
}
static void mageBufferCopy(VkBuffer sourceBuffer, VkBuffer destinationBuffer, VkDeviceSize size, struct mageRenderer *renderer)
{
    VkCommandBuffer commandBuffer = mageCommandBufferBegin(renderer);
        VkBufferCopy copyRegion;
        memset(&copyRegion, 0, sizeof(VkBufferCopy));
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, sourceBuffer, destinationBuffer, 1, &copyRegion);
    mageCommandBufferEnd(commandBuffer, renderer);    
}
void mageBufferCreate(struct mageBuffer *buffer, const mageBufferType bufferType, void *data, uint32_t dataByteSize, struct mageRenderer *renderer)
{
    buffer->Bytes = dataByteSize;
    buffer->BufferType = bufferType;
    struct mageBufferWrapper stagingBuffer;

    mageBufferWrapperAllocate(&stagingBuffer, dataByteSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, renderer);

    void *memory;
    vkMapMemory(renderer->Device, stagingBuffer.AllocatedMemory, 0, dataByteSize, 0, &memory);
        memcpy(memory, data, dataByteSize);
    vkUnmapMemory(renderer->Device, stagingBuffer.AllocatedMemory);

    mageBufferWrapperAllocate(&buffer->Wrapper, dataByteSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | mageBufferTypeToFlag(bufferType), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, renderer);    

    mageBufferCopy(stagingBuffer.Buffer, buffer->Wrapper.Buffer, dataByteSize, renderer);

    mageBufferWrapperDestroy(&stagingBuffer, renderer);
}
VkBuffer mageBufferGetNativeBuffer(struct mageBuffer *buffer)
{
    return buffer->Wrapper.Buffer;
}
void mageBufferUpdate(struct mageBuffer *buffer, void *data, uint32_t size, struct mageRenderer *renderer)
{

    if (buffer->Bytes != size)
    {
        /* Reallocate the buffer */
        mageBufferDestroy(buffer, renderer);
        mageBufferCreate(buffer, buffer->BufferType, data, size, renderer);
    }
    struct mageBufferWrapper stagingBuffer;
    mageBufferWrapperAllocate(&stagingBuffer, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, renderer);
    
    void *memory;
    vkMapMemory(renderer->Device, stagingBuffer.AllocatedMemory, 0, size, 0, &memory);
        memcpy(memory, data, size);
    vkUnmapMemory(renderer->Device, stagingBuffer.AllocatedMemory);

    mageBufferCopy(stagingBuffer.Buffer, buffer->Wrapper.Buffer, size, renderer);
    mageBufferWrapperDestroy(&stagingBuffer, renderer);
}
void mageBufferDestroy(struct mageBuffer *buffer, struct mageRenderer *renderer)
{
    mageBufferWrapperDestroy(&buffer->Wrapper, renderer);
}
void mageVertexInitialise(struct mageVertex *vertexInstance, struct mageVector2 vertex, struct mageVector3 color)
{
    vertexInstance->Vertex = vertex;
    vertexInstance->Color = color; 
}
void mageBufferWrapperAllocate(struct mageBufferWrapper *buffer, uint32_t dataSize, const VkBufferUsageFlags bufferUsage, VkMemoryPropertyFlags flags, struct mageRenderer *renderer)
{
    VkMemoryRequirements requirements;
    
    VkBufferCreateInfo bufferInfo;
    memset(&bufferInfo, 0, sizeof(VkBufferCreateInfo));

    bufferInfo.sType        = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size         = dataSize;
    bufferInfo.usage        = bufferUsage;
    bufferInfo.sharingMode  = VK_SHARING_MODE_EXCLUSIVE;

    MAGE_VULKAN_CHECK(vkCreateBuffer(renderer->Device, &bufferInfo, NULL, &buffer->Buffer));

    vkGetBufferMemoryRequirements(renderer->Device, buffer->Buffer, &requirements);

    VkMemoryAllocateInfo allocateInfo;
    memset(&allocateInfo, 0, sizeof(VkMemoryAllocateInfo));
    allocateInfo.sType              = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.memoryTypeIndex    = mageFindMemoryType(requirements.memoryTypeBits, flags, renderer);
    allocateInfo.allocationSize     = requirements.size;
    MAGE_VULKAN_CHECK(vkAllocateMemory(renderer->Device, &allocateInfo, NULL, &buffer->AllocatedMemory));
    MAGE_VULKAN_CHECK(vkBindBufferMemory(renderer->Device, buffer->Buffer, buffer->AllocatedMemory, 0));
}
void mageBufferWrapperDestroy(struct mageBufferWrapper *buffer, struct mageRenderer *renderer)
{
    vkFreeMemory(renderer->Device, buffer->AllocatedMemory, NULL);
    vkDestroyBuffer(renderer->Device, buffer->Buffer, NULL);
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
    VkVertexInputAttributeDescription *attributeDescriptions = MAGE_MEMORY_ARRAY_ALLOCATE(3, sizeof(VkVertexInputAttributeDescription));
    *count = 3;
    memset(attributeDescriptions, 0, sizeof(VkVertexInputAttributeDescription) * 2);

    attributeDescriptions[0].binding    = 0;
    attributeDescriptions[0].location   = 0;
    attributeDescriptions[0].format     = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset     = offsetof(struct mageVertex, Vertex);

    attributeDescriptions[1].binding    = 0;
    attributeDescriptions[1].location   = 1;
    attributeDescriptions[1].format     = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset     = offsetof(struct mageVertex, Color);
    
    attributeDescriptions[2].binding    = 0;
    attributeDescriptions[2].location   = 2;
    attributeDescriptions[2].format     = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset     = offsetof(struct mageVertex, TextureLocation);

    return attributeDescriptions;
}
