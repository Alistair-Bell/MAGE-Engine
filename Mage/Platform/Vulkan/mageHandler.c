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

    static const char * const RequiredExtensions[] = 
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    #if defined (MAGE_RELEASE)
        VK_KHR_SURFACE_EXTENSION_NAME,
    #endif 
    };


    static const char *mageRequiredValidationLayers[] =
    {
        "VK_LAYER_KHRONOS_validation",
    };

    uint32_t mageFindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties *gpuMemoryProperties, const VkMemoryRequirements *memoryRequirements, const VkMemoryPropertyFlags memoryProperties)
    {
        uint32_t i;
        for (i = 0; i < gpuMemoryProperties->memoryTypeCount; i++) 
        {
            if (memoryRequirements->memoryTypeBits & (1 << i)) 
            {
                if ((gpuMemoryProperties->memoryTypes[i].propertyFlags & memoryProperties ) == memoryProperties ) 
                {
                    return i;
                }
            }
	    }
        MAGE_LOG_CORE_ERROR("GPU memory index was not found\n", NULL);
	    return UINT32_MAX;
    }
    static uint8_t mageCheckRequiredValidiationLayersPresent()
    {
        uint32_t layerCount, requiredLayerCount, i, j;
        requiredLayerCount = 1;
        vkEnumerateInstanceLayerProperties(&layerCount, NULL);
        VkLayerProperties *properties = calloc(layerCount, sizeof(VkLayerProperties));
        vkEnumerateInstanceLayerProperties(&layerCount, properties);

        uint8_t layerFound = 0;
        for (i = 0; i < requiredLayerCount; i++)
        {
            for (j = 0; j < layerCount; j++)
            {   
                if (strcmp(mageRequiredValidationLayers[i], properties[j].layerName) == 0)
                {
                    layerFound = 1;
                    break;
                }
            }
        }
        return layerFound;
    }
    static char **mageGetRequiredInstanceExtensions(uint32_t *clientCount)
    {
        
        *clientCount = 0;
        char **extensions;
        char **windowExtensions;
        const char *debugExtensions[] = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
        uint32_t windowCount;
        uint32_t debugCount;
        
        #if defined (MAGE_GLFW)
            windowExtensions = (char **) glfwGetRequiredInstanceExtensions(&windowCount);
        #elif defined (MAGE_SDL)
            windowCount = 0;
        #endif

        #if defined (MAGE_DEBUG)
            char **temp = calloc(windowCount + debugCount, sizeof(char *));
            debugCount = 1;
            uint32_t i = 0;

            for (i = 0; i < windowCount + debugCount; i++)
            {
                if (i < windowCount)
                {
                    MAGE_LOG_CORE_INFORM("%s\n", windowExtensions[i]);
                    temp[i] = malloc(strlen(windowExtensions[i]));
                    strcpy(temp[i], windowExtensions[i]);
                }
                else
                {
                    temp[i] = malloc(strlen(debugExtensions[i - windowCount]));
                    strcpy(temp[i], debugExtensions[i - windowCount]);
                }
            }
            extensions = temp;
        #else
            debugCount = 0;
            extensions = windowExtensions;
        #endif


        *clientCount = windowCount + debugCount;
        return (char **) extensions;
    }
    static VKAPI_ATTR VkBool32 VKAPI_CALL mageVulkanDebugCallback( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *callbackData, void *pUserData) 
    {
        switch (messageType)
        {   
            case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
                MAGE_LOG_CORE_INFORM("Validation Layers %s\n", callbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
                MAGE_LOG_CORE_FATAL_ERROR("Validation Layers : violation issue %s\n", callbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
                MAGE_LOG_CORE_WARNING("Validation Layers : performance issue %s\n", callbackData->pMessage);
                break; 
            default:
                MAGE_LOG_CORE_ERROR("Validation Layers : Unknown validation error\n", NULL);
                break;  
        }
        return VK_TRUE;
    }
    static VkResult mageCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) 
    {
        PFN_vkCreateDebugUtilsMessengerEXT function = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (function != NULL) 
        {
            return function(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } 
        else 
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }
    static void mageDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) 
    {
        PFN_vkDestroyDebugUtilsMessengerEXT function = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (function != NULL) 
        {
            function(instance, debugMessenger, pAllocator);
        }
    }
    static void magePopulateValidationLayerCallback(VkDebugUtilsMessengerCreateInfoEXT *info)
    {
        memset(info, 0, sizeof(VkDebugUtilsMessengerCreateInfoEXT));
        info->sType                 = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        info->messageSeverity       = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        info->messageType           = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        info->pfnUserCallback       = mageVulkanDebugCallback;
        info->pUserData             = NULL;
    }
    static mageResult mageSetupValidationLayerCallback(struct mageVulkanHandler *handler, struct mageWindow *window)
    {
    #if defined (MAGE_DEBUG)
        if (mageCreateDebugUtilsMessengerEXT(handler->Instance, &handler->DebugMessengerCreateInformation, NULL, &handler->DebugMessenger) != VK_SUCCESS) 
        {   
            MAGE_LOG_CORE_FATAL_ERROR("Debug messenger has failed to be created\n", NULL);
            return MAGE_DEBUG_MESSENGER_FAILED;
        }   
        MAGE_LOG_CORE_INFORM("Debug messenger was set up succesfully\n", NULL);
    #endif
        return MAGE_SUCCESS;
    }

    static mageResult mageCreateInstance(struct mageVulkanHandler *handler, struct mageWindow *window)
    {
    #if defined (MAGE_DEBUG)
            
        if (!mageCheckRequiredValidiationLayersPresent())
        {
            MAGE_LOG_CORE_FATAL_ERROR("Validation layers were not present!\n", NULL);
            return MAGE_HARDWARE_NOT_PRESENT;
        }
        MAGE_LOG_CORE_INFORM("Vulkan validation layers are in use\n", NULL);

    #endif
        VkInstanceCreateInfo createInfo;
        VkApplicationInfo applicationInfo;
        uint32_t count;
        memset(&createInfo, 0, sizeof(VkInstanceCreateInfo));
        memset(&applicationInfo, 0, sizeof(VkApplicationInfo));

        char **extensions = mageGetRequiredInstanceExtensions(&count);
        applicationInfo.sType                   = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.apiVersion              = VK_API_VERSION_1_2;
        applicationInfo.pApplicationName        = window->Title;
        applicationInfo.pEngineName             = "MAGE-ENGINE";
        applicationInfo.engineVersion           = VK_MAKE_VERSION(1, 0, 0);
        createInfo.sType                        = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo             = &applicationInfo;
        createInfo.ppEnabledExtensionNames      = (const char **)extensions;
        createInfo.enabledExtensionCount        = count;
    #if defined (MAGE_DEBUG)
        createInfo.enabledLayerCount            = 1;
        createInfo.ppEnabledLayerNames          = mageRequiredValidationLayers;
        magePopulateValidationLayerCallback(&handler->DebugMessengerCreateInformation);
        createInfo.pNext                        = (VkDebugUtilsMessengerCreateInfoEXT*) &handler->DebugMessengerCreateInformation;
    #else
        createInfo.pNext                        = NULL;
    #endif

        VkResult result = vkCreateInstance(&createInfo, NULL, &handler->Instance);

        if (result != VK_SUCCESS)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Vulkan instance has failed to be created\n", NULL);
            return MAGE_INSTANCE_CREATION_FAILURE;
        }
        MAGE_LOG_CORE_INFORM("Vulkan instance has been created succesfully\n", NULL);
        
    #if defined (MAGE_DEBUG)
        uint32_t i;
        {
            for (i = 0; i < count; i++) free(extensions[i]);
                free(extensions);
        }
    #endif
        return MAGE_SUCCESS;
    }
    static uint32_t mageScoreDevice(VkPhysicalDevice device)
    {
        VkPhysicalDeviceProperties properties;
        memset(&properties, 0, sizeof(VkPhysicalDeviceProperties));

        vkGetPhysicalDeviceProperties(device, &properties);

        uint32_t score = 0;

        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 10000;
        }

        score += properties.limits.maxImageDimension2D;

        return score;
    }
    static int32_t mageGetFamilyIndex(VkPhysicalDevice device)
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
    static VkPhysicalDevice mageSelectDevice(VkPhysicalDevice *devices, uint32_t deviceCount)
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
    static mageResult mageCreateDevice(struct mageVulkanHandler *handler, struct mageWindow *window)
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
        deviceCreateInfo.ppEnabledExtensionNames = RequiredExtensions;
        deviceCreateInfo.enabledExtensionCount   = sizeof(RequiredExtensions) / sizeof(char *);

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
        #if defined (MAGE_DEBUG)
            mageSetupValidationLayerCallback,
        #endif
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
        
        #if defined (MAGE_DEBUG)
            mageDestroyDebugUtilsMessengerEXT(handler->Instance, handler->DebugMessenger, NULL);
        #endif
        
        vkDestroyInstance(handler->Instance, NULL);
        MAGE_LOG_CORE_INFORM("Vulkan has been cleaned up\n", NULL);
    }

#endif
