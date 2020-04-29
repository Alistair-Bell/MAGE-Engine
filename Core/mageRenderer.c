#include "mageAPI.h"

void *mageVulkanHandlerAllocate()
{
    return malloc(sizeof(struct MAGE_VULKAN_HANDLER_STRUCT));
}
void mageVulkanHandlerInitialise(mageVulkanHandler *vulkanHandler, uint8 *success)
{
    #if defined(MAGE_VULKAN)
        
        uint8 flag;
    
        
        VkApplicationInfo applicationInformation;
        applicationInformation.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInformation.pNext = NULL;
        applicationInformation.pApplicationName = "Game Engine";
        applicationInformation.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
        applicationInformation.pEngineName = "MAGE";
        applicationInformation.apiVersion = VK_MAKE_VERSION(1, 0, 26);

        VkInstanceCreateInfo creatInformation;
        creatInformation.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        creatInformation.pNext = NULL;
        creatInformation.flags = 0;
        creatInformation.pApplicationInfo = &applicationInformation;
        creatInformation.enabledExtensionCount = 0;
        creatInformation.enabledLayerCount = 0;
        creatInformation.ppEnabledLayerNames = NULL;

        mageVulkanHandlerVulkanInstanceInitialise(vulkanHandler, creatInformation, &flag);
        mageVulkanHandlerGetExtensions(vulkanHandler);
        mageVulkanHandlerGetGPU(vulkanHandler, &flag);
        



        mageTryDumpSuccess(flag, success);

    #endif

}
void mageVulkanHandlerGetExtensions(mageVulkanHandler *vulkanHandler)
{
    #if defined(MAGE_VULKAN)
        uint32 count;
        vkEnumerateInstanceExtensionProperties(NULL, &count, NULL);
        VkExtensionProperties *extensions = calloc(count, sizeof(struct VkExtensionProperties));
        vkEnumerateInstanceExtensionProperties(NULL, &count, extensions);

        vulkanHandler->Extensions = calloc(count, sizeof(char*));
        
        vulkanHandler->ExtensionsCount = count;

        uint32 i;
        for (i = 0; i < count; i++)
        {
            vulkanHandler->Extensions[i] = malloc(strlen(extensions[i].extensionName));
            strcpy(vulkanHandler->Extensions[i], extensions[i].extensionName);
        }
        mageFreeMethod(extensions);
    #endif
}
void mageVulkanHandlerGetGPU(mageVulkanHandler *vulkanHandler, uint8 *success)
{
    #if defined(MAGE_VULKAN)
        
        
        uint32 deviceCount;
        vkEnumeratePhysicalDevices(vulkanHandler->VulkanInstance, &deviceCount, NULL);

        if (deviceCount == 0)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Unable to find physical device\n", NULL);
            mageTryDumpSuccess(0, success);
            return;
        }
        vulkanHandler->GPUCount = deviceCount;
        VkPhysicalDevice *devices = calloc(deviceCount, sizeof(VkPhysicalDevice));
        vkEnumeratePhysicalDevices(vulkanHandler->VulkanInstance, &deviceCount, devices);
        vulkanHandler->GPU = devices[1];
        vkGetPhysicalDeviceProperties(vulkanHandler->GPU, &vulkanHandler->GPUProperties);
        

        MAGE_LOG_CORE_INFORM("Physical device found\n", NULL);

        mageFreeMethod(devices);
        mageTryDumpSuccess(1, success);
    #endif
}
 void mageVulkanHandlerVulkanInstanceInitialise(mageVulkanHandler *vulkanHandler, const VkInstanceCreateInfo instanceInfo, uint8 *success)
{
    #if defined(MAGE_VULKAN)
        VkResult result = vkCreateInstance(&instanceInfo, NULL, &vulkanHandler->VulkanInstance);
        if (result != VK_SUCCESS)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Unable to create vulkan instance\n", NULL);
            mageTryDumpSuccess(0, success);
        }
    
    MAGE_LOG_CORE_INFORM("Vulkan instance has been created succesfully\n", NULL);
    
    mageTryDumpSuccess(1, success);
    
    #endif    
}

void mageVulkanHandlerFree(mageVulkanHandler *vulkanHandler)
{
    uint8 i;
    for (i = 0; i < vulkanHandler->ExtensionsCount; i++) 
    {
        mageFreeMethod(vulkanHandler->Extensions[i]);
    }
    mageFreeMethod(vulkanHandler->Extensions);
}









