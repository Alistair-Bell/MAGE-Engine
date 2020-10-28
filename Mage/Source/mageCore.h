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
#include "../Externals/glfw3/include/GLFW/glfw3native.h"


#if defined (MAGE_PLATFORM_LINUX)
	/* Including the linux kernel headers and native libraries */	
	#define __USE_XOPEN_EXTENDED
	#include <linux/kernel.h>
	#include <linux/module.h>
	#include <unistd.h>
	#include <pthread.h>

#elif defined (MAGE_PLATFORM_MAC_OS)
	#include <TargetConditionals.h>

#elif defined(MAGE_PLATFORM_WINDOWS) 
	
	/* Including the native windows API */
	#include <Windows.h>
	#if defined (near)
		#undef near
	#endif
	
	#if defined (far)
		#undef far
	#endif
#endif

#define MAGE_ENTRY_POINT() \
	int32_t main(const int32_t argumentCount, char **arguments)


#if defined (MAGE_BUILD_SHARED_LIBRARY) && defined (MAGE_PLATFORM_WINDOWS_64)
	#if defined (MAGE_CORE)
		#define MAGE_API __declspec(dllexport)
	#else
		#define MAGE_API __declspec(dllimport)
	#endif
#else
	#define MAGE_API 
#endif

#if defined (MAGE_OPENAL_BACKEND)
	#include <AL/al.h>
	#include <AL/alc.h>
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

#include "../Externals/libogg/include/ogg/ogg.h"
#include "../Externals/stb-image/src/stb_image.h"



#endif
