#include <mageAPI.h>

static uint16_t mageIndexes[] = { 0, 1, 2, 2, 3, 0 };
static struct mageVertex mageVerticies[] = (struct mageVertex[]) 
{ 
    { .Vertex = { .X = -1.0f, .Y = -1.00f },   .Color = { .X = 1.0f, .Y = 0.0f, .Z = 0.0f}, .TextureLocation = { .X = 0.0f, .Y = 0.0f } },  
    { .Vertex = { .X =  1.0f, .Y = -1.00f },    .Color = { .X = 0.0f, .Y = 1.0f, .Z = 0.0f}, .TextureLocation = { .X = 1.0f, .Y = 0.0f } },  
    { .Vertex = { .X =  1.0f, .Y =  1.00f },     .Color = { .X = 0.0f, .Y = 0.0f, .Z = 1.0f}, .TextureLocation = { .X = 1.0f, .Y = 1.0f } },  
    { .Vertex = { .X = -1.0f, .Y =  1.00f },    .Color = { .X = 0.0f, .Y = 0.0f, .Z = 1.0f}, .TextureLocation = { .X = 0.0f, .Y = 1.0f } }
};

static void mageTextureCreateInfoDefault(struct mageTextureCreateInfo *createInfo)
{
    createInfo->SamplerMode = MAGE_TEXTURE_SAMPLER_CLAMP_TO_BORDER;
    createInfo->TexturePath = "Mage/Resources/Textures/Vulkan/Vulkan_White_170px_Dec16.png";
}


mageResult mageRenderableCreate(struct mageRenderable *renderable, struct mageRenderableCreateInfo *info, struct mageRenderer *renderer)
{
    assert(info);
    if (info->IndexCount == 0 || info->Indicies == NULL)
    {
        info->IndexCount    = 6;
        info->Indicies      = mageIndexes; 
    }
    if (info->VertexCount == 0 || info->Verticies == NULL)
    {
        info->VertexCount   = 4;
        info->Verticies     = mageVerticies;
    }
    mageBufferCreate(&renderable->IndexBuffer, MAGE_BUFFER_TYPE_INDEX, info->Indicies, sizeof(uint16_t) * info->IndexCount, renderer); 
    mageBufferCreate(&renderable->VertexBuffer, MAGE_BUFFER_TYPE_VERTEX, info->Verticies, sizeof(struct mageVertex) * info->VertexCount, renderer);
    mageTextureCreate(&renderable->Texture, info->TextureCreateInfo.TexturePath, info->TextureCreateInfo.SamplerMode, renderer);

    MAGE_LOG_CORE_INFORM("Renderable has been created\n", NULL);
    return MAGE_RESULT_SUCCESS;
}
void mageRenderableDestroy(struct mageRenderable *renderable, struct mageRenderer *renderer)
{
    mageBufferDestroy(&renderable->IndexBuffer, renderer);
    mageBufferDestroy(&renderable->VertexBuffer, renderer);
    mageTextureDestroy(&renderable->Texture, renderer);
}
void mageRenderableQuadCreate(struct mageRenderableQuad *quad, struct mageRenderableCreateInfo *info, struct mageRenderer *renderer)
{
    if (info->VertexCount == 0 || info->Verticies == NULL)
    {
        info->VertexCount   = 4;
        info->Verticies     = mageVerticies;
    }
    
    mageBufferCreate(&quad->VertexBuffer, MAGE_BUFFER_TYPE_VERTEX, info->Verticies, sizeof(struct mageVertex) * info->VertexCount, renderer);
    mageTextureCreate(&quad->Texture, info->TextureCreateInfo.TexturePath, info->TextureCreateInfo.SamplerMode, renderer);

    MAGE_LOG_CORE_INFORM("Quad has been created\n", NULL);
}
void mageRenderableQuadDestroy(struct mageRenderableQuad *quad, struct mageRenderer *renderer)
{
    mageBufferDestroy(&quad->VertexBuffer, renderer);
    mageTextureDestroy(&quad->Texture, renderer);
}