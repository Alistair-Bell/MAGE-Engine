#ifndef __RESOURCE_LOADER__
#define __RESOURCE_LOADER__

#include "Sandbox.h"

void SetupLoader(
    const char *file
);

void LoadEngineConfig(
    uint32_t *width,
    uint32_t *height,
    double *version,
    char *title
);






#endif

