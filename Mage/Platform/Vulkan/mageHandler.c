/*!************************
 * This file handles the vulkan setup required to render
 * Only compiled when using vulkan
 * #if defined macros should guard the code so if it is compiled no errors should occur 
**************************/
#include <mageAPI.h>

void *mageVulkanHandlerAllocate()
{
    return malloc(sizeof(struct mageVulkanHandler));
}

#if defined (MAGE_VULKAN)

    mageResult mageCreateInstance(struct mageVulkanHandler *handler, struct mageWindow *window)
    {
        VkInstanceCreateInfo createInfo;
        VkApplicationInfo applicationInfo;
        memset(&createInfo, 0, sizeof(VkInstanceCreateInfo));
        memset(&applicationInfo, 0, sizeof(VkApplicationInfo));

        applicationInfo.sType                   = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.apiVersion              = VK_API_VERSION_1_2;
        applicationInfo.pApplicationName        = window->Title;
        applicationInfo.pEngineName             = "MAGE-ENGINE";
        applicationInfo.engineVersion           = VK_MAKE_VERSION(1, 0, 0);
        createInfo.sType                        = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo             = &applicationInfo;
        
        #if defined (MAGE_GLFW)
            uint32_t count;
            const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&count);
            createInfo.ppEnabledExtensionNames  = glfwExtensions;
            createInfo.enabledExtensionCount    = count;  
        #else
            createInfo.ppEnabledExtensionNames  = NULL;
            createInfo.enabledExtensionCount    = 0;
        #endif


        VkResult result = vkCreateInstance(&createInfo, NULL, &handler->Instance);

        if (result != VK_SUCCESS)
        {
            MAGE_LOG_CLIENT_FATAL_ERROR("Vulkan instance has failed to be created\n", NULL);
            return MAGE_INSTANCE_CREATION_FAILURE;
        }
        MAGE_LOG_CORE_INFORM("Vulkan instance has been created succesfully\n", NULL);
    
        return MAGE_SUCCESS;
    }
    uint32_t mageScoreDevice(VkPhysicalDevice device)
    {
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures features;
        memset(&properties, 0, sizeof(VkPhysicalDeviceProperties));
        memset(&features, 0, sizeof(VkPhysicalDeviceFeatures));
        
        vkGetPhysicalDeviceProperties(device, &properties);
        vkGetPhysicalDeviceFeatures(device, &features);

        uint32_t score = 0;

        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 10000;
        }

        score += properties.limits.maxImageDimension2D;

        return score;
    }
    int32_t mageGetFamilyIndex(VkPhysicalDevice device)
    {
        uint32_t queueCount, i;
        int32_t index = -1;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, NULL);
        VkQueueFamilyProperties *properties = calloc(queueCount, sizeof(VkQueueFamilyProperties));
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueCount, properties);
        
        for (i = 0; i < queueCount; i++)
        {
            if (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                index = i;
            }
        }
        free(properties);
        return index;
    }
    VkPhysicalDevice mageSelectDevice(VkPhysicalDevice *devices, uint32_t deviceCount)
    {
        uint32_t index = 0;
        uint32_t i;
        uint32_t *scores = calloc(deviceCount, sizeof(uint32_t));

        for (i = 0; i < deviceCount; i++)
        {
            scores[i] = mageScoreDevice(devices[i]);            
            if (scores[index] <= scores[i]) index = i; 
        }

        free(scores);

        return devices[index];
    }
    mageResult mageCreateDevice(struct mageVulkanHandler *handler, struct mageWindow *window)
    {   
        {
            uint32_t deviceCount;
            vkEnumeratePhysicalDevices(handler->Instance, &deviceCount, NULL);
            VkPhysicalDevice *devices = calloc(deviceCount, sizeof(VkPhysicalDevice));
            vkEnumeratePhysicalDevices(handler->Instance, &deviceCount, devices);

            if (deviceCount <= 0)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Unable to find any GPU's within your hardware!\n", NULL);
                return MAGE_HARDWARE_INVALID;
            }

            handler->PhysicalDevice = mageSelectDevice(devices, deviceCount);

            vkGetPhysicalDeviceProperties(handler->PhysicalDevice, &handler->PhysicalProperties);

            vkGetPhysicalDeviceMemoryProperties(handler->PhysicalDevice, &handler->PhysicalMemoryProperties);

            MAGE_LOG_CORE_INFORM("GPU selected %s\n", handler->PhysicalProperties.deviceName);
            free(devices);
        }

        VkDeviceCreateInfo deviceCreateInfo;
        VkDeviceQueueCreateInfo queueCreateInfo;
        memset(&deviceCreateInfo, 0, sizeof(VkDeviceCreateInfo));
        memset(&queueCreateInfo, 0, sizeof(VkDeviceQueueCreateInfo));

        int32_t index = mageGetFamilyIndex(handler->PhysicalDevice);
        if (index == -1)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Graphics family index not found!\n", NULL);
            return MAGE_HARDWARE_INVALID;
        }
        
        handler->GraphicsFamilyIndex = (uint32_t) index;

        float queuePriorites[] = { 1.0f };

        queueCreateInfo.sType                    = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueCount               = 1;
        queueCreateInfo.queueFamilyIndex         = handler->GraphicsFamilyIndex ;
        queueCreateInfo.pQueuePriorities         = queuePriorites;

        deviceCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount    = 1;
        deviceCreateInfo.pQueueCreateInfos       = &queueCreateInfo;
        deviceCreateInfo.ppEnabledExtensionNames = (const char * const []) { VK_KHR_SWAPCHAIN_EXTENSION_NAME, };
        deviceCreateInfo.enabledExtensionCount   = 1;


        VkResult result = vkCreateDevice(handler->PhysicalDevice, &deviceCreateInfo, NULL, &handler->Device);
        if (result != VK_SUCCESS)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Vulkan has failed to create device\n", NULL);
            return MAGE_DEVICE_CREATION_FAILURE;
        }
        MAGE_LOG_CORE_INFORM("Vulkan device has been created succesfully\n", NULL);

        return MAGE_SUCCESS;
    }
    
    mageResult mageVulkanHandlerInitialise(struct mageVulkanHandler *handler, struct mageWindow *window)
    {
        typedef mageResult (*requiredFunctions)(struct mageVulkanHandler *, struct mageWindow *);
        uint32_t i;

        requiredFunctions functions[] =
        {
            mageCreateInstance,
            mageCreateDevice,
        };
        const uint32_t functionCount = sizeof(functions) / sizeof(requiredFunctions);

        for (i = 0; i < functionCount; i++)
        {
            mageResult r = functions[i](handler, window);
            if (r != MAGE_SUCCESS) return r;
        }

        return MAGE_SUCCESS;
    }
    void mageVulkanHandlerCleanup(struct mageVulkanHandler *handler)
    {
        vkDestroyDevice(handler->Device, NULL);
        vkDestroyInstance(handler->Instance, NULL);
        MAGE_LOG_CORE_INFORM("Vulkan has been cleaned up\n", NULL);
    }

#endif
