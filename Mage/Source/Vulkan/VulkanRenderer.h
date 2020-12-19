#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include "../Includes.h"
#include "../CreateStructures.h"

typedef struct MageRendererOverseer
{
    VkInstance               Instance;
    VkDevice                 PrimaryDevice;
    VkDebugUtilsMessengerEXT DebugMessenger;
} MageRendererOverseer;

typedef struct MageRenderer
{
    MageRendererOverseer Overseer;
} MageRenderer;

extern U8 MageVulkanRendererCreateInstance(MageRendererCreateInfo *info, MageRenderer *renderer);
extern U8 MageVulkanRendererCreateDebugLayers(MageRendererCreateInfo *info, MageRenderer *renderer);
extern U8 MageVulkanRendererCreateSurface(MageRendererCreateInfo *info,  MageRenderer *renderer);
extern VKAPI_ATTR VkBool32 VKAPI_CALL MageVulkanValidationLayersCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, U0 *pUserData);

#endif