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

    Application      = malloc(sizeof(struct mageApplication));
    ApplicationProps = ClientApplicationProps();
    RendererProps    = ClientRendererProps();

    /* Use default shaders */
    if (RendererProps.RuntimeShaders == NULL || RendererProps.ShaderCount == 0)
    {
        MAGE_LOG_CORE_WARNING("Using default shaders, client specified none\n", NULL);
        struct mageShader shaders[2];
        struct mageShader defaultVertex, defaultFragment;
        mageShaderInitialise(&defaultVertex, "Mage/Resources/Shaders/vertex.sprv", "main", MAGE_VERTEX_SHADER);
        mageShaderInitialise(&defaultFragment, "Mage/Resources/Shaders/fragment.sprv", "main", MAGE_FRAGMENT_SHADER);
        shaders[0] = defaultVertex;
        shaders[1] = defaultFragment;

        RendererProps.ShaderCount = 2;
        RendererProps.RuntimeShaders = shaders;
    }
    mageApplicationInitialise(Application, ApplicationProps, RendererProps);
    mageApplicationRun(Application); 
    free(Application);

#if defined (MAGE_DEBUG)
    mageLogEnd();
#endif

    return 1;
}






