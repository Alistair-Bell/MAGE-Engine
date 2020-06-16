#include "mageAPI.h"

const char *mageToString(mageResult result)
{
	switch (result)
	{
		case MAGE_SUCCESS:
			return "Success";
			break;
		case MAGE_LIBRARY_FAILURE:
			return "Library initialisation has failed";
			break;
		case MAGE_INVALID_INPUT:
			return "Input was invalid";
			break;
		case MAGE_HARDWARE_INVALID:
			return "System hardware is invalid";
			break;
		case MAGE_INSTANCE_CREATION_FAILURE:
			return "Instance creation has failed";
			break;
		case MAGE_CONTEXT_CREATION_FAILED:
			return "Context creation failed";
			break;
		case MAGE_DEVICE_CREATION_FAILURE:
			return "Device creation failed";
			break;
		case MAGE_SURFACE_CREATION_FAILURE:
			return "Surface creation failed";
			break;
		case MAGE_FENCE_CREATION_FAILURE:
			return "Fence creation failed";	
			break;
		case MAGE_SEMAPHORE_CREATION_FAILURE:
			return "Semaphore creation failed";
			break;
		case MAGE_COMMAND_POOL_CREATION_FAILURE:
			return "Command pool creation failed";
			break;
		case MAGE_ALLOCATE_COMMAND_FAILURE:
			return "Buffer allocation creation failed";
			break;
		case MAGE_SWAPCHAIN_CREATION_FAILED:
			return "Swap chain creation failed";
			break;
		case MAGE_QUEUE_SUBMITION_FAILURE:
			return "Queue sub";
			break;
		case MAGE_HARDWARE_NOT_PRESENT:
			return "Hardware present";
			break;
		case MAGE_START_METHOD_FAILURE:
			return "Start method failure";
			break;
		case MAGE_UPDATE_FAILURE:
			return "Start method failure";
			break;
		case MAGE_DESTROY_METHOD_FAILURE:
			return "Destroy method failure";
			break;
		default:
			return "Unknown error";
	}
}
mageResult mageFileReadContents(const char *file, char *buffer, const uint8_t reallocatable)
{
	FILE *f = fopen(file, "rt");
	if (f == NULL)
	{
		return MAGE_INVALID_INPUT;
	}
    fseek(f, 0, SEEK_END);
    uint64_t length = ftell(f);	
	char *foo = malloc(sizeof(char) * (length + 1));
	memset(foo, 0, length + 1);
    fseek(f, 0, SEEK_SET);
    fread(foo, 1, length, f);
    fclose(f);
	
	if (reallocatable)
	{
		buffer = realloc(buffer, sizeof(foo));
	}
	strcpy(buffer, foo);
	free(foo);
	return MAGE_SUCCESS;
}
mageResult mageFileDumpContents(const char *file, const char *buffer, const uint8_t clean)
{
	FILE *f;
		switch(clean)
	{
		case 1:
			f = fopen(file, "w");
		default:
			f = fopen(file, "a");
	}
	if (f == NULL)
	{
		return MAGE_INVALID_INPUT;
	}
	fprintf(f, "%s", buffer);
	fclose(f);

	return MAGE_SUCCESS;
}

