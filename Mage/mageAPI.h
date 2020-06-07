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
	Open source 2D game engine written in clean c89

**************************/

/*!************************
 * @brief A typedef for a memory allocation callback  void *func(uint64_t size)
**************************/
typedef void *(*mageAllocationCallback)(uint64_t);
/*!************************
 * @brief A typedef for a memory allocation callback  void func(void *mem)
**************************/
typedef void (*mageFreeCallback)(void *);
/*!************************
 * @brief Returned from functions and can describe the error that has occured
**************************/
typedef enum MAGE_RESULT_ENUM mageResult;
/*!************************
 * @brief The wrapper that holds the required objects needed for the engine
**************************/
typedef struct MAGE_API MAGE_APPLICATION_STRUCT mageApplication;
/*!************************
 * @brief The callback used by the application once on startup
**************************/
typedef mageResult (*mageApplicationStartCallback)(mageApplication *);
/*!************************
 * @brief The callback used by the application once on startup
**************************/
typedef void (*mageApplicationUpdateCallback)(mageApplication *);
/*!************************
 * @brief The callback used by the application once on startup
**************************/
typedef mageResult (*mageApplicationDestroyCallback)(mageApplication *);


/*!************************
 * @brief Returned from functions and can describe the error that has occured
**************************/
enum MAGE_RESULT_ENUM 
{
	/*!************************
 	 * @brief The error was undefined therefore unknown
	**************************/
	MAGE_UNKNOWN = -1,
	/*!************************
 	 * @brief The function was executed succesfully
	**************************/
	MAGE_SUCCESS,
	/*!************************
 	 * @brief Libraries were not succesfully initialised
	**************************/
	MAGE_LIBRARY_FAILURE,
	/*!************************
 	 * @brief The inputed file was not valid or unsuccesful
	**************************/
	MAGE_INVALID_INPUT,
	/*!************************
 	 * @brief The hardware that is trying to be used is invalid and cannot be used  
	**************************/
	MAGE_HARDWARE_INVALID,
	/*!************************
 	 * @brief The context that was being made was invalid or unsuccesful 
	**************************/
	MAGE_CONTEXT_CREATION_FAILED,
	/*!************************
 	 * @brief The instance that is being created had failed (USED BY THE VULKAN RENDERER MODE)
	**************************/
	MAGE_INSTANCE_CREATION_FAILURE,
	/*!************************
 	 * @brief The device that is being created had failed (USED BY THE VULKAN RENDERER MODE)
	**************************/
	MAGE_DEVICE_CREATION_FAILURE,
	/*!************************
 	 * @brief The surface that is being created had failed (USED BY THE VULKAN RENDERER MODE)
	**************************/
	MAGE_SURFACE_CREATION_FAILURE,
	/*!************************
 	 * @brief The fence that is being created had failed (USED BY THE VULKAN RENDERER MODE)
	**************************/
	MAGE_FENCE_CREATION_FAILURE,
	/*!************************
 	 * @brief The semaphore that is being created had failed (USED BY THE VULKAN RENDERER MODE)
	**************************/
	MAGE_SEMAPHORE_CREATION_FAILURE,
	/*!************************
 	 * @brief The command pool that is being created had failed (USED BY THE VULKAN RENDERER MODE)
	**************************/
	MAGE_COMMAND_POOL_CREATION_FAILURE,
	/*!************************
 	 * @brief The command buffer allocation had failed (USED BY THE VULKAN RENDERER MODE)
	**************************/
	MAGE_ALLOCATE_COMMAND_FAILURE,
	/*!************************
 	 * @brief The swapchain pool that is being created had failed (USED BY THE VULKAN RENDERER MODE)
	**************************/
	MAGE_SWAPCHAIN_CREATION_FAILED,
	/*!************************
 	 * @brief The queue submition had failed (USED BY THE VULKAN RENDERER MODE)
	**************************/
	MAGE_QUEUE_SUBMITION_FAILURE,
	/*!************************
 	 * @brief The machine has devices not present (USED BY THE VULKAN RENDERER MODE)
	**************************/
	MAGE_HARDWARE_NOT_PRESENT,
	/*!************************
 	 * @brief The application's start method had failed
	**************************/
	MAGE_START_METHOD_FAILURE,
	/*!************************
 	 * @brief The application's update method had failed
	**************************/
	MAGE_START_UPDATE_FAILURE,
	/*!************************
 	 * @brief The application's destroy method had failed
	**************************/
	MAGE_DESTROY_METHOD_FAILURE,

};

