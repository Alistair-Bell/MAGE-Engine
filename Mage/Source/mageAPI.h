#ifndef __MAGE_API__
#define __MAGE_API__

#include "mageCore.h"


/*!************************
	 __  __          _____ ______   ______             _            
	|  \/  |   /\   / ____|  ____| |  ____|           (_)           
	| \  / |  /  \ | |  __| |__    | |__   _ __   __ _ _ _ __   ___ 
	| |\/| | / /\ \| | |_ |  __|   |  __| | '_ \ / _` | | '_ \ / _ \
	| |  | |/ ____ \ |__| | |____  | |____| | | | (_| | | | | |  __/
	|_|  |_/_/    \_\_____|______| |______|_| |_|\__, |_|_| |_|\___|
												__/ |             
											    |___/              
	
	Open source 2D game engine written in with low memory footprint and performance in mind
	The engine is not the next unity or unreal. Just a fun tool to mess around with
	Documentation has been moved to the github wiki so use that for the documentation or ask me personally
	Documentation will be released once version 1.0 is released
	To contribute go to https://github.com/MTECGamesStudio/MAGE-Engine
	For use please read the license

**************************/

#if defined (MAGE_VULKAN)

#define MAGE_CHECK_VULKAN(function) \
	mageHandleVulkanResult(#function, function)

#endif


	
struct mageApplication;
struct mageApplicationProps;
struct mageRendererProps;
struct mageWindow;
struct mageRenderer;
struct mageIndiciesIndexes;
struct mageSwapChainSupportDetails;
struct mageVertex;

typedef enum MAGE_RESULT_ENUM 					mageResult;
typedef enum MAGE_EVENT_ENUM 					mageEventType;
typedef enum MAGE_EVENT_CATEGORY_BITS_ENUM 		mageEventCategoryBit;
typedef enum MAGE_EVENT_REQUIRED_BYTE_SIZE_ENUM mageEventRequiredByteSize;
typedef enum MAGE_SHADER_TYPE_ENUM				mageShaderType;
typedef enum MAGE_KEYCODE_ENUM					mageKeycode;

typedef mageResult  (*mageApplicationStartCallback)		(struct mageApplication *);
typedef void 		(*mageApplicationUpdateCallback)	(struct mageApplication *);
typedef mageResult  (*mageApplicationDestroyCallback)	(struct mageApplication *);
typedef void 		(*mageEventListenerCallback)		(void *, mageEventType);


enum MAGE_RESULT_ENUM 
{
	MAGE_UNKNOWN 								= -1,
	MAGE_SUCCESS,
	MAGE_LIBRARY_FAILURE,
	MAGE_INVALID_INPUT,
	MAGE_HARDWARE_INVALID,
	MAGE_VULKAN_NOT_READY,
	MAGE_CONTEXT_CREATION_FAILED,
	MAGE_DEBUG_MESSENGER_FAILED,
	MAGE_INSTANCE_CREATION_FAILURE,
	MAGE_DEVICE_CREATION_FAILURE,
	MAGE_SURFACE_CREATION_FAILURE,
	MAGE_FENCE_CREATION_FAILURE,
	MAGE_IMAGE_CREATION_FAILURE,
	MAGE_IMAGE_VIEW_CREATION_FAILURE,
	MAGE_SEMAPHORE_CREATION_FAILURE,
	MAGE_COMMAND_POOL_CREATION_FAILURE,
	MAGE_ALLOCATE_COMMAND_FAILURE,
	MAGE_SWAPCHAIN_CREATION_FAILED,
	MAGE_RENDER_PASS_CREATION_FAILURE,
	MAGE_PIPELINE_CREATION_FAILURE,
	MAGE_FRAME_BUFFER_CREATION_FAILED,
	MAGE_GRAPHICS_PIPELINE_CREATION_FAILURE,
	MAGE_QUEUE_SUBMITION_FAILURE,
	MAGE_HARDWARE_NOT_PRESENT,
	MAGE_SHADER_CREATION_FAILURE,
	MAGE_START_METHOD_FAILURE,
	MAGE_UPDATE_FAILURE,
	MAGE_DESTROY_METHOD_FAILURE,
};
enum MAGE_EVENT_ENUM
{
    MAGE_NONE_EVENT                     		= 0,
    MAGE_WINDOW_CLOSE_EVENT             		= 1,
    MAGE_WINDOW_FOCUS_EVENT             		= 2,
    MAGE_WINDOW_LOST_FOCUS_EVENT        		= 3,
    MAGE_WINDOW_MOVED_EVENT             		= 4,
    MAGE_APPLICATION_TICK_EVENT         		= 5,
    MAGE_APPLICATION_UPDATE_EVENT       		= 6,
    MAGE_APPLICATION_RENDER_EVENT       		= 7,
    MAGE_KEY_PRESSED_EVENT              		= 8,
    MAGE_KEY_RELEASED_EVENT             		= 9,
    MAGE_KEY_REPEAT_EVENT               		= 10,
    MAGE_MOUSE_BUTTON_PRESSED_EVENT     		= 11,
    MAGE_MOUSE_BUTTON_RELEASED_EVENT    		= 12,
    MAGE_MOUSE_MOVED_EVENT              		= 13,
    MAGE_MOUSE_SCROLLED_EVENT           		= 14,
};
enum MAGE_EVENT_REQUIRED_BYTE_SIZE_ENUM
{
	MAGE_NONE_EVENT_BYTE_SIZE 					= 0,
	MAGE_WINDOW_CLOSE_EVENT_BYTE_SIZE 			= sizeof(int16_t),
	MAGE_WINDOW_FOCUS_EVENT_BYTE_SIZE 			= sizeof(uint16_t),
	MAGE_WINDOW_LOST_FOCUS_EVENT_BYTE_SIZE		= sizeof(uint16_t),
	MAGE_WINDOW_MOVED_EVENT_BYTE_SIZE 			= sizeof(uint16_t) + (sizeof(uint32_t) * 2),
	MAGE_APPLICATION_TICK_EVENT_BYTE_SIZE 		= sizeof(uint16_t) + sizeof(int32_t),
	MAGE_APPLICATION_UPDATE_EVENT_BYTE_SIZE 	= sizeof(uint16_t),
	MAGE_APPLICATION_RENDER_EVENT_BYTE_SIZE 	= sizeof(uint16_t),
	MAGE_KEY_PRESSED_EVENT_BYTE_SIZE 			= sizeof(uint16_t) + sizeof(uint8_t),
	MAGE_KEY_REPEAT_EVENT_BYTE_SIZE				= sizeof(uint16_t) + sizeof(uint8_t),
	MAGE_KEY_RELEASED_EVENT_BYTE_SIZE 			= sizeof(uint16_t) + sizeof(uint8_t),
	MAGE_MOUSE_BUTTON_PRESSED_EVENT_BYTE_SIZE 	= sizeof(uint16_t) + sizeof(uint8_t),
	MAGE_MOUSE_BUTTON_RELEASED_EVENT_BYTE_SIZE 	= sizeof(uint16_t) + sizeof(uint8_t),
	MAGE_MOUSE_MOVED_EVENT_BYTE_SIZE 			= sizeof(uint16_t) + (sizeof(double) * 2),
	MAGE_MOUSE_SCROLLED_EVENT_BYTE_SIZE 		= sizeof(uint16_t) + (sizeof(double) * 2),
};
enum MAGE_EVENT_CATEGORY_BITS_ENUM
{
	MAGE_NONE_CATEGORY 							= -1,
	MAGE_APPLICATION_CATEGORY					= 4,
	MAGE_INPUT_CATEGORY							= 5,
	MAGE_KEYBOARD_CATEGORY 						= 6,
	MAGE_MOUSE_CATEGORY							= 7,
	MAGE_MOUSE_BUTTON_CATEGORY					= 8,
};
enum MAGE_SHADER_TYPE_ENUM
{
	MAGE_VERTEX_SHADER							= 1,
	MAGE_TESSELLATION_CONTROL_SHADER			= 2,
	MAGE_TESSELLATION_EVALUATION_SHADER			= 3,
	MAGE_GEOMETRY_SHADER						= 4,
	MAGE_FRAGMENT_SHADER						= 5,
	MAGE_COMPUTE_SHADER							= 6,
};


#if defined (MAGE_VULKAN)

struct mageSwapChainSupportDetails
{
#if defined (MAGE_VULKAN)
	VkSurfaceCapabilitiesKHR 				Capabilities;
	VkSurfaceFormatKHR 						*Formats;
	VkPresentModeKHR						*PresentModes;
	VkExtent2D								Extent;
#endif
	uint32_t								FormatCount;
	uint32_t								PresentCount;
};

#endif

struct mageWindow
{
	int32_t 								Width;
	int32_t 								Height;
	uint32_t 								Running;	
	const char 							   *Title;
	GLFWwindow 							   	*Context;
};
struct mageIndiciesIndexes
{
	uint32_t								*GraphicIndexes;
	uint32_t								*PresentIndexes;
	uint32_t								GraphicIndexesCount;
	uint32_t								PresentIndexesCount;
};
struct mageRenderer
{
#if defined (MAGE_VULKAN)
	
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

#endif
};
struct mageApplicationProps
{
	double 									Version;
	uint32_t 								Width;
	uint32_t 								Height;
	uint8_t									Fullscreen;
	uint8_t									FixedResolution;
	mageApplicationStartCallback 			StartMethod;
	mageApplicationUpdateCallback 			UpdateMethod;
	mageApplicationDestroyCallback 			DestroyMethod;
	mageEventListenerCallback				*Listeners;
	uint32_t 								ListenerCount;
	char 						   			*Name;
	const char 								*WindowIcon;
};
struct mageRendererProps
{
	struct mageShader						*RuntimeShaders;
	uint32_t 								ShaderCount;
};
struct mageApplication
{
	struct mageRenderer 					*Renderer;
	struct mageWindow 						*Window;
	struct mageApplicationProps 			Props;
	struct mageRendererProps				RendererProps;
	uint8_t 								Running;
};
struct mageShader
{
	mageShaderType 							ShaderType;
	const char 								*FilePath;
	const char 								*RuntimeFunctionName;
};
struct mageVertex
{
	struct vector2 							Vertex;
	struct vector3							Color;
};
struct mageBuffer
{
#if defined (MAGE_VULKAN)
	VkBuffer								Buffer;
	VkDeviceMemory							AllocatedMemory;
#endif
};
struct mageVertexBuffer
{
	struct mageBuffer						MemoryBuffer;
	struct mageVertex						*Vertexes;
	uint32_t								Count;
};
struct mageIndexBuffer
{
	struct mageBuffer						MemoryBuffer;
	uint16_t								*Indexes;
	uint16_t								Count;
};



extern mageResult mageEngineInitialise(

);
extern const char *mageToString(
	mageResult result
);
extern void mageLogMessage(
	const uint8_t user, 
	const uint8_t severity, 
	const char *format, 
	...
);
extern void mageLogEnd(

);
extern void mageLogInitialise(
	const char *outputFile
);
extern char *mageFileReadContents(
	const char *file, 
	const char *readmode, 
	uint32_t *fileSize
);
extern mageResult mageFileDumpContents(
	const char *file, 
	const char *buffer, 
	const uint8_t clean
);
extern mageResult mageWindowInitialise(
	struct mageWindow *window, 
	struct mageApplicationProps *props
);
extern void mageWindowTerminate(
	struct mageWindow *window
);
extern void mageInputSetup(
	struct mageApplication *application
);
extern void mageEventSetupMaster(

);
extern mageEventCategoryBit *mageEventGenerateCategories(
	const mageEventType type
);
extern uint16_t mageEventHandleCreate(
	const mageEventType type
);
extern mageEventType mageEventExtractEventType(
	const uint16_t handle
);
extern uint8_t mageEventInCategory(
	const uint16_t handle, 
	const mageEventCategoryBit category
);
extern void mageEventRegisterListener(
	mageEventListenerCallback callback
);
extern void mageEventFormatWindowClose(
	void *buffer
);
extern void mageEventFormatWindowFocus(
	void *buffer
);
extern void mageEventFormatWindowLostFocus(
	void *buffer
);
extern void mageEventFormatWindowMoved(
	void *buffer, 
	const int32_t x, 
	const int32_t y
);
extern void mageEventFormatKeyPressed(
	void *buffer, 
	const uint8_t keycode
);
extern void mageEventFormatKeyReleased(
	void *buffer, 
	const uint8_t keycode
);
extern void mageEventFormatKeyRepeat(
	void *buffer, 
	const uint8_t keycode
);
extern void mageEventFormatMouseButtonPressed(
	void *buffer, 
	const uint8_t mousecode
);
extern void mageEventFormatMouseButtonRelease(
	void *buffer, 
	const uint8_t mousecode
);
extern void mageEventFormatMouseMoved(
	void *buffer, 
	const double x, 
	const double y
);
extern void mageEventFormatMouseWheelMoved(
	void *buffer, 
	const double x, 
	const double y
);
extern void mageEventDispatch(
	void *event
);
extern mageResult mageRendererInitialise(
	struct mageRenderer *renderer, 
	struct mageWindow *window, 
	struct mageRendererProps *props
);	
extern void mageRendererResize(
	struct mageRenderer *renderer, 
	struct mageWindow *window,
	struct mageRendererProps *rendererProps
);
extern void mageRendererRender(
	struct mageRenderer *renderer
);
extern void mageRendererClear(
	struct mageRenderer *renderer
);
extern void mageRendererDestroy(
	struct mageRenderer *renderer
);

extern mageResult mageShaderInitialise(
	struct mageShader *shader, 
	const char *shaderFile, 
	const char *runtimeFunctionName, 
	const mageShaderType shaderType
);
extern mageShaderType mageShaderTypeFromString(
	const char *name
);
extern void mageVertexBufferCreate(
	struct mageVertexBuffer *buffer,
	struct mageVertex *vertexes,
	uint32_t vertexCount,
	struct mageRenderer *renderer
);
extern void mageVertexBufferDestroy(
	struct mageVertexBuffer *buffer,
	struct mageRenderer *renderer
);
extern void mageIndexBufferCreate(
	struct mageIndexBuffer *buffer,
	uint16_t *indexes,
	uint32_t indexCount,
	struct mageRenderer *renderer
);
extern void mageIndexBufferDestroy(
	struct mageIndexBuffer *buffer,
	struct mageRenderer *renderer
);


#if defined (MAGE_VULKAN)

extern void mageVertexInitialise(
	struct mageVertex *vertexInstance, 
	struct vector2 vertex, 
	struct vector3 color
);
extern void mageBufferAllocate(
	struct mageBuffer *buffer,
	void *data,
	uint32_t dataSize,
	const VkBufferUsageFlags bufferUsage,
	const VkBufferUsageFlags flags,
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

extern mageResult mageApplicationInitialise(
	struct mageApplication *application, 
	struct mageApplicationProps engineProps, 
	struct mageRendererProps rendererProps
);
extern mageResult mageApplicationRun(
	struct mageApplication *application
);
extern void mageApplicationDestroy(
	struct mageApplication *application
);



#endif  


