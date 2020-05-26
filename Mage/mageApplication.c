#include "mageAPI.h"

void *mageApplicationAllocate()
{
    return malloc(sizeof(struct MAGE_APPLICATION_STRUCT));
}
mageResult mageApplicationInitialise(mageApplication *application, const mageApplicationProps *props)
{
    application->Renderer = mageRendererAllocate();
    application->Window = mageWindowAllocate();

    char temp[255];
    sprintf(temp, "%s : Version %.2f", props->Name, props->Version);
        
    mageResult result = mageWindowInitialise(application->Window, props->Width, props->Height, temp);
    if (result != MAGE_SUCCESS)
    {
        return result;
    }

    result = mageRendererInitialise(application->Renderer, application->Window);
    
    if (result != MAGE_SUCCESS)
    {
        return result;
    }
    return MAGE_SUCCESS;
}