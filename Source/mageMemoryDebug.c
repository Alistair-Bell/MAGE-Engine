#include "mageAPI.h"

void *mageMallocDebug(const uint64 size, const char *file, const uint32 line)
{
    printf("Called debug\n");
}
void *mageCallocDebug(const uint64 items, const uint64 size, const char *file, const uint32 line)
{
    printf("Called debug\n");
}
void *mageReallocDebug(void *memory, const uint64 size, const char *file, const uint32 line)
{
	printf("Called debug\n");
}
