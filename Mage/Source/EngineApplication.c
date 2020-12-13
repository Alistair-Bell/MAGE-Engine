#include "EngineApplication.h"

U8 MageEngineApplicationCreate(MageEngineApplicationCreateInfo *info, MageEngineApplication *engine)
{
    memset(engine, 0, sizeof(MageEngineApplication));
    MAGE_HANDLE_ERROR_MESSAGE(info == NULL || engine == NULL, printf("Error: Any passed parameters are NULL!\n"));   
    engine->Window          = (MageApplicationWindow *)malloc(sizeof(MageApplicationWindow));
    engine->InputHandler    = (MageInputHandler *)malloc(sizeof(MageInputHandler));
    
    U8 currentResult = MageApplicationWindowCreate(&info->ApplicationWindowCreateInfo, engine->Window);
    MAGE_HANDLE_ERROR_MESSAGE(!currentResult, printf("Error: Failed to create engine\n")); /* macros are wacky */
    
    info->InputEventHandlerCreateInfo.ApplicationWindow = engine->Window;
    currentResult = MageInputHandlerCreate(&info->InputEventHandlerCreateInfo, engine->InputHandler);

    printf("Log: Created engine\n");
    return currentResult;
}
U8 MageEngineApplicationDestroy(MageEngineApplication *engine)
{
    MageApplicationWindowDestroy(engine->Window);
    free(engine->Window);
    free(engine->InputHandler);
    return MageTrue;
}