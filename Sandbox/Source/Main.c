#include "Core.h"

#define SANDBOX_ENTITY_COUNT 5

/* Application instance */
static struct mageApplication   *SandboxApplication;
static struct mageShader        shaders[2];
static struct mageRenderable    renderable, renderable2;

void CreateShaders()
{
    mageShaderCreate(&shaders[0], "Mage/Resources/Shaders/DefaultTexture.sprv", "main", MAGE_SHADER_TYPE_FRAGMENT);
    mageShaderCreate(&shaders[1], "Mage/Resources/Shaders/Default.sprv", "main", MAGE_SHADER_TYPE_VERTEX);
}
void TransformConstructer(void *data, uint64_t size)
{
    struct mageTransform *t = ((struct mageTransform *)data);
    mageVector3CreateFromFloats(&t->Location, 1.0f, 2.0f, 3.0f);
}
void *System(void *package)
{
    return MAGE_SYSTEM_SUCCESS;
}

MAGE_ENTRY_POINT()
{
    mageLogInitialise("Logs/mage.log");
    SandboxApplication = malloc(sizeof(struct mageApplication));
#if 1
    CreateShaders();    

    struct mageUserInputInquirerSetupInfo inputSetup;
    memset(&inputSetup, 0, sizeof(struct mageUserInputInquirerSetupInfo));
    inputSetup.CursorFlags                      = (mageMouseCursorSetupFlags[]) { MAGE_MOUSE_CURSOR_SETUP_FLAGS_HIDDEN };
    inputSetup.CursorFlagsCount                 = 1;
    inputSetup.MouseFlags                       = (mageMouseSetupFlags[]) { MAGE_MOUSE_SETUP_FLAGS_RAW_MOTION };
    inputSetup.MouseFlagsCount                  = 1;
    inputSetup.KeyboardFlags                    = NULL;
    inputSetup.KeyboardFlagsCount               = 0;
    inputSetup.ExternalInputFlags               = (mageExternalInputSetupFlags[]) { MAGE_EXTERNAL_INPUT_SETUP_REQUIRE_PRESENT_GAMEPAD };
    inputSetup.ExtenalInputFlagsCount           = 1;

    struct mageApplicationCreateInfo applicationCreateInfo;
    memset(&applicationCreateInfo, 0, sizeof(struct mageApplicationCreateInfo));

    applicationCreateInfo.FixedResolution        = MAGE_TRUE;
    applicationCreateInfo.Fullscreen             = MAGE_FALSE;
    applicationCreateInfo.Width                  = 1920;
    applicationCreateInfo.Height                 = 1080;
    applicationCreateInfo.Name                   = "Sandbox Application";
    applicationCreateInfo.WindowIcon             = "Mage/Resources/Textures/Vulkan/Vulkan_500px_Dec16.jpg";
    applicationCreateInfo.InputSetup             = &inputSetup;


    struct mageRendererCreateInfo rendererCreateInfo;
    memset(&rendererCreateInfo, 0, sizeof(struct mageRendererCreateInfo));

    rendererCreateInfo.PipelineShaders          = shaders;
    rendererCreateInfo.ShaderCount              = sizeof(shaders) / sizeof(struct mageShader);
    rendererCreateInfo.TextureTransparency      = MAGE_TRUE;
    rendererCreateInfo.BackgroundColor          = (struct mageVector4) { .Values[0] = 0.0f, .Values[1] = 0.0f, .Values[2] = 0.0f, .Values[3] = 1.0f };
    
    mageApplicationCreate(SandboxApplication, &applicationCreateInfo, &rendererCreateInfo);

    struct mageVertex verticies1[] = 
    {
        { .Vertex = { .X = -0.5f, .Y = -0.5f },   .Color = { .X = 1.0f, .Y = 0.0f, .Z = 0.0f}, .TextureLocation = { .X = 0.0f, .Y = 0.0f } },  
        { .Vertex = { .X =  0.5f, .Y = -0.5f },   .Color = { .X = 0.0f, .Y = 1.0f, .Z = 0.0f}, .TextureLocation = { .X = 1.0f, .Y = 0.0f } },  
        { .Vertex = { .X =  0.5f, .Y =  0.5f },   .Color = { .X = 0.0f, .Y = 0.0f, .Z = 1.0f}, .TextureLocation = { .X = 1.0f, .Y = 1.0f } },  
        { .Vertex = { .X = -0.5f, .Y =  0.5f },   .Color = { .X = 0.0f, .Y = 0.0f, .Z = 1.0f}, .TextureLocation = { .X = 0.0f, .Y = 1.0f } }
    
    };

    struct mageRenderableCreateInfo info;
    memset(&info, 0, sizeof(struct mageRenderableCreateInfo));
    info.Verticies          = verticies1;
    info.VertexCount        = 4;
    info.IndexCount         = 0;
    info.Indicies           = NULL;
    info.TextureCreateInfo  = (struct mageTextureCreateInfo) { .SamplerMode = MAGE_TEXTURE_SAMPLER_CLAMP_TO_BORDER, .TexturePath = "Sandbox/Resources/Textures/texture.jpg" };
    mageRenderableCreate(&renderable, &info, SandboxApplication->Renderer);

    struct mageRenderable *r[] = { &renderable };

    struct mageVulkanMemoryHeap heap;
    
    VkBuffer b, bb;
    uint64_t data[] = { 1, 2, 3, 4, 5, 6 };

    struct mageVulkanMemoryHeapCreateInfo i;
    memset(&i, 0, sizeof(struct mageVulkanMemoryHeapCreateInfo));
    i.AllocationSize                = 1024 * 64;
    i.AssociatedHeap                = 0;
    i.PhysicalDevice                = SandboxApplication->Renderer->PhysicalDevice;
    i.Device                        = SandboxApplication->Renderer->Device;
    
    struct mageVulkanMemoryBufferReference re, rb;
    
    struct mageVulkanMemoryMapBufferInfo m;
    memset(&m, 0, sizeof(struct mageVulkanMemoryMapBufferInfo));
    m.AssociatedHeap                = 0;
    m.Buffer                        = &b;
    m.BufferUsage                   = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    m.PhysicalDevice                = SandboxApplication->Renderer->PhysicalDevice;
    m.Device                        = SandboxApplication->Renderer->Device;
    m.CommandPool                   = &SandboxApplication->Renderer->CommandPool;
    m.CopyCommandBuffer             = NULL;
    m.Data                          = data;
    m.DataSize                      = sizeof(data);
    m.Buffer                        = &b;
    m.Reference                     = &re;

    // mageVulkanMemoryAllocateHeap(&heap, &i);
    // mageVulkanMemoryBufferMapToBlock(&heap, &m);

    m.Buffer                        = &bb;
    m.Reference                     = &rb;
    // mageVulkanMemoryBufferMapToBlock(&heap, &m);
    // mageVulkanMemoryBufferUnmapBufferToBlock(&heap, &(struct mageVulkanMemoryUnmapBufferInfo){ .Device = SandboxApplication->Renderer->Device, .PhysicalDevice = SandboxApplication->Renderer->PhysicalDevice, .Reference = &re });
    // mageVulkanMemoryBufferUnmapBufferToBlock(&heap, &(struct mageVulkanMemoryUnmapBufferInfo){ .Device = SandboxApplication->Renderer->Device, .PhysicalDevice = SandboxApplication->Renderer->PhysicalDevice, .Reference = &rb });
    // mageVulkanMemoryFreeMemory(SandboxApplication->Renderer->Device, &heap);

    while (!(glfwWindowShouldClose(SandboxApplication->Window->Context)))
    {
        mageRendererDrawRenderables(SandboxApplication->Renderer, r, 1);
        glfwPollEvents();
    }
    vkDeviceWaitIdle(SandboxApplication->Renderer->Device);
    mageRenderableDestroy(&renderable, SandboxApplication->Renderer);
    mageApplicationDestroy(SandboxApplication);

#endif
#if 0
    struct mageScene s;
    struct mageSceneCreateInfo i;
    i.ComponentLimit                = 10;
    i.EntityLimit                   = 10;
    i.SceneTag                      = "Hello World";
    i.RegisterDefaultComponents     = MAGE_FALSE;
    mageSceneCreate(&s, &i);
    uint32_t transform              = MAGE_ECS_REGISTER_COMPONENT(&s, struct mageTransform, TransformConstructer, NULL, MAGE_ECS_COMPONENT_REGISTERING_MODE_OPTIONAL);
    uint32_t vector3                = MAGE_ECS_REGISTER_COMPONENT(&s, struct mageVector3, NULL, NULL, MAGE_ECS_COMPONENT_REGISTERING_MODE_OPTIONAL);
    
    
    
    struct mageTransform t;
    
    struct mageVector3 v;
    memset(&t, 0, sizeof(struct mageTransform));
    memset(&v, 0, sizeof(struct mageVector3));

    mageEntity e = mageSceneEntityCreate(&s);
    struct mageComponentHandle h1 = MAGE_ECS_BIND_NEW_COMPONENT_BY_ID_TO_ENTITIES(&s, transform, &t, &e, 1);
    
    struct mageTransform trf = MAGE_ECS_GET_COMPONENT_BY_HANDLE(&s, struct mageTransform, h1, e);
    

    MAGE_ECS_REGISTER_SYSTEM(&s, System, MAGE_ECS_SYSTEM_TYPE_UPDATE, MAGE_ECS_SYSTEM_THREAD_PRIORITY_NONE, 1, struct mageTransform);

    mageSceneTick(&s);
    mageSceneDestroy(&s);
#endif
    free(SandboxApplication);
    mageLogEnd();
    return 1;
}

        
