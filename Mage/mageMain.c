#include "mageAPI.h"

struct mageApplication          *Application;
struct mageApplicationProps     ApplicationProps;
struct mageRendererProps        RendererProps;

/* Implimented by client */
extern struct mageApplicationProps ClientApplicationProps();
extern struct mageRendererProps ClientRendererProps();


int32_t main(int32_t argc, char **args)
{  
#if defined (MAGE_DEBUG)
    mageLogInitialise("Logs/mage.log");
#endif
    Application = malloc(sizeof(struct mageApplication));

    memset(Application, 0, sizeof(struct mageApplication));
    memset(&ApplicationProps, 0, sizeof(struct mageApplicationProps));
    memset(&RendererProps, 0, sizeof(struct mageRendererProps));

    ApplicationProps.Name = "Hello World";
    ApplicationProps.Version = 1.0;
    ApplicationProps.Height = 1080;
    ApplicationProps.Width = 1920;

    RendererProps.RuntimeShaders = NULL;
    RendererProps.ShaderCount = 0;


    mageApplicationInitialise(Application, ApplicationProps, RendererProps);


#if defined (MAGE_DEBUG)
    mageLogEnd();
#endif
    return 1;
}






