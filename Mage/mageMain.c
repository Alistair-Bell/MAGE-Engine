#include "mageAPI.h"




static void clientUpdate()
{

}
static mageApplicationProps clientCreateProps()
{
    mageApplicationProps props;

    props.Name                   = "Vulkan GLFW";
    props.Width                  = 1080;
    props.Height                 = 720;
    props.Version                = 1.0;
    props.StartMethod            = NULL;
    props.UpdateMethod           = clientUpdate;
    props.DestroyMethod          = NULL;
    props.AllocationCallback     = NULL;
    props.DestroyMethod          = NULL;
    return props;
}

static mageApplication App;

#if defined(MAGE_MONO_EXTERNALS)
    static mageMonoHandler MonoHandler;
#endif

int32_t main(int32_t argc, char **args)
{
    mageEngineInitialise(); 

    /*    const mageApplicationProps props = clientCreateProps();
    mageApplicationInitialise(&App, &props);
    mageApplicationRun(&App);*/

    #if defined(MAGE_MONO_EXTERNALS)
        mageMonoHandlerInitialise(&MonoHandler, "Build/Binaries/Sandbox/Sandbox.dll");
    #endif

    MonoClass *class = mono_class_from_name(MonoHandler.Image, "Client", "Example");

    MonoMethod *method = mageMonoHandlerFindMethod(class, "Start"); 
    
    MonoObject *object = mono_object_new(MonoHandler.Domain, class);
    
    
    mono_runtime_object_init(object);

    mono_runtime_invoke(method, object, NULL, NULL);
    
    mageLogEnd();
}






