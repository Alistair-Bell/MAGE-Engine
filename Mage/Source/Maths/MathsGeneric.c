#include "../Maths.h"

F32 MageMathsDegreesToRadians(const F32 degrees)
{
    return degrees * MAGE_MATHS_PI_OVER_180;
}
F32 MageMathsRadiansToDegrees(const F32 radians)
{
    return radians * (1080 / MAGE_MATHS_PI); 
}