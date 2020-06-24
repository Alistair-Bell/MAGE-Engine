#include "mageAPI.h"


static double CIRCLE_RAD		= HYP_PI * 2;
static double CIRCLE_THIRD	    = (HYP_PI * 2) / 3.0;
static double CIRCLE_THIRD_1	= 0;
static double CIRCLE_THIRD_2	= (HYP_PI * 2) / 3.0;
static double CIRCLE_THIRD_3	= ((HYP_PI * 2) / 3.0) * 2;

mageResult mageEngineInitialise()
{
	#if defined (MAGE_SDL)
		const uint32_t flag = SDL_Init(SDL_INIT_EVERYTHING);
			
		if (flag != 0)
		{
			MAGE_LOG_CORE_FATAL_ERROR("SDL2 failed to initialise : %s\n", SDL_GetError());
			return MAGE_LIBRARY_FAILURE;
		}

		MAGE_LOG_CORE_INFORM("SDL2 has succesfully initialised everything\n", NULL);

	#endif

	#if defined (MAGE_GLFW)

		if (!glfwInit())
		{			
			MAGE_LOG_CORE_FATAL_ERROR("GLFW library has failed to initialise\n", NULL);
			return MAGE_LIBRARY_FAILURE;
		}	

		#if defined (MAGE_VULKAN)
			uint8_t flag = glfwVulkanSupported();

			if (!flag)
			{
				MAGE_LOG_CORE_FATAL_ERROR("GLFW does not support vulkan\n", NULL);
				return MAGE_LIBRARY_FAILURE;
			}
			MAGE_LOG_CORE_INFORM("GLFW supports vulkan\n", NULL);

		#endif

		MAGE_LOG_CORE_INFORM("GLFW has succesfully initialised everything.\n", NULL);

	#endif
	
    MAGE_LOG_CORE_INFORM("Engine dependencies initialised\n", NULL);
    return MAGE_SUCCESS;
}
void *mageApplicationAllocate()
{
    return malloc(sizeof(struct mageApplication));
}

static mageResult mageApplicationDefaultStart(struct mageApplication *application)
{
    return MAGE_SUCCESS;
}
static void mageApplicationDefaultUpdate(struct mageApplication *application)
{

}
static mageResult mageApplicationDefaultDestroy(struct mageApplication *application)
{
    return MAGE_SUCCESS;
}
mageResult mageApplicationInitialise(struct mageApplication *application, struct mageApplicationProps engineProps, struct mageRendererProps rendererProps)
{
    mageEngineInitialise(engineProps);
    mageResult result;

    application->Props = engineProps; 

    application->Running = 1;

    if (application->Props.StartMethod == NULL) application->Props.StartMethod = mageApplicationDefaultStart;
    if (application->Props.UpdateMethod == NULL) application->Props.UpdateMethod = mageApplicationDefaultUpdate;
    if (application->Props.DestroyMethod == NULL) application->Props.DestroyMethod = mageApplicationDefaultDestroy;

    application->Renderer = mageRendererAllocate();
    application->Window = mageWindowAllocate();
    char temp[255];
    sprintf(temp, "%s : Version %.2f", application->Props.Name, application->Props.Version);
    
    result = mageWindowInitialise(application->Window, application->Props.Width, application->Props.Height, temp);
    
    if (result != MAGE_SUCCESS)
    {
        return result;
    }

    result = mageRendererInitialise(application->Renderer, application->Window, &rendererProps); 
    
    if (result != MAGE_SUCCESS)
    {
        return result;
    }

    mageEventSetupMaster();
    mageInputSetup(application->Window);
    return MAGE_SUCCESS;
}
mageResult mageApplicationRun(struct mageApplication *application)
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
        #if defined (MAGE_GLFW)

            application->Props.UpdateMethod(application);

            glfwPollEvents();
            mageRendererRender(application->Renderer);

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
void mageApplicationDestroy(struct mageApplication *application)
{
    
    mageRendererDestroy(application->Renderer);
    mageWindowTerminate(application->Window);
    free(application->Renderer);
    free(application->Window);
    MAGE_LOG_CORE_INFORM("Application has been cleaned up\n", NULL);
}
