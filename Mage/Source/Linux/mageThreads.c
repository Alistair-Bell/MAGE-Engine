#include <mageAPI.h>

struct mageThreadType
{
    pthread_t NativeThread;

};

static void mageThreadHandle(int32_t error)
{
    if (error)
    {
        MAGE_LOG_CORE_ERROR("PThread error: %d\n", strerror(error));
    }
    assert(error != 1);
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
void mageThreadDestroy(mageThread thread)
{
    struct mageThreadType type = (*(struct mageThreadType *)thread);
    mageThreadHandle(pthread_join(type.NativeThread, NULL));
    free(thread);
}