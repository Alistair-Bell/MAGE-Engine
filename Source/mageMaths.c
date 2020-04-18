#include "mageAPI.h"

void mageVector2Initialise(Vector2 *vector2, const float value1, const float value2)
{
	vector2->x = value1;
	vector2->y = value2;
}
void mageVector2Add(Vector2 *left, const Vector2 *right)
{
	left->x += right->x;
	left->y += right->y;
}
void mageVector2Subtract(Vector2 *left, const Vector2 *right)
{
	left->x -= right->x;
	left->y -= right->y;
}
void mageVector2Multiply(Vector2 *left, const Vector2 *right)
{
	left->x *= right->x;
	left->y *= right->y;
}
void mageVector2Divide(Vector2 *left, const Vector2 *right)
{
	left->x /= right->x;
	left->y /= right->y;
}
void mageVector3Initialise(Vector3 *vector3, const float value1, const float value2, const float value3)
{
	vector3->x = value1;
	vector3->y = value2;
	vector3->z = value3;
}
void mageVector3Add(Vector3 *left, const Vector3 *right)
{
	left->x += right->x;
	left->y += right->y;
	left->z += right->z;
}
void mageVector3Subtract(Vector3 *left, const Vector3 *right)
{
	left->x -= right->x;
	left->y -= right->y;
	left->z -= right->z;
}
void mageVector3Multiply(Vector3 *left, const Vector3 *right)
{
	left->x *= right->x;
	left->y *= right->y;
	left->z *= right->z;
}
void mageVector3Divide(Vector3 *left, const Vector3 *right)
{
	left->x /= right->x;
	left->y /= right->y;
	left->z /= right->z;
}
void mageVector4Initialise(Vector4 *vector4, const float value1, const float value2, const float value3, const float value4)
{
	vector4->x = value1;
	vector4->y = value2;
	vector4->z = value3;
	vector4->w = value4;
}
void mageVector4InitialiseVector2(Vector4 *vector4, const Vector2 *left, const Vector2 *right)
{
	vector4->x = left->x;
	vector4->y = left->y;
	vector4->z = right->x;
	vector4->w = right->y;
}
void mageVector4Add(Vector4 *left, const Vector4 *right)
{
	left->x += right->x;
	left->y += right->y;
	left->z += right->z;
	left->w += right->w;
}
void mageVector4Subtract(Vector4 *left, const Vector4 *right)
{
	left->x -= right->x;
	left->y -= right->y;
	left->z -= right->z;	
	left->w -= right->w;
}
void mageVector4Multiply(Vector4 *left, const Vector4 *right)
{
	left->x *= right->x;
	left->y *= right->y;
	left->z *= right->z;
	left->w *= right->w;
}
void mageVector4Divide(Vector4 *left, const Vector4 *right)
{
	left->x /= right->x;
	left->y /= right->y;
	left->z /= right->z;
	left->w /= right->w;
}
void mageMatrixInitialise(Matrix4x4 *matrix, const Vector4 *row0, const Vector4 *row1, const Vector4 *row2, const Vector4 *row3)
{
	matrix->Elements[0] = row0->x;
	matrix->Elements[1] = row1->x;
	matrix->Elements[2] = row2->x;
	matrix->Elements[3] = row3->x;

	matrix->Elements[4] = row0->y;
	matrix->Elements[5] = row1->y;
	matrix->Elements[6] = row2->y;
	matrix->Elements[7] = row3->y;

	matrix->Elements[8] = row0->z;
	matrix->Elements[9] = row1->z;
	matrix->Elements[10] = row2->z;
	matrix->Elements[11] = row3->z;

	matrix->Elements[12] = row0->w;
	matrix->Elements[13] = row1->w;
	matrix->Elements[14] = row2->w;
	matrix->Elements[15] = row3->w;

}
void mageMatrixMultiply(Matrix4x4 *left, const Matrix4x4 *right)
{
	float data[16];
	uint8 row;
	uint8 col;
	uint8 e;

    for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			float sum = 0.0f;
			for (e = 0; e < 4; e++)
			{
				sum += left->Elements[e + row * 4] * right->Elements[col + e * 4];
			}
			data[col + row * 4] = sum;
		}
	}
	memcpy(left->Elements, data, 4 * 4 * sizeof(float));
}




















