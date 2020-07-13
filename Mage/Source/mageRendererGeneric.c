#include "mageAPI.h"

mageShaderType mageShaderTypeFromString(const char *name)
{
    if (strcmp(name, "vertex")) return MAGE_SHADER_TYPE_VERTEX;
    if (strcmp(name, "fragement")) return MAGE_SHADER_TYPE_FRAGMENT;

    MAGE_LOG_CORE_ERROR("%s is not a recognised shader type\n", name);
    return MAGE_SHADER_TYPE_VERTEX;
}