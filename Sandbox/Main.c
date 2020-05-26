#include "../Mage/mageAPI.h"

static mageApplication app;
static mageApplicationProps props;

int main(int32_t argc, char **args)
{   
    if (mageEngineInitialise() != MAGE_SUCCESS) 
        return -1;

    props.Height = 720;
    props.Width = 1080;
    props.Version = 1.0f;
    props.Name = "MAGE Engine";

    mageApplicationInitialise(&app, &props);


    mageLogEnd();
    return 0;
}
