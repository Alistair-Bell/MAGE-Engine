#include "mageAPI.h"

struct mageApplication          *Application;
struct mageApplicationProps     ApplicationProps;
struct mageRendererProps        RendererProps;

/* Implimented by client */
extern struct mageApplicationProps ClientApplicationProps();
extern struct mageRendererProps ClientRendererProps();


static struct mageShader mageDefaultShaders[2];


int32_t main(int32_t argc, char **args)
{  
#if defined (MAGE_DEBUG)
    mageFileDumpContents("Logs/mage.log", "", 1);
    mageLogInitialise("Logs/mage.log");
#endif
    Application = malloc(sizeof(struct mageApplication));

    memset(Application, 0, sizeof(struct mageApplication));
    memset(&ApplicationProps, 0, sizeof(struct mageApplicationProps));
    memset(&RendererProps, 0, sizeof(struct mageRendererProps));

    ApplicationProps    = ClientApplicationProps();
    RendererProps       = ClientRendererProps();

    if (RendererProps.ShaderCount <= 0 || RendererProps.RuntimeShaders == NULL)
    {
        MAGE_LOG_CORE_WARNING("Using default shaders, none specified\n", NULL);
        struct mageShader fragment, vertex;
    }



    mageApplicationInitialise(Application, ApplicationProps, RendererProps);
    mageApplicationRun(Application);

#if defined (MAGE_DEBUG)
    mageLogEnd();
#endif
    return 1;
}






