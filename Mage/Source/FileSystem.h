#ifndef MAGE_FILESYSTEM_H
#define MAGE_FILESYSTEM_H

#include "Includes.h"
#include "CreateStructures.h"

#define MAGE_FILE_SYSTEM_READ_MODE_RAW "r"
#define MAGE_FILE_SYSTEM_READ_MODE_BINARY "rb"

typedef struct MageFileSystemMountInfo
{
    const char  *MountPoint;
    U32         *MountIndex;
} MageFileSystemMountInfo;

typedef struct MageFileSystemReadInfo
{
    U8                     SearchOverride;
    U32                    MountPointIndex;
    U64                    StreamSize;
    char                   *StreamData;
    const char             *FilePath;
    const char             *ReadMode;
} MageFileSystemReadInfo;

typedef struct MageFileSystem
{
    const char **Mounted;
    U32 NextIndex;
    U32 MountCount;
} MageFileSystem;

extern U8 MageFileSystemGetUserHomeDirectroy(char *buffer, const U32 lenght); /* uses uid to get the home directory on linux, window it goes C:\Users\user\Documents  */
extern U8 MageFileSystemSetWorkingDirectory(const char *directory);

extern U8 MageFileSystemCreate(MageFileSystemCreateInfo *info, MageFileSystem *system);
extern U8 MageFileSystemMountDirectory(MageFileSystemMountInfo *info, MageFileSystem *system);
extern U8 MageFileSystemReadMountedDirectory(MageFileSystemReadInfo *info, MageFileSystem *system);
extern U8 MageFileSystemUnmountDirectory(MageFileSystem *system, const char *mountPoint);
extern U8 MageFileSystemDestory(MageFileSystem *system);

#endif
