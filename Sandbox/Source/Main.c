#include "Core.h"

/* Application instance */
static struct mageApplication *SandboxApplication;
static struct mageShader shaders[2];
static struct mageScene currentScene;
static struct mageRenderable renderable;

typedef struct
{
    uint32_t data;
} foo; 

void CreateShaders()
{
    mageShaderCreate(&shaders[0], "Mage/Resources/Shaders/fragment.sprv", "main", MAGE_SHADER_TYPE_FRAGMENT);
    mageShaderCreate(&shaders[1], "Mage/Resources/Shaders/vertex.sprv", "main", MAGE_SHADER_TYPE_VERTEX);
}

MAGE_ENTRY_POINT()
{
    SandboxApplication = malloc(sizeof(struct mageApplication));
    mageLogInitialise("Logs/mage.log");
#if 0
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
    
    mageApplicationCreate(SandboxApplication, applicationCreateInfo, rendererCreateInfo);
    mageRenderableCreate(&renderable, MAGE_RENDERABLE_PIPELINE_MODE_PRIMARY, SandboxApplication->Renderer);


    while (!(glfwWindowShouldClose(SandboxApplication->Window->Context)))
    {
        glfwPollEvents();
        mageRendererDraw(SandboxApplication->Renderer, &renderable);
        
    }
    vkDeviceWaitIdle(SandboxApplication->Renderer->Device);
    mageRenderableDestroy(&renderable, SandboxApplication->Renderer);
    mageApplicationDestroy(SandboxApplication);    


#endif
    struct mageHeapAllocater allocater = mageHeapAllocaterDefault();
    mageSceneCreate(&currentScene, 256, "Current Scene", &allocater);
    mageEntity e = mageEntityCreate(&currentScene);
    
    
    
    
    MAGE_ECS_REGISTER_COMPONENT(&currentScene, struct mageTexture);
    MAGE_ECS_REGISTER_COMPONENT(&currentScene, struct mageRenderable);
    MAGE_ECS_REGISTER_COMPONENT(&currentScene, struct mageRenderer);

    
    
    


    mageEntityDestroy(&currentScene, e);
    mageSceneDestroy(&currentScene);
    
    free(SandboxApplication);
    mageLogEnd();
    return 1;
}

