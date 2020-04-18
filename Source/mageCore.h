#ifndef __MAGE_CORE__
#define __MAGE_CORE__


#ifdef __MAGE_VULKAN__
	/* Includes the vulkan api headers and related extensions */
	#include <vulkan/vulkan.h>

#elif __MAGE_DIRECTX__
	/* Includes the directx api headers and the related headers */
#else
    /* Includes the opengl api headers and the related extensions */
	/* This api defaults to opengl for its rendering */
	#define __MAGE_OPENGL_
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <GLFW/glfw3.h>

#ifdef __MAGE_VULKAN__
	#define GLFW_INCLUDE_VULKAN
#endif


#endif
