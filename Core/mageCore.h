#ifndef __MAGE_CORE__
#define __MAGE_CORE__

#if defined (MAGE_DIRECTX)
	/*!************************ 
		Includes the directx api headers and related extensions 
	**************************/
#else 
	#define MAGE_VULKAN
	/*!************************
		Includes the vulkan api headers and related extensions 
	**************************/
	#include <vulkan/vulkan.h>
#endif

/*!************************
	Allows for support on platforms without glfw 
**************************/

#if defined (MAGE_SDL2)
	#include <SDL2/SDL.h>

	#if defined (MAGE_VULKAN) 
		#include <SDL2/SDL_vulkan.h>
	#endif

#else
	#define MAGE_GLFW
	#if defined (MAGE_VULKAN) 
		#define GLFW_INCLUDE_VULKAN
	#endif
	#include <GLFW/glfw3.h>
#endif


/*!************************
	Platform checking 
**************************/


/*!************************
	Checking if using linux platforms 
**************************/
#if defined (__linux__)
	#define MAGE_PLATFORM_LINUX
	
	/*!************************
		Checking if it is android linux as android uses the linux kernel
	**************************/
	#if defined (__ANDROID__)
		#undef MAGE_PLATFORM_LINUX
		#define MAGE_PLATFORM_ANDROID_LINUX
	#endif

/*!************************
	Checking if using one of the apple platforms
**************************/
#elif defined (__APPLE__)
	#include <TargetConditionals.h>

	#if TARGET_OS_IPHONE == 1
		/*!************************
			Identified the platform as the mobile IOS platform
		**************************/
		#define MAGE_PLATFORM_IOS
	#elif TARGET_OS_MAC == 1
		/*!************************
			Identified the platform as the desktop MACOS
		**************************/
		#define MAGE_PLATFORM_MACOS
	#else
		#error "Unknown Apple platform!"
	#endif

/*!************************
	Checking if using a windows platform 
**************************/
#elif defined(_WIN32) 
	#define MAGE_PLATFORM_WINDOWS_32
	
	/*!************************
		Checking if using a 64 bit windows as _WIN32 flags for either 32 or 64 bit systems
	**************************/
	#if defined(_WIN64)
		#undef MAGE_PLATFORM_WINDOWS_32
		#define MAGE_PLATFORM_WINDOWS_64
	#endif

	/*!************************
		Including the native windows API
	**************************/
	#include <Windows.h>

#else
	#error "Undefined platform MAGE currently supports Windows, Apple OS's and Linux"
#endif 


/*!************************
	Compiler checking 
**************************/


/*!************************
	Checking if the microsoft visual studio compiler is being used
**************************/
#if defined(_MSC_VER)
	#define MAGE_COMPILER_MVS

/*!************************
	Checking if using the gcc or gcc for c++ compiler
**************************/
#elif defined(__GNUC__)	|| defined(__GNUG__)
	#define MAGE_COMPILER_GCC
	
/*!************************
	Checking if using the clang compiler 
**************************/
#elif defined(__clang__)
	#define MAGE_COMPILER_CLANG

/*!************************
	Checking if mingw compiler 
**************************/
#elif defined(__MINGW32__)
	#define MAGE_COMPILER_MINGW_32

	/*!************************
		Checking if using a 64 bit compiler as the 32 flag is still flagged
	**************************/
	#if defined(__MINGW64__)
		#undef MAGE_COMPILER_MINGW_32
		#define MAGE_COMPILER_MINGW_64
	#endif
#endif


/*!************************
	DLL building check
**************************/


/*!************************
	Checking if exporting DLL
**************************/
#if defined(MAGE_PLATFORM_WINDOWS_32) || defined(MAGE_PLATFORM_WINDOWS_64)

	#if defined(MAGE_CORE)
		
		/*!************************
			If its the core then it will be exporting to the dll 
		**************************/
		#define MAGE_API __declspec(dllexport)
	#else
		
		/*!************************
			If its the core then it will be importing the dll 
		**************************/
		#define MAGE_API __declspec(dllimport)
	#endif
#else
	/*!************************
		Prefix used to for core methods and stuff for dll export use
	**************************/
	#define MAGE_API
#endif




