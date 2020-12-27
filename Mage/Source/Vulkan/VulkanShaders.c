#include "VulkanShaders.h"

U8 MageVulkanShaderCreate(MageVulkanShaderCreateInfo *info, MageVulkanShader *shader)
{
    MageFileSystemReadInfo ri;
    memset(&ri, 0, sizeof(MageFileSystemReadInfo));
    ri.FilePath                 = info->ShaderFile;
    ri.SearchOverride           = MageFalse;
    
    U8 readResult = MageFileSystemReadMountedDirectory(&info->MountedFileSystem, &ri);
    MAGE_HANDLE_ERROR_MESSAGE(!readResult, printf("Error: Unable to create shader, cannot find file in mounted system\n"));
   
    VkShaderModuleCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkShaderModuleCreateInfo));
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.flags = MageVulkanShaderAbstractToNativeType(info->Type);
    createInfo.pCode =      (U32 *)ri.StreamData;
    createInfo.codeSize =   ri.StreamSize * sizeof(U32);

    VkResult shaderCreateResult = vkCreateShaderModule(info->DesiredDevice, &createInfo, NULL, &shader->RawModule);
    free(ri.StreamData);
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
VkPipelineShaderStageCreateInfo MageVulkanShaderGeneratePipelineStage(MageVulkanShaderCreateInfo *info, VkShaderModule module)
{
    VkPipelineShaderStageCreateInfo returnValue;
    memset(&returnValue, 0, sizeof(VkPipelineShaderStageCreateInfo));
    returnValue.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    returnValue.stage  = MageVulkanShaderAbstractToNativeType(info->Type);
    returnValue.pName  = info->EntryPoint;
    returnValue.module = module;
    return returnValue;
}
U8 MageVulkanShaderDestroy(VkDevice device, MageVulkanShader *shader)
{
    vkDestroyShaderModule(device, shader->RawModule, NULL); 
    return MageTrue;
}
