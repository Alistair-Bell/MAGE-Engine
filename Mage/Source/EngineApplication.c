#include "EngineApplication.h"

U8 MageEngineApplicationCreate(MageEngineApplicationCreateInfo *info, MageEngineApplication *engine)
{
    memset(engine, 0, sizeof(MageEngineApplication));
    MAGE_HANDLE_ERROR_MESSAGE(info == NULL || engine == NULL, printf("Error: Any passed parameters are NULL!\n"));   
    engine->Window = (MageApplicationWindow *)malloc(sizeof(MageApplicationWindow));
    
    MAGE_HANDLE_ERROR_MESSAGE(MageApplicationWindowCreate(&info->ApplicationWindowCreateInfo, engine->Window), )
    printf("Log: Created engine\n");
    return MageTrue;
}
U8 MageEngineApplicationDestroy(MageEngineApplication *engine)
{
    MageApplicationWindowDestroy(engine->Window);
    free(engine->Window);
    return MageTrue;
}