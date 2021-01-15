#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include "../Includes.h"
#include "../CreateStructures.h"
#include "../Window.h"
#include "../Maths.h"
#include "../FileSystem.h"

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
    VkRenderPass            RenderPass;
} MageRendererPipeline;

typedef struct MageRendererFrameBuffer
{
    VkFramebuffer *Buffers;
    U32            Count;
} MageRendererFrameBuffer;

typedef struct MageRendererCommandBuffers
{
    VkCommandPool   Pool;
    U32             ResidentCount; 
    VkCommandBuffer *Residents;

} MageRendererCommandBuffers;

typedef struct MageRendererSyncronisation
{
    VkSemaphore        *AvailableSemaphores;
    VkSemaphore        *FinishedSemaphores;
    VkFence            *ActiveFences;
    VkFence            *ImageFences;
    U32                ThreadCount;

} MageRendererSyncronisation;

typedef U32 MageVulkanMemoryBlockSections;

typedef struct MageVulkanMemoryBlock
{
    MageVulkanMemoryBlockSections *Sections;
    VkDeviceMemory                AssociatedMemory;
    U32                           MaxSectionCount;
    U32                           ActiveSectionCount; 
    U8                            Allocated;
} MageVulkanMemoryBlock;

typedef struct MageVulkanMemoryHeap
{
    VkMemoryPropertyFlagBits Flags;
    U32                      HeapIndex;
    U32                      BlockCount;
    MageVulkanMemoryBlock    *Blocks;
} MageVulkanMemoryHeap;

typedef struct MageVulkanMemoryRequestInfo
{
    MageGraphicsBufferCreateInfo  *BufferInfo;
    VkMemoryPropertyFlagBits      DesiredFlagBits;
} MageVulkanMemoryRequestInfo; 

typedef struct MageRenderer
{
    MageRendererOverseer        Overseer;
    MageRendererSurface         Surface;
    MageRendererPhysicalDevice  Device;
    MageRendererQueues          QueueHandles;
    MageRendererSwapChain       SwapChain;
    MageRendererFrameBuffer     FrameBuffer;
    MageRendererPipeline        Pipeline;
    MageRendererCommandBuffers  CommandRecorders;
    MageRendererSyncronisation  Syncronisation;
    MageVulkanMemoryHeap        *Heaps;
    MageVulkanMemoryBlock       *NextBlock;
    VkClearValue                ClearValues;
    U32                         HeapCount;
    U32                         NextBlockIndex;
    U32                         ActiveIndex;
} MageRenderer;

typedef enum MageShaderType
{
    MAGE_SHADER_TYPE_VERTEX,
    MAGE_SHADER_TYPE_FRAGMENT,
} MageShaderType;

typedef struct MageShaderCreateInfo
{
   MageFileSystem                      MountedFileSystem;
   MageFileSystemReadInfo              ReadInfo;
   MageShaderType                      Type;
   const char                          *EntryPoint;
} MageShaderCreateInfo;

typedef struct MageShader
{
    VkShaderModule   Module;
    MageShaderType   Type;
    const char       *EntryPoint;
} MageShader;

extern U8 MageVulkanRendererInstanceCreate(MageRendererCreateInfo *info, MageRenderer *renderer);

extern U8  MageVulkanRendererPhysicalDeviceCreate(MageRendererCreateInfo *info, MageRenderer *renderer);
extern U64 MageVulkanRendererRatePhysicalDevice(MageRendererCreateInfo *info, MageRenderer *renderer, VkPhysicalDevice device);
extern U8  MageVulkanRendererFindPhysicalDeviceQueueFamiles(MageRenderer *renderer, VkPhysicalDevice device, MageRendererPhysicalDeviceQueueFamilies *families);
extern U8  MageVulkanRendererFindPhysicalDeviceExtensions(VkPhysicalDevice device, const char *deviceExtensions[], const U32 count);

extern U8 MageVulkanRendererSurfaceCreate(MageRendererCreateInfo *info,  MageRenderer *renderer);
extern U8 MageVulkanRendererSurfaceFindSwapchainSupport(MageRenderer *renderer, MageRendererSurfaceSwapchainSupport *info);
extern U8 MageVulkanRendererSurfacePickCorrectFormats(MageRenderer *renderer, MageRendererSurfaceSwapchainSupport *info, VkSurfaceFormatKHR *format, VkPresentModeKHR *present);
extern U8 MageVulkanRendererSurfaceSwapchainSupportDestroy(MageRendererSurfaceSwapchainSupport *info);
extern VkExtent2D MageVulkanRendererSurfaceHandleExtent(MageRendererCreateInfo *info, MageRendererSurfaceSwapchainSupport *surfaceSupport, MageRenderer *renderer);

extern U8 MageVulkanRendererSwapChainCreate(MageRendererCreateInfo *info, MageRenderer *renderer);
extern U8 MageVulkanRendererGetSwapChainImages(MageRendererCreateInfo *info, MageRenderer *renderer);
extern U8 MageVulkanRendererSwapChainImagesCreate(MageRendererCreateInfo *info, MageRenderer *renderer);

extern U8 MageShaderCreate(MageShaderCreateInfo *info, MageShader *shader, MageRenderer *renderer);
extern VkShaderStageFlagBits MageVulkanShaderAbstractToNativeType(const MageShaderType type);
extern VkPipelineShaderStageCreateInfo MageVulkanShaderCreatePipelineStage(MageShaderCreateInfo *info, MageShader *shader);
extern U8 MageShaderDestroy(MageShader *shader, MageRenderer *renderer);

extern U8 MageVulkanRendererGraphicsPipelineCreate(MageRendererCreateInfo *info, MageRenderer *renderer);
extern U8 MageVulkanRendererRenderPassCreate(MageRendererCreateInfo *info, MageRenderer *renderer);

extern U8 MageVulkanRendererFrameBuffersCreate(MageRendererCreateInfo *info, MageRenderer *renderer);

extern U8 MageVulkanRendererCommandBuffersCreate(MageRendererCreateInfo *info, MageRenderer *renderer);

extern U8 MageVulkanRendererHeapsCreate(MageRendererCreateInfo *info, MageRenderer *renderer);
extern U8 MageVulkanRendererHeapsRequestMemory(MageRenderer *renderer, MageVulkanMemoryRequestInfo *requestInfo, VkDeviceMemory *sectionMemory, MageVulkanMemoryBlockSections *section);
extern U8 MageVulkanRendererHeapsDestroy(MageRenderer *renderer);

extern U8 MageVulkanRendererSyncronisationCreate(MageRendererCreateInfo *info, MageRenderer *renderer);
extern U8 MageVulkanRendererCreateDebugLayers(MageRendererCreateInfo *info, MageRenderer *renderer);
extern U8 MageVulkanRendererValidateExtensionsPresent(const char *extensions[], const U32 count);
extern U8 MageVulkanRendererValidateLayersPresent(const char *layers[], const U32 count);

extern VKAPI_ATTR VkBool32 VKAPI_CALL MageVulkanValidationLayersCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, U0 *pUserData);

#endif
