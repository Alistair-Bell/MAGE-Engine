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

        VkInstanceCreateInfo createInformation;
        createInformation.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInformation.pNext = NULL;
        createInformation.flags = 0;
        createInformation.pApplicationInfo = &applicationInformation;
        createInformation.enabledExtensionCount = 0;
        createInformation.enabledLayerCount = 0;
        createInformation.ppEnabledLayerNames = NULL;
        
        
        mageVulkanHandlerVulkanInstanceInitialise(vulkanHandler, createInformation, &flag);
        mageVulkanHandlerGetGPU(vulkanHandler, &flag);
    
        mageVulkanHandlerGPUInitialise(vulkanHandler, &flag);
        mageVulkanHandlerGetPhysicalLayerProperties(vulkanHandler, success); 
        
        
        mageTryDumpSuccess(flag, success);

    #endif

}
void mageVulkanHandlerGetInstanceExtensions(mageVulkanHandler *vulkanHandler)
{
    #if defined(MAGE_VULKAN)
        
        vkEnumerateInstanceExtensionProperties(NULL, &vulkanHandler->InstanceExtensionsCount, NULL);
        VkExtensionProperties *extensions = calloc(vulkanHandler->InstanceExtensionsCount, sizeof(struct VkExtensionProperties));
        vkEnumerateInstanceExtensionProperties(NULL, &vulkanHandler->InstanceExtensionsCount, extensions);

        vulkanHandler->InstanceExtensions = calloc(vulkanHandler->InstanceExtensionsCount, sizeof(char*));
        
        uint32 i;
        uint32 f;
        for (i = 0; i < vulkanHandler->InstanceExtensionsCount; i++)
        {
            f = strlen(extensions[i].extensionName);
            vulkanHandler->InstanceExtensions[i] = malloc(f);
            memcpy(vulkanHandler->InstanceExtensions[i], extensions[i].extensionName, f);
            MAGE_LOG_CORE_INFORM("Instance extension [%s]\n", extensions[i].extensionName);
        }
        mageFreeMethod(extensions);

    #endif
}
void mageVulkanHandlerGetGPUExtensions(mageVulkanHandler *vulkanHandler)
{
    #if defined(MAGE_VULKAN)
        
        vkEnumerateDeviceExtensionProperties(vulkanHandler->GPU, NULL, &vulkanHandler->GPUExtensionsCount, NULL);
        VkExtensionProperties *extensions = calloc(vulkanHandler->GPUExtensionsCount, sizeof(struct VkExtensionProperties));
        vkEnumerateDeviceExtensionProperties(vulkanHandler->GPU, NULL, &vulkanHandler->GPUExtensionsCount, extensions);
        vulkanHandler->GPUExtensions = calloc(vulkanHandler->GPUExtensionsCount, sizeof(char*));

        uint64 i;
        for (i = 0; i < vulkanHandler->GPUExtensionsCount; i++)
        {
            uint64 j = strlen(extensions[i].extensionName);
            vulkanHandler->GPUExtensions[i] = malloc(j + 5  );
            
            strcpy(vulkanHandler->GPUExtensions[i], extensions[i].extensionName);
          
            MAGE_LOG_CORE_INFORM("GPU extension [%s]\n", vulkanHandler->GPUExtensions[i]);
        }
       
    #endif
}
void mageVulkanHandlerGetGPU(mageVulkanHandler *vulkanHandler, uint8 *success)
{
    #if defined(MAGE_VULKAN)
        
        vkEnumeratePhysicalDevices(vulkanHandler->VulkanInstance, &vulkanHandler->GPUCount, NULL);

        if (vulkanHandler->GPUCount == 0)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Unable to find physical device\n", NULL);
            mageTryDumpSuccess(0, success);
            return;
        }
        
        VkPhysicalDevice *devices = calloc(vulkanHandler->GPUCount, sizeof(VkPhysicalDevice));
        VkPhysicalDeviceProperties *properties = calloc(vulkanHandler->GPUCount, sizeof(VkPhysicalDeviceProperties));
        
        vkEnumeratePhysicalDevices(vulkanHandler->VulkanInstance, &vulkanHandler->GPUCount, devices);
        
        uint8 i;
        
        for (i = 0; i < vulkanHandler->GPUCount; i++)
        {
            vkGetPhysicalDeviceProperties(devices[i], &properties[i]);
            MAGE_LOG_CORE_INFORM("Device %d of %d found [%s] version [%d]\n", i + 1, vulkanHandler->GPUCount, properties[i].deviceName, properties[i].deviceID);
        }
        
        vulkanHandler->GPUProperties = properties[1];
        vulkanHandler->GPUIndex = 1;

        vulkanHandler->GPU = devices[1];
        MAGE_LOG_CORE_INFORM("Using graphics card [%s] version [%d]\n", vulkanHandler->GPUProperties.deviceName, vulkanHandler->GPUProperties.deviceID);


        mageFreeMethod(properties);
        mageFreeMethod(devices);
        mageTryDumpSuccess(1, success);

        
    #endif
}
void mageVulkanHandlerGetLayerProperties(mageVulkanHandler *vulkanHandler, uint8 *success)
{
    #if defined(MAGE_VULKAN)
        vkEnumerateInstanceLayerProperties(&vulkanHandler->InstanceLayerCount, NULL);
        vulkanHandler->InstanceLayerProperties = calloc(vulkanHandler->InstanceLayerCount, sizeof(VkLayerProperties));
        vkEnumerateInstanceLayerProperties(&vulkanHandler->InstanceLayerCount, vulkanHandler->InstanceLayerProperties);
        uint8 i;

        for (i = 0; i < vulkanHandler->InstanceLayerCount; i++)
        {
            MAGE_LOG_CORE_INFORM("Instance layer found [%s][%s]\n", vulkanHandler->InstanceLayerProperties[i].layerName, vulkanHandler->InstanceLayerProperties[i].description);
        }
       
    #endif
}
void mageVulkanHandlerGetPhysicalLayerProperties(mageVulkanHandler *vulkanHandler, uint8 *success)
{
    #if defined(MAGE_VULKAN)
        vkEnumerateDeviceLayerProperties(vulkanHandler->GPU, &vulkanHandler->PhysicalLayerCount, NULL);
        MAGE_LOG_CORE_ERROR("%d\n", vulkanHandler->PhysicalLayerCount);
    
        
       
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
void mageVulkanHandlerGPUInitialise(mageVulkanHandler *vulkanHandler, uint8 *success)
{
    #if defined(MAGE_VULKAN)
            uint32 familyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(vulkanHandler->GPU, &familyCount, NULL);
            VkQueueFamilyProperties *familyProperties = calloc(familyCount, sizeof(VkQueueFamilyProperties));
            vkGetPhysicalDeviceQueueFamilyProperties(vulkanHandler->GPU, &familyCount, familyProperties);
            
            uint32 i;
            uint32 found = 0;
            for (i = 0; i < familyCount; i++)
            {
                if (familyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    found = 1;
                    vulkanHandler->GPUFamilyIndex = i;
                }
            }
            mageFreeMethod(familyProperties);

            if (!found)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Graphics family index has not been found\n", NULL);
                mageTryDumpSuccess(0, success);
                return;
            }
            MAGE_LOG_CORE_INFORM("Graphics family index has been found\n", NULL);

            float *queuePriorities = calloc(1, sizeof(float));
            queuePriorities[0] = 1.0f;

            VkDeviceQueueCreateInfo deviceQueueCreateInformation;
            deviceQueueCreateInformation.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            deviceQueueCreateInformation.queueFamilyIndex = vulkanHandler->GPUFamilyIndex;       
            deviceQueueCreateInformation.queueCount = 1;
            deviceQueueCreateInformation.pQueuePriorities = queuePriorities;

            VkPhysicalDeviceFeatures features;

            VkDeviceCreateInfo deviceCreateInformation;
            deviceCreateInformation.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            deviceCreateInformation.pNext = NULL;
            deviceCreateInformation.flags = 0;
            deviceCreateInformation.queueCreateInfoCount = 1;
            deviceCreateInformation.pQueueCreateInfos = &deviceQueueCreateInformation;
            deviceCreateInformation.enabledLayerCount = 0;
            deviceCreateInformation.ppEnabledLayerNames = NULL;
            deviceCreateInformation.enabledExtensionCount = 0;
            deviceCreateInformation.ppEnabledExtensionNames = NULL;
            deviceCreateInformation.pEnabledFeatures = &features;

                 
            VkResult result = vkCreateDevice(vulkanHandler->GPU, &deviceCreateInformation, NULL, &vulkanHandler->VulkanDevice);
    
            switch (result)
            {
            case VK_SUCCESS:
                MAGE_LOG_CORE_INFORM("Created physical device\n", NULL);
                mageTryDumpSuccess(1, success);
                break;

            case VK_ERROR_OUT_OF_HOST_MEMORY:
                MAGE_LOG_CORE_FATAL_ERROR("Failed to create physical device, host out of memory\n", NULL);
                mageTryDumpSuccess(0, success);
                break;

            case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                MAGE_LOG_CORE_FATAL_ERROR("Failed to create physical device, device out of memory\n", NULL);
                mageTryDumpSuccess(0, success);
                break;
            
            case VK_ERROR_INITIALIZATION_FAILED:
                MAGE_LOG_CORE_FATAL_ERROR("Failed to create physical device, initializing failed\n", NULL);
                mageTryDumpSuccess(0, success);
                break;

            case VK_ERROR_EXTENSION_NOT_PRESENT:
                MAGE_LOG_CORE_FATAL_ERROR("Failed to create physical device, extensions not present\n", NULL);
                mageTryDumpSuccess(0, success);
                break;

            case VK_ERROR_FEATURE_NOT_PRESENT:
                MAGE_LOG_CORE_FATAL_ERROR("Failed to create physical device, feature not present\n", NULL);
                mageTryDumpSuccess(0, success);
                break;
            
            case VK_ERROR_TOO_MANY_OBJECTS:
                MAGE_LOG_CORE_FATAL_ERROR("Failed to create physical device, too many objects\n", NULL);
                mageTryDumpSuccess(0, success);
                break;

            case VK_ERROR_DEVICE_LOST:
                MAGE_LOG_CORE_FATAL_ERROR("Failed to create physical device, device lost\n", NULL);
                mageTryDumpSuccess(0, success);
                break;
            
            default:
                MAGE_LOG_CORE_FATAL_ERROR("Failed to create physical device, undefined error\n", NULL);
                mageTryDumpSuccess(0, success); 
            }

            mageFreeMethod(queuePriorities);
    #endif
}



void mageVulkanHandlerFree(mageVulkanHandler *vulkanHandler)
{
    
}









void *mageRendererAllocate()
{
    return malloc(sizeof(struct MAGE_RENDERER_STRUCT));
}

