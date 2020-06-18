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
	To contribute go to https://github.com/MTECGamesStudio/MAGE-Engine
	For use please read the license

**************************/

	
/*!************************
 * @brief The wrapper that holds the required objects needed for the engine
**************************/
struct mageApplication;
/*!************************
 * @brief The props used to contruct the engine
**************************/
struct mageApplicationProps;
/*!************************
 * @brief Window context used by the engine to render to
 * @brief The window can either use SDL2 or GLFW3 as its framework
**************************/
struct mageWindow;
/*!************************
 * @brief Handles the mono implimentation of the mage engine 
**************************/
struct mageMonoHandler;
/*!************************
 * @brief Renderer that renders objects pushed into the pipeline
**************************/
struct mageRenderer;
/*!************************
 * @brief Hanldes vulkan stuff
**************************/
struct mageVulkanHandler;
	
/*!************************
 * @brief Returned from functions and can describe the error that has occured
**************************/
typedef enum MAGE_RESULT_ENUM mageResult;
/*!************************
 * @brief Each event has a type and listeners can listen for specific events
**************************/
typedef enum MAGE_EVENT_ENUM mageEventType;
/*!************************
 * @brief The category of the event for listeners to handle
 * @brief Each category represents a bit and events can have multiple categories
**************************/
typedef enum MAGE_EVENT_CATEGORY_BITS_ENUM mageEventCategoryBit;
/*!************************
 * @brief An enum of the required sizes to store an event package along with the data
**************************/
typedef enum MAGE_EVENT_REQUIRED_BYTE_SIZE_ENUM mageEventRequiredByteSize;

/*!************************
 * @brief The callback used by the application once on startup
**************************/
typedef mageResult (*mageApplicationStartCallback)(struct mageApplication *);
/*!************************
 * @brief The callback used by the application once on startup
**************************/
typedef void (*mageApplicationUpdateCallback)(struct mageApplication *);
/*!************************
 * @brief The callback used by the application once on startup
**************************/
typedef mageResult (*mageApplicationDestroyCallback)(struct mageApplication *);
/*!************************
 * @brief The callback used to register and event listener
**************************/
typedef void (*mageEventListenerCallback)(void *, mageEventType);


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
 	 * @brief The image that is being created had failed (USED BY THE VULKAN RENDERER MODE)
	**************************/
	MAGE_IMAGE_CREATION_FAILURE,
	/*!************************
 	 * @brief The image that is being created had failed (USED BY THE VULKAN RENDERER MODE)
	**************************/
	MAGE_IMAGE_VIEW_CREATION_FAILURE,
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
	MAGE_UPDATE_FAILURE,
	/*!************************
 	 * @brief The application's destroy method had failed
	**************************/
	MAGE_DESTROY_METHOD_FAILURE,

};

/*!************************
 * @brief Initialises the dependencies of the library and other important stuff
 * @return The success of the method
**************************/
extern mageResult mageEngineInitialise();
/*!************************
 * @brief Turns a result enum to string form
 * @return The string
**************************/
extern const char *mageToString(mageResult result);
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
extern void mageLogMessage(const uint8_t user, const uint8_t severity, const char *format, ...);
/*!************************
 * @brief Sets up the log of the file
 * @return Nothing 
**************************/
extern void mageLogInitialise(const char *outputFile);
/*!************************
 * @brief Resets the color of the console
 * @return Nothing 
**************************/
extern void mageLogEnd();
/*!************************
 * @brief Reads the files contents dumping the contents into the buffer passed int
 * @param file Path to the file 
 * @param buffer Pointer to a char buffer where the contents will be dumped
 * @param reallocatable If true the program will resize the buffer to prevent overflow
 * @return The success of the function
 * @warning If the buffer is too small then the data will be potentially incomplete
**************************/
extern mageResult mageFileReadContents(const char *file, char *buffer, const uint8_t reallocatable);
/*!************************
 * @brief Dumps the contents of a buffer to a file
 * @param file Path to the file 
 * @param buffer Pointer to a char buffer containing the contents to be dumped to the file
 * @param clean If true the program will clean the contents of the file before dumping
 * @return The success of the function
**************************/
extern mageResult mageFileDumpContents(const char *file, const char *buffer, const uint8_t clean);



/*!************************
 * @brief Window context used by the engine to render to
 * @brief The window can either use SDL2 or GLFW3 as its framework
**************************/
struct mageWindow
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
	
	#if defined (MAGE_SDL)
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

};

