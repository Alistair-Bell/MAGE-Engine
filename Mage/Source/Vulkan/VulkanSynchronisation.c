#include "VulkanRenderer.h"

U8 MageVulkanRendererSyncronisationCreate(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    MAGE_HANDLE_ERROR_MESSAGE(info->ConcurrentThreadCount <= 0, printf("Error: Threadcount specified must be greater than 0\n"));

    VkSemaphoreCreateInfo semaphoreInfo;
    memset(&semaphoreInfo, 0, sizeof(VkSemaphoreCreateInfo));
    semaphoreInfo.sType      = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    VkFenceCreateInfo fenceInfo;
    memset(&fenceInfo, 0, sizeof(VkFenceCreateInfo));
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
 
    U32 i;
    renderer->Syncronisation.ThreadCount           = info->ConcurrentThreadCount;
    renderer->Syncronisation.AvailableSemaphores   = calloc(info->ConcurrentThreadCount, sizeof(VkSemaphore));
    renderer->Syncronisation.FinishedSemaphores    = calloc(info->ConcurrentThreadCount, sizeof(VkSemaphore));
    renderer->Syncronisation.ActiveFences          = calloc(info->ConcurrentThreadCount, sizeof(VkFence));
    
    renderer->Syncronisation.ImageFences           = calloc(renderer->SwapChain.ImagesCount, sizeof(VkFence));
    memset(renderer->Syncronisation.ImageFences, VK_NULL_HANDLE, sizeof(VkFence) * renderer->SwapChain.ImagesCount);

    VkResult r1, r2, r3;
    for (i = 0; i < info->ConcurrentThreadCount; i++)
    {
        r1 = vkCreateSemaphore(renderer->Device.LogicalDevice, &semaphoreInfo, NULL, &renderer->Syncronisation.AvailableSemaphores[i]);
        r2 = vkCreateSemaphore(renderer->Device.LogicalDevice, &semaphoreInfo, NULL, &renderer->Syncronisation.FinishedSemaphores[i]);
        r3 = vkCreateFence(renderer->Device.LogicalDevice, &fenceInfo, NULL, &renderer->Syncronisation.ActiveFences[i]); 
        MAGE_HANDLE_ERROR_MESSAGE((r1 != VK_SUCCESS) || (r2 != VK_SUCCESS) || (r3 != VK_SUCCESS), printf("Error: Failed to create syncronisation\n"));
    }    
    return MageTrue;
}
