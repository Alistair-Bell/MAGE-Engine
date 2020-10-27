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

    # Making logs directory
    directoriesToCreate = [ "Logs", "Config" ]
    for x in directoriesToCreate:
        if not CheckExistence(x):
            CreateDirectory(x)

    assetsToUnzip = {}
    # Unziping assets 
    with open("Helpers/AssetPaths.json", "r") as f:
        fileData = json.loads(f.read())
        for x, y in fileData.items():
            assetsToUnzip.update(y)

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

    

    LogMessage("Succesfully setup the Mage Engine environment!, if the locations specified were wrong or have changed edit Config/Locations.json", LogModes["Inform"])

if __name__ == '__main__':
    if DisplayStartingInfo():
        Main()
    LogReset()
    