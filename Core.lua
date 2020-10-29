BuildTargetPath = "Build/Binaries/%{prj.name}-%{cfg.buildcfg}"
BuildObjectPath = "Build/Objects/%{prj.name}-%{cfg.buildcfg}"

newoption 
{
    trigger     = "renderer",
    value       = "API",
    description = "Choose a particular rendering backend",
    default     = "opengl",
    allowed = {
       { "vulkan",    "Vulkan 1.2" },
       { "gles",  "OpenGL ES 2.0" },
    }
}
newoption
{
    trigger     = "audio-backend",
    value       = "API",
    description = "Choose a particular audio backend",
    default     = "none",
    allowed     = {
        { "none", "No audio backend" },
        { "openal", "OpenAL version 1.1" },
    }
}
newoption
{
    trigger     = "tests",
    value       = "build",
    description = "Build unit tests for the engine",
    default     = "none",
    allowed     = {
        { "no-tests", "Disables building tests" },
        { "all", "Builds all unit tests" },
    },
}

-- Table of libraries that should be linked againsed
ConfigurationLinks = {}
-- Renderers
ConfigurationLinks["vulkan"]        = "vulkan"
ConfigurationLinks["gles"]          = "glad"
-- Audio
ConfigurationLinks["openal"]        = "openal"

-- Table of macros that should be defined
ConfigurationDefines = {}
-- Renderers
ConfigurationDefines["vulkan"]  = "MAGE_VULKAN_BACKEND"
ConfigurationDefines["gles"]    = "MAGE_GLES_BACKEND"
-- Audio
ConfigurationDefines["openal"]   = "MAGE_OPENAL_BACKEND"

-- Table of files to include
ConfigurationFiles  = {}
ConfigurationFiles["vulkan"]    = "Mage/Source/Vulkan/**.*"
ConfigurationFiles["gles"]      = "Mage/Source/OpenGL/**.*"
ConfigurationFiles["openal"]    = "Mage/Source/OpenAL/**.*"

PlatformDefines     = {}
PlatformDefines["windows"]      = { "MAGE_PLATFORM_WINDOWS", "GLFW_EXPOSE_NATIVE_WIN32" }
PlatformDefines["linux"]        = { "MAGE_PLATFORM_LINUX", "GLFW_EXPOSE_NATIVE_X11" }
PlatformDefines["macosx"]       = { "MAGE_PLATFORM_MAC_OS", "GLFW_EXPOSE_NATIVE_COCOA" }

PlatformLinks       = {}
PlatformLinks["windows"]        = { "gdi32" }

PlatformFiles       = {}
PlatformFiles["windows"]     = "Mage/Source/Windows/**.*"
PlatformFiles["linux"]       = "Mage/Source/Linux/**.*"
PlatformFiles["macosx"]      = "Mage/Source/MacOS/**.*"

ClientLinks =
{
    "MageEngine",
    "GLFW",
    "stb-image",
    "dr_libs",
    ConfigurationLinks[_OPTIONS["renderer"]],
    ConfigurationLinks[_OPTIONS["audio-backend"]],
    PlatformLinks[_OPTIONS["os"]]
}

workspace "Mage"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
    }
    startproject "Sandbox"




group "Externals"
if _OPTIONS["renderer"] == "gles" then
    include "Mage/Externals/glad/"
end
include "Mage/Externals/stb-image"
include "Mage/Externals/glfw3"
include "Mage/Externals/dr_libs"

if _OPTIONS["tests"] == "all" then
    -- Include the tests to build
    group "Tests"
    --include "Tests/Tests.lua" TODO Add unit tests
end

-- Engine Project
project "MageEngine"
    location "Mage"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir (BuildTargetPath)
    objdir (BuildObjectPath)

    pchheader "Mage/Source/mageAPI.h"
    pchsource "Mage/Source/mageAPI.c"

    files
    {
        -- Files all systems use 
        "Mage/Source/*.h",
        "Mage/Source/*.c",
        "Mage/Source/Maths/*.h",
        "Mage/Source/Maths/*.c",
        "Mage/Source/ECS/*.h",
        "Mage/Source/ECS/*.c",
        PlatformFiles[_OPTIONS["os"]],
        ConfigurationFiles[_OPTIONS["renderer"]],
        ConfigurationFiles[_OPTIONS["audio-backend"]],
    }
    defines
    {
        "MAGE_CORE",
        "MAGE_ASSERTS",
        ConfigurationDefines[_OPTIONS["renderer"]],
        ConfigurationDefines[_OPTIONS["audio-backend"]],
        PlatformDefines[_OPTIONS["os"]],
    }
    includedirs
    {
        "Mage/Source/",
    }
    flags
    {
        "FatalWarnings",
        "FatalCompileWarnings",
        "FatalLinkWarnings",
    }

filter "configurations:Debug"
    defines "MAGE_DEBUG"
    runtime "Debug"
    symbols "On"
    

filter "configurations:Release"
    defines "MAGE_RELEASE"
    runtime "Release"
    optimize "Speed"


project "Sandbox"
    location "%{prj.name}"
    kind "ConsoleApp"
    language "C"
    targetdir (BuildTargetPath)
    objdir (BuildObjectPath)

    includedirs
    {
        "Mage/Source",
    }
    
    files
    {
        "%{prj.name}/**.h",   
        "%{prj.name}/**.c",
    }

    links
    {
        ClientLinks
    }
    
    filter "system:linux"
        links 
        { 
            "dl",
            "pthread",
            "m",
        }

    filter "configurations:Debug"
        defines "SANDBOX_DEBUG"    
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "SANDBOX_RELEASE"
        runtime "Release"
        optimize "Speed"
