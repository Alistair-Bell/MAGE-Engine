#include "mageAPI.h"

void *mageVector2Allocate()
{
	return malloc(sizeof(struct MAGE_VECTOR2_STRUCT));
}
void mageVector2Initialise(mageVector2 *vector2, const float value1, const float value2)
{
	vector2->x = value1;
	vector2->y = value2;
}
void mageVector2Add(mageVector2 *left, const mageVector2 *right)
{
	left->x += right->x;
	left->y += right->y;
}
void mageVector2Subtract(mageVector2 *left, const mageVector2 *right)
{
	left->x -= right->x;
	left->y -= right->y;
}
void mageVector2Multiply(mageVector2 *left, const mageVector2 *right)
{
	left->x *= right->x;
	left->y *= right->y;
}
void mageVector2Divide(mageVector2 *left, const mageVector2 *right)
{
	left->x /= right->x;
	left->y /= right->y;
}
void mageVector2Destroy(mageVector2 *vector)
{
	mageFreeMethod(vector);
}
void *mageVector3Allocate()
{
	return malloc(sizeof(struct MAGE_VECTOR3_STRUCT));
}
void mageVector3Initialise(mageVector3 *vector3, const float value1, const float value2, const float value3)
{
	vector3->x = value1;
	vector3->y = value2;
	vector3->z = value3;
}
void mageVector3Add(mageVector3 *left, const mageVector3 *right)
{
	left->x += right->x;
	left->y += right->y;
	left->z += right->z;
}
void mageVector3Subtract(mageVector3 *left, const mageVector3 *right)
{
	left->x -= right->x;
	left->y -= right->y;
	left->z -= right->z;
}
void mageVector3Multiply(mageVector3 *left, const mageVector3 *right)
{
	left->x *= right->x;
	left->y *= right->y;
	left->z *= right->z;
}
void mageVector3Divide(mageVector3 *left, const mageVector3 *right)
{
	left->x /= right->x;
	left->y /= right->y;
	left->z /= right->z;
}
void mageVector3Destroy(mageVector3 *vector)
{
	mageFreeMethod(vector);
}
void mageVector4Initialise(mageVector4 *vector4, const float value1, const float value2, const float value3, const float value4)
{
	vector4->x = value1;
	vector4->y = value2;
	vector4->z = value3;
	vector4->w = value4;
}
void mageVector4InitialiseVector2(mageVector4 *vector4, const mageVector2 *left, const mageVector2 *right)
{
	vector4->x = left->x;
	vector4->y = left->y;
	vector4->z = right->x;
	vector4->w = right->y;
}
void mageVector4Add(mageVector4 *left, const mageVector4 *right)
{
	left->x += right->x;
	left->y += right->y;
	left->z += right->z;
	left->w += right->w;
}
void mageVector4Subtract(mageVector4 *left, const mageVector4 *right)
{
	left->x -= right->x;
	left->y -= right->y;
	left->z -= right->z;	
	left->w -= right->w;
}
void mageVector4Multiply(mageVector4 *left, const mageVector4 *right)
{
	left->x *= right->x;
	left->y *= right->y;
	left->z *= right->z;
	left->w *= right->w;
}
void mageVector4Divide(mageVector4 *left, const mageVector4 *right)
{
	left->x /= right->x;
	left->y /= right->y;
	left->z /= right->z;
	left->w /= right->w;
}
void mageVector4Destroy(mageVector4 *vector)
{
	mageFreeMethod(vector);
}