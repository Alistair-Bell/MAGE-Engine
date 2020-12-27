#include "VulkanRenderer.h"


U8 MageShaderCreate(MageShaderCreateInfo *info, MageShader *shader, MageRenderer *renderer)
{
    U8 r = MageFileSystemReadMountedDirectory(&info->ReadInfo, &info->MountedFileSystem);
    MAGE_HANDLE_ERROR_MESSAGE(!r, printf("Error: Cannot read file %s for creating a new shader\n", info->ReadInfo.FilePath));


    VkShaderModuleCreateInfo moduleInfo;
    memset(&moduleInfo, 0, sizeof(VkShaderModuleCreateInfo));
    moduleInfo.sType          = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    return MageTrue;
}
VkShaderStageFlagBits MageVulkanShaderAbstractToNativeType(const MageShaderType type)
{
    switch (type)
    {
        case MAGE_SHADER_TYPE_FRAGMENT:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
        case MAGE_SHADER_TYPE_VERTEX:
            return VK_SHADER_STAGE_VERTEX_BIT;
    }
    return 0;
}
U8 MageShaderDestroy(VkDevice device, MageShader *shader)
{
    return MageTrue;
}


