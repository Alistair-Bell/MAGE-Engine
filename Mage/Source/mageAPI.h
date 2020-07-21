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
	
	This header contains the generic function, enums and structures that are used in the engine
	Structs that change the data stored based on the platform or api are foward declared and are implimented in the respected submodule

	Open source 2D game engine written with low memory footprint and performance in mind
	The engine is not the next unity or unreal. Just a fun tool to mess around with
	Documentation has been moved to the github wiki so use that for the documentation or ask me personally
	Documentation will be released once version 1.0 is released
	To contribute go to https://github.com/MTECGamesStudio/MAGE-Engine
	For use please read the license

**************************/

#define MAGE_SET_BIT(input, index, value) (input |= value << index)

#define MAGE_VULKAN_CHECK(function) \
	mageHandleVulkanResult(#function, function)

#define MAGE_PNG_LOADE_CHECK(function, data)\
	uint32_t error = function; \
	if (error) { MAGE_LOG_CORE_ERROR("PNG Error %d, %s was a bad file\n", error, lodepng_error_text(error)); free(data); }

#define MAGE_ECS_REGISTER_COMPONENT(scene, component) \
	mageEntityRegisterComponent(scene, #component, sizeof(component))
#define MAGE_ECS_BIND_COMPONENT(scene, entity, ...) \
	mageEntityBindComponents(scene, entity, __VA_ARGS__);

#define MAGE_BIT(index) (1 << index) 

typedef uint32_t mageEventHandle;
typedef uint64_t mageEntity;
typedef uint64_t mageComponentID;

typedef enum MAGE_LOG_MODE_ENUM
{
	MAGE_LOG_MODE_INFORM 		= 0,
	MAGE_LOG_MODE_WARNING 		= 1,
	MAGE_LOG_MODE_ERROR 		= 2,
	MAGE_LOG_MODE_FATAL_ERROR 	= 3,
} mageLogMode;

typedef enum MAGE_LOG_USER_ENUM
{
	MAGE_LOG_USER_CORE 		= 0,
	MAGE_LOG_USER_CLIENT 	= 1,
} mageLogUser;

typedef enum MAGE_RESULT_ENUM 
{
	MAGE_RESULT_UNKNOWN = -1,
	MAGE_RESULT_SUCCESS,
	MAGE_RESULT_LIBRARY_FAILURE,
	MAGE_RESULT_INVALID_INPUT,
	MAGE_RESULT_HARDWARE_INVALID,
	MAGE_RESULT_VULKAN_NOT_READY,
	MAGE_RESULT_CONTEXT_CREATION_FAILED,
	MAGE_RESULT_DEBUG_MESSENGER_FAILED,
	MAGE_RESULT_INSTANCE_CREATION_FAILURE,
	MAGE_RESULT_DEVICE_CREATION_FAILURE,
	MAGE_RESULT_SURFACE_CREATION_FAILURE,
	MAGE_RESULT_FENCE_CREATION_FAILURE,
	MAGE_RESULT_IMAGE_CREATION_FAILURE,
	MAGE_RESULT_IMAGE_VIEW_CREATION_FAILURE,
	MAGE_RESULT_SEMAPHORE_CREATION_FAILURE,
	MAGE_RESULT_COMMAND_POOL_CREATION_FAILURE,
	MAGE_RESULT_ALLOCATE_COMMAND_FAILURE,
	MAGE_RESULT_SWAPCHAIN_CREATION_FAILED,
	MAGE_RESULT_RENDER_PASS_CREATION_FAILURE,
	MAGE_RESULT_PIPELINE_CREATION_FAILURE,
	MAGE_RESULT_FRAME_BUFFER_CREATION_FAILED,
	MAGE_RESULT_GRAPHICS_PIPELINE_CREATION_FAILURE,
	MAGE_RESULT_QUEUE_SUBMITION_FAILURE,
	MAGE_RESULT_HARDWARE_NOT_PRESENT,
	MAGE_RESULT_SHADER_CREATION_FAILURE,
	MAGE_RESULT_START_METHOD_FAILURE,
	MAGE_RESULT_UPDATE_FAILURE,
	MAGE_RESULT_DESTROY_METHOD_FAILURE,
} mageResult;

typedef enum MAGE_KEYCODE_ENUM
{
	MAGE_KEYCODE_SPACE = 0,
	MAGE_KEYCODE_APOSTROPHE,
	MAGE_KEYCODE_COMMA,
	MAGE_KEYCODE_MINUS,
	MAGE_KEYCODE_PERIOD,
	MAGE_KEYCODE_SLASH,
	MAGE_KEYCODE_D0,
	MAGE_KEYCODE_D1,
	MAGE_KEYCODE_D2,
	MAGE_KEYCODE_D3,
	MAGE_KEYCODE_D4,
	MAGE_KEYCODE_D5,
	MAGE_KEYCODE_D6,
	MAGE_KEYCODE_D7,
	MAGE_KEYCODE_D8,
	MAGE_KEYCODE_D9,
	MAGE_KEYCODE_SEMICOLON,
	MAGE_KEYCODE_EQUAL,
	MAGE_KEYCODE_A,
	MAGE_KEYCODE_B,
	MAGE_KEYCODE_C,
	MAGE_KEYCODE_D,
	MAGE_KEYCODE_E,
	MAGE_KEYCODE_F,
	MAGE_KEYCODE_G,
	MAGE_KEYCODE_H,
	MAGE_KEYCODE_I,
	MAGE_KEYCODE_J,
	MAGE_KEYCODE_K,
	MAGE_KEYCODE_L,
	MAGE_KEYCODE_M,
	MAGE_KEYCODE_N,
	MAGE_KEYCODE_O,
	MAGE_KEYCODE_P,
	MAGE_KEYCODE_Q,
	MAGE_KEYCODE_R,
	MAGE_KEYCODE_S,
	MAGE_KEYCODE_T,
	MAGE_KEYCODE_U,
	MAGE_KEYCODE_V,
	MAGE_KEYCODE_W,
	MAGE_KEYCODE_X,
	MAGE_KEYCODE_Y,
	MAGE_KEYCODE_Z,
	MAGE_KEYCODE_LEFTBRACKET,
	MAGE_KEYCODE_BACKSLASH,
	MAGE_KEYCODE_RIGHTBRACKET,
	MAGE_KEYCODE_GRAVEACCENT,
	MAGE_KEYCODE_ESCAPE,
	MAGE_KEYCODE_ENTER,
	MAGE_KEYCODE_TAB,
	MAGE_KEYCODE_BACKSPACE,
	MAGE_KEYCODE_INSERT,
	MAGE_KEYCODE_DELETE,
	MAGE_KEYCODE_RIGHT,
	MAGE_KEYCODE_LEFT,
	MAGE_KEYCODE_DOWN,
	MAGE_KEYCODE_UP,
	MAGE_KEYCODE_PAGEUP,
	MAGE_KEYCODE_PAGEDOWN,
	MAGE_KEYCODE_HOME,
	MAGE_KEYCODE_END,
	MAGE_KEYCODE_CAPSLOCK,
	MAGE_KEYCODE_SCROLLLOCK,
	MAGE_KEYCODE_NUMLOCK,
	MAGE_KEYCODE_PRINTSCREEN,
	MAGE_KEYCODE_PAUSE,
	MAGE_KEYCODE_F1,
	MAGE_KEYCODE_F2,
	MAGE_KEYCODE_F3,
	MAGE_KEYCODE_F4,
	MAGE_KEYCODE_F5,
	MAGE_KEYCODE_F6,
	MAGE_KEYCODE_F7,
	MAGE_KEYCODE_F8,
	MAGE_KEYCODE_F9,
	MAGE_KEYCODE_F10,
	MAGE_KEYCODE_F11,
	MAGE_KEYCODE_F12,
	MAGE_KEYCODE_F13,
	MAGE_KEYCODE_F14,
	MAGE_KEYCODE_F15,
	MAGE_KEYCODE_F16,
	MAGE_KEYCODE_F17,
	MAGE_KEYCODE_F18,
	MAGE_KEYCODE_F19,
	MAGE_KEYCODE_F20,
	MAGE_KEYCODE_F21,
	MAGE_KEYCODE_F22,
	MAGE_KEYCODE_F23,
	MAGE_KEYCODE_F24,
	MAGE_KEYCODE_KEYPAD_0,
	MAGE_KEYCODE_KEYPAD_1,
	MAGE_KEYCODE_KEYPAD_2,
	MAGE_KEYCODE_KEYPAD_3,
	MAGE_KEYCODE_KEYPAD_4,
	MAGE_KEYCODE_KEYPAD_5,
	MAGE_KEYCODE_KEYPAD_6,
	MAGE_KEYCODE_KEYPAD_7,
	MAGE_KEYCODE_KEYPAD_8,
	MAGE_KEYCODE_KEYPAD_9,
	MAGE_KEYCODE_KEYPAD_DECIMAL,
	MAGE_KEYCODE_KEYPAD_DIVIDE,
	MAGE_KEYCODE_KEYPAD_MULTIPLY,
	MAGE_KEYCODE_KEYPAD_SUBTRACT,
	MAGE_KEYCODE_KEYPAD_ADD,
	MAGE_KEYCODE_KEYPAD_ENTER,
	MAGE_KEYCODE_KEYPAD_EQUAL,
	MAGE_KEYCODE_LEFTSHIFT,
	MAGE_KEYCODE_LEFTCONTROL,
	MAGE_KEYCODE_LEFTALT,
	MAGE_KEYCODE_LEFTSUPER,
	MAGE_KEYCODE_RIGHTSHIFT,
	MAGE_KEYCODE_RIGHTCONTROL,
	MAGE_KEYCODE_RIGHTALT,
	MAGE_KEYCODE_RIGHTSUPER,
} mageKeyCode;

typedef enum MAGE_MOUSE_CODE_ENUM
{
	MAGE_MOUSECODE_BUTTON_1 = 0,
	MAGE_MOUSECODE_BUTTON_2,
	MAGE_MOUSECODE_BUTTON_3,
	MAGE_MOUSECODE_BUTTON_4,
	MAGE_MOUSECODE_BUTTON_5,
	MAGE_MOUSECODE_BUTTON_6,
	MAGE_MOUSECODE_BUTTON_7,
	MAGE_MOUSECODE_BUTTON_8,
} mageMouseCode;

typedef enum MAGE_EVENT_ENUM
{
    MAGE_EVENT_NONE                     		= 0,
    MAGE_EVENT_WINDOW_CLOSE             		= 1,
    MAGE_EVENT_WINDOW_FOCUS             		= 2,
    MAGE_EVENT_WINDOW_LOST_FOCUS        		= 3,
    MAGE_EVENT_WINDOW_MOVED             		= 4,
    MAGE_EVENT_APPLICATION_TICK         		= 5,
    MAGE_EVENT_APPLICATION_UPDATE       		= 6,
    MAGE_EVENT_APPLICATION_RENDER       		= 7,
    MAGE_EVENT_KEY_PRESSED              		= 8,
    MAGE_EVENT_KEY_RELEASED             		= 9,
    MAGE_EVENT_KEY_REPEAT               		= 10,
    MAGE_EVENT_MOUSE_BUTTON_PRESSED     		= 11,
    MAGE_EVENT_MOUSE_BUTTON_RELEASED    		= 12,
    MAGE_EVENT_MOUSE_MOVED              		= 13,
    MAGE_EVENT_MOUSE_SCROLLED           		= 14,
} mageEventType;

typedef enum MAGE_EVENT_REQUIRED_BYTE_SIZE_ENUM
{
	MAGE_EVENT_BYTE_SIZE_NONE 					= 0,
	MAGE_EVENT_BYTE_SIZE_WINDOW_CLOSE 			= sizeof(mageEventHandle),
	MAGE_EVENT_BYTE_SIZE_WINDOW_FOCUS 			= sizeof(mageEventHandle),
	MAGE_EVENT_BYTE_SIZE_WINDOW_LOST_FOCUS		= sizeof(mageEventHandle),
	MAGE_EVENT_BYTE_SIZE_WINDOW_MOVED 			= sizeof(mageEventHandle) + (sizeof(uint32_t) * 2),
	MAGE_EVENT_BYTE_SIZE_APPLICATION_TICK 		= sizeof(mageEventHandle) + sizeof(int32_t),
	MAGE_EVENT_BYTE_SIZE_APPLICATION_UPDATE 	= sizeof(mageEventHandle),
	MAGE_EVENT_BYTE_SIZE_APPLICATION_RENDER 	= sizeof(mageEventHandle),
	MAGE_EVENT_BYTE_SIZE_KEY_PRESSED 			= sizeof(mageEventHandle) + sizeof(mageKeyCode),
	MAGE_EVENT_BYTE_SIZE_KEY_REPEAT				= sizeof(mageEventHandle) + sizeof(mageKeyCode),
	MAGE_EVENT_BYTE_SIZE_KEY_RELEASED 			= sizeof(mageEventHandle) + sizeof(mageKeyCode),
	MAGE_EVENT_BYTE_SIZE_MOUSE_BUTTON_PRESSED 	= sizeof(mageEventHandle) + sizeof(mageMouseCode),
	MAGE_EVENT_BYTE_SIZE_MOUSE_BUTTON_RELEASED 	= sizeof(mageEventHandle) + sizeof(mageMouseCode),
	MAGE_EVENT_BYTE_SIZE_MOUSE_MOVED 			= sizeof(mageEventHandle) + (sizeof(double) * 2),
	MAGE_EVENT_BYTE_SIZE_MOUSE_SCROLLED 		= sizeof(mageEventHandle) + (sizeof(double) * 2),
} mageEventRequiredByteSize;

typedef enum MAGE_EVENT_CATEGORY_BITS_ENUM
{
	MAGE_EVENT_CATEGORY_NONE 							= -1,
	MAGE_EVENT_CATEGORY_APPLICATION						= 4,
	MAGE_EVENT_CATEGORY_INPUT							= 5,
	MAGE_EVENT_CATEGORY_KEYBOARD 						= 6,
	MAGE_EVENT_CATEGORY_MOUSE							= 7,
	MAGE_EVENT_CATEGORY_MOUSE_BUTTON					= 8,
} mageEventCategoryBit;

typedef enum MAGE_SHADER_TYPE_ENUM
{
	MAGE_SHADER_TYPE_VERTEX								= 1,
	MAGE_SHADER_TYPE_TESSELLATION_CONTROL				= 2,
	MAGE_SHADER_TYPE_TESSELLATION_EVALUATION			= 3,
	MAGE_SHADER_TYPE_GEOMETRY							= 4,
	MAGE_SHADER_TYPE_FRAGMENT							= 5,
	MAGE_SHADER_TYPE_COMPUTE							= 6,
} mageShaderType;

typedef enum MAGE_RENDERABLE_PIPELINE_MODE_ENUM
{
	MAGE_RENDERABLE_PIPELINE_MODE_PRIMARY,
	/* Todo: allow for custom rendering pipelines */
} mageRenderablePipeLineMode;

typedef enum MAGE_BUFFER_TYPE_ENUM
{
	MAGE_BUFFER_TYPE_VERTEX,
	MAGE_BUFFER_TYPE_INDEX,
	MAGE_BUFFER_TYPE_SOURCE,
	MAGE_BUFFER_TYPE_TRANSFER,
} mageBufferType;

typedef enum MAGE_TEXTURE_SAMPLER_MODE_ENUM
{
	MAGE_TEXTURE_SAMPLER_REPEAT,
	MAGE_TEXTURE_SAMPLER_MIRRORED_REPEAT,
	MAGE_TEXTURE_SAMPLER_CLAMP_TO_EDGE,
	MAGE_TEXTURE_SAMPLER_CLAMP_TO_BORDER
} mageTextureSamplerMode;

typedef enum MAGE_TEXTURE_FILE_FORMAT_ENUM
{
	MAGE_TEXTURE_FILE_FORMAT_JPEG,
	MAGE_TEXTURE_FILE_FORMAT_PNG,
} mageTextureFileFormat;

typedef void (*mageEventListenerCallback)(void *, mageEventType);
typedef void *(*mageMemoryAllocaterMethod)(uint64_t);
typedef void (*mageMemoryFreeMethod)(void *);
typedef void *(*mageMemoryListAllocaterMethod)(uint64_t, uint64_t);
typedef void *(*mageMemoryReallocater)(void *, uint64_t);

struct mageQueue
{
	uint32_t Count;
    uint32_t DataSize;
	void *Data;
};
struct mageHeapAllocater
{
	mageMemoryAllocaterMethod 				Allocate;
	mageMemoryFreeMethod 					Free;
	mageMemoryListAllocaterMethod 			ListAllocater;
	mageMemoryReallocater 					Reallocater;
};
struct mageComponentTable
{
    const char 								*Tag;
    uint64_t 								Count;
    uint64_t 								ByteSize;
    uint64_t 								ComponentMask;
    void 									**Components;
};
struct mageEntityPool
{
    mageEntity 								*Pooled;
	struct mageQueue						AvailableQueue;
	struct mageComponentTable 				*ComponentTables;
    uint64_t 								EntityPooledCount;
	uint64_t								ComponentTableCount;
};
struct mageScene
{
    struct mageEntityPool 					Pool;
	struct mageHeapAllocater 				Allocater;
	const char *Tag;
};
struct mageIndiciesIndexes
{
	uint32_t								*GraphicIndexes;
	uint32_t								*PresentIndexes;
	uint32_t								GraphicIndexesCount;
	uint32_t								PresentIndexesCount;
};

struct mageShader
{
	mageShaderType 							ShaderType;
	const char 								*FilePath;
	const char 								*EntryPoint;
};
struct mageVertex
{
	struct vector2 							Vertex;
	struct vector3							Color;
};
struct mageTransform
{
	struct vector3 							Position;
	struct vector3							Scale;
	struct vector3							Rotation;
};
struct mageSwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR 				Capabilities;
	VkSurfaceFormatKHR 						*Formats;
	VkPresentModeKHR						*PresentModes;
	VkExtent2D								Extent;
	uint32_t								FormatCount;
	uint32_t								PresentCount;
};
struct mageBufferWrapper
{
	VkBuffer								Buffer;
	VkDeviceMemory							AllocatedMemory;
};
struct mageBuffer
{
	mageBufferType							BufferType;
	struct mageBufferWrapper				Wrapper;
	void									*Data;
	uint32_t								Bytes;
};
struct mageTexture
{
	VkSampler 								Sampler;
	VkImage									Image;
	VkImageView								View;
	VkImageLayout 							ImageLayout;
	VkDeviceMemory 							DeviceMemory;
	uint32_t								Width;
	uint32_t								Height;
};
struct mageRenderable
{
	struct mageBuffer						IndexBuffer;
	struct mageBuffer						VertexBuffer;
	struct mageTexture						Texture;
};

struct mageWindow
{
	int32_t 								Width;
	int32_t 								Height;
	uint32_t 								Running;	
	const char 							   	*Title;
	GLFWwindow 							   	*Context;
};
struct mageApplicationCreateInfo
{
	uint32_t 								Width;
	uint32_t 								Height;
	uint8_t									Fullscreen;
	uint8_t									FixedResolution;
	char 						   			*Name;
	const char 								*WindowIcon;
};
struct mageRendererCreateInfo
{
	struct mageShader						*PipelineShaders;
	uint32_t 								ShaderCount;
};
struct mageRenderer
{
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

	VkClearValue							ClearValue;
	VkRect2D								RenderArea;
	
	VkCommandBuffer							*CommandBuffers;

	VkSemaphore								*WaitSemaphores;
	VkSemaphore								*SignalSemaphores;

	VkFence									*FencesInUse;
	VkFence									*SwapChainImagesInUse;


	VkDebugUtilsMessengerCreateInfoEXT		DebugMessengerCreateInfo;
	VkDebugUtilsMessengerEXT				DebugMessenger;
	struct mageIndiciesIndexes				Indexes;
	struct mageSwapChainSupportDetails		SwapChainSupportInfo;

	uint32_t 								SwapChainImageCount;
	uint32_t								ConcurentFrames;
	uint32_t								CurrentFrame;
};
struct mageApplication
{
	struct mageRenderer 					*Renderer;
	struct mageWindow 						*Window;
	struct mageApplicationCreateInfo		CreateInfo;
	struct mageRendererCreateInfo			RendererCreateInfo;
	uint8_t 								Running;
};

/* Allocater */
struct mageHeapAllocater mageHeapAllocaterDefault(
);

/* Defined data structures */
extern void mageQueueCreate(
	struct mageQueue *queue, 
	const uint32_t dataSize, 
	const struct mageHeapAllocater *allocater
);
extern void mageQueueCreateFromSet(
	struct mageQueue *queue,
	uint32_t elementCount,
	uint32_t elementSize,
	void *data,
	struct mageHeapAllocater *allocater 
);
extern void mageQueuePush(
	struct mageQueue *queue, 
	void *data, 
	const struct mageHeapAllocater *allocater
);
extern void mageQueuePop(
	struct mageQueue *queue,
	void *buffer,
	const struct mageHeapAllocater *allocater
);
extern void mageQueueDestroy(
	struct mageQueue *queue, 
	const struct mageHeapAllocater *allocater
);


/* Entity component system */
extern void mageSceneCreate(
	struct mageScene *scene,
	const char *sceneTag,
	const struct mageHeapAllocater *allocater
);
extern void mageSceneDisplayInformation(
	const struct mageScene *scene
);
extern mageEntity mageEntityCreate(
	struct mageScene *scene
);
extern void mageEntityBindComponents(
	struct mageScene *scene,
	mageEntity entity,
	...
);
extern void mageEntityDestroy(
	struct mageScene *scene,
	const mageEntity entity
);
extern void mageEntityRegisterComponent(
	struct mageScene *scene,
	const char *id, 
	const uint64_t size
);
extern void mageComponentTableFree(
	struct mageHeapAllocater *allocater, 
	struct mageComponentTable *table
);
extern void mageSceneDestroy(
	struct mageScene *scene
);

/* Generic */
extern mageResult mageEngineInitialise(

);
extern void mageLogMessage(
	const mageLogUser user, 
	const mageLogMode severity, 
	const char *format, 
	...
);
extern VkResult mageHandleVulkanResult(
	const char *functionName,
	VkResult functionResult
);
extern void mageLogEnd(

);
extern void mageLogInitialise(
	const char *outputFile
);

/* File IO */
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

/* Window */
extern mageResult mageWindowCreate(
	struct mageWindow *window, 
	struct mageApplicationCreateInfo *createInfo
);
extern void mageWindowDestroy(
	struct mageWindow *window
);

/* Event system */ 
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

/* Vertexes */
extern void mageVertexCreate(
	struct mageVertex *vertexInstance, 
	struct vector2 vertex, 
	struct vector3 color
);
extern VkVertexInputBindingDescription mageVertexBindingDescription(
);
extern VkVertexInputAttributeDescription *mageVertexGetAttributeDescriptions(
	uint32_t *count
);

/* Buffers */
extern VkBufferUsageFlags mageBufferTypeToFlag(
	const mageBufferType type
);
extern void mageBufferWrapperAllocate(
	struct mageBufferWrapper *buffer,
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
extern VkBuffer mageBufferGetNativeBuffer(
	struct mageBuffer *buffer
);
extern void mageBufferDestroy(
	struct mageBuffer *buffer,
	struct mageRenderer *renderer
);

/* Textures */ 
extern mageResult mageTextureCreate(
	struct mageTexture *texture,
	const char *texturePath,
	mageTextureSamplerMode samplerMode,
	mageTextureFileFormat format,
	struct mageRenderer *renderer
);
extern void mageTextureDestroy(
	struct mageTexture *texture,
	struct mageRenderer *renderer
);

/* Shaders */
extern mageResult mageShaderCreate(
	struct mageShader *shader, 
	const char *shaderFile, 
	const char *entryPoint, 
	const mageShaderType shaderType
);
extern mageShaderType mageShaderTypeFromString(
	const char *name
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

/* Swap chain support */
extern void mageSwapChainSupportCreate(
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

/* Graphics card indexes */
extern mageResult mageGetDeviceIndexes(
	struct mageRenderer *renderer,
	VkPhysicalDevice device,
	struct mageIndiciesIndexes *indicies
);
extern void mageIndiciesIndexesCreate(
	struct mageIndiciesIndexes *indicies, 
	const uint32_t *graphics, 
	const uint32_t graphicCount, 
	const uint32_t *presents, 
	const uint32_t presentCount
);
extern void mageIndiciesIndexesDestroy(
	struct mageIndiciesIndexes *indicies
);
extern uint32_t mageFindMemoryType(
	uint32_t typeFilter, 
	VkMemoryPropertyFlags properties,
	struct mageRenderer *renderer
);

/* Command buffers */
extern VkCommandBuffer mageCommandBufferBegin(
	struct mageRenderer *renderer
);
extern void mageCommandBufferEnd(
	VkCommandBuffer commandBuffer,
	struct mageRenderer *renderer
);

/* Renderer */
extern mageResult mageRendererCreate(
	struct mageRenderer *renderer, 
	struct mageWindow *window, 
	struct mageRendererCreateInfo *props
);	
extern void mageRendererResize(
	struct mageRenderer *renderer, 
	struct mageWindow *window,
	struct mageRendererCreateInfo *rendererProps
);
extern void mageRendererRecord(
	struct mageRenderer *renderer,
	struct mageRenderable *renderable
);
extern void mageRendererDraw(
	struct mageRenderer *renderer,
	struct mageRenderable *renderable
);
extern void mageRendererDestroy(
	struct mageRenderer *renderer,
	struct mageRendererCreateInfo *rendererInfo
);

/* Renderables */
extern mageResult mageRenderableCreate(
	struct mageRenderable *renderable,
	mageRenderablePipeLineMode pipelineMode,
	struct mageRenderer *renderer
);
extern void mageRenderableDestroy(
	struct mageRenderable *renderable,
	struct mageRenderer *renderer
);


extern mageResult mageApplicationCreate(
	struct mageApplication *application, 
	struct mageApplicationCreateInfo applicationInfo, 
	struct mageRendererCreateInfo rendererInfo
);
extern void mageApplicationDestroy(
	struct mageApplication *application
);

#endif  


