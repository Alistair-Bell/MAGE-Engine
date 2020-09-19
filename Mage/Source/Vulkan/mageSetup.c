#include <mageAPI.h>

#define MAGE_VULKAN_VERSION "1.2"

static const char *mageRequiredExtensions[] = 
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME
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
            MAGE_LOG_CORE_WARNING("Validation Layers %s\n", callbackData->pMessage);
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
    return VK_FALSE;
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
    VkLayerProperties *properties = MAGE_MEMORY_ARRAY_ALLOCATE(layerCount, sizeof(VkLayerProperties));
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
        MAGE_MEMORY_FREE(properties);
        MAGE_LOG_CORE_FATAL_ERROR("Required validation layers not found\n", NULL);
        return MAGE_FALSE;
    }
    MAGE_LOG_CORE_INFORM("Required validation layers found\n", NULL);
    
    MAGE_MEMORY_FREE(properties);
    return MAGE_TRUE;
}
static char **mageGetRequiredExtensions(uint32_t *count)
{
    uint32_t debugCount = 1;
    uint32_t totalCount;
    uint32_t glfwCount;
    uint32_t i;
    char *debugExtensions[] = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwCount);
#if defined (MAGE_DEBUG)
    totalCount = glfwCount + debugCount;
#else
    totalCount = glfwCount;
