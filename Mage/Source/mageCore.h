#ifndef __MAGE_CORE__
#define __MAGE_CORE__

#if defined (MAGE_DIRECTX)
	/*!************************ 
		Includes the directx api headers and related extensions 
	**************************/
#else 
	#define MAGE_VULKAN
	#include "../Externals/Vulkan-Headers/include/vulkan/vulkan.h"
#endif

#define GLFW_INCLUDE_NONE
#define MAGE_GLFW
#if defined (MAGE_VULKAN) 
	#define GLFW_INCLUDE_VULKAN
#endif
#include "../Externals/glfw3/include/GLFW/glfw3.h"


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

#define HYPATIA_SINGLE_PRECISION_FLOATS
#include "../Externals/hypatia/src/hypatia.h"
#include "../Externals/lodepng/src/lodepng.h"
#include "../Externals/libyaml/include/yaml.h"


#define MAGE_LOG_MODE_INFORM 0
#define MAGE_LOG_MODE_WARNING 1
#define MAGE_LOG_MODE_ERROR 2
#define MAGE_LOG_MODE_FATAL_ERROR 3
#define MAGE_LOG_USER_CORE 0
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
#else
	#define MAGE_LOG_CORE_INFORM(x, ...)
	#define MAGE_LOG_CORE_WARNING(x, ...)
	#define MAGE_LOG_CORE_ERROR(x, ...)
	#define MAGE_LOG_CORE_FATAL_ERROR(x, ...)
	#define MAGE_DEBUG_BREAK
#endif



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


#define MAGE_SET_BIT(input, index, value) (input |= value << index)
#define MAGE_BIT(index) (1 << index) 

#endif
