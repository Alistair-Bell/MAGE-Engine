#include <mageAPI.h>

/* Application instance */
static struct mageApplication *SandboxApplication;
static struct mageShader shaders[2];

void ExampleListener(void *event, mageEventType type)
{
    if (type == MAGE_EVENT_APPLICATION_RENDER)
    {
        
    }
}

int32_t main(const int32_t argumentCount, char **arguments)
{
    SandboxApplication = malloc(sizeof(struct mageApplication));

    
    mageLogInitialise("Logs/mage.log");

    struct mageApplicationCreateInfo applicationCreateInfo;
    memset(&applicationCreateInfo, 0, sizeof(struct mageApplicationCreateInfo));

    applicationCreateInfo.FixedResolution        = 0;
    applicationCreateInfo.Fullscreen             = 0;
    applicationCreateInfo.Width                  = 1920;
    applicationCreateInfo.Height                 = 1080;
    applicationCreateInfo.Name                   = "Sandbox Application";
    applicationCreateInfo.Version                = 0.0;
    applicationCreateInfo.WindowIcon             = "Mage/Resources/Textures/MTEC/Logo.png";

    struct mageRendererCreateInfo rendererCreateInfo;
    memset(&rendererCreateInfo, 0, sizeof(struct mageRendererCreateInfo));

    mageShaderInitialise(&shaders[0], "Mage/Resources/Shaders/fragment.sprv", "main", MAGE_SHADER_TYPE_FRAGMENT);
    mageShaderInitialise(&shaders[1], "Mage/Resources/Shaders/vertex.sprv", "main", MAGE_SHADER_TYPE_VERTEX);

    rendererCreateInfo.RuntimeShaders           = shaders;
    rendererCreateInfo.ShaderCount              = 2;
    
    mageApplicationInitialise(SandboxApplication, applicationCreateInfo, rendererCreateInfo);

    while (SandboxApplication->Running)
    {
        glfwPollEvents();
        mageRendererRender(SandboxApplication->Renderer);
        SandboxApplication->Running = !(glfwWindowShouldClose(SandboxApplication->Window->Context));
    }



    mageApplicationDestroy(SandboxApplication);
    free(SandboxApplication);

    mageLogEnd();
    return 1;
}