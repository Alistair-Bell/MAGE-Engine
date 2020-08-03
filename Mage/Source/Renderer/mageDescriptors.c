#include <mageAPI.h>

VkDescriptorSetLayout mageDescriptorSetLayoutCreate(struct mageRenderer *renderer)
{
    VkDescriptorSetLayout descriptorLayout;
    VkDescriptorSetLayoutBinding layout;
    memset(&layout, 0, sizeof(VkDescriptorSetLayoutBinding));

    layout.binding                              = 0;
    layout.descriptorType                       = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    layout.descriptorCount                      = 1;
    layout.stageFlags                           = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
    memset(&descriptorSetLayoutCreateInfo, 0, sizeof(VkDescriptorSetLayoutCreateInfo));

    descriptorSetLayoutCreateInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.bindingCount  = 1;
    descriptorSetLayoutCreateInfo.pBindings     = &layout;

    MAGE_VULKAN_CHECK(vkCreateDescriptorSetLayout(renderer->Device, &descriptorSetLayoutCreateInfo, NULL, &descriptorLayout));
    return descriptorLayout;
}
