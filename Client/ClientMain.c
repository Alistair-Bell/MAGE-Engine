#include "../Core/mageAPI.h"

mageApplication app;
mageApplicationProps props;

#define LOG(...) printf(__VA_ARGS__)


int main(int32 argc, char **args)
{   
    mageEngineInitialise(NULL);

    props.Height = 720;
    props.Width = 1080;
    props.Name = "Hello World";
    props.Version = 1.0;

    mageApplicationInitialise(&app, &props, NULL);

    while (app.Window->Running)
    {
        glfwPollEvents();

        mageWindowSwapBuffers(app.Window);

        app.Window->Running = !(glfwWindowShouldClose(app.Window->Context));
    }

    mageLogReset();
    return 0;
}
