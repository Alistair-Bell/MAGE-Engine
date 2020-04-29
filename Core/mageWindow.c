#include "mageAPI.h"
#include <GLFW/glfw3.h>

void *mageWindowAllocate()
{
	return malloc(sizeof(struct MAGE_WINDOW_STRUCT));
}
void mageWindowInitialise(mageWindow *window, const sint32 xResolution, const sint32 yResolution, const char *title, uint8 *success)
{

	window->Height = yResolution;
	window->Width = xResolution;
	window->Title = title;
	window->Running = 1;
	
	#if defined(MAGE_SDL2)

		MAGE_LOG_CORE_INFORM("Using SDL2 as window mode.\n", NULL);

		const uint32 flag = SDL_Init(SDL_INIT_EVERYTHING);
		
		if (flag != 0)
		{
			MAGE_LOG_CLIENT_FATAL_ERROR("SDL2 failed to initialise : %s.\n", SDL_GetError());
			mageTryDumpSuccess(0, success);
			return;
		}
		
		MAGE_LOG_CORE_INFORM("SDL2 has succesfully initialised everything.\n", NULL);

		window->Context = SDL_CreateWindow(window->Title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window->Width, window->Height, 0);

		if (window->Context == NULL)
		{
			MAGE_LOG_CORE_FATAL_ERROR("SDL2 context was failed to initialise.\n", NULL);
			mageTryDumpSuccess(0, success);
			return;
		}	

		MAGE_LOG_CORE_INFORM("SDL2 window has succesfully been created.\n", NULL);


	#elif defined(MAGE_GLFW)
		
		MAGE_LOG_CORE_INFORM("Using GLFW as window mode\n", NULL);

		if (!glfwInit())
		{
			mageTryDumpSuccess(0, success);
			MAGE_LOG_CORE_FATAL_ERROR("GLFW library has failed to initialise\n", NULL);
			return;
		}	
		window->Context = glfwCreateWindow(window->Width, window->Height, window->Title, NULL, NULL);

		if (window->Context == NULL)
		{
			glfwTerminate();
			mageTryDumpSuccess(0, success);
			MAGE_LOG_CORE_FATAL_ERROR("GLFW context has failed to create\n", NULL);
			return;
		}
		
		MAGE_LOG_CORE_INFORM("GLFW context created\n", NULL);
		
		glfwMakeContextCurrent(window->Context);
		glfwSetWindowSizeCallback(window->Context, mageWindowResizeCallback);
	
	#endif
	
	mageTryDumpSuccess(1, success);
}
void mageWindowSwapBuffers(mageWindow *window)
{
	#if defined(MAGE_SLD2)

	#elif defined(MAGE_GLFW)
		glfwSwapBuffers(window->Context);
	#endif
	
}
#if defined(MAGE_GLFW)
	void mageWindowResizeCallback(GLFWwindow *window, sint32 xResolution, sint32 yResolution)
	{

	}
#endif
void mageWindowDestroy(mageWindow *window)
{
	#if defined(MAGE_GLFW)
		glfwTerminate();
	#elif defined(MAGE_SDL2)
		SDL_DestroyWindow(window->Context);	
		SDL_Quit();
	#endif	
	mageFreeMethod(window);
}
