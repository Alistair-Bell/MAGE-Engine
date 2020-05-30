workspace "MAGE"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Distribution"
    }

-- Engine Project 


BuildTargetPath = "Build/Binaries/%{prj.name}-%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
BuildObjectPath = "Build/Objects/%{prj.name}-%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


project "MageEngine"
    location "Mage"
    kind "SharedLib"
    language "C"
    cdialect "C89"

    targetdir (BuildTargetPath)
    objdir (BuildObjectPath)
    
    pchheader "Mage/mageAPI.h"
    pchsource "Mage/mageAPI.c"


    files
    {
        "Mage/**.h",
        "Mage/**.c"
    }
    defines
    {
        "MAGE_CORE"
    }

    filter "configurations:Debug"
        defines "MAGE_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "MAGE_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "MAGE_DISTRIBUTION"
        runtime "Release"
        optimize "On"


-- Client Project

project "Sandbox"
    location "%{prj.name}"
    kind "ConsoleApp"
    language "C"
    cdialect "C89"

    targetdir (BuildTargetPath)
    objdir (BuildObjectPath)

    files
    {
        "%{prj.name}/**.h",    
        "%{prj.name}/**.c",
    }

    links
    {
        "MageEngine",
        "glfw3",
        "vulkan",
        "X11",
        "dl",
        "pthread",
        "m"
    }
    
    filter "configurations:Debug"
        defines "CLIENT_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "CLIENT_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "CLIENT_DISTRIBUTION"
        runtime "Release"
        optimize "On"