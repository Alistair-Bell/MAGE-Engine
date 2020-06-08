#include <mageAPI.h>

#if defined(MAGE_VULKAN)

    mageResult mageRendererInitialise(mageRenderer *renderer, mageWindow *window)
    {
        mageResult result = mageVulkanHandlerInitialise(&renderer->Handler, window);

        if (result != MAGE_SUCCESS) 
        {
            return result;
        }
    
    }

#endif