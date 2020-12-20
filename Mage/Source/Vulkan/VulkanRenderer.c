#include "VulkanRenderer.h"

typedef U8 (*MageVulkanCreateCallback)(MageRendererCreateInfo *, MageRenderer *);

static U0 MageVulkanRendererDestroyValidationLayers(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    PFN_vkDestroyDebugUtilsMessengerEXT function = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (function != NULL)
        function(instance, debugMessenger, pAllocator);
}
static VkResult MageVulkanRendererHandleLayerCreation(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
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
    // messageSeverity != VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT && 
    if (messageSeverity != VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
        printf("Warning: Vulkan validation layer message: %s\n", pCallbackData->pMessage);
    return VK_SUCCESS;
}

U8 MageVulkanRendererCreateInstance(MageRendererCreateInfo *info, MageRenderer *renderer)
{
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
    instanceInfo.enabledExtensionCount   = sizeof(MageVulkanRendererRequiredExtensions) / sizeof(const char *);
    instanceInfo.ppEnabledExtensionNames = MageVulkanRendererRequiredExtensions;

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
U8 MageVulkanRendererCreateDebugLayers(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo;
    MageVulkanRendererFillValidationLayerCreateInfo(&messengerCreateInfo);
    VkResult result = MageVulkanRendererHandleLayerCreation(renderer->Overseer.Instance, &messengerCreateInfo, NULL, &renderer->Overseer.DebugMessenger);

    return result == VK_SUCCESS;
}
U8 MageVulkanRendererCreateSurface(MageRendererCreateInfo *info,  MageRenderer *renderer)
{
    VkResult result;

#if MAGE_BUILD_PLATFORM_WINDOWS

    VkWin32SurfaceCreateInfoKHR win32SurfaceInfo;
    memset(&win32SurfaceInfo, 0, sizeof(VkWin32SurfaceCreateInfoKHR));
    win32SurfaceInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    win32SurfaceInfo.flags     = 0;
    win32SurfaceInfo.hinstance = info->Window->Instance;
    win32SurfaceInfo.hwnd      = info->Window->NativeWindow;
    result = vkCreateWin32SurfaceKHR(renderer->Overseer.Instance, &win32SurfaceInfo, NULL, &renderer->Surface.Surface);

#elif MAGE_BUILD_PLATFORM_LINUX
    
    VkXlibSurfaceCreateInfoKHR xlibSurfaceInfo;
    memset(&xlibSurfaceInfo, 0, sizeof(VkXlibSurfaceCreateInfoKHR));
    xlibSurfaceInfo.sType   = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
    xlibSurfaceInfo.flags   = 0;
    xlibSurfaceInfo.window  = info->Window->ContextWindow;
    xlibSurfaceInfo.dpy     = info->Window->WindowDisplay;
    result = vkCreateXlibSurfaceKHR(renderer->Overseer.Instance, &xlibSurfaceInfo, NULL, &renderer->Surface.Surface);

#endif

    return result == VK_SUCCESS;
}

U8 MageRendererCreate(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    MAGE_HANDLE_ERROR_MESSAGE(info == NULL || renderer == NULL, printf("Error: Any passed parameters are NULL!\n"));
    memset(renderer, 0, sizeof(MageRenderer));
    
    MageVulkanCreateCallback methods[] = 
    {
        MageVulkanRendererCreateInstance,
        #if MAGE_BUILD_DEBUG_MODE
            MageVulkanRendererCreateDebugLayers,
        #endif
        MageVulkanRendererCreateSurface,
        MageVulkanRendererCreatePhysicalDevice
    };

    U64 count = sizeof(methods) / sizeof(MageVulkanCreateCallback);
    U64 i;
    
    for (i = 0; i < count; i++)
    {
        VkResult current = methods[i](info, renderer);
        MAGE_HANDLE_ERROR_MESSAGE(!current, printf("Error: Failed to create renderer, passed %lu of %lu operations\n", i, count));
    }
    printf("Inform: Renderer has been created, passed %lu of %lu operations\n", i, count);
    return MageTrue;
}
U8 MageRendererHandleWindowResize(MageRenderer *renderer)
{
    return MageTrue;
}
U8 MageRendererDestroy(MageRenderer *renderer)
{

    #if MAGE_BUILD_DEBUG_MODE
        MageVulkanRendererDestroyValidationLayers(renderer->Overseer.Instance, renderer->Overseer.DebugMessenger, NULL);
    #endif
    vkDestroySurfaceKHR(renderer->Overseer.Instance, renderer->Surface.Surface, NULL);
    vkDestroyInstance(renderer->Overseer.Instance, NULL);
    return MageTrue;
}