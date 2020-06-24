#include <mageAPI.h>

#if defined (MAGE_VULKAN)
    
    static mageResult mageCreateFence(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
    {

        renderer->Fences = calloc(renderer->MaxImagesInFlight, sizeof(VkFence));
        VkFenceCreateInfo fenceCreateInfo;
        memset(&fenceCreateInfo, 0, sizeof(VkFenceCreateInfo));
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        uint32_t i;
        for (i = 0; i < renderer->MaxImagesInFlight; i++)
        {
            vkCreateFence(renderer->Handler.Device, &fenceCreateInfo, NULL, &renderer->Fences[i]);
        }
        
        MAGE_LOG_CORE_INFORM("Fence creation was succesfull\n", NULL);
        return MAGE_SUCCESS;
    }
    static mageResult mageCreateSemaphore(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
    {
        renderer->AvailableSemaphores = calloc(renderer->MaxImagesInFlight, sizeof(VkSemaphore));
        renderer->RenderFinishedSemaphores = calloc(renderer->MaxImagesInFlight, sizeof(VkSemaphore));

        VkSemaphoreCreateInfo semaphoreCreateInfo;
        memset(&semaphoreCreateInfo, 0, sizeof(VkSemaphoreCreateInfo));
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        
        uint32_t i;
        for (i = 0; i < renderer->MaxImagesInFlight; i++)
        {
            if (vkCreateSemaphore(renderer->Handler.Device, &semaphoreCreateInfo, NULL, &renderer->AvailableSemaphores[i]) != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Semaphore creation has failed\n", NULL);
                return MAGE_SEMAPHORE_CREATION_FAILURE;
            }
            if (vkCreateSemaphore(renderer->Handler.Device, &semaphoreCreateInfo, NULL, &renderer->RenderFinishedSemaphores[i]) != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Semaphore creation has failed\n", NULL);
                return MAGE_SEMAPHORE_CREATION_FAILURE;
            }
        }

        MAGE_LOG_CORE_INFORM("Semaphore creation was succesfull\n", NULL);

        return MAGE_SUCCESS;
    }
    static mageResult mageCreateCommandPool(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
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
        return MAGE_SUCCESS;
    }
    static mageResult mageCreateCommandBuffers(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
    {
        renderer->CommandBuffers = calloc(renderer->SwapChainImageCount, sizeof(VkCommandBuffer));
        VkCommandBufferAllocateInfo allocateInfo;
        memset(&allocateInfo, 0, sizeof(VkCommandBufferAllocateInfo));
        allocateInfo.sType                  = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.commandPool            = renderer->CommandPool;
        allocateInfo.level                  = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount     = (uint32_t) renderer->SwapChainImageCount;

        if (vkAllocateCommandBuffers(renderer->Handler.Device, &allocateInfo, renderer->CommandBuffers) != VK_SUCCESS)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Command buffer allocation has failed\n", NULL);
            MAGE_ALLOCATE_COMMAND_FAILURE;
        }
        uint32_t i;

        for (i = 0; i < renderer->SwapChainImageCount; i++)
        {
            VkCommandBufferBeginInfo beginInfo;
            memset(&beginInfo, 0, sizeof(VkCommandBufferBeginInfo));
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            if (vkBeginCommandBuffer(renderer->CommandBuffers[i], &beginInfo) != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Command buffer %d has failed to begin\n", NULL);
                return MAGE_UNKNOWN;
            }   
           
           
            VkExtent2D extent;
            extent.height = window->Height;
            extent.width  = window->Width;
            VkOffset2D offset;
            offset.x = 0;
            offset.y = 0;

            VkClearValue clearValues[2];
            memset(&clearValues, 0, sizeof(VkClearValue) * 2);
            clearValues[0].color.float32[0] = 0.0f;
            clearValues[0].color.float32[1] = 0.0f;
            clearValues[0].color.float32[2] = 0.0f;
            clearValues[0].color.float32[3] = 1.0f;

            clearValues[1].color.float32[0] = 0.0f;
            clearValues[1].color.float32[1] = 0.0f;
            clearValues[1].color.float32[2] = 0.0f;
            clearValues[1].color.float32[3] = 1.0f;
    
            VkRenderPassBeginInfo renderPassInfo;
            memset(&renderPassInfo, 0, sizeof(VkRenderPassBeginInfo));
            renderPassInfo.sType                = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass           = renderer->RenderPass;
            renderPassInfo.framebuffer          = renderer->FrameBuffers[i];
            renderPassInfo.renderArea.offset    = offset;
            renderPassInfo.renderArea.extent    = extent;
            renderPassInfo.pClearValues         = clearValues;
            renderPassInfo.clearValueCount      = 2;

            vkCmdBeginRenderPass(renderer->CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(renderer->CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->GraphicsPipeline);
            MAGE_LOG_CORE_INFORM("Command buffer %d bound to pipeline\n", i);
            vkCmdDraw(renderer->CommandBuffers[i], 3, 1, 0, 0);
            vkCmdEndRenderPass(renderer->CommandBuffers[i]);
            vkEndCommandBuffer(renderer->CommandBuffers[i]);

        }
        MAGE_LOG_CORE_INFORM("Command buffers created\n", NULL);
        return MAGE_SUCCESS;
    }
    static mageResult mageCreateSurface(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
    {
        #if defined (MAGE_GLFW)
            
            if (glfwCreateWindowSurface(renderer->Handler.Instance, window->Context, NULL, &renderer->Surface))
            {
                MAGE_LOG_CORE_FATAL_ERROR("Vulkan surface has failed to be created (glfw mode)\n", NULL);
                return MAGE_SURFACE_CREATION_FAILURE;
            }
            MAGE_LOG_CORE_INFORM("Vulkan surface has been created (glfw mode)\n", NULL);
            
        #elif defined(MAGE_SDL)  
            /* SDL_Vulkan_CreateSurface */
        
        #endif

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
            MAGE_LOG_CORE_FATAL_ERROR("Format count is too low! %d found\n", formatCount);
            return MAGE_HARDWARE_NOT_PRESENT;
        }

        VkSurfaceFormatKHR *formats = calloc(formatCount, sizeof(VkSurfaceFormatKHR));
        vkGetPhysicalDeviceSurfaceFormatsKHR(renderer->Handler.PhysicalDevice, renderer->Surface, &formatCount, formats);

        if (formats[0].format == VK_FORMAT_UNDEFINED)
        {
            renderer->SurfaceFormat.format       = VK_FORMAT_B8G8R8A8_UNORM;
            renderer->SurfaceFormat.colorSpace   = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        }
        else
        {
            renderer->SurfaceFormat = formats[0];
        }

        MAGE_LOG_CORE_INFORM("Physical device formats found\n", NULL);


        free(formats);

       return MAGE_SUCCESS;
    }
    static mageResult mageCreateExtent2D(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
    {
        renderer->Extent2D.height = window->Height;
        renderer->Extent2D.width  = window->Width;
        return MAGE_SUCCESS;
    }
    static mageResult mageCreateRenderArea(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
    {
        memset(&renderer->RenderArea, 0, sizeof(VkRect2D));
        renderer->RenderArea.offset.x = 0;
        renderer->RenderArea.offset.y = 0;
        renderer->RenderArea.extent   = renderer->Extent2D;
        return MAGE_SUCCESS;
    }
    static mageResult mageCreateSwapChain(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
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


            createInfo.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.sType					= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface					= renderer->Surface;
            createInfo.minImageCount			= count;
            createInfo.imageFormat				= renderer->SurfaceFormat.format;
            createInfo.imageColorSpace			= renderer->SurfaceFormat.colorSpace;
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
    static mageResult mageCreateSwapChainImages(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
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
            createInfo.format                               = renderer->SurfaceFormat.format;
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
    static mageResult mageCreateDepthStencilImage(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
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
            MAGE_LOG_CORE_FATAL_ERROR("Deph stencil image has failed to been created\n", NULL);
            return MAGE_IMAGE_CREATION_FAILURE;
        }
        VkMemoryRequirements requirements;
        memset(&requirements, 0, sizeof(VkMemoryRequirements));
        vkGetImageMemoryRequirements(renderer->Handler.Device, renderer->DepthStencilImage, &requirements);
        
        uint32_t memoryIndex = mageFindMemoryTypeIndex(&renderer->Handler.PhysicalMemoryProperties, &requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);


        VkMemoryAllocateInfo memoryAllocateInfo;
        memset(&memoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));
        memoryAllocateInfo.sType            = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.pNext            = NULL;
        memoryAllocateInfo.allocationSize   = requirements.size;
        memoryAllocateInfo.memoryTypeIndex  = memoryIndex;

        vkAllocateMemory(renderer->Handler.Device, &memoryAllocateInfo, NULL, &renderer->DepthStencilImageMemory);
        vkBindImageMemory(renderer->Handler.Device, renderer->DepthStencilImage, renderer->DepthStencilImageMemory, 0);

    

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
            MAGE_LOG_CORE_FATAL_ERROR("Deph stencil image view has failed to be created\n", NULL);
            return MAGE_IMAGE_VIEW_CREATION_FAILURE;
        }

        MAGE_LOG_CORE_INFORM("Deph stencil image created\n", NULL);
        return MAGE_SUCCESS;
    }
    static mageResult mageCreateRenderPass(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
    {
        VkAttachmentDescription attachmentDescription[2];
        memset(&attachmentDescription, 0, sizeof(VkAttachmentDescription) * 2);
        attachmentDescription[0].flags						= 0;
	    attachmentDescription[0].format						= renderer->DepthStencilFormat;
	    attachmentDescription[0].samples					= VK_SAMPLE_COUNT_1_BIT;
	    attachmentDescription[0].loadOp						= VK_ATTACHMENT_LOAD_OP_CLEAR;
	    attachmentDescription[0].storeOp					= VK_ATTACHMENT_STORE_OP_DONT_CARE;
	    attachmentDescription[0].stencilLoadOp				= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	    attachmentDescription[0].stencilStoreOp				= VK_ATTACHMENT_STORE_OP_STORE;
	    attachmentDescription[0].initialLayout				= VK_IMAGE_LAYOUT_UNDEFINED;
	    attachmentDescription[0].finalLayout                = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	    
        attachmentDescription[1].flags						= 0;
	    attachmentDescription[1].format						= renderer->SurfaceFormat.format;
	    attachmentDescription[1].samples                    = VK_SAMPLE_COUNT_1_BIT;
	    attachmentDescription[1].loadOp						= VK_ATTACHMENT_LOAD_OP_CLEAR;
	    attachmentDescription[1].storeOp					= VK_ATTACHMENT_STORE_OP_STORE;
	    attachmentDescription[1].initialLayout				= VK_IMAGE_LAYOUT_UNDEFINED;
	    attachmentDescription[1].finalLayout                = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        
        VkAttachmentReference depthStencilAttachment;
        memset(&depthStencilAttachment, 0, sizeof(VkAttachmentReference));
        depthStencilAttachment.attachment                   = 0;
        depthStencilAttachment.layout                       = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference attachmentReference[1];
        memset(attachmentReference, 0, sizeof(VkAttachmentReference) * 1);
        attachmentReference[0].attachment                   = 1;
        attachmentReference[0].layout                       = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpassDescriptions[1];
        memset(subpassDescriptions, 0, sizeof(VkSubpassDescription) * 1);
        
        subpassDescriptions[0].pipelineBindPoint            = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpassDescriptions[0].pColorAttachments            = attachmentReference;
        subpassDescriptions[0].colorAttachmentCount         = 1;
        subpassDescriptions[0].pDepthStencilAttachment      = &depthStencilAttachment;
    
        VkRenderPassCreateInfo createInfo; 
        memset(&createInfo, 0, sizeof(VkRenderPassCreateInfo));
        createInfo.sType            = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        createInfo.attachmentCount  = sizeof(attachmentDescription) / sizeof(VkAttachmentDescription);
        createInfo.pAttachments     = attachmentDescription; 
        createInfo.subpassCount     = sizeof(subpassDescriptions) / sizeof(VkSubpassDescription);
        createInfo.pSubpasses       = subpassDescriptions; 
        
        if (vkCreateRenderPass(renderer->Handler.Device, &createInfo, NULL, &renderer->RenderPass) != VK_SUCCESS)
        {
            return MAGE_RENDER_PASS_CREATION_FAILURE;
            MAGE_LOG_CORE_FATAL_ERROR("Render pass had failed to be created\n", NULL);
        }
        
        MAGE_LOG_CORE_INFORM("Render pass has been succesfully created\n", NULL);
        return MAGE_SUCCESS;
    }
    static mageResult mageCreateFrameBuffers(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
    {
        renderer->FrameBuffers = calloc(renderer->SwapChainImageCount, sizeof(VkFramebuffer));
        uint32_t i;

        for (i = 0; i < renderer->SwapChainImageCount; i++)
        {
            VkImageView views[2];
            memset(views, 0, sizeof(VkImageView) * 2);
            views[0] = renderer->DepthStencilImageView;
            views[1] = renderer->SwapChainImageViews[i];


            VkFramebufferCreateInfo createInfo;
            memset(&createInfo, 0, sizeof(VkFramebufferCreateInfo));
            createInfo.sType            = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            createInfo.renderPass       = renderer->RenderPass;
            createInfo.attachmentCount  = 2;
            createInfo.pAttachments     = views;
            createInfo.width            = window->Width;
            createInfo.height           = window->Height;
            createInfo.layers           = 1;


            if (vkCreateFramebuffer(renderer->Handler.Device, &createInfo, NULL, &renderer->FrameBuffers[i]) != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Frame buffer %d has failed to be created\n", NULL);
                return MAGE_FRAME_BUFFER_CREATION_FAILED;
            }   
        }

        MAGE_LOG_CORE_INFORM("Frame buffers has been succesfully created\n", NULL);
        return MAGE_SUCCESS;
    }
    static mageResult mageCreateRenderPipeline(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
    {      
        VkPipelineShaderStageCreateInfo *pipelineShaderStages = calloc(props->ShaderCount, sizeof(VkPipelineShaderStageCreateInfo));
        VkShaderModule *pipelineShaderModules                 = calloc(props->ShaderCount, sizeof(VkShaderModule));
        {
            uint32_t i;
            for (i = 0; i < props->ShaderCount; i++)
            {
                VkShaderModule module = mageShaderCreateModule(renderer->Handler.Device, props->RuntimeShaders[i].FilePath);

                VkPipelineShaderStageCreateInfo stageCreateInfo;
                memset(&stageCreateInfo, 0, sizeof(VkPipelineShaderStageCreateInfo));
                stageCreateInfo.sType     = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                stageCreateInfo.stage     = mageShaderTypeToBit(props->RuntimeShaders[i].ShaderType);
                stageCreateInfo.module    = module;
                stageCreateInfo.pName     = props->RuntimeShaders[i].RuntimeFunctionName;
                pipelineShaderStages[i]   = stageCreateInfo;
                pipelineShaderModules[i]  = module;
                MAGE_LOG_CORE_INFORM("Creating shader %s with entry point of %s, shader %d of %d\n", props->RuntimeShaders[i].FilePath, props->RuntimeShaders[i].RuntimeFunctionName, i + 1, props->ShaderCount);
            }
        }

        
        /* Pipeline verexes */
        VkPipelineVertexInputStateCreateInfo pipelineVertexStateCreateInfo;
        memset(&pipelineVertexStateCreateInfo, 0, sizeof(VkPipelineVertexInputStateCreateInfo));
        pipelineVertexStateCreateInfo.sType                             = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        pipelineVertexStateCreateInfo.vertexBindingDescriptionCount     = 0;
        pipelineVertexStateCreateInfo.pVertexBindingDescriptions        = NULL; 
        pipelineVertexStateCreateInfo.vertexAttributeDescriptionCount   = 0;
        pipelineVertexStateCreateInfo.pVertexAttributeDescriptions      = NULL; 

        /* Pipeline assembly */
        VkPipelineInputAssemblyStateCreateInfo pipelineAssemblyCreateInfo;
        memset(&pipelineAssemblyCreateInfo, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));
        pipelineAssemblyCreateInfo.sType                    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        pipelineAssemblyCreateInfo.topology                 = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pipelineAssemblyCreateInfo.primitiveRestartEnable   = VK_FALSE;
        
        /* Creating scissors extent */
        VkExtent2D extent;
        memset(&extent, 0, sizeof(VkExtent2D));
        extent.height    = (uint32_t)window->Height;
        extent.width     = (uint32_t)window->Width;
        
        /* Creating scissors */
        VkRect2D pipelineScissors;
        memset(&pipelineScissors, 0, sizeof(VkRect2D));
        pipelineScissors.offset.x = 0.0;
        pipelineScissors.offset.y = 0.0;
        pipelineScissors.extent   = extent;

        /* Linking viewport(s) to pipelines */
        VkPipelineViewportStateCreateInfo pipelineViewportCreateInfo;
        memset(&pipelineViewportCreateInfo, 0, sizeof(VkPipelineViewportStateCreateInfo));
        pipelineViewportCreateInfo.sType            = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        pipelineViewportCreateInfo.viewportCount    = 1;
        pipelineViewportCreateInfo.pViewports       = &renderer->Viewport;
        pipelineViewportCreateInfo.pScissors        = &pipelineScissors;
        pipelineViewportCreateInfo.scissorCount     = 1;

        /* Rasterisation for the pipeline */
        VkPipelineRasterizationStateCreateInfo pipelineRasterizationCreateInfo;
        memset(&pipelineRasterizationCreateInfo, 0, sizeof(VkPipelineRasterizationStateCreateInfo));
        pipelineRasterizationCreateInfo.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        pipelineRasterizationCreateInfo.depthBiasEnable         = VK_FALSE;
        pipelineRasterizationCreateInfo.rasterizerDiscardEnable = VK_TRUE;
        pipelineRasterizationCreateInfo.polygonMode             = VK_POLYGON_MODE_FILL;
        pipelineRasterizationCreateInfo.lineWidth               = 1.0f;
        pipelineRasterizationCreateInfo.cullMode                = VK_CULL_MODE_BACK_BIT;
        pipelineRasterizationCreateInfo.frontFace               = VK_FRONT_FACE_CLOCKWISE;
        pipelineRasterizationCreateInfo.depthBiasEnable         = VK_FALSE;
        pipelineRasterizationCreateInfo.depthBiasConstantFactor = 0.0f;
        pipelineRasterizationCreateInfo.depthBiasClamp          = 0.0f;
        pipelineRasterizationCreateInfo.depthBiasSlopeFactor    = 0.0f;

        /* Multisampling for the pipeline */
        VkPipelineMultisampleStateCreateInfo pipelineMultisamplingCreateInfo;
        memset(&pipelineMultisamplingCreateInfo, 0, sizeof(VkPipelineMultisampleStateCreateInfo));
        pipelineMultisamplingCreateInfo.sType                     = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        pipelineMultisamplingCreateInfo.sampleShadingEnable       = VK_FALSE;
        pipelineMultisamplingCreateInfo.rasterizationSamples      = VK_SAMPLE_COUNT_1_BIT;
        pipelineMultisamplingCreateInfo.minSampleShading          = 1.0f; 
        pipelineMultisamplingCreateInfo.pSampleMask               = NULL; 
        pipelineMultisamplingCreateInfo.alphaToCoverageEnable     = VK_FALSE; 
        pipelineMultisamplingCreateInfo.alphaToOneEnable          = VK_FALSE; 

        /* Color blending attachment for the pipeline */
        VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentCreateInfo;
        memset(&pipelineColorBlendAttachmentCreateInfo, 0, sizeof(VkPipelineColorBlendAttachmentState));
        pipelineColorBlendAttachmentCreateInfo.colorWriteMask         = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        pipelineColorBlendAttachmentCreateInfo.blendEnable            = VK_TRUE;
        pipelineColorBlendAttachmentCreateInfo.srcColorBlendFactor    = VK_BLEND_FACTOR_SRC_ALPHA;
        pipelineColorBlendAttachmentCreateInfo.dstColorBlendFactor    = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        pipelineColorBlendAttachmentCreateInfo.colorBlendOp           = VK_BLEND_OP_ADD;
        pipelineColorBlendAttachmentCreateInfo.srcAlphaBlendFactor    = VK_BLEND_FACTOR_ONE;
        pipelineColorBlendAttachmentCreateInfo.dstAlphaBlendFactor    = VK_BLEND_FACTOR_ZERO;
        pipelineColorBlendAttachmentCreateInfo.alphaBlendOp           = VK_BLEND_OP_ADD;
        
        /* Color blending for the pipeline */
        VkPipelineColorBlendStateCreateInfo pipelineColorBlendCreateInfo;
        memset(&pipelineColorBlendCreateInfo, 0, sizeof(VkPipelineColorBlendStateCreateInfo));
        pipelineColorBlendCreateInfo.sType              = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        pipelineColorBlendCreateInfo.logicOpEnable      = VK_FALSE;
        pipelineColorBlendCreateInfo.logicOp            = VK_TRUE;
        pipelineColorBlendCreateInfo.attachmentCount    = 1;
        pipelineColorBlendCreateInfo.pAttachments       = &pipelineColorBlendAttachmentCreateInfo;
        pipelineColorBlendCreateInfo.blendConstants[0]  = 0.0f;
        pipelineColorBlendCreateInfo.blendConstants[1]  = 0.0f;
        pipelineColorBlendCreateInfo.blendConstants[2]  = 0.0f;
        pipelineColorBlendCreateInfo.blendConstants[3]  = 0.0f;

        /* Dynamic states used by the pipeline */
        VkDynamicState pipelineDynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };
        
        VkPipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo;
        memset(&pipelineDynamicStateCreateInfo, 0, sizeof(VkPipelineDynamicStateCreateInfo));
        pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        pipelineDynamicStateCreateInfo.dynamicStateCount = 2;
        pipelineDynamicStateCreateInfo.pDynamicStates = pipelineDynamicStates;


        /* Creating pipeline layout information */
        VkPipelineLayoutCreateInfo layoutCreateInfo;
        memset(&layoutCreateInfo, 0, sizeof(VkPipelineLayoutCreateInfo));
        layoutCreateInfo.sType                    = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        layoutCreateInfo.setLayoutCount           = 0; 
        layoutCreateInfo.pSetLayouts              = NULL; 
        layoutCreateInfo.pushConstantRangeCount   = 0; 
        layoutCreateInfo.pPushConstantRanges      = NULL; 
        
        /* Creating the layout */
        if (vkCreatePipelineLayout(renderer->Handler.Device, &layoutCreateInfo, NULL, &renderer->PipeLineLayout) != VK_SUCCESS)
        {
            MAGE_LOG_CORE_FATAL_ERROR("Renderer pipeline has failed to be created\n", NULL);
            return MAGE_PIPELINE_CREATION_FAILURE;
        }
        MAGE_LOG_CORE_INFORM("Render pipeline layout created succesfully\n", NULL);
        VkGraphicsPipelineCreateInfo pipelineCreateInfo;
        memset(&pipelineCreateInfo, 0, sizeof(VkGraphicsPipelineCreateInfo));
        pipelineCreateInfo.sType                    = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineCreateInfo.stageCount               = props->ShaderCount;
        pipelineCreateInfo.pStages                  = pipelineShaderStages;
        pipelineCreateInfo.pVertexInputState        = &pipelineVertexStateCreateInfo;
        pipelineCreateInfo.pInputAssemblyState      = &pipelineAssemblyCreateInfo;
        pipelineCreateInfo.pViewportState           = &pipelineViewportCreateInfo;
        pipelineCreateInfo.pRasterizationState      = &pipelineRasterizationCreateInfo;
        pipelineCreateInfo.pMultisampleState        = &pipelineMultisamplingCreateInfo;
        pipelineCreateInfo.pColorBlendState         = &pipelineColorBlendCreateInfo;
        pipelineCreateInfo.layout                   = renderer->PipeLineLayout;
        pipelineCreateInfo.renderPass               = renderer->RenderPass;
        pipelineCreateInfo.subpass                  = 0;
        pipelineCreateInfo.basePipelineHandle       = VK_NULL_HANDLE;
        
        if (vkCreateGraphicsPipelines(renderer->Handler.Device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, NULL, &renderer->GraphicsPipeline) != VK_SUCCESS) 
        {
            MAGE_LOG_CORE_FATAL_ERROR("Graphics pipeline has failed to be created\n", NULL);
            return MAGE_GRAPHICS_PIPELINE_CREATION_FAILURE;
        }
        uint32_t i;
        for (i = 0; i < props->ShaderCount; i++)
        {
            vkDestroyShaderModule(renderer->Handler.Device, pipelineShaderModules[i], NULL);
        }
        free(pipelineShaderModules);
        free(pipelineShaderStages);
        MAGE_LOG_CORE_INFORM("Graphics pipeline was created succesfully\n", NULL);
        return MAGE_SUCCESS;
    }
    mageResult mageRendererInitialise(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
    {
        renderer->MaxImagesInFlight = 2;
        renderer->CurrentFrame = 0;
        mageResult result = mageVulkanHandlerInitialise(&renderer->Handler, window);

        if (result != MAGE_SUCCESS) 
        {
            return result;
        }
        
        typedef mageResult (*rendererSetupFunctions)(struct mageRenderer *, struct mageWindow *, struct mageRendererProps *);

        const rendererSetupFunctions functions[] = 
        {
            mageCreateFence,
            mageCreateSurface,
            mageCreateExtent2D,
            mageCreateRenderArea,
            mageCreateSwapChain,
            mageCreateSwapChainImages,
            mageCreateDepthStencilImage,
            mageCreateRenderPass,
            mageCreateRenderPipeline,
            mageCreateFrameBuffers, 
            mageCreateSemaphore,
            mageCreateCommandPool,
            mageCreateCommandBuffers,
        };
        const uint32_t functionCount = (sizeof(functions) / sizeof(rendererSetupFunctions));
        uint32_t i;

        for (i = 0; i < functionCount; i++)
        {
            result = functions[i](renderer, window, props);
            if (result != MAGE_SUCCESS) return result;
        }
        MAGE_LOG_CORE_INFORM("Vulkan renderer has been setup succesfully\n", NULL);
        return MAGE_SUCCESS;
    }
    void mageRendererCleanup(struct mageRenderer *renderer)
    {

        vkDestroySwapchainKHR(renderer->Handler.Device, renderer->SwapChain, NULL);
        vkDestroySurfaceKHR(renderer->Handler.Instance, renderer->Surface, NULL);
        vkDestroyRenderPass(renderer->Handler.Device, renderer->RenderPass, NULL);
        vkDestroyPipeline(renderer->Handler.Device, renderer->GraphicsPipeline, NULL);
        vkDestroyPipelineLayout(renderer->Handler.Device, renderer->PipeLineLayout, NULL);

        uint32_t i;

        for (i = 0; i < renderer->SwapChainImageCount; i++) 
        {
            vkDestroyImageView(renderer->Handler.Device, renderer->SwapChainImageViews[i], NULL);
        }
        /* Depth stencil */
        vkDestroyImageView(renderer->Handler.Device, renderer->DepthStencilImageView, NULL);
        vkFreeMemory(renderer->Handler.Device, renderer->DepthStencilImageMemory, NULL);
        vkDestroyImage(renderer->Handler.Device, renderer->DepthStencilImage, NULL);

        /* Frame buffers */
        for (i = 0; i < renderer->SwapChainImageCount; i++) 
        {
            vkDestroyFramebuffer(renderer->Handler.Device, renderer->FrameBuffers[i], NULL); 
        }
        for (i = 0; i < renderer->MaxImagesInFlight; i++)
        {
            vkDestroySemaphore(renderer->Handler.Device, renderer->AvailableSemaphores[i], NULL);
            vkDestroySemaphore(renderer->Handler.Device, renderer->RenderFinishedSemaphores[i], NULL);
            vkDestroyFence(renderer->Handler.Device, renderer->Fences[i], NULL);
        }
        vkDestroyCommandPool(renderer->Handler.Device, renderer->CommandPool, NULL);
        
        free(renderer->AvailableSemaphores);
        free(renderer->RenderFinishedSemaphores);
        free(renderer->FrameBuffers);
        free(renderer->Fences);
        
        mageVulkanHandlerCleanup(&renderer->Handler);
    }
    void mageRendererDestroy(struct mageRenderer *renderer)
    {
        mageRendererCleanup(renderer);
        MAGE_LOG_CORE_INFORM("Renderer has been destroyed\n", NULL);
    }

#endif
