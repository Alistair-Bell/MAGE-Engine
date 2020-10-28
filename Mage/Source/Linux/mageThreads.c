#include <mageAPI.h>

#if defined (MAGE_PLATFORM_LINUX)

static uint8_t mageThreadHandle(int32_t error)
{
    if (error)
    {
        MAGE_LOG_CORE_ERROR("PThread error code: %d\n", error);
        return 0;
    }
    return 1;
}

void mageThreadCreate(struct mageThread *thread, struct mageThreadCreateInfo *info)
{
   memset(thread, 0, sizeof(struct mageThread));
}
void mageThreadBegin(struct mageThread *thread, struct mageThreadBeginInfo *info)
{
    /* Creating thread */
    mageThreadHandle(pthread_create(&thread->Native, NULL, info->Job, info->SubmitData));
    MAGE_LOG_CORE_INFORM("Created pthread of id %lu\n", thread->Native);
}
uint64_t mageThreadGetID(const struct mageThread *thread)
{
    return (uint64_t)thread->Native;
}
void mageThreadEnd(struct mageThread *thread)
{   
    MAGE_ASSERT(thread != NULL);
    uint8_t result;
    MAGE_LOG_CORE_INFORM("Ending pthread thread of id %lu\n", thread->Native);
    mageThreadHandle(pthread_join(thread->Native, (void **)&result));
}
void mageThreadTerminate(struct mageThread *thread)
{
    MAGE_LOG_CORE_WARNING("Terminating pthread of id %lu, thread may have not completed\n", thread->Native);
    pthread_cancel(thread->Native);
}

#endif