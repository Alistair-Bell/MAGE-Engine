#include <mageAPI.h>

/* Application instance */
static struct mageApplication *HelloWorld;
/* Configuration for the application wrapped in a structure */
static struct mageApplicationCreateInfo ApplicationInfo;
/* Configuration for the renderer wrapped in a structure */
static struct mageRendererCreateInfo RendererInfo;

/* Allows for win32 main and default main in an easy package */
MAGE_ENTRY_POINT()
{   
    /* Cleaning the memory*/
    memset(&ApplicationInfo, 0, sizeof(struct mageApplicationCreateInfo));
    memset(&RendererInfo, 0, sizeof(struct mageRendererCreateInfo));

    /* Open up logging for the engine */
    mageLogInitialise("Logs/Hello-World.txt");

    /* Allocating application memory */
    HelloWorld = malloc(sizeof(*HelloWorld));
    
    /* Window will be unresizable */
    ApplicationInfo.FixedResolution         = 0;
    /* Window will be fullscreen */
    ApplicationInfo.Fullscreen              = 0;
    /* Width of the window */
    ApplicationInfo.Width                   = 1920;
    /* Height of the window */
    ApplicationInfo.Height                  = 1080;
    /* Name of the window */
    ApplicationInfo.Name                    = "Hello-World";
    /* Icon to be used (file path), NULL uses the default logo */
    ApplicationInfo.WindowIcon              = NULL;

    /* Shaders for the pipeline to use, specify null for default */
    RendererInfo.PipelineShaders            = NULL;
    /* Count of the pipeline shaders in use, if shaders were NULL then specify 0 */
    RendererInfo.ShaderCount                = 0;

    /* Create the application for use */
    mageResult result = mageApplicationCreate(HelloWorld, ApplicationInfo, RendererInfo);
    assert(result == MAGE_RESULT_SUCCESS);

    

    /* Give back what you borrow, good practice */
    free(HelloWorld);
    return 1;
}