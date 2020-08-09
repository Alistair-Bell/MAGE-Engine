#include "Core.h"

#define SANDBOX_ENTITY_COUNT 5

/* Application instance */
static struct mageApplication *SandboxApplication;
static struct mageShader shaders[2];
static struct mageRenderable renderable, renderable2;


void CreateShaders()
{
    mageShaderCreate(&shaders[0], "Mage/Resources/Shaders/fragment.sprv", "main", MAGE_SHADER_TYPE_FRAGMENT);
    mageShaderCreate(&shaders[1], "Mage/Resources/Shaders/vertex.sprv", "main", MAGE_SHADER_TYPE_VERTEX);
}

uint8_t ExampleSystem(void **components, const uint32_t count)
{
    struct mageRenderable r = *((struct mageRenderable *) components[0]);
    return MAGE_TRUE;
}

    MAGE_ENTRY_POINT()
{
    mageLogInitialise("Logs/mage.log");
    SandboxApplication = malloc(sizeof(struct mageApplication));
#if 0
    CreateShaders();    

    struct mageApplicationCreateInfo applicationCreateInfo;
    memset(&applicationCreateInfo, 0, sizeof(struct mageApplicationCreateInfo));

    applicationCreateInfo.FixedResolution        = 0;
    applicationCreateInfo.Fullscreen             = 0;
    applicationCreateInfo.Width                  = 1920;
    applicationCreateInfo.Height                 = 1080;
    applicationCreateInfo.Name                   = "Sandbox Application";
    applicationCreateInfo.WindowIcon             = "Mage/Resources/Textures/Vulkan/Vulkan_LogoBug_48px_Nov17.png";

    struct mageRendererCreateInfo rendererCreateInfo;
    memset(&rendererCreateInfo, 0, sizeof(struct mageRendererCreateInfo));

    rendererCreateInfo.PipelineShaders          = shaders;
    rendererCreateInfo.ShaderCount              = sizeof(shaders) / sizeof(struct mageShader);
    rendererCreateInfo.TextureTransparency      = MAGE_TRUE;

    mageApplicationCreate(SandboxApplication, applicationCreateInfo, rendererCreateInfo);
    struct mageVertex verticies1[] = 
    {
        { .Vertex = { .X = -1.0f, .Y = -1.00f },   .Color = { .X = 1.0f, .Y = 0.0f, .Z = 0.0f}, .TextureLocation = { .X = 0.0f, .Y = 0.0f } },  
        { .Vertex = { .X =  1.0f, .Y = -1.00f },   .Color = { .X = 0.0f, .Y = 1.0f, .Z = 0.0f}, .TextureLocation = { .X = 1.0f, .Y = 0.0f } },  
        { .Vertex = { .X =  1.0f, .Y =  1.00f },   .Color = { .X = 0.0f, .Y = 0.0f, .Z = 1.0f}, .TextureLocation = { .X = 1.0f, .Y = 1.0f } },  
        { .Vertex = { .X = -1.0f, .Y =  1.00f },   .Color = { .X = 0.0f, .Y = 0.0f, .Z = 1.0f}, .TextureLocation = { .X = 0.0f, .Y = 1.0f } }
    
    };

    
    struct mageRenderableCreateInfo info;
    memset(&info, 0, sizeof(struct mageRenderableCreateInfo));
    info.Verticies          = verticies1;
    info.VertexCount        = 4;
    info.IndexCount         = 0;
    info.Indicies           = NULL;
    info.TextureCreateInfo  = (struct mageTextureCreateInfo) { .SamplerMode = MAGE_TEXTURE_SAMPLER_MIRRORED_REPEAT, .TexturePath = "Sandbox/Resources/Textures/tango.jpg" };
    mageRenderableCreate(&renderable, &info, SandboxApplication->Renderer);

    struct mageRenderable *r[] = { &renderable };

    while (!(glfwWindowShouldClose(SandboxApplication->Window->Context)))
    {
        glfwPollEvents();
        mageRendererDrawRenderables(SandboxApplication->Renderer, r, 1);
    }
    vkDeviceWaitIdle(SandboxApplication->Renderer->Device);
    
    mageRenderableDestroy(&renderable, SandboxApplication->Renderer);
    mageApplicationDestroy(SandboxApplication);    
#endif
    
    struct mageScene scene;
    struct mageSceneCreateInfo info;
    memset(&info, 0, sizeof(struct mageSceneCreateInfo));

    info.Allocater      = mageHeapAllocaterDefault();
    info.EntityLimit    = 12;
    info.SceneTag       = "Hello World";

    mageSceneCreate(&scene, &info);
    MAGE_ECS_REGISTER_COMPONENT(&scene, struct mageRenderable);
    MAGE_ECS_REGISTER_COMPONENT(&scene, struct mageTexture);

    SANDBOX_LOG_CORE_WARNING("First component id -> %d\n", scene.Pool.ComponentTables[0].ID);

    MAGE_ECS_REGISTER_SYSTEM(&scene, ExampleSystem, MAGE_SYSTEM_TYPE_FIXED_UPDATE, MAGE_SYSTEM_THREAD_PRIORITY_FORCE, 2, struct mageRenderable, struct mageTexture);
    mageSceneDestroy(&scene);
    free(SandboxApplication);
    mageLogEnd();
    return 1;
}

        
