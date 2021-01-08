#include "../Threading.h"

U8 MageThreadCreate(MageThreadCreateInfo *info, MageThread *thread)
{
    return (U8)!(pthread_create(&thread->Identifier, NULL, info->Callback, info->CallbackData)); /* pthread uses 0 as success, how annoying */
}
U8 MageThreadEnd(MageThread *thread)
{
    pthread_cancel(thread->Identifier); 
    return MageTrue; 
}
U8 MageThreadDestroy(MageThread *thread)
{
    void *result = 0;
    pthread_join(thread->Identifier, (U0 **)&result);
    return (result == MAGE_THREAD_RETURN_SUCCESS); /* Returns the threads status code */
}
