#include <mageAPI.h>

void mageTextureCreate(struct mageTexture *texture, const char *texturePath, mageTextureSamplerMode samplerMode, struct mageRenderer *renderer)
{
    uint32_t fileSize;
    char *fileData = mageFileReadContents(texturePath, "r", &fileSize);
    VkFormat textureFormat = VK_FORMAT_R8G8B8A8_UNORM;


    free(fileData);
}
void mageTextureDestroy(struct mageTexture *texture, struct mageRenderer *renderer)
{


}
