#include "VulkanRenderer.h"

U8 MageVulkanRendererCreateCommandBuffers(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    U32 count = renderer->FrameBuffer.Count;
    renderer->CommandRecorders.Residents = calloc(count, sizeof(VkCommandBuffer));

    VkCommandPoolCreateInfo poolInfo;
    memset(&poolInfo, 0, sizeof(VkCommandPoolCreateInfo));
    poolInfo.sType               = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex    = renderer->Device.QueueFamilies.GraphicsFamilyIndex;
    VkResult r = vkCreateCommandPool(renderer->Device.LogicalDevice, &poolInfo, NULL, &renderer->CommandRecorders.Pool);

    VkCommandBufferAllocateInfo allocInfo;
    memset(&allocInfo, 0, sizeof(VkCommandBufferAllocateInfo));
    allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool        = renderer->CommandRecorders.Pool;
    allocInfo.commandBufferCount = count;
    
    r = vkAllocateCommandBuffers(renderer->Device.LogicalDevice, &allocInfo, renderer->CommandRecorders.Residents);
    MAGE_HANDLE_ERROR_MESSAGE(r != VK_SUCCESS, printf("Error: Unable to allocate %d command buffers to primary pool\n", count));
    return MageTrue;
}
