
BuildTargetPath = "Build/Binaries/%{prj.name}"
BuildObjectPath = "Build/Objects/%{prj.name}"
    

workspace "MAGE"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Distribution"
    } 
-- Engine Project

group "Externals"
    include "Mage/Externals/hypatia"
    include "Mage/Externals/glfw3"

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
        "Mage/Platform/Mono/**.c"
    }
    defines
    {
        "MAGE_CORE",
        "MAGE_MONO_EXTERNALS"
    }
    linkoptions 
    { 
        "`wx-config --libs`"
    }

    includedirs
    {
        "Mage",
        "/usr/local/include/mono-2.0"
            
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
        "X11",
        "dl",
        "pthread",
        "m",
        "mono-2.0",
    }


    filter "system:windows"
        systemversion "latest"
    
    
    filter "configurations:Debug"
        defines "MAGE_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "MAGE_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Distribution"
        defines "MAGE_DISTRIBUTION"
        runtime "Release"
        optimize "On"


project "Sandbox"
    location "%{prj.name}"
    kind "SharedLib"
    language "C#"
    targetdir (BuildTargetPath)
    objdir (BuildObjectPath)
    
    files
    {
        "Mage/mageAPI.cs",
        "%{prj.name}/**.cs",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"
        
    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        
    filter "configurations:Distribution"
        runtime "Release"
        optimize "On"

        
