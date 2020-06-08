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
void mageTryDumpSuccess(uint8_t contents, uint8_t *state)
{
	if (state != NULL)
		memcpy(state, &contents, sizeof(*state));
		return;	
}
void *mageResizableListAllocate()
{
	return malloc(sizeof(struct MAGE_RESIZABLE_LIST_STRUCT));
}
void mageResizableListInitialise(mageResizableList *resizableList, const uint32_t size)
{
	resizableList->Quantity = 0;
	resizableList->ElementSize = size;
	resizableList->Elements = calloc(1, size);
}
void mageResizableListPush(mageResizableList *resizableList, void *item)
{
	resizableList->Quantity++;
	void *buffer = malloc(resizableList->ElementSize);
	memcpy(buffer, item, resizableList->ElementSize);
	resizableList->Elements = realloc(resizableList->Elements, resizableList->ElementSize * resizableList->Quantity);
	resizableList->Elements[resizableList->Quantity - 1] = buffer;
}
void mageResizableListPop(mageResizableList *resizableList)
{	
	free(resizableList->Elements[resizableList->Quantity - 1]);
	resizableList->Quantity--;
	resizableList->Elements = realloc(resizableList->Elements, resizableList->ElementSize * resizableList->Quantity);
}
void mageResizableListFreeElements(mageResizableList *resizableList)
{
	uint32_t i;
	for (i = 0; i < resizableList->Quantity - 1; i++)
	{
		free(resizableList->Elements[i]);
	}
}
void mageResizableListDestroy(mageResizableList *resizableList)
{
	mageResizableListFreeElements(resizableList);
	free(resizableList);	
}
void *magePairAllocate()
{
	return malloc(sizeof(struct MAGE_PAIR_STRUCT));
}
void magePairInitialise(magePair *pair, const uint32_t firstSize, const uint32_t secondSize)
{
	pair->First = malloc(firstSize);
	pair->Second = malloc(secondSize);

	pair->FirstSize = firstSize;
	pair->SecondSize = secondSize;
}
void magePairSetFirst(magePair *pair, void *item)
{
	memcpy(pair->First, item, sizeof(item));
}
void magePairSetSecond(magePair *pair, void *item)
{
	memcpy(pair->Second, item, sizeof(item));
}
void magePairSetBoth(magePair *pair, void *first, void *second)
{
	magePairSetFirst(pair, first);
	magePairSetSecond(pair, second);
}
void magePairGetFist(magePair *pair, void *buffer, uint8_t reallocatable)
{
	if (reallocatable)
		buffer = realloc(buffer, pair->FirstSize);
	memcpy(buffer, pair->First, pair->FirstSize);
}
void magePairGetSecond(magePair *pair, void *buffer, uint8_t reallocatable)
{
	if (reallocatable)
		buffer = realloc(buffer, pair->SecondSize);
	memcpy(buffer, pair->Second, pair->SecondSize);
}
void magePairGetBoth(magePair *pair, void *buffer1, void *buffer2, uint8_t reallocatable)
{
	magePairGetFist(pair, buffer1, reallocatable);
	magePairGetSecond(pair, buffer2, reallocatable);
}
void magePairResizeFirst(magePair *pair, const uint32_t newSize)
{
	pair->First = realloc(pair->First, newSize);
	pair->FirstSize = newSize;
}
void magePairResizeSecond(magePair *pair, const uint32_t newSize)
{
	pair->First = realloc(pair->Second, newSize);
	pair->SecondSize = newSize;
}
void magePairResizeBoth(magePair *pair, const uint32_t newFirstSize, const uint32_t newSecondSize)
{
	magePairResizeFirst(pair, newFirstSize);
	magePairResizeSecond(pair, newSecondSize);
}
void magePairFree(magePair *pair)
{
	free(pair->First);
	free(pair->Second);
}
void magePairDestroy(magePair *pair)
{
	magePairFree(pair);
	free(pair);
}
void *mageDictionaryAllocate()
{
	return malloc(sizeof(struct MAGE_DICTIONARY_STRUCT));
}
void mageDictionaryInitialise(mageDictionary *dictionary)
{
	dictionary->Elements = mageResizableListAllocate();
	mageResizableListInitialise(dictionary->Elements, sizeof(struct MAGE_PAIR_STRUCT));
}
void mageDictionaryPush(mageDictionary *dictionary, magePair *pair)
{	
	mageResizableListPush(dictionary->Elements, pair);
}
void mageDictionaryPop(mageDictionary *dictionary)
{
	mageResizableListPop(dictionary->Elements);
}
void mageDictionaryFetch(mageDictionary *dictionary, magePair *buffer)
{
	memcpy(buffer, dictionary->Elements->Elements[dictionary->Elements->Quantity - 1], sizeof(*buffer));
}
void mageDictionaryFetchIndex(mageDictionary *dictionary, magePair *buffer, const uint32_t index)
{
	memcpy(buffer, dictionary->Elements->Elements[index], sizeof(*buffer));
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

