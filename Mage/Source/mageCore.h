#ifndef __MAGE_CORE__
#define __MAGE_CORE__


#define MAGE_VULKAN
#include "../Externals/Vulkan-Headers/include/vulkan/vulkan.h"

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
	#define __USE_XOPEN_EXTENDED
	#include <unistd.h>

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
	Entry point 
**************************/
#if defined (MAGE_PLATFORM_WINDOWS_32) || defined (MAGE_PLATFORM_WINDOWS_64)
	#define MAGE_ENTRY_POINT() \
	 	int WINAPI wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR arguments, int32_t commandShow);
#else
	#define MAGE_ENTRY_POINT() \
	 	int32_t main(const int32_t argumentCount, char **arguments)
#endif


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


#endif
