#include "mageAPI.h"

void main(int32_t argc, char **args)
{  
    struct mageApplication App;
    struct mageApplicationProps Props;
    memset(&Props, 0, sizeof(struct mageApplicationProps));

    Props.Name      = "Hello World Program";
    Props.Width     = 1920;
    Props.Height    = 1080;
    Props.Version   = 1.0;
    Props.ClientDLL = "Build/Binaries/Sandbox/Sandbox.dll";
    
    mageApplicationInitialise(&App, Props);
    mageApplicationRun(&App);
}






