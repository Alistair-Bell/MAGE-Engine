import os, pathlib

from Utility import *

def ZipRecursive(zipHandle, folder):
    for root, dirs, files in os.walk(folder):
        for file in files:
            # Ignore sprv binary
            if not file.endswith(".sprv"):
                zipHandle.write(os.path.join(root, file))

def ZipAssets(folder):
    zipping = next(os.walk(folder))[1]
    LogMessage("Zipping assets folder(s) %s in %s, generating zip %s archive(s)" % (zipping, folder, len(zipping)))
    
    os.chdir(folder)
    
    for x in zipping:
        output = "%s.zip" % (x)
        with zipfile.ZipFile(output, "w", compression=zipfile.ZIP_DEFLATED) as z:
            ZipRecursive(z, x)
        LogMessage("Created archive %s/%s" % (folder, output))
    os.chdir(GetWorkingDirectory())


def Main():
    
    folders = []
    rootDirectory = next(os.walk(GetWorkingDirectory()))[1]
    for x in rootDirectory:
        assetPath = "%s/Resources" % (x)
        if CheckExistence(assetPath):
            folders.append(assetPath)
    LogMessage("Found %s folders with Resources folders %s" % (len(folders), folders))
    
    for x in folders:
        if CheckExistence(x):
            ZipAssets(x)

if __name__ == '__main__':
    DisplayStartingInfo()
    if GetPlatform() not in GetSupportedBuildPlatforms():
        LogMessage("%s platform is not supported by MAGE!, supported platforms %s" % (GetPlatform(), GetSupportedBuildPlatforms()), LogModes["Fatal Error"])
    else:
        Main()  
    LogReset()