/*!************************
 * @brief Initialises the dependencies of the library and other important stuff
 * @return The success of the method
**************************/
extern MAGE_API mageResult mageEngineInitialise();
/*!************************
 * @brief Turns a result enum to string form
 * @return The string
**************************/
extern MAGE_API const char *mageToString(mageResult result);
/*!************************ 
 * @brief Free method used by the destory methods throughout the API
 * @param item pointer to a block of memory which will be freed
 * @return Nothing
**************************/
extern MAGE_API void mageFreeMethod(void *item);
/*!************************ 
 * @brief Allocation method used by the allocate methods throughout the API
 * @param size The size of the allocation in bytes
 * @return Void pointer to the block of memory allocated
**************************/
extern MAGE_API void *mageAllocationMethod(const uint64_t size);
/*!************************
 * @brief Tries to dump the contents into a pointer passed in
 * @param contents The contents to dump in
 * @param state The block of memory to dump the contents to
**************************/
extern MAGE_API void mageTryDumpSuccess(uint8_t contents, uint8_t *state);
/*!************************
 * @brief Logs to the console and writes to an output
 * @param user Core = 0 | Client = 1
 * @param severity Inform = 0 | Warning = 1 | Error = 2 | Fatal Error = 3 (causes an assert break)
 * @param line Line where it is being called from
 * @param file File where it is being called from
 * @param format Formatting using the printf style
 * @param ... Values for the formatting 
 * @return Nothing
**************************/
extern MAGE_API void mageLogMessage(const uint8_t user, const uint8_t severity, const char *format, ...);
/*!************************
 * @brief Sets up the log of the file
 * @return Nothing 
**************************/
extern void MAGE_API mageLogInitialise(const char *outputFile);
/*!************************
 * @brief Resets the color of the console
 * @return Nothing 
**************************/
extern MAGE_API void mageLogEnd();
/*!************************
 * @brief Resizable list for storing varying amounts of one type
 * @warning The list can only store 4294967295 numbers (2^32 - 1)
**************************/
typedef struct MAGE_API MAGE_RESIZABLE_LIST_STRUCT
{
	/*!************************
	 * @brief Array of void pointers to the element stored
	**************************/
	void **Elements;
	/*!************************
	 * @brief Count of the amount of elements stored
	**************************/
	uint32_t Quantity;
	/*!************************ 
	 * @brief Size of each element in bytes 
	**************************/
	uint32_t ElementSize;

} mageResizableList;

/*!************************
 * @brief Allocates a block of memory for the resizable list
 * @return Void pointer to the block of memory allocated
 * @warning The pointer has not been type casted
**************************/
extern MAGE_API void *mageResizableListAllocate();
/*!************************
 * @brief Initialises the resizable list populating its members
 * @param resizableList A pointer to a instance of a resizable list
 * @param size Size of the element being stored in bytes
 * @return Nothing
**************************/
extern MAGE_API void mageResizableListInitialise(mageResizableList *resizableList, const uint32_t size);
/*!************************
 * @brief Pushes element to the resizable list
 * @param resizableList A pointer to a instance of a resizable list
 * @param item A pointer to the block of memory that will be pushed to the elements
 * @return Nothing
 * @warning If the item's size in bytes is greater than the size previously specified errors will occur
 * @warning Passing a uninitialised list will make erorrs occur
**************************/
extern MAGE_API void mageResizableListPush(mageResizableList *resizableList, void *item);
/*!************************
 * @brief Pops the last element pushed onto the array allowing the client to handle the memory 
 * @param resizableList A pointer to a instance of a resizable list
 * @return Nothing
**************************/
extern MAGE_API void mageResizableListPop(mageResizableList *resizableList);
/*!************************ 
 * @brief Frees all the elements
 * @param resizableList A pointer to a instance of a resizable list
 * @return Nothing
**************************/
extern MAGE_API void mageResizableListFreeElements(mageResizableList *resizableList);
/*!************************ 
 * @brief Destroys the resizable list freeing itelsf
 * @param resizableList A pointer to a instance of a resizable list
 * @return Nothing
 * @warning If the array not allocated on the heap then do not call this method
**************************/
extern MAGE_API void mageResizableListDestroy(mageResizableList *resizableList);

