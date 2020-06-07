#include "mageAPI.h"

typedef struct MAGE_API MAGE_LOGING_STRUCT
{   
    uint64_t LogCount;
    uint64_t WarningCount;
    uint64_t ErrorCount;
    time_t Timer;
    struct tm *Time;
    FILE *DebugFile;
    const char *OuputName;

} mageLoger;

static mageLoger Logger;

void mageLogInitialise(const char *outputFile)
{
    Logger.OuputName = outputFile;
    Logger.WarningCount = 0;
    Logger.ErrorCount = 0;   
    Logger.DebugFile = fopen(Logger.OuputName, "a");
}
void mageLogMessage(const uint8_t user, const uint8_t severity, const char *format, ...)
{   
    switch (severity)
	{
	case MAGE_LOG_MODE_INFORM: 
		printf("%s", "\x1b[32m");
		break;
	case MAGE_LOG_MODE_WARNING: 
		Logger.WarningCount++;
        printf("%s", "\x1b[33m");
		break;
	case MAGE_LOG_MODE_ERROR: 
		Logger.ErrorCount++;
        printf("%s", "\x1b[34m");
        break;
	case MAGE_LOG_MODE_FATAL_ERROR: 
		Logger.ErrorCount++; 
		printf("%s", "\x1b[36m");
		break;
	}

    time(&Logger.Timer);
    Logger.Time = localtime(&Logger.Timer);

    
    char timeFormat[15];
    char userFormat[256];
    char finalFormat[270];

    sprintf(timeFormat, "%02d:%02d:%02d", Logger.Time->tm_hour, Logger.Time->tm_min, Logger.Time->tm_sec);
    
	va_list args;
    va_start(args, format);
    vsprintf(userFormat, format, args);
	va_end(args);
    
    switch (user)
    {
        case MAGE_LOG_USER_CORE:
            sprintf(finalFormat, "[%s] Engine: %s", timeFormat, userFormat);
            break;
        case MAGE_LOG_USER_CLIENT:
            sprintf(finalFormat, "[%s] Client: %s", timeFormat, userFormat);
            break;
    }
    
    printf("%s", finalFormat);
    fprintf(Logger.DebugFile, "%s", finalFormat);
    Logger.LogCount++;
}
void mageLogEnd()
{
	MAGE_LOG_CORE_INFORM("Logging Details:\nLog Count -> %d\nWarning Count -> %d\nError Count -> %d\n", Logger.LogCount, Logger.WarningCount, Logger.ErrorCount);

    #if defined(MAGE_DEBUG) || defined(CLIENT_DEBUG)

        fclose(Logger.DebugFile);
        
        printf("%s", "\x1b[0m");

    #endif
}

