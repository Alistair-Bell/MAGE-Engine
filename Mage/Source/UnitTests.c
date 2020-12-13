#include "UnitTests.h"

U8 MageUnitTestRunTests(MageUnitTestCreateInfo *tests, const U64 count, MageUnitTestRuntimeInfo *runtime)
{
    MAGE_HANDLE_ERROR_MESSAGE(tests == NULL || runtime == NULL, printf("Error: Any passed parameters are NULL!\n"));
    MAGE_HANDLE_ERROR_MESSAGE(count <= 0, printf("Error: No tests specified\n"));

    U64 i;
    printf("Running %lu tests\n", count);
    
    for (i = 0; i < count; i++)
    {
        MageUnitTestCreateInfo *current = &tests[i];
        U8 testResult = current->Callback(current->ProgramData);

        if (testResult != current->ExpectedResult)
        {
            printf("%s has failed\n%s", current->TestName, current->FailMessage);
            if (current->Assertions)
                assert(testResult != current->ExpectedResult);
            runtime->FailCount++;
        }
        else
        {
            printf("%s has run successfully\n", current->TestName);
            runtime->SuccessCount++;
        }
    }

    if (runtime->FailCount <= 0)
        return 0;
    else
        return 1;
}