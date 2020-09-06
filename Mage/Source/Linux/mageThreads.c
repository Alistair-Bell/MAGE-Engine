#include <mageAPI.h>

#if defined (MAGE_PLATFORM_LINUX)

struct mageThreadType
{
    pthread_t       NativeThread;
    uint32_t        Active;
};

static uint8_t mageThreadHandle(int32_t error)
{
    if (error)
    {
        MAGE_LOG_CORE_ERROR("PThread error code: %d\n", error);
        return 0;
    }
    return 1;
}

mageThread mageThreadCreate()
{
   return MAGE_MEMORY_ALLOCATE(sizeof(struct mageThreadType));
}
void mageThreadBegin(mageThread thread, mageThreadJobCallback callback, void *data)
{
    struct mageThreadType type = (*(struct mageThreadType *)thread);
    /* Creating thread */
    
    MAGE_LOG_CORE_INFORM("Beggining thread\n", NULL);
    type.Active = mageThreadHandle(pthread_create(&type.NativeThread, NULL, callback, data));
    if (!type.Active) 
    {
        MAGE_LOG_CORE_ERROR("Unable to create thread!, disabling multithreading, calling regulary\n", NULL);
        callback(data);
    }
    memcpy(thread, &type, sizeof(struct mageThreadType));
}
uint32_t mageThreadGetID(const mageThread thread)
{
    return (uint32_t) pthread_self();
}
void mageThreadEnd(mageThread thread)
{   
    if (thread == NULL) return;
    struct mageThreadType type = (*(struct mageThreadType *)thread);
    if (!type.Active) return;
    uint8_t result;
    MAGE_LOG_CORE_INFORM("Joining thread to master thread\n", NULL);
    mageThreadHandle(pthread_join(type.NativeThread, (void **)&result));
    memcpy(thread, &type, sizeof(struct mageThreadType));
}
void mageThreadTerminate(mageThread thread)
{
    struct mageThreadType type = (*(struct mageThreadType *)thread);
    pthread_cancel(type.NativeThread);
    MAGE_LOG_CORE_WARNING("Terminating thread, thread may have not completed\n", NULL);
    memcpy(thread, &type, sizeof(struct mageThreadType));    
}

#endif