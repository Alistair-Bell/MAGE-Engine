#include "../Core/mageAPI.h"

static mageApplication app;
static mageApplicationProps props;

int main(int32_t argc, char **args)
{   
    mageEngineInitialise(NULL);

    props.Height = 720;
    props.Width = 1080;
    props.Version = 1.0f;
    props.Name = "MAGE Engine";

    mageApplicationInitialise(&app, &props, NULL);


    mageLogEnd();
    return 0;
}
