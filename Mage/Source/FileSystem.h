#ifndef MAGE_FILESYSTEM_H
#define MAGE_FILESYSTEM_H

#include "Includes.h"
#include "CreateStructures.h"

typedef struct MageFileSystemMountInfo
{
    const char  *MountPoint;
    U32         *MountIndex;
} MageFileSystemMountInfo;

typedef struct MageFileSystemReadInfo
{
    U8          SearchOverride;
    U32         MountPointIndex;
    U64         StreamSize;
    char        *StreamData;
    const char  *FilePath;
} MageFileSystemReadInfo;

typedef struct MageFileSystem
{
    const char **Mounted;
    U32 NextIndex;
    U32 MountCount;
} MageFileSystem;

extern U8 MageFileSystemCreate(MageFileSystemCreateInfo *info, MageFileSystem *system);
extern U8 MageFileSystemMountDirectory(MageFileSystem *system, MageFileSystemMountInfo *info);
extern U8 MageFileSystemReadMountedDirectory(MageFileSystem *system, MageFileSystemReadInfo *info);
extern U8 MageFileSystemUnmountDirectory(MageFileSystem *system, const char *mountPoint);
extern U8 MageFileSystemDestory(MageFileSystem *system);

#endif
