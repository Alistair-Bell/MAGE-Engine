#ifndef MAGE_FILESYSTEM_H
#define MAGE_FILESYSTEM_H

#include "Includes.h"
#include "CreateStructures.h"

typedef struct MageFileSystem
{
    const char **Mounted;
    U32 NextIndex;
    U32 MountCount;
} MageFileSystem;

extern U8 MageFileSystemCreate(MageFileSystemCreateInfo *info, MageFileSystem *system);
extern U8 MageFileSystemMountDirectory(MageFileSystem *system, const char *mountPoint, U32 *mountIndex);
extern U8 MageFileSystemReadMountedDirectory(MageFileSystem *system, const char *file, const U8 searchOverride, const U32 overrideIndex);
extern U8 MageFileSystemUnmountDirectory(MageFileSystem *system, const char *mountPoint);
extern U8 MageFileSystemDestory(MageFileSystem *system);

#endif
