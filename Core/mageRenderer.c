#include "mageAPI.h"


void *mageVulkanHandlerAllocate()
{
    return malloc(sizeof(struct MAGE_VULKAN_HANDLER_STRUCT));
}
void mageVulkanHandlerInitialise(mageVulkanHandler *handler, mageWindow *window, uint8 *success)
{
    #if defined(MAGE_VULKAN)
        
        const char * const RequiredValidationLayers[] = { "VK_LAYER_KHRONOS_validation", };
        const char * const RequiredDeviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, };
        

        {
            const validationLayerCount = 1;

            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount, NULL);

            VkLayerProperties *availableLayers = calloc(layerCount, sizeof(struct VkLayerProperties));
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

            uint8 i, j, layerFound;

            for (i = 0; i < validationLayerCount; i++)
            {
                layerFound = 0;

                for (j = 0; j < layerCount; j++)
                {
                    if (strcmp(RequiredValidationLayers[i], availableLayers[j].layerName) == 0)
                    {
                        MAGE_LOG_CORE_INFORM("Required validation layer found [%s]\n", availableLayers[j].layerName);
                        layerFound = 1;
                        break;
                    }
                }
            }

            if (!layerFound) 
            {
                MAGE_LOG_CORE_FATAL_ERROR("Failed to find vulkan validation layers\n", NULL);
                mageTryDumpSuccess(0, success);
                return;
            }

            mageFreeMethod(availableLayers);
        }
        {
            /*!************************
                Creating instance
            **************************/
            VkApplicationInfo applicationInformation;
            applicationInformation.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            applicationInformation.pNext = NULL;
            applicationInformation.pApplicationName = window->Title;
            applicationInformation.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
            applicationInformation.pEngineName = "MAGE";
            applicationInformation.apiVersion = VK_MAKE_VERSION(1, 0, 26);

            VkInstanceCreateInfo createInformation;
            createInformation.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInformation.pNext = NULL;
            createInformation.flags = 0;
            createInformation.pApplicationInfo = &applicationInformation;
            createInformation.enabledExtensionCount = 1;
            
            #if MAGE_VALIDATION_LAYERS
                createInformation.ppEnabledLayerNames = RequiredValidationLayers;
                createInformation.enabledLayerCount = 1;
            #else
                createInformation.ppEnabledLayerNames = NULL;
                createInformation.enabledLayerCount = 0;
            #endif
            
            #if defined(MAGE_GLFW)

                uint32 glfwExtensionCount;
                const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

                

                createInformation.enabledExtensionCount = glfwExtensionCount;
                createInformation.ppEnabledExtensionNames = glfwExtensions;

            #endif

            VkResult result = vkCreateInstance(&createInformation, NULL, &handler->Instance);

            if (result != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Failed to create vulkan instance\n", NULL);
                mageTryDumpSuccess(0, success);
                return;
            }
            MAGE_LOG_CORE_INFORM("Vulkan instance has been created\n", NULL);
        }
        {
            /*!************************
                Getting devices
            **************************/
            uint32 count;
            vkEnumeratePhysicalDevices(handler->Instance, &count, NULL);
    
            if (count <= 0)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Unable to find physical device!\n", NULL);
                mageTryDumpSuccess(0, success);
                return;
            }

            VkPhysicalDevice *devices = calloc(count, sizeof(VkPhysicalDevice));
            vkEnumeratePhysicalDevices(handler->Instance, &count, devices);

            handler->PhysicalDevice = devices[1];

            mageFreeMethod(devices);
        }
        {
            /*!************************
                Checking if device is suitible
            **************************/
            VkPhysicalDeviceProperties deviceProperties;
            
            VkPhysicalDeviceFeatures deviceFeatures;
            
            vkGetPhysicalDeviceProperties(handler->PhysicalDevice, &deviceProperties);
            
            vkGetPhysicalDeviceFeatures(handler->PhysicalDevice, &deviceFeatures);
 
            uint8 flag = deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;

            if (!flag) 
            {
                MAGE_LOG_CORE_FATAL_ERROR("GPU is unsuitable\n", NULL);
                mageTryDumpSuccess(0, success);
                return;
            }
            handler->PhysicalProperties = deviceProperties;

            MAGE_LOG_CORE_INFORM("GPU is suitable, using [%s]\n", deviceProperties.deviceName);
        }
        {
            vkGetPhysicalDeviceMemoryProperties(handler->PhysicalDevice, &handler->MemoryProperties);
        }
        {
            /*!************************
                Getting queue properties
            **************************/
            uint32 count, i;
            uint8 found;
            vkGetPhysicalDeviceQueueFamilyProperties(handler->PhysicalDevice, &count, NULL);
            VkQueueFamilyProperties *family = calloc(count, sizeof(struct VkQueueFamilyProperties));
            vkGetPhysicalDeviceQueueFamilyProperties(handler->PhysicalDevice, &count, family);
            
            for (i = 0; i < count; i++)
            {
                if (family[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    found = 1;
                    handler->GraphicsFamilyIndex = i;
                }
            }
            if (!found)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Graphics family index not found\n", NULL);
                mageTryDumpSuccess(0, success);
                return;
            }
            MAGE_LOG_CORE_INFORM("Graphics family index found\n", NULL);

            mageFreeMethod(family);      
        }
        {   
            /*!************************
                Creating device
            **************************/
            VkResult result = vkCreateDevice(handler->PhysicalDevice,
                  &(VkDeviceCreateInfo) {
                     .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                     .queueCreateInfoCount = 1,
                     .pQueueCreateInfos = &(VkDeviceQueueCreateInfo) {
                        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                        .queueFamilyIndex = 0,
                        .queueCount = 1,
                        .pQueuePriorities = (float []) { 1.0f },
                     },
                     .enabledExtensionCount = 1,
                     .ppEnabledExtensionNames = RequiredDeviceExtensions,
                  },
                  NULL,
                  &handler->Device);

            if (result != VK_SUCCESS)
            {
                MAGE_LOG_CORE_FATAL_ERROR("Vulkan device has failed to be created\n", NULL);
                mageTryDumpSuccess(0, success);
                return;
            }

            MAGE_LOG_CORE_INFORM("Vulkan device has been created\n", NULL);
        }
        {
            /*!************************
                Creating surface
            **************************/
            
            #if defined(MAGE_GLFW)
                VkResult result = glfwCreateWindowSurface(handler->Instance, window->Context, NULL, &handler->Surface);

                if (result != VK_SUCCESS)
                {
                    MAGE_LOG_CORE_FATAL_ERROR("GLFW has failed to create a window surface\n", NULL);
                    mageTryDumpSuccess(0, success);
                    return;  
                }
                MAGE_LOG_CORE_INFORM("GLFW has succesfully created a window surface\n", NULL);
            #endif
        
            uint32 flag; 
            vkGetPhysicalDeviceSurfaceSupportKHR(handler->PhysicalDevice, handler->GraphicsFamilyIndex, handler->Surface, &flag);

            if (flag)
            {
                MAGE_LOG_CORE_INFORM("Physical device supports KHR surface\n", NULL);
                handler->GraphicsPresentFamily = handler->GraphicsFamilyIndex;
            }
        }
    {
    
    }

    #endif
    mageTryDumpSuccess(1, success);
}
void mageVulkanHandlerCleanup(mageVulkanHandler *handler)
{
    #if defined(MAGE_VULKAN)
        vkDestroyDevice(handler->Device, NULL);
        vkDestroySurfaceKHR(handler->Instance, handler->Surface, NULL);
        vkDestroyInstance(handler->Instance, NULL);
    #endif
}
void *mageRendererAllocate()
{
    return malloc(sizeof(struct MAGE_RENDERER_STRUCT));
}
void mageRendererInitialise(mageRenderer *renderer,  mageWindow *window, uint8 *success)
{
    #if defined(MAGE_VULKAN)
        mageVulkanHandlerInitialise(&renderer->Handler, window, success);
    #endif
}
void mageRendererDestroy(mageRenderer *renderer)
{
    #if defined(MAGE_VULKAN)
        mageVulkanHandlerCleanup(&renderer->Handler);
    #endif
}