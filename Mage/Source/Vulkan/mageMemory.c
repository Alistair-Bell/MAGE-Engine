#include <mageAPI.h>

VkPhysicalDeviceMemoryProperties mageVulkanMemoryGetDeviceProperties(VkPhysicalDevice device)
{
    VkPhysicalDeviceMemoryProperties properties;
    memset(&properties, 0, sizeof(VkPhysicalDeviceMemoryProperties));
    vkGetPhysicalDeviceMemoryProperties(device, &properties);
    return properties;
}
VkResult mageVulkanMemoryAllocateHeap(VkDevice device, VkPhysicalDevice gpu, struct mageVulkanMemoryHeap *heap, const uint64_t bytes)
{    
    memset(heap, 0, sizeof(struct mageVulkanMemoryHeap));

    /* For optimistations to the space that the memory uses align to 1024 */
    if (heap->BlockSize % 1024 != 0)
    {
        MAGE_LOG_CORE_ERROR("Memory heap allocation failed, block size of %lu not alligned to whole bits!\n", bytes);
        return VK_ERROR_UNKNOWN;
    }
    VkMemoryAllocateInfo info;
    memset(&info, 0, sizeof(VkMemoryAllocateInfo));
    info.sType              = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.allocationSize     = (VkDeviceSize) bytes;

    MAGE_VULKAN_CHECK(vkAllocateMemory(device, &info, NULL, &heap->Memory));

    heap->Unallocated       = heap->BlockSize;
    heap->BlockSize         = (VkDeviceSize)bytes;
    heap->NextOffset        = &heap->BufferBlockOffsets[0];

    MAGE_LOG_CORE_INFORM("Allocating vulkan memory of size %luK\n", bytes / 1024);
    return VK_SUCCESS;
}
void mageVulkanMemoryMapBufferToBlock(VkDevice device, struct mageVulkanMemoryHeap *heap, VkBuffer *buffer, const VkBufferUsageFlags flags, void *data, const uint64_t size)
{   
    VkBufferCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkBufferCreateInfo));
    createInfo.sType        = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.size         = (VkDeviceSize)size;
    createInfo.sharingMode  = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.usage        = flags;

    MAGE_VULKAN_CHECK(vkCreateBuffer(device, &createInfo, NULL, buffer));
    MAGE_LOG_CORE_INFORM("Binding buffer memory to heap of %luK, using offset of %lu\n", (uint64_t)heap->BlockSize / 1024, (uint64_t)*heap->NextOffset);

    vkBindBufferMemory(device, *buffer, heap->Memory, *heap->NextOffset);
    VkDeviceSize previousOffset = *heap->NextOffset;
    heap->NextOffset++;
    *heap->NextOffset += (previousOffset + size);

    MAGE_LOG_CORE_INFORM("Using next buffer offset of %lu\n", *heap->NextOffset);
}
void mageVulkanMemoryFreeMemory(VkDevice device, struct mageVulkanMemoryHeap *heap)
{
    /* Saftey net for the memory being invalid */
    if (heap->BlockSize >= 0)
    {
        MAGE_LOG_CORE_INFORM("Freeing vulkan memory of size %luK\n", (uint64_t)heap->BlockSize / 1024);
        vkFreeMemory(device, heap->Memory, NULL);
    }
}