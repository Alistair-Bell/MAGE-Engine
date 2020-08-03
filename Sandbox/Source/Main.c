#include "Core.h"

#define SANDBOX_ENTITY_COUNT 3

/* Application instance */
static struct mageApplication *SandboxApplication;
static struct mageShader shaders[2];
static struct mageRenderable renderable;

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
    applicationCreateInfo.WindowIcon             = "Mage/Resources/Textures/MTEC/Logo.png";

    struct mageRendererCreateInfo rendererCreateInfo;
    memset(&rendererCreateInfo, 0, sizeof(struct mageRendererCreateInfo));

    rendererCreateInfo.PipelineShaders          = shaders;
    rendererCreateInfo.ShaderCount              = sizeof(shaders) / sizeof(struct mageShader);
    
    mageApplicationCreate(SandboxApplication, applicationCreateInfo, rendererCreateInfo);
    struct mageMatrix4 model;
    struct mageMatrix4 projection;
    struct mageMatrix4 view;
    struct mageVertex verticies[] = 
    {
        { .Vertex = { .X = -0.25f, .Y = -0.25f }, .Color = { .X = 1.0f, .Y = 0.0f, .Z = 0.0f} },
        { .Vertex = { .X = 0.25f, .Y = -0.25f }, .Color = { .X = 0.0f, .Y = 1.0f, .Z = 0.0f} },
        { .Vertex = { .X = 0.25f, .Y = 0.25f }, .Color = { .X = 0.0f, .Y = 0.0f, .Z = 1.0f} },
        { .Vertex = { .X = -0.25f, .Y = 0.25f }, .Color = { .X = 0.0f, .Y = 0.0f, .Z = 1.0f} }
    };

    memset(&model, 0, sizeof(struct mageMatrix4));
    memset(&projection, 0, sizeof(struct mageMatrix4));
    memset(&view, 0, sizeof(struct mageMatrix4));


    mageRenderableCreate(
        &renderable, 
        &(struct mageRenderableCreateInfo)
        {
            .IndexCount         = 6,
            .Indicies           = (uint16_t[]) { 0, 1, 2, 2, 3, 0 },
            .VertexCount        = 4,
            .Verticies          = verticies,
            .TextureCreateInfo  = &(struct mageTextureCreateInfo)
            {
                .SamplerMode = MAGE_TEXTURE_SAMPLER_REPEAT,
                .TexturePath = "Mage/Resources/Textures/Vulkan/Vulkan_White_170px_Dec16.png",
            }
        },
        SandboxApplication->Renderer
    );
    
    while (!(glfwWindowShouldClose(SandboxApplication->Window->Context)))
    {
        glfwPollEvents();
        mageRendererDraw(SandboxApplication->Renderer, &renderable);
    }
    vkDeviceWaitIdle(SandboxApplication->Renderer->Device);
    
    mageRenderableDestroy(&renderable, SandboxApplication->Renderer);
    mageApplicationDestroy(SandboxApplication);    
    free(SandboxApplication);
    mageLogEnd();
    return 1;
}

        