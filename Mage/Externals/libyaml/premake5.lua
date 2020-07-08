project "yaml"
    kind "StaticLib"
    language "C"
    cdialect "C89"

    files
    {
        "include/**.h",
        "src/**.c"
    }
    includedirs
    {
        "include"
    }
    defines
    {
        "YAML_VERSION_MAJOR=0",
        "YAML_VERSION_MINOR=2",
        "YAML_VERSION_PATCH=5",
        "YAML_VERSION_STRING=\"0.25\""
    }

    targetdir (BuildTargetPath)
    objdir (BuildObjectPath)

    filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "Speed"