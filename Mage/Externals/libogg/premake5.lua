project "libogg"
    kind "StaticLib"
    language "C"
    cdialect "C89"

    files
    {
        "include/ogg/**.h",
        "include/ogg/**.c",
        "src/**.h",
        "src/**.c"
    }
    includedirs
    {
        "include/ogg"
    }

    targetdir (BuildTargetPath)
    objdir (BuildObjectPath)

    filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "Speed"