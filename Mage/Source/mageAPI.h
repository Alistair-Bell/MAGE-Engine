#ifndef MAGE_API_HEADER
#define MAGE_API_HEADER

#include "mageCore.h"


/*
	    __  ___                    ______            _          
	   /  |/  ____ _____ ____     / ________  ____ _(_____  ___ 
	  / /|_/ / __ `/ __ `/ _ \   / __/ / __ \/ __ `/ / __ \/ _ \
	 / /  / / /_/ / /_/ /  __/  / /___/ / / / /_/ / / / / /  __/
	/_/  /_/\__,_/\__, /\___/  /_____/_/ /_/\__, /_/_/ /_/\___/ 
	             /____/                    /____/            

	This header contains the generic function, enums and structures that are used in the engine
	
	Open source 2D game engine written with low memory footprint and performance in mind
	The engine is not the next unity or unreal. Just a fun tool to mess around with
	Documentation has been moved to the github wiki so use that for the documentation or ask me personally
	Documentation will be released once version 1.0 is released
	To contribute go to https://github.com/Alistair-Bell/Mage-Engine
	For use please read the license

*/

#define MAGE_SET_BIT(input, index, value) \
	(input |= value << index)

#define MAGE_VULKAN_CHECK(function) \
	mageHandleVulkanResult(#function, function)

#define MAGE_VOID_POINTER_CAST(data, type) \
	(*(type *)data)

#if defined (MAGE_ASSERTS)
	#define MAGE_ASSERT(expression) \
		assert(expression)
#else
	#define MAGE_ASSERT(expression)
#endif

#if defined (MAGE_DEBUG)
	#define MAGE_LOG_CORE_INFORM(x, ...) 		mageLogMessage(MAGE_LOG_USER_CORE, MAGE_LOG_MODE_INFORM, x, __VA_ARGS__)
	#define MAGE_LOG_CORE_WARNING(x, ...) 		mageLogMessage(MAGE_LOG_USER_CORE, MAGE_LOG_MODE_WARNING, x, __VA_ARGS__)
	#define MAGE_LOG_CORE_ERROR(x, ...) 		mageLogMessage(MAGE_LOG_USER_CORE, MAGE_LOG_MODE_ERROR, x, __VA_ARGS__)
	#define MAGE_LOG_CORE_FATAL_ERROR(x, ...) 	mageLogMessage(MAGE_LOG_USER_CORE, MAGE_LOG_MODE_FATAL_ERROR, x, __VA_ARGS__)
#else
	#define MAGE_LOG_CORE_INFORM(x, ...)
	#define MAGE_LOG_CORE_WARNING(x, ...)
	#define MAGE_LOG_CORE_ERROR(x, ...)
	#define MAGE_LOG_CORE_FATAL_ERROR(x, ...)
	#define MAGE_DEBUG_BREAK
#endif

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

#define MAGE_TRUE 1
#define MAGE_FALSE !MAGE_TRUE

struct mageMatrix2;
struct mageMatrix3;
struct mageMatrix4;
struct mageTransform;
struct mageOrthographicCamera;
struct mageQueue;
struct mageRenderer;
struct mageShader;
struct mageTexture;
struct mageBuffer;
struct mageBufferWrapper;
struct mageAudioDriver;
struct mageWindow;
struct mageRendererCreateInfo;
struct mageRenderableQuad;
struct mageRenderable;
struct mageApplicationCreateInfo;

#define mageColor mageVector3

/* 
    Universal declarations
*/
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

/* 
	Generic
*/

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

/* 
    Maths 
*/

#define MAGE_PI_SQUARED 9.86960440109
#define MAGE_PI 		3.14159265358
#define MAGE_PI_HALF	1.57079632679

extern MAGE_API float mageDegreesToRadians(
	const float degress
);
extern MAGE_API float mageRadiansToDegrees(
	const float radians
);

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

/* 
	Transform
*/

struct mageTransform
{
	struct mageVector3						Position;
	struct mageVector3						Location;
	struct mageVector3						Rotation;
};

/* 
	Orthographic camera
*/

struct mageOrthographicCamera
{
	struct mageMatrix4 						ProjectionMatrix;
	struct mageMatrix4						ViewMatrix;
	struct mageMatrix4						ViewProjectionMatrix;
};

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

/*
    Threading
*/

typedef void        *mageThread;
typedef void 		*(*mageThreadJobCallback)(void *);

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
extern MAGE_API void mageThreadEnd(
	mageThread thread
);
extern MAGE_API void mageThreadTerminate(
	mageThread thread
);

/* 
    Generic data structures
*/
struct mageQueue
{
	uint32_t                            Count;
    uint32_t                            DataSize;
	void                                *Data;
};

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

/* 
    ECS systems
*/

typedef void 		*(*mageSystemCallback)(void *);
typedef void		(*mageComponentConstructer)(void *, const uint64_t);
typedef void		(*mageComponentDeconstructer)(void *);
typedef             uint64_t mageEntity;


#define MAGE_SYSTEM_SUCCESS ((void *)1)
#define MAGE_SYSTEM_FAILURE ((void *)0)

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
extern MAGE_API mageResult mageSceneTick(
	struct mageScene *scene
);
extern MAGE_API void mageSystemTableDestroy(
	struct mageSystemTable *table
);
extern MAGE_API void mageSceneComponentTableFree(
	struct mageComponentTable *table
);
extern MAGE_API void mageSceneDestroy(
	struct mageScene *scene
);

/* 
    Vulkan GPU queue indexes
*/
struct mageIndiciesIndexes
{
	uint32_t								*GraphicIndexes;
	uint32_t								*PresentIndexes;
	uint32_t								GraphicIndexesCount;
	uint32_t								PresentIndexesCount;
};

#if defined (MAGE_VULKAN_BACKEND)

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

#endif

/* 
    Shaders
*/

typedef enum MAGE_SHADER_TYPE_ENUM
{
	MAGE_SHADER_TYPE_VERTEX								= 1,
	MAGE_SHADER_TYPE_TESSELLATION_CONTROL				= 2,
	MAGE_SHADER_TYPE_TESSELLATION_EVALUATION			= 3,
	MAGE_SHADER_TYPE_GEOMETRY							= 4,
	MAGE_SHADER_TYPE_FRAGMENT							= 5,
	MAGE_SHADER_TYPE_COMPUTE							= 6,
} mageShaderType;

typedef enum MAGE_TEXTURE_SAMPLER_MODE_ENUM
{
	MAGE_TEXTURE_SAMPLER_REPEAT,
	MAGE_TEXTURE_SAMPLER_MIRRORED_REPEAT,
	MAGE_TEXTURE_SAMPLER_CLAMP_TO_EDGE,
	MAGE_TEXTURE_SAMPLER_CLAMP_TO_BORDER,
	MAGE_TEXTURE_SAMPLER_MIRRORED_CLAMP_TO_EDGE,
} mageTextureSamplerMode;

struct mageShader
{
	mageShaderType 							ShaderType;
	const char 								*FilePath;
	const char 								*EntryPoint;
};

extern MAGE_API mageResult mageShaderCreate(
	struct mageShader *shader, 
	const char *shaderFile, 
	const char *entryPoint, 
	const mageShaderType shaderType
);
extern MAGE_API mageShaderType mageShaderTypeFromString(
	const char *name
);

#if defined (MAGE_VULKAN_BACKEND)

extern MAGE_API VkShaderStageFlagBits mageShaderTypeToBit(
	const mageShaderType shaderType
);
extern MAGE_API VkShaderModule mageShaderCreateModule(
	struct mageShader *shader, 
	VkDevice device
);

#endif

/*
	Textures
*/

struct mageTextureCreateInfo
{
	mageTextureSamplerMode					SamplerMode;
	const char 								*TexturePath;
};

#if defined (MAGE_VULKAN_BACKEND)

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

#else

struct mageTexture
{
    void *VoidData;
};

#endif

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


/* 
	Images
*/

#if defined (MAGE_VULKAN_BACKEND)

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

#endif

/* 
	Buffers
*/

typedef enum MAGE_BUFFER_TYPE_ENUM
{
	MAGE_BUFFER_TYPE_VERTEX,
	MAGE_BUFFER_TYPE_INDEX,
	MAGE_BUFFER_TYPE_SOURCE,
	MAGE_BUFFER_TYPE_TRANSFER,
} mageBufferType;

#if defined (MAGE_VULKAN_BACKEND)

struct mageBufferWrapper
{
	VkBuffer								Buffer;
	VkDeviceMemory							AllocatedMemory;
};

#else

struct mageBufferWrapper
{
    void *VoidData;

};

#endif

struct mageBuffer
{
	mageBufferType							BufferType;
	struct mageBufferWrapper				Wrapper;
	uint32_t								Bytes;
};

#if defined (MAGE_VULKAN_BACKEND)

extern MAGE_API VkBufferUsageFlags mageBufferTypeToFlag(
	const mageBufferType type
);

#endif

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

/* 
	Vertex bindings 
*/

struct mageVertex
{
	struct mageVector2						Vertex;
	struct mageVector3						Color;
	struct mageVector2						TextureLocation;
};

extern MAGE_API void mageVertexCreate(
	struct mageVertex *vertexInstance, 
	struct mageVector2 vertex, 
	struct mageVector3 color
);

#if defined (MAGE_VULKAN_BACKEND)

extern MAGE_API VkVertexInputBindingDescription mageVertexBindingDescription(
);
extern MAGE_API VkVertexInputAttributeDescription *mageVertexGetAttributeDescriptions(
	uint32_t *count
);

#endif


/* 
	Graphics swap chain support
*/

#if defined (MAGE_VULKAN_BACKEND)

struct mageSwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR 				Capabilities;
	VkSurfaceFormatKHR 						*Formats;
	VkPresentModeKHR						*PresentModes;
	VkExtent2D								Extent;
	uint32_t								FormatCount;
	uint32_t								PresentCount;
};

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

