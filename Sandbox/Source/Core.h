#ifndef __SANDBOX_CORE__
#define __SANDBOX_CORE__

#include <mageAPI.h>

#if defined (SANDBOX_DEBUG)
    #define SANDBOX_LOG_CORE_INFORM(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_INFORM, x, __VA_ARGS__)
    #define SANDBOX_LOG_CORE_WARNING(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_WARNING, x, __VA_ARGS__)
    #define SANDBOX_LOG_CORE_ERROR(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_ERROR, x, __VA_ARGS__)
    #define SANDBOX_LOG_CORE_FATAL_ERROR(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_FATAL_ERROR, x, __VA_ARGS__)
#else
    #define SANDBOX_LOG_CORE_INFORM(x, ...)
    #define SANDBOX_LOG_CORE_WARNING(x, ...)
    #define SANDBOX_LOG_CORE_ERROR(x, ...)
    #define SANDBOX_LOG_CORE_FATAL_ERROR(x, ...)
#endif

#endif