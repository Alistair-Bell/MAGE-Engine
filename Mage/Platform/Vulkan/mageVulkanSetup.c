#include <mageAPI.h>

#if defined (MAGE_VULKAN)

static const char *mageRequiredExtensions[] = 
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SURFACE_EXTENSION_NAME,
    
};
static const char *const mageRequiredLayers[] = 
{
    "VK_LAYER_KHRONOS_validation",
};

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
static uint8_t mageCheckValidationLayers(struct mageRenderer *renderer, struct mageWindow *window)
{
    uint32_t i, j;
    uint32_t layerCount;
    uint32_t layerFound = 0;

    vkEnumerateInstanceLayerProperties(&layerCount, NULL);
    VkLayerProperties *properties = calloc(layerCount, sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&layerCount, properties);

    for (i = 0; i < layerCount; i++)
    {
        for (j = 0; j < sizeof(mageRequiredLayers) / sizeof(const char *); j++)
        {
            if (strcmp(mageRequiredLayers[j], properties[i].layerName) == 0)
            {
                layerFound = 1;
                break;
            }
        }
    }
    if (!layerFound)
    {
        MAGE_LOG_CORE_FATAL_ERROR("Required validation layers not found\n", NULL);
        return 0;
    }
    MAGE_LOG_CORE_INFORM("Required validation layers found\n", NULL);
    
    free(properties);
    return 1;
}
static char **mageGetRequiredExtensions(uint32_t *count)
{
    uint32_t debugCount = 1;
    uint32_t glfwCount;
    uint32_t i;
    char *debugExtensions[] = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwCount);
    uint32_t totalCount = glfwCount + debugCount;

    char **extensions = calloc(totalCount, sizeof(char *));

    for (i = 0; i < totalCount; i++)
    {
        if (i < glfwCount)
        {
            extensions[i] = (char *)glfwExtensions[i];
        }
        else
        {
            extensions[i] = (char *)debugExtensions[i - glfwCount];
        }
    }   
    *count = totalCount;
    return extensions;
}
static mageResult mageSetupValidationLayerCallback(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{
    if (mageCreateDebugUtilsMessengerEXT(renderer->Instance, &renderer->DebugMessengerCreateInfo, NULL, &renderer->DebugMessenger) != VK_SUCCESS) 
    {   
        MAGE_LOG_CORE_FATAL_ERROR("Debug messenger has failed to be created\n", NULL);
        return MAGE_DEBUG_MESSENGER_FAILED;
    }   
    MAGE_LOG_CORE_INFORM("Debug messenger was set up succesfully\n", NULL);
    return MAGE_SUCCESS;
}
static uint8_t mageIsDeviceSuitable(struct mageRenderer *renderer, VkPhysicalDevice device)
{
    struct mageIndiciesIndexes indicies;
    mageResult result = mageGetDeviceIndexes(renderer, device, &indicies);
    if (result != MAGE_SUCCESS)
    {
        mageIndiciesIndexesDestroy(&indicies);
        return 0;
    }
    VkBool32 supported;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, indicies.GraphicIndexes[indicies.GraphicIndexesCount - 1], renderer->Surface, &supported);
    
    if (!supported)
    {
        mageIndiciesIndexesDestroy(&indicies);
        return 0;
    }

    mageIndiciesIndexesDestroy(&indicies);
    return 1;
}
static uint32_t mageRateDevice(struct mageRenderer *renderer, VkPhysicalDevice device)
{
    uint32_t score = 0;
    
    if (!mageIsDeviceSuitable(renderer, device))
    {
        return score;
    }
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;

    vkGetPhysicalDeviceFeatures(device, &features);
    vkGetPhysicalDeviceProperties(device, &properties);

    switch (properties.deviceType)
    {
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: score += 1000; break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: score += 100; break;
        default: score += 0; break;
    }
    if (!features.geometryShader) { score = 0; return score; }
    return score;
}
static uint32_t mageRankScores(uint32_t *scores, uint32_t count)
{
    uint32_t indexLead = 0;
    uint32_t i;
    
    for (i = 0; i < count; i++)
    {   
        if (scores[indexLead] < scores[i])
        {
            indexLead = i;
        }
    }
    return indexLead;
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



static mageResult mageCreateInstance(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{
    if (!mageCheckValidationLayers(renderer, window))
    {
        return MAGE_HARDWARE_INVALID;
    }
    VkInstanceCreateInfo instanceCreateInfo;
    VkApplicationInfo applicationInfo;
    uint32_t count;
    memset(&instanceCreateInfo, 0, sizeof(VkInstanceCreateInfo));
    memset(&applicationInfo, 0, sizeof(VkApplicationInfo));
    
    applicationInfo.sType                           = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.apiVersion                      = VK_API_VERSION_1_2;
    applicationInfo.pApplicationName                = window->Title;
    applicationInfo.pEngineName                     = "MAGE-ENGINE";
    applicationInfo.engineVersion                   = VK_MAKE_VERSION(1, 0, 0);
    
    const char **extensions = (const char **)mageGetRequiredExtensions(&count);

    instanceCreateInfo.sType                        = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo             = &applicationInfo;
    instanceCreateInfo.ppEnabledExtensionNames      = extensions;
    instanceCreateInfo.enabledExtensionCount        = count;

#if defined (MAGE_VULKAN)
    instanceCreateInfo.ppEnabledLayerNames          = mageRequiredLayers;
    instanceCreateInfo.enabledLayerCount            = 1;
    magePopulateValidationLayerCallback(&renderer->DebugMessengerCreateInfo);
    instanceCreateInfo.pNext                        = (VkDebugUtilsMessengerCreateInfoEXT*) &renderer->DebugMessengerCreateInfo;
#else
    instanceCreateInfo.ppEnabledLayerNames          = NULL;
    instanceCreateInfo.enabledLayerCount            = 0;
#endif

    if (vkCreateInstance(&instanceCreateInfo, NULL, &renderer->Instance) != VK_SUCCESS)
    {
        MAGE_LOG_CORE_FATAL_ERROR("Failed to create vulkan instance\n", NULL);
        return MAGE_INSTANCE_CREATION_FAILURE;
    }

    free(extensions);
    MAGE_LOG_CORE_INFORM("Vulkan instance has been created succesfully\n", NULL);
    return MAGE_SUCCESS;
}
static mageResult mageCreateSurface(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{
    if (glfwCreateWindowSurface(renderer->Instance, window->Context, NULL, &renderer->Surface) != VK_SUCCESS)
    {
        MAGE_LOG_CORE_FATAL_ERROR("GLFW surface has failed to be created\n", NULL);
        return MAGE_SURFACE_CREATION_FAILURE;
    }
   MAGE_LOG_CORE_INFORM("GLFW surface has been creates succesfully\n", NULL);
   return MAGE_SUCCESS;
}
static mageResult magePickPhysicalDevice(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{
    uint32_t deviceCount, i;
    vkEnumeratePhysicalDevices(renderer->Instance, &deviceCount, NULL);
    if (deviceCount <= 0)
    {
        MAGE_LOG_CORE_FATAL_ERROR("Unable to find any vulkan physical devices\n", NULL);
        return MAGE_HARDWARE_INVALID;
    }
    VkPhysicalDevice *devices = calloc(deviceCount, sizeof(VkPhysicalDevice));
    uint32_t *scores = calloc(deviceCount, sizeof(uint32_t));    
    vkEnumeratePhysicalDevices(renderer->Instance, &deviceCount, devices);

    for (i = 0; i < deviceCount; i++)
    {
        scores[i] = mageRateDevice(renderer, devices[i]);
    }
    uint32_t index = mageRankScores(scores, deviceCount);
    renderer->PhysicalDevice = devices[index];

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(renderer->PhysicalDevice, &properties);
    MAGE_LOG_CORE_INFORM("Device picked %s\n", properties.deviceName);
    mageGetDeviceIndexes(renderer, renderer->PhysicalDevice, &renderer->Indexes);
    free(devices);
    free(scores);
    return MAGE_SUCCESS;
}
static mageResult mageCreateDevice(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{
    const float priorities[] = { 1.0f };
    VkDeviceQueueCreateInfo queueCreateInfo;
    VkDeviceCreateInfo deviceCreateInfo;

    memset(&deviceCreateInfo, 0, sizeof(VkDeviceCreateInfo));
    memset(&queueCreateInfo, 0, sizeof(VkDeviceQueueCreateInfo));
    queueCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex        = renderer->Indexes.GraphicIndexes[0];
    queueCreateInfo.pQueuePriorities        = priorities;
    queueCreateInfo.queueCount              = 1;

    deviceCreateInfo.sType                  = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos      = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount   = 1;

    if (vkCreateDevice(renderer->PhysicalDevice, &deviceCreateInfo, NULL, &renderer->Device) != VK_SUCCESS)
    {
        MAGE_LOG_CORE_FATAL_ERROR("Vulkan device has failed to be created\n", NULL);
        return MAGE_DEVICE_CREATION_FAILURE;
    }

    MAGE_LOG_CORE_INFORM("Vulkan device has been created succesfully\n", NULL);
    return MAGE_SUCCESS;
}
static mageResult mageFetchQueues(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{
    vkGetDeviceQueue(renderer->Device, renderer->Indexes.GraphicIndexes[renderer->Indexes.GraphicIndexesCount - 1], 0, &renderer->GraphicalQueue);
    vkGetDeviceQueue(renderer->Device, renderer->Indexes.PresentIndexes[renderer->Indexes.PresentIndexesCount - 1], 0, &renderer->PresentQueue);
    MAGE_LOG_CORE_INFORM("Present and graphic queues had been fetched\n", NULL);
}
mageResult mageRendererInitialise(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{
    uint32_t i;
    typedef mageResult (*function)(struct mageRenderer *, struct mageWindow *, struct mageRendererProps *);
    function functions[] = 
    { 
        mageCreateInstance, 
    #if defined (MAGE_DEBUG)
        mageSetupValidationLayerCallback,
    #endif 
        mageCreateSurface, 
        magePickPhysicalDevice, 
        mageCreateDevice,
        mageFetchQueues,
    };

    for (i = 0; i < sizeof(functions) / sizeof(function); i++)
    {
        mageResult result = functions[i](renderer, window, props);
        if (result != MAGE_SUCCESS) return result;
    }

    return MAGE_SUCCESS;
}
void mageRendererDestroy(struct mageRenderer *renderer)
{
    vkDestroySurfaceKHR(renderer->Instance, renderer->Surface, NULL);
    
    
    
    mageDestroyDebugUtilsMessengerEXT(renderer->Instance, renderer->DebugMessenger, NULL);
    vkDestroyDevice(renderer->Device, NULL);
    vkDestroyInstance(renderer->Instance, NULL); 
    mageIndiciesIndexesDestroy(&renderer->Indexes);
}


#endif