void *mageRendererAllocate()
{
    return malloc(sizeof(struct MAGE_RENDERER_STRUCT));
}
/*
void mageRendererDeviceInitialise(mageRenderer *renderer, uint8 *success)
{
    #if defined(MAGE_VULKAN)
        {
            
            uint32 gpuCount;
            vkEnumeratePhysicalDevices(renderer->VulkanInstance, &gpuCount, NULL);
            VkPhysicalDevice *devices = malloc(sizeof(VkPhysicalDevice) * gpuCount);
            vkEnumeratePhysicalDevices(renderer->VulkanInstance, &gpuCount, devices);
            renderer->GPU = devices[0];
            renderer->Environment.GPUCount = gpuCount;

            mageFreeMethod(devices);
            
           
            uint32 familyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(renderer->GPU, &familyCount, NULL);
            VkQueueFamilyProperties *familyProperties = malloc(sizeof(VkQueueFamilyProperties) * familyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(renderer->GPU, &familyCount, familyProperties);
            
            uint32 i;
            uint32 found = 0;
            for (i = 0; i < familyCount; i++)
            {
                if (familyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    found = 1;
                    renderer->Environment.GraphicsFamilyIndex = i;
                }
            }
            mageFreeMethod(familyProperties);
            
            switch (found)
            {
            case 1: MAGE_LOG_CORE_INFORM("Graphics family index has been found\n", NULL); break;
            case 2: 
                MAGE_LOG_CORE_FATAL_ERROR("Graphics family index not found!\n", NULL); 
                mageTryDumpSuccess(0, success);
                return;
            }


        }   
        float *queuePriorities = calloc(1, sizeof(float));
        queuePriorities[0] = 1.0f;
       
        VkDeviceQueueCreateInfo deviceQueueCreateInformation;
        deviceQueueCreateInformation.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInformation.queueFamilyIndex = renderer->Environment.GraphicsFamilyIndex;        
        deviceQueueCreateInformation.queueCount = 1;
        deviceQueueCreateInformation.pQueuePriorities = queuePriorities;


        VkDeviceCreateInfo deviceCreateInformation;
        deviceCreateInformation.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInformation.queueCreateInfoCount = 1;
        deviceCreateInformation.pQueueCreateInfos = &deviceQueueCreateInformation;
        
        

        VkResult result;
        
        result = vkCreateDevice(renderer->GPU, &deviceCreateInformation, NULL, &renderer->VulkanDevice);
        
        if (result != VK_SUCCESS)
        {   
            MAGE_LOG_CORE_FATAL_ERROR("Failed to create vulkan device\n", NULL);
            mageTryDumpSuccess(0, success);
            return;
        }

        
        mageTryDumpSuccess(1, success);
        MAGE_LOG_CORE_INFORM("Vulkan device has been succesfully created\n", NULL);
       
        mageFreeMethod(queuePriorities);

    #endif
}
void mageRendererInitialise(mageRenderer *renderer, uint8 *success)
{
    #if defined(MAGE_VULKAN)
        
        VkApplicationInfo applicationInformation;
        VkInstanceCreateInfo creatInformation;
        
        applicationInformation.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInformation.pNext = NULL;
        applicationInformation.pApplicationName = "Game Engine";
        applicationInformation.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
        applicationInformation.pEngineName = "MAGE";
        applicationInformation.apiVersion = VK_MAKE_VERSION(1, 0, 26);

       
        creatInformation.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        creatInformation.pNext = NULL;
        creatInformation.flags = 0;
        creatInformation.pApplicationInfo = &applicationInformation;
        creatInformation.enabledExtensionCount = 0;
        creatInformation.enabledLayerCount = 0;
        creatInformation.ppEnabledLayerNames = NULL;

        VkResult result = vkCreateInstance(&creatInformation, NULL, &renderer->VulkanInstance);
        
        if (result != VK_SUCCESS)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Vulkan instance has failed to initialise\n", NULL);
            mageTryDumpSuccess(0, success);
            return;
        }
        MAGE_LOG_CORE_INFORM("Vulkan instance has been created succesfully\n", NULL);
        
        mageRendererDeviceInitialise(renderer, success);
    #endif
}
void mageRendererDeviceDestroy(mageRenderer *renderer)
{
    #if defined(MAGE_VULKAN)
       vkDestroyDevice(renderer->VulkanDevice, NULL);
    #endif
}
void mageRendererDestroy(mageRenderer *renderer)
{
    #if defined(MAGE_VULKAN)
        mageRendererDeviceDestroy(renderer);
        mageFreeMethod(renderer);
    #endif
}
*/