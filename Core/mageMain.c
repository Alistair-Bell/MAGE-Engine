#include "mageAPI.h"

static mageVulkanHandler vulkan;

int main(void)
{
	mageVulkanHandlerInitialise(&vulkan, NULL);
	
	MAGE_LOG_CLIENT_INFORM("[%s]\n", vulkan.GPUProperties.deviceName);
	
	
	mageVulkanHandlerFree(&vulkan);
	mageLogReset();
}

