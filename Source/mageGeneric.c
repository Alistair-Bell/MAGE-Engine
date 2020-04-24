#include "mageAPI.h"

void mageFreeMethod(void *item)
{
	free(item);
}
void mageTryDumpSuccess(uint8 contents, void *state)
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
	case MAGE_LOG_INFORM: 
		printf("%s", "\x1b[32m");
		strcpy(modeString, "Inform");
		break;
	case MAGE_LOG_WARNING: 
		printf("%s", "\x1b[33m");
		strcpy(modeString, "Warning");
		break;
	case MAGE_LOG_ERROR: 
		printf("%s", "\x1b[35m"); 
		strcpy(modeString, "Error");
		break;
	case MAGE_LOG_FATAL_ERROR: 
		printf("%s", "\x1b[31m");
		strcpy(modeString, "Fatal");
		break;
	}
	switch (user)
	{
	case MAGE_LOG_CORE: strcpy(userString, "core"); break;
	case MAGE_LOG_CLIENT: strcpy(userString, "client"); break;
	}
	
	char str[256];
	va_list args;
    va_start(args, format);
    vsprintf(str, format, args);
	va_end(args);

	char *bar = malloc(128);
	sprintf(bar, "[Log %s %s][%s : %d] : %s", userString, modeString, file, line, str);

	printf("%s", bar);
	mageFileDumpContents("Logs/mage.log", bar, 1, NULL); 

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
}
void mageResizableListPush(mageResizableList *resizableList, void *item)
{
	resizableList->Quantity++;
	resizableList->Elements = realloc(resizableList->Elements, resizableList->ElementSize * resizableList->Quantity);
	resizableList->Elements[resizableList->Quantity - 1] = item;
}
void mageResizableListPop(mageResizableList *resizableList, void *buffer, const uint8 reallocatable)
{	
	if (reallocatable)
		buffer = realloc(buffer, resizableList->ElementSize);
	
	memcpy(buffer, resizableList->Elements[resizableList->Quantity - 1], resizableList->ElementSize);
	mageFreeMethod(resizableList->Elements[resizableList->Quantity - 1]);
	resizableList->Quantity--;
	resizableList->Elements = realloc(resizableList->Elements, resizableList->ElementSize * resizableList->Quantity);
}
void mageResizableListDestroy(mageResizableList *resizableList)
{
	mageFreeMethod(resizableList);	
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
		case 0:
			f = fopen(file, "w");
		default:
			f = fopen(file, "w+");
	}
	if (f == NULL)
	{
		mageTryDumpSuccess(0, success);
		return;
	}
	fprintf(f, "\n");
	fprintf(f, "%s", buffer);
	fclose(f);

	mageTryDumpSuccess(1, success);

}

