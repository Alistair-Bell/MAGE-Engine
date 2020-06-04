/*!************************
 * This file handles the vulkan setup required to render
 * Only compiled when using vulkan
 * #if defined macros should guard the code so if it is compiled no errors should occur 
**************************/
#include <mageAPI.h>

#if defined(MAGE_VULKAN)

    mageResult mageCreateInstance(mageVulkanHandler *handler, mageWindow *window)
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
        
        #if defined(MAGE_GLFW)
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
        mageFreeMethod(properties);
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

        mageFreeMethod(scores);

        return devices[index];
    }
    mageResult mageCreateDevice(mageVulkanHandler *handler, mageWindow *window)
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

            MAGE_LOG_CORE_INFORM("GPU selected %s\n", handler->PhysicalProperties.deviceName);
            mageFreeMethod(devices);
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

        vkGetDeviceQueue(handler->Device, handler->GraphicsFamilyIndex, 0, &handler->Queue);

        return MAGE_SUCCESS;
    }
    
    mageResult mageCreateFence(mageVulkanHandler *handler, mageWindow *window)
    {
        VkFenceCreateInfo fenceCreateInfo;
        memset(&fenceCreateInfo, 0, sizeof(VkFenceCreateInfo));
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        
        if (vkCreateFence(handler->Device, &fenceCreateInfo, NULL, &handler->Fence) != VK_SUCCESS)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Fence creation has failed\n", NULL);
            return MAGE_FENCE_CREATION_FAILURE;
        }
        MAGE_LOG_CORE_INFORM("Fence creation was succesfull\n", NULL);

        return MAGE_SUCCESS;
    }
    mageResult mageCreateSemaphore(mageVulkanHandler *handler, mageWindow *window)
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo;
        memset(&semaphoreCreateInfo, 0, sizeof(VkSemaphoreCreateInfo));
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        
        if (vkCreateSemaphore(handler->Device, &semaphoreCreateInfo, NULL, &handler->Semaphore) != VK_SUCCESS)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Semaphore creation has failed\n", NULL);
            return MAGE_SEMAPHORE_CREATION_FAILURE;
        }

        MAGE_LOG_CORE_INFORM("Semaphore creation was succesfull\n", NULL);

        return MAGE_SUCCESS;
    }
    mageResult mageCreateCommandPool(mageVulkanHandler *handler, mageWindow *window)
    {
        {   
            VkCommandPoolCreateInfo commandPoolInfo;
            memset(&commandPoolInfo, 0, sizeof(VkCommandPoolCreateInfo));
            
            commandPoolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            commandPoolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            commandPoolInfo.queueFamilyIndex = handler->GraphicsFamilyIndex;

            VkResult result = vkCreateCommandPool(handler->Device, &commandPoolInfo, NULL, &handler->CommandPool);

            if (result != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Failed to create Vulkan command pool 1\n", NULL);
                return MAGE_COMMAND_POOL_CREATION_FAILURE;
            }
            MAGE_LOG_CORE_INFORM("Command pool has been created succesfully\n", NULL);
        }
        {
            VkCommandBufferAllocateInfo allocateInfo;        
            memset(&allocateInfo, 0, sizeof(VkCommandBufferAllocateInfo));

            allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocateInfo.commandPool            = handler->CommandPool;
            allocateInfo.commandBufferCount     = 2;
            allocateInfo.level                  = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

            if (vkAllocateCommandBuffers(handler->Device, &allocateInfo, handler->CommandBuffer) != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Command buffer allocation has failed\n", NULL);
                return MAGE_ALLOCATE_COMMAND_FAILURE;
            }

            MAGE_LOG_CORE_INFORM("Command buffer allocation was succesfull\n", NULL);
        }
        {
            VkCommandBufferBeginInfo bufferBeginInfo;
            memset(&bufferBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));
            
            bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            vkBeginCommandBuffer(handler->CommandBuffer[0], &bufferBeginInfo);

            vkCmdPipelineBarrier(handler->CommandBuffer[0], 
                                VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                                VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                                0,
                                0, NULL,
                                0, NULL,
                                0, NULL);

            handler->Viewport.height    = window->Height;
            handler->Viewport.width     = window->Width;
            handler->Viewport.x         = 0;
            handler->Viewport.y         = 0;
            handler->Viewport.maxDepth  = 0.0f;
            vkCmdSetViewport(handler->CommandBuffer[0], 0, 1, &handler->Viewport);
            
            MAGE_LOG_CORE_INFORM("Command viewport 1 set\n", NULL);
            
            vkEndCommandBuffer(handler->CommandBuffer[0]);
        }
        {
            VkCommandBufferBeginInfo bufferBeginInfo;
            memset(&bufferBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));
            
            bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            vkBeginCommandBuffer(handler->CommandBuffer[1], &bufferBeginInfo);

            handler->Viewport.height    = window->Height;
            handler->Viewport.width     = window->Width;
            handler->Viewport.x         = 0;
            handler->Viewport.y         = 0;
            handler->Viewport.maxDepth  = 0.0f;
            vkCmdSetViewport(handler->CommandBuffer[1], 0, 1, &handler->Viewport);
            
            MAGE_LOG_CORE_INFORM("Command viewport 2 set\n", NULL);
            
            vkEndCommandBuffer(handler->CommandBuffer[1]);
        }
        {
            VkSubmitInfo submitInfo;
            memset(&submitInfo, 0, sizeof(VkSubmitInfo));
            submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount   = 1;
            submitInfo.pCommandBuffers      = &handler->CommandBuffer[0];
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores    = &handler->Semaphore;

            if (vkQueueSubmit(handler->Queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Queue submition has failed\n", NULL);
                return MAGE_QUEUE_SUBMITION_FAILURE;
            }
            MAGE_LOG_CORE_INFORM("Queue submition 1 was succesfull\n", NULL);
        }
        {
            const VkPipelineStageFlags flags[] = { VK_PIPELINE_STAGE_ALL_COMMANDS_BIT };
            VkSubmitInfo submitInfo;
            memset(&submitInfo, 0, sizeof(VkSubmitInfo));
            submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount   = 1;
            submitInfo.pCommandBuffers      = &handler->CommandBuffer[1];
            submitInfo.waitSemaphoreCount   = 1;
            submitInfo.pWaitSemaphores      = &handler->Semaphore;
            submitInfo.pWaitDstStageMask    = flags;

            if (vkQueueSubmit(handler->Queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Queue submition 2 has failed\n", NULL);
                return MAGE_QUEUE_SUBMITION_FAILURE;
            }
            MAGE_LOG_CORE_INFORM("Queue submition 2 was succesfull\n", NULL);
        }

        return MAGE_SUCCESS;
    }
    mageResult mageCreateSurface(mageVulkanHandler *handler, mageWindow *window)
    {
        #if defined(MAGE_GLFW)
            
            if (glfwCreateWindowSurface(handler->Instance, window->Context, NULL, &handler->Surface))
            {
                MAGE_LOG_CORE_FATAL_ERROR("Vulkan surface has failed to be created (glfw mode)\n", NULL);
                return MAGE_SURFACE_CREATION_FAILURE;
            }
            MAGE_LOG_CORE_INFORM("Vulkan surface has been created (glfw mode)\n", NULL);
            
            VkBool32 supported;
            memset(&handler->SurfaceCapabilities, 0, sizeof(VkSurfaceCapabilitiesKHR));
            
            vkGetPhysicalDeviceSurfaceSupportKHR(handler->PhysicalDevice, handler->GraphicsFamilyIndex, handler->Surface, &supported);

            if (supported != VK_TRUE)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Physical device does not support surface\n", NULL);
                return MAGE_HARDWARE_INVALID;
            }
            MAGE_LOG_CORE_INFORM("Physical device supports surface\n", NULL);
        
            if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handler->PhysicalDevice, handler->Surface, &handler->SurfaceCapabilities) != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Surface capabilities were not met\n", NULL);
                return MAGE_DEVICE_CREATION_FAILURE;
            }

            uint32_t formatCount = 0; 
            vkGetPhysicalDeviceSurfaceFormatsKHR(handler->PhysicalDevice, handler->Surface, &formatCount, NULL);

            if (formatCount <= 0) 
            {
                MAGE_LOG_CLIENT_INFORM("Format count is too low! %d found\n", formatCount);
                return MAGE_HARDWARE_NOT_PRESENT;
            }

            VkSurfaceFormatKHR *formats = calloc(formatCount, sizeof(VkSurfaceFormatKHR));
            vkGetPhysicalDeviceSurfaceFormatsKHR(handler->PhysicalDevice, handler->Surface, &formatCount, formats);

            if (formats[0].format == VK_FORMAT_UNDEFINED)
            {
                handler->SurfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
                handler->SurfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
            }
            else
            {
                handler->SurfaceFormat = formats[0];
            }

            MAGE_LOG_CORE_INFORM("Physical device formats found\n", NULL);


            mageFreeMethod(formats);


        #else  
            #error I am currently to lazy to add sdl support for now. Just use glfw
        #endif

       return MAGE_SUCCESS;
    }
    mageResult mageCreateSwapChain(mageVulkanHandler *handler, mageWindow *window)
    {   
        {
            uint32_t count = handler->SurfaceCapabilities.minImageCount;
            uint32_t i;
            uint32_t flag;
            if (count == 0) count++;
            VkSwapchainCreateInfoKHR createInfo;
            memset(&createInfo, 0, sizeof(VkSwapchainCreateInfoKHR));
            
            VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
            uint32_t presentModeCount;

            vkGetPhysicalDeviceSurfacePresentModesKHR(handler->PhysicalDevice, handler->Surface, &presentModeCount, NULL);
            VkPresentModeKHR *presentModes = calloc(presentModeCount, sizeof(VkPresentModeKHR));
            vkGetPhysicalDeviceSurfacePresentModesKHR(handler->PhysicalDevice, handler->Surface, &presentModeCount, presentModes);

            for (i = 0; i < presentModeCount; i++)
            {
                if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) presentMode = presentModes[i];
                flag = 1;
            }  
            if (flag) MAGE_LOG_CORE_INFORM("GPU supports mailbox present mode, using mailbox present mode over FIFO mode\n", NULL);



            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.sType					= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface					= handler->Surface;
            createInfo.minImageCount			= count;
            createInfo.imageFormat				= handler->SurfaceFormat.format;
            createInfo.imageColorSpace			= handler->SurfaceFormat.colorSpace;
            createInfo.imageExtent.width		= window->Width;
            createInfo.imageExtent.height		= window->Height;
            createInfo.imageArrayLayers			= 1;
            createInfo.imageUsage				= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            createInfo.imageSharingMode			= VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount	= 0;
            createInfo.pQueueFamilyIndices		= NULL;
            createInfo.preTransform				= VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
            createInfo.compositeAlpha			= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode				= presentMode;
            createInfo.clipped					= VK_TRUE;
            createInfo.oldSwapchain				= VK_NULL_HANDLE;
            

            if (vkCreateSwapchainKHR(handler->Device, &createInfo, NULL, &handler->SwapChain) != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Swap chain creation has failed\n", NULL);
                return MAGE_SWAPCHAIN_CREATION_FAILED;
            }

            MAGE_LOG_CLIENT_INFORM("Swap chain has been created succesfully\n", NULL);

            vkGetSwapchainImagesKHR(handler->Device, handler->SwapChain, &handler->SwapChainImageCount, NULL);

            mageFreeMethod(presentModes);
        }
            
       
        return MAGE_SUCCESS;
    }
    mageResult mageCreateSwapChainImages(mageVulkanHandler *handler, mageWindow *window)
    {
        {
            handler->SwapChainImages = calloc(handler->SwapChainImageCount, sizeof(VkImage));
            handler->SwapChainImageViews = calloc(handler->SwapChainImageCount, sizeof(VkImageView));
            memset(handler->SwapChainImages, 0, sizeof(VkImage) * handler->SwapChainImageCount);
            memset(handler->SwapChainImageViews, 0, sizeof(VkImageView) * handler->SwapChainImageCount);
        }
        uint32_t i;

        vkGetSwapchainImagesKHR(handler->Device, handler->SwapChain, &handler->SwapChainImageCount, handler->SwapChainImages);

        for (i = 0; i < handler->SwapChainImageCount; i++)
        {
            
            /*
                vkCreateImageView(handler->Device, &createInfo, NULL, handler->SwapChainImageViews[i]);
            */
        }

        return MAGE_SUCCESS;
    }
    mageResult mageRendererInitialise(mageRenderer *renderer, mageWindow *window)
    {
        typedef mageResult (*requiredFunctions)(mageVulkanHandler *, mageWindow *);
        uint32_t i;

        requiredFunctions functions[] =
        {
            mageCreateInstance,
            mageCreateDevice,
            mageCreateSemaphore,
            mageCreateFence,
            mageCreateCommandPool,
            mageCreateSurface,
            mageCreateSwapChain,
            mageCreateSwapChainImages,
    
        };
        const uint32_t functionCount = sizeof(functions) / sizeof(requiredFunctions);

        for (i = 0; i < functionCount; i++)
        {
            mageResult r = functions[i](&renderer->Handler, window);
            if (r != MAGE_SUCCESS) return r;
        }

        return MAGE_SUCCESS;
    }
    void mageVulkanHandlerClean(mageVulkanHandler *handler)
    {
        vkDestroyFence(handler->Device, handler->Fence, NULL);
        vkDestroyCommandPool(handler->Device, handler->CommandPool, NULL);
        vkDestroySurfaceKHR(handler->Instance, handler->Surface, NULL);
        vkDestroySwapchainKHR(handler->Device, handler->SwapChain, NULL);
        vkDestroyDevice(handler->Device, NULL);
        vkDestroyInstance(handler->Instance, NULL);
        MAGE_LOG_CORE_INFORM("Vulkan has been cleaned up\n", NULL);
    }
    void mageRendererDestroy(mageRenderer *renderer)
    {
        mageVulkanHandlerClean(&renderer->Handler);
    }

#endif
