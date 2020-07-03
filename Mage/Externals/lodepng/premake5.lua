project "lodepng"
    kind "StaticLib"
    language "C"
    cdialect "C89"

    files
    {
        "src/**.h",
        "src/**.c"
    }

    targetdir (BuildTargetPath)
    objdir (BuildObjectPath)

    filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "Speed"