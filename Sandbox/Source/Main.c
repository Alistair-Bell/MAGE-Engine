#include <Mage.h>

I32 main()
{
    MageFileSystem system;

    MageFileSystemCreateInfo ci;
    memset(&ci, 0, sizeof(MageFileSystemCreateInfo));

    MageFileSystemMountInfo mi;
    U32 index = 0;
    memset(&mi, 0, sizeof(MageFileSystemMountInfo));
    mi.MountIndex = &index;
    mi.MountPoint = "SharedResources";

    MageFileSystemReadInfo ri;
    memset(&ri, 0, sizeof(MageFileSystemCreateInfo));
    ri.FilePath = "Shaders/Colored/Fragment.frag";
    ri.MountPointIndex = index;
    ri.SearchOverride  = MageTrue;

    MageFileSystemCreate(&ci, &system);
    MageFileSystemMountDirectory(&system, &mi);
    MageFileSystemReadMountedDirectory(&system, &ri);
    MageFileSystemUnmountDirectory(&system, "SharedResources");
    MageFileSystemDestory(&system);
    
    free(ri.StreamData);


    MageEngineApplication engineContext;
   
    MageRendererCreateInfo rendererCreateInfo;
    memset(&rendererCreateInfo, 0, sizeof(MageRendererCreateInfo));

    MageApplicationWindowCreateInfo windowCreateInfo;
    memset(&windowCreateInfo, 0, sizeof(MageApplicationWindowCreateInfo));
    windowCreateInfo.Width              = 1920;
    windowCreateInfo.Height             = 1080;
    windowCreateInfo.Flags              = MAGE_APPLICATION_WINDOW_CREATE_FLAGS_ALLOW_RESIZING;
    
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

    while (MageInputHandlerPollEvents(engineContext.InputHandler, engineContext.Window)); 
    MageEngineApplicationDestroy(&engineContext);

    printf("Inform: Ran successfully\n");
    return MageTrue;
}
