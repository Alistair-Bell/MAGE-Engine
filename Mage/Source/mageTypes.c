#include "mageAPI.h"

void mageQueueCreate(struct mageQueue *queue, const uint32_t dataSize, const struct mageHeapAllocater *allocater)
{   
    queue->DataSize     = dataSize;
    queue->Count        = 0;
    queue->Data         = allocater->ListAllocater(0, dataSize);
}
void mageQueueCreateFromSet(struct mageQueue *queue, uint32_t elementCount, uint32_t elementSize, void *data, struct mageHeapAllocater *allocater)
{
    queue->DataSize     = elementSize;
    queue->Count        = elementCount;
    queue->Data         = allocater->ListAllocater(elementCount, elementSize);
    memcpy(queue->Data, data, elementCount * elementSize);
}
void mageQueuePush(struct mageQueue *queue, void *data, const struct mageHeapAllocater *allocater)
{
    uint64_t index = queue->DataSize * queue->Count;
    queue->Count++;
	queue->Data = allocater->Reallocater(queue->Data, queue->DataSize * queue->Count);
	memcpy(queue->Data + index, data, queue->DataSize);
}
mageResult mageQueuePop(struct mageQueue *queue, void *buffer, const struct mageHeapAllocater *allocater)
{
    if (queue->Count <= 0)
    {
        MAGE_LOG_CORE_WARNING("Queue (%p) has nothing to pop\n", queue);
        return MAGE_RESULT_DATA_NOT_PRESENT;
    }
    queue->Count--;
    uint64_t index = queue->DataSize * queue->Count;
    memcpy(buffer, queue->Data + index, queue->DataSize);
    queue->Data = allocater->Reallocater(queue->Data, queue->DataSize * queue->Count);
    return MAGE_RESULT_SUCCESS;
}
void mageQueueDestroy(struct mageQueue *queue, const struct mageHeapAllocater *allocater)
{
    free(queue->Data);
}
