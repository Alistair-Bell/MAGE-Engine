#include <mageAPI.h>


#if defined (MAGE_VULKAN)

    VkShaderStageFlagBits mageShaderTypeToBit(const mageShaderType shaderType)
    {
        switch (shaderType)
        {
            case MAGE_VERTEX_SHADER:
                return VK_SHADER_STAGE_VERTEX_BIT;
            
            case MAGE_TESSELLATION_CONTROL_SHADER:
                return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
            
            case MAGE_FRAGMENT_SHADER:
                return VK_SHADER_STAGE_FRAGMENT_BIT;
            
            case MAGE_GEOMETRY_SHADER:
                return VK_SHADER_STAGE_GEOMETRY_BIT;						
	        
            case MAGE_TESSELLATION_EVALUATION_SHADER:
                return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;						
	        
            case MAGE_COMPUTE_SHADER:
                return VK_SHADER_STAGE_COMPUTE_BIT;

            default:
                MAGE_LOG_CORE_ERROR("Undefined shader type\n", NULL);
                return;
        }
    }
    mageResult mageShaderInitialise(struct mageShader *shader, const char *shaderFile, const char *runtimeFunctionName, const mageShaderType shaderType)
    {
        shader->FilePath            = shaderFile;
        shader->ShaderType          = shaderType;
        shader->RuntimeFunctionName = runtimeFunctionName;
    }
    VkFramebuffer mageRendererGetActiveFrameBuffer(struct mageRenderer *renderer)
    {
        return renderer->FrameBuffers[renderer->ActiveSwapChainImageId];
    }
    VkShaderModule mageShaderCreateModule(VkDevice device, const char *file)
    {
        VkShaderModule module;
        uint32_t codeSize;
        char *code = mageFileReadContents(file, "rb", &codeSize);
        if (code == NULL) return module;
        
        VkShaderModuleCreateInfo shaderCreateInfo;
        memset(&shaderCreateInfo, 0, sizeof(VkShaderModuleCreateInfo));
        shaderCreateInfo.sType        = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderCreateInfo.codeSize     = codeSize; 
        shaderCreateInfo.pCode        = (uint32_t*)code;

        vkCreateShaderModule(device, &shaderCreateInfo, NULL, &module);
        free(code);
        return module;
    }
    void mageRendererRender(struct mageRenderer *renderer)
    {

        int32_t imageIndex;
        /* 
        vkWaitForFences(renderer->Handler.Device, 1, renderer->Fences[renderer->CurrentFrame], VK_TRUE, UINT64_MAX);
        vkAcquireNextImageKHR(renderer->Handler.Device, renderer->SwapChain, UINT64_MAX, renderer->AvailableSemaphores[renderer->CurrentFrame], VK_NULL_HANDLE, &imageIndex); */
    
        
        
        
        
        
        
        
        
        
        renderer->CurrentFrame = (renderer->CurrentFrame + 1) % renderer->MaxImagesInFlight;
    }
    void mageRendererClear(struct mageRenderer *renderer)
    {
    
    }
    void mageRendererEndRendering(struct mageRenderer *renderer)
    {
    
    
    }

#endif
