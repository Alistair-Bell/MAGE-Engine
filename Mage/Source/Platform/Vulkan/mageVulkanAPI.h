#ifndef __MAGE_VULKAN_API__
#define __MAGE_VULKAN_API__

#include <mageAPI.h>

#if defined (MAGE_VULKAN)

struct mageSwapChainSupportDetails
{
	mageStructureType						StructureType;
	VkSurfaceCapabilitiesKHR 				Capabilities;
	VkSurfaceFormatKHR 						*Formats;
	VkPresentModeKHR						*PresentModes;
	VkExtent2D								Extent;
	uint32_t								FormatCount;
	uint32_t								PresentCount;
};
struct mageBufferWrapper
{
	mageStructureType						StructureType;
	VkBuffer								Buffer;
	VkDeviceMemory							AllocatedMemory;
};
struct mageBuffer
{
	mageStructureType 						StructureType;
	struct mageBufferWrapper				Wrapper;
	void									*Data;
	uint32_t								Bytes;
};	

struct mageRenderer
{
    mageStructureType						StructureType;
	
	VkInstance 								Instance;
	VkDevice								Device;
	VkPhysicalDeviceMemoryProperties		PhysicalDeviceMemoryProperties;
	
	VkPhysicalDevice						PhysicalDevice;
	VkSurfaceKHR 							Surface;
	VkQueue 								PresentQueue;
	VkQueue									GraphicalQueue;
	
	VkSwapchainKHR							SwapChain;
	VkImage									*SwapChainImages;
	VkImageView								*SwapChainImageViews;

	VkExtent2D								SwapChainExtent;
	VkFormat								SwapChainFormat;

	VkPipelineLayout 						GraphicsPipelineLayout;
	VkPipeline								GraphicsPipeline;

	VkRenderPass							PrimaryRenderPass;
	VkFramebuffer							*Framebuffers;
	VkCommandPool							CommandPool;
	
	VkCommandBuffer							*CommandBuffers;

	VkSemaphore								*ImageAvailableSemaphores;
	VkSemaphore								*RenderFinishedSemaphores;
	VkFence									*ConcurentFences;
	VkFence									*ConcurrentImages;


	VkDebugUtilsMessengerCreateInfoEXT		DebugMessengerCreateInfo;
	VkDebugUtilsMessengerEXT				DebugMessenger;
	struct mageIndiciesIndexes				Indexes;
	struct mageSwapChainSupportDetails		SwapChainSupportInfo;

	uint32_t 								SwapChainImageCount;
	uint32_t								ConcurentFrames;
	uint32_t								CurrentFrame;
};

#define MAGE_CHECK_VULKAN(function) \
	mageHandleVulkanResult(#function, function)

extern VkBufferUsageFlagBits mageBufferTypeToFlagBits(
	const mageBufferType type
);

extern void mageVertexInitialise(
	struct mageVertex *vertexInstance, 
	struct vector2 vertex, 
	struct vector3 color
);
extern void mageBufferWrapperAllocate(
	struct mageBufferWrapper *buffer,
	void *data,
	uint32_t dataSize,
	const VkBufferUsageFlags bufferUsage,
	const VkBufferUsageFlags flags,
	struct mageRenderer *renderer
);
extern void mageBufferWrapperDestroy(
	struct mageBufferWrapper *buffer,
	struct mageRenderer *renderer	
);
extern void mageBufferCreate(
	struct mageBuffer *buffer,
	mageBufferType bufferType,
	void *data,
	uint32_t dataByteSize,
	struct mageRenderer *renderer
);
extern void mageBufferDestroy(
	struct mageBuffer *buffer,
	struct mageRenderer *renderer
);


extern VkResult mageHandleVulkanResult(
	const char *functionName,
	VkResult functionResult
);
extern uint32_t mageFindMemoryType(
	uint32_t typeFilter, 
	VkMemoryPropertyFlags properties,
	struct mageRenderer *renderer
);
extern VkShaderStageFlagBits mageShaderTypeToBit(
	const mageShaderType shaderType
);
extern VkFramebuffer mageRendererGetActiveFrameBuffer(
	struct mageRenderer *renderer
);
extern VkShaderModule mageShaderCreateModule(
	struct mageShader *shader, 
	VkDevice device
);
extern mageResult mageGetDeviceIndexes(
	struct mageRenderer *renderer,
	VkPhysicalDevice device,
	struct mageIndiciesIndexes *indicies
);
extern void mageIndiciesIndexesInitialise(
	struct mageIndiciesIndexes *indicies, 
	const uint32_t *graphics, 
	const uint32_t graphicCount, 
	const uint32_t *presents, 
	const uint32_t presentCount
);
extern void mageIndiciesIndexesDestroy(
	struct mageIndiciesIndexes *indicies
);
extern void mageSwapChainSupportInitialise(
	struct mageSwapChainSupportDetails *swapChainSupport,
	const VkSurfaceCapabilitiesKHR surfaceCapabilities,
	VkSurfaceFormatKHR *formats,
	const uint32_t formatCount,
	VkPresentModeKHR *presentModes,
	const uint32_t presentCount,
	VkExtent2D extent
);
extern mageResult mageGetSwapChainSupport(
	struct mageSwapChainSupportDetails *swapChainSupport,
	struct mageWindow *window,
	VkPhysicalDevice physicalDevice,
	VkSurfaceKHR surface
);
extern VkPresentModeKHR mageSwapChainSupportPickPresentMode(
	struct mageSwapChainSupportDetails *swapChainSupport
);
extern VkSurfaceFormatKHR mageSwapChainSupportPickSurfaceFormat(
	struct mageSwapChainSupportDetails *swapChainDetails
);
extern void mageSwapChainSupportDestroy(
	struct mageSwapChainSupportDetails *swapChainSupport
);
extern VkVertexInputBindingDescription mageVertexBindingDescription(
);
extern VkVertexInputAttributeDescription *mageVertexGetAttributeDescriptions(
	uint32_t *count
);


#endif
#endif