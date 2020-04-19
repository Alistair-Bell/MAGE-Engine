#include "mageAPI.h"

int main(void)
{
	mageWindow *window;
	mageWindowAllocate(window);

	mageWindowInitialise(window, 1080, 720, "Hello World");
	uint8 flag;	

	const char *foo = "Hello World\n";
	mageFileDumpContents("hello.txt", foo, 0, &flag);		
	
	while(window->Running == 1)
	{
		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	
		mageWindowSwapBuffers(window);

		glfwPollEvents();
		window->Running = (!glfwWindowShouldClose(window->Context));
	}

	return 0;
}
