#include "mageAPI.h"

void *mageRenderableAllocate()
{
	return malloc(sizeof(struct MAGE_RENDERABLE_STRUCT));
}
void mageRenderableInitialse(mageRenderable *renderable, mageVector3 scale, mageVector3 position)
{
	renderable->Scale = scale;
	renderable->Position = position;
	
	renderable->VertexArrayObject = mageVertexArrayAllocate();
	renderable->IndexBufferObject = mageIndexBufferAllocate();
	mageVertexArrayInitialise(renderable->VertexArrayObject);
	float verticies[12] = 
    {
        0, 0, 0,
        0, scale.y, 0,
        scale.x, scale.y, 0,
        scale.x, 0, 0
    };
	mageBuffer *buff = mageBufferAllocate();
	mageBufferInitialise(buff, verticies, 4 * 3, 3);
	mageVertexArrayPush(renderable->VertexArrayObject, buff, 0);
	const uint16 indicies[6] = { 0, 1, 2, 2, 3, 0 };
	mageIndexBufferInitialise(renderable->IndexBufferObject, indicies, 6);
}
void mageRenderableDestroy(mageRenderable *renderable)
{
	mageFreeMethod(renderable);
}

void *mageRendererAllocate()
{
	return malloc(sizeof(struct MAGE_RENDERER_STRUCT));
}
void mageRendererInitialises(mageRenderer *renderer, const uint8 flushContents)
{
	renderer->Flushable = flushContents;
	renderer->PipeLine = mageResizableListAllocate();
	mageResizableListInitialise(renderer->PipeLine, sizeof(struct MAGE_RENDERABLE_STRUCT));
}
void mageRendererDestroy(mageRenderer *renderer)
{
	mageResizableListDestroy(renderer->PipeLine);
}
void mageRenderPush(mageRenderer *renderer, mageRenderable *renderable)
{
	mageResizableListPush(renderer->PipeLine, renderable);
}
void mageRenderer2D(mageRenderer *renderer)
{
	uint32 i, j;
	#ifdef MAGE_OPENGL
		for (i = 0; i < renderer->PipeLine->Quantity - 1; i++)
		{
			
			/* Simple first in first out */ 	
			mageRenderable *current = renderer->PipeLine->Elements[i];
			
			/* Enables all stored shaders */
			for (j = 0; j < current->Shaders->Quantity - 1; i++)
				mageShaderEnable(current->Shaders->Elements[i]);
			
			mageVertexArrayBind(current->VertexArrayObject);	
			mageIndexBufferBind(current->IndexBufferObject);		
		
			GLCall(glDrawElements(GL_TRIANGLES, current->IndexBufferObject->ComponentCount, GL_UNSIGNED_SHORT, 0));
		
			mageVertexArrayUnBind(current->VertexArrayObject);
			mageIndexBufferUnBind(current->IndexBufferObject);
			
			switch(renderer->Flushable)
			{
			/* Does not flush the object */
			case 0:
				break;
			default:
				break;

			}
		}
	#elif defined(MAGE_VULKAN)

	#elif MAGE_DIRECTX

	#endif
}
void mageRendererClear(mageRenderer *renderer)
{
	#ifdef MAGE_OPENGL
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	#endif
}

















