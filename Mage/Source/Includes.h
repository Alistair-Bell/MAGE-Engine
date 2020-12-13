#ifndef INCLUDES_H
#define INCLUDES_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#if MAGE_BUILD_PLATFORM_LINUX
    #include <X11/Xlib.h>
    #include <X11/X.h>
#endif

#if MAGE_BUILD_DEBUG_MODE
    #define MAGE_HANDLE_ERROR_MESSAGE(condition, execute) if ((condition)) { execute; return MageFalse; }
#else
    #define MAGE_HANDLE_ERROR_MESSAGE(condition, execute)
#endif

typedef void U0;
typedef char I8;
typedef unsigned char U8;
typedef short I16;
typedef unsigned short U16;
typedef int I32;
typedef unsigned int U32;
typedef long I64;
typedef unsigned long U64;

typedef float F32;
typedef double F64;

#define MageTrue 1
#define MageFalse 0

#endif
