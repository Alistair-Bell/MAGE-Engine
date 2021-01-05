#ifndef RENDERER_H
#define RENDERER_H

/* Contains generic methods for the renderer */

#include "Includes.h"
#include "CreateStructures.h"

extern U8 MageRendererCreate(MageRendererCreateInfo *info, MageRenderer *renderer);
extern U8 MageRendererHandleWindowResize(MageRendererResizeHandleInfo *info, MageRenderer *renderer);
extern U8 MageRendererRecordHardCoded(MageRenderer *renderer, U32 recorderIndex);
extern U8 MageRendererPresentRecorded(MageRenderer *renderer);
extern U8 MageRendererDestroy(MageRenderer *renderer);

#endif
