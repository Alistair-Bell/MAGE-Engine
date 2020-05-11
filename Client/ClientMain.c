#include "../Core/mageAPI.h"

static mageWindow window;

int main(void)
{
    mageWindowInitialise(&window, 1080, 720, "Client", NULL);

    while (window.Running)
    {
        glfwPollEvents();

        mageWindowSwapBuffers(&window);



        window.Running = (!glfwWindowShouldClose(window.Context));
    }


    mageWindowTerminate(&window);
    mageLogReset();
    return 0;
}