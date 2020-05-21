#include "mageAPI.h"

static const char * const RequiredValidationLayers[] = { "VK_LAYER_KHRONOS_validation", };
static const uint8 RequiredLayersCount = 1;
static const char * const RequiredDeviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, };



void *mageQueueHandlerAllocate()
{
    return malloc(sizeof(struct MAGE_QUEUE_HANDLER_STRUCT));
}
void mageQueueHandlerInitialise(mageQueueHandler *handler)
{   
    handler->GraphicsCount = 0;
    handler->PresentCount = 0;
   
    handler->PresentFamilies = calloc(1, sizeof(uint32));
    handler->GraphicsFamiles = calloc(1, sizeof(uint32));
}
void mageQueueHandlerPushIndex(mageQueueHandler *handler, int32 graphicsIndex, int32 presentIndex)
{  
    handler->GraphicsCount++;
    handler->PresentCount++;

    if (-1 < graphicsIndex) 
    {
        handler->GraphicsFamiles = realloc(handler->GraphicsFamiles, sizeof(uint32) * handler->GraphicsCount);
        handler->GraphicsFamiles[handler->GraphicsCount - 1] = graphicsIndex;
    }
    if (-1 < presentIndex) 
    {
        handler->PresentFamilies = realloc(handler->PresentFamilies, sizeof(uint32) * handler->PresentCount);
        handler->PresentFamilies[handler->PresentCount - 1] = presentIndex;
    }
}
void mageQueueHandlerFree(mageQueueHandler *handler)
{
    mageFreeMethod(handler->GraphicsFamiles);
    mageFreeMethod(handler->PresentFamilies);
}
void *mageVulkanHandlerAllocate()
{
    return malloc(sizeof(struct MAGE_VULKAN_HANDLER_STRUCT));
}
#if defined (MAGE_VULKAN)
    static void mageVulkanCheckLayerExtensions(mageVulkanHandler *handler, uint8 *success)
    {
        mageTryDumpSuccess(0, success);
        /*!************************
            Checking layer extensions
        **************************/
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, NULL);

        VkLayerProperties *availableLayers = calloc(layerCount, sizeof(struct VkLayerProperties));
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

        if (layerCount <= 0)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Instance has no availabe extensions\n", NULL);
            return;
        }

        uint8 i, j, layerFound;

        for (i = 0; i < RequiredLayersCount; i++)
        {
            layerFound = 0;

            for (j = 0; j < layerCount; j++)
            {
                if (strcmp(RequiredValidationLayers[i], availableLayers[j].layerName) == 0)
                {
                    MAGE_LOG_CORE_INFORM("Required validation layer found [%s]\n", availableLayers[j].layerName);
                    layerFound = 1;
                    break;
                }
            }
        }
        if (!layerFound) 
        {
            MAGE_LOG_CORE_FATAL_ERROR("Failed to find vulkan validation layers\n", NULL);
            return;
        }

        mageFreeMethod(availableLayers);

        mageTryDumpSuccess(1, success);
    }
    static void mageVulkanRateDevice(VkPhysicalDevice device, uint32 *score)
    {
        /*!************************
            Rating passed in device
        **************************/
        uint32 deviceScore = 0;

        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures features;

        vkGetPhysicalDeviceProperties(device, &properties);
        vkGetPhysicalDeviceFeatures(device, &features);

        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            deviceScore += 1000;
        }

        deviceScore += properties.limits.maxImageDimension2D;

        if (!features.geometryShader)
        {
            MAGE_LOG_CORE_WARNING("Device %s does not have geometry shader, unable to be used\n", properties.deviceName);
            *score = 0;
        }        
        *score = deviceScore;
    }
    static void mageVulkanFindQueueFamilies(VkPhysicalDevice device, mageVulkanHandler *vulkan, mageQueueHandler *handler)
    {
        /*!************************
            Getting queues from device
        **************************/
        uint32_t queueFamilyCount = 0;
        uint32 i;
        uint8 familyFlag = 0; 
        uint8 presentFlag = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
        VkQueueFamilyProperties *properties = calloc(queueFamilyCount, sizeof(struct VkQueueFamilyProperties));
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, properties);
        
        for (i = 0; i < queueFamilyCount; i++)
        {
            if (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                mageQueueHandlerPushIndex(handler, i, -1);
                familyFlag = 1;
            }    
            VkBool32 presentSupported;

            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vulkan->Surface, &presentSupported);

            if (presentSupported)
            {
                mageQueueHandlerPushIndex(handler, -1, i);
                presentFlag = 1;
            }
        }

        if (familyFlag && presentFlag == 0)
        {
            MAGE_LOG_CORE_WARNING("Unable to find graphics or present index\n", NULL);
        }
        mageFreeMethod(properties);
    }
    static void mageVulkanIsDeviceSuitable(VkPhysicalDevice device, mageVulkanHandler *handler, uint8 *is)
    {
        mageQueueHandler queue;
        mageQueueHandlerInitialise(&queue);

        mageVulkanFindQueueFamilies(device, handler, &queue);

        uint8 flag = 1 <= queue.GraphicsCount && 1 <= queue.PresentCount; 

        mageQueueHandlerFree(&queue);
        
        if (flag != 1)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Device is not suitable, queues not present\n", NULL);
            return;
        }

        MAGE_LOG_CORE_INFORM("Device %s has queues present\n", handler->PhysicalProperties.deviceName);
    
        MAGE_LOG_CORE_INFORM("Device %s is suitable\n", handler->PhysicalProperties.deviceName);
        
        *is = 1;
    }
