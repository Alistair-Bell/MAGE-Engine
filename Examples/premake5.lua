project "Hello-World"
    location "%{prj.name}"
    kind "ConsoleApp"
    language "C"
    cdialect "C89"
    targetdir (BuildTargetPath)
    objdir (BuildObjectPath)

    files
    {
        "%{prj.name}/Source/**.c"
    }
    links
    {
        "MageEngine",
        "stb-image",
        "GLFW",
        "vulkan",
        "yaml",
        "libogg"
    }
    includedirs
    {
        "../Mage/Source",
    }

    filter "system:linux"
    
        links 
        { 
            "dl",
            "pthread",
            "m",
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "Speed"

