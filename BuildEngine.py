import os, sys, json

from Utility import *



"""
    Script that wraps up the premake build system allowing for loading build modes and automatically executing them
    Arguments:
        () -> 
        
    Does:
        () -> Parses arguments

        
"""

CommandLineOptions = {
    "--config=": [ "debug", "release" ],
    "--platform=": list(GetSupportedBuildPlatforms()),
    "--targets=": [ "all", "sandbox", "engine", "externals" ],
    "--generator=": [ "vs2019", "vs2017", "xcode", "makefile", "codelite" ],
    "--renderer=": [ "vulkan", "gles" ],
}

def FindArgumentByKey(searchingKey, values, dictionary):
    keyValues = dictionary.get(searchingKey)
    if keyValues is None:
        LogMessage("Invalid dictionary key %s!" % (searchingKey), LogModes["Error"])
        return None
    for x in values:
        if x in keyValues:
            return x
    return None

def ScriptHelp():
    LogMessage("Usage: %s:" % (__file__))
    i = 1
    for x, y in CommandLineOptions.items():
        print("\tArgument %s: prefix = %s, acceptable = %s" % (i, x, y))
        i += 1

def Main():
    
    # By default the os will dump the file name of the program into args[0], stripping it from the parser
    parsing = sys.argv[1 : len(sys.argv)]
    arguments = ParseCommandLineArgument(parsing, CommandLineOptions, ScriptHelp)
    
    # checking if help was called
    if arguments == []:
        return
    
    # arguments valid 

    locations = {
        "linux": "./Externals/Linux/premake5",
        "win32": "Externals/Windows/premake5.exe",
        # Currently the macos binary is not being distributed by us, todo by version <= 1.0.0
        "darwin": "",
    } 
    if locations[GetPlatform()] == "":
        LogMessage("%s platform has no distributed premake binaries, support currently incomplete!" % (GetPlatform()), LogModes["Fatal Error"])
        return

    LogMessage("Premake binary found, generating build commands!")

    if not CheckExistence("Config/BuildScript.json"):
        LogMessage("No build script present, only using command line arguments", LogModes["Warning"])
    else:
        # todo script loading
        pass

    config          = FindArgumentByKey("--config=", arguments, CommandLineOptions)
    platform        = FindArgumentByKey("--platform=", arguments, CommandLineOptions)
    targets         = FindArgumentByKey("--targets=", arguments, CommandLineOptions)
    generator       = FindArgumentByKey("--generator=", arguments, CommandLineOptions)
    renderer        = FindArgumentByKey("--renderer=", arguments, CommandLineOptions)
    
    if generator == "makefile":
        generator = "gmake2"
    
    targetSwitcher = {
        "all":          "all",
        "sandbox":      "Sandbox",
        "engine":       "MageEngine",
        "externals":    "GLFW stb-image"
    }
    

    LogMessage("Build options \
                \n\tConfig -> %s \
                \n\tPlatform -> %s \
                \n\tTargets -> %s \
                \n\tGenerator -> %s \
                \n\tHardware renderer -> %s" % (config, platform, targets, generator, renderer))

    LogMessage("Calling premake")
    premakeString = "%s --fatal --verbose --file=premake5.lua --renderer=%s %s" % (locations[GetPlatform()], renderer, generator)
    foo = Command(premakeString, premakeString, premakeString)
    foo.CallCommand() 
    

    if generator == "gmake2":
        LogMessage("Using makefile as build system, calling make")
        makeString = "make config=%s %s" % (config, targetSwitcher[targets])
        bar = Command(makeString, makeString, makeString)
        bar.CallCommand()


if __name__ == '__main__':
    if GetPlatform() not in GetSupportedBuildPlatforms():
        LogMessage("%s platform is not supported by MAGE!, supported platforms %s" % (GetPlatform(), GetSupportedBuildPlatforms()), LogModes["Fatal Error"])
    else:
        Main()
    LogReset()
    