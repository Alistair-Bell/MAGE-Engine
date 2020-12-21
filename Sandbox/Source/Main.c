#include <Mage.h>

I32 main()
{
    MageEngineApplication engineContext;
   
    MageRendererCreateInfo rendererCreateInfo;
    memset(&rendererCreateInfo, 0, sizeof(MageRendererCreateInfo));
       
    MageApplicationWindowCreateInfo windowCreateInfo;
    memset(&windowCreateInfo, 0, sizeof(MageApplicationWindowCreateInfo));
    windowCreateInfo.Width              = 1920;
    windowCreateInfo.Height             = 1080;
    windowCreateInfo.Flags              = MAGE_APPLICATION_WINDOW_CREATE_FLAGS_AUTO_CENTRE;
    
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
    
    if (!MageEngineApplicationCreate(&engineCreateInfo, &engineContext)) return MageFalse;
    MageApplicationWindowSetTitle(engineContext.Window, "New Name Was Set");

    while (MageInputHandlerPollEvents(engineContext.InputHandler, engineContext.Window)); 
    MageEngineApplicationDestroy(&engineContext);

    printf("Inform: Ran successfully\n");
    return MageTrue;
}
