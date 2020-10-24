import json
import os, sys, subprocess, zipfile, glob, platform


LogModes = {
    "Inform":           "\x1b[32m",
    "Warning":          "\x1b[33m",
    "Error":            "\x1b[34m",
    "Fatal Error":      "\x1b[36m",
    "Reset":            "\x1b[0m"
}

def GetPlatform():
    return sys.platform

def GetPythonVersionToString():
    return "%s.%s.%s" % (sys.version_info[0], sys.version_info[1], sys.version_info[2])

def GetSupportedBuildPlatforms():
    return [ "win32", "linux", "darwin" ]

def GetWorkingDirectory():
    return os.path.dirname(os.path.realpath(__file__))

# Allows interaction with the command line 
class Command:
    def __init__(self, windowsCommand, linuxCommand, macCommand):
       self.PlatformCommands = { 
           "win32":         windowsCommand,
           "linux":         linuxCommand,
           "macCommand":    macCommand
       }

    def UpdateCommand(self, newCommand, platform):
        self.PlatformCommands[platform] = newCommand

    @staticmethod
    def FormatCommand(command):
        tokens = [ x.strip() for x in command.strip('[]').split(' ') ]
        return tokens

    @staticmethod
    def DeleteFileCommand(paths):
        for x in paths:
            win32 = "del /f %s" % (sys.path)
            other = "rm %s" % (sys.path)
            c = Command(win32, other, other)
            c.CallCommand()

    def CallCommand(self, forcePlatform=GetPlatform()):
        command = self.PlatformCommands[forcePlatform]
        if command == None:
            return
        subprocess.run(args=command, shell = True, check=True )

def LogMessage(message, mode = LogModes["Inform"]):
    if GetPlatform() != "win32":
        print("%s%s" % (mode, message))
    else:
        print("%s" % (message))

def LogReset():
    if not GetPlatform() == "win32":
        print(LogModes["Reset"], end = '')

def HandleUserInput(message, choices):
    answer = ""
    while answer not in choices:
        LogMessage("%s" % (message))
        answer = str(input())
        if answer in choices:
            return answer
        LogMessage("Invalid choice %s, valid choices %s" % (answer, choices))

def CheckExistence(localPath):
    return os.path.exists(localPath)
        
def CreateDirectory(localPath):
    combined = "%s" % (localPath)
    if (CheckExistence(combined) == False):
        # all the platforms have the same command
        command = "mkdir %s" % (combined)
        LogMessage("Creating %s directory" % (localPath), LogModes["Inform"])
        makeDirectory = Command(command, command, command)
        makeDirectory.CallCommand()

def GetFilesInDirectory(localPath, extension = "*"):
    files = []
    if CheckExistence(localPath) == False:
        LogMessage("%s directory was not found!" % (localPath), LogModes["Error"])
        return files
    files = [f for f in glob.glob("%s/*%s" % (localPath, extension))]
    return files

def UnzipFile(localPath, output):
    if os.path.isfile(localPath) == True:
        LogMessage("Unziping %s" % (localPath), LogModes["Inform"])
        file = zipfile.ZipFile(localPath, "r")
        file.extractall(output) 
    else:
        LogMessage("%s zip file was not found, engine assets may be missing!" % (localPath), LogModes["Error"]) 

# terminology may be incorrect
def LoadArgumentsFromJSON(file, key, runtimeGenerated=None):
    arguments = {}
    with open (file, "r") as f:
        fileData = f.read()
        arguments = json.loads(fileData)
    keys = [*arguments]
    if key not in keys:
        LogMessage("Unable to find key %s in returned keys %s" % (key, keys), LogModes["Error"])
        return {}

    returning = arguments[key]
    if runtimeGenerated is not None:
        returning.update(dict(runtimeGenerated))
    return returning

def GenerateDefaultArguments(dictionary):
    returning = []
    for x, y in dictionary.items():
        returning.append(y[0])
    return returning

