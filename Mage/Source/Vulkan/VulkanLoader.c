#include "VulkanRenderer.h"

U8  MageVulkanRendererValidateExtensionsPresent(const char *extensions[], const U64 count)
{
	U32 rawCount = 0;

	vkEnumerateInstanceExtensionProperties(NULL, &rawCount, NULL);
	
	if (rawCount < count)
		return MageFalse;

	VkExtensionProperties *properties = calloc(rawCount, sizeof(VkExtensionProperties));
	vkEnumerateInstanceExtensionProperties(NULL, &rawCount, properties);
	
	U32 foundCount = 0;
	U32 i, j;

	/* Linear string search, laugh at it's inefficiency */

	for (i = 0; i < count; i++)
	{
		const char *current = extensions[i];
		for (j = 0; j < rawCount; j++)
		{
			if (strcmp(current, properties[j].extensionName) == 0)
			{
				foundCount++;
				break;
			}
		}
	}
	printf("Inform: Vulkan loader: found %d of the %d extensions requested\n", foundCount, count);

	free(properties);
	return (foundCount == count);
}
U8  MageVulkanRendererValidateLayersPresent(const char *layers[], const U64 count)
{
	U32 rawCount;
	vkEnumerateInstanceLayerProperties(&rawCount, 0);
	
	if (rawCount < count)
		return MageFalse;

	VkLayerProperties *properties = calloc(rawCount, sizeof(VkLayerProperties));
	vkEnumerateInstanceLayerProperties(&rawCount, properties);

	U32 foundCount = 0;
	U32 i, j;

	/* Linear string search, laugh at it's inefficiency */

	for (i = 0; i < count; i++)
	{
		const char *current = layers[i];
		for (j = 0; j < rawCount; j++)
		{
			if (strcmp(current, properties[j].layerName) == 0)
			{
				foundCount++;
				break;
			}
		}
	}

	printf("Inform: Vulkan loader: found %d of the %d layers requested\n", foundCount, count);
	free(properties);
	return MageTrue;
}