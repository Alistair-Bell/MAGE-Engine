#ifndef MAGE_ENGINE_APPLICATION_H
#define MAGE_ENGINE_APPLICATION_H

#include "Includes.h"
#include "CreateStructures.h"
#include "Window.h"

typedef struct MageEngineApplication
{
    MageApplicationWindow       *Window;
    U8                          Running;
} MageEngineApplication;

extern U8 MageEngineApplicationCreate(MageEngineApplicationCreateInfo *info, MageEngineApplication *engine);
extern U8 MageEngineApplicationDestroy(MageEngineApplication *engine);


#endif