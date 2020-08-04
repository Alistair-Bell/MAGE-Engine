#include <mageAPI.h>

void mageDescriptorPoolCreate(struct mageRenderer *renderer)
{
    VkDescriptorPoolSize pools;
    memset(&pools, 0, sizeof(pools));
    
    /* Texture stuff */
    pools.type                  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    pools.descriptorCount        = 1;

    VkDescriptorPoolCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkDescriptorPoolCreateInfo));

    createInfo.sType            = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.pPoolSizes       = &pools;
    createInfo.poolSizeCount    = 1;
    createInfo.maxSets          = 10;

    MAGE_VULKAN_CHECK(vkCreateDescriptorPool(renderer->Device, &createInfo, NULL, &renderer->DescriptorPool));
}
void mageDescriptorSetLayoutCreate(struct mageRenderer *renderer)
{
    VkDescriptorSetLayoutBinding textureSampler;
    memset(&textureSampler, 0, sizeof(VkDescriptorSetLayoutBinding));

    textureSampler.binding                              = 0;
    textureSampler.descriptorType                       = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    textureSampler.descriptorCount                      = 1;
    textureSampler.stageFlags                           = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutBinding bindings[] = { textureSampler };
    
    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
    memset(&descriptorSetLayoutCreateInfo, 0, sizeof(VkDescriptorSetLayoutCreateInfo));

    descriptorSetLayoutCreateInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.bindingCount  = 1;
    descriptorSetLayoutCreateInfo.pBindings     = &textureSampler;
    

    MAGE_VULKAN_CHECK(vkCreateDescriptorSetLayout(renderer->Device, &descriptorSetLayoutCreateInfo, NULL, &renderer->DescriptorSetLayout));
}
void mageDescriptorSetsAllocate(struct mageRenderer *renderer)
{
    VkDescriptorSetAllocateInfo allocateInfo;
    memset(&allocateInfo, 0, sizeof(VkDescriptorSetAllocateInfo));

    VkDescriptorSetLayout layouts[] = { renderer->DescriptorSetLayout };

    allocateInfo.sType                  = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocateInfo.descriptorPool         = renderer->DescriptorPool;
    allocateInfo.descriptorSetCount     = 1;
    allocateInfo.pSetLayouts            = layouts;


    MAGE_VULKAN_CHECK(vkAllocateDescriptorSets(renderer->Device, &allocateInfo, &renderer->DescriptorSet));
}
void mageDescriptorWriteCreate(const VkImage *image, const VkImageView *view, const VkSampler *textureSampler, const VkSamplerAddressMode mode, struct mageRenderer *renderer)
{
    VkDescriptorImageInfo imageInfo;
    memset(&imageInfo, 0, sizeof(VkDescriptorImageInfo));

    imageInfo.imageLayout       = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView         = *(view);
    imageInfo.sampler           = *(textureSampler);

    VkWriteDescriptorSet writeSets;
    memset(&writeSets, 0, sizeof(VkWriteDescriptorSet));

    writeSets.sType             = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeSets.dstSet            = renderer->DescriptorSet;
    writeSets.dstBinding        = 0;
    writeSets.dstArrayElement   = 0;
    writeSets.descriptorType    = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    writeSets.descriptorCount   = 1;
    writeSets.pImageInfo        = &imageInfo;
    vkUpdateDescriptorSets(renderer->Device, 1, &writeSets, 0, NULL);
}