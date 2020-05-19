#include "../Core/mageAPI.h"

static mageApplication app;
static mageApplicationProps props;
static uint8 flag;




int main(int32 argc, char **args)
{   
    mageEngineInitialise(&flag);

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

    mageRendererDestroy(app.Renderer);
    mageLogReset();
    return 0;
}