/*!************************ 
 * @brief Allocates a block of memory for the game window
 * @return Void pointer to the block of memory allocated
 * @warning The pointer has not been type casted
**************************/ 
extern void *mageWindowAllocate();
/*!************************ 
 * @brief Initialises the game window populating its members
 * @param window A pointer to a instance of a window
 * @param xResolution Width of the window created
 * @param yResolution Height of thw window created
 * @param title Title of the window
 * @return The success of the function
**************************/
extern mageResult mageWindowInitialise(struct mageWindow *window, const int32_t xResolution, const int32_t yResolution, const char *title);
/*!************************
 * @brief Terminates the window API
 * @param window A pointer to a instance of a window
 * @return Nothing
**************************/
extern void mageWindowTerminate(struct mageWindow *window);


/*!************************
 * @brief Each event has a type and listeners can listen for specific events
**************************/
enum MAGE_EVENT_ENUM
{
	/*!************************
	 * @brief An event that has no type and will not be dispatched to any listeners
	**************************/
	MAGE_NONE_EVENT = 0,
	/*!************************
	 * @brief Event when the window has closed (shut down)
	**************************/
	MAGE_WINDOW_CLOSE_EVENT, 
	/*!************************
	 * @brief Event when the window has been focused and is now polling
	**************************/
	MAGE_WINDOW_FOCUS_EVENT, 
	/*!************************
	 * @brief Event when the window is losted focus and in no longer polling
	**************************/
	MAGE_WINDOW_LOST_FOCUS_EVENT, 
	/*!************************
	 * @brief Event when the window has moved along the desktop
	**************************/
	MAGE_WINDOW_MOVED_EVENT,
	/*!************************
	 * @brief Event of an application tick (1 per second)
	**************************/
	MAGE_APPLICATION_TICK_EVENT, 
	/*!************************
	 * @brief Event when the application updates (not the same as tick)
	**************************/
	MAGE_APPLICATION_UPDATE_EVENT, 
	/*!************************
	 * @brief Event once a round of rendering is done
	**************************/
	MAGE_APPLICATION_RENDER_EVENT,
	/*!************************
	 * @brief Event when a key is pressed
	**************************/
	MAGE_KEY_PRESSED_EVENT, 
	/*!************************
	 * @brief Event when a key is released
	**************************/
	MAGE_KEY_RELEASED_EVENT,
	/*!************************
	 * @brief Event when a key is repeated
	**************************/
	MAGE_KEY_REPEAT_EVENT,
	/*!************************
	 * @brief Event when the button is pressed
	**************************/
	MAGE_MOUSE_BUTTON_PRESSED_EVENT, 
	/*!************************
	 * @brief Event when a button is released
	**************************/
	MAGE_MOUSE_BUTTON_RELEASED_EVENT, 
	/*!************************
	 * @brief Event when the mouse cursor has moved
	**************************/
	MAGE_MOUSE_MOVED_EVENT, 
	/*!************************
	 * @brief Event when the mouse wheel is scrolled up or down
	**************************/
	MAGE_MOUSE_SCROLLED_EVENT,

};

