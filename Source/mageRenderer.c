#include "mageAPI.h"

void *mageRenderableAllocate()
{
	return malloc(sizeof(struct MAGE_RENDERABLE_STRUCT));
}
void mageRenderableInitialse(mageRenderable *renderable, const mageVector3 scale, const mageVector3 position)
{
	renderable->Scale = scale;
	renderable->Position = position;
}
void mageRenderableDestroy(mageRenderable *renderable, void (*freeMethod)(void *item))
{
	if (freeMethod == NULL)
		freeMethod = free;
	freeMethod(renderable);
}









void *mageRendererAllocate()
{
	return malloc(sizeof(struct MAGE_RENDERER_STRUCT));
}
void mageRendererInitialises(mageRenderer *renderer, void (*freeMethod)(void *item), const uint8 flushContents)
{
	renderer->FreeMethod = freeMethod;
	renderer->Flushable = flushContents;
	renderer->PipeLine = mageResizableListAllocate();
	mageResizableListInitialise(renderer->PipeLine, sizeof(struct MAGE_RENDERABLE_STRUCT));
}
void mageRendererDestroy(mageRenderer *renderer)
{
	mageResizableListDestroy(renderer->PipeLine, renderer->FreeMethod);
	renderer->FreeMethod(renderer);
}
void mageRenderPush(mageRenderer *renderer, mageRenderable *renderable)
{
	mageResizableListPush(renderer->PipeLine, renderable);
}
void mageRenderer2D(mageRenderer *renderer)
{
	printf("No override applied\n");
}	
void mageRenderOpenGL2D(mageRenderer *renderer)
{
	printf("OpenGL Render Called\n");
}
void mageRenderVulkan2D(mageRenderer *renderer)
{
	printf("Vulkan Render Called\n");
}
void mageRenderDirextX2D(mageRenderer *renderer)
{
	printf("DirectX Render Called\n");	
}
