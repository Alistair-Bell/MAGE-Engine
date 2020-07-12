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
mageResult mageApplicationInitialise(struct mageApplication *application, struct mageApplicationProps engineProps, struct mageRendererProps rendererProps)
{
    mageResult returnCode = mageEngineInitialise();
    if (returnCode != MAGE_RESULT_SUCCESS) { return returnCode; }

    return MAGE_RESULT_SUCCESS;
}
mageResult mageApplicationRun(struct mageApplication *application)
{

 
    return MAGE_RESULT_SUCCESS;
}
void mageApplicationDestroy(struct mageApplication *application)
{
    
    mageRendererDestroy(application->Renderer);
    mageWindowTerminate(application->Window);
    free(application->Renderer);
    free(application->Window);
    MAGE_LOG_CORE_INFORM("Application has been cleaned up\n", NULL);
}
