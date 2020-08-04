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

## hypatia
- Maintained under `custom` license
  - Changes
    - Removed `cmake`, `help`, `docs`, `scripts` and `tests` directories
    - Removed `.gitignore`, `.travis.yml`, `CMakelists.txt`, `appveyor.yml` and `check.mak`
    - Added `premake5` build support
    - Moved `hypatia.h` to `src` directory
    - Added `hypatia.c` in `src` directory moving all definitions in to the source file
    - Removed the need for `HYPATIA_IMPLEMENTATION`, removed `HYPAPI` from function definitons
- Version `2.0.0`
- Sourced from `https://github.com/dagostinelli/hypatia`

## libyaml
- Maintained under `MIT` license
  - Changes
    - Removed `.github`, `cmake`, `doc`, `docker`, `examples`, `pkg`, `regression-inputs` and `tests` directories
    - Removed `.appveyor.yml`, `.gitignore`, `.indent.pro`, `.makefile`, `.travis.yml`, `CMakeLists.txt`, `Changes`, `Makefile.am`,`announcement.msg`, `bootstrap`, `configure.ac`, `yaml-0.1.pc.in`, `yamlConfig.cmake.in `#
    - Added `premake5` build support 
- Version `0.2.5`
- Sourced from `https://github.com/yaml/libyaml`

## lodepng
- Maintained under `zlib` license
  - Changes
    - Removed `examples` directory
    - Removed `Makefile` `lodepng_benchmark.cpp`, `lodepng_fuzzer.cpp`, `lodepng_unittest.cpp`, `lodepng_util.cpp`, `lodepng_util.h`, `pngdetail.cpp`
    - Added `premake5` build support
    - Renamed `lodepng.cpp` to `lodepng.c`
    - Moved `lodepng.h` and `lodepng.c` to `src` directory
- Sourced from `https://github.com/lvandeve/lodepng`

## Vulkan-Headers
- Maintained under the `Apache License 2.0`
  - Changes
    - Removed `cmake` and `registery` directories
    - Removed `.cmake-format.py`, `.gitattributes`, `.gitignore`, `BUILD.gn`, `BUILD.md`, `CMakeLists.txt` and `vulkan.hpp`
- Version `1.2`
- Sourced from `https://github.com/KhronosGroup/Vulkan-Headers`

## libogg
- Maintained under the `BSD-3-Clause`
- Changes
  - Removed `cmake`, `doc`, `m4`, `macosx` and `win32` directories
  - Removed `aclocal.m4`, `CMakeLists.txt`, `compile`, `config.guess`, `config.sub`, `configure`, `configure.ac`, `depcomp`, `install-sh`, `libogg.spec`, `libogg.spec.in`, `ltmain.sh`, `Makefile.am`, `Makefile.in`, `missing`, `ogg.m4`, `ogg.pc.in` and`ogg-uninstalled.pc.in` files from root
  - Removed `include/Makefile.am`, `include/Makefile.in`, `include/ogg/Makefile.am`, `include/ogg/Makefile.in`, `include/config_types.h.in`, `src/Makefile.am` and `src/Makefile.in`
  - Added `premake5` build support
  - Added `ogg_types` defining the ogg integer types
  - Changed include directories to local includes rather than system wide
- Version `1.3.4`
- Sourced from `https://www.xiph.org/ogg/`

## stb-image
- Maintained under the `MIT-License`
- Changes
  - Moved `stb_image.h` to src
  - Added `stb_image.c` for implimentation
  - Added `premake5.lua` build system
- Version `2.26`
- Sourced from `https://github.com/nothings/stb`