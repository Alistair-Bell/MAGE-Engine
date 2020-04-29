#ifndef __MAGE_CORE__
#define __MAGE_CORE__

#define MAGE_VULKAN
#define MAGE_DEBUG

#if defined(MAGE_DIRECTX)
	/* Includes the directx api headers and related extensions */
#else 
	#define MAGE_VULKAN
	/* Includes the vulkan api headers and related extensions */
	#include <vulkan/vulkan.h>
#endif

/* Allows for support on platforms without glfw (switch?) */

#if defined(MAGE_SDL2)
	#include <SDL2/SDL.h>
#else
	#define MAGE_GLFW
	#include <GLFW/glfw3.h>
#endif

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h> 
#include <math.h>

#if defined(MAGE_VULKAN) && defined(MAGE_GLFW)
	#define GLFW_INCLUDE_VULKAN

#elif defined(MAGE_SDL2) && defined(MAGE_VULKAN)
	#include <SDL2/SDL_vulkan.h>
#endif



#endif
