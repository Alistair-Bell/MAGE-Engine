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
#if defined (MAGE_SDL)
	#include <SDL2/SDL.h>

	#if defined (MAGE_VULKAN) 
		#include <SDL2/SDL_vulkan.h>
	#endif
	#error This mode has not been fully implimented, please use the glfw mode
#else
	#define GLFW_INCLUDE_NONE
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
		Including the linux kernal headers  
	**************************/
	#include <linux/kernel.h>
	#include <linux/module.h>

	/*!************************
		Checking if it is android as android uses the linux kernel
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
	#if defined (_WIN64)
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
#if defined (_MSC_VER)
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
	#if defined (__MINGW64__)
		#undef MAGE_COMPILER_MINGW_32
		#define MAGE_COMPILER_MINGW_64
	#endif
#endif



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
#include <stdint.h>
#include <pthread.h>

/*!************************
	Local includes 
**************************/
#include "Externals/hypatia/src/hypatia.h"

/*!************************
	Allows mono c# scripting 
**************************/
#if defined (MAGE_MONO_EXTERNALS)
	#include <mono-2.0/mono/metadata/assembly.h>
	#include <mono-2.0/mono/jit/jit.h>
#endif

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

	#if defined (MAGE_COMPILER_MVS)
		#define MAGE_DEBUG_BREAK __debugbreak()
	#elif defined(MAGE_PLATFORM_LINUX)
		#define MAGE_DEBUG_BREAK raise(SIGABRT)
	#else
		#define MAGE_DEBUG_BREAK
	#endif

	#define MAGE_LOG_CORE_INFORM(x, ...) mageLogMessage(MAGE_LOG_USER_CORE, MAGE_LOG_MODE_INFORM, x, __VA_ARGS__)
	#define MAGE_LOG_CORE_WARNING(x, ...) mageLogMessage(MAGE_LOG_USER_CORE, MAGE_LOG_MODE_WARNING, x, __VA_ARGS__)
	#define MAGE_LOG_CORE_ERROR(x, ...) mageLogMessage(MAGE_LOG_USER_CORE, MAGE_LOG_MODE_ERROR, x, __VA_ARGS__)
	#define MAGE_LOG_CORE_FATAL_ERROR(x, ...) mageLogMessage(MAGE_LOG_USER_CORE, MAGE_LOG_MODE_FATAL_ERROR, x, __VA_ARGS__)
	#define MAGE_LOG_CLIENT_INFORM(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_INFORM, x, __VA_ARGS__)
	#define MAGE_LOG_CLIENT_WARNING(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_WARNING, x, __VA_ARGS__)
	#define MAGE_LOG_CLIENT_ERROR(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_ERROR, x, __VA_ARGS__)
	#define MAGE_LOG_CLIENT_FATAL_ERROR(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_FATAL_ERROR, x, __VA_ARGS__)
#else
	#define MAGE_LOG_CORE_INFORM(x, ...)
	#define MAGE_LOG_CORE_WARNING(x, ...)
	#define MAGE_LOG_CORE_ERROR(x, ...)
	#define MAGE_LOG_CORE_FATAL_ERROR(x, ...)
	#define MAGE_DEBUG_BREAK
	#define MAGE_LOG_CLIENT_INFORM(x, ...)
	#define MAGE_LOG_CLIENT_WARNING(x, ...)
	#define MAGE_LOG_CLIENT_ERROR(x, ...)
	#define MAGE_LOG_CLIENT_FATAL_ERROR(x, ...)
#endif