enum MAGE_EVENT_REQUIRED_BYTE_SIZE_ENUM
{
	/*!************************
	 * @brief Byte size of a none event
	**************************/
	MAGE_NONE_EVENT_BYTE_SIZE 				= 0,
	/*!************************
	 * @brief Byte size of an window close event
	**************************/
	MAGE_WINDOW_CLOSE_EVENT_BYTE_SIZE 			= sizeof(int16_t),
	/*!************************
	 * @brief Byte size of an window focus event
	**************************/
	MAGE_WINDOW_FOCUS_EVENT_BYTE_SIZE 			= sizeof(uint16_t),
	/*!************************
	 * @brief Byte size of an window focus event
	**************************/
	MAGE_WINDOW_LOST_FOCUS_EVENT_BYTE_SIZE		= sizeof(uint16_t),
	/*!************************
	 * @brief Byte size of an window focus event
	 * @brief Byte 3 - 6: X position of the window (int32_t)
	 * @brief Byte 7 - 10 Y position of the window (int32_t)
	**************************/
	MAGE_WINDOW_MOVED_EVENT_BYTE_SIZE 			= sizeof(uint16_t) + (sizeof(uint32_t) * 2),
	/*!************************
	 * @brief Byte size of an application tick event
	 * @brief Byte 3 - 6: The current tick of the application (uint32_t)
	**************************/
	MAGE_APPLICATION_TICK_EVENT_BYTE_SIZE 		= sizeof(uint16_t) + sizeof(int32_t),
	/*!************************
	 * @brief Byte size of an application update event
	**************************/
	MAGE_APPLICATION_UPDATE_EVENT_BYTE_SIZE 	= sizeof(uint16_t),
	/*!************************
	 * @brief Byte size of an application render update event
	**************************/
	MAGE_APPLICATION_RENDER_EVENT_BYTE_SIZE 	= sizeof(uint16_t),
	/*!************************
	 * @brief Byte size of an key pressent event
	 * @brief Byte 3 -> 4 The keycode of the key (last bit unused) (uint8_t)
	**************************/
	MAGE_KEY_PRESSED_EVENT_BYTE_SIZE 			= sizeof(uint16_t) + sizeof(uint8_t),
	/*!************************
	 * @brief Byte size of an key repeat event
	 * @brief Byte 3 -> 4 The keycode of the key (last bit unused) (uint8_t)
	**************************/
	MAGE_KEY_REPEAT_EVENT_BYTE_SIZE				= sizeof(uint16_t) + sizeof(uint8_t),
	/*!************************
	 * @brief Byte size of an key released event
	 * @brief Byte 3 -> 4 The keycode of the key (last bit unused) (uint8_t)
	**************************/
	MAGE_KEY_RELEASED_EVENT_BYTE_SIZE 			= sizeof(uint16_t) + sizeof(uint8_t),
	/*!************************
	 * @brief Byte size of an mouse button pressed event
	 * @brief Byte 3 -> 4 The mousecode of the button (last 5 bits unused) (uint8_t)
	**************************/
	MAGE_MOUSE_BUTTON_PRESSED_EVENT_BYTE_SIZE 	= sizeof(uint16_t) + sizeof(uint8_t),
	/*!************************
	 * @brief Byte size of an mouse button released event
	 * @brief Byte 3 -> 4 The mousecode of the button (last 5 bits unused) (uint8_t)
	**************************/
	MAGE_MOUSE_BUTTON_RELEASED_EVENT_BYTE_SIZE 	= sizeof(uint16_t) + sizeof(uint8_t),
	/*!************************
	 * @brief Byte size of an mouse moved event
	 * @brief Byte 3 -> 10 The mouse x position (double)
	 * @brief Byte 11 -> 18 The mouse y position (double) 
	**************************/
	MAGE_MOUSE_MOVED_EVENT_BYTE_SIZE 			= sizeof(uint16_t) + (sizeof(double) * 2),
	/*!************************
	 * @brief Byte size of an mouse moved event
	 * @brief Byte 3 -> 10 The wheel x offset (double)
	 * @brief Byte 11 -> 18 The mouse y offset (double) 
	**************************/
	MAGE_MOUSE_SCROLLED_EVENT_BYTE_SIZE 		= sizeof(uint16_t) + (sizeof(double) * 2),
};

enum MAGE_EVENT_CATEGORY_BITS_ENUM
{
	/*!************************
	 * @brief Events with not category, usually a red flag
	**************************/
	MAGE_NONE_CATEGORY 				= -1,
	/*!************************
	 * @brief Events around the application, ticks and render cycles
	**************************/
	MAGE_APPLICATION_CATEGORY		= 4,
	/*!************************
	 * @brief Events around input, mouse movement, key presses and mouse buttons being pressed
	**************************/
	MAGE_INPUT_CATEGORY				= 5,
	/*!************************
	 * @brief Events based on keyboard inputs and presses
	**************************/
	MAGE_KEYBOARD_CATEGORY 			= 6,
	/*!************************
	 * @brief Events based on the mouse, movement and button presses
	**************************/
	MAGE_MOUSE_CATEGORY				= 7,
	/*!************************
	 * @brief Events based on the mouse buttons, presses and states
	**************************/
	MAGE_MOUSE_BUTTON_CATEGORY		= 8,

};

