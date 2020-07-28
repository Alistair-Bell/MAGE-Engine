#include "Core.h"
#include <AL/al.h>

#define SANDBOX_ENTITY_COUNT 3

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
    struct matrix4 model;
    struct matrix4 projection;
    struct matrix4 view;
    memset(&model, 0, sizeof(struct matrix4));
    memset(&projection, 0, sizeof(struct matrix4));
    memset(&view, 0, sizeof(struct matrix4));


    mageRenderableCreate(
        &renderable, 
        &(struct mageRenderableCreateInfo)
        {
            .IndexCount     = 6,
            .Indicies       = (uint16_t[]) { 0, 1, 2, 2, 3, 0 },
            .VertexCount    = 4,
            .Verticies      = (struct mageVertex[]) 
            { 
                { .Vertex = { .x = -0.5f, .y = -0.5f }, .Color = { .x = 1.0f, .y = 0.0f, .z = 0.0f} },
                { .Vertex = { .x = 0.5f, .y = -0.5f }, .Color = { .x = 0.0f, .y = 1.0f, .z = 0.0f} },
                { .Vertex = { .x = 0.5f, .y = 0.5f }, .Color = { .x = 0.0f, .y = 0.0f, .z = 1.0f} },
                { .Vertex = { .x = -0.5f, .y = 0.5f }, .Color = { .x = 0.0f, .y = 0.0f, .z = 1.0f} }
            },
            .Uniform        = (struct mageUniformObject) 
                { .View = view, .Model = model, .Projection = projection },
            
            .TextureCreateInfo = &(struct mageTextureCreateInfo)
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
    
#if 0
    struct mageHeapAllocater allocater = mageHeapAllocaterDefault();
    mageSceneCreate(&currentScene, SANDBOX_ENTITY_COUNT, "Current Scene", &allocater);
    
    mageEntity entities[3];
    
    MAGE_ECS_REGISTER_COMPONENT(&currentScene, struct vector3);
    MAGE_ECS_REGISTER_COMPONENT(&currentScene, struct vector2);

    struct vector2 data;
    data.v[0] = 0.0f;
    data.v[1] = 2.0f;

    struct vector3 other;
    other.v[0] = 0.0f;
    other.v[1] = 2.0f;
    other.v[2] = 4.0f;
    
    uint32_t i;
    for (i = 0; i < SANDBOX_ENTITY_COUNT; i++)
    {
        entities[i] = mageEntityCreate(&currentScene);
        MAGE_ECS_BIND_COMPONENT(&currentScene, entities[i], struct vector2, &data);
        MAGE_ECS_BIND_COMPONENT(&currentScene, entities[i], struct vector3, &other);
    }
    for (i = 0; i < SANDBOX_ENTITY_COUNT; i++)
    {
        mageEntityDestroy(&currentScene, entities[i]);
    }

    mageSceneDestroy(&currentScene);
#endif
    
    free(SandboxApplication);
    mageLogEnd();
    return 1;
}

        