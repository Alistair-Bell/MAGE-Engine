#include <mageAPI.h>

#if defined (MAGE_VULKAN)

    VkFramebuffer mageRendererGetActiveFrameBuffer(struct mageRenderer *renderer)
    {
        return renderer->FrameBuffers[renderer->ActiveSwapChainImageId];
    }
    void mageRendererBeginRender(struct mageRenderer *renderer)
    {
        vkAcquireNextImageKHR(renderer->Handler.Device, renderer->SwapChain, UINT64_MAX, VK_NULL_HANDLE, renderer->Fence, &renderer->ActiveSwapChainImageId);
        vkWaitForFences(renderer->Handler.Device, 1, &renderer->Fence, VK_TRUE, UINT64_MAX);
        vkResetFences(renderer->Handler.Device, 1, &renderer->Fence);
        vkQueueWaitIdle(renderer->GraphicsQueue);
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