
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
    default     = "pulse",
    allowed     = {
        { "pulse", "Pulse Audio 13.99.1" },
    }
}
newoption
{
    trigger     = "tests",
    value       = "build",
    description = "Build unit tests for the engine",
    default     = "none",
    allowed     = {
        { "none", "Disables building tests" },
        { "all", "Builds all unit tests" },
    },
}

-- Table of libraries that should be linked againsed
ConfigurationLinks = {}
-- Renderers
ConfigurationLinks["vulkan"]        = "vulkan"
ConfigurationLinks["gles"]          = "glad"
-- Audio
ConfigurationLinks["pulse"]         = "pulse"

-- Table of macros that should be defined
ConfigurationDefines = {}
-- Renderers
ConfigurationDefines["vulkan"]  = "MAGE_VULKAN_BACKEND"
ConfigurationDefines["gles"]    = "MAGE_GLES_BACKEND"
-- Audio
ConfigurationDefines["pulse"]   = "MAGE_PULSE_AUDIO_BACKEND"

-- Table of files to include
ConfigurationFiles  = {}
ConfigurationFiles["vulkan"]    = "Mage/Source/Vulkan/**.*"
ConfigurationFiles["gles"]      = "Mage/Source/OpenGL/**.*"
ConfigurationFiles["pulse"]     = "Mage/Source/Pulse-Audio/**.*"

PlatformDefines     = {}
PlatformDefines["windows"]      = { "MAGE_PLATFORM_WINDOWS", "GLFW_EXPOSE_NATIVE_WIN32" }
PlatformDefines["linux"]        = { "MAGE_PLATFORM_LINUX", "GLFW_EXPOSE_NATIVE_X11" }
PlatformDefines["macosx"]       = { "MAGE_PLATFORM_MAC_OS", "GLFW_EXPOSE_NATIVE_COCOA" }

ClientLinks =
{
    "MageEngine",
    "GLFW",
    "stb-image",
    ConfigurationLinks[_OPTIONS["renderer"]],
    ConfigurationLinks[_OPTIONS["audio-backend"]],
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
    
    targetdir (BuildTargetPath)
    objdir (BuildObjectPath)

    files
    {
        -- Files all systems use 
        "Mage/Source/*.h",
        "Mage/Source/*.c",
        "Mage/Source/Maths/*.h",
        "Mage/Source/Maths/*.c",
        "Mage/Source/ECS/*.h",
        "Mage/Source/ECS/*.c",
        ConfigurationFiles[_OPTIONS["renderer"]],
        ConfigurationFiles[_OPTIONS["audio-backend"]],
        ConfigurationFiles[_OPTIONS["scripting-language"]]   
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

filter "system:linux"
    pchheader "Mage/Source/mageAPI.h"
    pchsource "Mage/Source/mageAPI.c"

    files
    {
        "Mage/Source/Linux/*.h",
        "Mage/Source/Linux/*.c",
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