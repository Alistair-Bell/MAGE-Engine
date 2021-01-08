#ifndef MAGE_H
#define MAGE_H

#include "Source/Includes.h"
#include "Source/CreateStructures.h"
#include "Source/Window.h"
#include "Source/UnitTests.h"
#include "Source/InputHandler.h"
#include "Source/EngineApplication.h"
#include "Source/FileSystem.h"
#include "Source/Assets/AssetsLoader.h"
#include "Source/Threading.h"
#include "Source/Renderer.h"

#ifdef MAGE_BUILD_RENDERER_VULKAN
    #include "Source/Vulkan/VulkanRenderer.h"
#endif

#endif
