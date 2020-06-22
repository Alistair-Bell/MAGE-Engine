#include "mageAPI.h"


static double CIRCLE_RAD		= HYP_PI * 2;
static double CIRCLE_THIRD	    = (HYP_PI * 2) / 3.0;
static double CIRCLE_THIRD_1	= 0;
static double CIRCLE_THIRD_2	= (HYP_PI * 2) / 3.0;
static double CIRCLE_THIRD_3	= ((HYP_PI * 2) / 3.0) * 2;

mageResult mageEngineInitialise()
{
    #if defined (MAGE_DEBUG)
		mageFileDumpContents("Logs/mage.log", "", 1);
		mageLogInitialise("Logs/mage.log");
		MAGE_LOG_CORE_WARNING("Debug mode in uses, for best performance turn debug mode of\n", NULL);
		MAGE_LOG_CORE_INFORM("Cleaned previous file contents\n", NULL);
	#endif

	#if defined (MAGE_SDL)
		const uint32_t flag = SDL_Init(SDL_INIT_EVERYTHING);
			
		if (flag != 0)
		{
			MAGE_LOG_CLIENT_FATAL_ERROR("SDL2 failed to initialise : %s\n", SDL_GetError());
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

    #if defined (MAGE_VULKAN)
    
    VkExtent2D extent;
    memset(&extent, 0, sizeof(VkExtent2D));
    extent.height = application->Window->Height;
    extent.width  = application->Window->Width;

    VkRect2D renderArea;
    memset(&renderArea, 0, sizeof(VkRect2D));
    renderArea.offset.x = 0;
    renderArea.offset.y = 0;
    renderArea.extent   = extent;

    #endif

    float f = 0.0f;

    while (application->Running)
    {
        #if defined (MAGE_GLFW)

            application->Props.UpdateMethod(application);

            glfwPollEvents();
            
            mageRendererBeginRender(application->Renderer);
            #if defined (MAGE_VULKAN)
                
                VkCommandBufferBeginInfo bufferBeginInfo;
                memset(&bufferBeginInfo, 0, sizeof(VkCommandBufferBeginInfo));
                bufferBeginInfo.sType             = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                bufferBeginInfo.flags             = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
                vkBeginCommandBuffer(application->Renderer->CommandBuffer[0], &bufferBeginInfo);
                
                f += 0.001;

                VkClearValue values[2];
                memset(&values, 0, sizeof(VkClearValue) * 2);
                values[0].depthStencil.depth    = 0.0f;
                values[0].depthStencil.stencil  = 0;
                values[1].color.float32[0]		= sin(f + CIRCLE_THIRD_1) * 0.5 + 0.5;
		        values[1].color.float32[1]		= sin(f + CIRCLE_THIRD_2) * 0.5 + 0.5;
		        values[1].color.float32[2]		= sin(f + CIRCLE_THIRD_3) * 0.5 + 0.5;
		        values[1].color.float32[3]		= 1.0f;


                VkRenderPassBeginInfo renderPassBeginInfo;
                memset(&renderPassBeginInfo, 0, sizeof(VkRenderPassBeginInfo));
                renderPassBeginInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                renderPassBeginInfo.renderPass      = application->Renderer->RenderPass;
                renderPassBeginInfo.framebuffer     = mageRendererGetActiveFrameBuffer(application->Renderer);
                renderPassBeginInfo.renderArea      = renderArea;
                renderPassBeginInfo.clearValueCount = 2;
                renderPassBeginInfo.pClearValues    = values;


                vkCmdBeginRenderPass(application->Renderer->CommandBuffer[0], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

                vkCmdEndRenderPass(application->Renderer->CommandBuffer[0]);

                vkEndCommandBuffer(application->Renderer->CommandBuffer[0]);

                VkSubmitInfo submitInfo;
                memset(&submitInfo, 0, sizeof(VkSubmitInfo));
                submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                submitInfo.waitSemaphoreCount   = 0; 
                submitInfo.pWaitSemaphores      = NULL;
                submitInfo.pWaitDstStageMask    = NULL;
                submitInfo.commandBufferCount   = 1;
                submitInfo.pCommandBuffers      = &application->Renderer->CommandBuffer[0];
                submitInfo.signalSemaphoreCount = 1;
                submitInfo.pSignalSemaphores    = &application->Renderer->Semaphore; 

                vkQueueSubmit(application->Renderer->GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

                mageRendererEndRendering(application->Renderer);
                #endif
            application->Running = !(glfwWindowShouldClose(application->Window->Context));
        #endif
    }

    #if defined (MAGE_VULKAN)

    vkQueueWaitIdle(application->Renderer->GraphicsQueue);
    
    #endif

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
#if defined (MAGE_MONO_EXTERNALS)
    mageMonoCleanup(application->MonoHandler);
    free(application->MonoHandler);
#endif
    MAGE_LOG_CORE_INFORM("Application has been cleaned up\n", NULL);
    mageLogEnd();
}
