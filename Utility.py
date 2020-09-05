import os, sys, subprocess, zipfile, glob, pathlib


LogModes = {
    "Inform":           "\x1b[32m",
    "Warning":          "\x1b[33m",
    "Error":            "\x1b[34m",
    "Fatal Error":      "\x1b[36m",
    "Reset":            "\x1b[0m"
}


def GetPlatform():
    return sys.platform

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

    def CallCommand(self):
        command = self.PlatformCommands[GetPlatform()]
        if command == None:
            return
        subprocess.run(args=command, shell = True, check=True )

def LogMessage(message, mode = LogModes["Inform"]):
    print("%s%s" % (mode, message))

def LogReset():
    print(LogModes["Reset"], end = '')

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

def GenerateDefaultArguments(dictionary):
    returning = []
    for x, y in dictionary.items():
        returning.append(y[0])
    return returning

# The default uses the key: value[0]
def ParseCommandLineArgument(rawArguments, searchingDictionary, helpInfo=None):
    returnValues = []
    actualArguments = rawArguments[1 : len(rawArguments)]

    # No arguments specified
    if len(rawArguments) < 1:
        LogMessage("No arguments specified")
        if helpInfo != None:
            helpInfo()
            return GenerateDefaultArguments(searchingDictionary)
    
    requiredKeys = list(searchingDictionary.keys())

    # removing the uneccesary command line arguments
    for raw in actualArguments:
        splitter = -1
        try:
            splitter = int(raw.index("="))
        except ValueError:
            LogMessage("%s does not follow the argument formatting!, %s" % (raw, searchingDictionary.keys()), LogModes["Error"])
        else:
            passed = False
            for x, y in searchingDictionary.items():
                lowerBound = str(raw[0 : splitter + 1])
                upperBound = str(raw[splitter + 1: len(raw)]) 
                if not (lowerBound != x or upperBound not in y):
                    requiredKeys.remove(lowerBound)
                    returnValues.append(upperBound)
                    passed = True
        
            # Not acceptable
            if not passed:
                currentKey = list(searchingDictionary.keys())[len(returnValues)]
                defaultValue = searchingDictionary.get(currentKey)[0]
                LogMessage("Invalid option %s, key did not match or value not present" % (raw), LogModes["Error"])
                returnValues.append(defaultValue)


    for x in requiredKeys:
        value = list(searchingDictionary[x])[0]
        LogMessage("Value %s not present, using default value %s" % (x, value))
        returnValues.append(value)

    # Removing any possible duplicates
    returnValues = list(dict.fromkeys(returnValues))
    return returnValues


if __name__ == '__main__':
    LogMessage("%s was called, this file provides utility for the build system and environemt setup, on it's on it has no functionality. Run Setup.py to use the engine!" % (__file__), LogModes["Warning"])
    LogReset()