/*!************************
	@brief Using vulkan rendering mode
**************************/
#define MAGE_VULKAN
/*!************************
	@brief Using a debug mode with logging
**************************/
#define MAGE_DEBUG
/*!************************
	@brief Core to say this is being called from the core library so do dll export
**************************/
#define MAGE_CORE



/*!************************
	C standard includes 
**************************/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h> 
#include <math.h>
#include <time.h>
#include <signal.h> 



/*!************************
	@brief The mode that is a inform 
**************************/
#define MAGE_LOG_MODE_INFORM 0
/*!************************
	@brief The mode that is a warning error and wont cause a assert trigger
**************************/
#define MAGE_LOG_MODE_WARNING 1
/*!************************
	@brief The mode that is a error and wont cause a assert trigger
**************************/
#define MAGE_LOG_MODE_ERROR 2
/*!************************
	@brief The mode that is a fatal error and will cause a assert trigger
**************************/
#define MAGE_LOG_MODE_FATAL_ERROR 3
/*!************************
	@brief The mode for the logging core
**************************/
#define MAGE_LOG_USER_CORE 0
/*!************************
	@brief The mode for the logging client
**************************/
#define MAGE_LOG_USER_CLIENT 1

#if defined (MAGE_DEBUG)

	#define MAGE_VALIDATION_LAYERS 1

	#if defined(MAGE_COMPILER_MVS)
		#define MAGE_DEBUG_BREAK __debugbreak()
	#elif defined(MAGE_PLATFORM_LINUX)
		#define MAGE_DEBUG_BREAK raise(SIGABRT)
	#else
		#define MAGE_DEBUG_BREAK
	#endif

	#define MAGE_LOG_CORE_INFORM(x, ...) mageLogMessage(MAGE_LOG_USER_CORE, MAGE_LOG_MODE_INFORM, __LINE__, __FILE__, x, __VA_ARGS__)
	#define MAGE_LOG_CORE_WARNING(x, ...) mageLogMessage(MAGE_LOG_USER_CORE, MAGE_LOG_MODE_WARNING, __LINE__, __FILE__, x, __VA_ARGS__)
	#define MAGE_LOG_CORE_ERROR(x, ...) mageLogMessage(MAGE_LOG_USER_CORE, MAGE_LOG_MODE_ERROR, __LINE__, __FILE__, x, __VA_ARGS__)
	#define MAGE_LOG_CORE_FATAL_ERROR(x, ...) mageLogMessage(MAGE_LOG_USER_CORE, MAGE_LOG_MODE_FATAL_ERROR, __LINE__, __FILE__, x, __VA_ARGS__); MAGE_DEBUG_BREAK
	
	#define MAGE_LOG_CLIENT_INFORM(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_INFORM, __LINE__, __FILE__, x, __VA_ARGS__)
	#define MAGE_LOG_CLIENT_WARNING(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_WARNING, __LINE__, __FILE__, x, __VA_ARGS__)
	#define MAGE_LOG_CLIENT_ERROR(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_ERROR, __LINE__, __FILE__, x, __VA_ARGS__)
	#define MAGE_LOG_CLIENT_FATAL_ERROR(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_FATAL_ERROR, __LINE__, __FILE__, x, __VA_ARGS__); MAGE_DEBUG_BREAK
#else
	#define MAGE_LOG_CORE_INFORM(x, ...)
	#define MAGE_LOG_CORE_WARNING(x, ...)
	#define MAGE_LOG_CORE_ERROR(x, ...)
	#define MAGE_LOG_CORE_FATAL_ERROR(x, ...)
	#define MAGE_LOG_CLIENT_INFORM(x, ...)
	#define MAGE_LOG_CLIENT_WARNING(x, ...)
	#define MAGE_LOG_CLIENT_ERROR(x, ...)
	#define MAGE_LOG_CLIENT_FATAL_ERROR(x, ...)
	#define MAGE_VALIDATION_LAYERS 0
	#define MAGE_DEBUG_BREAK
#endif

#if defined (MAGE_SDL2)
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


