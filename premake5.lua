
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
        "LUA_C89_NUMBERS",
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
        --"LinkTimeOptimization",
    }
    links
    {
        "Sandbox",
        "hypatia",
        "GLFW",
        "vulkan",
        "lua"
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
    
    flags
    {
        "FatalWarnings",
        "FatalCompileWarnings",
    }

filter "configurations:Release"
    defines "MAGE_RELEASE"
    runtime "Release"
    optimize "Speed"
    symbols "On"

project "Sandbox"
    location "%{prj.name}"
    kind "StaticLib"
    language "c"
    cdialect "C89"
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
    defines
    {
        "LUA_USE_C89",
        
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
    include "Mage/Externals/lua-5.4.0/"