# The default uses the key: value[0]
def ParseCommandLineArgument(rawArguments, searchingDictionary, helpInfo=None):
    returnValues = []

    # No arguments specified
    if len(rawArguments) < 1:
        LogMessage("No arguments specified")
        if helpInfo != None:
            helpInfo()
        return []
    
    requiredKeys = list(searchingDictionary.keys())
    # removing the uneccesary command line arguments
    for raw in rawArguments:
        splitter = -1
        try:
            splitter = int(raw.index("="))
        except ValueError:
            LogMessage("%s does not follow the argument formatting!, %s" % (raw, [*searchingDictionary]), LogModes["Error"])
        else:
            passed = False
            for x, y in searchingDictionary.items():
                lowerBound = str(raw[0 : splitter + 1])
                upperBound = str(raw[splitter + 1: len(raw)])
            
                if not (lowerBound != x or upperBound not in y):
                    if lowerBound in requiredKeys:
                        requiredKeys.remove(lowerBound)
                        returnValues.append(upperBound)
                    else:
                        LogMessage("Repeat option found %s, using first instance" % (lowerBound), LogModes["Error"])
                        
                    passed = True
        
            # Not acceptable
            if not passed:
                currentKey = list(searchingDictionary.keys())[len(returnValues)]
                defaultValue = searchingDictionary.get(currentKey)[0]
                LogMessage("Invalid option %s, key did not match or value not present" % (raw), LogModes["Error"])
                returnValues.append(defaultValue)


    for x in requiredKeys:
        if len(list(searchingDictionary[x])) <= 0:
            LogMessage("No available values for %s" % (list(searchingDictionary[x])), LogModes["Fatal Error"])
            returnValues.append("")
        value = list(searchingDictionary[x])[0]
        LogMessage("Value %s not present, using default value %s" % (x, value), LogModes["Warning"])
        returnValues.append(value)

    # Removing any possible duplicates
    returnValues = list(dict.fromkeys(returnValues))
    # sorting by key

    sortedList = [None] * len(returnValues)
    
    for returnIndex in range(0, len(returnValues)):
        keys = [*searchingDictionary.keys()]
        for keyIndex in range(0, len(keys)):
            if returnValues[returnIndex] in searchingDictionary[keys[keyIndex]]:
                sortedList[keyIndex] = returnValues[returnIndex]
    return sortedList

def DisplayStartingInfo():
    
    # clear the screen
    clearScreen = Command("CLS", "clear", "clear")
    clearScreen.CallCommand()

    if GetPlatform() is not "win32":
        asciiText = " \
\x1b[91m    __  ___                    ______            _           \n \
\x1b[92m   /  |/  ____ _____ ____     / ________  ____ _(_____  ___  \n \
\x1b[94m  / /|_/ / __ `/ __ `/ _ \   / __/ / __ \/ __ `/ / __ \/ _ \ \n \
\x1b[91m / /  / / /_/ / /_/ /  __/  / /___/ / / / /_/ / / / / /  __/ \n \
\x1b[92m/_/  /_/\__,_/\__, /\___/  /_____/_/ /_/\__, /_/_/ /_/\___/  \n \
\x1b[94m             /____/                    /____/                "
        
    else:
        asciiText = " \
            __  ___                    ______            _           \n \
           /  |/  ____ _____ ____     / ________  ____ _(_____  ___  \n \
          / /|_/ / __ `/ __ `/ _ \   / __/ / __ \/ __ `/ / __ \/ _ \ \n \
         / /  / / /_/ / /_/ /  __/  / /___/ / / / /_/ / / / / /  __/ \n \
        /_/  /_/\__,_/\__, /\___/  /_____/_/ /_/\__, /_/_/ /_/\___/  \n \
                     /____/                    /____/                "

    LogMessage("%s\n" % asciiText)
    
    if GetPlatform() not in GetSupportedBuildPlatforms():
        LogMessage("Host platform %s not currently supported! see supported platforms %s" % (GetPlatform(), GetSupportedBuildPlatforms()), LogModes["Fatal Error"])
        return False

    if sys.version_info[0] < 3 or sys.version_info[1] < 6:
        LogMessage("Python version too old!, current %s < 3.6.x " % (GetPythonVersionToString()), LogModes["Fatal Error"])
        return False
    
    LogMessage("Welcome to Mage Engine python utility's system")
    LogMessage("Using python version %s\nHost platform recognised as %s\nLicense BSD 2-Clause License https://opensource.org/licenses/BSD-2-Clause\nThis is free software: you are free to change and redistribute it" % (GetPythonVersionToString(), GetPlatform()))
    LogMessage("%s\n" % ('-' * 73), LogModes["Reset"])
    return True

if __name__ == '__main__':
    DisplayStartingInfo()
    LogMessage("%s was called, this file provides utility for the build system and environemt setup, on it's on it has no functionality. Run Setup.py to use the engine!" % (__file__), LogModes["Warning"])
    LogReset()