/*!************************
 * @brief Sets up the callbacks for the window input for use 
 * @param window A pointer to the instance of the window structure
 * @return Nothing
**************************/
void mageInputSetup(struct mageWindow *window);
/*!************************
 * @brief Sets up the event system for use
 * @param callback An array of functions listeners for events
 * @param callbackCount The amount of listeners passed in
 * @return The success of the function
**************************/
void mageEventSetupMaster();
/*!************************
 * @brief Creates and formats the bits for the event handle and package
 * @param type The type of the event 
 * @return The formatted binary 
**************************/
extern uint16_t mageEventHandleCreate(const mageEventType type);
/*!************************
 * @brief Creates an array of the catagories that an event type lives in
 * @param type The type of the event 
 * @return Array of categories 
**************************/
extern mageEventCategoryBit *mageEventGenerateCategories(const mageEventType type);
/*!************************
 * @brief Interrogates the binary to find what event type it is 
 * @param handle The formatted binary
 * @return The event type
**************************/
extern mageEventType mageEventExtractEventType(const uint16_t handle);
/*!************************
 * @brief Checks whether the handle is in category
 * @param handle A formated event handle 
 * @param category The category to check for
 * @return Whether it is in the category
**************************/
extern uint8_t mageEventInCategory(const uint16_t handle, const mageEventCategoryBit category);
/*!************************
 * @brief Adds an event listen callback and is sent the binary of each event
 * @param callback The function to be run when the event is found
 * @return Nothing
**************************/
extern void mageEventRegisterListener(mageEventListenerCallback callback);
/*!************************
 * @brief Formats the bytes 1 - 2 to hold data related to the to the window close event
 * @param buffer Where the 2 bytes will be formatted
 * @warning The required buffer size is 2 bytes, if less then we are going to have a bad time
 * @return Nothing
**************************/
extern void mageEventFormatWindowClose(void *buffer);
/*!************************
 * @brief Formats the bytes 1 - 2 to hold data related to the window focused event
 * @param buffer Where the 2 bytes will be formatted
 * @warning The required buffer size is 2 bytes, if less then we are going to have a bad time
 * @return Nothing
**************************/
extern void mageEventFormatWindowFocus(void *buffer);
/*!************************
 * @brief Formats the bytes 1 - 2 to hold data related to the window lost focus event
 * @param buffer Where the 2 bytes will be formatted
 * @warning The required buffer size is 2 bytes, if less then we are going to have a bad time
 * @return Nothing
**************************/
extern void mageEventFormatWindowLostFocus(void *buffer);
/*!************************
 * @brief Formats the bytes 1 - 10 to hold data related to the window moved event
 * @param buffer Where the 10 bytes will be formatted
 * @param x The x position of the window
 * @param y The y position of the window
 * @warning The required buffer size is 10 bytes, if less then we are going to have a bad time
 * @return Nothing
**************************/
extern void mageEventFormatWindowMoved(void *buffer, const int32_t x, const int32_t y);
/*!************************
 * @brief Formats the bytes 1 - 2 to hold data related to the keycode of the key pressed event
 * @param buffer Where the 3 bytes will be formatted
 * @param keycode The keycode that is being pressed
 * @warning The required buffer size is 2 bytes, if less then we are going to have a bad time
 * @return Nothing
**************************/
extern void mageEventFormatKeyPressed(void *buffer, const uint8_t keycode);
/*!************************
 * @brief Formats the bytes 1 - 2 to hold data related to the keycode of the key released event
 * @param buffer Where the 3 bytes will be formatted
 * @param keycode The keycode that is being pressed
 * @warning The required buffer size is 3 bytes, if less then we are going to have a bad time
 * @return Nothing
**************************/
extern void mageEventFormatKeyReleased(void *buffer, const uint8_t keycode);
/*!************************
 * @brief Formats the bytes 1 - 2 to hold data related to the keycode of the key repeat event
 * @param buffer Where the 3 bytes will be formatted
 * @param keycode The keycode that is being pressed
 * @warning The required buffer size is 3 bytes, if less then we are going to have a bad time
 * @return Nothing
**************************/
extern void mageEventFormatKeyRepeat(void *buffer, const uint8_t keycode);
/*!************************
 * @brief Formats the bytes 1 - 2 to hold data related to the mousecode of the mouse press event
 * @param buffer Where the 3 bytes will be formatted
 * @param mousecode The mousecode that is being pressed
 * @warning The required buffer size is 3 bytes, if less then we are going to have a bad time
 * @return Nothing
**************************/
extern void mageEventFormatMouseButtonPressed(void *buffer, const uint8_t mousecode);
/*!************************
 * @brief Formats the bytes 1 - 2 to hold data related to the mousecode of the mouse release event
 * @param buffer Where the 3 bytes will be formatted
 * @param mousecode The mousecode that is being pressed
 * @warning The required buffer size is 3 bytes, if less then we are going to have a bad time
 * @return Nothing
**************************/
extern void mageEventFormatMouseButtonRelease(void *buffer, const uint8_t mousecode);
/*!************************
 * @brief Formats the bytes 1 - 18 to hold data related the mouse positions of the mouse event
 * @param buffer Where the 18 bytes will be formatted
 * @param x The x position of the mouse
 * @param y The y position of the mouse
 * @warning The required buffer size is 18 bytes, if less then we are going to have a bad time
 * @return Nothing
**************************/
extern void mageEventFormatMouseMoved(void *buffer, const double x, const double y);
/*!************************
 * @brief Formats the bytes 1 - 18 to hold data related the mouse wheel positions of the event
 * @param buffer Where the 18 bytes will be formatted
 * @param x If the middle mouse button is being pressed when scrolling the wheel
 * @param y The x & y position of the mouse wheel
 * @warning The required buffer size is 18 bytes, if less then we are going to have a bad time
 * @return Nothing
**************************/
extern void mageEventFormatMouseWheelMoved(void *buffer, const double x, const double y);
/*!************************
 * @brief Dispatches an event to the master handler
 * @param event A pointer to the memory that the event stores
 * @warning The first 2 bytes of the event should be formated  
 * @warning By dispatching an event the event conditions are assumed to be met
 * @return Nothing
**************************/
extern void mageEventDispatch(void *event);



