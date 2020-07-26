#include "mageAPI.h"

mageResult mageEngineInitialise()
{
    uint32_t result = glfwInit();
    if (result != GLFW_TRUE)
    {
        MAGE_LOG_CORE_INFORM("GLFW has failed to be created\n", NULL);
        return MAGE_RESULT_LIBRARY_FAILURE;
    }
    
    return MAGE_RESULT_SUCCESS;
}
mageResult mageApplicationCreate(struct mageApplication *application, struct mageApplicationCreateInfo applicationInfo, struct mageRendererCreateInfo rendererInfo)
{
    application->Window = malloc(sizeof(struct mageWindow));
    application->Renderer = malloc(sizeof(struct mageRenderer));
    application->RendererCreateInfo = rendererInfo;

    if (applicationInfo.WindowIcon == NULL)
    {
        applicationInfo.WindowIcon = "Mage/Resources/Textures/MTEC/Logo.png";
    }
    if (rendererInfo.PipelineShaders == NULL || rendererInfo.ShaderCount <= 0)
    {
        MAGE_LOG_CORE_WARNING("Here\n", NULL);
        struct mageShader defaultShaders[2];
        mageShaderCreate(&defaultShaders[0], "Mage/Resources/Shaders/fragment.sprv", "main", MAGE_SHADER_TYPE_FRAGMENT);
        mageShaderCreate(&defaultShaders[1], "Mage/Resources/Shaders/vertex.sprv", "main", MAGE_SHADER_TYPE_VERTEX);
        rendererInfo.PipelineShaders    = defaultShaders;
        rendererInfo.ShaderCount        = 2;
    }
    application->Running = 1;
    mageResult returnCode = mageEngineInitialise();
    if (returnCode != MAGE_RESULT_SUCCESS) { return returnCode; }
    mageWindowCreate(application->Window, &applicationInfo);
    mageRendererCreate(application->Renderer, application->Window, &application->RendererCreateInfo);
    mageInputSetup(application);
    return MAGE_RESULT_SUCCESS;
}
void mageApplicationDestroy(struct mageApplication *application)
{
    mageWindowDestroy(application->Window);
    mageRendererDestroy(application->Renderer, &application->RendererCreateInfo);
    free(application->Window);
    free(application->Renderer);
    
    MAGE_LOG_CORE_INFORM("Application has been cleaned up\n", NULL);
}
