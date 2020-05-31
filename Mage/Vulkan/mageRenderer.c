/*!************************
 * This file handles the vulkan setup required to render
 * Only compiled when using vulkan
 * #if defined macros should guard the code so if it is compiled no errors should occur 
**************************/
#include "../mageAPI.h"

#if defined(MAGE_VULKAN)

    mageResult mageCreateInstance(mageVulkanHandler *handler, mageWindow *window)
    {
        VkInstanceCreateInfo createInfo;
        VkApplicationInfo applicationInfo;
        memset(&createInfo, 0, sizeof(VkInstanceCreateInfo));
        memset(&applicationInfo, 0, sizeof(VkApplicationInfo));

        applicationInfo.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.apiVersion       = VK_API_VERSION_1_2;
        applicationInfo.pApplicationName = window->Title;
        applicationInfo.pEngineName      = "MAGE-ENGINE";
        applicationInfo.engineVersion    = VK_MAKE_VERSION(1, 0, 0);



        createInfo.sType                 = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo      = &applicationInfo;
        

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

        queueCreateInfo.sType                 = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueCount            = 1;
        queueCreateInfo.queueFamilyIndex      = handler->GraphicsFamilyIndex ;
        queueCreateInfo.pQueuePriorities      = queuePriorites;

        deviceCreateInfo.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pQueueCreateInfos    = &queueCreateInfo;


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
                MAGE_LOG_CORE_FATAL_ERROR("Failed to create Vulkan command pool\n", NULL);
                return MAGE_COMMAND_POOL_FAILURE;
            }
            MAGE_LOG_CORE_INFORM("Command pool has been created succesfully\n", NULL);
        }
        {
            VkCommandBufferAllocateInfo allocateInfo;        
            memset(&allocateInfo, 0, sizeof(VkCommandBufferAllocateInfo));

            allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocateInfo.commandPool            = handler->CommandPool;
            allocateInfo.commandBufferCount     = 1;
            allocateInfo.level                  = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

            VkResult result = vkAllocateCommandBuffers(handler->Device, &allocateInfo, &handler->CommandBuffer);

            if (result != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Command buffer allocation has failed\n", NULL);
                return MAGE_ALLOCATE_COMMAND_FAILURE;
            }

            MAGE_LOG_CORE_INFORM("Command buffer allocation was succesfull\n", NULL);
        }
        {
            VkCommandBufferBeginInfo bufferBeginInfo;
            VkSubmitInfo submitInfo;
            memset(&bufferBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));
            memset(&submitInfo, 0, sizeof(VkSubmitInfo));
            bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            vkBeginCommandBuffer(handler->CommandBuffer, &bufferBeginInfo);
            vkEndCommandBuffer(handler->CommandBuffer);

            submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount   = 1;
            submitInfo.pCommandBuffers      = &handler->CommandBuffer;

            VkResult result = vkQueueSubmit(handler->Queue, 1, &submitInfo, VK_NULL_HANDLE);

            if (result != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Queue submition has failed\n", NULL);
                return MAGE_QUEUE_SUBMITION_FAILURE;
            }
            MAGE_LOG_CORE_INFORM("Queue submition has was succesfull\n", NULL);

            vkQueueWaitIdle(handler->Queue);

        }
        return MAGE_SUCCESS;
    }
    mageResult mageRendererInitialise(mageRenderer *renderer, mageWindow *window)
    {
        memset(&renderer->Handler.Device, 0, sizeof(VkDevice));
        memset(&renderer->Handler.PhysicalDevice, 0, sizeof(VkPhysicalDevice));
        memset(&renderer->Handler.CommandPool, 0, sizeof(VkCommandPool));
        memset(&renderer->Handler.CommandBuffer, 0, sizeof(VkCommandBuffer));
        memset(&renderer->Handler.Queue, 0, sizeof(VkQueue));
        memset(&renderer->Handler.PhysicalProperties, 0, sizeof(VkPhysicalDeviceProperties));
        memset(&renderer->Handler.Fence, 0, sizeof(VkFence));

        typedef mageResult (*requiredFunctions)(mageVulkanHandler *, mageWindow *);
        const static uint32_t functionCount = 3;
        uint32_t i;

        requiredFunctions functions[] =
        {
            mageCreateInstance,
            mageCreateDevice,
            mageCreateCommandPool,
        };

        for (i = 0; i < functionCount; i++)
        {
            mageResult r = functions[i](&renderer->Handler, window);
            if (r != MAGE_SUCCESS) return r;
        }

        return MAGE_SUCCESS;
    }
    void mageVulkanHandlerClean(mageVulkanHandler *handler)
    {
        vkDestroyInstance(handler->Instance, NULL);
        vkDestroyDevice(handler->Device, NULL);
        vkDestroyCommandPool(handler->Device, handler->CommandPool, NULL);
    }
    void mageRendererDestroy(mageRenderer *renderer)
    {
        mageVulkanHandlerClean(&renderer->Handler);
    }

#endif