#if defined (MAGE_VULKAN)

	struct mageVulkanHandler
	{	
		VkPhysicalDeviceMemoryProperties 		PhysicalMemoryProperties;
		VkSurfaceCapabilitiesKHR 				SurfaceCapabilities;
		VkSurfaceFormatKHR 						SurfaceFormat;
		VkDevice 								Device;
		VkPhysicalDevice 						PhysicalDevice;
		VkPhysicalDeviceProperties 				PhysicalProperties;
		VkInstance 								Instance;
		#if defined (MAGE_DEBUG) || defined (CLIENT_DEBUG)
			VkDebugUtilsMessengerEXT			DebugMessenger;
			VkDebugUtilsMessengerCreateInfoEXT	DebugMessengerCreateInformation;
		#endif
		
		uint32_t						 		DephStencilAvailable;
		uint32_t						 		GraphicsFamilyIndex;
		uint32_t 								GraphicsPresentFamily;
		
	};

	/*!************************ 
	 * @brief Allocates a block of memory for the handler
	 * @return Void pointer to the block of memory allocated
	 * @warning The pointer has not been type casted
	**************************/
	extern void *mageVulkanHandlerAllocate();
	/*!************************
	 * @brief Sets up the vulkan renderer
	 * @param handler A pointer to a instance of a vulkan handler
	 * @param window A pointer to a instance of a window
	 * @return The success of the method
	**************************/
	extern mageResult mageVulkanHandlerInitialise(struct mageVulkanHandler *handler, struct mageWindow *window); 
	/*!************************
	 * @brief Destroys the vulkan devices
	 * @param handler A pointer to a instance of a vulkan handler
	 * @return Nothing
	**************************/
	extern void mageVulkanHandlerCleanup(struct mageVulkanHandler *handler);

#endif

/*!************************
 * @brief Renderer that renders objects pushed into the pipeline
**************************/
struct mageRenderer
{
	#if defined (MAGE_VULKAN)
		/*!************************
		 * @brief Handler for the vulkan setup
		**************************/
		struct mageVulkanHandler 	Handler;
		/*!************************
		 * @brief The graphics queue for the rendering
		**************************/
		VkQueue 					GraphicsQueue;
		/*!************************
		 * @brief The command buffer including a primary and secondary
		**************************/
		VkCommandBuffer				CommandBuffer[2];
		/*!************************
		 * @brief The main viewport of the surface
		**************************/
		VkViewport 					Viewport;
		/*!************************
		 * @brief The window surface used to render to
		**************************/
		VkSurfaceKHR 				Surface;
		/*!************************
		 * @brief A depth stencil used for rendering
		**************************/
		VkImage 					DepthStencilImage;
		/*!************************
		 * @brief The format of the depth stencil
		**************************/
		VkFormat					DepthStencilFormat;
		/*!************************
		 * @brief The image view of the depth stencil
		**************************/
		VkImageView 				DepthStencilImageView;
		/*!************************
		 * @brief The command pool used by the renderer
		**************************/
		VkCommandPool 				CommandPool;
		/*!************************
		 * @brief The swapchain of the renderer
		**************************/
		VkSwapchainKHR 				SwapChain;
		/*!************************
		 * @brief An array of swap chain images
		**************************/
		VkImage 				   	*SwapChainImages;
		/*!************************
		 * @brief An array of the swap chain image views
		**************************/
		VkImageView 				*SwapChainImageViews;
		/*!************************
		 * @brief The semaphore used by the renderer
		**************************/
		VkSemaphore 				Semaphore;
		/*!************************
		 * @brief The fence used by the renderer
		**************************/
		VkFence 					Fence;
		/*!************************
		 * @brief Count of the swap chain images the renderer has
		**************************/
		uint32_t 					SwapChainImageCount;
	#endif

};

