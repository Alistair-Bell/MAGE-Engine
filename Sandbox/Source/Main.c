#include <Mage.h>

I32 main(I32 argc, const char **args)
{
    MageEngineApplication engineContext;
   
    MageFileSystem system;
    U32 mntPoint;
    MageFileSystemCreate(&(MageFileSystemCreateInfo) {}, &system);

    MageFileSystemMountInfo mountInfo;
    mountInfo.MountPoint = "SharedResources/Shaders";
    mountInfo.MountIndex = &mntPoint;
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

    MageFileSystemDestory(&system);
    MageEngineApplicationDestroy(&engineContext);

    printf("Inform: Ran successfully\n");
    return MageTrue;
}
