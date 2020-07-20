#include "mageAPI.h"

struct mageHeapAllocater mageHeapAllocaterDefault()
{
	return (struct mageHeapAllocater){ .Allocate = malloc, .Free = free, .ListAllocater = calloc, .Reallocater = realloc };
}

void mageQueueCreate(struct mageQueue *queue, const uint32_t dataSize, const struct mageHeapAllocater *allocater)
{
    queue->Count        = 0;
    queue->DataSize     = dataSize;
    queue->Data         = allocater->ListAllocater(0, dataSize); 
}
extern void mageQueuePush(struct mageQueue *queue, void *data, const struct mageHeapAllocater *allocater)
{
	queue->Count++;
	register uint32_t index = queue->Count - 1;	
	queue->Data = allocater->Reallocater(queue->Data, queue->DataSize * queue->Count);
	queue->Data[index] = allocater->Allocate(queue->DataSize);
	memcpy(queue->Data[index], data, queue->DataSize);
}
extern void *mageQueuePop(struct mageQueue *queue, const struct mageHeapAllocater *allocater)
{
	if (queue->Count <= 0) 
	{ 
		MAGE_LOG_CORE_WARNING("Queue has no elements to pop\n", NULL); 
		return NULL; 
	}
	queue->Count--;
	void *data; 
	memcpy(data, queue->Data[queue->Count], queue->DataSize);
	queue->Data = allocater->Reallocater(queue->Data, queue->DataSize * queue->Count);
	return data;
}
void mageQueueDestroy(struct mageQueue *queue, const struct mageHeapAllocater *allocater)
{
	uint32_t i;
	for (i = 0; i < queue->Count; i++)
	{
		allocater->Free(queue->Data[i]);
	}
	allocater->Free(queue->Data);
}

char *mageFileReadContents(const char *file, const char *readmode, uint32_t *fileSize)
{
	FILE *f = fopen(file, readmode);
	if (f == NULL)
	{
		MAGE_LOG_CORE_ERROR("Invalid file %s\n", file);
		return NULL;
	}
    fseek(f, 0, SEEK_END);
    uint64_t length = ftell(f);	
	char *foo = malloc(sizeof(char) * (length + 1));
	memset(foo, 0, length + 1);
    fseek(f, 0, SEEK_SET);
    fread(foo, 1, length, f);
    fclose(f);
	
	if (fileSize != NULL) *fileSize = length;
	return foo;
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
		return MAGE_RESULT_INVALID_INPUT;
	}
	fprintf(f, "%s", buffer);
	fclose(f);

	return MAGE_RESULT_SUCCESS;
}

