#ifndef WINDOW_H
#define WINDOW_H

#include "Includes.h"
#include "CreateStructures.h"

#if MAGE_BUILD_PLATFORM_LINUX

/* Thanks XLib for using wonderful names such as Display and Window for predefined types without any prefix :| */

typedef struct MageApplicationWindow
{
    Display     *WindowDisplay;
    Window      ContextWindow;
    Window      RootWindow;
    XEvent      PollingEvent;
    Colormap    ColorMap;
    Cursor      ContextCursor;
} MageApplicationWindow;

#elif MAGE_BUILD_PLATFORM_WINDOWS

#else
    #error "Platform not implimented"
#endif


extern U8 MageApplicationWindowCreate(MageApplicationWindowCreateInfo *info, MageApplicationWindow *window);
extern U8 MageApplicationWindowDestroy(MageApplicationWindow *window);

#endif