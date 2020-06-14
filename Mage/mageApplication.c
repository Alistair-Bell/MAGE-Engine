#include "mageAPI.h"

mageResult mageEngineInitialise()
{
    #if defined (MAGE_DEBUG)
		mageFileDumpContents("Logs/mage.log", "", 1);
		mageLogInitialise("Logs/mage.log");
		MAGE_LOG_CORE_WARNING("Debug mode in uses, for best performance turn debug mode of\n", NULL);
		MAGE_LOG_CORE_INFORM("Cleaned previous file contents\n", NULL);
	#endif

	#if defined (MAGE_SDL2)
		const uint32_tflag = SDL_Init(SDL_INIT_EVERYTHING);
			
		if (flag != 0)
		{
			MAGE_LOG_CLIENT_FATAL_ERROR("SDL2 failed to initialise : %s\n", SDL_GetError());
			return MAGE_LIBRARY_FAILURE
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
				MAGE_LOG_CLIENT_FATAL_ERROR("GLFW does not support vulkan\n", NULL);
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
mageResult mageApplicationInitialise(struct mageApplication *application, struct mageApplicationProps props)
{
    mageResult engineStart = mageEngineInitialise(props);
    mageResult result;

    application->Props = props; 

    application->Running = 1;

    if (application->Props.StartMethod == NULL) application->Props.StartMethod = mageApplicationDefaultStart;
    if (application->Props.UpdateMethod == NULL) application->Props.UpdateMethod = mageApplicationDefaultUpdate;
    if (application->Props.DestroyMethod == NULL) application->Props.DestroyMethod = mageApplicationDefaultDestroy;


    application->Renderer = mageRendererAllocate();
    application->Window = mageWindowAllocate();
    #if defined (MAGE_MONO_EXTERNALS)
        
        application->MonoHandler = mageMonoHandlerAllocate();

        result = mageMonoHandlerInitialise(application->MonoHandler, application->Props.ClientDLL);

        if (result != MAGE_SUCCESS) 
        { 
            return result; 
        }

    #endif

    char temp[255];
    sprintf(temp, "%s : Version %.2f", application->Props.Name, application->Props.Version);
        
    result = mageWindowInitialise(application->Window, application->Props.Width, application->Props.Height, temp);
    
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
    mageWindowTerminate(application->Window);
    mageMonoCleanup(application->MonoHandler);
    mageRendererDestroy(application->Renderer);
    free(application->Renderer);
    free(application->Window);
    free(application->MonoHandler);
    MAGE_LOG_CORE_INFORM("Application has been cleaned up\n", NULL);
    mageLogEnd();
}