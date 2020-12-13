#include <Mage.h>

#define CHECK_FUNCTION(method) if (!method) return MageFalse;

I32 main()
{
    MageEngineApplication engineContext;
    
    MageApplicationWindowCreateInfo windowCreateInfo;
    memset(&windowCreateInfo, 0, sizeof(MageApplicationWindowCreateInfo));
    windowCreateInfo.Title = "Hello World";
    windowCreateInfo.Height = 1080;
    windowCreateInfo.Width  = 1920;


    MageEngineApplicationCreateInfo engineCreateInfo;
    memset(&engineCreateInfo, 0, sizeof(MageEngineApplicationCreateInfo));
    engineCreateInfo.ApplicationWindowCreateInfo = windowCreateInfo;

    MageEngineApplicationCreate(&engineCreateInfo, &engineContext);


    MageEngineApplicationDestroy(&engineContext);
    printf("Ran successfully\n");


    return MageTrue;
}