from json import load
import os, sys, json, time, platform

from Utility import *



"""
    Script that wraps up the premake build system allowing for loading build modes and automatically executing them    
"""

BuildScript = "Config/BuildScript.json"

RuntimeGeneratedCommandLineOptions = {
    "--platform=": list(GetSupportedBuildPlatforms()),
}

CommandLineOptions = LoadArgumentsFromJSON("Helpers/CommandLineArguments.json", __file__, RuntimeGeneratedCommandLineOptions)

def ScriptHelp():
    LogMessage("Usage: %s:" % (__file__))
    LogMessage("Arguments presented may be in any order")
    LogMessage("Some IDE environment may be incompatable with some specified tools!", LogModes["Warning"])
    i = 1
    for x, y in CommandLineOptions.items():
        LogMessage("\tArgument %s: prefix = %s, acceptable = %s" % (i, x, y))
        i += 1

def LoadConfigFile():
    data    = [None] * 2
    data[1] = []
    with open(BuildScript, "r") as f:
        fileData = f.read()
        if len(fileData) <= 5:
            LogMessage("%s file has lack of data!" % (BuildScript), LogModes["Error"])
            data[0] = 0
            data[1] = ""
            return data
        dictionary = json.loads(fileData)
        for x, y in dictionary.items():
            data[1].append("%s%s" % (x, y))

    data[1] = ParseCommandLineArgument(data[1], CommandLineOptions, None)
    if data[1] == []:
        data[0] = 0
        return
    data[0] = 1
    return data

def UseCommandLineArguments():
    # By default the os will dump the file name of the program into args[0], stripping it from the parser
    parsing = sys.argv[1 : len(sys.argv)]
    arguments = ParseCommandLineArgument(parsing, CommandLineOptions, ScriptHelp)
    return arguments

def CallSingleCommand(win32 = "", linux = "", darwin = ""):
    foo = Command(win32, linux, darwin)
    foo.CallCommand()

def CallMake(config):
    command = "make all config=%s" % (config)
    make = Command(command, command, command)
    make.CallCommand()

def Main():

    arguments = []

    if not CheckExistence(BuildScript):
        LogMessage("No build script present, only using command line arguments", LogModes["Inform"])
        arguments = UseCommandLineArguments()
    else:
        LogMessage("Build script present (%s), override command line arguments" % (BuildScript))
        choices = [ "y", "n" ]
        choice = HandleUserInput("Override command line arguments? %s" % (choices), choices)

        if choice == "y":
            result = LoadConfigFile()
            if not result[0] or result[1] == []:
                return
            arguments = result[1]
        else:
            arguments = UseCommandLineArguments()

    # checking if help was called
    if arguments == []:
        return
    
    # arguments valid 

    locations = {
        "linux": "./Externals/Linux/premake5",
        "win32": "CALL Externals\\Windows\\premake5.exe",
        # Currently the macos binary is not being distributed by us, todo by version <= 1.0.0
        "darwin": None,
    } 
    if locations[GetPlatform()] == None:
        LogMessage("%s platform has no distributed premake binaries, support currently incomplete!" % (GetPlatform()), LogModes["Fatal Error"])
        return

    LogMessage("Premake binary found, generating build commands!")


    config              = arguments[0]
    generator           = arguments[1]
    compiler            = arguments[2]
    renderer            = arguments[3]
    audioBackend        = arguments[4]
    buildTests          = arguments[5]
    platform            = arguments[6]


    generatorSwitcher = {
        "vsproject":        "vs2019",
        "xcode":            "xcode",
        "makefile":         "gmake2",
        "codelite":         "codelite",
    }
    platformSwitcher = {
        "win32":            "windows",
        "linux":            "linux",
        "darwin":           "macosx",
    }

    LogMessage("Build options: \
                \n\tConfig -> %s \
                \n\tPlatform -> %s \
                \n\tGenerator -> %s" % (config, platform, generator))

    LogMessage("Calling premake")
    premakeString = "%s --file=Core.lua --os=%s --renderer=%s --audio-backend=%s --tests=%s --cc=%s %s" % (locations[GetPlatform()], platformSwitcher[platform], renderer, audioBackend, buildTests, compiler, generatorSwitcher[generator])
    foo = Command(premakeString, premakeString, premakeString)
    foo.CallCommand() 
    
    if generator == "makefile":
        CallMake(config)

if __name__ == '__main__':
    if DisplayStartingInfo():
        Main()  
    LogReset()