#endif

/*
	Graphics card indexes
*/

#if defined (MAGE_VULKAN_BACKEND)

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

#endif

/*
	Command buffers
*/

#if defined (MAGE_VULKAN_BACKEND)

extern MAGE_API VkCommandBuffer mageCommandBufferBegin(
	struct mageRenderer *renderer
);
extern MAGE_API void mageCommandBufferEnd(
	VkCommandBuffer commandBuffer,
	struct mageRenderer *renderer
);

#endif

/*
	Descriptor sets
*/

#if defined (MAGE_VULKAN_BACKEND)

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

#endif

/*
	Vulkan memory management
*/

#if defined (MAGE_VULKAN_BACKEND)

#define MAGE_VULKAN_MEMORY_DEFAULT_BLOCK_SIZE 1024 * 1024 * 64 


#define MAGE_VULKAN_MEMORY_MAX_HEAP_COUNT 6
#define MAGE_VULKAN_MEMORY_MAX_OFFSET_COUNTS 32

typedef enum MAGE_VULKAN_MEMORY_BLOCK_MODES_ENUM
{
	MAGE_VULKAN_MEMORY_BLOCK_MODES_STORAGE_BUFFER 	= 0x01,
	MAGE_VULKAN_MEMORY_BLOCK_MODES_STORAGE_TEXTURE 	= 0x02,
	MAGE_VULKAN_MEMORY_BLOCK_MODES_STORAGE_SAMPLER	= 0x03
} mageVulkanMemoryBlockModes;

