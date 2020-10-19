#include <mageAPI.h>

#define SORT_NAME sort_algorithms
#define SORT_TYPE uint32_t
#include "../../Externals/sort/sort.h"

uint32_t mageVulkanMemoryFindMemoryType(VkPhysicalDevice device, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    uint32_t i;
    VkPhysicalDeviceMemoryProperties p = mageVulkanMemoryGetDeviceProperties(device);
    for (i = 0; i < p.memoryTypeCount; i++) 
    {
        if ((typeFilter & (1 << i)) && (p.memoryTypes[i].propertyFlags & properties) == properties) 
        {
            return i;
        }
    }
    MAGE_ASSERT_MESSAGE(MAGE_TRUE != MAGE_TRUE, "Unable to find memory type index!\n", NULL);
    return UINT32_MAX;
}
uint32_t mageSearchOffsetIndex(uint32_t *array, uint32_t low, uint32_t high, uint32_t key)
{
    if (low <= high)
	{
		uint32_t mid = low + (high - low) / 2;
		if (key == array[mid])
			return mid;
		
        else if (key < array[mid])
			return mageSearchOffsetIndex(array, low, mid - 1, key);
		
        else
			return mageSearchOffsetIndex(array, mid + 1, high, key);
	}
	MAGE_ASSERT_MESSAGE(1 == 1, "Unable to find offset value %d\n", key);
    return UINT32_MAX;
}
VkPhysicalDeviceMemoryProperties mageVulkanMemoryGetDeviceProperties(VkPhysicalDevice device)
{
    VkPhysicalDeviceMemoryProperties properties;
    memset(&properties, 0, sizeof(VkPhysicalDeviceMemoryProperties));
    vkGetPhysicalDeviceMemoryProperties(device, &properties);
    return properties;
}
VkResult mageVulkanMemoryAllocateHeap(struct mageVulkanMemoryHeap *heap, struct mageVulkanMemoryHeapCreateInfo *info)
{    
    heap->Offsets = MAGE_MEMORY_ARRAY_ALLOCATE(MAGE_VULKAN_MEMORY_MAX_OFFSET_COUNTS, sizeof(uint32_t));
    memset(heap->Offsets, 0, sizeof(uint32_t) * MAGE_VULKAN_MEMORY_MAX_OFFSET_COUNTS);
    VkPhysicalDeviceMemoryProperties memoryProperties = mageVulkanMemoryGetDeviceProperties(info->PhysicalDevice);

    VkDeviceSize bytes = info->AllocationSize;
    uint32_t heapIndex = info->AssociatedHeap;

    /* For optimistations to the space that the memory uses align to 1024 */
    MAGE_ASSERT_MESSAGE((bytes % 1024 == 0), "Memory heap allocation failed, block size of %lu not alligned to whole bits!\n", bytes);

    MAGE_LOG_CORE_INFORM("Forcing using gpu heap with index of %lu, of %lu indexes\n", heapIndex, memoryProperties.memoryHeapCount);
    MAGE_ASSERT_MESSAGE(memoryProperties.memoryHeapCount >= heapIndex, "GPU memory heap index too large!\n", NULL);
    MAGE_ASSERT_MESSAGE(memoryProperties.memoryHeaps[heapIndex].size >= bytes, "Memory requested exceeds heap size of %luK\n", memoryProperties.memoryHeaps[heapIndex].size / 1024);
    
    VkMemoryAllocateInfo allocateInfo;
    memset(&allocateInfo, 0, sizeof(VkMemoryAllocateInfo));
    allocateInfo.sType                      = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize             = (VkDeviceSize) bytes;
    allocateInfo.memoryTypeIndex            = heapIndex;

    MAGE_VULKAN_CHECK(vkAllocateMemory(info->Device, &allocateInfo, NULL, &heap->Memory));

    heap->AssociatedHeapIndex   = heapIndex;
    heap->BlockSize             = bytes;
    heap->Unallocated           = bytes;
    heap->OffsetCount           = 0;
    heap->PreviousOffset        = &heap->Offsets[0];
    heap->Flags                 = memoryProperties.memoryTypes[heapIndex].propertyFlags;

    uint32_t incriment = info->AllocationSize / MAGE_VULKAN_MEMORY_MAX_OFFSET_COUNTS;
    uint32_t i;
    /* For binary search algorithms it is important that no repeat values are stored */
    for (i = 0; i < MAGE_VULKAN_MEMORY_MAX_OFFSET_COUNTS; i++)
        heap->Offsets[i] = i * incriment;

    MAGE_LOG_CORE_INFORM("Allocating vulkan memory of size %luK\n", bytes / 1024);
    return VK_SUCCESS;
}
void mageVulkanMemoryBufferMapToBlock(struct mageVulkanMemoryHeap *heap, struct mageVulkanMemoryMapBufferInfo *info)
{   
    MAGE_ASSERT_MESSAGE(info->DataSize <= heap->Unallocated, "Heap has too little space for buffer of %luK, only %luK remaining\n", (uint64_t)(info->DataSize / 1024), (uint64_t)(heap->Unallocated / 1024));

    VkBufferCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkBufferCreateInfo));
    createInfo.sType        = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.size         = (VkDeviceSize)info->DataSize;
    createInfo.sharingMode  = VK_SHARING_MODE_EXCLUSIVE;
    
    if (heap->Flags &= VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
        createInfo.usage    = VK_BUFFER_USAGE_TRANSFER_DST_BIT;    
    else
        createInfo.usage    = info->BufferUsage;
    
    uint32_t bufferOffset;
    if (heap->OffsetCount <= 0)
        bufferOffset = 0;
    else
        bufferOffset = *heap->PreviousOffset + info->DataSize;

    MAGE_VULKAN_CHECK(vkCreateBuffer(info->Device, &createInfo, NULL, info->Buffer));
    vkBindBufferMemory(info->Device, *info->Buffer, heap->Memory, bufferOffset);

    heap->Offsets[heap->OffsetCount]    = bufferOffset;
    heap->PreviousOffset                    = &heap->Offsets[heap->OffsetCount];
    heap->Unallocated                       -= info->DataSize;

    if (heap->Flags &= VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
    {
        /* 
            VK_MEMORY_HEAP_DEVICE_LOCAL_BIT requires a buffer to be coppied to the GPU memory
            vkMapMemory will fail if directly used on a buffer which is GPU memory and not host visable
        */
        
        VkBufferCreateInfo transferInfo;
        memset(&transferInfo, 0, sizeof(VkBufferCreateInfo));
        transferInfo.sType          = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        transferInfo.usage          = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
        transferInfo.sharingMode    = VK_SHARING_MODE_EXCLUSIVE;
        transferInfo.size           = info->DataSize;
        
        VkBuffer sourceBuffer;
        MAGE_VULKAN_CHECK(vkCreateBuffer(info->Device, &transferInfo, NULL, &sourceBuffer));

        VkMemoryRequirements requirements;
        vkGetBufferMemoryRequirements(info->Device, sourceBuffer, &requirements);

        /* Might be worth creating an area of memory in which these transfer buffers can live, TODO */
        VkMemoryAllocateInfo memoryInfo;
        memset(&memoryInfo, 0, sizeof(VkMemoryAllocateInfo));
        memoryInfo.sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryInfo.allocationSize       = info->DataSize;
        memoryInfo.memoryTypeIndex      = mageVulkanMemoryFindMemoryType(info->PhysicalDevice, requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

        VkDeviceMemory deviceMemory;
        MAGE_VULKAN_CHECK(vkAllocateMemory(info->Device, &memoryInfo, NULL, &deviceMemory));
        
        vkBindBufferMemory(info->Device, sourceBuffer, deviceMemory, 0);

        void *mapping;
        vkMapMemory(info->Device, deviceMemory, 0, info->DataSize, 0, &mapping);
            memcpy(mapping, info->Data, info->DataSize);
        vkUnmapMemory(info->Device, deviceMemory);

        VkCommandBufferBeginInfo beginInfo;
        memset(&beginInfo, 0, sizeof(VkCommandBufferBeginInfo));
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VkCommandBuffer command;
        if (info->CopyCommandBuffer == NULL)
        {
            MAGE_ASSERT_MESSAGE(info->CommandPool != NULL, "info->CommandPool must be a valid command pool pointer!\n", NULL);
            VkCommandBufferAllocateInfo allocationInfo;
            memset(&allocationInfo, 0, sizeof(VkCommandBufferAllocateInfo));
            allocationInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocationInfo.commandBufferCount       = 1;
            allocationInfo.commandPool              = *info->CommandPool;
            allocationInfo.level                    = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

            MAGE_VULKAN_CHECK(vkAllocateCommandBuffers(info->Device, &allocationInfo, &command));
        }
        else
            command = *info->CopyCommandBuffer;
        
        VkBufferCopy copyInfo;
        memset(&copyInfo, 0, sizeof(VkBufferCopy));
        copyInfo.srcOffset  = 0;
        copyInfo.dstOffset  = 0;
        copyInfo.size       = info->DataSize;

        vkBeginCommandBuffer(command, &beginInfo);
            vkCmdCopyBuffer(command, sourceBuffer, *info->Buffer, 1, &copyInfo);
        vkEndCommandBuffer(command);

        vkDestroyBuffer(info->Device, sourceBuffer, NULL);
        vkFreeMemory(info->Device, deviceMemory, NULL);
    }
    else
    {
        void *mapping;    
        vkMapMemory(info->Device, heap->Memory, bufferOffset, info->DataSize, 0, &mapping);
            memcpy(mapping, info->Data, info->DataSize);
        vkUnmapMemory(info->Device, heap->Memory);
    }

    MAGE_LOG_CORE_INFORM("Binding buffer memory to heap of %luK, using offset of %lu, heap has %luK remaining\n", (uint64_t)heap->BlockSize / 1024, (uint64_t)bufferOffset, (uint64_t)heap->Unallocated / 1024);
    
    if (info->Reference != NULL)
    {
        info->Reference->Buffer = info->Buffer;
        info->Reference->Offset = &heap->Offsets[heap->OffsetCount];
        info->Reference->Parent = heap;
        info->Reference->Size   = info->DataSize;
    }
    heap->OffsetCount++;
}
void mageVulkanMemoryBufferUnmapBufferToBlock(struct mageVulkanMemoryHeap *heap, struct mageVulkanMemoryUnmapBufferInfo *info)
{
    MAGE_ASSERT(0 < heap->OffsetCount);
    MAGE_ASSERT(0 != info->Reference->Size);
    MAGE_ASSERT(heap->OffsetCount < MAGE_VULKAN_MEMORY_MAX_OFFSET_COUNTS);

    uint32_t index = mageSearchOffsetIndex(heap->Offsets, 0, MAGE_VULKAN_MEMORY_MAX_OFFSET_COUNTS - 1, *info->Reference->Offset);
    heap->Unallocated += info->Reference->Size;
    
    vkDestroyBuffer(info->Device, *info->Reference->Buffer, NULL);
    /* TODO check performance on different algorithms, where sourced quick > heap > in place merge sort */
    sort_algorithms_quick_sort(heap->Offsets, MAGE_VULKAN_MEMORY_MAX_OFFSET_COUNTS + 1);
}

void mageVulkanMemoryFreeMemory(VkDevice device, struct mageVulkanMemoryHeap *heap)
{
    /* Saftey net for the memory being invalid */
    if (heap->BlockSize >= 0)
    {
        MAGE_LOG_CORE_INFORM("Freeing vulkan memory of size %luK\n", (uint64_t)heap->BlockSize / 1024);
        vkFreeMemory(device, heap->Memory, NULL);
    }
    MAGE_MEMORY_FREE(heap->Offsets);
}