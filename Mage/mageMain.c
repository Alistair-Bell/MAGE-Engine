#include "mageAPI.h"

struct mageApplication          *Application;
struct mageApplicationProps     ApplicationProps;
struct mageRendererProps        RendererProps;

/* Implimented by client */
extern void ClientLoadProps(struct mageApplicationProps *applicationProps, struct mageRendererProps *rendererProps);


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

    ClientLoadProps(&ApplicationProps, &RendererProps);

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
    free(RendererProps.RuntimeShaders);
    return 1;
}






