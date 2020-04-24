#ifndef __MAGE_API__
#define __MAGE_API__

#include "mageCore.h"

typedef char sint8;
typedef unsigned char uint8;
typedef short sint16;
typedef unsigned short uint16;
typedef int sint32;
typedef unsigned int uint32;
typedef long sint64;
typedef unsigned long uint64;


/*! 
	@brief Free method used by the destory methods throughout the API
	@param item pointer to a block of memory which will be freed
	@return Nothing
	@warning When overriding the method using a macro still call free() or leaks will occur
*/
extern void mageFreeMethod(void *item);
/* Tries to dump the data to the pointer checking for memory size and if the pointer is null*/
extern void mageTryDumpSuccess(uint8 contents, void *state);

/* Logging methods */

/* Logging macros for the type of the logs */
#define MAGE_LOG_INFORM 0
#define MAGE_LOG_WARNING 1  
#define MAGE_LOG_ERROR 2
#define MAGE_LOG_FATAL_ERROR 3
/* Logging macros for the location of the log */
#define MAGE_LOG_CORE 0
#define MAGE_LOG_CLIENT 1

/*!
	@brief Logs to the console and writes to an output
	@param user Core = 0 | Client = 1
	@param severity Inform = 0 | Warning = 1 | Error = 2 | Fatal Error = 3 (causes an assert break)
	@param line Line where it is being called from
	@param file File where it is being called from
	@param format Formatting using the printf style
	@param ... Values for the formatting 
	@returns Nothing
*/
extern void mageLogMessage(const uint8 user, const uint8 severity, const uint32 line, const char *file, const char *format, ...);
/*!
	@brief Resets the color of the console
	@returns Nothing 
*/
extern void mageLogReset();




#if defined(MAGE_DEBUG)
	#define MAGE_LOG_CORE_INFORM(x, ...) mageLogMessage(MAGE_LOG_CORE, MAGE_LOG_INFORM, __LINE__, __FILE__, x, __VA_ARGS__)
	#define MAGE_LOG_CORE_WARNING(x, ...) mageLogMessage(MAGE_LOG_CORE, MAGE_LOG_WARNING, __LINE__, __FILE__, x, __VA_ARGS__)
	#define MAGE_LOG_CORE_ERROR(x, ...) mageLogMessage(MAGE_LOG_CORE, MAGE_LOG_ERROR, __LINE__, __FILE__, x, __VA_ARGS__)
	#define MAGE_LOG_CORE_FATAL_ERROR(x, ...) mageLogMessage(MAGE_LOG_CORE, MAGE_LOG_FATAL_ERROR, __LINE__, __FILE__, x, __VA_ARGS__)
	
	#define MAGE_LOG_CLIENT_INFORM(x, ...) mageLogMessage(MAGE_LOG_CLIENT, MAGE_LOG_INFORM, __LINE__, __FILE__, x, __VA_ARGS__)
	#define MAGE_LOG_CLIENT_WARNING(x, ...) mageLogMessage(MAGE_LOG_CLIENT, MAGE_LOG_WARNING, __LINE__, __FILE__, x, __VA_ARGS__)
	#define MAGE_LOG_CLIENT_ERROR(x, ...) mageLogMessage(MAGE_LOG_CLIENT, MAGE_LOG_ERROR, __LINE__, __FILE__, x, __VA_ARGS__)
	#define MAGE_LOG_CLIENT_FATAL_ERROR(x, ...) mageLogMessage(MAGE_LOG_CLIENT, MAGE_LOG_FATAL_ERROR, __LINE__, __FILE__, x, __VA_ARGS__)
#else
	#define MAGE_LOG_CORE_INFORM(x, ...)
	#define MAGE_LOG_CORE_WARNING(x, ...)
	#define MAGE_LOG_CORE_ERROR(x, ...)
	#define MAGE_LOG_CORE_FATAL_ERROR(x, ...)
	#define MAGE_LOG_CLIENT_INFORM(x, ...)
	#define MAGE_LOG_CLIENT_WARNING(x, ...)
	#define MAGE_LOG_CLIENT_ERROR(x, ...)
	#define MAGE_LOG_CLIENT_FATAL_ERROR(x, ...)
