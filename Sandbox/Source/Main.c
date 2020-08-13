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
    struct mageVector2 *v = (struct mageVector2 *)data;

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
#if 0
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
        mageRendererDrawRenderables(SandboxApplication->Renderer, r, 1);
        mageRenderableDestroy(&renderable, SandboxApplication->Renderer);

#endif
    while (!(glfwWindowShouldClose(SandboxApplication->Window->Context)))
    {
        glfwPollEvents();
    }
    vkDeviceWaitIdle(SandboxApplication->Renderer->Device);
    
    mageApplicationDestroy(SandboxApplication);    

    struct mageScene s;
    struct mageSceneCreateInfo i;
    struct mageHeapAllocater h = mageHeapAllocaterDefault();
    i.Allocater         = &h;
    i.ComponentLimit    = UINT32_MAX;
    i.EntityLimit       = UINT32_MAX;
    i.SceneTag          = "Hello World";
    
    mageSceneCreate(&s, &i);

    MAGE_ECS_REGISTER_COMPONENT(&s, struct mageVector2, exampleConstructer, exampleDeconstructer, MAGE_COMPONENT_REGISTERING_MODE_REQUIRED);
    MAGE_ECS_REGISTER_COMPONENT(&s, struct mageVector3, exampleConstructer, exampleDeconstructer, MAGE_COMPONENT_REGISTERING_MODE_REQUIRED);
    MAGE_ECS_REGISTER_COMPONENT(&s, struct mageVector4, exampleConstructer, exampleDeconstructer, MAGE_COMPONENT_REGISTERING_MODE_REQUIRED);


    mageSceneDestroy(&s);
    free(SandboxApplication);
    mageLogEnd();
    return 1;
}

        
