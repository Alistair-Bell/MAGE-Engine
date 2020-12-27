#include "FileSystem.h"

U8 MageFileSystemCreate(MageFileSystemCreateInfo *info, MageFileSystem *system)
{
    memset(system, 0, sizeof(MageFileSystem));
    system->Mounted = calloc(0, sizeof(const char *));
    system->NextIndex = UINT32_MAX;
    return MageTrue;
}
U8 MageFileSystemMountDirectory(MageFileSystemMountInfo *info, MageFileSystem *system)
{
    U32 cnt = system->MountCount;
    system->MountCount++;
    if (system->NextIndex != UINT32_MAX)
    {
        system->Mounted[system->NextIndex] = info->MountPoint;
        if (info->MountIndex != NULL)
            *info->MountIndex = system->NextIndex;
        system->NextIndex = UINT32_MAX;
        return MageTrue;
    }
    system->Mounted = realloc(system->Mounted, sizeof(const char *) * cnt + 1);
    system->Mounted[cnt] = info->MountPoint;
    if (info->MountIndex != NULL)
        *info->MountIndex = cnt;
    return MageTrue;
}
U8 MageFileSystemReadMountedDirectory(MageFileSystemReadInfo *info, MageFileSystem *system)
{    
    FILE *f;
    char buffer[255];
    memset(buffer, 0, sizeof(buffer));

    if (info->SearchOverride)
    {
        U32 overrideIndex = info->MountPointIndex;
        MAGE_HANDLE_ERROR_MESSAGE(system->MountCount < overrideIndex, printf("Error: Filesystem has too little mounted for valid override index, %d mounted\n", system->MountCount));
        sprintf(buffer, "%s/%s", system->Mounted[overrideIndex], info->FilePath);
        f = fopen(buffer, "r");
        MAGE_HANDLE_ERROR_MESSAGE(f == NULL, printf("Error: Filesystem was unable to open virtual file %s\n", buffer));
    }

    U32 i, foundFile = MageFalse;
    for (i = 0; i < system->MountCount; i++)
    {
        sprintf(buffer, "%s/%s", system->Mounted[i], info->FilePath);
        f = fopen(buffer, "r");
        if (f != NULL)
        {
            foundFile = MageTrue;
            break;
        }
    }

    MAGE_HANDLE_ERROR_MESSAGE(!foundFile, printf("Error: Unable to find requested [%s] file in filesystem\n", info->FilePath));
    fseek(f, 0, SEEK_END); 
    info->StreamSize = ftell(f);
    info->StreamData = malloc(info->StreamSize + 1);

    fseek(f, 0, SEEK_SET);
    fread(info->StreamData, 1, info->StreamSize, f);
    fclose(f);
    return MageTrue;
}
U8 MageFileSystemUnmountDirectory(MageFileSystem *system, const char *mountPoint)
{
    MAGE_HANDLE_ERROR_MESSAGE(system->MountCount <= 0, printf("Error: Filesystem has not mounted directories active\n"));
    U32 i;
    
    for (i = 0; i < system->MountCount; i++)
    {
        if (strcmp(mountPoint, system->Mounted[i]) == 0)
            goto unmount;
    }
    
    MAGE_HANDLE_ERROR_MESSAGE(0 == 0, printf("Error: Unable to find mounted directory of %s in filesystem\n", mountPoint));
    

    unmount:
    {
        system->NextIndex = i;
        system->Mounted[i] = NULL;
        system->MountCount--;
    }

    return MageTrue;
}
U8 MageFileSystemDestory(MageFileSystem *system)
{
    
    free(system->Mounted);
    return MageTrue;
}
