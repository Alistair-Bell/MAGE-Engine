#include <Mage.h>

I32 main()
{
    MageEngineApplication engineContext;
    
    MageApplicationWindowCreateInfo windowCreateInfo;
    memset(&windowCreateInfo, 0, sizeof(MageApplicationWindowCreateInfo));
    windowCreateInfo.Title = "Hello World";
    windowCreateInfo.Height = 1080;
    windowCreateInfo.Width  = 1920;

    MageInputHandlerCreateInfo inputCreateInfo;
    memset(&inputCreateInfo, 0, sizeof(MageInputHandlerCreateInfo));
    inputCreateInfo.InputFlags    = MAGE_INPUT_HANDLER_EVENT_LISTEN_FLAGS_ALL_FLAGS;
    inputCreateInfo.JoystickFlags = MAGE_INPUT_HANDLER_EVENT_JOYSTICK_LISTEN_FLAGS_ALL_FLAGS;

    MageEngineApplicationCreateInfo engineCreateInfo;
    memset(&engineCreateInfo, 0, sizeof(MageEngineApplicationCreateInfo));
    engineCreateInfo.ApplicationWindowCreateInfo = windowCreateInfo;
    engineCreateInfo.InputEventHandlerCreateInfo = inputCreateInfo;

    MageEngineApplicationCreate(&engineCreateInfo, &engineContext);

    XEvent *e = &engineContext.Window->PollingEvent;
    while (1)
    {
        XNextEvent(engineContext.Window->WindowDisplay, e);
    
        if (e->type == KeyPress)
        {
            printf("Log: X11 Keycode : %d\n", e->xkey.keycode);
            break;   
        }
    }


    MageEngineApplicationDestroy(&engineContext);
    printf("Ran successfully\n");


    return MageTrue;
}