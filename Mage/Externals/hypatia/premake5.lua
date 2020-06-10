project "hypatia"
    kind "SharedLib"
    language "C"
    cdialect "C89"

    files
    {
        "src/**.h",
        "src/**.c"
    }

    targetdir (BuildTargetPath)
    objdir (BuildObjectPath)

