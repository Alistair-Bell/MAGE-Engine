# Externals

These libraries help power the Mage Engine and I am thankful for the authors of these external libraries for making them free and open source for the public

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
  - Removed `generate_bitonic_sort.py`, `.git`, `.gitignore`, `stresstest.c`, `.travis.yml`, `CONTRIBUTING.md`, `CONTRIBUTORS.md`, `astyle.options`, `Makefile`, `benchmark.c`, `demo.c`, `multdemo.c` and `stresstest.c`
  - Removed `doc` directory
- Sourced from `https://github.com/swenson/sort`

## glad
- Maintained under the `MIT-License`
- Sourced from `https://glad.dav1d.de/` (alternative `https://github.com/Dav1dde/glad`)

# libogg
- Maintained under the BSD-3-Clause
- Changes
  - Removed cmake, doc, m4, macosx and win32 directories
  - Removed aclocal.m4, CMakeLists.txt, compile, config.guess, config.sub, configure, configure.ac, depcomp, install-sh, libogg.spec, libogg.spec.in, ltmain.sh, Makefile.am, Makefile.in, missing, ogg.m4, ogg.pc.in andogg-uninstalled.pc.in files from root
  - Removed include/Makefile.am, include/Makefile.in, include/ogg/Makefile.am, include/ogg/Makefile.in, include/config_types.h.in, src/Makefile.am and src/Makefile.in
- Added premake5 build support
  - Added ogg_types defining the ogg integer types
  - Changed include directories to local includes rather than system wide
- Version 1.3.4
- Sourced from https://www.xiph.org/ogg/