#endif



/*!
	@brief Resizable list for storing varying amounts of one type
	@warning The list can only store 4294967295 numbers
*/
typedef struct MAGE_RESIZABLE_LIST_STRUCT
{
	/*!
		@brief Array of void pointers to the element stored
	*/
	void **Elements;
	/*!
		@brief Count of the amount of elements stored
	*/
	uint32 Quantity;
	/*! 
		@brief Size of each element in bytes 
	*/
	uint32 ElementSize;

} mageResizableList;

/*!
	@brief Allocates a block of memory for the resizable list
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
*/
extern void *mageResizableListAllocate();
/*!
	@brief Initialises the resizable list populating its members
	@param resizableList A pointer to a instance of a resizable list
	@param size Size of the element being stored in bytes
	@return Nothing
*/
extern void mageResizableListInitialise(mageResizableList *resizableList, const uint32 size);
/*!
	@brief Pushes element to the resizable list
	@param resizableList A pointer to a instance of a resizable list
	@param item A pointer to the block of memory that will be pushed to the elements
	@return Nothing
	@warning If the item's size in bytes is greater than the size previously specified errors will occur
	@warning Passing a uninitialised list will make erorrs occur
*/
extern void mageResizableListPush(mageResizableList *resizableList, void *item);
/*!
	@brief Pops the last element pushed onto the array allowing the client to handle the memory 
	@param resizableList A pointer to a instance of a resizable list
	@param buffer A pointer to a block of memory where the popped item will be moved to for handling
	@param reallocatable If true the program will resize the buffer to prevent overflow
	@return Nothing
	@warning Passing an unallocated buffer will cause a segmentation fault even if reallocatable is enabled
*/
extern void mageResizableListPop(mageResizableList *resizableList, void *buffer, const uint8 reallocatable);
/*! 
	@brief Destroys the resizable list freeing itelsf
	@param resizableList A pointer to a instance of a resizable list
	@return Nothing
	@warning If the array not allocated on the heap then do not call this method
*/
extern void mageResizableListDestroy(mageResizableList *resizableList);
/*!
	@brief Reads the files contents dumping the contents into the buffer passed int
	@param file Path to the file 
	@param buffer Pointer to a char buffer where the contents will be dumped
	@param reallocatable If true the program will resize the buffer to prevent overflow
	@param success A pointer where the success of the function will be dumped
	@return Nothing
	@warning If the buffer is too small then the data will be potentially incomplete
*/
extern void mageFileReadContents(const char *file, char *buffer, const uint8 reallocatable, uint8 *success);
/*!
	@brief Dumps the contents of a buffer to a file
	@param file Path to the file 
	@param buffer Pointer to a char buffer containing the contents to be dumped to the file
	@param clean If true the program will clean the contents of the file before dumping
	@param success A pointer where the success of the function will be dumped
	@return Nothing
*/
extern void mageFileDumpContents(const char *file, const char *buffer, const uint8 clean, uint8 *success);


/*!
	@brief Window context used by the engine to render to
	@brief The window can either use SDL2 or GLFW3 as its framework
*/
typedef struct MAGE_WINDOW_STRUCT
{
	/*!
		@brief Width of the window context
	*/
	sint32 Width;
	/*!
		@brief Height of the window context
	*/
	sint32 Height;
	/*!
		@brief Title of the window context
	*/
	const char *Title;
	
	#if defined(MAGE_SDL2)
		/*!
			@brief Window context using the SDL2 framework
		*/
		SDL_Window *Context;
		/*!
			@brief SDL2 event poller context
		*/
		SDL_Event Events;
	#elif defined(MAGE_GLFW)
		/*! 
			@brief Window context using the GLFW3 framework
		*/		
		GLFWwindow *Context;
	#endif
	
	/*!
		@brief A flag whether the window is running
	*/
	uint8 Running;	
} mageWindow;