/*!************************
 * @brief Stores a pair of values
**************************/
typedef struct MAGE_API MAGE_PAIR_STRUCT
{
	/*!************************
	 * @brief A pointer to the first value stored
	**************************/
	void *First;	
	/*!************************
	 * @brief A pointer to the second value stored
	**************************/
	void *Second;
	/*!************************
	 * @brief The size of the first value in bytes
	**************************/
	uint32_t FirstSize;
	/*!************************
	 * @brief The size of the first value in bytes
	**************************/
	uint32_t SecondSize;

} magePair;

/*!************************ 
 * @brief Allocates a block of memory for the pair
 * @return Void pointer to the block of memory allocated
 * @warning The pointer has not been type casted
**************************/
extern MAGE_API void *magePairAllocate();
/*!************************ 
 * @brief Initialises the pair populating the members
 * @param pair A pointer to a instance of a pair
 * @param firstSize Size of the first value in bytes
 * @param secondSize Size of the second value in bytes
 * @return Nothing
**************************/
extern MAGE_API void magePairInitialise(magePair *pair, const uint32_t firstSize, const uint32_t secondSize);
/*!************************ 
 * @brief Sets the first value in the pair
 * @param pair A pointer to a instance of a pair
 * @param item The item that will be pushed to the first value		
 * @return Nothing
**************************/
extern MAGE_API void magePairSetFirst(magePair *pair, void *item);
/*!************************ 
 * @brief Sets the second value in the pair
 * @param pair A pointer to a instance of a pair
 * @param item The item that will be pushed to the second value	
 * @return Nothing
 * @warning If the item passed in is bigger than the size specified then problems will occur	
**************************/
extern MAGE_API void magePairSetSecond(magePair *pair, void *item);
/*!************************ 
 * @brief Sets both values in the pair
 * @param pair A pointer to a instance of a pair
 * @param first The item that will be pushed to the first value	
 * @param second The item that will be pushed to the second value	
 * @return Nothing
**************************/
extern MAGE_API void magePairSetBoth(magePair *pair, void *first, void *second);
/*!************************ 
 * @brief Gets the first value copying it into the buffer
 * @param pair A pointer to a instance of a pair
 * @param buffer Pointer to a block of memory where the contents will be dumped
 * @param reallocatable If true the program will resize the buffer to prevent overflow
 * @return Nothing
 * @warning If reallocatable is true passing an unallocated buffer will cause a segmentation fault
**************************/
extern MAGE_API void magePairGetFist(magePair *pair, void *buffer, uint8_t reallocatable);
/*!************************ 
 * @brief Gets the second value copying it into the buffer
 * @param pair A pointer to a instance of a pair
 * @param buffer Pointer to a block of memory where the contents will be dumped
 * @param reallocatable If true the program will resize the buffer to prevent overflow
 * @return Nothing
 * @warning If reallocatable is true passing an unallocated buffer will cause a segmentation fault
**************************/
extern MAGE_API void magePairGetSecond(magePair *pair, void *buffer, uint8_t reallocatable);
/*!************************ 
 * @brief Sets both values in the pair
 * @param pair A pointer to a instance of a pair
 * @param buffer1 Pointer to a block of memory where the contents of first will be dumped
 * @param buffer2 Pointer to a block of memory where the contents of second will be dumped	
 * @param reallocatable If true the program will resize the buffer to prevent overflow
 * @return Nothing
 * @warning If reallocatable is true passing an unallocated buffer will cause a segmentation fault
**************************/
extern MAGE_API void magePairGetBoth(magePair *pair, void *buffer1, void *buffer2, uint8_t reallocatable);
/*!************************
 * @brief Resizes the memory buffer of the pair's first buffer
 * @param pair A pointer to a instance of a pair
 * @param newSize The new size of the buffer
 * @return Nothing
**************************/
extern MAGE_API void magePairResizeFirst(magePair *pair, const uint32_t newSize);
/*!************************
 * @brief Resizes the memory buffer of the pair's second buffer
 * @param pair A pointer to a instance of a pair
 * @param newSize The new size of the buffer
 * @return Nothing
**************************/
extern MAGE_API void magePairResizeSecond(magePair *pair, const uint32_t newSize);
/*!************************
 * @brief Resizes the memory buffer of the both pair's value buffers
 * @param pair A pointer to a instance of a pair
 * @param newFirstSize The new size of the first buffer
 * @param newSecondSize The new size of the second buffer
 * @return Nothing
**************************/
extern MAGE_API void magePairResizeBoth(magePair *pair, const uint32_t newFirstSize, const uint32_t newSecondSize);
/*!************************
 * @brief Frees the first and second item
 * @param pair A pointer to a instance of a pair
 * @return Nothing
**************************/
extern MAGE_API void magePairFree(magePair *pair);
/*!************************ 
 * @brief Destroys the pair freeing itelsf
 * @param pair A pointer to a instance of a pair
 * @return Nothing
 * @warning If the array not allocated on the heap then do not call this method
**************************/
extern MAGE_API void magePairDestroy(magePair *pair);

