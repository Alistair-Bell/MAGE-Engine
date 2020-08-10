
BuildTargetPath = "Build/Binaries/%{prj.name}-%{cfg.buildcfg}"
BuildObjectPath = "Build/Objects/%{prj.name}-%{cfg.buildcfg}"

workspace "MAGE"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
    }

    startproject "Sandbox"

group "Externals"
    include "Mage/Externals/glfw3"
    include "Mage/Externals/libyaml"
    include "Mage/Externals/libogg"
    include "Mage/Externals/stb-image"

    
    
-- Engine Project
project "MageEngine"
    location "Mage"
    kind "StaticLib"
    language "C"
    cdialect "C89"

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
        "Mage/Source/Renderer/*.h",
        "Mage/Source/Renderer/*.c",
    }
    defines
    {
        "MAGE_CORE",
        "MAGE_ASSERTS"
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
    cdialect "C89"
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
        "MageEngine",
        "GLFW",
        "vulkan",
        "yaml",
        "libogg",
        "stb-image"
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
        
group "Examples"
    include "Examples/"