/*! 
	@brief Allocates a block of memory for the game window
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
*/ 
extern void *mageWindowAllocate();
/*! 
	@brief Initialises the game window populating its members
	@param window A pointer to a instance of a window
	@param xResolution Width of the window created
	@param yResolution Height of thw window created
	@param title Title of the window
	@param success A pointer where the success of the function will be dumped
	@return Nothing
	@warning The window will not be constructed if the libraries fail to be initialsied
*/
extern void mageWindowInitialise(mageWindow *window, const sint32 xResolution, const sint32 yResolution, const char *title, uint8 *success);
/*!
	@brief Swaps the buffers of the window
	@param window A pointer to a instance of a window
	@return Nothing
*/
extern void mageWindowSwapBuffers(mageWindow *window);

#if defined(MAGE_GLFW)
	/*!
		@brief Updates the viewport for rendering
		@return Nothing
	*/
	extern void mageWindowResizeCallback(GLFWwindow *window, sint32 xResolution, sint32 yResolution);
#endif
/*! 
	@brief Destroys the resizable list freeing itelsf
	@param window A pointer to a instance of a window
	@return Nothing
	@warning If the window not allocated on the heap then do not call this method
*/
extern void mageWindowDestroy(mageWindow *window); 


/*! 
	@brief A struct containing system infomation used by the renderer
*/
typedef struct MAGE_SYSTEM_ENVIRONMENT_STRUCT
{
	/*!
		@brief Count of the GPU's within the system
	*/
	uint32 GPUCount;
	

} mageSystemEnvironment;

/*! 
	@brief Allocates a block of memory for the system environments
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
*/ 
extern void *mageSystemEnvironmentAllocate();
/*! 
	@brief Destroys the system environment freeing itelsf
	@param system A pointer to a instance of a system environment
	@return Nothing
	@warning If the system environments not allocated on the heap then do not call this method
*/
extern void mageSystemEnvironmentDestroy(mageSystemEnvironment *system);


#if defined(MAGE_SDL2)
	#define MAGE_KEY_A SDL_SCANCODE_A
	#define MAGE_KEY_B SDL_SCANCODE_B
	#define MAGE_KEY_C SDL_SCANCODE_C
/* GLFW keycode abstraction */
#elif defined(MAGE_GLFw)

#endif

