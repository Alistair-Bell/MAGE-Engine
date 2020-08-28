import os, sys, subprocess
"""
    Helper scripts that helps setup the environment for MAGE
    Platform wrapper for the commands
    

    Does:
        (0) -> Allows for the linux shell scrips
        (1) -> Builds the directories
        (2) -> Unzips assets

"""

FullPath                = os.path.dirname(os.path.realpath(__file__))
RuntimePlatform         = sys.platform
SupportedPlatforms      = [ "win32", "linux", "darwin" ]

LogModes = {
    "Inform":           "\x1b[32m",
    "Warning":          "\x1b[33m",
    "Error":            "\x1b[34m",
    "Fatal Error":      "\x1b[36m",
    "Reset":            "\x1b[0m"
}

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

# Nice bundle of the native commands to be run
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
        command = self.PlatformCommands[RuntimePlatform]
        if command == None:
            return
        formatted = Command.FormatCommand(command)
        subprocess.call(formatted)

def CheckDirectory(absolutePath):
    return os.path.exists(absolutePath)

def CreateDirectory(localPath):
    combined = "%s/%s" % (FullPath, localPath)
    if (CheckDirectory(combined) == False):
        # all the platforms have the same command
        command = "mkdir %s" % (combined)
        LogMessage("Creating %s directory" % (localPath), LogModes["Inform"])
        makeDirectory = Command(command, command, command)
        makeDirectory.CallCommand()

def UnzipFile(localPath):
    if os.path.isfile(localPath) == True:
        LogMessage("Unziping %s zip file", LogModes["Inform"])

    else:
        LogMessage("%s zip file was not found, engine assets may be missing!" % (localPath), LogModes["Error"]) 


def Main():
    
    # Checking supported platform
    if RuntimePlatform not in SupportedPlatforms:
        LogMessage("Unsupported platform %s, supported platforms are: %s" % (RuntimePlatform, SupportedPlatforms), LogModes["Fatal Error"])
        LogReset()
        return

    # Allowing the use of the shell scripts for the engine 
    if (RuntimePlatform == "linux"):
        linuxScriptPath = "./Scripts/Linux"
        linuxScripts = ["Clean.sh", "Commit.sh", "CompileShaders.sh", "Makefile.sh", "ValidateShaders.sh"]
        command = Command(windowsCommand = None, linuxCommand = None, macCommand = None)
        LogMessage("Allowing shell scripts to be run %s" % (linuxScripts), LogModes["Inform"])
        for x in linuxScripts:
            command.UpdateCommand("chmod +x %s/%s" % (linuxScriptPath, x), "linux")
            command.CallCommand()
    
    # Making logs directory
    directoriesToCreate = [ "Logs", "Examples/Logs" ]
    for x in directoriesToCreate:
        CreateDirectory(x)

    # Unziping assets 
    assetsToUnzip = [ "Mage/Resources/Resources.zip" ]
    for x in assetsToUnzip:
        UnzipFile(x)

    LogReset()

if __name__ == '__main__':
    Main()
    