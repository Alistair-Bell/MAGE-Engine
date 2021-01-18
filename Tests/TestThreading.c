#include "CommonResources.h"

static MAGE_THREAD_RETURN_VALUE addition(U0 *data)
{
    return MAGE_THREAD_RETURN_SUCCESS;
}

static U8 TestThreadCreateAndEnd(U0 *data)
{
    MageThread t;

    MageThreadCreateInfo ti;
    memset(&ti, 0, sizeof(MageThreadCreateInfo));
    ti.Callback     = addition;
    ti.CallbackData = &ti;
    if (!MageThreadCreate(&ti, &t)) return MageFalse;
    return MageThreadDestroy(&t);
}

I32 main()
{
    MageUnitTestCreateInfo tests[1];
    memset(tests, 0, sizeof(tests));

    tests[0].TestName               = "`Unit Tests` - thread creation test";
    tests[0].FailMessage            = "`Threading Framework` module has failed";
    tests[0].ProgramData            = NULL;
    tests[0].ExpectedResult         = MageTrue;
    tests[0].Assertions             = MageFalse;
    tests[0].Callback               = TestThreadCreateAndEnd; 
    
    MageUnitTestRuntimeInfo runtimeInfo;
    return MageUnitTestRunTests(tests, sizeof(tests) / sizeof(MageUnitTestCreateInfo), &runtimeInfo);
}
