#include <mageAPI.h>

VkShaderStageFlagBits mageShaderTypeToBit(mageShaderType shaderType)
{
    switch (shaderType)
    {
        case MAGE_SHADER_TYPE_VERTEX:
            return VK_SHADER_STAGE_VERTEX_BIT;
	    case MAGE_SHADER_TYPE_TESSELLATION_CONTROL:
            return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	    case MAGE_SHADER_TYPE_TESSELLATION_EVALUATION:
            return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	    case MAGE_SHADER_TYPE_GEOMETRY:
            return VK_SHADER_STAGE_GEOMETRY_BIT;
	    case MAGE_SHADER_TYPE_FRAGMENT:
            return VK_SHADER_STAGE_FRAGMENT_BIT;
	    case MAGE_SHADER_TYPE_COMPUTE:
            return VK_SHADER_STAGE_COMPUTE_BIT;    
    }
}
mageResult mageShaderCreate(struct mageShader *shader, const char *shaderFile, const char *entryPoint, const mageShaderType shaderType)
{
    shader->FilePath                = shaderFile;
    shader->ShaderType              = shaderType;
    shader->EntryPoint              = entryPoint;
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
    
    VkResult result = MAGE_VULKAN_CHECK(vkCreateShaderModule(device, &createInfo, NULL, &module));
    return module;
}