#endif   
void mageVulkanHandlerCreateInstance(mageVulkanHandler *handler, mageWindow *window, uint8 *success) 
{
    #if defined (MAGE_VULKAN)
        mageTryDumpSuccess(0, success);    
        /*!************************
            Creating instance
        **************************/
        VkApplicationInfo applicationInformation;
        applicationInformation.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInformation.pNext = NULL;
        applicationInformation.pApplicationName = window->Title;
        applicationInformation.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
        applicationInformation.pEngineName = "MAGE";
        applicationInformation.apiVersion = VK_MAKE_VERSION(1, 0, 26);

        VkInstanceCreateInfo createInformation;
        createInformation.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInformation.pNext = NULL;
        createInformation.flags = 0;
        createInformation.pApplicationInfo = &applicationInformation;
        
        #if MAGE_VALIDATION_LAYERS
            
            uint8 flag;
            mageVulkanHandlerSetupDebug(handler, &flag);

            if (!flag)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Vulkan debug mode has failed\n", NULL);
                return;
            }
            
            createInformation.ppEnabledLayerNames = RequiredValidationLayers;
            createInformation.enabledLayerCount = 1;
        #else
            createInformation.ppEnabledLayerNames = NULL;
            createInformation.enabledLayerCount = 0;
        #endif
        
        #if defined (MAGE_GLFW)

            uint32 glfwExtensionCount;
        
            const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            createInformation.enabledExtensionCount = glfwExtensionCount;
            createInformation.ppEnabledExtensionNames = glfwExtensions;
        
        #else
            createInformation.enabledExtensionCount = 0;
            createInformation.ppEnabledExtensionNames = NULL;
        #endif


        VkResult result = vkCreateInstance(&createInformation, NULL, &handler->Instance);

        if (result != VK_SUCCESS)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Failed to create vulkan instance\n", NULL);
            return;
        }
        MAGE_LOG_CORE_INFORM("Vulkan instance has been created\n", NULL);
    
    #endif
    mageTryDumpSuccess(1, success);
}
void mageVulkanHandlerSetupDebug(mageVulkanHandler *handler, uint8 *success)
{
    /*!************************
        Setting up the debug
    **************************/
    #if defined (MAGE_VULKAN)

        uint8 flag;
        mageVulkanCheckLayerExtensions(handler, &flag);



        MAGE_LOG_CORE_INFORM("Vulkan debug mode has been created succesfully\n", NULL);

    #endif
    mageTryDumpSuccess(1, success);
}
void mageVulkanPickDevice(mageVulkanHandler *handler, mageWindow * window, uint8 *success)
{
    /*!************************
        Picking device
    **************************/
    #if defined (MAGE_VULKAN)
        uint32 count, i;

        vkEnumeratePhysicalDevices(handler->Instance, &count, NULL);
        VkPhysicalDevice *devices = calloc(count, sizeof(VkPhysicalDevice));
        uint32 *deviceScores = calloc(count, sizeof(uint32));
        vkEnumeratePhysicalDevices(handler->Instance, &count, devices);

        if (count <= 0)
        {
            MAGE_LOG_CORE_FATAL_ERROR("No physical devices present\n", NULL);
            mageTryDumpSuccess(0, success);
            return;
        }

        uint32 index = 0;

        for (i = 0; i < count; i++)
        {
            mageVulkanRateDevice(devices[i], &deviceScores[i]);    
            uint8 flag = deviceScores[index] <= deviceScores[i];
        
            if (flag) index = i;
        }
        
        VkPhysicalDeviceProperties features;
        vkGetPhysicalDeviceProperties(devices[index], &features);

        MAGE_LOG_CORE_INFORM("Using GPU %s out of %d candidates\n", features.deviceName, count);

        handler->PhysicalDevice = devices[index];
        handler->PhysicalProperties = features;


        mageFreeMethod(devices);
        mageFreeMethod(deviceScores);
        
    #endif
    mageTryDumpSuccess(1, success);
}
void mageVulkanHandlerCreateDevice(mageVulkanHandler *handler, mageWindow *window, uint8 *success)
{
    mageTryDumpSuccess(0, success);
    #if defined (MAGE_VULKAN)
        /*!************************
            Creating device
        **************************/
       
        VkResult result = vkCreateDevice(handler->PhysicalDevice,
                &(VkDeviceCreateInfo) {
                    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                    .queueCreateInfoCount = 1,
                    .pQueueCreateInfos = &(VkDeviceQueueCreateInfo) {
                    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .queueFamilyIndex = 0,
                    .queueCount = 1,
                    .pQueuePriorities = (float []) { 1.0f },
                    },
                    .enabledExtensionCount = 1,
                    .ppEnabledExtensionNames = RequiredDeviceExtensions,
                },
                NULL,
                &handler->Device);

        if (result != VK_SUCCESS)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Vulkan device has failed to be created\n", NULL);
            return;
        }


        MAGE_LOG_CORE_INFORM("Vulkan device has been created\n", NULL); 
    #endif
    mageTryDumpSuccess(1, success);
}
void mageVulkanHandlerCreateSurface(mageVulkanHandler *handler, mageWindow *window, uint8 *success)
{
    #if defined (MAGE_VULKAN)
        
        /*!************************
            Creating surface
        **************************/
        
        #if defined (MAGE_GLFW)
            VkResult result = glfwCreateWindowSurface(handler->Instance, window->Context, NULL, &handler->Surface);

            if (result != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("GLFW has failed to create a window surface\n", NULL);
                mageTryDumpSuccess(0, success);
                return;  
            }
            MAGE_LOG_CORE_INFORM("GLFW has succesfully created a window surface\n", NULL);
        #endif

        uint8 flag;
        mageVulkanIsDeviceSuitable(handler->PhysicalDevice, handler, &flag);
        
        if (flag)
        {
            MAGE_LOG_CORE_INFORM("Physical device supports KHR surface\n", NULL);
            handler->GraphicsPresentFamily = handler->GraphicsFamilyIndex;
        }
    
    #endif
    mageTryDumpSuccess(1, success);
}
void mageVulkanHandlerInitialise(mageVulkanHandler *handler, mageWindow *window, uint8 *success)
{
    mageTryDumpSuccess(0, success);
    #if defined (MAGE_VULKAN)


        uint8 flag;
        typedef void (*vulkanFunctions)(mageVulkanHandler *, mageWindow *, uint8 *);
        
        uint32 functionCount = 4; 
        uint32 i;
        vulkanFunctions funtions[] = 
        {         
            mageVulkanHandlerCreateInstance,
  
            mageVulkanPickDevice, 
            
            mageVulkanHandlerCreateDevice,   

            mageVulkanHandlerCreateSurface,         
        };

        for (i = 0; i < functionCount; i++)
        {
            funtions[i](handler, window, &flag);
        }
        
    
    #endif
    mageTryDumpSuccess(1, success);
}
void mageVulkanHandlerCleanup(mageVulkanHandler *handler)
{
    #if defined (MAGE_VULKAN)
        vkDestroyDevice(handler->Device, NULL);
        vkDestroySurfaceKHR(handler->Instance, handler->Surface, NULL);
        vkDestroyInstance(handler->Instance, NULL);
    #endif
}
void *mageRendererAllocate()
{
    return malloc(sizeof(struct MAGE_RENDERER_STRUCT));
}
void mageRendererInitialise(mageRenderer *renderer, mageWindow *window, uint8 *success)
{
    #if defined (MAGE_VULKAN)
        mageVulkanHandlerInitialise(&renderer->Handler, window, success);
    #endif
}
void mageRendererDestroy(mageRenderer *renderer)
{
    #if defined (MAGE_VULKAN)
        mageVulkanHandlerCleanup(&renderer->Handler);
    #endif
}