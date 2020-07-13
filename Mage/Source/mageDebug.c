#include "mageAPI.h"

struct mageLoger 
{   
    uint64_t LogCount;
    uint64_t WarningCount;
    uint64_t ErrorCount;
    uint64_t FatalErrorCount;
    time_t Timer;
    struct tm *Time;
    FILE *DebugFile;
    const char *OuputName;

};

static struct mageLoger Logger;



static void mageGLFWErrorCallback(int error, const char *description)
{
    MAGE_LOG_CORE_ERROR("GLFW Error %d : %s\n", error, description);
}

void mageLogInitialise(const char *outputFile)
{
    #if defined (MAGE_DEBUG) 
        Logger.OuputName = outputFile;
        Logger.WarningCount = 0;
        Logger.ErrorCount = 0;   
        Logger.DebugFile = fopen(Logger.OuputName, "a");
        glfwSetErrorCallback(mageGLFWErrorCallback);
    #endif
}
void mageLogMessage(const mageLogUser user, const mageLogMode severity, const char *format, ...)
{   
    const char *colors[] = 
    {
        "\x1b[32m",
        "\x1b[33m",
        "\x1b[34m",
        "\x1b[36m"
    };


    switch (severity)
	{
	case MAGE_LOG_MODE_WARNING: 
		Logger.WarningCount++;
		break;
	case MAGE_LOG_MODE_ERROR: 
		Logger.ErrorCount++;
        break;
	case MAGE_LOG_MODE_FATAL_ERROR: 
		Logger.ErrorCount++; 
		break;
    default:
        break;
    }
    printf("%s", colors[severity]);

    time(&Logger.Timer);
    Logger.Time = localtime(&Logger.Timer);

    
    char timeFormat[15];
    char userFormat[1024];
    char finalFormat[1024];

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
	MAGE_LOG_CORE_INFORM("Logging Details:\nLog Count         -> %d\nWarning Count     -> %d\nError Count       -> %d\nFatal Error Count -> %d\n", Logger.LogCount, Logger.WarningCount, Logger.ErrorCount, Logger.FatalErrorCount);

    #if defined (MAGE_DEBUG)

        fclose(Logger.DebugFile);
        
        printf("%s", "\x1b[0m");
    #endif
}

