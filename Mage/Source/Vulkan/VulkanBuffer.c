#include "VulkanRenderer.h"
#include "../GraphicsBuffer.h"

static VkBufferUsageFlagBits MageGraphicsBufferTypeToNative(const MageGraphicsBufferCreateInfoType flags)
{
    switch (flags)
    {
        case MAGE_GRAPHICS_BUFFER_CREATE_INFO_TYPE_VERTEX:  return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        case MAGE_GRAPHICS_BUFFER_CREATE_INFO_TYPE_INDEX:   return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        case MAGE_GRAPHICS_BUFFER_CREATE_INFO_TYPE_UNIFORM: return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    }
    return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
}

U8 MageGraphicsBufferCreate(MageGraphicsBufferCreateInfo *info, MageGraphicsBuffer *buffer, MageRenderer *render)
{
    if (info->Flags == MAGE_GRAPHICS_BUFFER_CREATE_INFO_FLAGS_STATIC)
    {
        /* Use dump in the gpu local heap */
        return MageTrue;  
    }

    VkBufferCreateInfo bufferInfo;
    memset(&bufferInfo, 0, sizeof(VkBufferCreateInfo));
    bufferInfo.sType            = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.sharingMode      = VK_SHARING_MODE_EXCLUSIVE;
    bufferInfo.size             = info->Size;
    bufferInfo.usage            = MageGraphicsBufferTypeToNative(info->Type);

    vkCreateBuffer(render->Device.LogicalDevice, &bufferInfo, NULL, &buffer->Buffer);
    return MageTrue;
}
U8 MageGraphicsBufferDestroy(MageGraphicsBuffer *buffer, MageRenderer *renderer)
{
    return MageTrue;
}


