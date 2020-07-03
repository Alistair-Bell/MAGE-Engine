#include <mageAPI.h>

#if defined (MAGE_VULKAN)

void mageRendererRender(struct mageRenderer *renderer)
{
    uint32_t index;
    vkWaitForFences(renderer->Device, 1, &renderer->ConcurentFences[renderer->CurrentFrame], VK_TRUE, UINT64_MAX);
    vkAcquireNextImageKHR(renderer->Device, renderer->SwapChain, UINT64_MAX, renderer->ImageAvailableSemaphores[renderer->CurrentFrame], VK_NULL_HANDLE, &index);
    
    if (renderer->ConcurrentImages[renderer->CurrentFrame] != VK_NULL_HANDLE)
    {
        vkWaitForFences(renderer->Device, 1, &renderer->ConcurrentImages[renderer->CurrentFrame], VK_TRUE, UINT64_MAX);
    }
    renderer->ConcurrentImages[renderer->CurrentFrame] = renderer->ConcurentFences[renderer->CurrentFrame];

    VkSubmitInfo submitInfo;
    memset(&submitInfo, 0, sizeof(VkSubmitInfo));
    VkSemaphore waitSemaphores[] = { renderer->ImageAvailableSemaphores[renderer->CurrentFrame] };
    VkSemaphore signalSemaphores[] = { renderer->RenderFinishedSemaphores[renderer->CurrentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = waitSemaphores;
    submitInfo.pWaitDstStageMask    = waitStages;
    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &renderer->CommandBuffers[index];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;


    

    vkResetFences(renderer->Device, 1, &renderer->ConcurentFences[renderer->CurrentFrame]);
    vkQueueSubmit(renderer->GraphicalQueue, 1, &submitInfo, renderer->ConcurrentImages[renderer->CurrentFrame]);
    
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




















#endif
