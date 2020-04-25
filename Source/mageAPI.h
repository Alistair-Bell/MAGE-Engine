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
    #define MAGE_KEYCODE_SPACE SDLK_SPACE  
    #define MAGE_KEYCODE_APOSTROPHE SDLK_QUOTE  
    #define MAGE_KEYCODE_COMMA SDLK_COMMA   
    #define MAGE_KEYCODE_MINUS SDLK_MINUS   
    #define MAGE_KEYCODE_PERIOD SDLK_PERIOD   
    #define MAGE_KEYCODE_SLASH SDLK_BACKSLASH  
    #define MAGE_KEYCODE_D0 SDLK_0  
    #define MAGE_KEYCODE_D1 SDLK_1  
    #define MAGE_KEYCODE_D2 SDLK_2   
    #define MAGE_KEYCODE_D3 SDLK_3  
    #define MAGE_KEYCODE_D4 SDLK_4  
    #define MAGE_KEYCODE_D5 SDLK_5  
    #define MAGE_KEYCODE_D6 SDLK_6
    #define MAGE_KEYCODE_D7 SDLK_7  
    #define MAGE_KEYCODE_D8 SDLK_8 
    #define MAGE_KEYCODE_D9 SDLK_9   
    #define MAGE_KEYCODE_SEMICOLON SDLK_SEMICOLON   
    #define MAGE_KEYCODE_EQUAL SDLK_EQUALS
    #define MAGE_KEYCODE_A SDLK_a  
    #define MAGE_KEYCODE_B SDLK_b
    #define MAGE_KEYCODE_C SDLK_c
    #define MAGE_KEYCODE_D SDLK_d
    #define MAGE_KEYCODE_E SDLK_e
    #define MAGE_KEYCODE_F SDLK_f
    #define MAGE_KEYCODE_G SDLK_g
    #define MAGE_KEYCODE_H SDLK_h 
    #define MAGE_KEYCODE_I SDLK_i 
    #define MAGE_KEYCODE_J SDLK_j 
    #define MAGE_KEYCODE_K SDLK_k 
    #define MAGE_KEYCODE_L SDLK_l 
    #define MAGE_KEYCODE_M SDLK_m 
    #define MAGE_KEYCODE_N SDLK_n 
    #define MAGE_KEYCODE_O SDLK_o   
    #define MAGE_KEYCODE_P SDLK_p   
    #define MAGE_KEYCODE_Q SDLK_q   
    #define MAGE_KEYCODE_R SDLK_r   
    #define MAGE_KEYCODE_S SDLK_s   
    #define MAGE_KEYCODE_T SDLK_t   
    #define MAGE_KEYCODE_U SDLK_u   
    #define MAGE_KEYCODE_V SDLK_v   
    #define MAGE_KEYCODE_W SDLK_w   
    #define MAGE_KEYCODE_X SDLK_x   
    #define MAGE_KEYCODE_Y SDLK_y   
    #define MAGE_KEYCODE_Z SDLK_z   
    #define MAGE_KEYCODE_LEFTBRACKET SDLK_LEFTBRACKET   
    #define MAGE_KEYCODE_BACKSLASH SDLK_SLASH
    #define MAGE_KEYCODE_RIGHTBRAC SDLK_RIGHTBRACKET 
    #define MAGE_KEYCODE_GRAVEACC SDLK_BACKQUOTE
    #define MAGE_KEYCODE_ESCAPE SDLK_ESCAPE  
    #define MAGE_KEYCODE_ENTER SDLK_RETURN  
    #define MAGE_KEYCODE_TAB SDLK_TAB
    #define MAGE_KEYCODE_BACKSPACE SDLK_BACKSPACE 
    #define MAGE_KEYCODE_INSERT SDLK_INSERT  
    #define MAGE_KEYCODE_DELETE SDLK_DELETE 
    #define MAGE_KEYCODE_RIGHT SDLK_RIGHT
    #define MAGE_KEYCODE_LEFT SDLK_LEFT 
    #define MAGE_KEYCODE_DOWN SDLK_DOWN  
    #define MAGE_KEYCODE_UP SDLK_UP
    #define MAGE_KEYCODE_PAGEUP SDLK_PAGEUP
    #define MAGE_KEYCODE_PAGEDOWN SDLK_PAGEDOWN 
    #define MAGE_KEYCODE_HOME SDLK_HOME
    #define MAGE_KEYCODE_END SDLK_END  
    #define MAGE_KEYCODE_CAPSLOCK SDLK_CAPSLOCK 
    #define MAGE_KEYCODE_SCROLLLOCK SDLK_SCROLLLOCK  
    #define MAGE_KEYCODE_NUMLOCK SDLK_NUMLOCKCLEAR 
    #define MAGE_KEYCODE_PRINTSCREEN SDLK_PRINTSCREEN  
    #define MAGE_KEYCODE_PAUSE SDLK_PAUSE   
    #define MAGE_KEYCODE_F1 SDLK_F1
    #define MAGE_KEYCODE_F2 SDLK_F2
    #define MAGE_KEYCODE_F3 SDLK_F3
    #define MAGE_KEYCODE_F4 SDLK_F4 
    #define MAGE_KEYCODE_F5 SDLK_F5
    #define MAGE_KEYCODE_F6 SDLK_F6
    #define MAGE_KEYCODE_F7 SDLK_F7
    #define MAGE_KEYCODE_F8 SDLK_F8
    #define MAGE_KEYCODE_F9 SDLK_F9 
    #define MAGE_KEYCODE_F10 SDLK_F10   
    #define MAGE_KEYCODE_F11 SDLK_F11
    #define MAGE_KEYCODE_F12 SDLK_F12  
    #define MAGE_KEYCODE_F13 SDLK_F13
    #define MAGE_KEYCODE_F14 SDLK_F14
    #define MAGE_KEYCODE_F15 SDLK_F15
    #define MAGE_KEYCODE_F16 SDLK_F16
    #define MAGE_KEYCODE_F17 SDLK_F17
    #define MAGE_KEYCODE_F18 SDLK_F18 
    #define MAGE_KEYCODE_F19 SDLK_F19 
    #define MAGE_KEYCODE_F20 SDLK_F20   
    #define MAGE_KEYCODE_F21 SDLK_F21  
    #define MAGE_KEYCODE_F22 SDLK_F22   
    #define MAGE_KEYCODE_F23 SDLK_F23  
    #define MAGE_KEYCODE_F24 SDLK_F24     
    #define MAGE_KEYCODE_KP0 SDLK_KP_0
    #define MAGE_KEYCODE_KP1 SDLK_KP_1   
    #define MAGE_KEYCODE_KP2 SDLK_KP_2  
    #define MAGE_KEYCODE_KP3 SDLK_KP_3   
    #define MAGE_KEYCODE_KP4 SDLK_KP_4   
    #define MAGE_KEYCODE_KP5 SDLK_KP_5   
    #define MAGE_KEYCODE_KP6 SDLK_KP_6  
    #define MAGE_KEYCODE_KP7 SDLK_KP_7
    #define MAGE_KEYCODE_KP8 SDLK_KP_8  
    #define MAGE_KEYCODE_KP9 SDLK_KP_9   
    #define MAGE_KEYCODE_KPDECIMAL SDLK_KP_DECIMAL  
    #define MAGE_KEYCODE_KPDIVIDE SDLK_KP_DIVIDE 
    #define MAGE_KEYCODE_KPMULTIPLY SDLK_KP_MULTIPLY
    #define MAGE_KEYCODE_KPSUBTRACT SDLK_KP_MINUS
    #define MAGE_KEYCODE_KPADD SDLK_KP_PLUS
    #define MAGE_KEYCODE_KPENTER SDLK_KP_ENTER 
    #define MAGE_KEYCODE_KPEQUAL SDLK_KP_EQUALS
    #define MAGE_KEYCODE_LEFTSHIFT SDLK_LSHIFT  
    #define MAGE_KEYCODE_LEFTCONTROL SDLK_LCTRL   
    #define MAGE_KEYCODE_LEFTALT SDLK_LALT 
    #define MAGE_KEYCODE_LEFTSUPER SDLK_LGUI  
    #define MAGE_KEYCODE_RIGHTSHIFT SDLK_RSHIFT  
    #define MAGE_KEYCODE_RIGHTCONTROL SDLK_RCTRL 
    #define MAGE_KEYCODE_RIGHTALT SDLK_RALT
    #define MAGE_KEYCODE_RIGHTSUPER SDLK_RGUI 
