#include "VulkanRenderer.h"

#define MAGE_MEMORY_CHUNK_SIZE 67108864 /* 1024 ^2 * 64 (64MB) */
#define MAGE_MEMORY_FOWARD_ALLOCATION_SIZE(num) num / 8
/* 
 * Handles vulkan memory, manages and optimises based on the specification of the user
 * Each memory heap corresponds to a heap that vulkan specifies 
 * The block is a chunk of allocated memory inside of a heap 
 * Each section is an offset of a chunk
*/

U8 MageVulkanRendererHeapsCreate(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(renderer->Device.GPU, &memoryProperties); 

    renderer->Heaps     = calloc(memoryProperties.memoryHeapCount, sizeof(MageVulkanMemoryHeap));
    renderer->HeapCount = memoryProperties.memoryHeapCount;

    U32 i, j;
    
    /* Block out the heaps */
    for (i = 0; i < renderer->HeapCount; i++)
    {
        MageVulkanMemoryHeap *heap = &renderer->Heaps[i]; 

        U64 memCount  = memoryProperties.memoryHeaps[i].size;
        U32 blockCount = memCount / MAGE_MEMORY_CHUNK_SIZE;
       
        heap->BlockCount = blockCount;
        heap->Blocks     = calloc(blockCount, sizeof(MageVulkanMemoryBlock));

        U32 fowardAllocateCount = MAGE_MEMORY_FOWARD_ALLOCATION_SIZE(blockCount);
        if (fowardAllocateCount == 0)
            fowardAllocateCount++;

        for (j = 0; j < fowardAllocateCount; j++)
        {
            MageVulkanMemoryBlock *block = &heap->Blocks[j];

            VkMemoryAllocateInfo allocateInfo;
            memset(&allocateInfo, 0, sizeof(VkMemoryAllocateInfo));
            allocateInfo.sType            = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocateInfo.allocationSize   = MAGE_MEMORY_CHUNK_SIZE;
            allocateInfo.memoryTypeIndex  = memoryProperties.memoryTypes[i].heapIndex;
            vkAllocateMemory(renderer->Device.LogicalDevice, &allocateInfo, NULL, &block->AssociatedMemory);
            block->Allocated = MageTrue;
        }
        printf("Heap %d, blocked into %d chunks of %luMB, foward allocated %d chunks (12.5%% approx)\n", i, blockCount, memCount / (1024 * 1024), fowardAllocateCount);
    }

    return MageTrue;
}
U8 MageVulkanRendererHeapsDestroy(MageRenderer *renderer)
{
    U32 i, j;
    for (i = 0; i < renderer->HeapCount; i++)
    {
        MageVulkanMemoryHeap *heap = &renderer->Heaps[i];
        for (j = 0; j < heap->BlockCount; j++)
        {
            MageVulkanMemoryBlock *block = &heap->Blocks[j];
            if (block->Allocated)
            {
                vkFreeMemory(renderer->Device.LogicalDevice, block->AssociatedMemory, NULL);
            }
            free(block->Sections);
        }
        free(heap->Blocks);
    }


    return MageTrue;
}
