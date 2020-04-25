# MAGE (multi api game engine) Version 0.1
## Native c89 linux game engine

MAGE is in currently unusable as an engine and will be for a good while so come back again and check later down the line.

## Getting Started
## You Will Need

This engine is being developed currently on linux (fedora 5.5.17-200) so **MACOS** and **Windows** support is currently untested for now.

Using and IDE is not required but can be useful when looking at runtime memory and performance.

- `c89 compiler (clang / gcc / MVSC) | To compile the code`
- `Make or mingw32-make | Build system`
- `Vulkan | Graphics rendering`
- `GLSL version: 4.50 | Shaders`
- `Vulkan SLD2 or GLFW added to path and libraries installed | Allowing our program to interact with libraries`
  
Firstly you will need to obtain the source code for mage. To do this clone the github page.

 >`git clone https://github.com/MTECGamesStudio/MAGE-Engine`

Then run the makefile doing.

> `make` or `make install`

The engine builds into an executable so edit Source/mageMain.c

## The End Goal
The goal for MAGE is for a high performance 2D renderer that can be built for 

- **Windows**
- **MACOS**
- **Linux** 
- **Switch**

making it simple to get stuck in and create games using native c89 to mess around in.

## Current Features

- `Basic SDL2 and GLFW3 window support`
- `Basic Maths`
- `Logging System`
- `Doxygen Support`

## Planned Features

- `DirectX Rendering`
- `Rendering Pipeline`
- `Advanced Input (Joystick)`
- `Sound And Audio`
- `Physics Engine`
- `Editor`