/*! 
	@brief Gets the mouse's x and y coordinates 
	@param window A pointer to a instance of a window
	@param x A pointer to where the x coordinate will be dumped 
	@param y A pointer to where the x coordinate will be dumped 
	@return Nothing
*/
extern void mageGetMousePosition(mageWindow *window, double *x, double *y);
/*! 
	@brief Gets the mouse's x coordinate 
	@param window A pointer to a instance of a window
	@param x A pointer to where the x coordinate will be dumped 
	@return Nothing
*/
extern void mageGetMousePositionX(mageWindow *window, double *x);
/*! 
	@brief Gets the mouse's y coordinate 
	@param window A pointer to a instance of a window
	@param y A pointer to where the y coordinate will be dumped 
	@return Nothing
*/
extern void mageGetMousePositionY(mageWindow *window, double *y);
/*! 
	@brief Gets the mouse's left and right button states
	@param window A pointer to a instance of a window
	@param left A pointer to where the left button state will be dumped 
	@param right A pointer to where the right button state will be dumped 
	@return Nothing
*/
extern void mageGetMouseButtonStates(mageWindow *window, sint32 *left, sint32 *right);
/*! 
	@brief Gets whether the left mouse button is being clicked
	@param window A pointer to a instance of a window
	@param state A pointer to whether its true will be dumped 
	@return Nothing
*/
extern void mageGetMouseButtonLeftClick(mageWindow *window, uint8 *state);
/*! 
	@brief Gets whether the right mouse button is being clicked
	@param window A pointer to a instance of a window
	@param state A pointer to whether its true will be dumped 
	@return Nothing
*/
extern void mageGetMouseButtonRightClick(mageWindow *window, uint8 *state);
/*! 
	@brief Gets whether the mouse is inside the window context
	@param window A pointer to a instance of a window
	@param state A pointer to whether its true will be dumped 
	@return Nothing
*/
extern void mageGetMouseInsideContext(mageWindow *window, uint8 *state);
/*! 
	@brief Sets the mouse x and y to a specific location
	@param window A pointer to a instance of a window
	@param x Location the mouses x coordinate will be set at
	@param y Location the mouses y coordinate will be set at
	@return Nothing
*/
extern void mageSetMousePosition(mageWindow *window, const double x, const double y);
/*! 
	@brief Gets whether the key is being pressed down
	@param window A pointer to a instance of a window
	@param key A keycode for the key that is being tested
	@param state A pointer to whether its true will be dumped 
	@return Nothing
*/
extern void mageGetKeyDown(mageWindow *window, const sint32 key, uint8 *state);
/*! 
	@brief Gets whether the key is not being pressed down
	@param window A pointer to a instance of a window
	@param key A keycode for the key that is being tested
	@param state A pointer to whether its true will be dumped 
	@return Nothing
*/
extern void mageGetKeyNotDown(mageWindow *window, const sint32 key, uint8 *state);

/*! 
	@brief Stores 2 floats within the struct
*/
typedef struct MAGE_VECTOR2_STRUCT
{
	/*! 
		@brief First value
	*/
	float x;
	/*! 
		@brief Second value
	*/
	float y;

} mageVector2;

/*! 
	@brief Allocates a block of memory for the vector2
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
*/ 
extern void *mageVector2Allocate();
/*! 
	@brief Intitalises the vector2 based on the values passed 
	@param vector2 A pointer to a instance of a vector2
	@param value1 The value which x will be set to
	@param value2 The value which y will be set to
	@return Nothing
*/
extern void mageVector2Initialise(mageVector2 *vector2, const float value1, const float value2);
/*! 
	@brief Adds the right vector2 to the left vector2 
	@param left A pointer to a instance of a vector2 which will be modified
	@param right A pointer to a instance of a vector2 which will add the left
	@return Nothing 	
*/
extern void mageVector2Add(mageVector2 *left, const mageVector2 *right);
/*! 
	@brief Subtracts the right vector2 to the left vector2 
	@param left A pointer to a instance of a vector2 which will be modified
	@param right A pointer to a instance of a vector2 which will subtract the left
	@return Nothing 	
*/
extern void mageVector2Subtract(mageVector2 *left, const mageVector2 *right);
/*! 
	@brief Multiplies the right vector2 to the left vector2 
	@param left A pointer to a instance of a vector2 which will be modified
	@param right A pointer to a instance of a vector2 which will multiply the left
	@return Nothing 	
*/
extern void magemageVector2Multiply(mageVector2 *left, const mageVector2 *right);
/*! 
	@brief Divides the right vector2 to the left vector2 
	@param left A pointer to a instance of a vector2 which will be modified
	@param right A pointer to a instance of a vector2 which will divide the left
	@return Nothing 	
*/
extern void mageVector2Divide(mageVector2 *left, const mageVector2 *right);
/*! 
	@brief Destroys the vector2 freeing itelsf
	@param vector A pointer to a instance of a vector2
	@return Nothing
	@warning If the vector2 not allocated on the heap then do not call this method
*/
extern void mageVector2Destroy(mageVector2 *vector);


/*! 
	@brief Stores 3 floats within the struct
*/
typedef struct MAGE_VECTOR3_STRUCT
{
	/*! 
		@brief First value
	*/
	float x;
	/*! 
		@brief Second value 
	*/
	float y;
	/*! 
		@brief Third value
	*/
	float z;	

} mageVector3;

