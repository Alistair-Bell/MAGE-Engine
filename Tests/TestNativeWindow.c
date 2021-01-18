#include "CommonResources.h"

static U8 TestNativeWindowCreate(U0 *data)
{
    MageApplicationWindow *window = ((MageApplicationWindow *)data);
    memset(window, 0, sizeof(MageApplicationWindow));

    MageApplicationWindowCreateInfo windowCreateInfo;
    CommonPopulateWindow(&windowCreateInfo);
    U8 result = MageApplicationWindowCreate(&windowCreateInfo, window);
    return result;
}
static U8 TestNativeWindowGetDimensions(U0 *data)
{
    MageApplicationWindow *w = ((MageApplicationWindow *)data);
    MageApplicationWindowDimensions d;
    memset(&d, 0, sizeof(MageApplicationWindowDimensions));

    U8 r = MageApplicationWindowGetDimensions(w, &d);
    return (r && (d.Width == COMMON_WINDOW_WIDTH || d.Height == COMMON_WINDOW_HEIGHT));
}
static U8 TestNativeWindowSetTitle(U0 *data)
{
    MageApplicationWindow *w = ((MageApplicationWindow *)data);
    return MageApplicationWindowSetTitle(w, "Set From Test");
}

I32 main()
{
    MageUnitTestCreateInfo tests[3];
    memset(&tests, 0, sizeof(tests));

    MageApplicationWindow window;
    memset(&window, 0, sizeof(MageApplicationWindow));
    tests[0].TestName               = "`Unit Tests` - window creation test";
    tests[0].FailMessage            = "`Window Framework` module has failed";
    tests[0].ProgramData            = &window;
    tests[0].ExpectedResult         = MageTrue;
    tests[0].Assertions             = MageTrue;
    tests[0].Callback               = TestNativeWindowCreate;

    tests[1].TestName               = "`Unit Tests` - window dimension retrieval test";
    tests[1].FailMessage            = "`Window Framework` module has failed";
    tests[1].ProgramData            = &window;
    tests[1].ExpectedResult         = MageTrue;
    tests[1].Assertions             = MageFalse;
    tests[1].Callback               = TestNativeWindowGetDimensions;

    tests[2].TestName               = "`Unit Tests` - window title update";
    tests[2].FailMessage            = "`Window Framework` module has failed";
    tests[2].ProgramData            = &window;
    tests[2].ExpectedResult         = MageTrue;
    tests[2].Assertions             = MageFalse;
    tests[2].Callback               = TestNativeWindowSetTitle;

    MageUnitTestRuntimeInfo runtimeInfo;
    return (MageUnitTestRunTests(tests, sizeof(tests) / sizeof(MageUnitTestCreateInfo), &runtimeInfo));
}