/*!************************
 * @brief Stores a list of pairs
 * @see magePair
**************************/
typedef struct MAGE_API MAGE_DICTIONARY_STRUCT
{
	/*!************************
	 * @brief List of all all the pairs being stored
	**************************/
	mageResizableList *Elements;
	
} mageDictionary;

/*!************************ 
 * @brief Allocates a block of memory for the dictionary
 * @return Void pointer to the block of memory allocated
 * @warning The pointer has not been type casted
**************************/
extern MAGE_API void *mageDictionaryAllocate();
/*!************************ 
 * @brief Initialises the dictionary
 * @param dictionary A pointer to a instance of a dictionary
 * @return Nothing
**************************/
extern MAGE_API void mageDictionaryInitialise(mageDictionary *dictionary);
/*!************************ 
 * @brief Pushes a pair to the dictionary
 * @param dictionary A pointer to a instance of a dictionary
 * @param pair A pointer to the pair to be pushed
 * @return Nothing
**************************/
extern MAGE_API void mageDictionaryPush(mageDictionary *dictionary, magePair *pair);
/*!************************
 * @brief Pops the last pair pushed
 * @param dictionary A pointer to a instance of a dictionary
 * @return Nothing
**************************/
extern MAGE_API void mageDictionaryPop(mageDictionary *dictionary);
/*!************************
 * @brief Dumps a copy of the first pair in the list
 * @param dictionary A pointer to a instance of a dictionary
 * @param buffer Where the coppy will be dumped
 * @return Nothing
**************************/
extern MAGE_API void mageDictionaryFetch(mageDictionary *dictionary, magePair *buffer);
/*!************************
 * @brief Dumps a copy of the pair specified by index in the list to the buffer
 * @param dictionary A pointer to a instance of a dictionary
 * @param buffer Where the coppy will be dumped
 * @param index The index in the list of pairs
 * @return Nothing
 * @warning Passing an invalid index will cause a memory error
**************************/
extern MAGE_API void mageDictionaryFetchIndex(mageDictionary *dictionary, magePair *buffer, const uint32_t index);
/*!************************
 * @brief Reads the files contents dumping the contents into the buffer passed int
 * @param file Path to the file 
 * @param buffer Pointer to a char buffer where the contents will be dumped
 * @param reallocatable If true the program will resize the buffer to prevent overflow
 * @return The success of the function
 * @warning If the buffer is too small then the data will be potentially incomplete
**************************/
extern MAGE_API mageResult mageFileReadContents(const char *file, char *buffer, const uint8_t reallocatable);
/*!************************
 * @brief Dumps the contents of a buffer to a file
 * @param file Path to the file 
 * @param buffer Pointer to a char buffer containing the contents to be dumped to the file
 * @param clean If true the program will clean the contents of the file before dumping
 * @return The success of the function
**************************/
extern MAGE_API mageResult mageFileDumpContents(const char *file, const char *buffer, const uint8_t clean);



