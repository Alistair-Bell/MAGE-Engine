#include <mageAPI.h>

#define SORT_NAME sort_algorithms
#define SORT_TYPE uint64_t
#define SORT_CMP(x, y) ((x) - (y))
#include "../../Externals/sort/sort.h"

uint32_t mageVulkanMemoryFindMemoryType(VkPhysicalDeviceMemoryProperties propeties, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{

    uint32_t i;
    for (i = 0; i < propeties.memoryTypeCount; i++) 
    {
        if ((typeFilter & (1 << i)) && (propeties.memoryTypes[i].propertyFlags & properties) == properties) 
        {
            return i;
        }
    }
    MAGE_ASSERT_MESSAGE(MAGE_TRUE != MAGE_TRUE, "Unable to find memory type index!\n", NULL);
    return UINT32_MAX;
}
VkPhysicalDeviceMemoryProperties mageVulkanMemoryGetDeviceProperties(VkPhysicalDevice device)
{
    VkPhysicalDeviceMemoryProperties properties;
    memset(&properties, 0, sizeof(VkPhysicalDeviceMemoryProperties));
    vkGetPhysicalDeviceMemoryProperties(device, &properties);
    return properties;
}
VkResult mageVulkanMemoryAllocateHeap(VkDevice device, VkPhysicalDevice gpu, struct mageVulkanMemoryHeap *heap, const uint32_t heapIndex, const uint64_t bytes)
{    
    memset(heap, 0, sizeof(struct mageVulkanMemoryHeap));
    memset(heap->HeapOffsets, 0, sizeof(VkDeviceSize) * MAGE_VULKAN_MEMORY_MAX_OFFSET_COUNTS);
    VkPhysicalDeviceMemoryProperties memoryProperties = mageVulkanMemoryGetDeviceProperties(gpu);

    /* For optimistations to the space that the memory uses align to 1024 */
    MAGE_ASSERT_MESSAGE((bytes % 1024 == 0), "Memory heap allocation failed, block size of %lu not alligned to whole bits!\n", bytes);


    MAGE_LOG_CORE_INFORM("Forcing using gpu heap with index of %lu, of %lu indexes\n", heapIndex, memoryProperties.memoryHeapCount);
    MAGE_ASSERT_MESSAGE(heapIndex <= memoryProperties.memoryHeapCount, "GPU memory heap index too large!\n", NULL);
    MAGE_ASSERT_MESSAGE(memoryProperties.memoryHeaps[heapIndex].size >= bytes, "Memory requested exceeds heap size of %luK\n", memoryProperties.memoryHeaps[heapIndex].size / 1024);
    
    VkMemoryAllocateInfo info;
    memset(&info, 0, sizeof(VkMemoryAllocateInfo));
    info.sType                      = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    info.allocationSize             = (VkDeviceSize) bytes;
    info.memoryTypeIndex            = heapIndex;

    MAGE_VULKAN_CHECK(vkAllocateMemory(device, &info, NULL, &heap->Memory));

    heap->AssociatedHeapIndex   = heapIndex;
    heap->BlockSize             = bytes;
    heap->Unallocated           = bytes;
    heap->OffsetCount           = 0;
    heap->NextOffset            = &heap->HeapOffsets[0];
    MAGE_LOG_CORE_INFORM("Allocating vulkan memory of size %luK\n", bytes / 1024);
    return VK_SUCCESS;
}
uint32_t mageVulkanMemoryBufferMapToBlock(VkDevice device, struct mageVulkanMemoryHeap *heap, VkBuffer *buffer, const VkBufferUsageFlags flags, void *data, const uint64_t size)
{   
    MAGE_ASSERT_MESSAGE(size <= heap->Unallocated, "Heap has too little space for buffer of %luK, only %luK remaining\n", (uint64_t)(size / 1024), (uint64_t)(heap->Unallocated / 1024));

    VkBufferCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkBufferCreateInfo));
    createInfo.sType        = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.size         = (VkDeviceSize)size;
    createInfo.sharingMode  = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.usage        = flags;

    uint32_t bufferOffset;
    if (heap->OffsetCount <= 0)
        bufferOffset = 0;
    else
        bufferOffset = *heap->NextOffset + size;

    MAGE_VULKAN_CHECK(vkCreateBuffer(device, &createInfo, NULL, buffer));
    vkBindBufferMemory(device, *buffer, heap->Memory, bufferOffset);

    heap->OffsetCount++;
    heap->NextOffset    = &heap->HeapOffsets[heap->OffsetCount];
    heap->Unallocated   -= size;

    MAGE_LOG_CORE_INFORM("Binding buffer memory to heap of %luK, using offset of %lu, heap has %luK remaining\n", (uint64_t)heap->BlockSize / 1024, (uint64_t)bufferOffset, (uint64_t)heap->Unallocated / 1024);
    return bufferOffset;
}
void mageVulkanMemoryBufferUnmapBufferToBlock(VkDevice device, struct mageVulkanMemoryHeap *heap, VkBuffer *buffer, const uint32_t bufferOffset)
{
    vkDestroyBuffer(device, *buffer, NULL);
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