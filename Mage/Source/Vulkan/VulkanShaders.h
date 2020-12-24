#ifndef VULKAN_SHADER_H
#define VULKAN_SHADER_H

#include "../Includes.h"
#include "../FileSystem.h"
#include "VulkanRenderer.h"

typedef enum MageVulkanShaderType
{
    MAGE_VULKAN_SHADER_TYPE_FRAGMENT,
    MAGE_VULKAN_SHADER_TYPE_VERTEX,
} MageVulkanShaderType;


typedef struct MageVulkanShader
{
    VkShaderModule RawModule;
} MageVulkanShader;

typedef struct MageVulkanShaderCreateInfo
{
    MageVulkanShaderType Type;
    MageFileSystem       MountedFileSystem;
    const char           *FilePath;
} MageVulkanShaderCreateInfo;

extern U8 MageVulkanShaderCreate(MageVulkanShaderCreateInfo *info, MageVulkanShader *shader);
extern U8 MageVulkanShaderDestroy(MageVulkanShader *shader);

#endif
