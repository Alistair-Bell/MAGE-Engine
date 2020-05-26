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
void *mageMatrix4x4Allocate()
{
	return malloc(sizeof(struct MAGE_MATRIX4X4));
}
void mageMatrix4x4Default(mageMatrix4x4 *matrix)
{
	memset(matrix->Elements, 0, 16 * sizeof(float));
}
void mageMatrix4x4InitialiseVector4(mageMatrix4x4 *matrix, const mageVector4 *column0, const mageVector4 *column1, const mageVector4 *column2, const mageVector4 *column3)
{
	matrix->Elements[0] = column0->x;
	matrix->Elements[4] = column0->x;
	matrix->Elements[8] = column0->x;
	matrix->Elements[12] = column0->x;

	matrix->Elements[1] = column1->y;
	matrix->Elements[5] = column1->y;
	matrix->Elements[9] = column1->y;
	matrix->Elements[13] = column1->y;

	matrix->Elements[2] = column2->z;
	matrix->Elements[6] = column2->z;
	matrix->Elements[10] = column2->z;
	matrix->Elements[14] = column2->z;

	matrix->Elements[3] = column3->w;
	matrix->Elements[7] = column3->w;
	matrix->Elements[11] = column3->w;
	matrix->Elements[15] = column3->w;
}
void mageMatrix4x4InitialiseArray(mageMatrix4x4 *matrix, const float *elements, const uint8_t count)
{		
	mageMatrix4x4Default(matrix);
	memcpy(matrix->Elements, elements, sizeof(float) * count);
}
void mageMatrix4x4InitialiseDiagonal(mageMatrix4x4 *matrix, const float diagonal)
{
	mageMatrix4x4Default(matrix);
	matrix->Elements[0 + 0 * 4] = diagonal;
	matrix->Elements[1 + 1 * 4] = diagonal;
	matrix->Elements[2 + 2 * 4] = diagonal;
	matrix->Elements[3 + 3 * 4] = diagonal;
}
void mageMatrix4x4Multiply(const mageMatrix4x4 *left, const mageMatrix4x4 *right, mageMatrix4x4 *result)
{
	uint8_t row, col, e;
	float data[16];
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
	memcpy(result->Elements, data, 16 * sizeof(float));
}
void mageMatrix4x4Perspective(mageMatrix4x4 *matrix, const float fov, const float aspectRatio, const float near, const float far)
{
	mageMatrix4x4 result;
	mageMatrix4x4InitialiseDiagonal(&result, 1.0f);
	float toRadians = tan(0.5f * fov);
	float radiansResult;
	mageToRadians(toRadians, &radiansResult);
	
	float q = 1.0f / toRadians;
	float a = q / aspectRatio;
	float b = (near + far) / (near - far);
	float c = (2.0f * near * far) / (near - far);
	
	result.Elements[0 + 0 * 4] = a;
	result.Elements[1 + 1 * 4] = q;
	result.Elements[2 + 2 * 4] = b;
	result.Elements[3 + 2 * 4] = -1.0f;
	result.Elements[2 + 3 * 4] = c;
	
	memcpy(matrix->Elements, result.Elements, sizeof(float) * 16);
}
void mageMatrix4x4Orthographic(mageMatrix4x4 *matrix, const float left, const float right, const float bottom, const float top, const float near, const float far)
{
	mageMatrix4x4 result;
	mageMatrix4x4InitialiseDiagonal(&result, 1.0f);

	result.Elements[0 + 0 * 4] = 2.0f / (right - left);
	result.Elements[1 + 1 * 4] = 2.0f / (top - bottom);
	result.Elements[2 + 2 * 4] = 2.0f / (near - far);

	result.Elements[0 + 3 * 4] = 2.0f / (left + right) / (left - right);
	result.Elements[1 + 3 * 4] = 2.0f / (bottom + top) / (bottom - top);
	result.Elements[2 + 3 * 4] = 2.0f / (far + near) / (far - near);

	memcpy(matrix->Elements, result.Elements, sizeof(float) * 16);
}
void mageMatrix4x4Translation(mageMatrix4x4 *matrix, const mageVector3 *translation)
{
	mageMatrix4x4 result;
	mageMatrix4x4InitialiseDiagonal(&result, 1.0f);

	result.Elements[0 + 3 * 4] = translation->x;
	result.Elements[1 + 3 * 4] = translation->y;
	result.Elements[2 + 3 * 4] = translation->z;
	memcpy(matrix->Elements, result.Elements, sizeof(float) * 16);
}
void mageMatrix4x4Rotation(mageMatrix4x4 *matrix, const float angle, const mageVector3 *axis)
{
	mageMatrix4x4 result;
	mageMatrix4x4InitialiseDiagonal(&result, 1.0f);

	float toRadians;
	mageToRadians(angle, &toRadians);
	float r = toRadians;
	float c = cos(r);
	float s = sin(r);
	float omc = 1.0f - c;
	
	float x = axis->x;
	float y = axis->y;
	float z = axis->z;

	result.Elements[0 + 0 * 4] = x * x * omc + c;
	result.Elements[0 + 1 * 4] = y * x * omc + z * s;
	result.Elements[0 + 2 * 4] = x * z * omc - y * s;

	result.Elements[1 + 0 * 4] = x * y * omc - z * s;
	result.Elements[1 + 1 * 4] = y * y * omc + c;
	result.Elements[1 + 2 * 4] = y * z * omc + x * s;

	result.Elements[2 + 0 * 4] = x * z * omc + y * s;
	result.Elements[2 + 1 * 4] = y * z * omc - x * s;
	result.Elements[2 + 2 * 4] = z * z * omc + c;
	
	memcpy(matrix->Elements, result.Elements, sizeof(float) * 16);
}
void mageMatrix4x4Scale(mageMatrix4x4 *matrix, const mageVector3 *scale)
{
	mageMatrix4x4 result;
	mageMatrix4x4InitialiseDiagonal(&result, 1.0f);

	result.Elements[0 + 0 * 4] = scale->x;
	result.Elements[1 + 1 * 4] = scale->y;
	result.Elements[2 + 2 * 4] = scale->z;
	memcpy(matrix, &result, sizeof(struct MAGE_MATRIX4X4));
}
void mageMatrix4x4Invert(mageMatrix4x4 *matrix)
{
	float foo[16];
	mageMatrix4x4 bar;

	mageMatrix4x4Default(&bar);

	foo[0] = bar.Elements[5] * bar.Elements[10] * bar.Elements[15] -
		bar.Elements[5] * bar.Elements[11] * bar.Elements[14] -
		bar.Elements[9] * bar.Elements[6] * bar.Elements[15] +
		bar.Elements[9] * bar.Elements[7] * bar.Elements[14] +
		bar.Elements[13] * bar.Elements[6] * bar.Elements[11] -
		bar.Elements[13] * bar.Elements[7] * bar.Elements[10];

	foo[4] = -bar.Elements[4] * bar.Elements[10] * bar.Elements[15] +
		bar.Elements[4] * bar.Elements[11] * bar.Elements[14] +
		bar.Elements[8] * bar.Elements[6] * bar.Elements[15] -
		bar.Elements[8] * bar.Elements[7] * bar.Elements[14] -
		bar.Elements[12] * bar.Elements[6] * bar.Elements[11] +
		bar.Elements[12] * bar.Elements[7] * bar.Elements[10];

	foo[8] = bar.Elements[4] * bar.Elements[9] * bar.Elements[15] -
		bar.Elements[4] * bar.Elements[11] * bar.Elements[13] -
		bar.Elements[8] * bar.Elements[5] * bar.Elements[15] +
		bar.Elements[8] * bar.Elements[7] * bar.Elements[13] +
		bar.Elements[12] * bar.Elements[5] * bar.Elements[11] -
		bar.Elements[12] * bar.Elements[7] * bar.Elements[9];

	foo[12] = -bar.Elements[4] * bar.Elements[9] * bar.Elements[14] +
		bar.Elements[4] * bar.Elements[10] * bar.Elements[13] +
		bar.Elements[8] * bar.Elements[5] * bar.Elements[14] -
		bar.Elements[8] * bar.Elements[6] * bar.Elements[13] -
		bar.Elements[12] * bar.Elements[5] * bar.Elements[10] +
		bar.Elements[12] * bar.Elements[6] * bar.Elements[9];

	foo[1] = -bar.Elements[1] * bar.Elements[10] * bar.Elements[15] +
		bar.Elements[1] * bar.Elements[11] * bar.Elements[14] +
		bar.Elements[9] * bar.Elements[2] * bar.Elements[15] -
		bar.Elements[9] * bar.Elements[3] * bar.Elements[14] -
		bar.Elements[13] * bar.Elements[2] * bar.Elements[11] +
		bar.Elements[13] * bar.Elements[3] * bar.Elements[10];

	foo[5] = bar.Elements[0] * bar.Elements[10] * bar.Elements[15] -
		bar.Elements[0] * bar.Elements[11] * bar.Elements[14] -
		bar.Elements[8] * bar.Elements[2] * bar.Elements[15] +
		bar.Elements[8] * bar.Elements[3] * bar.Elements[14] +
		bar.Elements[12] * bar.Elements[2] * bar.Elements[11] -
		bar.Elements[12] * bar.Elements[3] * bar.Elements[10];

	foo[9] = -bar.Elements[0] * bar.Elements[9] * bar.Elements[15] +
		bar.Elements[0] * bar.Elements[11] * bar.Elements[13] +
		bar.Elements[8] * bar.Elements[1] * bar.Elements[15] -
		bar.Elements[8] * bar.Elements[3] * bar.Elements[13] -
		bar.Elements[12] * bar.Elements[1] * bar.Elements[11] +
		bar.Elements[12] * bar.Elements[3] * bar.Elements[9];

	foo[13] = bar.Elements[0] * bar.Elements[9] * bar.Elements[14] -
		bar.Elements[0] * bar.Elements[10] * bar.Elements[13] -
		bar.Elements[8] * bar.Elements[1] * bar.Elements[14] +
		bar.Elements[8] * bar.Elements[2] * bar.Elements[13] +
		bar.Elements[12] * bar.Elements[1] * bar.Elements[10] -
		bar.Elements[12] * bar.Elements[2] * bar.Elements[9];

	foo[2] = bar.Elements[1] * bar.Elements[6] * bar.Elements[15] -
		bar.Elements[1] * bar.Elements[7] * bar.Elements[14] -
		bar.Elements[5] * bar.Elements[2] * bar.Elements[15] +
		bar.Elements[5] * bar.Elements[3] * bar.Elements[14] +
		bar.Elements[13] * bar.Elements[2] * bar.Elements[7] -
		bar.Elements[13] * bar.Elements[3] * bar.Elements[6];

	foo[6] = -bar.Elements[0] * bar.Elements[6] * bar.Elements[15] +
		bar.Elements[0] * bar.Elements[7] * bar.Elements[14] +
		bar.Elements[4] * bar.Elements[2] * bar.Elements[15] -
		bar.Elements[4] * bar.Elements[3] * bar.Elements[14] -
		bar.Elements[12] * bar.Elements[2] * bar.Elements[7] +
		bar.Elements[12] * bar.Elements[3] * bar.Elements[6];

	foo[10] = bar.Elements[0] * bar.Elements[5] * bar.Elements[15] -
		bar.Elements[0] * bar.Elements[7] * bar.Elements[13] -
		bar.Elements[4] * bar.Elements[1] * bar.Elements[15] +
		bar.Elements[4] * bar.Elements[3] * bar.Elements[13] +
		bar.Elements[12] * bar.Elements[1] * bar.Elements[7] -
		bar.Elements[12] * bar.Elements[3] * bar.Elements[5];

	foo[14] = -bar.Elements[0] * bar.Elements[5] * bar.Elements[14] +
		bar.Elements[0] * bar.Elements[6] * bar.Elements[13] +
		bar.Elements[4] * bar.Elements[1] * bar.Elements[14] -
		bar.Elements[4] * bar.Elements[2] * bar.Elements[13] -
		bar.Elements[12] * bar.Elements[1] * bar.Elements[6] +
		bar.Elements[12] * bar.Elements[2] * bar.Elements[5];

	foo[3] = -bar.Elements[1] * bar.Elements[6] * bar.Elements[11] +
		bar.Elements[1] * bar.Elements[7] * bar.Elements[10] +
		bar.Elements[5] * bar.Elements[2] * bar.Elements[11] -
		bar.Elements[5] * bar.Elements[3] * bar.Elements[10] -
		bar.Elements[9] * bar.Elements[2] * bar.Elements[7] +
		bar.Elements[9] * bar.Elements[3] * bar.Elements[6];

	foo[7] = bar.Elements[0] * bar.Elements[6] * bar.Elements[11] -
		bar.Elements[0] * bar.Elements[7] * bar.Elements[10] -
		bar.Elements[4] * bar.Elements[2] * bar.Elements[11] +
		bar.Elements[4] * bar.Elements[3] * bar.Elements[10] +
		bar.Elements[8] * bar.Elements[2] * bar.Elements[7] -
		bar.Elements[8] * bar.Elements[3] * bar.Elements[6];

	foo[11] = -bar.Elements[0] * bar.Elements[5] * bar.Elements[11] +
		bar.Elements[0] * bar.Elements[7] * bar.Elements[9] +
		bar.Elements[4] * bar.Elements[1] * bar.Elements[11] -
		bar.Elements[4] * bar.Elements[3] * bar.Elements[9] -
		bar.Elements[8] * bar.Elements[1] * bar.Elements[7] +
		bar.Elements[8] * bar.Elements[3] * bar.Elements[5];

	foo[15] = bar.Elements[0] * bar.Elements[5] * bar.Elements[10] -
		bar.Elements[0] * bar.Elements[6] * bar.Elements[9] -
		bar.Elements[4] * bar.Elements[1] * bar.Elements[10] +
		bar.Elements[4] * bar.Elements[2] * bar.Elements[9] +
		bar.Elements[8] * bar.Elements[1] * bar.Elements[6] -
		bar.Elements[8] * bar.Elements[2] * bar.Elements[5];

	float determinant = bar.Elements[0] * foo[0] + bar.Elements[1] * foo[4] + bar.Elements[2] * foo[8] + bar.Elements[3] * foo[12];
	determinant = 1.0f / determinant;

	uint8_t i;

	for (i = 0; i < 16; i++)
		bar.Elements[i] = foo[i] * determinant;

	memcpy(matrix->Elements, bar.Elements, sizeof(float) * 16);
}


void mageMatrix4x4Destroy(mageMatrix4x4 *matrix)
{
	mageFreeMethod(matrix);
} 
void mageToRadians(const float degrees, float *result)
{
	*result = degrees * (3.1415926535 / 180 );
}