#include "../FileSystem.h"


U8 MageFileSystemGetUserHomeDirectroy(char *buffer, const U32 lenght)
{
    uid_t uid          = geteuid();
    struct passwd *pw  = getpwuid(uid);
    MAGE_HANDLE_ERROR_MESSAGE(pw == NULL, printf("Error: Unable to find program UID\n"));
    strncpy(buffer, pw->pw_dir, lenght);
    return MageTrue;
}
U8 MageFileSystemSetWorkingDirectory(const char *directory)
{
    chdir(directory);
    return MageTrue;
}


