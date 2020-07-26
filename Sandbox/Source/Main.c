#include "Core.h"

#define SANDBOX_ENTITY_COUNT 10

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
MAGE_ENTRY_POINT()
{
    SandboxApplication = malloc(sizeof(struct mageApplication));
    mageLogInitialise("Logs/mage.log");
    /*
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
    */
    
    
    struct mageHeapAllocater allocater = mageHeapAllocaterDefault();
    mageSceneCreate(&currentScene, SANDBOX_ENTITY_COUNT, "Current Scene", &allocater);
    /* Entities */
    mageEntity entities[SANDBOX_ENTITY_COUNT];
    
    /* 
        Component registering 
        For each scene components must be re registered, maybe in the future that may change
    */
    MAGE_ECS_REGISTER_COMPONENT(&currentScene, struct vector2);
    
    /* Binded data */ 
    struct vector2 data;
    data.v[0] = 0.0f;
    data.v[1] = 2.0f;

    uint32_t i;
    for (i = 0; i < SANDBOX_ENTITY_COUNT; i++)
    {
        /* Creating entities */
        entities[i] = mageEntityCreate(&currentScene);
        
        /* Binding entitiy data */
        MAGE_ECS_BIND_COMPONENT(&currentScene, entities[i], struct vector2, &data);
    }
    
    for (i = 0; i < SANDBOX_ENTITY_COUNT; i++)
    {
        /*
            Whilst destroying the entities is not required, it is good practice to cleanup after yourself
            If at runtime a entity is not needed then please destroy it too save on runtime memory
        */
        mageEntityDestroy(&currentScene, entities[i]);
    }

    /* Destroy the scene to free its allocated memory */
    mageSceneDestroy(&currentScene);
    
    
    free(SandboxApplication);
    mageLogEnd();
    return 1;
}

        