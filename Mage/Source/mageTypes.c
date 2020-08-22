#include "mageAPI.h"

void mageQueueCreate(struct mageQueue *queue, const uint32_t dataSize)
{   
    queue->DataSize     = dataSize;
    queue->Count        = 0;
    queue->Data         = MAGE_MEMORY_ARRAY_ALLOCATE(0, dataSize);
}
void mageQueueCreateFromSet(struct mageQueue *queue, uint32_t elementCount, uint32_t elementSize, void *data)
{
    queue->DataSize     = elementSize;
    queue->Count        = elementCount;
    queue->Data         = MAGE_MEMORY_ARRAY_ALLOCATE(elementCount, elementSize);
    memcpy(queue->Data, data, elementCount * elementSize);
}
void mageQueuePush(struct mageQueue *queue, void *data)
{
    uint64_t index = queue->DataSize * queue->Count;
    queue->Count++;
	queue->Data = MAGE_MEMORY_REALLOCATE(queue->Data, queue->DataSize * queue->Count);
	memcpy(queue->Data + index, data, queue->DataSize);
}
mageResult mageQueuePop(struct mageQueue *queue, void *buffer)
{
    if (queue->Count <= 0)
    {
        MAGE_LOG_CORE_WARNING("Queue (%p) has nothing to pop\n", queue);
        return MAGE_RESULT_DATA_NOT_PRESENT;
    }
    queue->Count--;
    uint64_t index = queue->DataSize * queue->Count;
    memcpy(buffer, queue->Data + index, queue->DataSize);
    queue->Data = MAGE_MEMORY_REALLOCATE(queue->Data, queue->DataSize * queue->Count);
    return MAGE_RESULT_SUCCESS;
}
void mageQueueDestroy(struct mageQueue *queue)
{
    MAGE_MEMORY_FREE(queue->Data);
}
