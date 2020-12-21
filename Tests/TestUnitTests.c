#include <Mage.h>

U8 UnitTestsModuleTestSucceed(U0 *data)
{
    return MageTrue;
}
U8 UnitTestsModuleTestFailure(U0 *data)
{
    return MageFalse;
}

I32 main()
{
    MageUnitTestCreateInfo tests[1];
    memset(&tests, 0, sizeof(tests));

    tests[0].TestName               = "`Unit Tests` - module test";
    tests[0].FailMessage            = "`Unit Tests` module has failed";
    tests[0].ProgramData            = NULL;
    tests[0].ExpectedResult         = MageTrue;
    tests[0].Assertions             = MageFalse;
    tests[0].Callback               = UnitTestsModuleTestSucceed;

    tests[0].TestName               = "`Unit Tests` - module test";
    tests[0].FailMessage            = "`Unit Tests` module has failed";
    tests[0].ProgramData            = NULL;
    tests[0].ExpectedResult         = MageTrue;
    tests[0].Assertions             = MageFalse;
    tests[0].Callback               = UnitTestsModuleTestSucceed;

    MageUnitTestRuntimeInfo runtimeInfo;
    return (MageUnitTestRunTests(tests, sizeof(tests) / sizeof(MageUnitTestCreateInfo), &runtimeInfo));
}