#elif defined(MAGE_GLFW)
	#define MAGE_KEYCODE_SPACE  32
	#define MAGE_KEYCODE_APOSTROPHE  39
	#define MAGE_KEYCODE_COMMA  44 
	#define MAGE_KEYCODE_MINUS  45 
	#define MAGE_KEYCODE_PERIOD  46
	#define MAGE_KEYCODE_SLASH  47
	#define MAGE_KEYCODE_D0  48 
	#define MAGE_KEYCODE_D1  49 
	#define MAGE_KEYCODE_D2  50 
	#define MAGE_KEYCODE_D3  51 
	#define MAGE_KEYCODE_D4  52 
	#define MAGE_KEYCODE_D5  53 
	#define MAGE_KEYCODE_D6  54
	#define MAGE_KEYCODE_D7  55 
	#define MAGE_KEYCODE_D8  56 
	#define MAGE_KEYCODE_D9  57 
	#define MAGE_KEYCODE_SEMICOLON  59 
	#define MAGE_KEYCODE_EQUAL  61 
	#define MAGE_KEYCODE_A  65
	#define MAGE_KEYCODE_B  66
	#define MAGE_KEYCODE_C  67
	#define MAGE_KEYCODE_D  68
	#define MAGE_KEYCODE_E  69
	#define MAGE_KEYCODE_F  70
	#define MAGE_KEYCODE_G  71
	#define MAGE_KEYCODE_H  72
	#define MAGE_KEYCODE_I  73
	#define MAGE_KEYCODE_J  74
	#define MAGE_KEYCODE_K  75
	#define MAGE_KEYCODE_L  76
	#define MAGE_KEYCODE_M  77
	#define MAGE_KEYCODE_N  78
	#define MAGE_KEYCODE_O  79
	#define MAGE_KEYCODE_P  80
	#define MAGE_KEYCODE_Q  81
	#define MAGE_KEYCODE_R  82
	#define MAGE_KEYCODE_S  83
	#define MAGE_KEYCODE_T  84
	#define MAGE_KEYCODE_U  85
	#define MAGE_KEYCODE_V  86
	#define MAGE_KEYCODE_W  87
	#define MAGE_KEYCODE_X  88
	#define MAGE_KEYCODE_Y  89
	#define MAGE_KEYCODE_Z  90
	#define MAGE_KEYCODE_LEFTBRACKET  91 
	#define MAGE_KEYCODE_BACKSLASH  92
	#define MAGE_KEYCODE_RIGHTBRACKET
	#define MAGE_KEYCODE_GRAVEACCENT
	#define MAGE_KEYCODE_ESCAPE  256
	#define MAGE_KEYCODE_ENTER  257
	#define MAGE_KEYCODE_TAB  258
	#define MAGE_KEYCODE_BACKSPACE  259
	#define MAGE_KEYCODE_INSERT 260
	#define MAGE_KEYCODE_DELETE 261
	#define MAGE_KEYCODE_RIGHT  262
	#define MAGE_KEYCODE_LEFT 263
	#define MAGE_KEYCODE_DOWN 264
	#define MAGE_KEYCODE_UP 265
	#define MAGE_KEYCODE_PAGEUP 266
	#define MAGE_KEYCODE_PAGEDOWN 267
	#define MAGE_KEYCODE_HOME 268
	#define MAGE_KEYCODE_END 269
	#define MAGE_KEYCODE_CAPSLOCK 280
	#define MAGE_KEYCODE_SCROLLLOCK 281
	#define MAGE_KEYCODE_NUMLOCK  282
	#define MAGE_KEYCODE_PRINTSCREEN  283
	#define MAGE_KEYCODE_PAUSE  284
	#define MAGE_KEYCODE_F1 290
	#define MAGE_KEYCODE_F2 291
	#define MAGE_KEYCODE_F3 292
	#define MAGE_KEYCODE_F4 293
	#define MAGE_KEYCODE_F5 294
	#define MAGE_KEYCODE_F6 295
	#define MAGE_KEYCODE_F7 296
	#define MAGE_KEYCODE_F8 297
	#define MAGE_KEYCODE_F9 298
	#define MAGE_KEYCODE_F10  299
	#define MAGE_KEYCODE_F11  300
	#define MAGE_KEYCODE_F12  301
	#define MAGE_KEYCODE_F13  302
	#define MAGE_KEYCODE_F14  303
	#define MAGE_KEYCODE_F15  304
	#define MAGE_KEYCODE_F16  305
	#define MAGE_KEYCODE_F17  306
	#define MAGE_KEYCODE_F18  307
	#define MAGE_KEYCODE_F19  308
	#define MAGE_KEYCODE_F20  309
	#define MAGE_KEYCODE_F21  310
	#define MAGE_KEYCODE_F22  311
	#define MAGE_KEYCODE_F23  312
	#define MAGE_KEYCODE_F24  313
	#define MAGE_KEYCODE_KP0  320
	#define MAGE_KEYCODE_KP1  321
	#define MAGE_KEYCODE_KP2  322
	#define MAGE_KEYCODE_KP3  323
	#define MAGE_KEYCODE_KP4  324
	#define MAGE_KEYCODE_KP5  325
	#define MAGE_KEYCODE_KP6  326
	#define MAGE_KEYCODE_KP7  327
	#define MAGE_KEYCODE_KP8  328
	#define MAGE_KEYCODE_KP9  329
	#define MAGE_KEYCODE_KPDECIMAL  330
	#define MAGE_KEYCODE_KPDIVIDE 331
	#define MAGE_KEYCODE_KPMULTIPLY 332
	#define MAGE_KEYCODE_KPSUBTRACT 333
	#define MAGE_KEYCODE_KPADD  334
	#define MAGE_KEYCODE_KPENTER  335
	#define MAGE_KEYCODE_KPEQUAL  336
	#define MAGE_KEYCODE_LEFTSHIFT  340
	#define MAGE_KEYCODE_LEFTCONTROL  341
	#define MAGE_KEYCODE_LEFTALT  342
	#define MAGE_KEYCODE_LEFTSUPER  343
	#define MAGE_KEYCODE_RIGHTSHIFT 344
	#define MAGE_KEYCODE_RIGHTCONTROL 345
	#define MAGE_KEYCODE_RIGHTALT 346
	#define MAGE_KEYCODE_RIGHTSUPER 347
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
	@brief Stores 16 floating point numbers
	@brief When writing shaders in GLSL matrix4x4 is called mat4
	@warning This implimentation uses column majoring
	@see https://en.wikipedia.org/wiki/Row-_and_column-major_order