/*! 
	@brief Allocates a block of memory for the vector3
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
*/ 
extern void *mageVector3Allocate();
/*! 
	@brief Intitalises the vector3 based on the values passed 
	@param vector3 A pointer to a instance of a vector3
	@param value1 The value which x will be set to
	@param value2 The value which y will be set to
	@param value3 The value which z will be set to
	@return Nothing
*/
extern void mageVector3Initialise(mageVector3 *vector3, const float value1, const float value2, const float value3);
/*! 
	@brief Adds the right vector3 to the left vector3 
	@param left A pointer to a instance of a vector3 which will be modified
	@param right A pointer to a instance of a vector3 which will add the left
	@return Nothing 	
*/
extern void mageVector3Add(mageVector3 *left, const mageVector3 *right);
/*! 
	@brief Subtracts the right vector3 to the left vector3 
	@param left A pointer to a instance of a vector3 which will be modified
	@param right A pointer to a instance of a vector3 which will subtract the left
	@return Nothing 	
*/
extern void mageVector3Subtract(mageVector3 *left, const mageVector3 *right);
/*! 
	@brief Multiplies the right vector3 to the left vector3 
	@param left A pointer to a instance of a vector3 which will be modified
	@param right A pointer to a instance of a vector3 which will multiply the left
	@return Nothing 	
*/
extern void mageVector3Multiply(mageVector3 *left, const mageVector3 *right);
/*! 
	@brief Divides the right vector3 to the left vector3 
	@param left A pointer to a instance of a vector3 which will be modified
	@param right A pointer to a instance of a vector3 which will divide the left
	@return Nothing 	
*/
extern void mageVector3Divide(mageVector3 *left, const mageVector3 *right);
/*!
	@brief Destroys the vector3 freeing itelsf
	@param vector A pointer to a instance of a vector3
	@return Nothing
	@warning If the vector3 is not allocated on the heap then do not call this method
*/
extern void mageVector3Destroy(mageVector3 *vector);

/*! 
	@brief Stores 4 floats within the struct
*/
typedef struct MAGE_VECTOR4_STRUCT
{
	/*! 
		@brief First value
	*/
	float x;
	/*! 
		@brief Second value
	*/
	float y;
	/*! 
		@brief Third value
	*/
	float z;
	/*! 
		@brief Fourth value
	*/
	float w;	

} mageVector4;

/*! 
	@brief Allocates a block of memory for the vector4
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
*/ 
extern void *mageVector4Allocate();
/*! 
	@brief Intitalises the vector3 based on the values passed 
	@param vector4 A pointer to a instance of a vector4
	@param value1 The value which x will be set to
	@param value2 The value which y will be set to
	@param value3 The value which z will be set to
	@param value4 The value which w will be set to
	@return Nothing
*/
extern void mageVector4Initialise(mageVector4 *vector4, const float value1, const float value2, const float value3, const float value4);
/*! 
	@brief Intitalises the vector4 based 2 vector2's
	@param vector4 A pointer to a instance of a vector4
	@param left The vector's x and y will set the vector4's x and y 
	@param right The vector's x and y will set the vector4's z and w
	@return Nothing
*/
extern void mageVector4InitialiseVector2(mageVector4 *vector4, const mageVector2 *left, const mageVector2 *right);
/*! 
	@brief Adds the right vector4 to the left vector4 
	@param left A pointer to a instance of a vector4 which will be modified
	@param right A pointer to a instance of a vector4 which will add the left
	@return Nothing 	
*/
extern void mageVector4Add(mageVector4 *left, const mageVector4 *right);
/*! 
	@brief Subtracts the right vector4 to the left vector4 
	@param left A pointer to a instance of a vector4 which will be modified
	@param right A pointer to a instance of a vector4 which will subtract the left
	@return Nothing 	
*/
extern void mageVector4Subtract(mageVector4 *left, const mageVector4 *right);
/*! 
	@brief Multiplies the right vector4 to the left vector4 
	@param left A pointer to a instance of a vector4 which will be modified
	@param right A pointer to a instance of a vector4 which will multiply the left
	@return Nothing 	
*/
extern void mageVector4Multiply(mageVector4 *left, const mageVector4 *right);
/*! 
	@brief Divides the right vector4 to the left vector4 
	@param left A pointer to a instance of a vector4 which will be modified
	@param right A pointer to a instance of a vector4 which will divide the left
	@return Nothing 	
*/
extern void mageVector4Divide(mageVector4 *left, const mageVector4 *right);
/*!
	@brief Destroys the vector4 freeing itelsf
	@param vector A pointer to a instance of a vector4
	@return Nothing
	@warning If the vector4 is not allocated on the heap then do not call this method
*/
extern void mageVector4Destroy(mageVector4 *vector);

