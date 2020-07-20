#include <mageAPI.h>

/* Application instance */
static struct mageApplication *SandboxApplication;
static struct mageShader shaders[2];
static struct mageScene currentScene;
static struct mageRenderable renderable;

void CreateShaders()
{
    mageShaderCreate(&shaders[0], "Mage/Resources/Shaders/fragment.sprv", "main", MAGE_SHADER_TYPE_FRAGMENT);
    mageShaderCreate(&shaders[1], "Mage/Resources/Shaders/vertex.sprv", "main", MAGE_SHADER_TYPE_VERTEX);
}

int32_t main(const int32_t argumentCount, char **arguments)
{
    SandboxApplication = malloc(sizeof(struct mageApplication));
    mageLogInitialise("Logs/mage.log");
    CreateShaders();    

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
    rendererCreateInfo.Renderable               = &renderable;
    
    mageApplicationCreate(SandboxApplication, applicationCreateInfo, rendererCreateInfo);
    
    while (!(glfwWindowShouldClose(SandboxApplication->Window->Context)))
    {
        glfwPollEvents();
        /* mageRendererSubmit(SandboxApplication->Renderer, NULL); */
        mageRendererRender(SandboxApplication->Renderer);    
    }
    mageApplicationDestroy(SandboxApplication);


    /*
    mageSceneCreate(&currentScene, "Current Scene", NULL);
    uint32_t i;
    mageSceneDestroy(&currentScene);
    */
    free(SandboxApplication);
    mageLogEnd();
    return 1;
}
