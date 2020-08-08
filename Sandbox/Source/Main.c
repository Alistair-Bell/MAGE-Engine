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
    free(SandboxApplication);
    mageLogEnd();
    return 1;
}

        