/*!
	@brief Object that renders and is pushed to the renderer pipeline
*/
typedef struct MAGE_RENDERABLE_STRUCT
{
	/*! 
		@brief How the renderable will scale 
	*/
	mageVector3 Scale;
	/*! 
		@brief Position in the scene
	*/
	mageVector3 Position;

} mageRenderable;

/*! 
	@brief Allocates a block of memory for the renderable
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
*/
extern void *mageRenderableAllocate();
/*! 
	@brief Intitalises the renderable based on  a scale and position
	@param renderable A pointer to a instance of a renderable
	@param scale Scale of the surface that will be rendered
	@param position Position in the world the object will be at
	@warning Negative values for the scale may cause issues
*/
extern void mageRenderableInitialse(mageRenderable *renderable, mageVector3 scale, mageVector3 position);
/*!
	@brief Destroys the renderable freeing itelsf
	@param renderable A pointer to a instance of a renderable
	@return Nothing
	@warning If the renderable is not allocated on the heap then do not call this method
*/
extern void mageRenderableDestroy(mageRenderable *renderable);

/*!
	@brief Renderer that renders objects pushed into the pipeline
*/
typedef struct MAGE_RENDERER_STRUCT
{
	/*!
		@brief Pipline where objects are pushed rendered
	*/
	mageResizableList *PipeLine;
	/*!
		@brief Environment object that stores what hardware the renderer can use
	*/
	mageSystemEnvironment Environment;

	#if defined(MAGE_VULKAN)
		/*!
			@brief Vulkan instance object
		*/
		VkInstance VulkanInstance;
		/*!
			@brief Vulkan Device object
		*/
		VkDevice VulkanDevice;
	#endif


} mageRenderer;

/*! 
	@brief Allocates a block of memory for the renderer
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
*/ 
extern void *mageRendererAllocate();
/*!
	@brief Initialises the devices for vulkan to be used
	@param renderer A pointer to a instance of a renderer
	@param success A pointer where the success of the function will be dumped
	@return Nothing
	@warning If the devices fail the renderer will terminate
*/
extern void mageRendererDeviceInitialise(mageRenderer *renderer, uint8 *success);
/*!
	@brief Initialises the renderer
	@param renderer A pointer to a instance of a renderer
	@param success A pointer where the success of the function will be dumped
	@return Nothing
*/
extern void mageRendererInitialise(mageRenderer *renderer, uint8 *success);
/*!
	@brief Destroys the vulkan devices
	@param renderer A pointer to a instance of a renderer
	@return Nothing
*/
extern void mageRendererDeviceDestroy(mageRenderer *renderer);
/*!
	@brief Destroys the renderer freeing itelsf
	@param renderer A pointer to a instance of a renderer
	@return Nothing
	@warning If the renderer is not allocated on the heap then do not call this method
*/
extern void mageRendererDestroy(mageRenderer *renderer);






#endif


