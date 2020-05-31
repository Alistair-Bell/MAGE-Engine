#include "mageAPI.h"

void *mageApplicationAllocate()
{
    return malloc(sizeof(struct MAGE_APPLICATION_STRUCT));
}

static mageResult mageApplicationDefaultStart(mageApplication *application)
{
    MAGE_LOG_CORE_WARNING("Using default start method\n", NULL);
    return MAGE_SUCCESS;
}
static void mageApplicationDefaultUpdate(mageApplication *application)
{
    MAGE_LOG_CORE_WARNING("Using default update method\n", NULL);
}
static mageResult mageApplicationDefaultDestroy(mageApplication *application)
{
    MAGE_LOG_CORE_WARNING("Using default destroy method\n", NULL);
    return MAGE_SUCCESS;
}
mageResult mageApplicationInitialise(mageApplication *application, const mageApplicationProps *props)
{
    application->Props = *props;

    application->Running = 1;

    if (application->Props.AllocationCallback == NULL) application->Props.AllocationCallback = malloc;
    if (application->Props.FreeCallback == NULL) application->Props.FreeCallback = free;
    if (application->Props.StartMethod == NULL) application->Props.StartMethod = mageApplicationDefaultStart;
    if (application->Props.UpdateMethod == NULL) application->Props.UpdateMethod = mageApplicationDefaultUpdate;
    if (application->Props.DestroyMethod == NULL) application->Props.DestroyMethod = mageApplicationDefaultDestroy;


    application->Renderer = application->Props.AllocationCallback(sizeof(struct MAGE_RENDERER_STRUCT));
    application->Window = application->Props.AllocationCallback(sizeof(struct MAGE_WINDOW_STRUCT));

    char temp[255];
    sprintf(temp, "%s : Version %.2f", props->Name, props->Version);
        
    mageResult result = mageWindowInitialise(application->Window, props->Width, props->Height, temp);
    
    if (result != MAGE_SUCCESS)
    {
        return result;
    }

    result = mageRendererInitialise(application->Renderer, application->Window);
    
    if (result != MAGE_SUCCESS)
    {
        return result;
    }
    
    return MAGE_SUCCESS;
}
mageResult mageApplicationRun(mageApplication *application)
{
    mageResult startResult, destroyResult;
    
    startResult = application->Props.StartMethod(application);
    
    if (startResult != MAGE_SUCCESS)
    {
        MAGE_LOG_CORE_ERROR("Application start method has failed\n", NULL);
        return startResult;
    }

    while (application->Running)
    {
        application->Props.UpdateMethod(application);

        #if defined(MAGE_GLFW)

            /* Swap front and back buffers */
            mageWindowSwapBuffers(application->Window);

            /* Poll for and process events */
            glfwPollEvents();

            application->Running = !(glfwWindowShouldClose(application->Window->Context));

        #endif
    
    }
    
    destroyResult = application->Props.DestroyMethod(application);

    if (destroyResult != MAGE_SUCCESS)
    {
        MAGE_LOG_CORE_ERROR("Destroy method has failed\n", NULL);
        return destroyResult;
    }

    mageApplicationDestroy(application);
    return MAGE_SUCCESS;
}
void mageApplicationDestroy(mageApplication *application)
{
    application->Props.FreeCallback(application->Window);

    mageRendererDestroy(application->Renderer);

    application->Props.FreeCallback(application->Renderer);
}