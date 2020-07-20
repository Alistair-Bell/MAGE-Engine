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
void mageRendererRender(struct mageRenderer *renderer)
{
    uint32_t index;
    register uint32_t currentFrame = renderer->CurrentFrame;
    vkWaitForFences(renderer->Device, 1, &renderer->ConcurentFences[currentFrame], VK_TRUE, UINT64_MAX);
    vkAcquireNextImageKHR(renderer->Device, renderer->SwapChain, UINT64_MAX, renderer->ImageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &index);
    
    if (renderer->ConcurrentImages[currentFrame] != VK_NULL_HANDLE)
    {
        vkWaitForFences(renderer->Device, 1, &renderer->ConcurrentImages[currentFrame], VK_TRUE, UINT64_MAX);
    }
    renderer->ConcurrentImages[currentFrame] = renderer->ConcurentFences[currentFrame];

    VkSubmitInfo submitInfo;
    memset(&submitInfo, 0, sizeof(VkSubmitInfo));
    VkSemaphore waitSemaphores[] = { renderer->ImageAvailableSemaphores[currentFrame] };
    VkSemaphore signalSemaphores[] = { renderer->RenderFinishedSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = waitSemaphores;
    submitInfo.pWaitDstStageMask    = waitStages;
    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &renderer->CommandBuffers[index];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;
    vkResetFences(renderer->Device, 1, &renderer->ConcurentFences[currentFrame]);
    vkQueueSubmit(renderer->GraphicalQueue, 1, &submitInfo, renderer->ConcurrentImages[currentFrame]);
    
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
    
    renderer->CurrentFrame = (currentFrame + 1) % renderer->ConcurentFrames;
}

