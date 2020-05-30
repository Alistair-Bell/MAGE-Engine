/*!************************
 * This file handles the vulkan setup required to render
 * Only compiled when using vulkan
 * #if defined macros should guard the code so if it is compiled no errors should occur 
**************************/
#include "../mageAPI.h"

#if defined(MAGE_VULKAN)
    
    static const char * const RequiredDeviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, };
    static const uint32_t RequiredExtensionCount = 1;
    static const uint32_t MaxImageCount = 65000;
    
    typedef struct MAGE_QUEUE_FAMILIES_STRUCT
    {
        uint32_t *GraphicsFamily;
        
        uint32_t GraphicsCount;


    } mageQueueFamilies;

    static void mageQueueInitialise(mageQueueFamilies *queue)
    {
        queue->GraphicsCount = 0;
        queue->GraphicsFamily = calloc(0, sizeof(uint32_t));
    }    
    static void mageQueuePush(mageQueueFamilies *queue, uint32_t graphicsValue)
    {
        queue->GraphicsCount++;
        queue->GraphicsFamily = calloc(graphicsValue, sizeof(uint32_t));
        queue->GraphicsFamily[queue->GraphicsCount -1] = graphicsValue;
    }
    static void mageDeviceFindQueue(VkPhysicalDevice device, mageQueueFamilies *queue)
    {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);

        VkQueueFamilyProperties *queueFamilies = calloc(queueFamilyCount, sizeof(VkQueueFamilyProperties));
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

        uint32_t i = 0;

        for (i = 0; i < queueFamilyCount; i++)
        {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                mageQueuePush(queue, i);
                MAGE_LOG_CORE_INFORM("Graphics card graphics queue found\n", NULL);
            }
        }
    }
    static void mageQueueFree(mageQueueFamilies *queue)
    {
        mageFreeMethod(queue->GraphicsFamily); 
    }
    static mageResult mageCreateInstance(mageVulkanHandler *handler, mageWindow *window)
    {
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
        
        createInformation.ppEnabledLayerNames = NULL;
        createInformation.enabledLayerCount = 0;
        
        #if defined (MAGE_GLFW)

            uint32_t glfwExtensionCount;
        
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
            return MAGE_INSTANCE_CREATION_FAILURE;
        }
        MAGE_LOG_CORE_INFORM("Vulkan instance has been created\n", NULL);
    

        return MAGE_SUCCESS;
    }
    static mageResult mageIsDeviceSuitable(VkPhysicalDevice device)
    {
        uint32_t flag;
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);   
        
        flag = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;

        if (!flag) return MAGE_HARDWARE_INVALID;

        mageQueueFamilies families;
        mageQueueInitialise(&families);

        mageDeviceFindQueue(device, &families);

        if (families.GraphicsCount <= 0) return MAGE_HARDWARE_INVALID;

        mageQueueFree(&families);

        return MAGE_SUCCESS;
    }
    static mageResult magePickPhysicalDevice(mageVulkanHandler *handler)
    {
        uint32_t deviceCount, i;
        vkEnumeratePhysicalDevices(handler->Instance, &deviceCount, NULL);

        if (deviceCount <= 0)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Unable to find any physical devices\n", NULL);
            return MAGE_HARDWARE_NOT_PRESENT;
        }
        VkPhysicalDevice *devices = calloc(deviceCount, sizeof(VkPhysicalDevice));
        vkEnumeratePhysicalDevices(handler->Instance, &deviceCount, devices);

        uint8_t flag = 0;    
        for (i = 0; i < deviceCount; i++)
        {
            if (mageIsDeviceSuitable(devices[i]) == MAGE_SUCCESS) 
            {
                handler->PhysicalDevice = devices[i];
                vkGetPhysicalDeviceProperties(handler->PhysicalDevice, &handler->PhysicalProperties);
                MAGE_LOG_CORE_INFORM("Physical device chosen %s\n", handler->PhysicalProperties.deviceName);
                vkGetPhysicalDeviceMemoryProperties(handler->PhysicalDevice, &handler->MemoryProperties);
                flag = 1;
                break;
            }

        }
        if (!flag)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Unable to find suitable device for use\n", NULL);
            return MAGE_HARDWARE_NOT_PRESENT;
        }
        
        mageFreeMethod(devices);
        return MAGE_SUCCESS;
    }
    static mageResult mageCreateDevice(mageVulkanHandler *handler)
    {
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
                .enabledExtensionCount = RequiredExtensionCount,
                .ppEnabledExtensionNames = RequiredDeviceExtensions,
            },
            NULL,
            &handler->Device);

        if (result != VK_SUCCESS)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Vulkan logical device has failed to be created\n", NULL);
            return MAGE_DEVICE_CREATION_FAILURE;
        }
        MAGE_LOG_CORE_INFORM("Vulkan logical device has been created\n", NULL); 

        return MAGE_SUCCESS;
    }
    static mageResult mageCreateSurface(mageVulkanHandler *handler, mageWindow *window)
    {
        #if defined(MAGE_GLFW)

            VkResult result = glfwCreateWindowSurface(handler->Instance, window->Context, NULL, &handler->Surface);

            if (result != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Vulkan surface has failed to be created!\n", NULL);
                return MAGE_SURFACE_CREATION_FAILURE;
            }
            MAGE_LOG_CORE_INFORM("Surface has been created succesfully\n", NULL);

        #endif
        return MAGE_SUCCESS;
    }
    static mageResult mageCreateSwapchain(mageVulkanHandler *handler, mageWindow *window)
    {
        VkSurfaceCapabilitiesKHR surfaceCaps;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handler->PhysicalDevice, handler->Surface, &surfaceCaps);
        
        VkBool32 supported;
        vkGetPhysicalDeviceSurfaceSupportKHR(handler->PhysicalDevice, 0, handler->Surface, &supported);
        assert(supported);

        uint32_t count;
        vkGetPhysicalDeviceSurfacePresentModesKHR(handler->PhysicalDevice, handler->Surface, &count, NULL);
        VkPresentModeKHR *presentModes = calloc(count, sizeof(VkPresentModeKHR));
        vkGetPhysicalDeviceSurfacePresentModesKHR(handler->PhysicalDevice, handler->Surface, &count, presentModes);
        uint32_t i;
        VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
        for (i = 0; i < count; i++) 
        {
            if (presentModes[i] == VK_PRESENT_MODE_FIFO_KHR) 
            {
                presentMode = VK_PRESENT_MODE_FIFO_KHR;
                break;
            }
        }

        uint32_t minImageCount = 2;
        if (minImageCount < surfaceCaps.minImageCount) 
        {
            if (surfaceCaps.minImageCount > MaxImageCount)
                return MAGE_UNKNOWN;
            minImageCount = surfaceCaps.minImageCount;
        }

        if (surfaceCaps.maxImageCount > 0 && minImageCount > surfaceCaps.maxImageCount) 
        {
            minImageCount = surfaceCaps.maxImageCount;
        }
        vkCreateSwapchainKHR(handler->Device,
            &(VkSwapchainCreateInfoKHR) {
                .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                .surface = handler->Surface,
                .minImageCount = minImageCount,
                .imageFormat = handler->Format,
                .imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR,
                .imageExtent = { window->Width, window->Height },
                .imageArrayLayers = 1,
                .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
                .queueFamilyIndexCount = 1,
                .pQueueFamilyIndices = (uint32_t[]) { 0 },
                .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                .presentMode = presentMode,
            }, NULL, &handler->SwapChain);

        /*
        vkGetSwapchainImagesKHR(handler->Device, handler->SwapChain, &handler->ImageCount, NULL);
        
        VkImage *swapChainImages = calloc(handler->ImageCount, sizeof(VkImage));
        vkGetSwapchainImagesKHR(handler->Device, handler->SwapChain, &handler->ImageCount, swapChainImages);

        mageFreeMethod(presentModes);
        mageFreeMethod(swapChainImages);

        */
        return MAGE_SUCCESS;
    }
    mageResult mageVulkanHandlerInitialise(mageVulkanHandler *handler, mageWindow *window)
    {
        mageCreateInstance(handler, window);
        magePickPhysicalDevice(handler);
        mageCreateDevice(handler);
        mageCreateSurface(handler, window);
        mageCreateSwapchain(handler, window);

        return MAGE_SUCCESS;
    }
    mageResult mageRendererInitialise(mageRenderer *renderer, mageWindow *window)
    {
        return mageVulkanHandlerInitialise(&renderer->Handler, window);
    }

    void mageVulkanHandlerCleanup(mageVulkanHandler *handler)
    {
        vkDestroyDevice(handler->Device, NULL);
    }
#endif