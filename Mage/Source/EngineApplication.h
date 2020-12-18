#ifndef MAGE_ENGINE_APPLICATION_H
#define MAGE_ENGINE_APPLICATION_H

#include "Includes.h"
#include "CreateStructures.h"
#include "Window.h"
#include "InputHandler.h"
#include "Renderer.h"

#if MAGE_BUILD_RENDERER_VULKAN
    #include "Vulkan/VulkanRenderer.h"
#endif

typedef struct MageEngineApplication
{
    MageApplicationWindow       *Window;
    MageInputHandler            *InputHandler;
    MageRenderer                *Renderer;
    U8                          Running;
} MageEngineApplication;

extern U8 MageEngineApplicationCreate(MageEngineApplicationCreateInfo *info, MageEngineApplication *engine);
extern U8 MageEngineApplicationDestroy(MageEngineApplication *engine);


#endif