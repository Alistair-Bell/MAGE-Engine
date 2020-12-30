#include <Mage.h>

#if MAGE_BUILD_PLATFORM_WINDOWS
    const char *mntPoint = "../../SharedResources/Shaders";
#else
    const char *mntPoint = "SharedResources/Shaders";
#endif

I32 main(I32 argc, const char **args)
{
    MageEngineApplication engineContext;
   
    MageFileSystem system;
    U32 mntIndex;
    MageFileSystemCreateInfo fileCreateInfo;
    memset(&fileCreateInfo, 0, sizeof(MageFileSystemCreateInfo));
    MageFileSystemCreate(&fileCreateInfo, &system);
   
    MageFileSystemMountInfo mountInfo;
    mountInfo.MountPoint = mntPoint;
    mountInfo.MountIndex = &mntIndex;
    MageFileSystemMountDirectory(&mountInfo, &system);

    MageShaderCreateInfo vertex;
    memset(&vertex, 0, sizeof(MageShaderCreateInfo));
    vertex.MountedFileSystem = system;
    vertex.EntryPoint        = "main";
    vertex.Type              = MAGE_SHADER_TYPE_VERTEX;
    vertex.ReadInfo          = (MageFileSystemReadInfo) { .FilePath = "HardCoded/Vertex.vert.sprv" };

    MageShaderCreateInfo fragment;
    memset(&fragment, 0, sizeof(MageShaderCreateInfo));
    fragment.MountedFileSystem = system;
    fragment.EntryPoint        = "main";
    fragment.Type              = MAGE_SHADER_TYPE_VERTEX;
    fragment.ReadInfo          = (MageFileSystemReadInfo) { .FilePath = "HardCoded/Fragment.frag.sprv" };
    
    MageRendererCreateInfo rendererCreateInfo;
    memset(&rendererCreateInfo, 0, sizeof(MageRendererCreateInfo));
    rendererCreateInfo.PipelineShaderCount = 2;
    rendererCreateInfo.PipelineShadersInfo = (MageShaderCreateInfo[]) { fragment, vertex };

    MageApplicationWindowCreateInfo windowCreateInfo;
    memset(&windowCreateInfo, 0, sizeof(MageApplicationWindowCreateInfo));
    windowCreateInfo.Width              = 1080;
    windowCreateInfo.Height             = 720;
    windowCreateInfo.Flags              = MAGE_APPLICATION_WINDOW_CREATE_FLAGS_ALLOW_RESIZING | MAGE_APPLICATION_WINDOW_CREATE_FLAGS_AUTO_CENTRE;
    
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

    MageApplicationWindowDimensions d;
    MageApplicationWindowGetDimensions(engineContext.Window, &d);
    while (MageInputHandlerPollEvents(engineContext.InputHandler, engineContext.Window)); 

    MageEngineApplicationDestroy(&engineContext);
    //MageFileSystemDestory(&system);

    printf("Inform: Ran successfully\n");
    return MageTrue;
}
