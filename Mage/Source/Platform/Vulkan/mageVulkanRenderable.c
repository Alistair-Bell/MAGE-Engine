#include "mageVulkanAPI.h"

#if defined (MAGE_VULKAN)


mageResult mageRendererableCreate(struct mageRenderable *renderable, mageRenderablePipeLineMode pipelineMode, struct mageRenderer *renderer)
{
    uint16_t indicies[] = { 0, 1, 2, 2, 3, 0 };
    
 
    mageBufferCreate(&renderable->IndexBuffer, MAGE_BUFFER_TYPE_INDEX, indicies, sizeof(uint16_t) * 6, renderer); 


    return MAGE_RESULT_SUCCESS;
}
void mageRenderableDestroy(struct mageRenderable *renderable, struct mageRenderer *renderer)
{

}



#endif