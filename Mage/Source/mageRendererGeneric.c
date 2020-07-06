#include "mageAPI.h"

mageShaderType mageShaderTypeFromString(const char *name)
{
    if (strcmp(name, "vertex")) return MAGE_VERTEX_SHADER;
    if (strcmp(name, "fragement")) return MAGE_FRAGMENT_SHADER;

    MAGE_LOG_CORE_ERROR("%s is not a recognised shader type\n", name);
    return MAGE_FRAGMENT_SHADER;
}