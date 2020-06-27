#include <mageAPI.h>

#if defined (MAGE_VULKAN)

static mageResult mageCreateSurface(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{
    return MAGE_SUCCESS;
}
static mageResult mageCreateSwapChain(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{
    if (glfwCreateWindowSurface(renderer->Handler.Instance, window->Context, NULL, &renderer->Surface) != VK_SUCCESS)
    {
        MAGE_LOG_CORE_FATAL_ERROR("Unable to create surface\n", NULL);
        return MAGE_SURFACE_CREATION_FAILURE;
    }

    VkBool32 supported;
    /* vkGetPhysicalDeviceSurfaceSupportKHR(renderer->Handler.PhysicalDevice, ) */
 
    MAGE_LOG_CORE_INFORM("Surface creation was succesfull\n", NULL);
    return MAGE_SUCCESS;
}
static mageResult mageCreateImageViews(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{

    return MAGE_SUCCESS;
}
static mageResult mageCreateRenderPass(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{

    return MAGE_SUCCESS;
}
static mageResult mageCreateGraphicsPipeline(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{

    return MAGE_SUCCESS;
}
static mageResult mageCreateFramebuffers(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{

    return MAGE_SUCCESS;
}
static mageResult mageCreateCommandPool(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{

    return MAGE_SUCCESS;
}
static mageResult mageCreateCommandBuffers(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{

    return MAGE_SUCCESS;
}
static mageResult mageCreateSemaphores(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{

    return MAGE_SUCCESS;
}

mageResult mageRendererInitialise(struct mageRenderer *renderer, struct mageWindow *window, struct mageRendererProps *props)
{
    mageResult result = mageVulkanHandlerInitialise(&renderer->Handler, window);
    if (result != VK_SUCCESS) return result; 
    typedef mageResult (*functions)(struct mageRenderer *, struct mageWindow *, struct mageRendererProps *);

    functions required[] = { mageCreateSwapChain, mageCreateImageViews, mageCreateRenderPass, mageCreateGraphicsPipeline, mageCreateFramebuffers, mageCreateCommandPool, mageCreateCommandBuffers, mageCreateSemaphores };
    uint32_t i;

    for (i = 0; i < sizeof(required) / sizeof(functions); i++)
    {
        result = required[i](renderer, window, props);
        if (result != VK_SUCCESS) return result;
    }
    MAGE_LOG_CORE_INFORM("Renderer has been setup\n", NULL);
}
void mageRendererDestroy(struct mageRenderer *renderer)
{
    vkDestroySurfaceKHR(renderer->Handler.Instance, renderer->Surface, NULL);
    mageVulkanHandlerCleanup(&renderer->Handler);
}


#endif