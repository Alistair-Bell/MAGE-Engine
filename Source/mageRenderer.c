#include "mageAPI.h"

void *mageRendererAllocate()
{
    return malloc(sizeof(struct MAGE_RENDERER_STRUCT));
}
void mageRendererDeviceInitialise(mageRenderer *renderer, uint8 *success)
{
    #if defined(MAGE_VULKAN)
       
        renderer->PipeLine = mageResizableListAllocate();
        mageResizableListInitialise(renderer->PipeLine, sizeof(struct MAGE_RENDERABLE_STRUCT));


        VkApplicationInfo applicationInfo;
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.apiVersion = VK_VERSION_1_2;
        applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);

        VkInstanceCreateInfo instanceInformation;
        instanceInformation.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInformation.pApplicationInfo = &applicationInfo;

        auto error = vkCreateInstance(&instanceInformation, NULL, &renderer->VulkanInstance);
        
        if (error != VK_SUCCESS)
        {
            mageTryDumpSuccess(0, success);
            return;
        }
        
        uint32 gpuCount = 0;
        vkEnumeratePhysicalDevices(renderer->VulkanInstance, &gpuCount, NULL);
        
        

    #endif
}
void mageRendererInitialise(mageRenderer *renderer, uint8 *success)
{
    #if defined(MAGE_VULKAN)
        mageRendererDeviceInitialise(renderer, success);
    #endif
}
void mageRendererDeviceDestroy(mageRenderer *renderer)
{
    #if defined(MAGE_VULKAN)
        vkDestroyInstance(renderer->VulkanInstance, NULL);
    #endif
}
void mageRendererDestroy(mageRenderer *renderer)
{
    #if defined(MAGE_VULKAN)
        mageRendererDeviceDestroy(renderer);
        mageFreeMethod(renderer);
    #endif
}