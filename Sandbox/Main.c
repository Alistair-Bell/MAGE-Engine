#include "../Mage/mageAPI.h"

static mageApplication app;
static mageApplicationProps props;


static mageResult start(mageApplication *application)
{
    return MAGE_SUCCESS;
}
static void update(mageApplication *application)
{

}
static mageResult destroy(mageApplication *application)
{
    return MAGE_SUCCESS;
}

int main(int32_t argc, char **args)
{   
    if (mageEngineInitialise() != MAGE_SUCCESS) 
        return -1;



    props.Height = 720;
    props.Width = 1080;
    props.Version = 1.0f;
    props.Name = "MAGE Engine";
    props.StartMethod = start;
    props.UpdateMethod = update;
    props.DestroyMethod = destroy;

    mageApplicationInitialise(&app, &props);
    mageApplicationRun(&app);

    mageLogEnd();
    return 0;
}
