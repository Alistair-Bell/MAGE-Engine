import os, sys, json

from Utility import *

ShaderExtensions = {

    "Fragment": [ "frag" ],
    "Vertex": [ "vert" ],
    "Tessellation-Control": [ "tesc" ],
    "Tessellation-Evaluation": [ "tese" ],
    "Geometry": [ "geom" ],
    "Fragment": [ "frag" ],
    "Compute": [ "comp" ],
}

"""
    Script that compiles the shaders specified on the command line arguments
    Arguments:
        (x) -> Python string array of the directories of the shaders
        (x - 1) -> Optimisation flags:
            (0) -> None
            (1) -> Speed
            (2) -> File size

    Does:
        (0) -> Find all pr-existing SPIRV binaries in the specified folders and removes them
        (1) -> Get all the shaders and store them
        (2) -> Validate the file extensions and sort them into shader types
        (3) -> Runs the GLSL validator and validates all the shaders
        (4) -> Compiles the shaders outputing using NameOfShader.sprv

"""

def GetShaders(path):
    files = []
    for key, value in ShaderExtensions.items():
        for extensions in value:
            addition = GetFilesInDirectory(path, extensions)
            if not addition == []:
                files += addition
    return files

def GenerateSpirvFileName(before):
    extension = ".sprv"
    index = before.index(".")
    if index == -1:
        return before + extension
    else:
        new = before[0 : index]
        return new + extension

def ValidateShader(shader, validator):
    LogMessage("Validating %s, testing for errors!" % (shader))
    finalCommand = "%s %s" % (validator, shader)
    command = Command(finalCommand, finalCommand, finalCommand)
    command.CallCommand()
    LogMessage("Succesfully validated %s" % (shader))

def CompileShader(shader, compiler):
    output = GenerateSpirvFileName(shader)
    finalCommand = "%s -c %s -o %s" % (compiler, shader, output)
    LogMessage("Compiling %s outputing spirv binary as %s" % (shader, output))
    command = Command(finalCommand, finalCommand, finalCommand)
    command.CallCommand()
    LogMessage("Succesfully compiled %s" % (shader))

def HandleShader(shaders, validator, compiler):
    for shader in shaders:
        ValidateShader(shader, validator)
        CompileShader(shader, compiler)
    LogMessage("Validated and compiled all shaders!")




def Main():
    arguments = sys.argv
    
    # Validating input 
    if not CheckExistence("Config/Locations.json"):
        LogMessage("Config file was not found! make sure you run Setup.py and set the values in the Config/Setting.json", LogModes["Error"])
    LogMessage("Config file found! Reading data!", LogModes["Inform"])
    
    if len(arguments) < 3:
        LogMessage("Invalid argument count!, 2 are required!", LogModes["Error"])
        return

    glslValidator = ""
    glslCompiler  = ""

    # Getting data from config file
    with open("Config/Locations.json", "r") as file:
        contents = json.load(file)
        if len(contents) < 2:
            LogMessage("Config/Locations.json lacks required data, length too small", LogModes["Error"])
            return
        glslValidator = str(list(contents.values())[0])
        glslCompiler  = str(list(contents.values())[1])
        file.close()
    
    # Validation file exists
    if glslValidator == "" or CheckExistence(glslValidator):
        LogMessage("GLSL validator was not found, check the Config/Locations.json", LogModes["Error"])
        return
    
    if glslCompiler == "" or CheckExistence(glslCompiler):
        LogMessage("GLSL compiler was not found, check the Config/Locations.json", LogModes["Error"])
        return

    print(glslCompiler)

    paths       = []
    shaderFiles = []
    optimisation = str(arguments[-1]).lower()

    # Getting the directories
    for x in range(1, len(sys.argv) - 1):
        paths.append(str(arguments[x]))    

    # Getting files
    for path in paths:
        files = GetShaders(path)
        if files is not []:
            shaderFiles += files

    HandleShader(shaderFiles, glslValidator, glslCompiler)


if __name__ == '__main__':
    Main()
    LogReset()
    