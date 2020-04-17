#include "mageAPI.h"

int main(void)
{
	mageWindow *window;
	mageWindowAllocate(window);
	
	uint8 x;

	mageWindowInitialise(window, 1080, 720, "Hello World");

	while(window->Running == 1)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		mageWindowSwapBuffers(window);



		glfwPollEvents();
		window->Running = (!glfwWindowShouldClose(window->Context));
	}

	return 0;
}
