#include "VulkanRenderer.h"

#define MAGE_MEMORY_CHUNK_SIZE 67108864 /* 1024 ^2 * 64 (64MB) */
#define MAGE_MEMORY_FOWARD_ALLOCATION_SIZE(num) num / 8
/* 
 * Handles vulkan memory, manages and optimises based on the specification of the user
 * Each memory heap corresponds to a heap that vulkan specifies 
 * The block is a chunk of allocated memory inside of a heap 
 * Each section is an offset of a chunk
*/
static U8 MageVulkanMemoryAllocate(VkDevice device, U32 memoryTypeIndex, VkDeviceMemory *memory, U64 size)
{
    VkMemoryAllocateInfo allocateInfo;
    memset(&allocateInfo, 0, sizeof(VkMemoryAllocateInfo));
    allocateInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocateInfo.allocationSize  = (VkDeviceSize)size;
    allocateInfo.memoryTypeIndex = memoryTypeIndex;
    VkResult r = vkAllocateMemory(device, &allocateInfo, NULL, memory);
    return r == VK_SUCCESS; 
}
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
        heap->HeapIndex  = memoryProperties.memoryTypes[i].heapIndex;
        heap->Flags      = memoryProperties.memoryTypes[i].propertyFlags;

        U32 fowardAllocateCount = MAGE_MEMORY_FOWARD_ALLOCATION_SIZE(blockCount);
        if (fowardAllocateCount == 0)
            fowardAllocateCount++;

        for (j = 0; j < fowardAllocateCount; j++)
        {
            MageVulkanMemoryBlock *block = &heap->Blocks[j];
            U8 r = MageVulkanMemoryAllocate(renderer->Device.LogicalDevice, memoryProperties.memoryTypes[i].heapIndex, &block->AssociatedMemory, MAGE_MEMORY_CHUNK_SIZE);
            MAGE_HANDLE_ERROR_MESSAGE(!r, printf("Error: Unable to allocate memory, heap %d, requested size %lu\n", i, (U64)MAGE_MEMORY_CHUNK_SIZE));
            block->Allocated          = MageTrue;
            block->MaxSectionCount    = 128;
            block->Sections           = calloc(0, sizeof(MageVulkanMemoryBlockSections));
            block->ActiveSectionCount = 0;
        }
        printf("Heap %d, blocked into %d chunks of %luMB, foward allocated %d chunks (12.5%% approx)\n", i, blockCount, (memCount / (1024 * 1024)) /* / blockCount */, fowardAllocateCount);
    }
    renderer->NextBlock = &renderer->Heaps[0].Blocks[0];
    return MageTrue;
}
U8 MageVulkanRendererHeapsRequestMemory(MageRenderer *renderer, MageVulkanMemoryRequestInfo *requestInfo, VkDeviceMemory *sectionMemory, MageVulkanMemoryBlockSections *section)
{
    U32 i;
    /* Linear search is most likely the fastest, the small search size means heap or quick sort then binary might be slower, might need to test! */
    for (i = 0; i < renderer->HeapCount; i++)
    {
        if (renderer->Heaps[i].Flags & requestInfo->DesiredFlagBits)
            goto evaluate;   
    }
  
    MAGE_HANDLE_ERROR_MESSAGE(1 == 1, printf("Error: Cannot find heap with desired flags %d\n", requestInfo->DesiredFlagBits));
    evaluate:
    {
        MageVulkanMemoryBlock *current = renderer->NextBlock;

        if (!current->Allocated)
        {
            U8 r = MageVulkanMemoryAllocate(renderer->Device.LogicalDevice, renderer->Heaps[i].HeapIndex, &current->AssociatedMemory, requestInfo->BufferInfo->Size);
            MAGE_HANDLE_ERROR_MESSAGE(!r, printf("Error: Unable to allocate memory, heap %d, requested size %lu\n", i, requestInfo->BufferInfo->Size));
        }

        if (!(current->ActiveSectionCount < current->MaxSectionCount))
        {

        }

        current->Sections = realloc(current->Sections, sizeof(MageVulkanMemoryBlockSections) * (current->ActiveSectionCount + 1));
        section = &current->Sections[current->ActiveSectionCount];
        U32 activeCount = current->ActiveSectionCount;
        if (activeCount <= 0)
            *section = 0;
        else
            *section = current->Sections[activeCount - 1] + requestInfo->BufferInfo->Size;
        current->ActiveSectionCount++;

        sectionMemory = &current->AssociatedMemory;
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
