#include <mageAPI.h>

mageResult mageRenderableCreate(struct mageRenderable *renderable, mageRenderablePipeLineMode pipelineMode, struct mageRenderer *renderer)
{
    uint16_t indicies[] = { 0, 1, 2, 2, 3, 0 };
    struct mageVertex vertexes[] = { 
    { .Vertex = { .x = -0.5f, .y = -0.5f }, .Color = { .x = 1.0f, .y = 0.0f, .z = 0.0f} },
    { .Vertex = { .x = 0.5f, .y = -0.5f }, .Color = { .x = 0.0f, .y = 1.0f, .z = 0.0f} },
    { .Vertex = { .x = 0.5f, .y = 0.5f }, .Color = { .x = 0.0f, .y = 0.0f, .z = 1.0f} },
    { .Vertex = { .x = -0.5f, .y = 0.5f }, .Color = { .x = 0.0f, .y = 0.0f, .z = 1.0f} }};

    mageBufferCreate(&renderable->IndexBuffer, MAGE_BUFFER_TYPE_INDEX, indicies, sizeof(uint16_t) * 6, renderer); 
    mageBufferCreate(&renderable->VertexBuffer, MAGE_BUFFER_TYPE_VERTEX, vertexes, sizeof(struct mageVertex) * 4, renderer);
    mageTextureCreate(&renderable->Texture, "Mage/Resources/Textures/Vulkan/Vulkan_White_170px_Dec16.png", MAGE_TEXTURE_SAMPLER_CLAMP_TO_EDGE, MAGE_TEXTURE_FILE_FORMAT_PNG, renderer);
    MAGE_LOG_CORE_INFORM("Renderable has been created\n", NULL);
    return MAGE_RESULT_SUCCESS;
}
void mageRenderableDestroy(struct mageRenderable *renderable, struct mageRenderer *renderer)
{
    mageBufferDestroy(&renderable->IndexBuffer, renderer);
    mageBufferDestroy(&renderable->VertexBuffer, renderer);
    mageTextureDestroy(&renderable->Texture, renderer);
}
