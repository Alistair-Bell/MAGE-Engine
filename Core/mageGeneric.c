#include "mageAPI.h"

void mageEngineInitialise(uint8_t *success)
{
	#if defined(MAGE_DEBUG)
		mageFileDumpContents("Logs/mage.log", "", 1, NULL);
		mageLogInitialise("Logs/mage.log");
		MAGE_LOG_CORE_WARNING("Debug mode in uses, for best performance turn debug mode of\n", NULL);
		MAGE_LOG_CORE_INFORM("Cleaned previous file contents\n", NULL);
	#endif

	#if defined(MAGE_SDL2)
		const uint32_tflag = SDL_Init(SDL_INIT_EVERYTHING);
			
		if (flag != 0)
		{
			MAGE_LOG_CLIENT_FATAL_ERROR("SDL2 failed to initialise : %s\n", SDL_GetError());
			mageTryDumpSuccess(0, success);
			return;
		}

		MAGE_LOG_CORE_INFORM("SDL2 has succesfully initialised everything\n", NULL);

	#endif

	#if defined(MAGE_GLFW)

		if (!glfwInit())
		{
			mageTryDumpSuccess(0, success);
			MAGE_LOG_CORE_FATAL_ERROR("GLFW library has failed to initialise\n", NULL);
			return;
		}	

		#if defined(MAGE_VULKAN)
			uint8_t flag = glfwVulkanSupported();

			if (!flag)
			{
				MAGE_LOG_CLIENT_FATAL_ERROR("GLFW does not supoort vulkan\n", NULL);
				mageTryDumpSuccess(0, success);
				return;
			}
			MAGE_LOG_CORE_INFORM("GLFW supports vulkan\n", NULL);

		#endif

		MAGE_LOG_CORE_INFORM("GLFW has succesfully initialised everything.\n", NULL);

	#endif
	MAGE_LOG_CORE_INFORM("Engine dependencies initialised\n", NULL);


}

void *mageAllocationMethod(const uint64_t size)
{
	return malloc(size);
}
void mageFreeMethod(void *item)
{
	free(item);
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
	mageFreeMethod(resizableList->Elements[resizableList->Quantity - 1]);
	resizableList->Quantity--;
	resizableList->Elements = realloc(resizableList->Elements, resizableList->ElementSize * resizableList->Quantity);
}
void mageResizableListFreeElements(mageResizableList *resizableList)
{
	uint32_t i;
	for (i = 0; i < resizableList->Quantity - 1; i++)
	{
		mageFreeMethod(resizableList->Elements[i]);
	}
}
void mageResizableListDestroy(mageResizableList *resizableList)
{
	mageResizableListFreeElements(resizableList);
	mageFreeMethod(resizableList);	
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
	mageFreeMethod(pair->First);
	mageFreeMethod(pair->Second);
}
void magePairDestroy(magePair *pair)
{
	magePairFree(pair);
	mageFreeMethod(pair);
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
void mageFileReadContents(const char *file, char *buffer, const uint8_t reallocatable, uint8_t *success)
{
	FILE *f = fopen(file, "rt");
	if (f == NULL)
	{
		mageTryDumpSuccess(0, success);	
		return;
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
	mageTryDumpSuccess(1, success);
}
void mageFileDumpContents(const char *file, const char *buffer, const uint8_t clean, uint8_t *success)
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
		mageTryDumpSuccess(0, success);
		return;
	}
	fprintf(f, "%s", buffer);
	fclose(f);

	mageTryDumpSuccess(1, success);

}

