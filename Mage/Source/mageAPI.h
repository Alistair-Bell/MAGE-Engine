#ifndef __MAGE_API__
#define __MAGE_API__

#include "mageCore.h"


/*
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

*/

#define MAGE_SET_BIT(input, index, value) \
	(input |= value << index)

#define MAGE_VULKAN_CHECK(function) \
	mageHandleVulkanResult(#function, function)

#define MAGE_VOID_POINTER_CAST(data, type) \
	(*(type *)data)

/* Whith great power comes great responsiblity */
#if defined (MAGE_PRACTICAL_JOKES)
	#define MAGE_TRUE (rand() % 100 < 98)
	#define MAGE_FALSE !MAGE_TRUE
	#define true MAGE_TRUE
	#define false MAGE_FALSE
#else
	#define MAGE_TRUE 1
	#define MAGE_FALSE !MAGE_TRUE
	#define true MAGE_TRUE
	#define false MAGE_FALSE
#endif

#if defined (MAGE_ASSERTS)
	#define MAGE_ASSERT(expression) \
		assert(expression)
#else
	#define MAGE_ASSERT(expression)
#endif

#define MAGE_ECS_REGISTER_COMPONENT(scene, component, constructer, deconstructer, mode) \
	mageSceneRegisterComponent(scene, #component, sizeof(component), constructer, deconstructer, mode)

#define MAGE_ECS_BIND_NEW_COMPONENT_BY_TAG_TO_ENTITIES(scene, component, value, entities, count) \
	mageSceneComponentFromTagBindEntities(scene, #component, value, entities, count)

#define MAGE_ECS_BIND_NEW_COMPONENT_BY_ID_TO_ENTITIES(scene, tableID, value, entities, count) \
	mageSceneComponentFromIDBindEntities(scene, tableID, value, entities, count)

#define MAGE_ECS_BIND_EXISTING_COMPONENT_TO_ENTITIES(scene, componentHandle, entities, count) \
	mageSceneComponentBindExistingToEntities(scene, componentHandle, entities, count)

#define MAGE_ECS_REGISTER_SYSTEM(scene, callback, systemType, threadPriority, count, ...) \
	mageSceneSystemRegister(scene, callback, systemType, threadPriority, count, #__VA_ARGS__ )

#define MAGE_ECS_GET_COMPONENT_BY_HANDLE(scene, getType, handle, entity) \
	(*(getType *)mageSceneEntityFetchComponentByHandle(scene, #getType, handle, entity))


/* User config stuff */

/* Heap memory MAGE_MEMORY_ALLOCATE */
#define MAGE_MEMORY_ALLOCATE(size) \
	malloc(size)

/* Free memory */
#define MAGE_MEMORY_FREE(block) \
	free(block)

/* Realloc */
#define MAGE_MEMORY_REALLOCATE(block, newSize) \
	realloc(block, newSize)

#define MAGE_MEMORY_ARRAY_ALLOCATE(count, size) \
	calloc(count, size)

#define MAGE_SYSTEM_SUCCESS ((void *)1)
#define MAGE_SYSTEM_FAILURE ((void *)0)

#define MAGE_BIT(index) (1 << index) 
#define MAGE_PI_SQUARED 9.86960440109
#define MAGE_PI 		3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679
#define MAGE_PI_HALF	1.5707963267948966

#if defined (MAGE_USE_GLSL_STRUCTURES)
	#define vec2 mageVector2
	#define vec3 mageVector3
	#define vec4 mageVector4
	#define mat3 mageMatrix3
	#define mat4 mageMatrix4
#endif

#define mageColor mageVector3


typedef uint32_t mageEventHandle;
typedef uint32_t mageEntity;
typedef void 	*mageThread;

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
	MAGE_RESULT_DATA_NOT_PRESENT
} mageResult;

typedef enum MAGE_ECS_COMPONENT_REGISTERING_MODE_ENUM
{
	MAGE_ECS_COMPONENT_REGISTERING_MODE_REQUIRED,
	MAGE_ECS_COMPONENT_REGISTERING_MODE_OPTIONAL,
} mageComponentRegisteringMode;

typedef enum MAGE_SYSTEM_TYPE_ENUM
{
	MAGE_ECS_SYSTEM_TYPE_START				= 1,
	MAGE_ECS_SYSTEM_TYPE_AWAKE				= 2,
	MAGE_ECS_SYSTEM_TYPE_UPDATE				= 3,
	MAGE_ECS_SYSTEM_TYPE_LATE_UPDATE		= 4,
	MAGE_ECS_SYSTEM_TYPE_FIXED_UPDATE		= 5,
	MAGE_ECS_SYSTEM_TYPE_END				= 6,
} mageSystemType;

typedef enum MAGE_ECS_SYSTEM_THREAD_PRIORITY_ENUM
{
	MAGE_ECS_SYSTEM_THREAD_PRIORITY_NONE	= 1,
	MAGE_ECS_SYSTEM_THREAD_PRIORITY_FORCE	= 2,
} mageSystemThreadPriority;

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
	MAGE_TEXTURE_SAMPLER_CLAMP_TO_BORDER,
	MAGE_TEXTURE_SAMPLER_MIRRORED_CLAMP_TO_EDGE,
} mageTextureSamplerMode;


typedef void 		(*mageEventListenerCallback)(void *, mageEventType);
typedef void 		*(*mageMemoryAllocaterMethod)(uint64_t);
typedef void 		(*mageMemoryFreeMethod)(void *);
typedef void 		*(*mageMemoryListAllocaterMethod)(uint64_t, uint64_t);
typedef void 		*(*mageMemoryReallocater)(void *, uint64_t);
typedef void 		*(*mageThreadJobCallback)(void *);
typedef void 		*(*mageSystemCallback)(void *);
typedef void		(*mageComponentConstructer)(void *, const uint64_t);
typedef void		(*mageComponentDeconstructer)(void *);

struct mageVector2
{
	union
	{
		float Values[2];
		struct
		{
			float 				X;
			float 				Y;
		};
	};
};
struct mageVector3
{
	union
	{
		float Values[3];
		struct
		{
			float 				X;
			float 				Y;
			float 				Z;
		};
		struct
		{
			float 				Yaw;
			float 				Pitch;
			float 				Roll;
		};
	};
};
struct mageVector4
{
	union
	{
		float 					Values[4];
		float 					X;
		float 					Y;
		float 					Z;
		float 					W;
	};
};
struct mageMatrix3
{
	union
	{
		float 					Elements[9];
		struct
		{
			struct mageVector3 	Rows[3];
		};
	};
};
struct mageMatrix4
{
	union
	{
		float Elements[16];
		struct
		{
			struct mageVector4 Rows[4];
		};
	};
};



struct mageQueue
{
	uint32_t Count;
    uint32_t DataSize;
	void *Data;
};
struct mageSceneCreateInfo
{
	uint64_t								EntityLimit;
	uint64_t								ComponentLimit;
	const char								*SceneTag;
	uint8_t									RegisterDefaultComponents;
};
struct mageEntityPool
{
	uint32_t								ActiveCount;
	struct mageQueue						AvaliableQueue;
	mageEntity								*Pooled;
	struct mageComponentHandle				**Handles;
};
struct mageComponentHandle
{
	union
	{
		uint64_t Data;
		struct
		{
			uint32_t 						ComponentIndex;
			uint32_t 						TableIndex;
		};
		struct
		{
			uint32_t 						High;
			uint32_t 						Low;
		};
	};
};
struct mageComponent
{
	uint8_t									SharedCount;
	void									*Data;
};
struct mageComponentTable
{
	const char 								*Identifier;
	struct mageQueue						IndexQueues;
	struct mageComponent					*Stored;
	mageComponentConstructer				Constructer;
	mageComponentDeconstructer				Deconstructer;
	uint32_t								ID;
	uint32_t								StoredCount;
	uint32_t								ByteSize;
	mageComponentRegisteringMode			TableMode;
};
struct mageSystemTable
{
	uint32_t								Identifer;
	uint32_t								ComponentCount;
	mageSystemCallback						MethodCallback;
	mageSystemType							Type;
	mageThread								ThreadHandle;
	uint32_t 								*ComponentIDs;
};
struct mageSystemPackage
{
	uint32_t								Count;
	void									**Data;
};
struct mageScene
{
	const char 								*SceneTag;
	struct mageEntityPool					*Entities;
	struct mageSystemTable					*Systems;
	struct mageComponentTable				*ComponentTables;
	uint32_t								SystemCount;
	uint32_t								TableCount;
	uint32_t								RequiredTableCount;
	uint64_t								MaxComponents;
	uint64_t								MaxEntities;
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
	struct mageVector2						Vertex;
	struct mageColor						Color;
	struct mageVector2						TextureLocation;
};
struct mageTransform
{
	struct mageVector3						Position;
	struct mageVector3						Location;
	struct mageVector3						Rotation;
};
struct mageOrthographicCamera
{
	struct mageMatrix4 						ProjectionMatrix;
	struct mageMatrix4						ViewMatrix;
	struct mageMatrix4						ViewProjectionMatrix;
};

struct mageUniformObject
{
	struct mageMatrix4 						View;
	struct mageMatrix4 						Projection;
	struct mageMatrix4						Model;
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
	uint32_t								Bytes;
};
struct mageTextureCreateInfo
{
	mageTextureSamplerMode					SamplerMode;
	const char 								*TexturePath;
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
struct mageRenderableCreateInfo
{
	struct mageTextureCreateInfo			TextureCreateInfo;
	struct mageVertex						*Verticies;
	uint16_t								*Indicies;
	uint32_t								VertexCount;
	uint32_t								IndexCount;
};
struct mageRenderable
{
	struct mageBuffer						IndexBuffer;
	struct mageBuffer						VertexBuffer;
	struct mageTexture						Texture;
};
struct mageRenderableQuad
{
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
	uint8_t									TextureTransparency;
	uint32_t 								ShaderCount;
};
struct mageRenderer
{
	VkInstance 								Instance;
	VkDevice								Device;
	
	VkPhysicalDevice						PhysicalDevice;
	VkPhysicalDeviceMemoryProperties		PhysicalDeviceMemoryProperties;
	VkPhysicalDeviceFeatures				PhysicalDeviceFeatures;

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

	VkDescriptorSet							DescriptorSet;
	VkDescriptorPool						DescriptorPool;
	VkDescriptorSetLayout					DescriptorSetLayout;

	VkDebugUtilsMessengerCreateInfoEXT		DebugMessengerCreateInfo;
	VkDebugUtilsMessengerEXT				DebugMessenger;
	struct mageIndiciesIndexes				Indexes;
	struct mageSwapChainSupportDetails		SwapChainSupportInfo;

	struct mageBuffer						*DefaultSquareIndexBuffer;

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

/* Math formulas */
extern MAGE_API float mageDegreesToRadians(
	const float degress
);
extern MAGE_API float mageRadiansToDegrees(
	const float radians
);

/* Vectors */
extern MAGE_API void mageVector2CreateDefault(
	struct mageVector2 *vector
);
extern MAGE_API void mageVector2CreateFromFloats(
	struct mageVector2 *vector,
	const float x,
	const float y
);
extern MAGE_API void mageVector2CreateFromCopy(
	struct mageVector2 *destination,
	struct mageVector2 *source
);
extern MAGE_API void mageVector2Add(
	struct mageVector2 *destination,
	struct mageVector2 *other
);
extern MAGE_API void mageVector2Subtract(
	struct mageVector2 *destination,
	struct mageVector2 *other
);
extern MAGE_API void mageVector2Multiply(
	struct mageVector2 *destination,
	struct mageVector2 *other
);
extern MAGE_API void mageVector2Divide(
	struct mageVector2 *destination,
	struct mageVector2 *other
);
extern MAGE_API void mageVector2Log(
	const struct mageVector2 *vector,
	const char *name
);

extern MAGE_API void mageVector3CreateDefault(
	struct mageVector3 *vector
);
extern MAGE_API void mageVector3CreateFromFloats(
	struct mageVector3 *vector,
	const float x,
	const float y,
	const float z
);
extern MAGE_API void mageVector3CreateFromVector2AndFloat(
	struct mageVector3 *vector,
	const struct mageVector2 *xy,
	const float z 
);
extern MAGE_API void mageVector3CreateFromCopy(
	struct mageVector3 *destination,
	struct mageVector3 *source
);
extern MAGE_API void mageVector3Add(
	struct mageVector3 *destination,
	struct mageVector3 *other
);
extern MAGE_API void mageVector3Subtract(
	struct mageVector3 *destination,
	struct mageVector3 *other
);
extern MAGE_API void mageVector3Multiply(
	struct mageVector3 *destination,
	struct mageVector3 *other
);
extern MAGE_API void mageVector3Divide(
	struct mageVector3 *destination,
	struct mageVector3 *other
);
extern MAGE_API void mageVector3Log(
	const struct mageVector3 *vector,
	const char *name
);

extern MAGE_API void mageVector4CreateDefault(
	struct mageVector4 *vector
);
extern MAGE_API void mageVector4CreateFromFloats(
	struct mageVector4 *vector,
	const float x,
	const float y,
	const float z,
    const float w
);
extern MAGE_API void mageVector4CreateFrom2Vector2(
	struct mageVector4 *vector,
	const struct mageVector2 *xy,
	const struct mageVector2 *zw
);
extern MAGE_API void mageVector4CreateFromVector3AndFloat(
	struct mageVector4 *vector,
	const struct mageVector3 *xyz,
	const float w
);
extern MAGE_API void mageVector4CreateFromCopy(
	struct mageVector4 *destination,
	struct mageVector4 *source
);
extern MAGE_API void mageVector4Add(
	struct mageVector4 *destination,
	struct mageVector4 *other
);
extern MAGE_API void mageVector4Subtract(
	struct mageVector4 *destination,
	struct mageVector4 *other
);
extern MAGE_API void mageVector4Multiply(
	struct mageVector4 *destination,
	struct mageVector4 *other
);
extern MAGE_API void mageVector4Divide(
	struct mageVector4 *destination,
	struct mageVector4 *other
);
void mageVector4Log(
	const struct mageVector4 *vector,
	const char *name
);
	

/* Matrix's */
extern MAGE_API void mageMatrix3CreateDefault(
	struct mageMatrix3 *matrix
);
extern MAGE_API void mageMatrix3CreateFromRows(
	struct mageMatrix3 *matrix,
	const struct mageVector3 *row0,
	const struct mageVector3 *row1,
	const struct mageVector3 *row2
);
extern MAGE_API void mageMatrix3CreateFromSet(
	struct mageMatrix3 *matrix,
	const float *set,
	const uint32_t setCount
);
extern MAGE_API void mageMatrix3CreateFromDiagonal(
	struct mageMatrix3 *matrix,
	const float diagonal
);
extern MAGE_API struct mageVector3 mageMatrix3GetRow(
	const struct mageMatrix3 *matrix,
	const uint32_t index
);
extern MAGE_API struct mageVector3 mageMatrix3GetColumn(
	const struct mageMatrix3 *matrix,
	const uint32_t index
);
extern MAGE_API void mageMatrix3Log(
	const struct mageMatrix3 *matrix,
	const char *name
);

extern MAGE_API void mageMatrix4CreateDefault(
	struct mageMatrix4 *matrix
);
extern MAGE_API void mageMatrix4CreateFromRows(
	struct mageMatrix4 *matrix,
	const struct mageVector4 *row0,
	const struct mageVector4 *row1,
	const struct mageVector4 *row2,
	const struct mageVector4 *row3
);
extern MAGE_API void mageMatrix4CreateFromSet(
	struct mageMatrix4 *matrix,
	const float *set,
	const uint32_t setCount
);
extern MAGE_API void mageMatrix4CreateDiagonal(
	struct mageMatrix4 *matrix,
	const float diagonal
);
extern MAGE_API void mageMatrix4CreateFromCopy(
	struct mageMatrix4 *destination,
	const struct mageMatrix4 *source
);
extern MAGE_API struct mageVector4 mageMatrix4GetRow(
	const struct mageMatrix4 *matrix,
	const uint32_t index
);
extern MAGE_API struct mageVector4 mageMatrix4GetColumn(
	const struct mageMatrix4 *matrix,
	const uint32_t index
);
extern MAGE_API void mageMatrix4Multiply(
	struct mageMatrix4 *destination,
	const struct mageMatrix4 *source
);
extern MAGE_API void mageMatrix4ApplyOrthographicProjection(
	struct mageMatrix4 *destination,
	const float left,
	const float right,
	const float bottom,
	const float top,
	const float near,
	const float far
);
extern MAGE_API void mageMatrix4ApplyPerspectiveProjection(
	struct mageMatrix4 *destination,
	const float fieldOfView,
	const float aspectRatio,
	const float near,
	const float far
);
extern MAGE_API void mageMatrix4TranslateVector3(
	struct mageMatrix4 *destination,
	const struct mageVector3 *translation
);
extern MAGE_API void mageMatrix4RotateAxis(
	struct mageMatrix4 *destination,
	const float angle,
	const struct mageVector3 *axis
);
extern MAGE_API void mageMatrix4ScaleVector3(
	struct mageMatrix4 *destination,
	const struct mageVector3 *scale
);
extern MAGE_API void mageMatrix4Inverse(
	struct mageMatrix4 *destination
);
extern MAGE_API void mageMatrix4Log(
	const struct mageMatrix4 *matrix,
	const char *name
);


/* Queues */
extern MAGE_API void mageQueueCreate(
	struct mageQueue *queue, 
	const uint32_t dataSize
);
extern MAGE_API void mageQueueCreateFromSet(
	struct mageQueue *queue,
	uint32_t elementCount,
	uint32_t elementSize,
	void *data
);
extern MAGE_API void mageQueuePush(
	struct mageQueue *queue, 
	void *data
);
extern MAGE_API mageResult mageQueuePop(
	struct mageQueue *queue,
	void *buffer
);
extern MAGE_API void mageQueueDestroy(
	struct mageQueue *queue
);

/* Threads, abstraction of the native platform thread system */
extern MAGE_API void *mageThreadCreate(
	void
);
extern MAGE_API void mageThreadBegin(
	mageThread thread,
	mageThreadJobCallback callback,
	void *data
);
extern MAGE_API uint32_t mageThreadGetID(
	const mageThread thread
);
extern MAGE_API void mageThreadDestroy(
	mageThread thread
);

/* ECS system */
extern MAGE_API void mageSceneCreate(
	struct mageScene *scene,
	const struct mageSceneCreateInfo *info
);
extern MAGE_API uint32_t mageSceneRegisterComponent(
	struct mageScene *scene,
	const char *component,
	const uint32_t dataSize,
	mageComponentConstructer constructer,
	mageComponentDeconstructer deconstructer,
	const mageComponentRegisteringMode mode
);
extern MAGE_API mageEntity mageSceneEntityCreate(
	struct mageScene *scene
);
extern MAGE_API void *mageSceneEntityFetchComponentByHandle(
	struct mageScene *scene,
	const char *component,
	const struct mageComponentHandle handle,
	const mageEntity entity
);
extern MAGE_API void *mageSceneEntityFetchComponentByTableID(
	struct mageScene *scene,
	const uint32_t table,
	const mageEntity entity
);
extern MAGE_API void mageSceneEntityDestroy(
	struct mageScene *scene,
	mageEntity entity
);
extern MAGE_API void mageSceneBindEntityRequiredComponents(
	struct mageScene *scene,
	const mageEntity entity,
	const uint32_t *componentID,
	const uint32_t count
);
extern MAGE_API struct mageComponentHandle mageSceneComponentFromTagBindEntities(
	struct mageScene *scene,
	const char *component,
	void *data,
	mageEntity *entities,
	const uint64_t count
);
extern MAGE_API struct mageComponentHandle mageSceneComponentFromIDBindEntities(
	struct mageScene *scene,
	const uint32_t id,
	void *data,
	mageEntity *entities,
	const uint64_t count
);
extern MAGE_API void mageSceneComponentBindExistingToEntities(
	struct mageScene *scene,
	const struct mageComponentHandle componentHandle,
	mageEntity *entities,
	const uint32_t count
);
extern MAGE_API uint32_t mageSceneSystemRegister(
	struct mageScene *scene,
	const mageSystemCallback callback,
	const mageSystemType type,
	const mageSystemThreadPriority threadPriority,
	const uint32_t count,
	...
);
extern MAGE_API uint8_t mageSceneStart(
	struct mageScene *scene
);
extern MAGE_API uint8_t  mageSceneUpdate(
	struct mageScene *scene
);
extern MAGE_API uint8_t  mageSceneLateUpdate(
	struct mageScene *scene
);
extern MAGE_API uint8_t mageSceneFinsish(
	struct mageScene *scene,
	const uint8_t serialize,
	const char *output
);

extern MAGE_API void mageSystemTableDestroy(
	struct mageSystemTable *table
);


/* Placeholder for later
extern MAGE_API mageResult mageSceneSerialiseActive(
	struct mageScene *scene,
	const char *output
);
*/

extern MAGE_API void mageSceneComponentTableFree(
	struct mageComponentTable *table
);
extern MAGE_API void mageSceneDestroy(
	struct mageScene *scene
);

/* Generic */
extern MAGE_API mageResult mageEngineInitialise(
	void
);
extern MAGE_API void mageLogMessage(
	const mageLogUser user, 
	const mageLogMode severity, 
	const char *format, 
	...
);
extern MAGE_API VkResult mageHandleVulkanResult(
	const char *functionName,
	VkResult functionResult
);
extern MAGE_API void mageLogEnd(
	void
);
extern MAGE_API void mageLogInitialise(
	const char *outputFile
);

/* File IO */
extern MAGE_API char *mageFileReadContents(
	const char *file, 
	const char *readmode, 
	uint32_t *fileSize
);
extern MAGE_API mageResult mageFileDumpContents(
	const char *file, 
	const char *buffer, 
	const uint8_t clean
);

/* Window */
extern MAGE_API mageResult mageWindowCreate(
	struct mageWindow *window, 
	struct mageApplicationCreateInfo *createInfo
);
extern MAGE_API void mageWindowDestroy(
	struct mageWindow *window
);

/* Event system */ 
extern MAGE_API void mageInputSetup(
	struct mageApplication *application
);
extern MAGE_API void mageEventSetupMaster(
	void
);
extern MAGE_API mageEventCategoryBit *mageEventGenerateCategories(
	const mageEventType type
);
extern MAGE_API uint16_t mageEventHandleCreate(
	const mageEventType type
);
extern MAGE_API mageEventType mageEventExtractEventType(
	const uint16_t handle
);
extern MAGE_API uint8_t mageEventInCategory(
	const uint16_t handle, 
	const mageEventCategoryBit category
);
extern MAGE_API void mageEventRegisterListener(
	mageEventListenerCallback callback
);
extern MAGE_API void mageEventFormatWindowClose(
	void *buffer
);
extern MAGE_API void mageEventFormatWindowFocus(
	void *buffer
);
extern MAGE_API void mageEventFormatWindowLostFocus(
	void *buffer
);
extern MAGE_API void mageEventFormatWindowMoved(
	void *buffer, 
	const int32_t x, 
	const int32_t y
);
extern MAGE_API void mageEventFormatKeyPressed(
	void *buffer, 
	const uint8_t keycode
);
extern MAGE_API void mageEventFormatKeyReleased(
	void *buffer, 
	const uint8_t keycode
);
extern MAGE_API void mageEventFormatKeyRepeat(
	void *buffer, 
	const uint8_t keycode
);
extern MAGE_API void mageEventFormatMouseButtonPressed(
	void *buffer, 
	const uint8_t mousecode
);
extern MAGE_API void mageEventFormatMouseButtonRelease(
	void *buffer, 
	const uint8_t mousecode
);
extern MAGE_API void mageEventFormatMouseMoved(
	void *buffer, 
	const double x, 
	const double y
);
extern MAGE_API void mageEventFormatMouseWheelMoved(
	void *buffer, 
	const double x, 
	const double y
);
extern MAGE_API void mageEventDispatch(
	void *event
);

/* Vertexes */
extern MAGE_API void mageVertexCreate(
	struct mageVertex *vertexInstance, 
	struct mageVector2 vertex, 
	struct mageVector3 color
);
extern MAGE_API VkVertexInputBindingDescription mageVertexBindingDescription(
);
extern MAGE_API VkVertexInputAttributeDescription *mageVertexGetAttributeDescriptions(
	uint32_t *count
);

/* Buffers */
extern MAGE_API VkBufferUsageFlags mageBufferTypeToFlag(
	const mageBufferType type
);
extern MAGE_API void mageBufferWrapperAllocate(
	struct mageBufferWrapper *buffer,
	uint32_t dataSize,
	const VkBufferUsageFlags bufferUsage,
	const VkBufferUsageFlags flags,
	struct mageRenderer *renderer
);
extern MAGE_API void mageBufferWrapperDestroy(
	struct mageBufferWrapper *buffer,
	struct mageRenderer *renderer	
);
extern MAGE_API void mageBufferCreate(
	struct mageBuffer *buffer,
	mageBufferType bufferType,
	void *data,
	uint32_t dataByteSize,
	struct mageRenderer *renderer
);
extern MAGE_API VkBuffer mageBufferGetNativeBuffer(
	struct mageBuffer *buffer
);
extern MAGE_API void mageBufferUpdate(
	struct mageBuffer *buffer,
	void *memory,
	uint32_t dataSize,
	struct mageRenderer *renderer	
);
extern MAGE_API void mageBufferDestroy(
	struct mageBuffer *buffer,
	struct mageRenderer *renderer
);

/* Textures */ 
extern MAGE_API mageResult mageTextureCreate(
	struct mageTexture *texture,
	const char *texturePath,
	mageTextureSamplerMode samplerMode,
	struct mageRenderer *renderer
);
extern MAGE_API void mageTextureDestroy(
	struct mageTexture *texture,
	struct mageRenderer *renderer
);

/* Shaders */
extern MAGE_API mageResult mageShaderCreate(
	struct mageShader *shader, 
	const char *shaderFile, 
	const char *entryPoint, 
	const mageShaderType shaderType
);
extern MAGE_API mageShaderType mageShaderTypeFromString(
	const char *name
);
extern MAGE_API VkShaderStageFlagBits mageShaderTypeToBit(
	const mageShaderType shaderType
);
extern MAGE_API VkFramebuffer mageRendererGetActiveFrameBuffer(
	struct mageRenderer *renderer
);
extern MAGE_API VkShaderModule mageShaderCreateModule(
	struct mageShader *shader, 
	VkDevice device
);

/* Vulkan images */
VkResult mageImageCreate(
	VkImage *image, 
	VkDeviceMemory *memory, 
	const uint32_t width, 
	const uint32_t height, 
	VkFormat format, 
	VkImageTiling tilingMode, 
	VkImageUsageFlags usage, 
	VkMemoryPropertyFlags properties, 
	struct mageRenderer *renderer
);
VkResult mageImageViewCreate(
	VkImage image, 
	VkImageView *view, 
	VkFormat format, 
	struct mageRenderer *renderer
);

/* Swap chain support */
extern MAGE_API void mageSwapChainSupportCreate(
	struct mageSwapChainSupportDetails *swapChainSupport,
	const VkSurfaceCapabilitiesKHR surfaceCapabilities,
	VkSurfaceFormatKHR *formats,
	const uint32_t formatCount,
	VkPresentModeKHR *presentModes,
	const uint32_t presentCount,
	VkExtent2D extent
);
extern MAGE_API mageResult mageGetSwapChainSupport(
	struct mageSwapChainSupportDetails *swapChainSupport,
	struct mageWindow *window,
	VkPhysicalDevice physicalDevice,
	VkSurfaceKHR surface
);
extern MAGE_API VkPresentModeKHR mageSwapChainSupportPickPresentMode(
	struct mageSwapChainSupportDetails *swapChainSupport
);
extern MAGE_API VkSurfaceFormatKHR mageSwapChainSupportPickSurfaceFormat(
	struct mageSwapChainSupportDetails *swapChainDetails
);
extern MAGE_API void mageSwapChainSupportDestroy(
	struct mageSwapChainSupportDetails *swapChainSupport
);

/* Graphics card indexes */
extern MAGE_API mageResult mageGetDeviceIndexes(
	struct mageRenderer *renderer,
	VkPhysicalDevice device,
	struct mageIndiciesIndexes *indicies
);
extern MAGE_API void mageIndiciesIndexesCreate(
	struct mageIndiciesIndexes *indicies, 
	const uint32_t *graphics, 
	const uint32_t graphicCount, 
	const uint32_t *presents, 
	const uint32_t presentCount
);
extern MAGE_API void mageIndiciesIndexesDestroy(
	struct mageIndiciesIndexes *indicies
);
extern MAGE_API uint32_t mageFindMemoryType(
	uint32_t typeFilter, 
	VkMemoryPropertyFlags properties,
	struct mageRenderer *renderer
);

/* Command buffers */
extern MAGE_API VkCommandBuffer mageCommandBufferBegin(
	struct mageRenderer *renderer
);
extern MAGE_API void mageCommandBufferEnd(
	VkCommandBuffer commandBuffer,
	struct mageRenderer *renderer
);

/* Descriptor set */
extern MAGE_API void mageDescriptorPoolCreate(
	struct mageRenderer *renderer
);
extern MAGE_API void mageDescriptorSetLayoutCreate(
	struct mageRenderer *renderer
);
extern MAGE_API void mageDescriptorSetsAllocate(
	struct mageRenderer *renderer
);
extern MAGE_API void mageDescriptorSetsUpdate(
	const VkImage *image,
	const VkImageView *view,
	const VkSampler *textureSampler,
	const VkSamplerAddressMode mode,
	struct mageRenderer *renderer
);

/* Renderer */
extern MAGE_API mageResult mageRendererCreate(
	struct mageRenderer *renderer, 
	struct mageWindow *window, 
	struct mageRendererCreateInfo *props
);	
extern MAGE_API void mageRendererResize(
	struct mageRenderer *renderer, 
	struct mageWindow *window,
	struct mageRendererCreateInfo *rendererProps
);
extern MAGE_API void mageRendererDrawRenderables(
	struct mageRenderer *renderer,
	struct mageRenderable **renderables,
	const uint32_t count
);
extern MAGE_API void mageRendererDrawQuads(
	struct mageRenderer *renderer,
	struct mageRenderableQuad **quads,
	const uint32_t count
);
extern MAGE_API void mageRendererDestroy(
	struct mageRenderer *renderer,
	struct mageRendererCreateInfo *rendererInfo
);

/* Orthographic camera */
extern MAGE_API void mageOrthographicCameraCreate(
	struct mageOrthographicCamera *camera,
	const float left,
	const float right,
	const float bottom,
	const float top
);
extern MAGE_API void mageOrthographicCameraCalculateViewMatrix(
	struct mageOrthographicCamera *camera,
	struct mageTransform *transform
);
extern MAGE_API void mageOrthographicCameraSetProjection(
	struct mageOrthographicCamera *camera, 
	const float left, 
	const float right, 
	const float bottom, 
	const float top
);

/* Renderables */
extern MAGE_API mageResult mageRenderableCreate(
	struct mageRenderable *renderable,
	struct mageRenderableCreateInfo *info,
	struct mageRenderer *renderer
);
extern MAGE_API void mageRenderableDestroy(
	struct mageRenderable *renderable,
	struct mageRenderer *renderer
);
extern MAGE_API void mageRenderableQuadCreate(
	struct mageRenderableQuad *quad,
	struct mageRenderableCreateInfo *info,
	struct mageRenderer *renderer
);
extern MAGE_API void mageRenderableQuadDestroy(
	struct mageRenderableQuad *quad,
	struct mageRenderer *renderer
);

/* Application */
extern MAGE_API mageResult mageApplicationCreate(
	struct mageApplication *application, 
	struct mageApplicationCreateInfo applicationInfo, 
	struct mageRendererCreateInfo rendererInfo
);
extern MAGE_API void mageApplicationDestroy(
	struct mageApplication *application
);

#endif  

