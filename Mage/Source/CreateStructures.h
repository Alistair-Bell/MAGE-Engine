#ifndef CREATE_STRUCTURES_H
#define CREATE_STRUCTURES_H

#include "Includes.h"

typedef struct MageApplicationWindow MageApplicationWindow;
typedef struct MageRenderer MageRenderer;

typedef struct MageApplicationWindowCreateInfo
{
    U16         Width;
    U16         Height;
    I16         SpawnOffsetX;
    I16         SpawnOffsetY;
    U8          Resisable;
    const char *Title;
} MageApplicationWindowCreateInfo;

typedef enum MageInputHandlerEventListenFlags
{
    MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_KEYBOARD            = 0x1, /* Keyboard key inputs */
    MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_MOUSE               = 0x2, /* Mouse clicks and positions */
    MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_WINDOW_EVENTS       = 0x4, /* Window focus, refocus, closing, etc */
    MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_ALL_FLAGS           = 0xFFFFFFFF, /* Will allow for all events */
} MageInputHandlerEventListenFlags;

typedef enum MageInputHandlerEventJoystickListenFlags
{
    MAGE_INPUT_HANDLER_EVENT_JOYSTICK_LISTEN_FLAGS_ALL_FLAGS  = 0xFFFFFFFF, /* Will allow for all events */
} MageInputHandlerEventJoystickListenFlags;

typedef struct MageInputHandlerCreateInfo
{
    MageInputHandlerEventListenFlags            InputFlags;
    MageInputHandlerEventJoystickListenFlags    JoystickFlags;
    MageApplicationWindow                       *ApplicationWindow; /* If created with the engine wrapper then it is automatically assigned */
} MageInputHandlerCreateInfo;

typedef enum MageRendererSurfaceFlags
{
    MAGE_RENDERER_SURFACE_FLAGS_USE_NATIVE_WINDOW_SIZE      = 0x01, /* Takes dimensions from active window */
    MAGE_RENDERER_SURFACE_FLAGS_VIEWPORT_ENABLE_OFFSET      = 0x02, /* Allow for the offsetting of the viewport */
} MageRendererSurfaceFlags;

typedef struct MageRendererResizeHandleInfo
{
    U16         Width;
    U16         Height;
} MageRendererResizeHandleInfo;

typedef struct MageRendererSurfaceCreateInfo
{
    U32                                         SurfaceWidth;
    U32                                         SurfaceHeight;
    U32                                         ViewportOffsetX;
    U32                                         ViewportOffsetY;
    MageRendererSurfaceFlags                    Flags;
} MageRendererSurfaceCreateInfo;

typedef struct MageRendererCreateInfo
{
    MageApplicationWindow                       *Window;
    MageRendererSurfaceCreateInfo               SurfaceCreateInfo;
} MageRendererCreateInfo;

typedef struct MageEngineApplicationCreateInfo
{
    MageApplicationWindowCreateInfo             ApplicationWindowCreateInfo;
    MageInputHandlerCreateInfo                  InputEventHandlerCreateInfo;
} MageEngineApplicationCreateInfo;

#endif