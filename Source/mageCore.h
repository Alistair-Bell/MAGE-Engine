#ifndef __MAGE_CORE__
#define __MAGE_CORE__

#define MAGE_VULKAN

#ifdef MAGE_VULKAN
	/* Includes the vulkan api headers and related extensions */
	#include <vulkan/vulkan.h>
	#include <vulkan/vulkan_core.h>
	#include <vulkan/vk_platform.h>

#elif MAGE_DIRECTX
	/* Includes the directx api headers and the related headers */
#else
    /* Includes the opengl api headers and the related extensions */
	/* This api defaults to opengl for its rendering */
	#define MAGE_OPENGL
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <GLFW/glfw3.h>

#ifdef MAGE_VULKAN
	#define GLFW_INCLUDE_VULKAN
#endif


#endif
