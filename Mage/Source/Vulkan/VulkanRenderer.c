#include "VulkanRenderer.h"


static const char *MageRequiredExtensions[] = 
{
    VK_KHR_SURFACE_EXTENSION_NAME,
    #if MAGE_BUILD_PLATFORM_LINUX
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
    #elif MAGE_BUILD_PLATFORM_WINDOWS
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    #endif
    
    #if MAGE_BUILD_DEBUG_MODE
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    #endif
};


typedef U8 (*MageVulkanCreateCallback)(MageRendererCreateInfo *, MageRenderer *);

static U0 MageVulkanRendererValidationLayersDestroy(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    PFN_vkDestroyDebugUtilsMessengerEXT function = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (function != NULL)
        function(instance, debugMessenger, pAllocator);
}
static VkResult MageVulkanRendererValidationLayerCreate(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    PFN_vkCreateDebugUtilsMessengerEXT function = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (function != NULL)
        return function(instance, pCreateInfo, pAllocator, pDebugMessenger);
    else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}
static U0 MageVulkanRendererFillValidationLayerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT *data)
{
    memset(data, 0, sizeof(VkDebugUtilsMessengerCreateInfoEXT));
    data->sType               = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    data->pUserData           = NULL;
    data->messageSeverity     = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    data->messageType         = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    data->pfnUserCallback     = MageVulkanValidationLayersCallback;
    
}

