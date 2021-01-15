#ifndef GRAPHICS_BUFFER
#define GRAPHICS_BUFFER

#include "Includes.h"
#include "CreateStructures.h"

#if MAGE_BUILD_RENDERER_VULKAN
    #include "Vulkan/VulkanRenderer.h"
#endif

#if MAGE_BUILD_RENDERER_VULKAN

typedef struct MageGraphicsBuffer
{
    VkBuffer                      Buffer;
    MageVulkanMemoryBlockSections *AssociatedSection;
} MageGraphicsBuffer;

#endif

extern U8 MageGraphicsBufferCreate(MageGraphicsBufferCreateInfo *info, MageGraphicsBuffer *buffer, MageRenderer *render);
extern U8 MageGraphicsBufferDestroy(MageGraphicsBuffer *buffer, MageRenderer *renderer);



#endif
