#include <mageAPI.h>

float mageDegreesToRadians(const float degrees)
{
    return (float)(degrees * (MAGE_PI / 180.0f));
}
float mageRadiansToDegrees(const float radians)
{
    return (float)(radians * (180.0f / MAGE_PI));
}