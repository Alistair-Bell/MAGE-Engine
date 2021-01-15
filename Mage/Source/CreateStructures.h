#ifndef CREATE_STRUCTURES_H
#define CREATE_STRUCTURES_H

#include "Includes.h"

typedef struct MageShaderCreateInfo MageShaderCreateInfo;
typedef struct MageApplicationWindow MageApplicationWindow;
typedef struct MageRenderer MageRenderer;


#if MAGE_BUILD_PLATFORM_LINUX
    typedef U0 *(*MageThreadCallback)(U0 *);
    #define MAGE_THREAD_RETURN_VALUE U0 *
#elif MAGE_BUILD_PLATFORM_WINDOWS
    typedef DWORD (*MageThreadCallback)(U8 *);
    #define MAGE_THREAD_RETURN_VALUE DWORD
#endif

typedef enum MageGraphicsBufferCreateInfoFlags
{
    MAGE_GRAPHICS_BUFFER_CREATE_INFO_FLAGS_STATIC, /* Likely to be the same all the time, not likely to change */
    MAGE_GRAPHICS_BUFFER_CREATE_INFO_FLAGS_ADAPTABLE, /* Likely to change and be updated */
} MageGraphicsBufferCreateInfoFlags;

typedef enum MageGraphicsBufferCreateInfoType
{
    MAGE_GRAPHICS_BUFFER_CREATE_INFO_TYPE_VERTEX,
    MAGE_GRAPHICS_BUFFER_CREATE_INFO_TYPE_INDEX,
    MAGE_GRAPHICS_BUFFER_CREATE_INFO_TYPE_UNIFORM,
} MageGraphicsBufferCreateInfoType;

typedef struct MageGraphicsBufferCreateInfo
{
    MageGraphicsBufferCreateInfoFlags Flags;
    MageGraphicsBufferCreateInfoType  Type;
    U0                                *Data;
    U64                               Size;
}  MageGraphicsBufferCreateInfo;

typedef enum MageApplicationWindowCreateFlags
{
    MAGE_APPLICATION_WINDOW_CREATE_FLAGS_ALLOW_RESIZING = 0x01,
    MAGE_APPLICATION_WINDOW_CREATE_FLAGS_FULLSCREEN     = 0x02,
    MAGE_APPLICATION_WINDOW_CREATE_FLAGS_AUTO_CENTRE    = 0x04,
} MageApplicationWindowCreateFlags;
typedef struct MageApplicationWindowCreateInfo
{
    U16                                Width;
    U16                                Height;
    I16                                SpawnOffsetX;
    I16                                SpawnOffsetY;
    MageApplicationWindowCreateFlags   Flags;
    const char                        *Title; /* If created with the engine wrapper then it is automatically assigned as the ProgramName */
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

typedef struct MageRendererResizeHandleInfo
{
    U16         Width;
    U16         Height;
} MageRendererResizeHandleInfo;

typedef struct MageRendererCreateInfo
{
    MageApplicationWindow                       *Window;  /* If created with the engine wrapper then it is automatically assigned */
    MageShaderCreateInfo                        *PipelineShadersInfo;
    U32                                         PipelineShaderCount;
    U32                                         ConcurrentThreadCount;
} MageRendererCreateInfo;

typedef struct MageEngineApplicationCreateInfo
{
    const char                                  *ProgramName;
    MageApplicationWindowCreateInfo             ApplicationWindowCreateInfo;
    MageInputHandlerCreateInfo                  InputEventHandlerCreateInfo;
    MageRendererCreateInfo                      RendererCreateInfo;
} MageEngineApplicationCreateInfo;

typedef struct MageFileSystemCreateInfo
{
    U0 *TempData; /* This is a junk parameter, some compilers don't like empty fields */
} MageFileSystemCreateInfo;

typedef struct MageThreadCreateInfo
{
    MageThreadCallback   Callback; 
    U0                   *CallbackData;
} MageThreadCreateInfo; 

#endif
