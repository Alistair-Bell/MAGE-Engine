#include <mageAPI.h>

VkResult mageImageCreate(VkImage *image, VkDeviceMemory *memory, const uint32_t width, const uint32_t height, VkFormat format, VkImageTiling tilingMode, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, struct mageRenderer *renderer)
{
    VkImageCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkImageCreateInfo));
    createInfo.sType            = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.imageType        = VK_IMAGE_TYPE_2D;
    createInfo.extent.width     = width;
    createInfo.extent.height    = height;
    createInfo.extent.depth     = 1;
    createInfo.mipLevels        = 1;
    createInfo.arrayLayers      = 1;
    createInfo.format           = format;
    createInfo.tiling           = tilingMode;
    createInfo.initialLayout    = VK_IMAGE_LAYOUT_UNDEFINED;
    createInfo.usage            = usage;
    createInfo.samples          = VK_SAMPLE_COUNT_1_BIT;
    createInfo.sharingMode      = VK_SHARING_MODE_EXCLUSIVE;


    VkResult result = MAGE_VULKAN_CHECK(vkCreateImage(renderer->Device, &createInfo, NULL, image));
    if (result != VK_SUCCESS) { return result; }

    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(renderer->Device, *image, &memoryRequirements);

    VkMemoryAllocateInfo allocateInfo;
    memset(&allocateInfo, 0, sizeof(VkMemoryAllocateInfo));
    allocateInfo.sType                  = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.memoryTypeIndex        = mageFindMemoryType(memoryRequirements.memoryTypeBits, properties, renderer);
    allocateInfo.allocationSize         = memoryRequirements.size;

    result = MAGE_VULKAN_CHECK(vkAllocateMemory(renderer->Device, &allocateInfo, NULL, memory));
    if (result != VK_SUCCESS) { return result; }

    vkBindImageMemory(renderer->Device, *image, *memory, 0);
    return VK_SUCCESS;
}
VkResult mageImageViewCreate(VkImage image, VkImageView *view, VkFormat format, struct mageRenderer *renderer)
{
    VkImageViewCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkImageViewCreateInfo));
    
    createInfo.sType                            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image                            = image;
    createInfo.format                           = format;
    createInfo.viewType                         = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.subresourceRange.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel    = 0;
    createInfo.subresourceRange.levelCount      = 1;
    createInfo.subresourceRange.baseArrayLayer  = 0;
    createInfo.subresourceRange.layerCount      = 1;

    VkResult result = MAGE_VULKAN_CHECK(vkCreateImageView(renderer->Device, &createInfo, NULL, view));
    return result;
}
static void mageTransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, struct mageRenderer *renderer)
{
    VkCommandBuffer commandBuffer = mageCommandBufferBegin(renderer);
    
    VkImageMemoryBarrier barrier;
    memset(&barrier, 0, sizeof(VkImageMemoryBarrier));
    barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout                       = oldLayout;
    barrier.newLayout                       = newLayout;
    barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.image                           = image;
    barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel   = 0;
    barrier.subresourceRange.levelCount     = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount     = 1;
    
    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
    {
        barrier.srcAccessMask               = 0;
        barrier.dstAccessMask               = VK_ACCESS_TRANSFER_WRITE_BIT;
        sourceStage                         = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage                    = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } 
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
    {
        barrier.srcAccessMask               = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask               = VK_ACCESS_SHADER_READ_BIT;
        sourceStage                         = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage                    = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
        MAGE_LOG_CORE_FATAL_ERROR("Undefined image layout\n", NULL);
        assert(1 == 1);
    }
     

    vkCmdPipelineBarrier(commandBuffer, 0, 0, 0, 0, NULL, 0, NULL, 1, &barrier);

    mageCommandBufferEnd(commandBuffer, renderer);
}
static void mageCopyBufferToImage(struct mageBufferWrapper *wrapper, VkImage image, const uint32_t width, const uint32_t height, struct mageRenderer *renderer)
{
    VkCommandBuffer commandBuffer = mageCommandBufferBegin(renderer);
    
    VkBufferImageCopy region;
    memset(&region, 0, sizeof(VkBufferImageCopy));
    region.bufferOffset                     = 0;
    region.bufferRowLength                  = 0;
    region.bufferImageHeight                = 0;
    region.imageOffset                      = (VkOffset3D){ 0, 0, 0 };
    region.imageExtent                      = (VkExtent3D){ width, height, 1 };

    region.imageSubresource.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel        = 0;
    region.imageSubresource.baseArrayLayer  = 0;
    region.imageSubresource.layerCount      = 1;

    vkCmdCopyBufferToImage(commandBuffer, wrapper->Buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    mageCommandBufferEnd(commandBuffer, renderer);
}

static VkResult mageTextureSamplerCreate(VkSampler *sampler, VkSamplerAddressMode mode, struct mageRenderer *renderer)
{
    VkSamplerCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkSamplerCreateInfo));

    createInfo.sType                    = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    createInfo.magFilter                = VK_FILTER_LINEAR;
    createInfo.minFilter                = VK_FILTER_LINEAR;
    createInfo.addressModeU             = mode;
    createInfo.addressModeV             = mode;
    createInfo.addressModeW             = mode;
    createInfo.anisotropyEnable         = VK_TRUE;
    createInfo.maxAnisotropy            = 16.0f;
    createInfo.borderColor              = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    createInfo.compareEnable            = VK_FALSE;
    createInfo.compareOp                = VK_COMPARE_OP_ALWAYS;
    createInfo.unnormalizedCoordinates  = VK_FALSE;
    createInfo.mipmapMode               = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    return MAGE_VULKAN_CHECK(vkCreateSampler(renderer->Device, &createInfo, NULL, sampler));
}
static VkSamplerAddressMode mageSamplerModeToNative(mageTextureSamplerMode mode)
{
    switch (mode)
    {
    case MAGE_TEXTURE_SAMPLER_REPEAT:
        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case MAGE_TEXTURE_SAMPLER_MIRRORED_REPEAT:
        return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    case MAGE_TEXTURE_SAMPLER_CLAMP_TO_EDGE:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case MAGE_TEXTURE_SAMPLER_CLAMP_TO_BORDER:
        return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    case MAGE_TEXTURE_SAMPLER_MIRRORED_CLAMP_TO_EDGE:
        return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
    default:
        MAGE_LOG_CORE_WARNING("Unknown sampler mode, using repeat\n", NULL);
        return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    }
}
mageResult mageTextureCreate(struct mageTexture *texture, const char *texturePath, mageTextureSamplerMode samplerMode, mageTextureFileFormat format, struct mageRenderer *renderer)
{
    uint32_t width, height;
    uint8_t *fileData;

    switch (format)
    {
        case MAGE_TEXTURE_FILE_FORMAT_PNG:
        {
            MAGE_PNG_LOADE_CHECK(lodepng_decode32_file(&fileData, &width, &height, texturePath), fileData);
            break;
        }
        default:
            MAGE_LOG_CORE_ERROR("Only png file formats are currently supported\n", NULL);
            return MAGE_RESULT_INVALID_INPUT;
    }   
    VkDeviceSize textureSize = width * height * 4;

    struct mageBufferWrapper stagingBuffer;
    mageBufferWrapperAllocate(&stagingBuffer, textureSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, renderer); 
    
    void *memory;
    vkMapMemory(renderer->Device, stagingBuffer.AllocatedMemory, 0, textureSize, 0, &memory);
        memcpy(memory, fileData, textureSize);
    vkUnmapMemory(renderer->Device, stagingBuffer.AllocatedMemory);
    free(fileData);
    /*
    mageTransitionImageLayout(texture->Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, renderer);
        mageCopyBufferToImage(&stagingBuffer, texture->Image, width, height, renderer);
    mageTransitionImageLayout(texture->Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, renderer);
    */

    mageImageCreate(&texture->Image, &texture->DeviceMemory, width, height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, renderer); 
    mageImageViewCreate(texture->Image, &texture->View, VK_FORMAT_R8G8B8A8_SRGB, renderer);
    mageTextureSamplerCreate(&texture->Sampler, mageSamplerModeToNative(samplerMode), renderer);

    mageBufferWrapperDestroy(&stagingBuffer, renderer);
    MAGE_LOG_CORE_INFORM("Texture %s was created with source of %dpx by %dpx\n", texturePath, width, height);
    return MAGE_RESULT_SUCCESS;
}
void mageTextureDestroy(struct mageTexture *texture, struct mageRenderer *renderer)
{
    vkDestroyImageView(renderer->Device, texture->View, NULL);
    vkDestroyImage(renderer->Device, texture->Image, NULL);
    vkDestroySampler(renderer->Device, texture->Sampler, NULL);
    vkFreeMemory(renderer->Device, texture->DeviceMemory, NULL);
}
