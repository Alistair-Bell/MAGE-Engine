#include <mageAPI.h>

static VkResult mageCreateImage(VkImage *image, VkDeviceMemory *memory, const uint32_t width, const uint32_t height, VkFormat format, VkImageTiling tilingMode, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, struct mageRenderer *renderer)
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


    mageCreateImage(&texture->Image, &texture->DeviceMemory, width, height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, renderer); 
    mageBufferWrapperDestroy(&stagingBuffer, renderer);
    MAGE_LOG_CORE_INFORM("Texture %s was created with source of %dpx by %dpx\n", texturePath, width, height);
    free(fileData);
    return MAGE_RESULT_SUCCESS;
}
void mageTextureDestroy(struct mageTexture *texture, struct mageRenderer *renderer)
{
    vkFreeMemory(renderer->Device, texture->DeviceMemory, NULL);
    vkDestroyImage(renderer->Device, texture->Image, NULL);
}
