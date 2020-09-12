project "glad"
    kind "StaticLib"
    language "C"

    files
    {
        "**.h",
        "**.c"
    }

    targetdir (BuildTargetPath)
    objdir (BuildObjectPath)

    filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "Speed"