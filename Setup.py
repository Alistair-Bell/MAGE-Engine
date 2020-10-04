# Global libraries
import os, sys, subprocess, zipfile, json

# Local libraries
from Utility import *


"""
    Helper scripts that helps setup the environment for MAGE
    Platform wrapper for the commands
    

    Does:
        (0) -> Allows for the linux shell scrips
        (1) -> Builds the directories
        (2) -> Unzips assets
        (3) -> Stores vulkan sdk location and other information for the build system
"""

def GetBinaryLocation(question, mode=LogModes["Inform"]):
    userInput = ""
    while (1):
        LogMessage(question, mode)
        userInput = str(input())
        if userInput != "" or CheckExistence(userInput):
            break
    return userInput

def Main():
    
    # Checking supported platform
    if GetPlatform() not in GetSupportedBuildPlatforms():
        LogMessage("Unsupported platform %s, supported platforms are: %s" % (GetPlatform(), GetSupportedBuildPlatforms()), LogModes["Fatal Error"])
        LogReset()
        return

    # Allowing the use of the shell scripts for the engine 
    # if (GetPlatform() == "linux"):
    #     linuxScriptPath = "./Scripts/Linux"
    #     linuxScripts = GetFilesInDirectory(linuxScriptPath, ".sh")
    #     command = Command(windowsCommand = None, linuxCommand = None, macCommand = None)
    #     LogMessage("Allowing shell scripts to be run %s" % (linuxScripts), LogModes["Inform"])
    #     for x in linuxScripts:
    #         command.UpdateCommand("chmod +x %s" % (x), "linux")
    #         command.CallCommand()
    
    # Making logs directory
    directoriesToCreate = [ "Logs", "Config" ]
    for x in directoriesToCreate:
        if not CheckExistence(x):
            CreateDirectory(x)

    # Unziping assets 
    assetsToUnzip = {
        "Mage/Resources/Shaders.zip": "Mage/Resources/",
        "Mage/Resources/Textures.zip": "Mage/Resources/", 
        "Sandbox/Resources/Textures.zip": "Sandbox/Resources/",
         
    }
    for x, y in assetsToUnzip.items():
        UnzipFile(x, y)

    # Getting sdk related locations
    if CheckExistence("Config/Locations.json") is False:
        # Recieving input
        dump = {
            "GLSLValidator": str(GetBinaryLocation("Where is the glsl validator binary located?: ")),
            "GLSLCompiler":  str(GetBinaryLocation("Where is the glsl compiler binary located?: ")),
        }
        # Dumping to config file
        with open("Config/Locations.json", "w") as f:
            json.dump(dump, f, indent=6)
            f.close()

    

    LogMessage("Succesfully setup the MAGE-Engine environment!, if the locations specified were wrong or have changed edit Config/Locations.json", LogModes["Inform"])

if __name__ == '__main__':
    DisplayStartingInfo()
    Main()
    LogReset()
    