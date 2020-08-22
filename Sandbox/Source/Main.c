#include "Core.h"

#define SANDBOX_ENTITY_COUNT 5

/* Application instance */
static struct mageApplication   *SandboxApplication;
static struct mageShader        shaders[2];
static struct mageRenderable    renderable, renderable2;


void CreateShaders()
{
    mageShaderCreate(&shaders[0], "Mage/Resources/Shaders/fragment.sprv", "main", MAGE_SHADER_TYPE_FRAGMENT);
    mageShaderCreate(&shaders[1], "Mage/Resources/Shaders/vertex.sprv", "main", MAGE_SHADER_TYPE_VERTEX);
}
void exampleConstructer(void *data, const uint32_t size)
{
    struct mageTransform *t = ((struct mageTransform *)data);
    
}
void exampleDeconstructer(void *data)
{
    
}
void exampleListener(void *package, mageEventType type)
{
    if (type == MAGE_EVENT_KEY_PRESSED)
    {
    
    }
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

    mageEventRegisterListener(exampleListener);
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
    info.TextureCreateInfo  = (struct mageTextureCreateInfo) { .SamplerMode = MAGE_TEXTURE_SAMPLER_MIRRORED_REPEAT, .TexturePath = "Mage/Resources/Textures/MTEC/Logo.png" };
    mageRenderableCreate(&renderable, &info, SandboxApplication->Renderer);

    struct mageRenderable *r[] = { &renderable };

    while (!(glfwWindowShouldClose(SandboxApplication->Window->Context)))
    {
        mageRendererDrawRenderables(SandboxApplication->Renderer, r, 1);
        glfwPollEvents();
    }
    vkDeviceWaitIdle(SandboxApplication->Renderer->Device);
    mageRenderableDestroy(&renderable, SandboxApplication->Renderer);
    mageApplicationDestroy(SandboxApplication);

#endif
    struct mageScene s;
    struct mageSceneCreateInfo i;
    i.ComponentLimit                = 10;
    i.EntityLimit                   = 10;
    i.SceneTag                      = "Hello World";
    i.RegisterDefaultComponents     = MAGE_FALSE;
    mageSceneCreate(&s, &i);
    
    struct mageTransform t;
    memset(&t, 0, sizeof(struct mageTransform));

    struct mageVector3 v;

    uint32_t transform              = MAGE_ECS_REGISTER_COMPONENT(&s, struct mageTransform, NULL, NULL, MAGE_COMPONENT_REGISTERING_MODE_OPTIONAL);
    uint32_t vector3                = MAGE_ECS_REGISTER_COMPONENT(&s, struct mageVector3, NULL, NULL, MAGE_COMPONENT_REGISTERING_MODE_OPTIONAL);
    mageEntity civillian            = mageSceneEntityCreate(&s);
    mageEntity soldier              = mageSceneEntityCreate(&s);
    

    struct mageComponentHandle h1   = MAGE_ECS_BIND_NEW_COMPONENT_BY_TAG_TO_ENTITIES(&s, struct mageVector3, &v, &civillian,   1);
    struct mageComponentHandle h2   = MAGE_ECS_BIND_NEW_COMPONENT_BY_TAG_TO_ENTITIES(&s, struct mageTransform, &t, &civillian, 1);

    MAGE_ECS_BIND_EXISTING_COMPONENT_TO_ENTITIES(&s, h1, &soldier, 1);


    mageSceneDestroy(&s);
    free(SandboxApplication);
    mageLogEnd();
    return 1;
}

        
