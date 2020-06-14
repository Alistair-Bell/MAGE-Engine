#include <mageAPI.h>

#if defined (MAGE_VULKAN)

    static mageResult mageCreateFence(struct mageRenderer *renderer, struct mageWindow *window)
    {
        VkFenceCreateInfo fenceCreateInfo;
        memset(&fenceCreateInfo, 0, sizeof(VkFenceCreateInfo));
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        
        if (vkCreateFence(renderer->Handler.Device, &fenceCreateInfo, NULL, &renderer->Fence) != VK_SUCCESS)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Fence creation has failed\n", NULL);
            return MAGE_FENCE_CREATION_FAILURE;
        }
        MAGE_LOG_CORE_INFORM("Fence creation was succesfull\n", NULL);

        return MAGE_SUCCESS;
    }
    static mageResult mageCreateSemaphore(struct mageRenderer *renderer, struct mageWindow *window)
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo;
        memset(&semaphoreCreateInfo, 0, sizeof(VkSemaphoreCreateInfo));
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        
        if (vkCreateSemaphore(renderer->Handler.Device, &semaphoreCreateInfo, NULL, &renderer->Semaphore) != VK_SUCCESS)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Semaphore creation has failed\n", NULL);
            return MAGE_SEMAPHORE_CREATION_FAILURE;
        }

        MAGE_LOG_CORE_INFORM("Semaphore creation was succesfull\n", NULL);

        return MAGE_SUCCESS;
    }
    static mageResult mageCreateCommandPool(struct mageRenderer *renderer, struct mageWindow *window)
    {
        vkGetDeviceQueue(renderer->Handler.Device, renderer->Handler.GraphicsFamilyIndex, 0, &renderer->GraphicsQueue);
        {   
            VkCommandPoolCreateInfo commandPoolInfo;
            memset(&commandPoolInfo, 0, sizeof(VkCommandPoolCreateInfo));
            
            commandPoolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            commandPoolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            commandPoolInfo.queueFamilyIndex = renderer->Handler.GraphicsFamilyIndex;

            VkResult result = vkCreateCommandPool(renderer->Handler.Device, &commandPoolInfo, NULL, &renderer->CommandPool);

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
            allocateInfo.commandPool            = renderer->CommandPool;
            allocateInfo.commandBufferCount     = 2;
            allocateInfo.level                  = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

            if (vkAllocateCommandBuffers(renderer->Handler.Device, &allocateInfo, renderer->CommandBuffer) != VK_SUCCESS)
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
            vkBeginCommandBuffer(renderer->CommandBuffer[0], &bufferBeginInfo);

            vkCmdPipelineBarrier(renderer->CommandBuffer[0], 
                                VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                                VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                                0,
                                0, NULL,
                                0, NULL,
                                0, NULL);

            renderer->Viewport.height    = window->Height;
            renderer->Viewport.width     = window->Width;
            renderer->Viewport.x         = 0;
            renderer->Viewport.y         = 0;
            renderer->Viewport.maxDepth  = 0.0f;
            vkCmdSetViewport(renderer->CommandBuffer[0], 0, 1, &renderer->Viewport);
            
            MAGE_LOG_CORE_INFORM("Command viewport 1 set\n", NULL);
            
            vkEndCommandBuffer(renderer->CommandBuffer[0]);
        }
        {
            VkCommandBufferBeginInfo bufferBeginInfo;
            memset(&bufferBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));
            
            bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            vkBeginCommandBuffer(renderer->CommandBuffer[1], &bufferBeginInfo);

            renderer->Viewport.height    = window->Height;
            renderer->Viewport.width     = window->Width;
            renderer->Viewport.x         = 0;
            renderer->Viewport.y         = 0;
            renderer->Viewport.maxDepth  = 0.0f;
            vkCmdSetViewport(renderer->CommandBuffer[1], 0, 1, &renderer->Viewport);
            
            MAGE_LOG_CORE_INFORM("Command viewport 2 set\n", NULL);
            
            vkEndCommandBuffer(renderer->CommandBuffer[1]);
        }
        {
            VkSubmitInfo submitInfo;
            memset(&submitInfo, 0, sizeof(VkSubmitInfo));
            submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount   = 1;
            submitInfo.pCommandBuffers      = &renderer->CommandBuffer[0];
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores    = &renderer->Semaphore;

            if (vkQueueSubmit(renderer->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
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
            submitInfo.pCommandBuffers      = &renderer->CommandBuffer[1];
            submitInfo.waitSemaphoreCount   = 1;
            submitInfo.pWaitSemaphores      = &renderer->Semaphore;
            submitInfo.pWaitDstStageMask    = flags;

            if (vkQueueSubmit(renderer->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Queue submition 2 has failed\n", NULL);
                return MAGE_QUEUE_SUBMITION_FAILURE;
            }
            MAGE_LOG_CORE_INFORM("Queue submition 2 was succesfull\n", NULL);
        }

        return MAGE_SUCCESS;
    }
    static mageResult mageCreateSurface(struct mageRenderer *renderer, struct mageWindow *window)
    {
        #if defined (MAGE_GLFW)
            
            if (glfwCreateWindowSurface(renderer->Handler.Instance, window->Context, NULL, &renderer->Surface))
            {
                MAGE_LOG_CORE_FATAL_ERROR("Vulkan surface has failed to be created (glfw mode)\n", NULL);
                return MAGE_SURFACE_CREATION_FAILURE;
            }
            MAGE_LOG_CORE_INFORM("Vulkan surface has been created (glfw mode)\n", NULL);
            
            VkBool32 supported;
            memset(&renderer->Handler.SurfaceCapabilities, 0, sizeof(VkSurfaceCapabilitiesKHR));
            
            vkGetPhysicalDeviceSurfaceSupportKHR(renderer->Handler.PhysicalDevice, renderer->Handler.GraphicsFamilyIndex, renderer->Surface, &supported);

            if (supported != VK_TRUE)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Physical device does not support surface\n", NULL);
                return MAGE_HARDWARE_INVALID;
            }
            MAGE_LOG_CORE_INFORM("Physical device supports surface\n", NULL);
        
            if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(renderer->Handler.PhysicalDevice, renderer->Surface, &renderer->Handler.SurfaceCapabilities) != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Surface capabilities were not met\n", NULL);
                return MAGE_DEVICE_CREATION_FAILURE;
            }

            uint32_t formatCount = 0; 
            vkGetPhysicalDeviceSurfaceFormatsKHR(renderer->Handler.PhysicalDevice, renderer->Surface, &formatCount, NULL);

            if (formatCount <= 0) 
            {
                MAGE_LOG_CLIENT_INFORM("Format count is too low! %d found\n", formatCount);
                return MAGE_HARDWARE_NOT_PRESENT;
            }

            VkSurfaceFormatKHR *formats = calloc(formatCount, sizeof(VkSurfaceFormatKHR));
            vkGetPhysicalDeviceSurfaceFormatsKHR(renderer->Handler.PhysicalDevice, renderer->Surface, &formatCount, formats);

            if (formats[0].format == VK_FORMAT_UNDEFINED)
            {
                renderer->Handler.SurfaceFormat.format       = VK_FORMAT_B8G8R8A8_UNORM;
                renderer->Handler.SurfaceFormat.colorSpace   = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
            }
            else
            {
                renderer->Handler.SurfaceFormat = formats[0];
            }

            MAGE_LOG_CORE_INFORM("Physical device formats found\n", NULL);


            free(formats);


        #else  
            #error I am currently to lazy to add sdl support for now. Just use glfw
        #endif

       return MAGE_SUCCESS;
    }
    static mageResult mageCreateSwapChain(struct mageRenderer *renderer, struct mageWindow *window)
    {   
        {
            uint32_t count = renderer->Handler.SurfaceCapabilities.minImageCount;
            uint32_t i;
            uint32_t flag;
            if (count == 0) count++;
            VkSwapchainCreateInfoKHR createInfo;
            memset(&createInfo, 0, sizeof(VkSwapchainCreateInfoKHR));
            
            VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
            uint32_t presentModeCount;

            vkGetPhysicalDeviceSurfacePresentModesKHR(renderer->Handler.PhysicalDevice, renderer->Surface, &presentModeCount, NULL);
            VkPresentModeKHR *presentModes = calloc(presentModeCount, sizeof(VkPresentModeKHR));
            vkGetPhysicalDeviceSurfacePresentModesKHR(renderer->Handler.PhysicalDevice, renderer->Surface, &presentModeCount, presentModes);

            for (i = 0; i < presentModeCount; i++)
            {
                if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) presentMode = presentModes[i];
                flag = 1;
            }  
            if (flag) MAGE_LOG_CORE_INFORM("GPU supports mailbox present mode, using mailbox present mode over FIFO mode\n", NULL);



            createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.sType					= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface					= renderer->Surface;
            createInfo.minImageCount			= count;
            createInfo.imageFormat				= renderer->Handler.SurfaceFormat.format;
            createInfo.imageColorSpace			= renderer->Handler.SurfaceFormat.colorSpace;
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
            

            if (vkCreateSwapchainKHR(renderer->Handler.Device, &createInfo, NULL, &renderer->SwapChain) != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Swap chain creation has failed\n", NULL);
                return MAGE_SWAPCHAIN_CREATION_FAILED;
            }

            MAGE_LOG_CORE_INFORM("Swap chain has been created succesfully\n", NULL);

            vkGetSwapchainImagesKHR(renderer->Handler.Device, renderer->SwapChain, &renderer->SwapChainImageCount, NULL);

            free(presentModes);
        }
            
       
        return MAGE_SUCCESS;
    }
    static mageResult mageCreateSwapChainImages(struct mageRenderer *renderer, struct mageWindow *window)
    {
        {
            renderer->SwapChainImages = calloc(renderer->SwapChainImageCount, sizeof(VkImage));
            renderer->SwapChainImageViews = calloc(renderer->SwapChainImageCount, sizeof(VkImageView));
            memset(renderer->SwapChainImages, 0, sizeof(VkImage) * renderer->SwapChainImageCount);
            memset(renderer->SwapChainImageViews, 0, sizeof(VkImageView) * renderer->SwapChainImageCount);
        }
        uint32_t i;

        vkGetSwapchainImagesKHR(renderer->Handler.Device, renderer->SwapChain, &renderer->SwapChainImageCount, renderer->SwapChainImages);

        for (i = 0; i < renderer->SwapChainImageCount; i++)
        {
            VkImageViewCreateInfo createInfo;
            memset(&createInfo, 0, sizeof(VkImageViewCreateInfo));
            createInfo.sType                                = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image                                = renderer->SwapChainImages[i];
            createInfo.viewType                             = VK_IMAGE_TYPE_2D;
            createInfo.format                               = renderer->Handler.SurfaceFormat.format;
            createInfo.components.r                         = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g                         = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b                         = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a                         = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask          = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel        = 0;
            createInfo.subresourceRange.levelCount          = 1;
            createInfo.subresourceRange.baseArrayLayer      = 0;
            createInfo.subresourceRange.layerCount          = 1;
        
            if (vkCreateImageView(renderer->Handler.Device, &createInfo, NULL, &renderer->SwapChainImageViews[i]) != VK_SUCCESS)
            {
                MAGE_LOG_CORE_ERROR("Swap chain view %d of %d failed to be created\n", i, renderer->SwapChainImageCount);
            }

            MAGE_LOG_CORE_INFORM("Swap chain image view %d created\n", i);
        }   

        return MAGE_SUCCESS;
    }
    static mageResult mageCreateDepthStencilImage(struct mageRenderer *renderer, struct mageWindow *window)
    {
        {
            const VkFormat const tryFormats[] = 
            {   
                VK_FORMAT_D32_SFLOAT_S8_UINT,
                VK_FORMAT_D24_UNORM_S8_UINT,
                VK_FORMAT_D16_UNORM_S8_UINT,
                VK_FORMAT_D32_SFLOAT,
                VK_FORMAT_D16_UNORM
            };
            uint32_t i; 
            uint32_t count = (sizeof(tryFormats) / sizeof(VkFormat));
            renderer->Handler.DephStencilAvailable = 0;
            
            for (i = 0; i < 5; i++) 
            {
                VkFormatProperties formatProperties;
                memset(&formatProperties, 0, sizeof(VkFormatProperties));
                VkFormat format = tryFormats[i];

                vkGetPhysicalDeviceFormatProperties(renderer->Handler.PhysicalDevice, format, &formatProperties);
                if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) 
                {
                    renderer->DepthStencilFormat = format;
                    break;
                }
		    }
            if (renderer->DepthStencilFormat == VK_FORMAT_UNDEFINED)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Deph stencil format was not selected\n", NULL);
                return MAGE_HARDWARE_INVALID;
            }
            if ((renderer->DepthStencilFormat == VK_FORMAT_D32_SFLOAT_S8_UINT ) || ( renderer->DepthStencilFormat == VK_FORMAT_D24_UNORM_S8_UINT ) || ( renderer->DepthStencilFormat == VK_FORMAT_D16_UNORM_S8_UINT ) || ( renderer->DepthStencilFormat == VK_FORMAT_S8_UINT )) 
            {
                renderer->Handler.DephStencilAvailable = 1;
                MAGE_LOG_CORE_INFORM("Deph stencil available and chosen\n", NULL);
            }

        }
        
        VkImageCreateInfo imageCreateInfo;
        memset(&imageCreateInfo, 0, sizeof(VkImageCreateInfo));
        imageCreateInfo.sType                    = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.flags                    = 0;
        imageCreateInfo.imageType                = VK_IMAGE_TYPE_2D;
        imageCreateInfo.pNext                    = NULL;
        imageCreateInfo.format                   = renderer->DepthStencilFormat;
        imageCreateInfo.extent.depth             = 1;
        imageCreateInfo.extent.height            = window->Height;
        imageCreateInfo.extent.width             = window->Width; 
        imageCreateInfo.mipLevels                = 1;
        imageCreateInfo.arrayLayers              = 1;
        imageCreateInfo.samples                  = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.tiling                   = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.usage                    = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        imageCreateInfo.sharingMode              = VK_SHARING_MODE_EXCLUSIVE;
        imageCreateInfo.queueFamilyIndexCount    = VK_QUEUE_FAMILY_IGNORED;
        imageCreateInfo.pQueueFamilyIndices      = NULL;
        imageCreateInfo.initialLayout            = VK_IMAGE_LAYOUT_UNDEFINED;

        if (vkCreateImage(renderer->Handler.Device, &imageCreateInfo, NULL, &renderer->DepthStencilImage) != VK_SUCCESS)
        {
            MAGE_LOG_CLIENT_FATAL_ERROR("Deph stencil image has failed to been created\n", NULL);
            return MAGE_IMAGE_CREATION_FAILURE;
        }
        /*
        VkMemoryRequirements requirements;
        memset(&requirements, 0, sizeof(VkMemoryRequirements));
        vkGetImageMemoryRequirements(renderer->Handler.Device, renderer->Handler.DepthStencilImage, &requirements);

        VkMemoryAllocateInfo memoryAllocateInfo;
        memset(&memoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));
        memoryAllocateInfo.sType            = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.pNext            = NULL;
        memoryAllocateInfo.allocationSize   = requirements.size;
        memoryAllocateInfo.memoryTypeIndex  = ;

        vkAllocateMemory(renderer->Handler.Device, &memoryAllocateInfo, NULL, &renderer->Handler.DeviceMemory);
        */

        VkImageViewCreateInfo viewCreateInfo;
        memset(&viewCreateInfo, 0, sizeof(VkImageViewCreateInfo));

        viewCreateInfo.sType                                = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;                                
        viewCreateInfo.image                                = renderer->DepthStencilImage;
        viewCreateInfo.viewType                             = VK_IMAGE_VIEW_TYPE_2D;                             
        viewCreateInfo.format                               = renderer->DepthStencilFormat;                            
        viewCreateInfo.components.r                         = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewCreateInfo.components.g                         = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewCreateInfo.components.b                         = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewCreateInfo.components.a                         = VK_COMPONENT_SWIZZLE_IDENTITY;
        viewCreateInfo.subresourceRange.aspectMask		    = VK_IMAGE_ASPECT_DEPTH_BIT | ( renderer->Handler.DephStencilAvailable ? VK_IMAGE_ASPECT_STENCIL_BIT : 0 ); 
        viewCreateInfo.subresourceRange.baseMipLevel        = 0;
        viewCreateInfo.subresourceRange.levelCount          = 1;
        viewCreateInfo.subresourceRange.baseArrayLayer      = 0;
        viewCreateInfo.subresourceRange.layerCount          = 1;

        if (vkCreateImageView(renderer->Handler.Device, &viewCreateInfo, NULL, &renderer->DepthStencilImageView) != VK_SUCCESS)
        {
            MAGE_LOG_CLIENT_FATAL_ERROR("Deph stencil image view has failed to be created\n", NULL);
            return MAGE_IMAGE_VIEW_CREATION_FAILURE;
        }

        MAGE_LOG_CORE_INFORM("Deph stencil image created\n", NULL);
        return MAGE_SUCCESS;
    }
    mageResult mageRendererInitialise(struct mageRenderer *renderer, struct mageWindow *window)
    {
        mageResult result = mageVulkanHandlerInitialise(&renderer->Handler, window);

        if (result != MAGE_SUCCESS) 
        {
            return result;
        }
        
        typedef mageResult (*rendererSetupFunctions)(struct mageRenderer *, struct mageWindow *);

        const rendererSetupFunctions functions[] = 
        {
            mageCreateFence,
            mageCreateSemaphore,
            mageCreateCommandPool,
            mageCreateSurface,
            mageCreateSwapChain,
            mageCreateSwapChainImages,
            mageCreateDepthStencilImage,
        };
        const uint32_t functionCount = (sizeof(functions) / sizeof(rendererSetupFunctions));
        uint32_t i;

        for (i = 0; i < functionCount; i++)
        {
            result = functions[i](renderer, window);
            if (result != MAGE_SUCCESS) return result;
        }

    }
    void mageRendererCleanup(struct mageRenderer *renderer)
    {
        vkDestroyFence(renderer->Handler.Device, renderer->Fence, NULL);
        vkDestroyCommandPool(renderer->Handler.Device, renderer->CommandPool, NULL);
        vkDestroySurfaceKHR(renderer->Handler.Instance, renderer->Surface, NULL);
        vkDestroySwapchainKHR(renderer->Handler.Device, renderer->SwapChain, NULL);
        uint32_t i;

        for (i = 0; i < renderer->SwapChainImageCount; i++) 
        {
            vkDestroyImageView(renderer->Handler.Device, renderer->SwapChainImageViews[i], NULL);
        }
        mageVulkanHandlerCleanup(&renderer->Handler);
    }
    void mageRendererDestroy(struct mageRenderer *renderer)
    {
        mageRendererCleanup(renderer);
        MAGE_LOG_CORE_INFORM("Renderer has been destroyed\n", NULL);
    }

#endif