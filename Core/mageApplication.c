#include "mageAPI.h"

void *mageApplicationAllocate()
{
    return malloc(sizeof(struct MAGE_APPLICATION_STRUCT));
}
void mageApplicationInitialise(mageApplication *application, const mageApplicationProps *props, uint8 *success)
{
    application->Renderer = mageRendererAllocate();
    application->Window = mageWindowAllocate();

    uint8 flag;

    char *temp = malloc(sizeof(char) * strlen(props->Name) + 20);
    sprintf(temp, "%s : Version %.2f", props->Name, props->Version);
    mageWindowInitialise(application->Window, props->Width, props->Height, temp, &flag);
    
    if (!flag)
    {
        mageTryDumpSuccess(0, success);
        return;
    }
    
    mageRendererInitialise(application->Renderer, application->Window, &flag);

    if (!flag)
    {
        mageTryDumpSuccess(0, success);
        return;
    }


    
    mageFreeMethod(temp);
}