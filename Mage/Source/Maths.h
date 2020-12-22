#ifndef MATHS_H
#define MATHS_H

#include "Includes.h"

#ifndef max
    #define max(a, b)( ((a) > (b)) ? (a) : (b))
#endif

#ifndef min
    #define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define MAGE_MATHS_PI          3.14159265359
#define MAGE_MATHS_PI_SQUARED  9.86960440109
#define MAGE_MATHS_PI_OVER_180 0.01745329252

typedef struct MageVector2
{
    union
    {
        F32 Values[2];
        struct
        {
            F32 x;
            F32 y;
        };
    };
} MageVector2;

typedef struct MageVector3
{
    union
    {
        F32 Values[3];
        struct
        {
            F32 x;
            F32 y;
            F32 z;
        };
    };
} MageVector3;

typedef struct MageVector4
{
    union
    {
        F32 Values[4];
        struct
        {
            F32 x;
            F32 y;
            F32 z;
            F32 w;
        };
    };
} MageVector4;

extern F32 MageMathsDegreesToRadians(const F32 degrees);
extern F32 MageMathsRadiansToDegrees(const F32 radians);


#endif