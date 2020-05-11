import random
import os
import sys

# Used to help make commands


EngineMakefile = "./Scripts/engine.makefile"
EngineBuildEcho = "echo \x1b[35mBuilding Engine Library!\x1b[0m\n"
ClientMakefile = "./Scripts/client.makefile"
ClientBuildEcho = "echo \x1b[35mBuilding Client Executable!\x1b[0m\n"

Arguments = [
    
    ["-bd", "mkdir build && mkdir build/bin && mkdir build/lib && mkdir build/obj && mkdir Documentation && mkdir Logs", "creates folders for the project"],
    ["-ca", "%s make -f %s &&%s make -f %s" % (EngineBuildEcho, EngineMakefile, ClientBuildEcho, ClientMakefile), "compiles the engine then the client"],
    ["-ce", "%s make -f %s" % (EngineBuildEcho, EngineMakefile), "compiles the engine"],
    ["-cc", "%s make -f %s" % (ClientBuildEcho, ClientMakefile), "compiles the client"],
    ["-cla", "make -f %s && clean make -f %s" % (EngineMakefile, ClientMakefile), "cleans the clients and engines build and output files"],
    ["-cle", "make -f %s clean" % (EngineMakefile), "cleans the engines build and output files"],
    ["-clc", "make -f %s clean" % (ClientMakefile), "cleans the clients build and output files"],

]

def DisplayArguments():
    print("Arguments:")
    for x in range(0, len(Arguments)):
        print("%s) %s | %s" % (x + 1, Arguments[x][0], Arguments[x][2])) 

def Main():

    if (len(sys.argv) <= 1 or sys.argv[1] == "-h"):
        DisplayArguments()
    
    else:
        found = 0

        for x in range(0, len(Arguments)):
            for y in range(0, len(sys.argv)):        
                if sys.argv[y] == Arguments[x][0]:
                    found += 1   
                    os.system(Arguments[x][1])                
        
        if found <= 0:
            print("Argument(s) not recognised %s!" % (sys.argv[1 : len(sys.argv)]))
            DisplayArguments()


if __name__ == '__main__':
    Main()
    

