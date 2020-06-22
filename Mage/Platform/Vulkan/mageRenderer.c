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
                MAGE_LOG_CLIENT_WARNING("Undefined shader type\n", NULL);
        }
    }
    mageResult mageShaderInitialise(struct mageShader *shader, const char *shaderFile, const char *runtimeFunctionName, const mageShaderType shaderType)
    {
        shader->FilePath            = shaderFile;
        shader->ShaderType          = shaderType;
        shader->RuntimeFunctionName = runtimeFunctionName;
    }
    void mageShaderDestroy(struct mageRenderer *renderer, struct mageShader *shader)
    {
        vkDestroyShaderModule(renderer->Handler.Device, shader->ShaderModule, NULL);
        MAGE_LOG_CORE_INFORM("Destroying shader %s\n", shader->FilePath);
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
    void mageRendererBeginRender(struct mageRenderer *renderer)
    {
        vkAcquireNextImageKHR(renderer->Handler.Device, renderer->SwapChain, UINT64_MAX, VK_NULL_HANDLE, renderer->Fence, &renderer->ActiveSwapChainImageId);
        vkWaitForFences(renderer->Handler.Device, 1, &renderer->Fence, VK_TRUE, UINT64_MAX);
        vkResetFences(renderer->Handler.Device, 1, &renderer->Fence);
        vkQueueWaitIdle(renderer->GraphicsQueue);
    }
    void mageRendererClear(struct mageRenderer *renderer)
    {
        VkClearValue values[2];
        memset(&values, 0, sizeof(VkClearValue) * 2);
        values[0].depthStencil.depth    = 0.0f;
        values[0].depthStencil.stencil  = 0;
        values[1].color.float32[0]		= 1.0f;
        values[1].color.float32[1]		= 0.0f;
        values[1].color.float32[2]		= 0.0f;
		        
        VkRenderPassBeginInfo renderPassBeginInfo;
        memset(&renderPassBeginInfo, 0, sizeof(VkRenderPassBeginInfo));
        renderPassBeginInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassBeginInfo.renderPass      = renderer->RenderPass;
        renderPassBeginInfo.framebuffer     = mageRendererGetActiveFrameBuffer(renderer);
        renderPassBeginInfo.renderArea      = renderer->RenderArea;
        renderPassBeginInfo.clearValueCount = 2;
        renderPassBeginInfo.pClearValues    = values;


        vkCmdBeginRenderPass(renderer->CommandBuffer[0], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdEndRenderPass(renderer->CommandBuffer[0]);
    }
    void mageRendererEndRendering(struct mageRenderer *renderer)
    {
        VkResult result = VK_RESULT_MAX_ENUM;

        VkPresentInfoKHR info;
        memset(&info, 0, sizeof(VkPresentInfoKHR));

        info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores    = &renderer->Semaphore;
        info.swapchainCount     = 1;
        info.pSwapchains        = &renderer->SwapChain;
        info.pImageIndices      = &renderer->ActiveSwapChainImageId;
        info.pResults           = &result;

        vkQueuePresentKHR(renderer->GraphicsQueue, &info);
    }

#endif
