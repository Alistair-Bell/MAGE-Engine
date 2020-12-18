#include <Mage.h>

I32 main()
{
    MageEngineApplication engineContext;

    MageRendererSurfaceCreateInfo surfaceCreateInfo;
    surfaceCreateInfo.Flags = MAGE_RENDERER_SURFACE_FLAGS_USE_NATIVE_WINDOW_SIZE;
    
    MageRendererCreateInfo rendererCreateInfo;
    memset(&rendererCreateInfo, 0, sizeof(MageRendererCreateInfo));
    rendererCreateInfo.SurfaceCreateInfo = surfaceCreateInfo;
    
    MageApplicationWindowCreateInfo windowCreateInfo;
    memset(&windowCreateInfo, 0, sizeof(MageApplicationWindowCreateInfo));
    windowCreateInfo.Height = 1080;
    windowCreateInfo.Width  = 1920;

    MageInputHandlerCreateInfo inputCreateInfo;
    memset(&inputCreateInfo, 0, sizeof(MageInputHandlerCreateInfo));
    inputCreateInfo.InputFlags    = MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_ALL_FLAGS;
    inputCreateInfo.JoystickFlags = MAGE_INPUT_HANDLER_EVENT_JOYSTICK_LISTEN_FLAGS_ALL_FLAGS;

    MageEngineApplicationCreateInfo engineCreateInfo;
    memset(&engineCreateInfo, 0, sizeof(MageEngineApplicationCreateInfo));
    engineCreateInfo.ProgramName                 = "Sandbox Application";
    engineCreateInfo.ApplicationWindowCreateInfo = windowCreateInfo;
    engineCreateInfo.InputEventHandlerCreateInfo = inputCreateInfo;
    engineCreateInfo.RendererCreateInfo          = rendererCreateInfo;

    MageEngineApplicationCreate(&engineCreateInfo, &engineContext);
    
    while (MageInputHandlerPollEvents(engineContext.InputHandler, engineContext.Window)) { }

    MageEngineApplicationDestroy(&engineContext);
    printf("Inform: Ran successfully\n");
    return MageTrue;
}