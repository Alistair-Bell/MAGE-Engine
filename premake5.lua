
BuildTargetPath = "Build/Binaries/%{prj.name}"
BuildObjectPath = "Build/Objects/%{prj.name}"
    

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

    pchheader "Mage/mageAPI.h"
    pchsource "Mage/mageAPI.c"

    files
    {
        "Mage/*.h",
        "Mage/*.c",
        "Mage/Platform/Vulkan/**.c",
    }
    defines
    {
        "MAGE_CORE",
    }
    linkoptions 
    { 
        "`wx-config --libs`"
    }

    includedirs
    {
        "Mage",
    }
    flags
    {
        "FatalWarnings",
        "FatalCompileWarnings",
        "LinkTimeOptimization",
    }
    links
    {
        "Sandbox",
        "hypatia",
        "GLFW",
        "vulkan",
    }


    filter "system:windows"
    systemversion "latest"

    filter "system:linux"

    links 
    { 
        "dl",
        "m",
    }

filter "configurations:Debug"
    defines "MAGE_DEBUG"
    runtime "Debug"
    symbols "On"

filter "configurations:Release"
    defines "MAGE_RELEASE"
    runtime "Release"
    optimize "On"



project "Sandbox"
    location "%{prj.name}"
    kind "StaticLib"
    language "c"
    cdialect "c89"
    targetdir (BuildTargetPath)
    objdir (BuildObjectPath)

    includedirs
    {
        "Mage",
    }
    
    files
    {
        "%{prj.name}/**.h",   
        "%{prj.name}/**.c",
    }

    filter "configurations:Debug"
        defines "SANDBOX_DEBUG"    
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "SANDBOX_RELEASE"
        runtime "Release"
        optimize "On"


group "Externals"
    include "Mage/Externals/hypatia"
    include "Mage/Externals/glfw3"