struct mageVulkanMemoryHeap
{
	VkDeviceMemory	Memory;
	VkDeviceSize	BlockSize;
	VkDeviceSize 	Unallocated;
	VkDeviceSize	BufferBlockOffsets[MAGE_VULKAN_MEMORY_MAX_OFFSET_COUNTS];
	VkDeviceSize	*NextOffset;
};
struct mageVulkanMemoryBufferBlock
{
	uint32_t		MemoryTypeIndex;
	uint64_t 		Offset;
	uint64_t		Size;
};


extern MAGE_API VkResult mageVulkanMemoryAllocateHeap(
	VkDevice device,
	struct mageVulkanMemoryHeap *heap,
	uint64_t bytes
);
extern MAGE_API void mageVulkanMemoryMapBufferToBlock(
	VkDevice device,
	struct mageVulkanMemoryHeap *heap,
	const struct mageVulkanMemoryBufferBlock *block,
	VkBuffer buffer,
	const VkBufferUsageFlags flags,
	void *data,
	const uint64_t dataSize
);
extern MAGE_API VkPhysicalDeviceMemoryProperties mageVulkanMemoryGetDeviceProperties(
	VkPhysicalDevice device
);
extern MAGE_API uint32_t mageVulkanMemoryFindMemoryType(
	VkPhysicalDeviceMemoryProperties propeties,
	uint32_t typeFilter, 
	VkMemoryPropertyFlags properties
);
extern MAGE_API void mageVulkanMemoryFreeMemory(
	VkDevice device,
	struct mageVulkanMemoryHeap *heap
);