/*!************************
 * @brief Window context used by the engine to render to
 * @brief The window can either use SDL2 or GLFW3 as its framework
**************************/
typedef struct MAGE_API MAGE_WINDOW_STRUCT
{
	/*!************************
	 * @brief Width of the window context
	**************************/
	int32_t Width;
	/*!************************
	 * @brief Height of the window context
	**************************/
	int32_t Height;
	/*!************************
	 * @brief Title of the window context
	**************************/
	const char *Title;
	
	#if defined(MAGE_SDL2)
		/*!************************
		 * @brief Window context using the SDL2 framework
		**************************/
		SDL_Window *Context;
		/*!************************
		 * @brief SDL2 event poller context
		**************************/
		SDL_Event Events;
	#elif defined(MAGE_GLFW)
		/*!************************ 
		 * @brief Window context using the GLFW3 framework
		**************************/		
		GLFWwindow *Context;
		/*!************************
		 * @brief Renderer used by the application
		**************************/
	#endif
	/*!************************
	 * @brief A flag whether the window is running
	**************************/
	uint8_t Running;	

} mageWindow;

/*!************************ 
 * @brief Allocates a block of memory for the game window
 * @return Void pointer to the block of memory allocated
 * @warning The pointer has not been type casted
**************************/ 
extern MAGE_API void *mageWindowAllocate();
/*!************************ 
 * @brief Initialises the game window populating its members
 * @param window A pointer to a instance of a window
 * @param xResolution Width of the window created
 * @param yResolution Height of thw window created
 * @param title Title of the window
 * @return The success of the function
**************************/
extern MAGE_API mageResult mageWindowInitialise(mageWindow *window, const int32_t xResolution, const int32_t yResolution, const char *title);
/*!************************
 * @brief Swaps the buffers of the window
 * @param window A pointer to a instance of a window
 * @return Nothing
**************************/
extern MAGE_API void mageWindowSwapBuffers(mageWindow *window);
/*!************************
 * @brief Terminates the window API
 * @param window A pointer to a instance of a window
 * @return Nothing
**************************/
extern MAGE_API void mageWindowTerminate(mageWindow *window);
/*!************************ 
 * @brief Destroys the resizable list freeing itelsf
 * @param window A pointer to a instance of a window
 * @return Nothing
 * @warning If the window not allocated on the heap then do not call this method
**************************/
extern MAGE_API void mageWindowDestroy(mageWindow *window); 
/*!************************ 
 * @brief Sets up the input handle 
 * @param window A pointer to a instance of a window
 * @return Nothing
**************************/
extern MAGE_API void mageInputIntialise(mageWindow *window);
/*!************************ 
 * @brief Gets the mouse's x coordinate 
 * @param window A pointer to a instance of a window
 * @return The X coordinate
**************************/
extern MAGE_API double mageGetMousePositionX(mageWindow *window);
/*!************************ 
 * @brief Gets the mouse's y coordinate 
 * @param window A pointer to a instance of a window
 * @return The y coordinate
**************************/
extern MAGE_API double mageGetMousePositionY(mageWindow *window);
/*!************************ 
 * @brief Gets whether the left mouse button is being clicked
 * @param window A pointer to a instance of a window 
 * @return Whether the button is being clicked
**************************/
extern MAGE_API uint8_t mageGetMouseButtonLeftClick(mageWindow *window);
/*!************************ 
 * @brief Gets whether the right mouse button is being clicked
 * @param window A pointer to a instance of a window 
 * @return Whether the button is being clicked
**************************/
extern MAGE_API uint8_t mageGetMouseButtonRightClick(mageWindow *window);
/*!************************ 
 * @brief Gets whether the mouse is inside the window context
 * @param window A pointer to a instance of a window
 * @return Whether it is in the context
**************************/
extern MAGE_API uint8_t mageGetMouseInsideContext(mageWindow *window);
/*!************************ 
 * @brief Sets the mouse x and y to a specific location
 * @param window A pointer to a instance of a window
 * @param x Location the mouses x coordinate will be set at
 * @param y Location the mouses y coordinate will be set at
 * @return Nothing
**************************/
extern MAGE_API void mageSetMousePosition(mageWindow *window, const double x, const double y);
/*!************************ 
 * @brief Gets whether the key is being pressed down
 * @param window A pointer to a instance of a window
 * @param key A keycode for the key that is being tested
 * @return Whether the key is down
**************************/
extern MAGE_API uint8_t mageGetKeyDown(mageWindow *window, const int32_t key);
/*!************************ 
 * @brief Gets whether the key is not being pressed down
 * @param window A pointer to a instance of a window
 * @param key A keycode for the key that is being tested
 * @return Whether the key is not down
**************************/
extern MAGE_API uint8_t mageGetKeyNotDown(mageWindow *window, const int32_t key);