/*!************************ 
 * @brief Allocates a block of memory for the renderer
 * @return Void pointer to the block of memory allocated
 * @warning The pointer has not been type castedThe wrapper that holds the required objects needed for the engine
**************************/ 
extern void *mageRendererAllocate();
/*!************************
 * @brief Initialises the renderer
 * @param renderer A pointer to a instance of a renderer
 * @param window A pointer to a instance of a window
 * @return The success of the method
**************************/
extern mageResult mageRendererInitialise(struct mageRenderer *renderer, struct mageWindow *window);
/*!************************
 * @brief Destroys the renderer freeing itelsf
 * @param renderer A pointer to a instance of a renderer
 * @return Nothing
 * @warning If the renderer is not allocated on the heap then do not call this method
**************************/
extern void mageRendererDestroy(struct mageRenderer *renderer);

#if defined (MAGE_MONO_EXTERNALS)

	/*!************************
	 * @brief Handles the mono implimentation of the mage engine 
	**************************/
	struct mageMonoHandler
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
		 * @brief The assembler used by mono
		**************************/
		MonoImage *Image;
	};

	/*!************************ 
	 * @brief Allocates a block of memory for the handler
	 * @return Void pointer to the block of memory allocated
	 * @warning The pointer has not been type casted
	**************************/ 
	extern void *mageMonoHandlerAllocate();
	/*!************************ 
	 * @brief Sets up the mono for runtime use
	 * @param handler A pointer to a instance of a mono handler
	 * @param builtLibrary The name of the build dll from the mono compiler
	 * @return The result of the function
	**************************/ 
	mageResult mageMonoHandlerInitialise(struct mageMonoHandler *handler, const char *builtLibrary);
	/*!************************ 
	 * @brief Sets up the mono for runtime use
	 * @param monoClass The class that the method is in
	 * @param name The name of the method inside the class
	 * @return The method
	**************************/ 
	MonoMethod *mageMonoHandlerFindMethod(MonoClass *monoClass, const char *name);

#endif

struct mageApplicationProps
{
	/*!************************
	 * @brief Application version
	**************************/
	double 							Version;
	/*!************************
	 * @brief The width of the applications window
	**************************/
	uint32_t 						Width;
	/*!************************
	 * @brief The height of the applications window
	**************************/
	uint32_t 						Height;
	/*!************************
	 * @brief The name of the application 
	**************************/
	char 							*Name;
	/*!************************
	 * @brief The name mono dll used for scripting
	**************************/
	const char 						*ClientDLL;
	/*!************************
 	 * @brief The method at called once at the start of the game loop
	**************************/
	mageApplicationStartCallback 	StartMethod;
	/*!************************
 	 * @brief The method at called every frame at during the game loop
	**************************/
	mageApplicationUpdateCallback 	UpdateMethod;
	/*!************************
 	 * @brief The method at called every frame at during the game loop
	**************************/
	mageApplicationDestroyCallback 	DestroyMethod;
	
};

struct mageApplication
{
	struct mageRenderer 		*Renderer;
	struct mageWindow 			*Window;
	struct mageApplicationProps Props;
	#if defined (MAGE_MONO_EXTERNALS)
		struct mageMonoHandler *MonoHandler;
	#endif
	uint8_t 					Running;

};	

/*!************************ 
 * @brief Allocates a block of memory for the application
 * @return Void pointer to the block of memory allocated
 * @warning The pointer has not been type casted
**************************/ 
extern void *mageApplicationAllocate();
/*!************************
 * @brief Initialises the application
 * @param application A pointer to a instance of a application
 * @param props The props used to construct the application and its members
 * @return The success of the method 
**************************/
extern mageResult mageApplicationInitialise(struct mageApplication *application, struct mageApplicationProps props);
/*!************************
 * @brief Runs the application
 * @param application A pointer to a instance of a application
 * @return The result of the application's runtime
**************************/
extern mageResult mageApplicationRun(struct mageApplication *application);
/*!************************
 * @brief Destroys the application
 * @param application A pointer to a instance of a application
 * @return Nothing
**************************/
extern void mageApplicationDestroy(struct mageApplication *application);




#endif  


