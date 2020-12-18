#include "VulkanRenderer.h"

typedef U8 (*MageVulkanCreateCallback)(MageRendererCreateInfo *, MageRenderer *);

static const char *MageRequiredInstanceExtensions[] = 
{
    #if MAGE_BUILD_PLATFORM_LINUX
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
        VK_KHR_SURFACE_EXTENSION_NAME,
    #elif MAGE_BUILD_PLATFORM_WINDOWS
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    #endif
};

U8 MageVulkanRendererCreateInstance(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    VkApplicationInfo applicationInfo;
    memset(&applicationInfo, 0, sizeof(VkApplicationInfo));
    applicationInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.apiVersion          = VK_MAKE_VERSION(1, 0, 0);
    applicationInfo.pEngineName         = "MageEngine-Vulkan";

    VkInstanceCreateInfo instanceInfo;
    memset(&instanceInfo, 0, sizeof(VkInstanceCreateInfo));
    instanceInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo        = &applicationInfo;
    instanceInfo.enabledExtensionCount   = sizeof(MageRequiredInstanceExtensions) / sizeof(const char *);
    instanceInfo.ppEnabledExtensionNames = MageRequiredInstanceExtensions;

    #if MAGE_BUILD_DEBUG_MODE
        instanceInfo.ppEnabledLayerNames = (const char *[]) { "VK_LAYER_KHRONOS_validation" };
        instanceInfo.enabledLayerCount   = 1;
    #endif

    
    VkResult result = vkCreateInstance(&instanceInfo, NULL, &renderer->Overseer.Instance);
    MAGE_HANDLE_ERROR_MESSAGE(result != VK_SUCCESS, printf("Error: Failed to create vulkan instance. Error code %d\n", (I32)result));
    return result == VK_SUCCESS;
}

U8 MageRendererCreate(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    MAGE_HANDLE_ERROR_MESSAGE(info == NULL || renderer == NULL, printf("Error: Any passed parameters are NULL!\n"));
    memset(renderer, 0, sizeof(MageRenderer));

    MageVulkanCreateCallback methods[] = { MageVulkanRendererCreateInstance };
    U64 count = sizeof(methods) / sizeof(MageVulkanCreateCallback);
    U64 i;
    VkResult current = VK_SUCCESS;
    for (i = 0; i < count; i++)
    {
        current = methods[i](info, renderer);
        MAGE_HANDLE_ERROR_MESSAGE(!current, printf("Error: Failed to create renderer\n"));
    }
    printf("Inform: Renderer has been created\n");
    return MageTrue;
}
U8 MageRendererHandleWindowResize(MageRenderer *renderer)
{
    return MageTrue;
}
U8 MageRendererDestroy(MageRenderer *renderer)
{
    return MageTrue;
}