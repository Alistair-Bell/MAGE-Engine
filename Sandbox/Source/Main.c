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

#if 0
    MageApplicationWindow *w = engineContext.Window;
    XEvent  *xe = &w->PollingEvent;
    Display *xd = w->WindowDisplay;
    Window  *xw = &w->ContextWindow;
    I32 x, y, cx, cy;
    U32 msk;
    Window root;
    while (1)
    {
        XNextEvent(xd, xe);
        XQueryPointer(xd, (*xw), &root, &w->RootWindow, &x, &y, &cx, &cy, &msk);
        switch (xe->type)
        {
            case KeyPress: printf("Log: X11 Keycode : %d\n", xe->xkey.keycode); goto end;
        }
    }
#endif

    while (MageInputHandlerPollEvents(engineContext.InputHandler, engineContext.Window)) { }

    end:
    {
        MageEngineApplicationDestroy(&engineContext);
        printf("Inform: Ran successfully\n");
    }


    return MageTrue;
}