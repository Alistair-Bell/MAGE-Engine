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

	
struct mageApplication;
struct mageApplicationProps;
struct mageWindow;
struct mageMonoHandler;
struct mageRenderer;
struct mageVulkanHandler;
	
typedef enum MAGE_RESULT_ENUM mageResult;
typedef enum MAGE_EVENT_ENUM mageEventType;
typedef enum MAGE_EVENT_CATEGORY_BITS_ENUM mageEventCategoryBit;
typedef enum MAGE_EVENT_REQUIRED_BYTE_SIZE_ENUM mageEventRequiredByteSize;

typedef mageResult  (*mageApplicationStartCallback)		(struct mageApplication *);
typedef void 		(*mageApplicationUpdateCallback)	(struct mageApplication *);
typedef mageResult  (*mageApplicationDestroyCallback)	(struct mageApplication *);
typedef void 		(*mageEventListenerCallback)		(void *, mageEventType);


enum MAGE_RESULT_ENUM 
{
	MAGE_UNKNOWN = -1,
	MAGE_SUCCESS,
	MAGE_LIBRARY_FAILURE,
	MAGE_INVALID_INPUT,
	MAGE_HARDWARE_INVALID,
	MAGE_CONTEXT_CREATION_FAILED,
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
	MAGE_QUEUE_SUBMITION_FAILURE,
	MAGE_HARDWARE_NOT_PRESENT,
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

struct mageWindow
{
	int32_t 								Width;
	int32_t 								Height;
	uint32_t 								Running;	
	const char 							   *Title;
#if defined (MAGE_SDL)
	SDL_Window						       *Context;
	SDL_Event 								Events;
#elif defined(MAGE_GLFW)
	GLFWwindow 							   *Context;
#endif
};

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
	
	#if defined (MAGE_DEBUG)
		VkDebugUtilsMessengerEXT			DebugMessenger;
		VkDebugUtilsMessengerCreateInfoEXT	DebugMessengerCreateInformation;
	#endif
	
	uint32_t						 		DephStencilAvailable;
	uint32_t						 		GraphicsFamilyIndex;
	uint32_t 								GraphicsPresentFamily;
		
};
#endif
struct mageRenderer
{
#if defined (MAGE_VULKAN)
	struct mageVulkanHandler 				Handler;
	VkQueue 								GraphicsQueue;
	VkCommandBuffer							CommandBuffer[2];
	VkViewport 								Viewport;
	VkSurfaceKHR 							Surface;
	VkImage 								DepthStencilImage;
	VkFormat								DepthStencilFormat;
	VkImageView 							DepthStencilImageView;
	VkCommandPool 							CommandPool;
	VkSwapchainKHR 							SwapChain;
	VkImage 				   				*SwapChainImages;
	VkImageView 							*SwapChainImageViews;
	VkSemaphore 							Semaphore;
	VkFence 								Fence;
	uint32_t 								SwapChainImageCount;
#endif
};
#if defined (MAGE_MONO_EXTERNALS)
struct mageMonoHandler
{
	MonoDomain 								*Domain;
	MonoAssembly 							*Assembler;
	MonoImage 								*Image;
};
#endif
struct mageApplicationProps
{
	double 									Version;
	uint32_t 								Width;
	uint32_t 								Height;
	char 						   			*Name;
	const char 					   			*ClientDLL;
	mageApplicationStartCallback 			StartMethod;
	mageApplicationUpdateCallback 			UpdateMethod;
	mageApplicationDestroyCallback 			DestroyMethod;
};
struct mageApplication
{
	struct mageRenderer 					*Renderer;
	struct mageWindow 						*Window;
	struct mageApplicationProps 			Props;
#if defined (MAGE_MONO_EXTERNALS)
	struct mageMonoHandler 					*MonoHandler;
#endif
	uint8_t 								Running;

};	


extern mageResult  						 mageEngineInitialise();
extern const char 						*mageToString(mageResult result);
extern void 	   						 mageLogMessage(const uint8_t user, const uint8_t severity, const char *format, ...);
extern void 	   						 mageLogEnd();
extern void 	   						 mageLogInitialise(const char *outputFile);
extern mageResult  						 mageFileReadContents(const char *file, char *buffer, const uint8_t reallocatable);
extern mageResult  						 mageFileDumpContents(const char *file, const char *buffer, const uint8_t clean);
extern void 	 						*mageWindowAllocate();
extern mageResult 						 mageWindowInitialise(struct mageWindow *window, const int32_t xResolution, const int32_t yResolution, const char *title);
extern void 	  						 mageWindowTerminate(struct mageWindow *window);
extern void 							 mageInputSetup(struct mageWindow *window);
extern void 							 mageEventSetupMaster();
extern mageEventCategoryBit 			*mageEventGenerateCategories(const mageEventType type);
extern uint16_t 						 mageEventHandleCreate(const mageEventType type);
extern mageEventType 					 mageEventExtractEventType(const uint16_t handle);
extern uint8_t 							 mageEventInCategory(const uint16_t handle, const mageEventCategoryBit category);
extern void 							 mageEventRegisterListener(mageEventListenerCallback callback);
extern void 							 mageEventFormatWindowClose(void *buffer);
extern void 							 mageEventFormatWindowFocus(void *buffer);
extern void 							 mageEventFormatWindowLostFocus(void *buffer);
extern void 							 mageEventFormatWindowMoved(void *buffer, const int32_t x, const int32_t y);
extern void 							 mageEventFormatKeyPressed(void *buffer, const uint8_t keycode);
extern void 							 mageEventFormatKeyReleased(void *buffer, const uint8_t keycode);
extern void 							 mageEventFormatKeyRepeat(void *buffer, const uint8_t keycode);
extern void 							 mageEventFormatMouseButtonPressed(void *buffer, const uint8_t mousecode);
extern void 							 mageEventFormatMouseButtonRelease(void *buffer, const uint8_t mousecode);
extern void 							 mageEventFormatMouseMoved(void *buffer, const double x, const double y);
extern void 							 mageEventFormatMouseWheelMoved(void *buffer, const double x, const double y);
extern void 							 mageEventDispatch(void *event);
extern void 	   						*mageVulkanHandlerAllocate();
extern mageResult 						 mageVulkanHandlerInitialise(struct mageVulkanHandler *handler, struct mageWindow *window); 
extern void 							 mageVulkanHandlerCleanup(struct mageVulkanHandler *handler);
extern void 	   						*mageRendererAllocate();
extern mageResult 						 mageRendererInitialise(struct mageRenderer *renderer, struct mageWindow *window);
extern void 							 mageRendererDestroy(struct mageRenderer *renderer);
#if defined (MAGE_MONO_EXTERNALS)
extern void 							*mageMonoHandlerAllocate();
extern mageResult 						 mageMonoHandlerInitialise(struct mageMonoHandler *handler, const char *builtLibrary);
extern MonoMethod 						*mageMonoHandlerFindMethod(MonoClass *monoClass, const char *name);
#endif
extern void		 						*mageApplicationAllocate();
extern mageResult 						 mageApplicationInitialise(struct mageApplication *application, struct mageApplicationProps props);
extern mageResult 						 mageApplicationRun(struct mageApplication *application);
extern void 	  						 mageApplicationDestroy(struct mageApplication *application);




#endif  


