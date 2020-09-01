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
        formatted = Command.FormatCommand(command)
        subprocess.call(formatted)

def LogMessage(message, mode):
    
    switcher = {
        "\x1b[32m": "Inform     :",
        "\x1b[33m": "Warning    :",
        "\x1b[34m": "Error      :",
        "\x1b[36m": "FatalError :",
        "\x1b[0m":  "Reset      :",
    }   
    
    print("%sLog %s %s" % (mode, switcher[mode], message))

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

if __name__ == '__main__':
    LogMessage("%s was called, this file provides utility for the build system and environemt setup, on it's on it has no functionality. Run Setup.py to use the engine!" % (__file__), LogModes["Warning"])
    LogReset()