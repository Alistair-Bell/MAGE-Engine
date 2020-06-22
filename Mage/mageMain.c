#include "mageAPI.h"

struct mageApplication          *Application;
struct mageApplicationProps     *ApplicationProps;
struct mageRendererProps        *RendererProps;

int32_t main(int32_t argc, char **args)
{  
    Application = malloc(sizeof(struct mageApplication));
    ApplicationProps = malloc(sizeof(struct mageApplicationProps));
    RendererProps = malloc(sizeof(struct mageRendererProps));

    ApplicationProps->Name      = "MAGE - GLFW - Vulkan";
    ApplicationProps->Width     = 1920;
    ApplicationProps->Height    = 1080;
    ApplicationProps->Version   = 1.0;
    ApplicationProps->ClientDLL = "Build/Binaries/Sandbox/Sandbox.dll";
    
    mageApplicationInitialise(Application, *ApplicationProps, *RendererProps);
    mageApplicationRun(Application);


    free(Application);
    free(ApplicationProps);
    free(RendererProps);
    
}






