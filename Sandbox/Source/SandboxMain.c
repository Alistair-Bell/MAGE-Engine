#include <mageAPI.h>

static struct mageApplication *SandboxApplication;

void ExampleListener(void *event, mageEventType type)
{
    if (type == MAGE_EVENT_APPLICATION_RENDER)
    {

    }
}

int main(const int32_t argumentCount, char **arguments)
{
    SandboxApplication = malloc(sizeof(struct mageApplication));
    
    
    






    free(SandboxApplication);
}