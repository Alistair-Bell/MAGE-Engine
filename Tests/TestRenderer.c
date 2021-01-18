#include "CommonResources.h"

static U8 TestSurfaceCreation(U0 *data)
{
    return MageTrue;    
}

I32 main()
{

    MageUnitTestCreateInfo tests[0];
    memset(tests, 0, sizeof(tests));
    TestSurfaceCreation(tests);
    return MageTrue;
}
