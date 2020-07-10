
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
    
    
-- Engine Project
project "MageEngine"
    location "Mage"
    kind "ConsoleApp"
    language "C"
    cdialect "C89"

    targetdir (BuildTargetPath)
    objdir (BuildObjectPath)

    pchheader "Mage/Source/mageAPI.h"
    pchsource "Mage/Source/mageAPI.c"

    files
    {
        "Mage/Source/*.h",
        "Mage/Source/*.c",
        "Mage/Source/Platform/Vulkan/**.c",
    }
    defines
    {
        "MAGE_CORE",
    }
    includedirs
    {
        "Mage/Source/",
    }
    flags
    {
        "LinkTimeOptimization",
    }
    links
    {
        "Sandbox",
        "hypatia",
        "GLFW",
        "vulkan",
        "lodepng",
        "yaml"    
    }


    filter "system:windows"
    systemversion "latest"

    filter "system:linux"

        links 
        { 
            "dl",
            "pthread",
            "m",
        }

filter "configurations:Debug"
    defines "MAGE_DEBUG"
    runtime "Debug"
    symbols "On"
    
    flags
    {
        "FatalWarnings",
        "FatalCompileWarnings",
    }

filter "configurations:Release"
    defines "MAGE_RELEASE"
    runtime "Release"
    optimize "Speed"


project "Sandbox"
    location "%{prj.name}"
    kind "StaticLib"
    language "c"
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
    defines
    {
        
    }

    filter "configurations:Debug"
        defines "SANDBOX_DEBUG"    
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "SANDBOX_RELEASE"
        runtime "Release"
        optimize "Speed"
        

group "Externals"
    include "Mage/Externals/hypatia"
    include "Mage/Externals/glfw3"
    include "Mage/Externals/lodepng"
    include "Mage/Externals/libyaml"