/*!************************
	@brief Event type none
**************************/
#define MAGE_EVENT_TYPE_NONE 0
/*!************************
	@brief Event type of window being closed (terminated)
**************************/
#define MAGE_EVENT_TYPE_WINDOW_CLOSE 1
/*!************************
	@brief Event type of window being refucussed (tabbed in)
**************************/
#define MAGE_EVENT_TYPE_WINDOW_FOCUS 2
/*!************************
	@brief Event type of window being lost focus (tabbed out)
**************************/
#define MAGE_EVENT_TYPE_WINDOW_LOST_FOCUS 3
/*!************************
	@brief Event type of window being moved around (not available in full screen)
**************************/
#define MAGE_EVENT_TYPE_WINDOW_MOVED 4
/*!************************
	@brief Event type of window being resized (dimensions changed)
**************************/
#define MAGE_EVENT_TYPE_WINDOW_RESIZE 5
/*!************************
	@brief Event type of application update frame
**************************/
#define MAGE_EVENT_TYPE_APPLICATION_UPDATE 6
/*!************************
	@brief Event type of window tick being increased (not related to frame rate)
**************************/
#define MAGE_EVENT_TYPE_APPLICATION_TICK 7
/*!************************
	@brief Event type of application render call being executed
**************************/
#define MAGE_EVENT_TYPE_APPLICATION_RENDER 8
/*!************************
	@brief Event type of a key being pressed
**************************/
#define MAGE_EVENT_TYPE_KEY_PRESSED 9
/*!************************
	@brief Event type of a key being released
**************************/
#define MAGE_EVENT_TYPE_KEY_RELEASED 10
/*!************************
	@brief Event type of a continued input of text streams (text input)
**************************/
#define MAGE_EVENT_TYPE_KEY_TYPED 11 
/*!************************
	@brief Event type of a mouse button being presesd
**************************/
#define MAGE_EVENT_TYPE_MOUSE_BUTTON_PRESSED 12
/*!************************
	@brief Event type of a mouse button being released
**************************/
#define MAGE_EVENT_TYPE_MOUSE_BUTTON_RELEASED 13
/*!************************
	@brief Event type of a mouse being moved
**************************/
#define MAGE_EVENT_TYPE_MOUSE_MOVE 14
/*!************************
	@brief Event type of a mouse wheel being scrolled
**************************/
#define MAGE_EVENT_TYPE_MOUSE_SCROLL 15
/*!************************
	@brief Event catagory undefined
**************************/
#define MAGE_EVENT_CATAGORY_NONE 3
/*!************************
	@brief Event catagory window
**************************/
#define MAGE_EVENT_CATAGORY_WINDOW 4 
/*!************************
	@brief Event catagory application
**************************/
#define MAGE_EVENT_CATAGORY_APPLICATION 5 
/*!************************
	@brief Event catagory keyboard
**************************/
#define MAGE_EVENT_CATAGORY_KEYBOARD 6
/*!************************
	@brief Event catagory mouse
**************************/
#define MAGE_EVENT_CATAGORY_MOUSE 7
/*!************************
	@brief The bit of the type of the camera (0 = orthographic | 1 = perspective)
**************************/
#define MAGE_CAMERA_BIT_TYPE 0
/*!************************
	@brief The bit whether the camera is the master one (0 = extra camera | 1 = main camera)
**************************/
#define MAGE_CAMERA_BIT_MASTER 1
/*!************************
	@brief The bit whether it is static in the x axis (0 = not fixed | 1 = fixed)
**************************/
#define MAGE_CAMERA_BIT_STATIC_X 2
/*!************************
	@brief The bit whether it is static in the y axis (0 = not fixed | 1 = fixed)
**************************/
#define MAGE_CAMERA_BIT_STATIC_Y 3
/*!************************
	@brief The bit whether it is static in the z axis (0 = not fixed | 1 = fixed)
**************************/
#define MAGE_CAMERA_BIT_STATIC_Z 4
/*!************************
	@brief The bit whether it is staticly fixed in the rotation (0 = not fixed | 1 = fixed)
**************************/
#define MAGE_CAMERA_BIT_STATIC_ROTATION 5
/*!************************
	@brief The bit whether the override method is being used (0 = defualt | 1 = overriden)
**************************/
#define MAGE_CAMERA_BIT_UPDATE_OVERRIDE 6
/*!************************
	@brief The bit whether the camera is on (0 = off | 1 = on)
**************************/
#define MAGE_CAMERA_BIT_TOGGLE 7



#endif
