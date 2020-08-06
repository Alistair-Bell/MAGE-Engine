#include <mageAPI.h>

VkCommandBuffer mageCommandBufferBegin(struct mageRenderer *renderer)
{
    VkCommandBufferAllocateInfo allocateInfo;
    memset(&allocateInfo, 0, sizeof(VkCommandBufferAllocateInfo));
    allocateInfo.sType                 = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.level                 = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandPool           = renderer->CommandPool;
    allocateInfo.commandBufferCount    = 1;

    VkCommandBuffer commandBuffer;
    MAGE_VULKAN_CHECK(vkAllocateCommandBuffers(renderer->Device, &allocateInfo, &commandBuffer));

    VkCommandBufferBeginInfo beginInfo;
    memset(&beginInfo, 0, sizeof(VkCommandBufferBeginInfo));
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}
void mageCommandBufferEnd(VkCommandBuffer commandBuffer, struct mageRenderer *renderer)
{
    vkEndCommandBuffer(commandBuffer);
    VkSubmitInfo submitInfo;
    memset(&submitInfo, 0, sizeof(VkSubmitInfo));

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(renderer->GraphicalQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(renderer->GraphicalQueue);

    vkFreeCommandBuffers(renderer->Device, renderer->CommandPool, 1, &commandBuffer);
}
static uint32_t mageRendererPrepare(struct mageRenderer *renderer, VkCommandBufferBeginInfo *bufferBeginInfo, VkRenderPassBeginInfo *passBeginInfo)
{
    uint32_t index, i;
    vkWaitForFences(renderer->Device, 1, &renderer->FencesInUse[renderer->CurrentFrame], VK_TRUE, UINT64_MAX);
    vkAcquireNextImageKHR(renderer->Device, renderer->SwapChain, UINT64_MAX, renderer->WaitSemaphores[renderer->CurrentFrame], VK_NULL_HANDLE, &index);
    
    bufferBeginInfo->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    
    passBeginInfo->sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    passBeginInfo->framebuffer       = renderer->Framebuffers[index];
    passBeginInfo->clearValueCount   = 1;
    passBeginInfo->pClearValues      = &renderer->ClearValue;
    passBeginInfo->renderPass        = renderer->PrimaryRenderPass;
    passBeginInfo->renderArea        = renderer->RenderArea;

    if (renderer->SwapChainImagesInUse[renderer->CurrentFrame] != VK_NULL_HANDLE)
    {
        vkWaitForFences(renderer->Device, 1, &renderer->SwapChainImagesInUse[renderer->CurrentFrame], VK_TRUE, UINT64_MAX);
    }
    renderer->SwapChainImagesInUse[renderer->CurrentFrame] = renderer->FencesInUse[renderer->CurrentFrame];

    return index;
}
static void mageRendererPresent(struct mageRenderer *renderer, const uint32_t index)
{
    VkSubmitInfo submitInfo;
    memset(&submitInfo, 0, sizeof(VkSubmitInfo));
    
    VkSemaphore waitSemaphores[] = { renderer->WaitSemaphores[renderer->CurrentFrame] };
    VkSemaphore signalSemaphores[] = { renderer->SignalSemaphores[renderer->CurrentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = waitSemaphores;
    submitInfo.pWaitDstStageMask    = waitStages;
    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &renderer->CommandBuffers[index];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;
    vkResetFences(renderer->Device, 1, &renderer->FencesInUse[renderer->CurrentFrame]);
    vkQueueSubmit(renderer->GraphicalQueue, 1, &submitInfo, renderer->SwapChainImagesInUse[renderer->CurrentFrame]);
    
    VkPresentInfoKHR presentInfo;
    memset(&presentInfo, 0, sizeof(VkPresentInfoKHR));

    presentInfo.sType               = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount  = 1;
    presentInfo.pWaitSemaphores     = signalSemaphores;

    VkSwapchainKHR swapChains[] = { renderer->SwapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &index;
    
    vkQueuePresentKHR(renderer->PresentQueue, &presentInfo);
    
    renderer->CurrentFrame = (renderer->CurrentFrame + 1) % renderer->ConcurentFrames;
}
static void mageRendererRecordRenderables(VkCommandBuffer buffer, VkRenderPassBeginInfo beginInfo, VkCommandBufferBeginInfo bufferBeginInfo, VkPipeline pipeline, VkPipelineLayout layout, VkDescriptorSet set, struct mageRenderable **renderables, const uint32_t count)
{
    uint32_t i;
    vkBeginCommandBuffer(buffer, &bufferBeginInfo);
    vkCmdBeginRenderPass(buffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
        for (i = 0; i < count; i++)
        {
            VkBuffer useBuffers[] = { mageBufferGetNativeBuffer(&renderables[i]->VertexBuffer) };
            VkDeviceSize offsets[] = { 0 };
            

            
            /* Binding relative information */
            vkCmdBindVertexBuffers(buffer, 0, 1, useBuffers, offsets);
            vkCmdBindIndexBuffer(buffer, mageBufferGetNativeBuffer(&renderables[i]->IndexBuffer), 0, VK_INDEX_TYPE_UINT16);
            vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &set, 0, NULL);
            vkCmdDrawIndexed(buffer, 6, 1, 0, 0, 0);
        }
    vkCmdEndRenderPass(buffer);
    vkEndCommandBuffer(buffer);

}
static void mageRendererRecordQuads(VkBuffer sharedIndexBuffer, VkCommandBuffer buffer, VkRenderPassBeginInfo beginInfo, VkCommandBufferBeginInfo bufferBeginInfo, VkPipeline pipeline, VkPipelineLayout layout, VkDescriptorSet set, struct mageRenderableQuad **quads, const uint32_t count)
{
    uint32_t i;
    vkBeginCommandBuffer(buffer, &bufferBeginInfo);
    vkCmdBeginRenderPass(buffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
        for (i = 0; i < count; i++)
        {
            VkBuffer useBuffers[] = { mageBufferGetNativeBuffer(&quads[i]->VertexBuffer) };
            VkDeviceSize offsets[] = { 0 };
            


            /* Binding relative information */
            vkCmdBindVertexBuffers(buffer, 0, 1, useBuffers, offsets);
            vkCmdBindIndexBuffer(buffer, sharedIndexBuffer, 0, VK_INDEX_TYPE_UINT16);
            vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &set, 0, NULL);
            vkCmdDrawIndexed(buffer, 6, 1, 0, 0, 0);

        }
    vkCmdEndRenderPass(buffer);
    vkEndCommandBuffer(buffer);
}
void mageRendererDrawQuads(struct mageRenderer *renderer, struct mageRenderableQuad **quads, const uint32_t count)
{
    VkCommandBufferBeginInfo bufferBeginInfo;
    VkRenderPassBeginInfo passBeginInfo;
    memset(&bufferBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));
    memset(&passBeginInfo, 0, sizeof(VkRenderPassBeginInfo));
    uint32_t index = mageRendererPrepare(renderer, &bufferBeginInfo, &passBeginInfo);

    mageRendererRecordQuads(mageBufferGetNativeBuffer(renderer->DefaultSquareIndexBuffer), renderer->CommandBuffers[index], passBeginInfo, bufferBeginInfo, renderer->GraphicsPipeline, renderer->GraphicsPipelineLayout, renderer->DescriptorSet, quads, count);
    mageRendererPresent(renderer, index);
}


void mageRendererDrawRenderables(struct mageRenderer *renderer, struct mageRenderable **renderables, const uint32_t count)
{
    VkCommandBufferBeginInfo bufferBeginInfo;
    VkRenderPassBeginInfo passBeginInfo;
    memset(&bufferBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));
    memset(&passBeginInfo, 0, sizeof(VkRenderPassBeginInfo));
    uint32_t index = mageRendererPrepare(renderer, &bufferBeginInfo, &passBeginInfo);

    mageRendererRecordRenderables(renderer->CommandBuffers[index], passBeginInfo, bufferBeginInfo, renderer->GraphicsPipeline, renderer->GraphicsPipelineLayout, renderer->DescriptorSet, renderables, count);
    mageRendererPresent(renderer, index);
}