#endif

    char **extensions = MAGE_MEMORY_ARRAY_ALLOCATE(totalCount, sizeof(char *));

    for (i = 0; i < totalCount; i++)
    {
        if (i < glfwCount)
        {
            extensions[i] = (char *)glfwExtensions[i];
        }
#if defined (MAGE_DEBUG)
        else
        {
            extensions[i] = (char *)debugExtensions[i - glfwCount];
        }
#endif
        MAGE_LOG_CORE_INFORM("Required extensions %s (%d of %d)\n", extensions[i], i + 1, totalCount);
    }   
    *count = totalCount;
    return extensions;
}
static void magePopulateValidationLayerCallback(VkDebugUtilsMessengerCreateInfoEXT *info)
{
    memset(info, 0, sizeof(VkDebugUtilsMessengerCreateInfoEXT));
    info->sType                 = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    info->messageSeverity       = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    info->messageType           = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    info->pfnUserCallback       = mageVulkanDebugCallback;
}
static VkResult mageSetupValidationLayerCallback(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{
    magePopulateValidationLayerCallback(&renderer->DebugMessengerCreateInfo);
    VkResult result = MAGE_VULKAN_CHECK(mageCreateDebugUtilsMessengerEXT(renderer->Instance, &renderer->DebugMessengerCreateInfo, NULL, &renderer->DebugMessenger)); 
    return result;
}
static uint8_t mageIsDeviceSuitable(struct mageRenderer *renderer, VkPhysicalDevice device)
{
    struct mageIndiciesIndexes indicies;
    mageResult result = mageGetDeviceIndexes(renderer, device, &indicies);
    if (result != MAGE_RESULT_SUCCESS)
    {
        mageIndiciesIndexesDestroy(&indicies);
        return MAGE_FALSE;
    }
    VkBool32 supported;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, indicies.GraphicIndexes[indicies.GraphicIndexesCount - 1], renderer->Surface, &supported);
    
    if (!supported)
    {
        mageIndiciesIndexesDestroy(&indicies);
        return MAGE_FALSE;
    }

    mageIndiciesIndexesDestroy(&indicies);
    return MAGE_TRUE;
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




static VkResult mageCreateInstance(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{
    if (!mageCheckValidationLayers(renderer, window))
    {
        return VK_ERROR_INCOMPATIBLE_DRIVER;
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
    instanceCreateInfo.pNext                        = (VkDebugUtilsMessengerCreateInfoEXT *) &renderer->DebugMessengerCreateInfo;
#else
    instanceCreateInfo.ppEnabledLayerNames          = NULL;
    instanceCreateInfo.enabledLayerCount            = 0;
#endif

    VkResult result = MAGE_VULKAN_CHECK(vkCreateInstance(&instanceCreateInfo, NULL, &renderer->Instance));
    MAGE_MEMORY_FREE(extensions);
    return result;
}
static VkResult mageCreateSurface(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{
    VkResult result = MAGE_VULKAN_CHECK(glfwCreateWindowSurface(renderer->Instance, window->Context, NULL, &renderer->Surface));
    return result;
}
static VkResult magePickPhysicalDevice(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{
    uint32_t deviceCount, i;
    vkEnumeratePhysicalDevices(renderer->Instance, &deviceCount, NULL);
    if (deviceCount <= 0)
    {
        MAGE_LOG_CORE_FATAL_ERROR("Unable to find any vulkan physical devices\n", NULL);
        return VK_ERROR_INCOMPATIBLE_DRIVER;
    }
    VkPhysicalDevice *devices = MAGE_MEMORY_ARRAY_ALLOCATE(deviceCount, sizeof(VkPhysicalDevice));
    uint32_t *scores = MAGE_MEMORY_ARRAY_ALLOCATE(deviceCount, sizeof(uint32_t));    
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
    vkGetPhysicalDeviceMemoryProperties(renderer->PhysicalDevice, &renderer->PhysicalDeviceMemoryProperties);
    vkGetPhysicalDeviceFeatures(renderer->PhysicalDevice, &renderer->PhysicalDeviceFeatures);
    renderer->PhysicalDeviceFeatures.samplerAnisotropy = VK_TRUE;
    MAGE_MEMORY_FREE(devices);
    MAGE_MEMORY_FREE(scores);
    return VK_SUCCESS;
}
static VkResult mageCreateDevice(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{
    const float priorities[] = { 1.0f };
    VkDeviceQueueCreateInfo queueCreateInfo;
    VkDeviceCreateInfo deviceCreateInfo;

    memset(&deviceCreateInfo, 0, sizeof(VkDeviceCreateInfo));
    memset(&queueCreateInfo, 0, sizeof(VkDeviceQueueCreateInfo));
    queueCreateInfo.sType                       = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex            = renderer->Indexes.GraphicIndexes[0];
    queueCreateInfo.pQueuePriorities            = priorities;
    queueCreateInfo.queueCount                  = 1;

    deviceCreateInfo.sType                      = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.ppEnabledExtensionNames    = mageRequiredExtensions;
    deviceCreateInfo.enabledExtensionCount      = 2;    
    deviceCreateInfo.pQueueCreateInfos          = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount       = 1;
    deviceCreateInfo.pEnabledFeatures           = &renderer->PhysicalDeviceFeatures;

    VkResult result = MAGE_VULKAN_CHECK(vkCreateDevice(renderer->PhysicalDevice, &deviceCreateInfo, NULL, &renderer->Device));
    return result;
}
static VkResult mageFetchQueues(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{
    vkGetDeviceQueue(renderer->Device, renderer->Indexes.GraphicIndexes[renderer->Indexes.GraphicIndexesCount - 1], 0, &renderer->GraphicalQueue);
    vkGetDeviceQueue(renderer->Device, renderer->Indexes.PresentIndexes[renderer->Indexes.PresentIndexesCount - 1], 0, &renderer->PresentQueue);
    return VK_SUCCESS;
}
static VkResult mageCreateSwapChain(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{
    if (mageGetSwapChainSupport(&renderer->SwapChainSupportInfo, window, renderer->PhysicalDevice, renderer->Surface) != MAGE_RESULT_SUCCESS)
    {
        MAGE_LOG_CORE_FATAL_ERROR("Failed to create swap chain, hardware invalid\n", NULL);
    }
    renderer->SwapChainFormat = mageSwapChainSupportPickSurfaceFormat(&renderer->SwapChainSupportInfo).format;;
    uint32_t imageCount = renderer->SwapChainSupportInfo.Capabilities.minImageCount + 1;
    uint32_t indicies[] = { renderer->Indexes.GraphicIndexes[0], renderer->Indexes.PresentIndexes[0] };

    VkSwapchainCreateInfoKHR createInfo;
    memset(&createInfo, 0, sizeof(VkSwapchainCreateInfoKHR));
    
    createInfo.sType                        = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface                      = renderer->Surface;
    createInfo.imageFormat                  = renderer->SwapChainFormat;
    createInfo.imageColorSpace              = mageSwapChainSupportPickSurfaceFormat(&renderer->SwapChainSupportInfo).colorSpace;
    createInfo.imageExtent                  = renderer->SwapChainSupportInfo.Extent;
    createInfo.imageArrayLayers             = 1;
    createInfo.imageUsage                   = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    createInfo.minImageCount                = imageCount;
    createInfo.preTransform                 = renderer->SwapChainSupportInfo.Capabilities.currentTransform;
    createInfo.compositeAlpha               = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.clipped                      = VK_TRUE;
    createInfo.oldSwapchain                 = VK_NULL_HANDLE;

    if (renderer->Indexes.GraphicIndexes[0] != renderer->Indexes.PresentIndexes[0])
    {
        createInfo.imageSharingMode         = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount    = 2;
        createInfo.pQueueFamilyIndices      = indicies;
    }
    else
    {
        createInfo.imageSharingMode         = VK_SHARING_MODE_EXCLUSIVE;
    }
    
    VkResult result = MAGE_VULKAN_CHECK(vkCreateSwapchainKHR(renderer->Device, &createInfo, NULL, &renderer->SwapChain)); 
    VkRect2D renderArea;
    memset(&renderArea, 0, sizeof(VkRect2D));
    renderArea.offset = (VkOffset2D){ .x = 0, .y = 0};
    renderArea.extent = renderer->SwapChainSupportInfo.Extent;

    renderer->RenderArea = renderArea;
    return result;
}
static VkResult mageCreateSwapChainImages(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{
    uint32_t imageCount, i;
    vkGetSwapchainImagesKHR(renderer->Device, renderer->SwapChain, &imageCount, NULL);
    renderer->SwapChainImages = MAGE_MEMORY_ARRAY_ALLOCATE(imageCount, sizeof(VkImage));
    renderer->SwapChainImageViews = MAGE_MEMORY_ARRAY_ALLOCATE(imageCount, sizeof(VkImageView));
    vkGetSwapchainImagesKHR(renderer->Device, renderer->SwapChain, &imageCount, renderer->SwapChainImages);
    renderer->SwapChainImageCount = imageCount;

    for (i = 0; i < imageCount; i++)
    {
        VkImageViewCreateInfo createInfo;
        memset(&createInfo, 0, sizeof(VkImageViewCreateInfo));

        createInfo.sType                            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image                            = renderer->SwapChainImages[i];
        createInfo.viewType                         = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format                           = mageSwapChainSupportPickSurfaceFormat(&renderer->SwapChainSupportInfo).format;
        createInfo.components.r                     = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g                     = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b                     = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a                     = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel    = 0;
        createInfo.subresourceRange.levelCount      = 1;
        createInfo.subresourceRange.baseArrayLayer  = 0;
        createInfo.subresourceRange.layerCount      = 1;

        VkResult result = MAGE_VULKAN_CHECK(vkCreateImageView(renderer->Device, &createInfo, NULL, &renderer->SwapChainImageViews[i]));
        if (result != VK_SUCCESS) return result;
    }

    return VK_SUCCESS;
}
static VkResult mageCreateRenderPass(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{
    VkAttachmentDescription attachmentDescription;
    memset(&attachmentDescription, 0, sizeof(VkAttachmentDescription));
    attachmentDescription.format                = mageSwapChainSupportPickSurfaceFormat(&renderer->SwapChainSupportInfo).format;
    attachmentDescription.samples               = VK_SAMPLE_COUNT_1_BIT;
    attachmentDescription.loadOp                = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachmentDescription.storeOp               = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDescription.stencilLoadOp         = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDescription.stencilStoreOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDescription.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentDescription.finalLayout           = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference attachmentReference;
    memset(&attachmentReference, 0, sizeof(VkAttachmentReference));
    attachmentReference.attachment              = 0;
    attachmentReference.layout                  = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDescription;
    memset(&subpassDescription, 0, sizeof(VkSubpassDescription));
    subpassDescription.pipelineBindPoint            = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount         = 1;
    subpassDescription.pColorAttachments            = &attachmentReference;

    VkSubpassDependency dependency;
    memset(&dependency, 0, sizeof(VkSubpassDependency));

    dependency.srcSubpass       = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass       = 0;
    dependency.srcStageMask     = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask    = 0;
    dependency.dstStageMask     = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask    = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkRenderPassCreateInfo));
    createInfo.sType                = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.attachmentCount      = 1;
    createInfo.pAttachments         = &attachmentDescription;
    createInfo.subpassCount         = 1;
    createInfo.pSubpasses           = &subpassDescription;
    createInfo.dependencyCount      = 1;
    createInfo.pDependencies        = &dependency;

    return MAGE_VULKAN_CHECK(vkCreateRenderPass(renderer->Device, &createInfo, NULL, &renderer->PrimaryRenderPass));
}
static VkResult mageCreateGraphicsPipeline(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{
    if (rendererInfo->PipelineShaders == NULL || rendererInfo->ShaderCount <= 0)
    {
        struct mageShader defaultShaders[2];
        mageShaderCreate(&defaultShaders[0], "Mage/Resources/Shaders/fragment.sprv", "main", MAGE_SHADER_TYPE_FRAGMENT);
        mageShaderCreate(&defaultShaders[1], "Mage/Resources/Shaders/vertex.sprv", "main", MAGE_SHADER_TYPE_VERTEX);
        rendererInfo->PipelineShaders    = defaultShaders;
        rendererInfo->ShaderCount        = 2;
    }

    VkPipelineShaderStageCreateInfo *pipelineShaderStages = MAGE_MEMORY_ARRAY_ALLOCATE(rendererInfo->ShaderCount, sizeof(VkPipelineShaderStageCreateInfo));
    VkShaderModule *pipelineShaderModules                 = MAGE_MEMORY_ARRAY_ALLOCATE(rendererInfo->ShaderCount, sizeof(VkShaderModule));
    {
        uint32_t i;
        for (i = 0; i < rendererInfo->ShaderCount; i++)
        {
            VkShaderModule module = mageShaderCreateModule(&rendererInfo->PipelineShaders[i], renderer->Device);

            VkPipelineShaderStageCreateInfo stageCreateInfo;
            memset(&stageCreateInfo, 0, sizeof(VkPipelineShaderStageCreateInfo));
            stageCreateInfo.sType     = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            stageCreateInfo.stage     = mageShaderTypeToBit(rendererInfo->PipelineShaders[i].ShaderType);
            stageCreateInfo.module    = module;
            stageCreateInfo.pName     = rendererInfo->PipelineShaders[i].EntryPoint;
            pipelineShaderStages[i]   = stageCreateInfo;
            pipelineShaderModules[i]  = module;
            MAGE_LOG_CORE_INFORM("Creating pipeline shader %s with entry point of %s, shader %d of %d\n", rendererInfo->PipelineShaders[i].FilePath, rendererInfo->PipelineShaders[i].EntryPoint, i + 1, rendererInfo->ShaderCount);
        }
    }
    
    VkPipelineVertexInputStateCreateInfo vertexInputInfo;
    memset(&vertexInputInfo, 0, sizeof(VkPipelineVertexInputStateCreateInfo));

    uint32_t count;
    VkVertexInputBindingDescription bindingDescription = mageVertexBindingDescription();
    VkVertexInputAttributeDescription *inputDescriptions = mageVertexGetAttributeDescriptions(&count);

    vertexInputInfo.sType                               = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount       = 1;
    vertexInputInfo.pVertexBindingDescriptions          = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount     = count;
    vertexInputInfo.pVertexAttributeDescriptions        = inputDescriptions;
    
    VkPipelineInputAssemblyStateCreateInfo inputAssembly;
    memset(&inputAssembly, 0, sizeof(VkPipelineInputAssemblyStateCreateInfo));

    inputAssembly.sType                     = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology                  = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable    = VK_FALSE;

    VkViewport viewport;
    memset(&viewport, 0, sizeof(VkViewport));

    viewport.x              = 0.0f;
    viewport.y              = 0.0f;
    viewport.width          = (float) renderer->SwapChainSupportInfo.Extent.width;
    viewport.height         = (float) renderer->SwapChainSupportInfo.Extent.height;
    viewport.minDepth       = 0.0f;
    viewport.maxDepth       = 1.0f;

    VkRect2D scissor;
    memset(&scissor, 0, sizeof(VkRect2D));

    scissor.offset = (VkOffset2D){ .x = 0, .y = 0.0f };
    scissor.extent = renderer->SwapChainSupportInfo.Extent;

    VkPipelineViewportStateCreateInfo viewportState;
    memset(&viewportState, 0, sizeof(VkPipelineViewportStateCreateInfo));

    viewportState.sType             = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount     = 1;
    viewportState.pViewports        = &viewport;
    viewportState.scissorCount      = 1;
    viewportState.pScissors         = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer;
    memset(&rasterizer, 0, sizeof(VkPipelineRasterizationStateCreateInfo));

    rasterizer.sType                        = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable             = VK_FALSE;
    rasterizer.rasterizerDiscardEnable      = VK_FALSE;
    rasterizer.polygonMode                  = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth                    = 1.0f;
    rasterizer.cullMode                     = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace                    = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable              = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling;
    memset(&multisampling, 0, sizeof(VkPipelineMultisampleStateCreateInfo));

    multisampling.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable   = VK_FALSE;
    multisampling.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    memset(&colorBlendAttachment, 0, sizeof(VkPipelineColorBlendAttachmentState));

    if (rendererInfo->TextureTransparency == MAGE_TRUE)
    {
        colorBlendAttachment.blendEnable                = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor        = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor        = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp               = VK_BLEND_OP_ADD;
    }
    else
    {
        colorBlendAttachment.blendEnable                = VK_FALSE;
    }
    colorBlendAttachment.colorWriteMask                 = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    
    VkPipelineColorBlendStateCreateInfo colorBlending;
    memset(&colorBlending, 0, sizeof(VkPipelineColorBlendStateCreateInfo));

    colorBlending.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable     = VK_FALSE;
    colorBlending.logicOp           = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount   = 1;
    colorBlending.pAttachments      = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo;
    memset(&pipelineLayoutInfo, 0, sizeof(VkPipelineLayoutCreateInfo));

    pipelineLayoutInfo.sType                    = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount           = 1;
    pipelineLayoutInfo.pSetLayouts              = &renderer->DescriptorSetLayout;

    VkResult result = MAGE_VULKAN_CHECK(vkCreatePipelineLayout(renderer->Device, &pipelineLayoutInfo, NULL, &renderer->GraphicsPipelineLayout));
    
    if (result != VK_SUCCESS)
    {
        uint32_t i;
        for (i = 0; i < rendererInfo->ShaderCount; i++)
        {
            vkDestroyShaderModule(renderer->Device, pipelineShaderModules[i], NULL);
        }
        MAGE_MEMORY_FREE(pipelineShaderModules);
        MAGE_MEMORY_FREE(pipelineShaderStages);

        return result;  
    }

    VkGraphicsPipelineCreateInfo pipelineInfo;
    memset(&pipelineInfo, 0, sizeof(VkGraphicsPipelineCreateInfo));
    
    pipelineInfo.sType                  = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount             = rendererInfo->ShaderCount;
    pipelineInfo.pStages                = pipelineShaderStages;
    pipelineInfo.pVertexInputState      = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState    = &inputAssembly;
    pipelineInfo.pViewportState         = &viewportState;
    pipelineInfo.pRasterizationState    = &rasterizer;
    pipelineInfo.pMultisampleState      = &multisampling;
    pipelineInfo.pColorBlendState       = &colorBlending;
    pipelineInfo.layout                 = renderer->GraphicsPipelineLayout;
    pipelineInfo.renderPass             = renderer->PrimaryRenderPass;
    pipelineInfo.subpass                = 0;

    result = MAGE_VULKAN_CHECK(vkCreateGraphicsPipelines(renderer->Device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &renderer->GraphicsPipeline));

    uint32_t i;
    for (i = 0; i < rendererInfo->ShaderCount; i++)
    {
        vkDestroyShaderModule(renderer->Device, pipelineShaderModules[i], NULL);
    }
    MAGE_MEMORY_FREE(pipelineShaderModules);
    MAGE_MEMORY_FREE(pipelineShaderStages);
    MAGE_MEMORY_FREE(inputDescriptions);
    return result;  
}
static VkResult mageCreateDescriptors(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{
    mageDescriptorPoolCreate(renderer);
    mageDescriptorSetLayoutCreate(renderer);
    mageDescriptorSetsAllocate(renderer);
    return VK_SUCCESS;
}
static VkResult mageCreateFrameBuffers(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{   
    renderer->Framebuffers      = MAGE_MEMORY_ARRAY_ALLOCATE(renderer->SwapChainImageCount, sizeof(VkFramebuffer));

    uint32_t i;
    for (i = 0; i < renderer->SwapChainImageCount; i++)
    {
        VkFramebufferCreateInfo createInfo;
        memset(&createInfo, 0, sizeof(VkFramebufferCreateInfo));

        createInfo.sType                = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.attachmentCount      = 1;
        createInfo.pAttachments         = &renderer->SwapChainImageViews[i];
        createInfo.renderPass           = renderer->PrimaryRenderPass;
        createInfo.width                = renderer->SwapChainSupportInfo.Extent.width;
        createInfo.height               = renderer->SwapChainSupportInfo.Extent.height;
        createInfo.layers               = 1;

        MAGE_VULKAN_CHECK(vkCreateFramebuffer(renderer->Device, &createInfo, NULL, &renderer->Framebuffers[i]));
    }
    return VK_SUCCESS;
}
static VkResult mageCreateCommandPool(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{
    VkCommandPoolCreateInfo createInfo;
    memset(&createInfo, 0, sizeof(VkCommandPoolCreateInfo));
    
    createInfo.sType                = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.queueFamilyIndex     = renderer->Indexes.GraphicIndexes[renderer->Indexes.GraphicIndexesCount - 1];
    createInfo.flags                = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    
    return MAGE_VULKAN_CHECK(vkCreateCommandPool(renderer->Device, &createInfo, NULL, &renderer->CommandPool));
}
static VkResult mageCreateCommandBuffers(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{    
    uint32_t i;
    
    renderer->CommandBuffers = MAGE_MEMORY_ARRAY_ALLOCATE(renderer->SwapChainImageCount, sizeof(VkCommandBuffer));
    
    VkCommandBufferAllocateInfo allocateInfo;
    memset(&allocateInfo, 0, sizeof(VkCommandBufferAllocateInfo));
    allocateInfo.sType                      = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool                = renderer->CommandPool;
    allocateInfo.level                      = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount         = (uint32_t) renderer->SwapChainImageCount;

    memset(&renderer->ClearValue, 0, sizeof(VkClearValue));
    renderer->ClearValue.color.float32[0] = 0.15f;
    renderer->ClearValue.color.float32[1] = 0.15f;
    renderer->ClearValue.color.float32[2] = 0.15f;
    renderer->ClearValue.color.float32[3] = 0.0f;
    

    VkResult result = vkAllocateCommandBuffers(renderer->Device, &allocateInfo, renderer->CommandBuffers);
    return result;
}
static VkResult mageCreateSynchronisationObjects(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{
    uint32_t i;
    VkSemaphoreCreateInfo semaphorecreateInfo;
    memset(&semaphorecreateInfo, 0, sizeof(VkSemaphoreCreateInfo));
    semaphorecreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceCreateInfo;
    memset(&fenceCreateInfo, 0, sizeof(VkFenceCreateInfo));
    fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    renderer->SignalSemaphores      = MAGE_MEMORY_ARRAY_ALLOCATE(renderer->ConcurentFrames, sizeof(VkSemaphore));
    renderer->WaitSemaphores        = MAGE_MEMORY_ARRAY_ALLOCATE(renderer->ConcurentFrames, sizeof(VkSemaphore));
    renderer->FencesInUse           = MAGE_MEMORY_ARRAY_ALLOCATE(renderer->ConcurentFrames, sizeof(VkFence));
    renderer->SwapChainImagesInUse  = MAGE_MEMORY_ARRAY_ALLOCATE(renderer->ConcurentFrames, sizeof(VkFence));

    for (i = 0; i < renderer->ConcurentFrames; i++)
    {
        vkCreateSemaphore(renderer->Device, &semaphorecreateInfo, NULL, &renderer->SignalSemaphores[i]);
        vkCreateSemaphore(renderer->Device, &semaphorecreateInfo, NULL, &renderer->WaitSemaphores[i]);
        vkCreateFence(renderer->Device, &fenceCreateInfo, NULL, &renderer->FencesInUse[i]);
        memset(&renderer->SwapChainImagesInUse[i], 0, sizeof(VkFence));
    }
    
    return VK_SUCCESS;
}
static VkResult mageCreateDefaultBuffers(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{
    renderer->DefaultSquareIndexBuffer = MAGE_MEMORY_ALLOCATE(sizeof(struct mageBuffer));
    uint16_t indexes[] = { 0, 1, 2, 2, 3, 0 };
    mageBufferCreate(renderer->DefaultSquareIndexBuffer, MAGE_BUFFER_TYPE_INDEX, indexes, sizeof(indexes), renderer);

    MAGE_LOG_CORE_INFORM("Default buffers have been created\n", NULL);
    return VK_SUCCESS;
}

mageResult mageRendererCreate(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererInfo)
{   
    renderer->ConcurentFrames = 2;    
    renderer->CurrentFrame = 0;
    MAGE_LOG_CORE_INFORM("Creating vulkan %s backend\n", MAGE_VULKAN_VERSION);
    uint32_t i;
    typedef VkResult (*function)(struct mageRenderer *, struct mageWindow *, struct mageRendererCreateInfo *);
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
        mageCreateSwapChain,
        mageCreateSwapChainImages,
        mageCreateRenderPass,
        mageCreateDescriptors,
        mageCreateGraphicsPipeline,
        mageCreateFrameBuffers,
        mageCreateCommandPool,
        mageCreateCommandBuffers,
        mageCreateSynchronisationObjects,
        mageCreateDefaultBuffers
    };

    for (i = 0; i < sizeof(functions) / sizeof(function); i++)
    {
        VkResult result = functions[i](renderer, window, rendererInfo);
        if (result != VK_SUCCESS) return MAGE_RESULT_UNKNOWN;
    }
    MAGE_LOG_CORE_INFORM("Renderer passed in %d of %d operations, succesfully created vulkan %s backend\n", i, sizeof(functions) / sizeof(function), MAGE_VULKAN_VERSION);
    return MAGE_RESULT_SUCCESS;
}
static void mageCleanupSwapChain(struct mageRenderer *renderer, struct mageRendererCreateInfo *rendererInfo)
{
    uint32_t i;
    
    for (i = 0; i < renderer->SwapChainImageCount; i++)
    {
        vkDestroyFramebuffer(renderer->Device, renderer->Framebuffers[i], NULL);
    }
    vkFreeCommandBuffers(renderer->Device, renderer->CommandPool, renderer->SwapChainImageCount, renderer->CommandBuffers);
    vkDestroyRenderPass(renderer->Device, renderer->PrimaryRenderPass, NULL);
    vkDestroyPipeline(renderer->Device, renderer->GraphicsPipeline, NULL); 
    vkDestroyPipelineLayout(renderer->Device, renderer->GraphicsPipelineLayout, NULL);
    
    for (i = 0; i < renderer->SwapChainImageCount; i++)
    {
        vkDestroyImageView(renderer->Device, renderer->SwapChainImageViews[i], NULL);
    }

    vkDestroySwapchainKHR(renderer->Device, renderer->SwapChain, NULL);
    vkDestroyCommandPool(renderer->Device, renderer->CommandPool, NULL);

    /* Descriptor Sets */
    vkDestroyDescriptorSetLayout(renderer->Device, renderer->DescriptorSetLayout, NULL);
    vkDestroyDescriptorPool(renderer->Device, renderer->DescriptorPool, NULL);
    mageBufferDestroy(renderer->DefaultSquareIndexBuffer, renderer);

    MAGE_MEMORY_FREE(renderer->SwapChainImages);
    MAGE_MEMORY_FREE(renderer->SwapChainImageViews);
    MAGE_MEMORY_FREE(renderer->Framebuffers);
    MAGE_MEMORY_FREE(renderer->DefaultSquareIndexBuffer);
}
void mageRendererResize(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererCreateInfo *rendererProps)
{
    MAGE_LOG_CORE_INFORM("Recreating rendering swapchain, window / surface resized\n", NULL);
    vkDeviceWaitIdle(renderer->Device);
    mageCleanupSwapChain(renderer, rendererProps);
    typedef VkResult (*function)(struct mageRenderer *, struct mageWindow *, struct mageRendererCreateInfo *);
    function functions[] = 
    {
        mageCreateSwapChain,
        mageCreateSwapChainImages,
        mageCreateRenderPass,
        mageCreateDescriptors,
        mageCreateGraphicsPipeline,
        mageCreateFrameBuffers,
        mageCreateCommandPool,
        mageCreateCommandBuffers,
        mageCreateDefaultBuffers
    };
    uint32_t i;
    for (i = 0; i < sizeof(functions) / sizeof(function); i++)
    {
        functions[i](renderer, window, rendererProps);
    }
}
void mageRendererDestroy(struct mageRenderer *renderer, struct mageRendererCreateInfo *rendererInfo)
{
    uint32_t i;
    
    mageCleanupSwapChain(renderer, rendererInfo);

    for (i = 0; i < renderer->ConcurentFrames; i++)
    {
        vkDestroySemaphore(renderer->Device, renderer->SignalSemaphores[i], NULL);
        vkDestroySemaphore(renderer->Device, renderer->WaitSemaphores[i], NULL); 
        vkDestroyFence(renderer->Device, renderer->FencesInUse[i], NULL);
    }

    vkDestroySurfaceKHR(renderer->Instance, renderer->Surface, NULL);
    vkDestroyDevice(renderer->Device, NULL);


#if defined (MAGE_DEBUG)
    mageDestroyDebugUtilsMessengerEXT(renderer->Instance, renderer->DebugMessenger, NULL);
#endif

    vkDestroyInstance(renderer->Instance, NULL);
    
    mageIndiciesIndexesDestroy(&renderer->Indexes);
    mageSwapChainSupportDestroy(&renderer->SwapChainSupportInfo);

    MAGE_MEMORY_FREE(renderer->SignalSemaphores);
    MAGE_MEMORY_FREE(renderer->WaitSemaphores);
    MAGE_MEMORY_FREE(renderer->FencesInUse);

}   
