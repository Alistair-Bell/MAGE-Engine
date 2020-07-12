#include <mageAPI.h>

#if defined (MAGE_VULKAN)

VkShaderStageFlagBits mageShaderTypeToBit(mageShaderType shaderType)
{
    switch (shaderType)
    {
        case MAGE_VERTEX_SHADER:
            return VK_SHADER_STAGE_VERTEX_BIT;
	    case MAGE_TESSELLATION_CONTROL_SHADER:
            return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	    case MAGE_TESSELLATION_EVALUATION_SHADER:
            return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	    case MAGE_GEOMETRY_SHADER:
            return VK_SHADER_STAGE_GEOMETRY_BIT;
	    case MAGE_FRAGMENT_SHADER:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
	    case MAGE_COMPUTE_SHADER:
            return VK_SHADER_STAGE_COMPUTE_BIT;    
    }
}
mageResult mageShaderInitialise(struct mageShader *shader, const char *shaderFile, const char *runtimeFunctionName, const mageShaderType shaderType)
{
    shader->FilePath                = shaderFile;
    shader->ShaderType              = shaderType;
    shader->RuntimeFunctionName     = runtimeFunctionName;
    return MAGE_RESULT_SUCCESS;
}
VkShaderModule mageShaderCreateModule(struct mageShader *shader, VkDevice device)
{
    uint32_t codeSize;
    char *code = mageFileReadContents(shader->FilePath, "rb", &codeSize);
    
    VkShaderModule module;
    VkShaderModuleCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkShaderModuleCreateInfo));
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pCode = (uint32_t *)code;
    createInfo.codeSize = codeSize;
    
    VkResult result = MAGE_CHECK_VULKAN(vkCreateShaderModule(device, &createInfo, NULL, &module));
    return module;
}

#endif