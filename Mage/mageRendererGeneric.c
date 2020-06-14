#include "mageAPI.h"

void *mageRendererAllocate()
{
    return malloc(sizeof(struct mageRenderer));
}
