#include <Mage.h>

static U8 TestFileSystemLoadDataFromMounted(U0 *data)
{
    MageFileSystem f = *(MageFileSystem *)data; 
    U32 index = 0;
    
    MageFileSystemMountInfo mi;
    memset(&mi, 0, sizeof(MageFileSystemMountInfo));
    mi.MountIndex = &index;
    mi.MountPoint = "Mage";

    MageFileSystemMountDirectory(&f, &mi);
    printf("Inform: Mounted directory Mage, searching for Mage.h [Mage/Mage.h]\n");


    char *d = malloc(sizeof(char) * 1);

    MageFileSystemReadInfo ri;
    memset(&ri, 0, sizeof(MageFileSystemReadInfo));
    ri.FilePath = "Mage.h";
    ri.SearchOverride = MageTrue;
    ri.MountPointIndex = index;
    ri.StreamData = d;
    ri.StreamReallocatable = MageTrue;
    ri.StreamSize = 1;


    U8 r = MageFileSystemReadMountedDirectory(&f, &ri);
    free(d);
    return r;
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
