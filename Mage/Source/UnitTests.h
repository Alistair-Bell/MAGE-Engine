#ifndef UNIT_TESTS_H
#define UNIT_TESTS_H

#include "Includes.h"

typedef U8 (*MageUnitTestCallback)(U0 *data);

typedef struct MageUnitTestCreateInfo
{
    const char           *TestName;
    const char           *FailMessage;
    U0                   *ProgramData;
    U8                   Assertions;
    U8                   ExpectedResult;
    MageUnitTestCallback Callback;
} MageUnitTestCreateInfo;

typedef struct MageUnitTestRuntimeInfo
{
    U64             SuccessCount;
    U64             FailCount;
} MageUnitTestRuntimeInfo;

extern U8 MageUnitTestRunTests(MageUnitTestCreateInfo *tests, const U64 count, MageUnitTestRuntimeInfo *runtime);

#endif