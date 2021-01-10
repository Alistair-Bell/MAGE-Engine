#include "VulkanRenderer.h"

static U32 MageVulkanRendererRenderPassGetColorAttachment(VkAttachmentDescription *description, VkAttachmentReference *reference, MageRenderer *renderer, U32 count)
{
    memset(description, 0, sizeof(VkAttachmentDescription));
    description->format         = renderer->SwapChain.PrimaryFormat;
    description->samples        = VK_SAMPLE_COUNT_1_BIT;
    description->loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    description->storeOp        = VK_ATTACHMENT_STORE_OP_STORE; 
    description->stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    description->stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;   
    description->initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    description->finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; 

    memset(reference, 0, sizeof(VkAttachmentReference));
    reference->layout           = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    reference->attachment       = 0;
    return UINT32_MAX;
}

U8 MageVulkanRendererRenderPassCreate(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    VkAttachmentDescription description;
    VkAttachmentReference   reference;
    U32 count = 0;
    MageVulkanRendererRenderPassGetColorAttachment(&description, &reference, renderer, count);

    VkSubpassDescription subpassDescription;
    memset(&subpassDescription, 0, sizeof(VkSubpassDescription));
    subpassDescription.pipelineBindPoint            = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.pColorAttachments            = &reference;
    subpassDescription.colorAttachmentCount         = 1; /* Used by the shader location 0 = vec4 outColor */

    VkRenderPassCreateInfo renderPassInfo;
    memset(&renderPassInfo, 0, sizeof(VkRenderPassCreateInfo));
    renderPassInfo.sType                            = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.pSubpasses                       = &subpassDescription;
    renderPassInfo.subpassCount                     = 1;
    renderPassInfo.pAttachments                     = &description;
    renderPassInfo.attachmentCount                  = 1;

    VkResult r = vkCreateRenderPass(renderer->Device.LogicalDevice, &renderPassInfo, NULL, &renderer->Pipeline.RenderPass);
    return (r == VK_SUCCESS);
}
