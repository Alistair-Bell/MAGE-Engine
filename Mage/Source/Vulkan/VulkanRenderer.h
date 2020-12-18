#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include "../Includes.h"
#include "../CreateStructures.h"

typedef struct MageRendererOverseer
{
    VkInstance            Instance;
    VkDevice              PrimaryDevice;
} MageRendererOverseer;

typedef struct MageRenderer
{
    MageRendererOverseer Overseer;
} MageRenderer;

extern U8 MageVulkanRendererCreateInstance(MageRendererCreateInfo *info, MageRenderer *renderer);

#endif