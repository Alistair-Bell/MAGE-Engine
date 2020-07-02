#include <mageAPI.h>

#if defined (MAGE_VULKAN)

void mageRendererRender(struct mageRenderer *renderer)
{
    uint32_t index;
    vkAcquireNextImageKHR(renderer->Device, renderer->SwapChain, UINT64_MAX, renderer->ImageAvailableSemaphore, VK_NULL_HANDLE, &index);
    
    VkSubmitInfo submitInfo;
    memset(&submitInfo, 0, sizeof(VkSubmitInfo));
    VkSemaphore waitSemaphores[] = { renderer->ImageAvailableSemaphore };
    VkSemaphore signalSemaphores[] = { renderer->RenderFinishedSemaphore };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount   = 1;
    submitInfo.pWaitSemaphores      = waitSemaphores;
    submitInfo.pWaitDstStageMask    = waitStages;
    submitInfo.commandBufferCount   = 1;
    submitInfo.pCommandBuffers      = &renderer->CommandBuffers[index];
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;


    VkPresentInfoKHR presentInfo;
    memset(&presentInfo, 0, sizeof(VkPresentInfoKHR));

    presentInfo.sType               = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount  = 1;
    presentInfo.pWaitSemaphores     = signalSemaphores;

    VkSwapchainKHR swapChains[] = { renderer->SwapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &index;

    vkQueueSubmit(renderer->GraphicalQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueuePresentKHR(renderer->PresentQueue, &presentInfo);
    vkQueueWaitIdle(renderer->PresentQueue);


   
}























#endif
