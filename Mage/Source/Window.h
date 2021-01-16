#ifndef WINDOW_H
#define WINDOW_H

#include "Includes.h"
#include "CreateStructures.h"

typedef struct MageApplicationWindowDimensions
{
    U32 Width;
    U32 Height;
    U32 PositionX;
    U32 PositionY;
} MageApplicationWindowDimensions;

/* Thanks XLib for using wonderful names such as Display and Window for predefined types without any prefix :|*/

#if MAGE_BUILD_XLIB

typedef struct MageApplicationWindow
{
    Display     *WindowDisplay;
    Window      ContextWindow;
    Window      RootWindow;
    XEvent      PollingEvent;
} MageApplicationWindow;

#elif MAGE_BUILD_WAYLAND

typedef struct MageApplicationWindow
{
    struct wl_display    *Display;
    struct wl_surface    *Surface;
} MageApplicationWindow;

#endif

#if MAGE_BUILD_PLATFORM_WINDOWS

/* The one time I can commend Microsoft is for not having the most generic names for types! Thanks :| */

typedef struct MageApplicationWindow
{
    HINSTANCE   Instance;
    HWND        NativeWindow;
    WNDCLASSEX  NativeWindowClass;
} MageApplicationWindow;

#endif

#if MAGE_BUILD_RENDERER_VULKAN
extern U8 MageApplicationWindowCreateVulkanSurface(MageApplicationWindow *window, VkInstance instance, VkSurfaceKHR *surface); 
#endif

extern U8 MageApplicationWindowCreate(MageApplicationWindowCreateInfo *info, MageApplicationWindow *window);
extern U8 MageApplicationWindowSetTitle(MageApplicationWindow *window, const char *newName);
extern U8 MageApplicationWindowGetDimensions(MageApplicationWindow *window, MageApplicationWindowDimensions *dimensions);
extern U8 MageApplicationWindowGetFramebufferDimensions(MageApplicationWindow *window, MageApplicationWindowDimensions *dimensions);
extern U8 MageApplicationWindowDestroy(MageApplicationWindow* window);

#endif
