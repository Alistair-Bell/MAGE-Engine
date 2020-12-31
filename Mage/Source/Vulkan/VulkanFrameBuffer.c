#include "VulkanRenderer.h"

U8 MageVulkanRendererCreateFrameBuffers(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    U32 i;
    U32 count = renderer->SwapChain.ImagesCount;
    renderer->FrameBuffer.Count   = count;
    renderer->FrameBuffer.Buffers = calloc(count, sizeof(VkFramebuffer) * count);

    for (i = 0; i < renderer->FrameBuffer.Count; i++)
    {
        VkFramebufferCreateInfo bufferInfo;
        memset(&bufferInfo, 0, sizeof(VkFramebufferCreateInfo));
        bufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        bufferInfo.renderPass      = renderer->Pipeline.RenderPass;
        bufferInfo.pAttachments    = &renderer->SwapChain.ImageViews[i];
        bufferInfo.attachmentCount = 1;
        bufferInfo.layers          = 1;
        bufferInfo.width           = renderer->SwapChain.CurrentExtent.width;
        bufferInfo.height          = renderer->SwapChain.CurrentExtent.height;
        VkResult r = vkCreateFramebuffer(renderer->Device.LogicalDevice, &bufferInfo, 0, &renderer->FrameBuffer.Buffers[i]);
        MAGE_HANDLE_ERROR_MESSAGE(r != VK_SUCCESS, printf("Error: Unable to create framebuffer %d of %d\n", i + 1, renderer->FrameBuffer.Count));
    }
    return MageTrue;
}
