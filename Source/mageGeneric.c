#include "mageAPI.h"

void mageFreeMethod(void *item)
{
	free(item);
}
void mageTryDumpSuccess(uint64 contents, void *state)
{
	if (state != NULL && sizeof(*state) <= 1)
		state = contents;
		return;	
	MAGE_LOG_CLIENT_WARNING("Passsd in contents is less than 8 bits in size", NULL);

}
void mageLogMessage(const uint8 user, const uint8 severity, const uint32 line, const char *file, const char *format, ...)
{
	char modeString[8];
	char userString[10];
	switch (severity)
	{
	case MAGE_LOG_MODE_INFORM: 
		printf("%s", "\x1b[32m");
		strcpy(modeString, "Inform");
		break;
	case MAGE_LOG_MODE_WARNING: 
		printf("%s", "\x1b[33m");
		strcpy(modeString, "Warning");
		break;
	case MAGE_LOG_MODE_ERROR: 
		printf("%s", "\x1b[35m"); 
		strcpy(modeString, "Error");
		break;
	case MAGE_LOG_MODE_FATAL_ERROR: 
		printf("%s", "\x1b[31m");
		strcpy(modeString, "Fatal");
		break;
	}
	switch (user)
	{
	case MAGE_LOG_USER_CORE: strcpy(userString, "core"); break;
	case MAGE_LOG_USER_CLIENT: strcpy(userString, "client"); break;
	}
	
	char str[256];
	va_list args;
    va_start(args, format);
    vsprintf(str, format, args);
	va_end(args);

	char *bar = malloc(128);
	sprintf(bar, "[Log %s %s][%s : %d] : %s", userString, modeString, file, line, str);

	printf("%s", bar);
	mageFileDumpContents("Logs/mage.log", bar, 0, NULL); 

	mageFreeMethod(bar);
}
void mageLogReset()
{
	printf("%s", "\x1b[0m");
}


void *mageResizableListAllocate()
{
	return malloc(sizeof(struct MAGE_RESIZABLE_LIST_STRUCT));
}
void mageResizableListInitialise(mageResizableList *resizableList, const uint32 size)
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
	uint32 i;
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
void magePairInitialise(magePair *pair, const uint32 firstSize, const uint32 secondSize)
{
	pair->First = malloc(sizeof(firstSize));
	pair->Second = malloc(sizeof(secondSize));

	pair->Firstize = firstSize;
	pair->SecondSize = secondSize;
}
void magePairSetFirst(magePair *pair, void *item)
{
	memcpy(pair->First, item, pair->Firstize);
}
void magePairSetSecond(magePair *pair, void *item)
{
	memcpy(pair->Second, item, pair->SecondSize);
}
void magePairSetBoth(magePair *pair, void *first, void *second)
{
	magePairSetFirst(pair, first);
	magePairSetSecond(pair, second);
}
void magePairGetFist(magePair *pair, void *buffer, uint8 reallocatable)
{
	if (reallocatable)
		buffer = realloc(buffer, pair->Firstize);
	memcpy(buffer, pair->First, pair->Firstize);
}
void magePairGetSecond(magePair *pair, void *buffer, uint8 reallocatable)
{
	if (reallocatable)
		buffer = realloc(buffer, pair->SecondSize);
	memcpy(buffer, pair->Second, pair->SecondSize);
}
void magePairGetBoth(magePair *pair, void *buffer1, void *buffer2, uint8 reallocatable)
{
	magePairGetFist(pair, buffer1, reallocatable);
	magePairGetSecond(pair, buffer2, reallocatable);
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


void mageFileReadContents(const char *file, char *buffer, const uint8 reallocatable, uint8 *success)
{
	FILE *f = fopen(file, "rt");
	if (f == NULL)
	{
		mageTryDumpSuccess(0, success);	
		return;
	}
    fseek(f, 0, SEEK_END);
    uint64 length = ftell(f);	
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
void mageFileDumpContents(const char *file, const char *buffer, const uint8 clean, uint8 *success)
{
	FILE *f;
	switch(clean)
	{
		case 1:
			f = fopen(file, "w");
		default:
			f = fopen(file, "w+");
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

