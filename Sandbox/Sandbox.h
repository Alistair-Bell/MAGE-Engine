#ifndef __SANDBOX__
#define __SANDBOX__

#include <mageAPI.h>

#if defined (SANDBOX_DEBUG)

#define LOG_CLIENT_INFORM(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_INFORM, x, __VA_ARGS__)
#define LOG_CLIENT_WARNING(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_WARNING, x, __VA_ARGS__)
#define LOG_CLIENT_ERROR(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_ERROR, x, __VA_ARGS__)
#define LOG_CLIENT_FATAL_ERROR(x, ...) mageLogMessage(MAGE_LOG_USER_CLIENT, MAGE_LOG_MODE_FATAL_ERROR, x, __VA_ARGS__)

#else

#define LOG_CLIENT_INFORM(x, ...)
#define LOG_CLIENT_WARNING(x, ...)
#define LOG_CLIENT_ERROR(x, ...)
#define LOG_CLIENT_FATAL_ERROR(x, ...)

#endif

#endif
