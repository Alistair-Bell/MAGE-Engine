#include <mageAPI.h>

mageResult mageRenderableCreate(struct mageRenderable *renderable, struct mageRenderableCreateInfo *info, struct mageRenderer *renderer)
{
    assert(info);
    if (info->IndexCount == 0 || info->Indicies == NULL)
    {
        info->IndexCount    = 6;
        info->Indicies      = (uint16_t[]) { 0, 1, 2, 2, 3, 0 }; 
    }
    if (info->VertexCount == 0 || info->Verticies == NULL)
    {
        info->VertexCount   = 4;
        info->Verticies     = (struct mageVertex[])
        { 
            { .Vertex = { .X = -0.5f, .Y = -0.5f }, .Color = { .X = 1.0f, .Y = 0.0f, .Z = 0.0f} }, 
            { .Vertex = { .X = 0.5f, .Y = -0.5f }, .Color = { .X = 0.0f, .Y = 1.0f, .Z = 0.0f } },
            { .Vertex = { .X = 0.5f, .Y = 0.5f }, .Color = { .X = 0.0f, .Y = 0.0f, .Z = 1.0f  } },
            { .Vertex = { .X = -0.5f, .Y = 0.5f }, .Color = { .X = 0.0f, .Y = 0.0f, .Z = 1.0f } }
        };
    }
    if (info->TextureCreateInfo == NULL)
    {
        info->TextureCreateInfo = &(struct mageTextureCreateInfo) 
        { 
            .SamplerMode = MAGE_TEXTURE_SAMPLER_CLAMP_TO_BORDER, 
            .TexturePath = "Mage/Resources/Textures/Vulkan/Vulkan_White_170px_Dec16.png" 
        };
    }

    mageBufferCreate(&renderable->IndexBuffer, MAGE_BUFFER_TYPE_INDEX, info->Indicies, sizeof(uint16_t) * info->IndexCount, renderer); 
    mageBufferCreate(&renderable->VertexBuffer, MAGE_BUFFER_TYPE_VERTEX, info->Verticies, sizeof(struct mageVertex) * info->VertexCount, renderer);
    mageTextureCreate(&renderable->Texture, info->TextureCreateInfo->TexturePath, info->TextureCreateInfo->SamplerMode, MAGE_TEXTURE_FILE_FORMAT_JPEG, renderer);

    MAGE_LOG_CORE_INFORM("Renderable has been created\n", NULL);
    return MAGE_RESULT_SUCCESS;
}
void mageRenderableDestroy(struct mageRenderable *renderable, struct mageRenderer *renderer)
{
    mageBufferDestroy(&renderable->IndexBuffer, renderer);
    mageBufferDestroy(&renderable->VertexBuffer, renderer);
    mageTextureDestroy(&renderable->Texture, renderer);
}
