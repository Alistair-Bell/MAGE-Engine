#include <Mage.h>

static U8 TestFileSystemLoadDataFromMounted(U0 *data)
{
    MageFileSystem f = *(MageFileSystem *)data; 
    U32 index = 0;
    MageFileSystemMountDirectory(&f, "Mage", &index);
    printf("Inform: Mounted directory Mage, searching for Mage.h [Mage/Mage.h]\n");
    return MageFileSystemReadMountedDirectory(&f, "Mage.h", MageTrue, index); 
}


I32 main()
{
    MageFileSystem f;
    MageFileSystemCreateInfo info;
    memset(&info, 0, sizeof(MageFileSystemCreateInfo));

    MageFileSystemCreate(&info, &f);

    MageUnitTestCreateInfo tests[1];
    memset(tests, 0, sizeof(tests));
    U64 cnt = sizeof(tests) / sizeof(MageUnitTestCreateInfo);
    tests[0].TestName       = "`Unit Tests` - file system find file test";
    tests[0].FailMessage    = "`File System Loader` module has failed";
    tests[0].ProgramData    = &f;
    tests[0].Callback       = TestFileSystemLoadDataFromMounted;
    tests[0].ExpectedResult = MageTrue;
    tests[0].Assertions     = MageFalse;

    MageUnitTestRuntimeInfo ri;
    return MageUnitTestRunTests(tests, cnt, &ri);

}
