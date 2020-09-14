#ifndef MAGE_CORE_HEADER
#define MAGE_CORE_HEADER

/*
	This file contains core macros used by the engine and handles the includes for the engine
	If you are using the engine you should leave this file the way it is unless you want to
	mageAPI.h is the main interface you should use and mageCore.h is included by mageAPI.h
*/

#define MAGE_VULKAN
#include "../Externals/Vulkan-Headers/include/vulkan/vulkan.h"

#define GLFW_INCLUDE_NONE
#define MAGE_GLFW
#include "../Externals/glfw3/include/GLFW/glfw3.h"


/* Platform checking */


/* Checking if using linux platforms */
#if defined (__linux__)
	#define MAGE_PLATFORM_LINUX


	/* Including the linux kernel headers and native libraries */	
	#define __USE_XOPEN_EXTENDED
	#include <linux/kernel.h>
	#include <linux/module.h>
	#include <unistd.h>
	#include <pthread.h>

	/* Checking if it is android as android uses the linux kernel */	
	#if defined (__ANDROID__)
		#undef MAGE_PLATFORM_LINUX
		#define MAGE_PLATFORM_ANDROID_LINUX
	#endif

/* Checking if using one of the apple platforms */
#elif defined (__APPLE__)
	#include <TargetConditionals.h>

	#if TARGET_OS_IPHONE == 1
		
		/* Identified the platform as the mobile IOS platform */
		#define MAGE_PLATFORM_IOS
	#elif TARGET_OS_MAC == 1
		
		/* Identified the platform as the desktop MACOS */		
		#define MAGE_PLATFORM_MACOS
	#else
		#error "Unknown Apple platform!"
	#endif


/* Checking if using a windows platform */
#elif defined(_WIN64) 
	#define MAGE_PLATFORM_WINDOWS
	
	/* Including the native windows API */
	#include <Windows.h>
	#if defined (near)
		#undef near
	#elif defined (far)
		#undef far
	#endif
#else
	#error "Undefined platform MAGE currently supports Windows, Apple OS's and Linux, 64 bit systems only"
#endif 


/* Client entry point macro */
#if defined (MAGE_PLATFORM_WINDOWS_32) || defined (MAGE_PLATFORM_WINDOWS_64)
	#define MAGE_ENTRY_POINT() \
	 	int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR arguments, int32_t commandShow)
#else
	#define MAGE_ENTRY_POINT() \
	 	int32_t main(const int32_t argumentCount, char **arguments)
#endif

/* Checking if we are compiling with c++ */
#if defined (__cplusplus)
	#define MAGE_CPP
#endif

/* Checking if the microsoft visual studio compiler is being used */
#if defined (_MSC_VER)
	#define MAGE_COMPILER_MVS

/* Checking if using the gcc or gcc for c++ compiler */
#elif defined(__GNUC__)	|| defined(__GNUG__)
	#define MAGE_COMPILER_GCC
	
/* Checking if using the clang compiler */
#elif defined(__clang__)
	#define MAGE_COMPILER_CLANG


/* Checking if mingw compiler */
#elif defined(__MINGW32__)
	#define MAGE_COMPILER_MINGW_32

	/* Checking if using a 64 bit compiler as the 32 flag is still flagged */
	#if defined (__MINGW64__)
		#undef MAGE_COMPILER_MINGW_32
		#define MAGE_COMPILER_MINGW_64
	#endif
#endif

#if defined (MAGE_BUILD_SHARED_LIBRARY) && defined (MAGE_PLATFORM_WINDOWS_64)
	#if defined (MAGE_CORE)
		#define MAGE_API __declspec(dllexport)
	#else
		#define MAGE_API __declspec(dllimport)
	#endif
#else
	#define MAGE_API 
#endif

#if defined (MAGE_PULSE_AUDIO_BACKEND)
	#include <pulse/pulseaudio.h>
#endif


/* C standard includes */
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

#include "../Externals/stb-image/src/stb_image.h"



#endif
