# Externals

These libraries help power the MAGE engine and I am thankful for the authors of these external libraries for making them free and open source for the public

## glfw3
- Maintained under `zlib` license
- Changes 
    - Removed `CMAKE`, `deps`, `docs`, `examples` and `tests` directories
    - Removed `.appveyor.yml`,  `.gitattributes`, `.gitignore`, `.mailmap`, .`travis.yml` and `CMakeLists.txt` files
    - Added `premake5` build system
- Version `3.4.0`
- Sourced from `https://github.com/glfw/glfw`

## Vulkan-Headers
- Maintained under the `Apache License 2.0`
  - Changes
    - Removed `cmake` and `registery` directories
    - Removed `.cmake-format.py`, `.gitattributes`, `.gitignore`, `BUILD.gn`, `BUILD.md`, `CMakeLists.txt` and `vulkan.hpp`
- Version `1.2`
- Sourced from `https://github.com/KhronosGroup/Vulkan-Headers`

## stb-image
- Maintained under the `MIT-License`
- Changes
  - Moved `stb_image.h` to src
  - Added `stb_image.c` for implimentation
  - Added `premake5.lua` build system
- Version `2.26`
- Sourced from `https://github.com/nothings/stb`

## sort
- Maintained under the `MIT-License`
- Changes
  - Removed `generate_bitonic_sort.py`, `.git`, `.gitignore`, `stresstest.c`, `.travis.yml`, `CONTRIBUTING.md`, `CONTRIBUTORS.md`, `astyle.options`, `Makefile`
  - Removed `doc` directory
- Sourced from `https://github.com/swenson/sort`