VKAPI_ATTR VkBool32 VKAPI_CALL MageVulkanValidationLayersCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, U0 *pUserData)
{
    if (messageSeverity != VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
        printf("Warning: Vulkan validation layer message: %s\n", pCallbackData->pMessage);
    return VK_SUCCESS;
}

U8 MageVulkanRendererInstanceCreate(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    U8 foundExtensions = MageVulkanRendererValidateExtensionsPresent(MageRequiredExtensions, sizeof(MageRequiredExtensions) / sizeof(const char *));
    MAGE_HANDLE_ERROR_MESSAGE(!foundExtensions, printf("Error: Vulkan loader: Unable to find all the required instance extensions!\n"));
    
    #if MAGE_BUILD_DEBUG_MODE
        U8 foundLayers     = MageVulkanRendererValidateLayersPresent((const char *[]) { "VK_LAYER_KHRONOS_validation" }, 1);
        MAGE_HANDLE_ERROR_MESSAGE(!foundLayers, printf("Error: Vulkan loader: Unable to find all the required instance layers!\n"));
    #endif
    
    VkApplicationInfo applicationInfo;
    memset(&applicationInfo, 0, sizeof(VkApplicationInfo));
    applicationInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.apiVersion          = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName         = "MageEngine-Vulkan";
    applicationInfo.engineVersion       = VK_MAKE_VERSION(0, 1, 0);

    VkInstanceCreateInfo instanceInfo;
    memset(&instanceInfo, 0, sizeof(VkInstanceCreateInfo));
    instanceInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo        = &applicationInfo;
    instanceInfo.enabledExtensionCount   = sizeof(MageRequiredExtensions) / sizeof(const char *);
    instanceInfo.ppEnabledExtensionNames = MageRequiredExtensions;


    #if MAGE_BUILD_DEBUG_MODE
        VkDebugUtilsMessengerCreateInfoEXT data;
        MageVulkanRendererFillValidationLayerCreateInfo(&data);
        instanceInfo.ppEnabledLayerNames = (const char *[]) { "VK_LAYER_KHRONOS_validation" };
        instanceInfo.enabledLayerCount   = 1;
        instanceInfo.pNext               = (VkDebugUtilsMessengerCreateInfoEXT *)&data;
    #endif
 
    VkResult result = vkCreateInstance(&instanceInfo, NULL, &renderer->Overseer.Instance);
    return result == VK_SUCCESS;
}
U8 MageVulkanRendererDebugLayersCreate(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo;
    MageVulkanRendererFillValidationLayerCreateInfo(&messengerCreateInfo);
    VkResult result = MageVulkanRendererValidationLayerCreate(renderer->Overseer.Instance, &messengerCreateInfo, NULL, &renderer->Overseer.DebugMessenger);

    return result == VK_SUCCESS;
}

U8 MageRendererCreate(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    MAGE_HANDLE_ERROR_MESSAGE(info == NULL || renderer == NULL, printf("Error: Any passed parameters are NULL!\n"));
    memset(renderer, 0, sizeof(MageRenderer));
    
    MageVulkanCreateCallback methods[] = 
    {
        MageVulkanRendererInstanceCreate,
        #if MAGE_BUILD_DEBUG_MODE
            MageVulkanRendererDebugLayersCreate,
        #endif
        
        MageVulkanRendererSurfaceCreate, 
        MageVulkanRendererPhysicalDeviceCreate,
        MageVulkanRendererSwapChainCreate,
        MageVulkanRendererSwapChainImagesCreate,
        MageVulkanRendererRenderPassCreate,
        MageVulkanRendererGraphicsPipelineCreate,
        MageVulkanRendererFrameBuffersCreate,
        MageVulkanRendererCommandBuffersCreate,
        MageVulkanRendererSyncronisationCreate,
        MageVulkanRendererHeapsCreate,
    };

    U64 count = sizeof(methods) / sizeof(MageVulkanCreateCallback);
    U64 i;
    
    for (i = 0; i < count; i++)
    {
        VkResult current = methods[i](info, renderer);
        MAGE_HANDLE_ERROR_MESSAGE(!current, printf("Error: Failed to create renderer, passed %lu of %lu operations\n", i, count));
    }
    
    VkClearColorValue *colorValue = &renderer->ClearValues.color;
    colorValue->float32[0]  = 0.0f;
    colorValue->float32[1]  = 0.0f;
    colorValue->float32[2]  = 0.0f;
    colorValue->float32[3]  = 1.0f;

    printf("Inform: Renderer has been created, passed %lu of %lu operations\n", i, count);
    return MageTrue;
}
U8 MageRendererHandleWindowResize(MageRendererResizeHandleInfo *info, MageRenderer *renderer)
{
    return MageTrue;
}
U8 MageRendererRecordHardCoded(MageRenderer *renderer, U32 recorderIndex)
{
    VkCommandBufferBeginInfo beginInfo;
    memset(&beginInfo, 0, sizeof(VkCommandBufferBeginInfo));
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    vkBeginCommandBuffer(renderer->CommandRecorders.Residents[recorderIndex], &beginInfo);

    VkRenderPassBeginInfo renderPassInfo;
    memset(&renderPassInfo, 0, sizeof(VkRenderPassBeginInfo));

    renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass        = renderer->Pipeline.RenderPass;
    renderPassInfo.pClearValues      = &renderer->ClearValues;
    renderPassInfo.clearValueCount   = 1;
    renderPassInfo.renderArea.offset = (VkOffset2D) { .x = 0.0f, .y = 0.0f };
    renderPassInfo.renderArea.extent = renderer->SwapChain.CurrentExtent;
    renderPassInfo.framebuffer       = renderer->FrameBuffer.Buffers[recorderIndex];

    VkSubpassDependency dependency;
    memset(&dependency, 0, sizeof(VkSubpassDependency));
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;

    VkCommandBuffer c = renderer->CommandRecorders.Residents[recorderIndex];
    vkCmdBeginRenderPass(c, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(c, VK_PIPELINE_BIND_POINT_GRAPHICS, renderer->Pipeline.GraphicsPipeline);
        vkCmdDraw(c, 3, 1, 0, 0);   
    vkCmdEndRenderPass(c);

    vkEndCommandBuffer(c);
    return MageTrue;
}
U8 MageRendererPresentRecorded(MageRenderer *renderer)
{
    U32 imageIndex;
    VkSemaphore *imageAvailable = &renderer->Syncronisation.AvailableSemaphores[renderer->ActiveIndex];
    
    vkWaitForFences(renderer->Device.LogicalDevice, 1, &renderer->Syncronisation.ActiveFences[renderer->ActiveIndex], VK_TRUE, UINT64_MAX);
    vkAcquireNextImageKHR(renderer->Device.LogicalDevice, renderer->SwapChain.PrimarySwapchain, UINT64_MAX, (*imageAvailable), VK_NULL_HANDLE, &imageIndex); 

    VkFence *imageFence = &renderer->Syncronisation.ImageFences[imageIndex];

    /* Check if previous frame is using this image */
    if ((*imageFence) != VK_NULL_HANDLE)
        vkWaitForFences(renderer->Device.LogicalDevice, 1, imageFence, VK_TRUE, UINT64_MAX);

    /* Mark as active */
    *imageFence = renderer->Syncronisation.ActiveFences[renderer->ActiveIndex];

    VkSemaphore *signal = &renderer->Syncronisation.FinishedSemaphores[renderer->ActiveIndex];

    VkSubmitInfo submitInfo;
    memset(&submitInfo, 0, sizeof(VkSubmitInfo));
    submitInfo.sType                   = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pWaitDstStageMask       = (VkPipelineStageFlags[]) { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.pWaitSemaphores         = imageAvailable;
    submitInfo.waitSemaphoreCount      = 1;
    submitInfo.pCommandBuffers         = &renderer->CommandRecorders.Residents[imageIndex];
    submitInfo.commandBufferCount      = 1;
    submitInfo.pSignalSemaphores       = signal;
    submitInfo.signalSemaphoreCount    = 1;

    vkResetFences(renderer->Device.LogicalDevice, 1, &renderer->Syncronisation.ActiveFences[renderer->ActiveIndex]);
    VkResult r = vkQueueSubmit(renderer->QueueHandles.GraphicsQueue, 1, &submitInfo, (*imageFence)); 
    MAGE_HANDLE_ERROR_MESSAGE(r != VK_SUCCESS, printf("Error: Unable to submit queue for rendering\n"));
    
    VkPresentInfoKHR presentInfo;
    memset(&presentInfo, 0, sizeof(VkPresentInfoKHR));
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pSwapchains        = &renderer->SwapChain.PrimarySwapchain;
    presentInfo.swapchainCount     = 1;
    presentInfo.pWaitSemaphores    = signal;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pImageIndices      = &imageIndex;

    vkQueuePresentKHR(renderer->QueueHandles.PresentQueue, &presentInfo);
    renderer->ActiveIndex = (renderer->ActiveIndex + 1) % renderer->Syncronisation.ThreadCount;
    return MageTrue;
}
U8 MageRendererDestroy(MageRenderer *renderer)
{
    vkDeviceWaitIdle(renderer->Device.LogicalDevice); /* Wait for any operations to halt before destroying shit */

    #if MAGE_BUILD_DEBUG_MODE
        MageVulkanRendererValidationLayersDestroy(renderer->Overseer.Instance, renderer->Overseer.DebugMessenger, NULL);
    #endif
    vkDestroySwapchainKHR(renderer->Device.LogicalDevice, renderer->SwapChain.PrimarySwapchain, NULL);

    U32 i;
    for (i = 0; i < renderer->SwapChain.ImagesCount; i++)
    {
        vkDestroyImageView(renderer->Device.LogicalDevice, renderer->SwapChain.ImageViews[i], NULL);
        vkDestroyFramebuffer(renderer->Device.LogicalDevice, renderer->FrameBuffer.Buffers[i], NULL);
    }
    for (i = 0; i < renderer->Syncronisation.ThreadCount; i++)
    {
        vkDestroySemaphore(renderer->Device.LogicalDevice, renderer->Syncronisation.AvailableSemaphores[i], NULL);
        vkDestroySemaphore(renderer->Device.LogicalDevice, renderer->Syncronisation.FinishedSemaphores[i], NULL);
        vkDestroyFence(renderer->Device.LogicalDevice, renderer->Syncronisation.ActiveFences[i], NULL);
    }

    free(renderer->Syncronisation.AvailableSemaphores);
    free(renderer->Syncronisation.FinishedSemaphores);
    free(renderer->Syncronisation.ActiveFences);
    free(renderer->SwapChain.Images);
    free(renderer->SwapChain.ImageViews);
    free(renderer->FrameBuffer.Buffers);
    free(renderer->CommandRecorders.Residents);

    MageVulkanRendererHeapsDestroy(renderer); 
    free(renderer->Heaps);

    vkDestroyCommandPool(renderer->Device.LogicalDevice, renderer->CommandRecorders.Pool, NULL);
    vkDestroyPipeline(renderer->Device.LogicalDevice, renderer->Pipeline.GraphicsPipeline, NULL);
    vkDestroyPipelineLayout(renderer->Device.LogicalDevice, renderer->Pipeline.GraphicsPipelineLayout, NULL);
    vkDestroyRenderPass(renderer->Device.LogicalDevice, renderer->Pipeline.RenderPass, NULL);
    vkDestroySurfaceKHR(renderer->Overseer.Instance, renderer->Surface.Surface, NULL);
    vkDestroyDevice(renderer->Device.LogicalDevice, NULL);
    vkDestroyInstance(renderer->Overseer.Instance, NULL);
    return MageTrue;
}