#if defined (MAGE_SDL)
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
    #define MAGE_KEYCODE_D6 SDLK_
    #define MAGE_KEYCODE_D7 SDLK_7 
    #define MAGE_KEYCODE_D8 SDLK_8
    #define MAGE_KEYCODE_D9 SDLK_9  
    #define MAGE_KEYCODE_SEMICOLON SDLK_SEMICOLON  
    #define MAGE_KEYCODE_EQUAL SDLK_EQUAL
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
	#define MAGE_KEYCODE_SPACE  				32
	#define MAGE_KEYCODE_APOSTROPHE  			39
	#define MAGE_KEYCODE_COMMA  				44 
	#define MAGE_KEYCODE_MINUS  				45 
	#define MAGE_KEYCODE_PERIOD 				46
	#define MAGE_KEYCODE_SLASH  				47
	#define MAGE_KEYCODE_D0  					48 
	#define MAGE_KEYCODE_D1  					49 
	#define MAGE_KEYCODE_D2  					50 
	#define MAGE_KEYCODE_D3  					51 
	#define MAGE_KEYCODE_D4  					52 
	#define MAGE_KEYCODE_D5  					53 
	#define MAGE_KEYCODE_D6  					54
	#define MAGE_KEYCODE_D7  					55 
	#define MAGE_KEYCODE_D8  					56 
	#define MAGE_KEYCODE_D9  					57 
	#define MAGE_KEYCODE_SEMICOLON  			59 
	#define MAGE_KEYCODE_EQUAL  				61 
	#define MAGE_KEYCODE_A  					65
	#define MAGE_KEYCODE_B  					66
	#define MAGE_KEYCODE_C  					67
	#define MAGE_KEYCODE_D  					68
	#define MAGE_KEYCODE_E  					69
	#define MAGE_KEYCODE_F  					70
	#define MAGE_KEYCODE_G  					71
	#define MAGE_KEYCODE_H  					72
	#define MAGE_KEYCODE_I  					73
	#define MAGE_KEYCODE_J  					74
	#define MAGE_KEYCODE_K  					75
	#define MAGE_KEYCODE_L  					76
	#define MAGE_KEYCODE_M  					77
	#define MAGE_KEYCODE_N  					78
	#define MAGE_KEYCODE_O  					79
	#define MAGE_KEYCODE_P  					80
	#define MAGE_KEYCODE_Q  					81
	#define MAGE_KEYCODE_R  					82
	#define MAGE_KEYCODE_S  					83
	#define MAGE_KEYCODE_T  					84
	#define MAGE_KEYCODE_U  					85
	#define MAGE_KEYCODE_V  					86
	#define MAGE_KEYCODE_W  					87
	#define MAGE_KEYCODE_X  					88
	#define MAGE_KEYCODE_Y  					89
	#define MAGE_KEYCODE_Z  					90
	#define MAGE_KEYCODE_LEFTBRACKET 			91 
	#define MAGE_KEYCODE_BACKSLASH  			92
	#define MAGE_KEYCODE_RIGHTBRACKET           93
	#define MAGE_KEYCODE_GRAVEACCENT			96
	#define MAGE_KEYCODE_ESCAPE  				256
	#define MAGE_KEYCODE_ENTER  				257
	#define MAGE_KEYCODE_TAB  					258
	#define MAGE_KEYCODE_BACKSPACE  			259
	#define MAGE_KEYCODE_INSERT 				260
	#define MAGE_KEYCODE_DELETE 				261
	#define MAGE_KEYCODE_RIGHT  				262
	#define MAGE_KEYCODE_LEFT 					263
	#define MAGE_KEYCODE_DOWN 					264
	#define MAGE_KEYCODE_UP 					265
	#define MAGE_KEYCODE_PAGEUP 				266
	#define MAGE_KEYCODE_PAGEDOWN 				267
	#define MAGE_KEYCODE_HOME 					268
	#define MAGE_KEYCODE_END 					269
	#define MAGE_KEYCODE_CAPSLOCK 				280
	#define MAGE_KEYCODE_SCROLLLOCK 			281
	#define MAGE_KEYCODE_NUMLOCK  				282	
	#define MAGE_KEYCODE_PRINTSCREEN  			283
	#define MAGE_KEYCODE_PAUSE  				284
	#define MAGE_KEYCODE_F1   					290
	#define MAGE_KEYCODE_F2   					291
	#define MAGE_KEYCODE_F3   					292
	#define MAGE_KEYCODE_F4   					293
	#define MAGE_KEYCODE_F5   					294
	#define MAGE_KEYCODE_F6   					295
	#define MAGE_KEYCODE_F7   					296
	#define MAGE_KEYCODE_F8   					297
	#define MAGE_KEYCODE_F9   					298
	#define MAGE_KEYCODE_F10  					299
	#define MAGE_KEYCODE_F11  					300
	#define MAGE_KEYCODE_F12  					301
	#define MAGE_KEYCODE_F13  					302
	#define MAGE_KEYCODE_F14  					303
	#define MAGE_KEYCODE_F15  					304
	#define MAGE_KEYCODE_F16  					305
	#define MAGE_KEYCODE_F17  					306
	#define MAGE_KEYCODE_F18  					307
	#define MAGE_KEYCODE_F19  					308
	#define MAGE_KEYCODE_F20  					309
	#define MAGE_KEYCODE_F21  					310
	#define MAGE_KEYCODE_F22  					311
	#define MAGE_KEYCODE_F23  					312
	#define MAGE_KEYCODE_F24  					313
	#define MAGE_KEYCODE_KP0  					320
	#define MAGE_KEYCODE_KP1  					321
	#define MAGE_KEYCODE_KP2  					322
	#define MAGE_KEYCODE_KP3  					323
	#define MAGE_KEYCODE_KP4  					324
	#define MAGE_KEYCODE_KP5  					325
	#define MAGE_KEYCODE_KP6  					326
	#define MAGE_KEYCODE_KP7  					327
	#define MAGE_KEYCODE_KP8  					328
	#define MAGE_KEYCODE_KP9  					329
	#define MAGE_KEYCODE_KPDECIMAL  			330
	#define MAGE_KEYCODE_KPDIVIDE 				331
	#define MAGE_KEYCODE_KPMULTIPLY 			332
	#define MAGE_KEYCODE_KPSUBTRACT 			333
	#define MAGE_KEYCODE_KPADD  				334
	#define MAGE_KEYCODE_KPENTER  				335
	#define MAGE_KEYCODE_KPEQUAL  				336
	#define MAGE_KEYCODE_LEFTSHIFT  			340
	#define MAGE_KEYCODE_LEFTCONTROL  			341
	#define MAGE_KEYCODE_LEFTALT  				342
	#define MAGE_KEYCODE_LEFTSUPER  			343
	#define MAGE_KEYCODE_RIGHTSHIFT 			344
	#define MAGE_KEYCODE_RIGHTCONTROL   		345
	#define MAGE_KEYCODE_RIGHTALT 				346
	#define MAGE_KEYCODE_RIGHTSUPER     		347

	#define MAGE_MOUSECODE_BUTTON_1 			0
	#define MAGE_MOUSECODE_BUTTON_2 			1
	#define MAGE_MOUSECODE_BUTTON_3 			2
	#define MAGE_MOUSECODE_BUTTON_4 			3
	#define MAGE_MOUSECODE_BUTTON_5 			4
	#define MAGE_MOUSECODE_BUTTON_6 			5
	#define MAGE_MOUSECODE_BUTTON_7 			6
	#define MAGE_MOUSECODE_BUTTON_8 			7

	#define MAGE_MOUSECODE_BUTTON_LEFT  		1
	#define MAGE_MOUSECODE_BUTTON_RIGHT 		2
	#define MAGE_MOUSECODE_BUTTON_MIDDLE 		3

#endif

/*!************************
 * @brief Sets a bit in a value
 * @param input The number being modified
 * @param index The nth bit along (0 -> 7 in an byte) 0 = 1,  1 = 2,  2 = 4,  3 = 8,  4 = 16
 * @param value The value the bit will be set to (0 | 1)
**************************/
#define MAGE_SET_BIT(input, index, value) (input |= value << index)
/*!************************
 * @brief Specifies the shift for the bit
 * @param index The nth bit along (0 -> 7 in an byte) 0 = 1,  1 = 2,  2 = 4,  3 = 8,  4 = 16
**************************/
#define MAGE_BIT(index) (1 << index) 

#endif
