import os, sys, json
from pathlib import Path

from Utility import *



"""
    Script that compiles the shaders specified on the command line arguments

    Does:
        (0) -> Find all pr-existing SPIRV binaries in the specified folders and removes them
        (1) -> Get all the shaders and store them
        (2) -> Validate the file extensions and sort them into shader types
        (3) -> Runs the GLSL validator and validates all the shaders
        (4) -> Compiles the shaders outputing using NameOfShader.sprv

"""

ShaderExtensions = {

    "Fragment": [ "frag" ],
    "Vertex": [ "vert" ],
    "Tessellation-Control": [ "tesc" ],
    "Tessellation-Evaluation": [ "tese" ],
    "Geometry": [ "geom" ],
    "Fragment": [ "frag" ],
    "Compute": [ "comp" ],
}
CommandLineOptions = {
    # None does not care what 
    "--shaders=": [],
    "--optimisation=": [ "speed", "size", "none" ]
}

CompilerOptmisations = [ "none", "speed", "size" ]

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

def CompileShader(shader, optimisation, compiler):
    output = GenerateSpirvFileName(shader)
    switcher = {
        "none": "-O0",
        "speed": "-O",
        "size": "-Os",
    }

    finalCommand = "%s -c %s %s -o %s" % (compiler, shader, switcher[optimisation], output)
    LogMessage("Compiling %s outputing spirv binary as %s" % (shader, output))
    command = Command(finalCommand, finalCommand, finalCommand)
    command.CallCommand()
    LogMessage("Succesfully compiled %s" % (shader))

def HandleShader(shaders, optimisation, validator, compiler):
    for shader in shaders:
        ValidateShader(shader, validator)
        CompileShader(shader, optimisation, compiler)
    LogMessage("Validated and compiled all shaders!")

def GenerateShaderArguments():
    # This code is not too pretty but it will do for now
    returnValues = []
    for root, dirs, files in os.walk("."):
        for file in files:
            for x, y in ShaderExtensions.items():
                for v in y:
                    if file.endswith(v):
                        returnValues.append(str(root[2: len(root)]))
    return list(dict.fromkeys(returnValues))

def ScriptHelp():
    LogMessage("Script that validates, builds shaders into sprv assembly, renderers backends like vulkan require this binary format and opengl can use this")
    LogMessage("Usage: %s:" % (__file__))
    i = 1
    for x, y in CommandLineOptions.items():
        print("\tArgument %s: prefix = %s, acceptable = %s" % (i, x, y))
        i += 1

def Main():
    
    CommandLineOptions["--shaders="] = GenerateShaderArguments()
    
    arguments = ParseCommandLineArgument(sys.argv[1 : len(sys.argv)], CommandLineOptions, ScriptHelp)
    # checking if help was called
    if arguments == []:
        return
    
    optimisation = arguments[1]


    glslValidator = ""
    glslCompiler  = ""

    if CheckExistence("Config/Locations.json") == False:
        LogMessage("Unable to find Config/Locations.json, required for program to run", LogModes["Error"])
        return

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

    paths       = []
    shaderFiles = []

    # Getting the directories

    # Getting files
    
    files = GetShaders(arguments[0])
    if files is not []:
        shaderFiles += files

    LogMessage("Validating and building %s shaders with %s optmisation set" % (len(shaderFiles), optimisation))

    HandleShader(shaderFiles, optimisation, glslValidator, glslCompiler)

    
if __name__ == '__main__':
    if DisplayStartingInfo():
        Main()
    LogReset()
    
