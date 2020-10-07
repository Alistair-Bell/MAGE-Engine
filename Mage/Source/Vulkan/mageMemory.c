#include <mageAPI.h>

VkPhysicalDeviceMemoryProperties mageVulkanMemoryGetDeviceProperties(VkPhysicalDevice device)
{
    VkPhysicalDeviceMemoryProperties properties;
    memset(&properties, 0, sizeof(VkPhysicalDeviceMemoryProperties));
    vkGetPhysicalDeviceMemoryProperties(device, &properties);
    return properties;
}
VkResult mageVulkanMemoryAllocateHeap(VkDevice device, struct mageVulkanMemoryHeap *heap, uint64_t flags, uint64_t bytes)
{       
    heap->BlockSize         = (VkDeviceSize) bytes;

    VkMemoryAllocateInfo info;
    memset(&info, 0, sizeof(VkMemoryAllocateInfo));
    info.sType              = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.allocationSize     = (VkDeviceSize) bytes;

    MAGE_VULKAN_CHECK(vkAllocateMemory(device, &info, NULL, &heap->Memory));
    MAGE_LOG_CORE_INFORM("Allocating vulkan memory of size %lu\n", bytes);
    return VK_SUCCESS;
}
void mageVulkanMemoryFreeMemory(VkDevice device, struct mageVulkanMemoryHeap *heap)
{
    MAGE_LOG_CORE_INFORM("Freeing vulkan memory of size %lu\n", (uint64_t)heap->BlockSize);
    vkFreeMemory(device, heap->Memory, NULL);
}