/*!************************
 * @brief Hanldes vulkan stuff
**************************/
typedef struct MAGE_API MAGE_VULKAN_HANDLER_STRUCT
{
	#if defined(MAGE_VULKAN)

		VkViewport Viewport;
		
		VkQueue Queue;

		VkCommandBuffer CommandBuffer[2];

		VkSwapchainKHR SwapChain;
		
		VkFormat Format;

		VkPhysicalDeviceMemoryProperties MemoryProperties;

		VkSurfaceCapabilitiesKHR SurfaceCapabilities;
		
		VkSurfaceFormatKHR SurfaceFormat;

		VkDevice Device;

		VkPhysicalDevice PhysicalDevice;

		VkPhysicalDeviceProperties PhysicalProperties;

		VkCommandPool CommandPool;
	
		VkSemaphore Semaphore;

		VkSurfaceKHR Surface;

		VkQueue GraphicsQueue;

		VkFence Fence; 

		VkInstance Instance;

		VkImage *SwapChainImages;

		VkImageView *SwapChainImageViews;

		uint32_t GraphicsFamilyIndex;

		uint32_t GraphicsPresentFamily;

		uint32_t SwapChainImageCount;

	#endif

} mageVulkanHandler;

/*!************************ 
 * @brief Allocates a block of memory for the handler
 * @return Void pointer to the block of memory allocated
 * @warning The pointer has not been type casted
**************************/
extern MAGE_API void *mageVulkanHandlerAllocate();
/*!************************
 * @brief Sets up the vulkan renderer
 * @param handler A pointer to a instance of a vulkan handler
 * @param window A pointer to a instance of a window
 * @return The success of the method
**************************/
extern MAGE_API mageResult mageVulkanHandlerInitialise(mageVulkanHandler *handler, mageWindow *window); 
/*!************************
 * @brief Destroys the vulkan devices
 * @param handler A pointer to a instance of a vulkan handler
 * @return Nothing
**************************/
extern MAGE_API void mageVulkanHandlerCleanup(mageVulkanHandler *handler);

/*!************************
 * @brief Renderer that renders objects pushed into the pipeline
**************************/
typedef struct MAGE_API MAGE_RENDERER_STRUCT
{
	#if defined(MAGE_VULKAN)

		/*!************************
		 * @brief Handler for the vulkan setup
		**************************/
		mageVulkanHandler Handler;
	
	#endif

} mageRenderer;

/*!************************ 
 * @brief Allocates a block of memory for the renderer
 * @return Void pointer to the block of memory allocated
 * @warning The pointer has not been type casted
**************************/ 
extern MAGE_API void *mageRendererAllocate();
/*!************************
 * @brief Initialises the renderer
 * @param renderer A pointer to a instance of a renderer
 * @param window A pointer to a instance of a window
 * @return The success of the method
**************************/
extern MAGE_API mageResult mageRendererInitialise(mageRenderer *renderer, mageWindow *window);
/*!************************
 * @brief Destroys the renderer freeing itelsf
 * @param renderer A pointer to a instance of a renderer
 * @return Nothing
 * @warning If the renderer is not allocated on the heap then do not call this method
**************************/
extern MAGE_API void mageRendererDestroy(mageRenderer *renderer);
/*!************************
 * @brief Camera used for the scene
**************************/

