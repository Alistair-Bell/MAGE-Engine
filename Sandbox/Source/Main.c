#include <mageAPI.h>

/* Application instance */
static struct mageApplication *SandboxApplication;
static struct mageShader shaders[2];

void CreateShaders()
{
    mageShaderCreate(&shaders[0], "Mage/Resources/Shaders/fragment.sprv", "main", MAGE_SHADER_TYPE_FRAGMENT);
    mageShaderCreate(&shaders[1], "Mage/Resources/Shaders/vertex.sprv", "main", MAGE_SHADER_TYPE_VERTEX);
}

int32_t main(const int32_t argumentCount, char **arguments)
{
    mageLogInitialise("Logs/mage.log");
    CreateShaders();    
    SandboxApplication = malloc(sizeof(struct mageApplication));

    struct mageApplicationCreateInfo applicationCreateInfo;
    memset(&applicationCreateInfo, 0, sizeof(struct mageApplicationCreateInfo));

    applicationCreateInfo.FixedResolution        = 0;
    applicationCreateInfo.Fullscreen             = 0;
    applicationCreateInfo.Width                  = 1920;
    applicationCreateInfo.Height                 = 1080;
    applicationCreateInfo.Name                   = "Sandbox Application";
    applicationCreateInfo.WindowIcon             = "Mage/Resources/Textures/MTEC/Logo.png";

    struct mageRendererCreateInfo rendererCreateInfo;
    memset(&rendererCreateInfo, 0, sizeof(struct mageRendererCreateInfo));

    rendererCreateInfo.PipelineShaders          = shaders;
    rendererCreateInfo.ShaderCount              = sizeof(shaders) / sizeof(struct mageShader);
    if (mageApplicationCreate(SandboxApplication, applicationCreateInfo, rendererCreateInfo) != MAGE_RESULT_SUCCESS)
    {
        return -1;
    }
    
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
