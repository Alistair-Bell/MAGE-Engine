#include "mageAPI.h"

void *mageVulkanHandlerAllocate()
{
    return malloc(sizeof(struct MAGE_VULKAN_HANDLER_STRUCT));
}
void mageVulkanHandlerInitialise(mageVulkanHandler *handler, mageWindow *window, uint8 *success)
{
    #if defined(MAGE_VULKAN)
        {
            /*
                Creating instance
            */
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
            createInformation.enabledExtensionCount = 0;
            createInformation.enabledLayerCount = 0;
            createInformation.ppEnabledLayerNames = NULL;
            
            
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
            /*
                Getting devices
            */
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

            handler->PhysicalDevice = devices[count - 1];

            mageFreeMethod(devices);
        }
        {
            /*
                Checking if device is suitible
            */
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
            MAGE_LOG_CORE_INFORM("GPU is suitable found [%s]\n", deviceProperties.deviceName);
        }
        {
            /*
                Getting queue properties
            */
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
        float *queuePriorities = calloc(1, sizeof(float));
        queuePriorities[0] = 1.0f;

        VkDeviceQueueCreateInfo deviceQueueCreateInformation;
        deviceQueueCreateInformation.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInformation.queueFamilyIndex = handler->GraphicsFamilyIndex;       
        deviceQueueCreateInformation.queueCount = 1;
        deviceQueueCreateInformation.pQueuePriorities = queuePriorities;

        VkDeviceCreateInfo deviceCreateInformation;
        deviceCreateInformation.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInformation.queueCreateInfoCount = 1;
        deviceCreateInformation.pQueueCreateInfos = &deviceQueueCreateInformation;



       /* VkResult result = vkCreateDevice(handler->PhysicalDevice, &deviceCreateInformation, NULL, &handler->Device);
         */

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
