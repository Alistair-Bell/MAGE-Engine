#include "mageAPI.h"



static mageApplication App;

int32_t main(int32_t argc, char **args)
{
    mageApplicationProps props;
    memset(&props, 0, sizeof(mageApplicationProps));
    
    props.Name      = "Hello World Program";
    props.Width     = 1920;
    props.Height    = 1080;
    props.Version   = 1.0;
    props.ClientDLL = "Build/Binaries/Sandbox/Sandbox.dll";

    mageApplicationInitialise(&App, &props);


    /*
    MonoClass *class = mono_class_from_name(MonoHandler.Image, "Client", "Example");
    MonoMethod *method = mageMonoHandlerFindMethod(class, "Start"); 
    MonoObject *object = mono_object_new(MonoHandler.Domain, class);
    
    
    
    mono_runtime_object_init(object);

    mono_runtime_invoke(method, object, NULL, NULL);
    */  

    mageApplicationRun(&App);
    mageLogEnd();
}






