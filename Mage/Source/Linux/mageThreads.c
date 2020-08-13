#include <mageAPI.h>

struct mageThreadType
{
    pthread_t   NativeThread;
    uint32_t    WorkerID:
};

static void mageThreadHandle(int32_t error)
{
    if (error)
    {
        MAGE_LOG_CORE_ERROR("PThread error: %d\n", strerror(error));
    }
    MAGE_ASSERT(error != 1);
}

mageThread mageThreadCreate()
{
   return malloc(sizeof(struct mageThreadType));
}
void mageThreadBegin(mageThread thread, mageThreadJobCallback callback, void *data)
{
    struct mageThreadType type = (*(struct mageThreadType *)thread);
    /* Creating thread */
    mageThreadHandle(pthread_create(&type.NativeThread, NULL, callback, data));
    memcpy(thread, &type, sizeof(struct mageThreadType));
}
uint32_t mageThreadGetID(const mageThread thread)
{
    return (uint32_t) pthread_self();
}
void mageThreadDestroy(mageThread thread)
{   
    if (thread == NULL) return;
    struct mageThreadType type = (*(struct mageThreadType *)thread);
    uint8_t result;
    mageThreadHandle(pthread_join(type.NativeThread, (void **)&result));
    MAGE_ASSERT(result == 1);
}