/*!************************
 * @brief Constructer used for the application
**************************/
typedef struct MAGE_API MAGE_APPLICATION_PROPS_STRUCT
{
	/*!************************
	 * @brief Application version
	**************************/
	double Version;
	/*!************************
	 * @brief The width of the applications window
	**************************/
	uint32_t Width;
	/*!************************
	 * @brief The height of the applications window
	**************************/
	uint32_t Height;
	/*!************************
	 * @brief The name of the application 
	**************************/
	char *Name;
	/*!************************
 	 * @brief The method at called once at the start of the game loop
	**************************/
	mageApplicationStartCallback StartMethod;
	/*!************************
 	 * @brief The method at called every frame at during the game loop
	**************************/
	mageApplicationUpdateCallback UpdateMethod;
	/*!************************
 	 * @brief The method at called every frame at during the game loop
	**************************/
	mageApplicationDestroyCallback DestroyMethod;
	/*!************************
 	 * @brief The free method used by the application
	**************************/
	mageFreeCallback FreeCallback;
	/*!************************
 	 * @brief The free method used by the application
	**************************/
	mageAllocationCallback AllocationCallback;


} mageApplicationProps;

struct MAGE_APPLICATION_STRUCT
{
	/*!************************
 	 * @brief The renderer used by the application
	**************************/
	mageRenderer *Renderer;
	/*!************************
 	 * @brief The window used by the application
	**************************/
	mageWindow *Window;
	/*!************************
	 * @brief A pointer to an instance of the application props
	**************************/
	mageApplicationProps Props;
	/*!************************
 	 * @brief Flag whether the application is running
	**************************/
	uint8_t Running;
};	

/*!************************ 
 * @brief Allocates a block of memory for the application
 * @return Void pointer to the block of memory allocated
 * @warning The pointer has not been type casted
**************************/ 
extern MAGE_API void *mageApplicationAllocate();
/*!************************
 * @brief Initialises the application
 * @param application A pointer to a instance of a application
 * @param props The props used to construct the application and its members
 * @return The success of the method 
**************************/
extern MAGE_API mageResult mageApplicationInitialise(mageApplication *application, const mageApplicationProps *props);
/*!************************
 * @brief Runs the application
 * @param application A pointer to a instance of a application
 * @return The result of the application's runtime
**************************/
extern MAGE_API mageResult mageApplicationRun(mageApplication *application);
/*!************************
 * @brief Destroys the application
 * @param application A pointer to a instance of a application
 * @return Nothing
**************************/
extern MAGE_API void mageApplicationDestroy(mageApplication *application);


#if defined(MAGE_MONO_EXTERNALS)

	typedef struct MAGE_MONO_HANDLER_STRUCT
	{
		/*!************************
		 * @brief The domain used by mono
		**************************/
		MonoDomain *Domain;
		/*!************************
		 * @brief The assembler used by mono
		**************************/
		MonoAssembly *Assembler;
		/*!************************
		 * @brief The image used by mono
		**************************/
		MonoImage *Image;
		

	} mageMonoHandler;

	/*!************************ 
	 * @brief Allocates a block of memory for the handler
	 * @return Void pointer to the block of memory allocated
	 * @warning The pointer has not been type casted
	**************************/ 
	extern MAGE_API void *mageMonoHandlerAllocate();
	/*!************************ 
	 * @brief Sets up the mono for runtime use
	 * @param handler A pointer to a instance of a mono handler
	 * @param builtLibrary The name of the build dll from the mono compiler
	 * @return The result of the function
	**************************/ 
	mageResult mageMonoHandlerInitialise(mageMonoHandler *handler, const char *builtLibrary);
	/*!************************ 
	 * @brief Sets up the mono for runtime use
	 * @param monoClass The class that the method is in
	 * @param name The name of the method inside the class
	 * @return The method
	**************************/ 
	MonoMethod *mageMonoHandlerFindMethod(MonoClass *monoClass, const char *name);

#endif


#endif  


