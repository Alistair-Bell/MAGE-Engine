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
void mageThreadBegin(mageThread thread, struct mageThreadBeginInfo *info)
{
    struct mageThreadType type = (*(struct mageThreadType *)thread);
    /* Creating thread */
    
    MAGE_LOG_CORE_INFORM("Created pthread of id %lu\n", type.NativeThread);
    type.Active = mageThreadHandle(pthread_create(&type.NativeThread, NULL, info->Job, info->SubmitData));
    MAGE_ASSERT(type.Active != MAGE_TRUE);
    memcpy(thread, &type, sizeof(struct mageThreadType));
}
uint64_t mageThreadGetID(const mageThread thread)
{
    return (uint64_t)pthread_self();
}
void mageThreadEnd(mageThread thread)
{   
    MAGE_ASSERT(thread != NULL);
    struct mageThreadType type = (*(struct mageThreadType *)thread);
    if (!type.Active) return;

    uint8_t result;
    MAGE_LOG_CORE_INFORM("Ending pthread thread of id %lu\n", type.NativeThread);
    mageThreadHandle(pthread_join(type.NativeThread, (void **)&result));
    
    memcpy(thread, &type, sizeof(struct mageThreadType));
}
void mageThreadTerminate(mageThread thread)
{
    struct mageThreadType type = (*(struct mageThreadType *)thread);
    
    MAGE_LOG_CORE_WARNING("Terminating pthread of id %lu, thread may have not completed\n", type.NativeThread);
    pthread_cancel(type.NativeThread);
    
    memcpy(thread, &type, sizeof(struct mageThreadType));    
}

#endif