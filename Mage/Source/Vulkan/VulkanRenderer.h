#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include "../Includes.h"
#include "../CreateStructures.h"
#include "../Window.h"

typedef struct MageRendererOverseer
{
    VkInstance               Instance;
    VkDevice                 PrimaryDevice;
    VkDebugUtilsMessengerEXT DebugMessenger;
} MageRendererOverseer;

typedef struct MageRendererSurface
{
    VkSurfaceKHR            Surface;
} MageRendererSurface;

typedef struct MageRendererPhysicalDeviceQueueFamilies /* Queue has 4 silent letters, the english language is a wacky language */
{
    U32 GraphicsFamilyIndex;
    U32 PresentFamilyIndex;
} MageRendererPhysicalDeviceQueueFamilies;

typedef struct MageRendererPhysicalDevice
{
    VkDevice                                LogicalDevice;
    VkPhysicalDevice                        GPU;
    VkPhysicalDeviceProperties              Properties;
    VkPhysicalDeviceMemoryProperties        MemoryProperties;
    MageRendererPhysicalDeviceQueueFamilies QueueFamilies;
} MageRendererPhysicalDevice;

typedef struct MageRenderer
{
    MageRendererOverseer        Overseer;
    MageRendererSurface         Surface;
    MageRendererPhysicalDevice  Device;
} MageRenderer;

static const char *MageVulkanRendererRequiredExtensions[] = 
{
    VK_KHR_SURFACE_EXTENSION_NAME,
    #if MAGE_BUILD_PLATFORM_LINUX
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
    #elif MAGE_BUILD_PLATFORM_WINDOWS
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    #endif
    #if MAGE_BUILD_DEBUG_MODE
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    #endif
};


extern U8 MageVulkanRendererCreateInstance(MageRendererCreateInfo *info, MageRenderer *renderer);
extern U8 MageVulkanRendererCreateDebugLayers(MageRendererCreateInfo *info, MageRenderer *renderer);
extern U8 MageVulkanRendererCreateSurface(MageRendererCreateInfo *info,  MageRenderer *renderer);
extern U8 MageVulkanRendererCreatePhysicalDevice(MageRendererCreateInfo *info, MageRenderer *renderer);

extern U64 MageVulkanRendererRatePhysicalDevice(MageRendererCreateInfo *info, MageRenderer *renderer, VkPhysicalDevice device);
extern U8  MageVulkanRendererFindPhysicalDeviceQueueFamiles(MageRenderer *renderer, VkPhysicalDevice device, MageRendererPhysicalDeviceQueueFamilies *families);

extern VKAPI_ATTR VkBool32 VKAPI_CALL MageVulkanValidationLayersCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, U0 *pUserData);

#endif