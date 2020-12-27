#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include "../Includes.h"
#include "../CreateStructures.h"
#include "../Window.h"
#include "../Maths.h"

typedef struct MageRendererOverseer
{
    VkInstance               Instance;
    VkDebugUtilsMessengerEXT DebugMessenger;
} MageRendererOverseer;

typedef struct MageRendererSurface
{
    VkSurfaceKHR             Surface;
} MageRendererSurface;

typedef struct MageRendererPhysicalDeviceQueueFamilies /* Queue has 4 silent letters, the english language is a wacky language */
{
    U32 GraphicsFamilyIndex;
    U32 PresentFamilyIndex;
} MageRendererPhysicalDeviceQueueFamilies;

typedef struct MageRendererSurfaceSwapchainSupport
{
    VkSurfaceCapabilitiesKHR Capabilities;
    VkSurfaceFormatKHR       *Formats;
    VkPresentModeKHR         *PresentModes;
    U32                      FormatCount;
    U32                      PresentCount;
} MageRendererSurfaceSwapchainSupport;

typedef struct MageRendererPhysicalDevice
{
    VkDevice                                LogicalDevice;
    VkPhysicalDevice                        GPU;
    VkPhysicalDeviceProperties              Properties;
    VkPhysicalDeviceMemoryProperties        MemoryProperties;
    MageRendererPhysicalDeviceQueueFamilies QueueFamilies;
} MageRendererPhysicalDevice;

typedef struct MageRendererQueues
{
    VkQueue                 GraphicsQueue;
    VkQueue                 PresentQueue;
} MageRendererQueues;

typedef struct MageRendererSwapChain
{
    VkExtent2D              CurrentExtent;
    VkFormat                PrimaryFormat;
    VkSwapchainKHR          PrimarySwapchain;
    VkImage                 *Images;
    VkImageView             *ImageViews;
    U32                     ImagesCount;
} MageRendererSwapChain;

typedef struct MageRendererPipeline
{
    VkPipeline              GraphicsPipeline;
    VkPipelineLayout        GraphicsPipelineLayout;
} MageRendererPipeline;

typedef struct MageRenderer
{
    MageRendererOverseer        Overseer;
    MageRendererSurface         Surface;
    MageRendererPhysicalDevice  Device;
    MageRendererQueues          QueueHandles;
    MageRendererSwapChain       SwapChain;
    MageRendererPipeline        Pipeline;
} MageRenderer;


extern U8 MageVulkanRendererCreateInstance(MageRendererCreateInfo *info, MageRenderer *renderer);

extern U8 MageVulkanRendererCreatePhysicalDevice(MageRendererCreateInfo *info, MageRenderer *renderer);
extern U64 MageVulkanRendererRatePhysicalDevice(MageRendererCreateInfo *info, MageRenderer *renderer, VkPhysicalDevice device);
extern U8  MageVulkanRendererFindPhysicalDeviceQueueFamiles(MageRenderer *renderer, VkPhysicalDevice device, MageRendererPhysicalDeviceQueueFamilies *families);
extern U8  MageVulkanRendererFindPhysicalDeviceExtensions(VkPhysicalDevice device, const char *deviceExtensions[], const U32 count);

extern U8 MageVulkanRendererCreateSurface(MageRendererCreateInfo *info,  MageRenderer *renderer);
extern U8 MageVulkanRendererSurfaceFindSwapchainSupport(MageRenderer *renderer, MageRendererSurfaceSwapchainSupport *info);
extern U8 MageVulkanRendererSurfacePickCorrectFormats(MageRenderer *renderer, MageRendererSurfaceSwapchainSupport *info, VkSurfaceFormatKHR *format, VkPresentModeKHR *present);
extern U8 MageVulkanRendererSurfaceSwapchainSupportDestroy(MageRendererSurfaceSwapchainSupport *info);
extern VkExtent2D MageVulkanRendererSurfaceHandleExtent(MageRendererCreateInfo *info, MageRendererSurfaceSwapchainSupport *surfaceSupport, MageRenderer *renderer);

extern U8 MageVulkanRendererCreateSwapChain(MageRendererCreateInfo *info, MageRenderer *renderer);
extern U8 MageVulkanRendererGetSwapChainImages(MageRendererCreateInfo *info, MageRenderer *renderer);
extern U8 MageVulkanRendererCreateSwapChainImages(MageRendererCreateInfo *info, MageRenderer *renderer);

extern U8 MageVulkanRendererCreateGraphicsPipeline(MageRendererCreateInfo *info, MageRenderer *renderer);

extern U8 MageVulkanRendererCreateDebugLayers(MageRendererCreateInfo *info, MageRenderer *renderer);

extern U8  MageVulkanRendererValidateExtensionsPresent(const char *extensions[], const U32 count);
extern U8  MageVulkanRendererValidateLayersPresent(const char *layers[], const U32 count);

extern VKAPI_ATTR VkBool32 VKAPI_CALL MageVulkanValidationLayersCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, U0 *pUserData);

#endif
