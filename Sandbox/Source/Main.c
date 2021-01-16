#include <Mage.h>

static U8 GetAssestsMountPoint(char *buffer, U32 len)
{
    char localBuffer[255];
    U8 r = MageFileSystemGetUserHomeDirectroy(localBuffer, 213);
    MAGE_HANDLE_ERROR_MESSAGE(!r, );
    sprintf(buffer, "%s/.local/MageEngine/SharedResources/Shaders", localBuffer);
    return r;
}

I32 main(I32 argc, const char **args)
{
    char buff[255];
    GetAssestsMountPoint(buff, 255);

    MageEngineApplication engineContext;
    
    MageFileSystem system;
    U32 mntIndex;

    MageFileSystemCreateInfo fileCreateInfo;
    memset(&fileCreateInfo, 0, sizeof(MageFileSystemCreateInfo));
    MageFileSystemCreate(&fileCreateInfo, &system);
   
    MageFileSystemMountInfo mountInfo;
    mountInfo.MountPoint = buff;
    mountInfo.MountIndex = &mntIndex;
    MageFileSystemMountDirectory(&mountInfo, &system);

    MageShaderCreateInfo vertex;
    memset(&vertex, 0, sizeof(MageShaderCreateInfo));
    vertex.MountedFileSystem = system;
    vertex.EntryPoint        = "main";
    vertex.Type              = MAGE_SHADER_TYPE_VERTEX;
    vertex.ReadInfo          = (MageFileSystemReadInfo) { .FilePath = "HardCoded/Vertex.vert.sprv", .ReadMode = MAGE_FILE_SYSTEM_READ_MODE_BINARY };

    MageShaderCreateInfo fragment;
    memset(&fragment, 0, sizeof(MageShaderCreateInfo));
    fragment.MountedFileSystem = system;
    fragment.EntryPoint        = "main";
    fragment.Type              = MAGE_SHADER_TYPE_FRAGMENT;
    fragment.ReadInfo          = (MageFileSystemReadInfo) { .FilePath = "HardCoded/Fragment.frag.sprv", .ReadMode = MAGE_FILE_SYSTEM_READ_MODE_BINARY };

    MageRendererCreateInfo rendererCreateInfo;
    memset(&rendererCreateInfo, 0, sizeof(MageRendererCreateInfo));
    rendererCreateInfo.PipelineShadersInfo   = (MageShaderCreateInfo[]) { fragment, vertex };
    rendererCreateInfo.PipelineShaderCount   = 2;
    rendererCreateInfo.ConcurrentThreadCount = 2; /* Active frames to render */

    MageApplicationWindowCreateInfo windowCreateInfo;
    memset(&windowCreateInfo, 0, sizeof(MageApplicationWindowCreateInfo));
    windowCreateInfo.Width              = 1920;
    windowCreateInfo.Height             = 1080;
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
    
    U32 i;
    for (i = 0; i < engineContext.Renderer->CommandRecorders.ResidentCount; i++)
        MageRendererRecordHardCoded(engineContext.Renderer, i);

    MageEngineApplicationStart(&engineContext);

    MageEngineApplicationDestroy(&engineContext);
    MageFileSystemDestory(&system);

    printf("Inform: Ran successfully\n");
    return MageTrue;
}
