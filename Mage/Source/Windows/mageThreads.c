#include <mageAPI.h>

#if defined (MAGE_PLATFORM_WINDOWS)


struct mageThreadType
{
    HANDLE      NativeThread;
    DWORD       ThreadID;
};

struct mageThreadType mageGetNative(const mageThread thread)
{
    return MAGE_VOID_POINTER_CAST(thread, struct mageThreadType);
}

mageThread mageThreadCreate(void)
{
    return MAGE_MEMORY_ALLOCATE(sizeof(struct mageThreadType));
}
void mageThreadBegin(mageThread thread, struct mageThreadBeginInfo *info)
{

    struct mageThreadType native = MAGE_VOID_POINTER_CAST(thread, struct mageThreadType);
    
    MAGE_ASSERT(info != NULL);
    HANDLE result = CreateThread(
        NULL, 
        0, 
        info->Job, 
        info->SubmitData, 
        (DWORD)info->ThreadFlags, 
        &native.ThreadID
    );
    
    MAGE_ASSERT_MESSAGE(result != NULL, "Win32 thread error %lu\n", GetLastError());
    MAGE_LOG_CORE_INFORM("Created Win32 thread of id %lu\n", native.ThreadID);
    
    /* Copy back*/
    memcpy(thread, &native, sizeof(struct mageThreadType));
}
uint64_t mageThreadGetID(const mageThread thread)
{
    return (MAGE_VOID_POINTER_CAST(thread, struct mageThreadType).ThreadID);
}
void mageThreadEnd(mageThread thread)
{
    struct mageThreadType native = MAGE_VOID_POINTER_CAST(thread, struct mageThreadType);
    
    MAGE_LOG_CORE_INFORM("Ending Win32 thread of id %lu\n", native.ThreadID);
    WaitForSingleObject(native.NativeThread, INFINITE);

    CloseHandle(native.NativeThread);
    /* Copy back*/
    memcpy(thread, &native, sizeof(struct mageThreadType));
}
void mageThreadTerminate(mageThread thread)
{
    struct mageThreadType native = mageGetNative(thread);

    MAGE_LOG_CORE_INFORM("Terminating Win32 thread of id %lu\n", native.ThreadID);
    DWORD result = TerminateThread(native.NativeThread, THREAD_TERMINATE);
    MAGE_ASSERT_MESSAGE(result == 0, "Unable to terminate thread, Win32 error %lu\n", GetLastError());
    CloseHandle(native.NativeThread);
    
    /* Copy back*/
    memcpy(thread, &native, sizeof(struct mageThreadType));
}


#endif