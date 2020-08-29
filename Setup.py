# Global libraries
import os, sys, subprocess, zipfile

# Local libraries
from Utility import *


"""
    Helper scripts that helps setup the environment for MAGE
    Platform wrapper for the commands
    

    Does:
        (0) -> Allows for the linux shell scrips
        (1) -> Builds the directories
        (2) -> Unzips assets

"""

def Main():
    
    # Checking supported platform
    if GetPlatform() not in GetSupportedBuildPlatforms():
        LogMessage("Unsupported platform %s, supported platforms are: %s" % (GetPlatform(), GetSupportedBuildPlatforms()), LogModes["Fatal Error"])
        LogReset()
        return

    # Allowing the use of the shell scripts for the engine 
    if (GetPlatform() == "linux"):
        linuxScriptPath = "./Scripts/Linux"
        linuxScripts = ["Clean.sh",  "CompileShaders.sh", "Makefile.sh", "ValidateShaders.sh"]
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
    assetsToUnzip = {
        "Mage/Resources/Shaders.zip": "Mage/Resources/",
        "Mage/Resources/Textures.zip": "Mage/Resources/", 
         
    }
    for x, y in assetsToUnzip.items():
        UnzipFile(x, y)


    LogMessage("Succesfully setup the MAGE-Engine environment!", LogModes["Inform"])
    LogReset()

if __name__ == '__main__':
    Main()
    