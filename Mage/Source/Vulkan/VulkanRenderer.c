#include "VulkanRenderer.h"

U8 MageVulkanRendererCreateInstance(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    VkApplicationInfo applicationInfo;
    memset(&applicationInfo, 0, sizeof(VkApplicationInfo));
    applicationInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.apiVersion          = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo instanceInfo;
    memset(&instanceInfo, 0, sizeof(VkInstanceCreateInfo));
    instanceInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &applicationInfo;

    VkResult result = vkCreateInstance(&instanceInfo, NULL, &renderer->Overseer.Instance);
    return (result == VK_SUCCESS);
}


U8 MageRendererCreate(MageRendererCreateInfo *info, MageRenderer *renderer)
{
    MAGE_HANDLE_ERROR_MESSAGE(info == NULL || renderer == NULL, printf("Error: Any passed parameters are NULL!\n"));
    memset(renderer, 0, sizeof(MageRenderer));

    MageVulkanRendererCreateInstance(info, renderer);
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