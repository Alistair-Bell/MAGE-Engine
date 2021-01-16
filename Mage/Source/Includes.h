#ifndef INCLUDES_H
#define INCLUDES_H

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef NULL
    #define NULL ((void *)0)
#endif

#ifndef MageTrue
    #define MageTrue 1
#endif

#ifndef MageFalse
    #define MageFalse 0
#endif

#if MAGE_BUILD_RENDERER_VULKAN
    #include <vulkan/vulkan.h>
#endif


#if MAGE_BUILD_PLATFORM_LINUX
   #include <unistd.h>
    #include <pthread.h>
    #include <pwd.h>

#elif MAGE_BUILD_PLATFORM_WINDOWS
    #include <Windows.h>
    #include <windowsx.h>
    #if MAGE_BUILD_RENDERER_VULKAN
        #define VK_USE_PLATFORM_WIN32_KHR
        #include <vulkan/vulkan_win32.h>
    #endif
#endif

/* XLib specific libraries */
#if MAGE_BUILD_XLIB
    #include <X11/Xlib.h>
    #include <X11/X.h>
    #include <X11/keysym.h>
    #if MAGE_BUILD_RENDERER_VULKAN
        #define VK_USE_PLATFORM_XLIB_KHR
        #include <vulkan/vulkan_xlib.h>
    #endif
#endif

    /* Wayland specific libraries */
#if MAGE_BUILD_WAYLAND
    #include <wayland-server.h>
    #include <wayland-client.h>
    #include <wayland-util.h>
    #if MAGE_BUILD_RENDERER_VULKAN
        #define VK_USE_PLATFORM_WAYALND_KHR
        #include <vulkan//vulkan_wayland.h>
    #endif  
#endif



#if MAGE_BUILD_DEBUG_MODE
    #define MAGE_HANDLE_ERROR_MESSAGE(condition, execute) if ((condition)) { execute; return MageFalse; }
#else
    #define MAGE_HANDLE_ERROR_MESSAGE(condition, execute) if ((condition)) { return MageFalse; }
#endif

typedef void U0;
typedef char I8;
typedef unsigned char U8;
typedef short I16;
typedef unsigned short U16;
typedef int I32;
typedef unsigned int U32;
typedef long I64;
typedef unsigned long U64;

typedef float F32;
typedef double F64;

#endif
