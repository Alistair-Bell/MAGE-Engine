#ifndef THREADING_H
#define THREADING_H

#include "Includes.h"
#include "CreateStructures.h"

#if MAGE_BUILD_PLATFORM_LINUX

typedef struct MageThread
{
    pthread_t   Identifier;
} MageThread;

#define MAGE_THREAD_RETURN_SUCCESS ((U0 *)MageTrue) 
#define MAGE_THREAD_RETURN_FAILURE ((U0 *)MageFalse)

#elif MAGE_BUILD_PLATFORM_WINDOWS
    
typedef struct MageThread
{
    DWORD       Identifier;
    HANDLE      Handle;
} MageThread;

#define MAGE_THREAD_RETURN_SUCCESS (DWORD)0 /* Love it when 0 and 1 are flipped around */
#define MAGE_THREAD_RETURN_FAILURE (DWORD)1 /* Love it when 0 and 1 are flipped around */

#else
    #error "Threading system has not been implimented"
#endif

extern U8 MageThreadCreate(MageThreadCreateInfo *info, MageThread *thread); /* Creating the thread instantly starts the thread */
extern U8 MageThreadEnd(MageThread *thread); /* Ends thread based on a code specified, results vary on platfoms */
extern U8 MageThreadDestroy(MageThread *thread); /* Exits normally, joins the threads */


#endif 
