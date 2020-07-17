#include <mageAPI.h>

mageResult mageRendererableCreate(struct mageRenderable *renderable, mageRenderablePipeLineMode pipelineMode, struct mageRenderer *renderer)
{
    uint16_t indicies[] = { 0, 1, 2, 2, 3, 0 };
    struct mageVertex vertexes[] = { 
    { .Vertex = { .x = -0.5f, .y = -0.5f }, .Color = { .x = 1.0f, .y = 0.0f, .z = 0.0f} },
    { .Vertex = { .x = 0.5f, .y = -0.5f }, .Color = { .x = 0.0f, .y = 1.0f, .z = 0.0f} },
    { .Vertex = { .x = 0.5f, .y = 0.5f }, .Color = { .x = 0.0f, .y = 0.0f, .z = 1.0f} },
    { .Vertex = { .x = -0.5f, .y = 0.5f }, .Color = { .x = 0.0f, .y = 0.0f, .z = 1.0f} }};

    mageBufferCreate(&renderable->IndexBuffer, MAGE_BUFFER_TYPE_INDEX, indicies, sizeof(uint16_t) * 6, renderer); 
    mageBufferCreate(&renderable->VertexBuffer, MAGE_BUFFER_TYPE_VERTEX, vertexes, sizeof(struct mageVertex) * 4, renderer);
    return MAGE_RESULT_SUCCESS;
}
void mageRenderableDestroy(struct mageRenderable *renderable, struct mageRenderer *renderer)
{
    mageBufferDestroy(&renderable->IndexBuffer, renderer);
    mageBufferDestroy(&renderable->VertexBuffer, renderer);
}
