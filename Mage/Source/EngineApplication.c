#include "EngineApplication.h"

#define MAGE_CHECK_PRINT_EXIT(condition) MAGE_HANDLE_ERROR_MESSAGE(condition, printf("Error: Failed to create engine\n"))

static MAGE_THREAD_RETURN_VALUE FixedUpdateLoop(U0 *data) /* Runs at a locked framerate */
{
    MageEngineApplication *e = ((MageEngineApplication *)data);
    MageApplicationWindow *w = e->Window;
    MageInputHandler      *i = e->InputHandler;
    while (e->Running)
    {
        e->Running = MageInputHandlerPollEvents(i, w); 
    }
    return MAGE_THREAD_RETURN_SUCCESS;
}

U8 MageEngineApplicationCreate(MageEngineApplicationCreateInfo *info, MageEngineApplication *engine)
{
    memset(engine, 0, sizeof(MageEngineApplication));
    MAGE_HANDLE_ERROR_MESSAGE(info == NULL || engine == NULL, printf("Error: Any passed parameters are NULL!\n"));
    engine->Window          = (MageApplicationWindow *)malloc(sizeof(MageApplicationWindow));
    engine->InputHandler    = (MageInputHandler *)malloc(sizeof(MageInputHandler));
    engine->Renderer        = (MageRenderer *)malloc(sizeof(MageRenderer));

    if (info->ApplicationWindowCreateInfo.Title == NULL) info->ApplicationWindowCreateInfo.Title = info->ProgramName;
    U8 currentResult = MageApplicationWindowCreate(&info->ApplicationWindowCreateInfo, engine->Window);
    MAGE_CHECK_PRINT_EXIT(!currentResult);
    
    info->InputEventHandlerCreateInfo.ApplicationWindow = engine->Window;
    currentResult = MageInputHandlerCreate(&info->InputEventHandlerCreateInfo, engine->InputHandler);
    MAGE_CHECK_PRINT_EXIT(!currentResult)
    
    info->RendererCreateInfo.Window = engine->Window;
    currentResult = MageRendererCreate(&info->RendererCreateInfo, engine->Renderer);
    MAGE_CHECK_PRINT_EXIT(!currentResult)

    engine->Running = MageTrue;

    printf("Inform: Created engine\n");
    return currentResult;
}
U8 MageEngineApplicationStart(MageEngineApplication *engine)
{
    /* Fixed loop thread */
    MageThread fixedThread;

    MageThreadCreateInfo fixedThreadInfo;
    memset(&fixedThreadInfo, 0, sizeof(MageThreadCreateInfo));
    fixedThreadInfo.Callback       = FixedUpdateLoop;
    fixedThreadInfo.CallbackData   = (U0 *)engine;

    U8 result = MageThreadCreate(&fixedThreadInfo, &fixedThread);
    MAGE_HANDLE_ERROR_MESSAGE(result != MageTrue, printf("Error: Failed to create fixed update loop thread\n"));
    while (engine->Running)
    {
        MageRendererPresentRecorded(engine->Renderer);
    }


    return MageTrue;
}
U8 MageEngineApplicationDestroy(MageEngineApplication *engine)
{
    MageInputHandlerDestroy(engine->InputHandler);
    MageRendererDestroy(engine->Renderer);
    MageApplicationWindowDestroy(engine->Window);
    free(engine->Window);
    free(engine->InputHandler);
    free(engine->Renderer);
    return MageTrue;
}
