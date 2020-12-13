#include <Mage.h>
static U8 TestNativeWindowCreate(U0 *data)
{
    MageApplicationWindow window;
    memset(&window, 0, sizeof(MageApplicationWindow));

    MageApplicationWindowCreateInfo windowCreateInfo;
    memset(&windowCreateInfo, 0, sizeof(MageApplicationWindowCreateInfo));
    windowCreateInfo.Title          = "Native Window";
    windowCreateInfo.Resisable      = 0;
    windowCreateInfo.Width          = 1920;
    windowCreateInfo.Height         = 1080;
    U8 result = MageApplicationWindowCreate(&windowCreateInfo, &window);
    return result;
}

I32 main()
{
    MageUnitTestCreateInfo tests[1];
    memset(&tests, 0, sizeof(tests));

    tests[0].TestName               = "`Unit Tests` - window framework test";
    tests[0].FailMessage            = "`Window Framework` module has failed";
    tests[0].ProgramData            = NULL;
    tests[0].ExpectedResult         = MageTrue;
    tests[0].Assertions             = MageFalse;
    tests[0].Callback               = TestNativeWindowCreate;

    MageUnitTestRuntimeInfo runtimeInfo;
    memset(&runtimeInfo, 0, sizeof(MageUnitTestRuntimeInfo));

    
    return (MageUnitTestRunTests(tests, sizeof(tests) / sizeof(MageUnitTestCreateInfo), &runtimeInfo));

    return 0;
}