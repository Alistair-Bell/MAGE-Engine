#include "mageAPI.h"


int32_t main(int32_t argc, char **args)
{
    mageApplication App;
    mageApplicationProps props;
    memset(&props, 0, sizeof(mageApplicationProps));
    
    props.Name      = "Hello World Program";
    props.Width     = 1920;
    props.Height    = 1080;
    props.Version   = 1.0;
    props.ClientDLL = "Build/Binaries/Sandbox/Sandbox.dll";

    mageApplicationInitialise(&App, &props);

    mageApplicationRun(&App);
    
}