*/
typedef struct MAGE_MATRIX4X4
{
	/*!
		@brief Stores 16 floats
	*/
	float Elements[16];

} mageMatrix4x4;

/*! 
	@brief Allocates a block of memory for the matrix
	@return Void pointer to the block of memory allocated
	@warning The pointer has not been type casted
*/ 
extern void *mageMatrix4x4Allocate();
/*!
	@brief Sets all the matrix's elements to 0.0f 	
	@param matrix A pointer to a instance of a matrix
	@return Nothing
*/
extern void mageMatrix4x4Default(mageMatrix4x4 *matrix);
/*!
	@brief Initialises the matrix based on 4 vector4s
	@param matrix A pointer to a instance of a matrix
	@param column0 A pointer to a insatnce of a vector4 which the matrix's 0 index row will be set to.
				   Elements[0] = column0.x | Elements[4] = column0.y Elements[8] = column0.z | Elements[12] = column0.w
	@param column1 A pointer to a insatnce of a vector4 which the matrix's 1 index row will be set to.
				   Elements[1] = column1.x | Elements[5] = column1.y Elements[9] = column1.z | Elements[13] = column1.w
	@param column2 A pointer to a insatnce of a vector4 which the matrix's 2 index row will be set to.
				   Elements[2] = column1.x | Elements[6] = column1.y Elements[10] = column1.z | Elements[14] = column1.w
	@param column3 A pointer to a insatnce of a vector4 which the matrix's 3 index row will be set to.
				   Elements[3] = column1.x | Elements[7] = column1.y Elements[11] = column1.z | Elements[15] = column1.w
	@return Nothing
*/
extern void mageMatrix4x4InitialiseVector4(mageMatrix4x4 *matrix, const mageVector4 *column0, const mageVector4 *column1, const mageVector4 *column2, const mageVector4 *column3);
/*!
	@brief Initialises the matrix based on passing in a set number of floats
	@param matrix A pointer to a instance of a matrix
	@param elements A array of floating points
	@param count Count of the amount of floats being passed in
	@return Nothing
*/
extern void mageMatrix4x4InitialiseArray(mageMatrix4x4 *matrix, const float *elements, const uint8 count);
/*! 
	@brief Initialises the matrix using a diagonal
	@param matrix A pointer to a instance of a matrix
	@param diagonal Sets the diagonal elements to the value passed in
	@return Nothing
*/
extern void mageMatrix4x4InitialiseDiagonal(mageMatrix4x4 *matrix, const float diagonal);
/*!
`	@brief Multiplies the left matrix by the right matrix
	@param matrix A pointer to a instance of a matrix
	@return Nothing
*/
extern void mageMatrix4x4Multiply(mageMatrix4x4 *left, const mageMatrix4x4 *right);
/*!
	@brief Applies a perspective matrix to the matrix
	@param matrix A pointer to a instance of a matrix
	@param fov The angle the rays get cas
	@param aspectRatio The aspect ratio of the screen 
	@param near The nearest plane to clip
	@param far The furthest plane to clip
	@return Nothing
	@see https://en.wikipedia.org/wiki/3D_projection
*/
extern void mageMatrix4x4Perspective(mageMatrix4x4 *matrix, const float fov, const float aspectRatio, const float near, const float far);
/*!
	@brief Applies a orthographic matrix to the matrix
	@param matrix A pointer to a instance of a matrix
	@param left The leftmost plane
	@param right The rightmost plane
	@param bottom The bottommost plane
	@param top The topmost plan
	@param near The nearest plane included to the start of the projection
	@param far The furthest plane included to the start of the projection
	@return Nothing
	@see https://en.wikipedia.org/wiki/3D_projection
*/
extern void mageMatrix4x4Orthographic(mageMatrix4x4 *matrix, const float left, const float right, const float bottom, const float top, const float near, const float far);
/*!

*/
extern void mageMatrix4x4Translation(mageMatrix4x4 *matrix, const mageVector3 *translation);
/*!
	@brief Rotates a matrix using a axis and angle
	@param matrix A pointer to a instance of a matrix
	@param angle The angle of rotation
	@param axis Axis to be used for rotation
	@return Nothing
*/
extern void mageMatrix4x4Rotation(mageMatrix4x4 *matrix, const float angle, const mageVector3 *axis);
/*!
	@brief Scales a matrix using 3 floats (vector3)
	@param matrix A pointer to a instance of a matrix
	@param axis The scale the matrix will be applying
	@return Nothing
*/
extern void mageMatrix4x4Scale(mageMatrix4x4 *matrix, const mageVector3 *scale);
/*!
	@brief Destroys the matrix freeing itelsf
	@param matrix A pointer to a instance of a matrix
	@return Nothing
	@warning If the matrix is not allocated on the heap then do not call this method
*/
extern void mageMatrix4x4Destroy(mageMatrix4x4 *matrix);
/*!
	@brief Converts degrees into radians
	@param degrees The degrees to convert to radians
	@param result A pointer to where the result will be dumped
*/
extern void mageToRadians(const float degrees, float *result);



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


