#include "VulkanShaders.h"


U8 MageVulkanShaderCreate(MageVulkanShaderCreateInfo *info, MageVulkanShader *shader)
{
    VkShaderModuleCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkShaderModuleCreateInfo));
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.flags = MageVulkanShaderAbstractToNativeType(info->Type);
    
    MageFileSystemReadInfo ri;
    memset(&ri, 0, sizeof(MageFileSystemReadInfo));
    ri.FilePath                 = info->ShaderFile;
    ri.SearchOverride           = MageFalse;
    
    U8 readResult = MageFileSystemReadMountedDirectory(&info->MountedFileSystem, &ri);
    MAGE_HANDLE_ERROR_MESSAGE(!readResult, printf("Error: Unable to create shader, cannot find file in mounted system\n"));
   
    createInfo.pCode =      (U32 *)ri.StreamData;
    createInfo.codeSize =   ri.StreamSize * sizeof(U32);

    VkResult shaderCreateResult = vkCreateShaderModule(info->DesiredDevice, &createInfo, NULL, &shader->RawModule);
    return (shaderCreateResult == VK_SUCCESS);
}
VkShaderStageFlagBits MageVulkanShaderAbstractToNativeType(const MageVulkanShaderType type)
{
    switch (type)
    {
        case MAGE_VULKAN_SHADER_TYPE_FRAGMENT:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
        case MAGE_VULKAN_SHADER_TYPE_VERTEX:
            return VK_SHADER_STAGE_VERTEX_BIT;
        default:
            printf("Error: Undefined shader type %d, see enum for references\n", (U32)type); break;
    }
    return VK_SHADER_STAGE_VERTEX_BIT;
}
U8 MageVulkanShaderDestroy(MageVulkanShader *shader)
{

    return MageTrue;
}
