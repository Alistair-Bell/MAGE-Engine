project "hypatia"
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

