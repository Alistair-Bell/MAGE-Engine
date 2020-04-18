#include "mageAPI.h"

int main(void)
{
	mageWindow *window;
	mageWindowAllocate(window);

	mageWindowInitialise(window, 1080, 720, "Hello World");

	while(window->Running == 1)
	{
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
			
		glClear(GL_COLOR_BUFFER_BIT);
	
		mageWindowSwapBuffers(window);



		glfwPollEvents();
		window->Running = (!glfwWindowShouldClose(window->Context));
	}

	return 0;
}