#endif


/* 
	Renderer 
*/

#if defined (MAGE_VULKAN_BACKEND)

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

#endif

struct mageRendererCreateInfo
{
	struct mageShader						*PipelineShaders;
	uint8_t									TextureTransparency;
	uint32_t 								ShaderCount;
};

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
	struct mageRenderer *renderer
);

/* 
	Renderables
*/
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
struct mageRenderableCreateInfo
{
	struct mageTextureCreateInfo			TextureCreateInfo;
	struct mageVertex						*Verticies;
	uint16_t								*Indicies;
	uint32_t								VertexCount;
	uint32_t								IndexCount;
};

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

/* 
	Audio driver
*/

enum MAGE_AUDIO_DRIVER_FLAGS_ENUM
{
	MAGE_AUDIO_DRIVER_FLAGS_READY = 0x01
} mageAudioDriverFlags;

#if defined (MAGE_PULSE_AUDIO_BACKEND)

struct mageAudioDriver
{
	pa_mainloop			*MainLoop;
	pa_context			*Context;
};

#endif

extern MAGE_API mageResult mageAudioDriverCreate(
	struct mageAudioDriver *driver,
	void *userData
);
extern MAGE_API void mageAudioDriverDetectChannels(
	struct mageAudioDriver *driver
);
extern MAGE_API void mageAudioDriverDestroy(
	struct mageAudioDriver *driver
);

/* 
	Window
*/

struct mageWindow
{
	int32_t 								Width;
	int32_t 								Height;
	uint32_t 								Running;	
	const char 							   	*Title;
	GLFWwindow 							   	*Context;
};

extern MAGE_API mageResult mageWindowCreate(
	struct mageWindow *window, 
	struct mageApplicationCreateInfo *createInfo
);
extern MAGE_API void mageWindowDestroy(
	struct mageWindow *window
);

/*
	Application
*/

struct mageApplicationCreateInfo
{
	uint16_t 								Width;
	uint16_t 								Height;
	uint8_t									Fullscreen;
	uint8_t									FixedResolution;
	char 						   			*Name;
	const char 								*WindowIcon;
};
struct mageApplication
{
	struct mageRenderer 					*Renderer;
	struct mageWindow 						*Window;
	/*struct mageApplicationCreateInfo		CreateInfo;*/
	/*struct mageRendererCreateInfo			RendererCreateInfo;*/
	uint8_t 								Running;
};

/* Application */
extern MAGE_API mageResult mageApplicationCreate(
	struct mageApplication *application, 
	struct mageApplicationCreateInfo applicationInfo, 
	struct mageRendererCreateInfo rendererInfo
);
extern MAGE_API void mageApplicationDestroy(
	struct mageApplication *application
);

/* 
	File IO 
*/
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


#endif
