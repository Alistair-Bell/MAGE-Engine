#include "VulkanRenderer.h"


U8 MageShaderCreate(MageShaderCreateInfo *info, MageShader *shader, MageRenderer *renderer)
{
    U8 r = MageFileSystemReadMountedDirectory(&info->ReadInfo, &info->MountedFileSystem);
    MAGE_HANDLE_ERROR_MESSAGE(!r, printf("Error: Cannot read file %s for creating a new shader\n", info->ReadInfo.FilePath));
    MageFileSystemReadInfo *ri = &info->ReadInfo;

    VkShaderModuleCreateInfo moduleInfo;
    memset(&moduleInfo, 0, sizeof(VkShaderModuleCreateInfo));
    moduleInfo.sType          = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    moduleInfo.pCode          = (U32 *)ri->StreamData;
    moduleInfo.codeSize       = ri->StreamSize;
    VkResult createResult     = vkCreateShaderModule(renderer->Device.LogicalDevice, &moduleInfo, NULL, &shader->Module);
    
    shader->Type        = info->Type;
    return createResult == VK_SUCCESS;
}
VkPipelineShaderStageCreateInfo MageVulkanShaderCreatePipelineStage(MageShaderCreateInfo *info, MageShader *shader)
{
    VkPipelineShaderStageCreateInfo stageInfo;
    memset(&stageInfo, 0, sizeof(VkPipelineShaderStageCreateInfo));
    stageInfo.sType        = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stageInfo.stage        = MageVulkanShaderAbstractToNativeType(shader->Type);
    stageInfo.module       = shader->Module;
    stageInfo.pName        = info->EntryPoint; 
    return stageInfo;
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
U8 MageShaderDestroy(MageShader *shader, MageRenderer *renderer)
{
    vkDestroyShaderModule(renderer->Device.LogicalDevice, shader->Module, NULL);
    return MageTrue;
}


