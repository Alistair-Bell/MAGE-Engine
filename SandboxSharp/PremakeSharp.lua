BuildTargetPath = "Build/Binaries/%{prj.name}-%{cfg.buildcfg}"
BuildObjectPath = "Build/Objects/%{prj.name}-%{cfg.buildcfg}"


workspace "Mage"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
    }
    startproject "Sandbox"

project "SandboxSharp"
    location "%{prj.name}"
    kind "ConsoleApp"
    language "c#"
    targetdir (BuildTargetPath)
    objdir (BuildObjectPath)

    files
    {
        "../Mage/Source/Mono/**.cs",
        "Source/**.cs",   
        "Source/**.cs",
    }