#include <mageAPI.h>

void mageMatrix4CreateDefault(struct mageMatrix4 *matrix)
{
    memset(matrix, 0, sizeof(struct mageMatrix4));
}
void mageMatrix4CreateFromRows(struct mageMatrix4 *matrix, const struct mageVector4 *row0, const struct mageVector4 *row1, const struct mageVector4 *row2, const struct mageVector4 *row3)
{
    matrix->Rows[0] = *(row0);
    matrix->Rows[1] = *(row1);
    matrix->Rows[2] = *(row2);
    matrix->Rows[3] = *(row3);
}
void mageMatrix4CreateFromSet(struct mageMatrix4 *matrix, const float *set, const uint32_t setCount)
{
    MAGE_ASSERT(setCount <= 16);
    if (setCount < 16)
    {
        /* Cleaning possible "junk" data */
        mageMatrix4CreateDefault(matrix);
    }
    memcpy(matrix, set, sizeof(float) * setCount);
}
void mageMatrix4CreateDiagonal(struct mageMatrix4 *matrix, const float diagonal)
{
    mageMatrix4CreateDefault(matrix);
    matrix->Elements[0]     = diagonal;
    matrix->Elements[5]     = diagonal;
    matrix->Elements[10]    = diagonal;
    matrix->Elements[15]    = diagonal; 
}
struct mageVector4 mageMatrix4GetRow(const struct mageMatrix4 *matrix, const uint32_t index)
{
    MAGE_ASSERT(4 <= index);
    return matrix->Rows[index];
}
struct mageVector4 mageMatrix4GetColumn(const struct mageMatrix4 *matrix, const uint32_t index)
{
    MAGE_ASSERT(4 <= index);
    struct mageVector4 column;
    column.Values[0] = matrix->Elements[index + 0 * 4];
    column.Values[0] = matrix->Elements[index + 1 * 4];
    column.Values[0] = matrix->Elements[index + 2 * 4];
    column.Values[0] = matrix->Elements[index + 3 * 4];
    
    
    return column;
}
void mageMatrix4Multiply(struct mageMatrix4 *destination, const struct mageMatrix4 *source)
{
    float data[16];
    /* Coppied from the hypatia method and I am terrified I messed up an index, also I cannot be bothered to alling this stuff */

    data[0]     = destination->Elements[0] * source->Elements[0] + destination->Elements[4] * source->Elements[1] + destination->Elements[8] * source->Elements[2] + destination->Elements[12] * source->Elements[3];
	data[1]     = destination->Elements[1] * source->Elements[0] + destination->Elements[5] * source->Elements[1] + destination->Elements[9] * source->Elements[2] + destination->Elements[13] * source->Elements[3];
	data[2]     = destination->Elements[2] * source->Elements[0] + destination->Elements[6] * source->Elements[1] + destination->Elements[10] * source->Elements[2] + destination->Elements[14] * source->Elements[3];
	data[3]     = destination->Elements[3] * source->Elements[0] + destination->Elements[7] * source->Elements[1] + destination->Elements[11] * source->Elements[2] + destination->Elements[15] * source->Elements[3];

	/* second row */
	data[4]     = destination->Elements[0] * source->Elements[4] + destination->Elements[4] * source->Elements[5] + destination->Elements[8] * source->Elements[6] + destination->Elements[12] * source->Elements[7];
	data[5]     = destination->Elements[1] * source->Elements[4] + destination->Elements[5] * source->Elements[5] + destination->Elements[9] * source->Elements[6] + destination->Elements[13] * source->Elements[7];
	data[6]     = destination->Elements[2] * source->Elements[4] + destination->Elements[6] * source->Elements[5] + destination->Elements[10] * source->Elements[6] + destination->Elements[14] * source->Elements[7];
	data[7]     = destination->Elements[3] * source->Elements[4] + destination->Elements[7] * source->Elements[5] + destination->Elements[11] * source->Elements[6] + destination->Elements[15] * source->Elements[7];

	/* third row */
	data[8]     = destination->Elements[0] * source->Elements[8] + destination->Elements[4] * source->Elements[9] + destination->Elements[8] * source->Elements[10] + destination->Elements[12] * source->Elements[11];
	data[9]     = destination->Elements[1] * source->Elements[8] + destination->Elements[5] * source->Elements[9] + destination->Elements[9] * source->Elements[10] + destination->Elements[13] * source->Elements[11];
	data[10]    = destination->Elements[2] * source->Elements[8] + destination->Elements[6] * source->Elements[9] + destination->Elements[10] * source->Elements[10] + destination->Elements[14] * source->Elements[11];
	data[11]    = destination->Elements[3] * source->Elements[8] + destination->Elements[7] * source->Elements[9] + destination->Elements[11] * source->Elements[10] + destination->Elements[15] * source->Elements[11];

	/* fourth row */
	data[12]    = destination->Elements[0] * source->Elements[12] + destination->Elements[4] * source->Elements[13] + destination->Elements[8] * source->Elements[14] + destination->Elements[12] * source->Elements[15];
	data[13]    = destination->Elements[1] * source->Elements[12] + destination->Elements[5] * source->Elements[13] + destination->Elements[9] * source->Elements[14] + destination->Elements[13] * source->Elements[15];
	data[14]    = destination->Elements[2] * source->Elements[12] + destination->Elements[6] * source->Elements[13] + destination->Elements[10] * source->Elements[14] + destination->Elements[14] * source->Elements[15];
	data[15]    = destination->Elements[3] * source->Elements[12] + destination->Elements[7] * source->Elements[13] + destination->Elements[11] * source->Elements[14] + destination->Elements[15] * source->Elements[15];
    memcpy(destination->Elements, data, 4 * 4 * sizeof(float));
}
void mageMatrix4ApplyOrthographicProjection(struct mageMatrix4 *destination, const float left, const float right, const float bottom, const float top, const float near, const float far)
{
    destination->Elements[0]    = 2.0f / (right - left);
    destination->Elements[5]    = 2.0f / (top - bottom);
    destination->Elements[10]   = 2.0f / (near - far);
    destination->Elements[3]    = (left + right) / (left - right);
    destination->Elements[7]    = (bottom + top) / (bottom - top);
    destination->Elements[11]   = (far + near) / (far - near);
    destination->Elements[15]   = 1.0f;
}
void mageMatrix4ApplyPerspectiveProjection(struct mageMatrix4 *destination, const float fieldOfView, const float aspectRatio, const float near, const float far)
{
    const float q = 1.0f / tan(mageDegreesToRadians(0.5f * aspectRatio));
    mageMatrix4CreateDiagonal(destination, 1.0f);
    destination->Elements[0]    = q / aspectRatio;
    destination->Elements[5]    = q;
    destination->Elements[10]   = (near + far) / (near - far);
    destination->Elements[14]   = -1.0f;
    destination->Elements[15]   = (2.0f * near * far) / (near - far);
}   
void mageMatrix4TranslateVector3(struct mageMatrix4 *destination, const struct mageVector3 *translation)
{
    mageMatrix4CreateDefault(destination);
    destination->Elements[12] = translation->Values[0];
    destination->Elements[13] = translation->Values[1];
    destination->Elements[14] = translation->Values[2];
}
void mageMatrix4RotateVector3(struct mageMatrix4 *destination, const float angle, const struct mageVector3 *axis)
{
    mageMatrix4CreateDefault(destination);
    const float r       = mageDegreesToRadians(angle);
    const float c       = cos(r); 
    const float s       = sin(r);
    const float omc     = 1.0f - c;

    destination->Elements[0]    = axis->Values[0] * axis->Values[0] * omc + c;
    destination->Elements[4]    = axis->Values[1] * axis->Values[0] * omc + axis->Values[2] * s;
    destination->Elements[8]    = axis->Values[0] * axis->Values[2] * omc - axis->Values[1] * s;
    destination->Elements[1]    = axis->Values[0] * axis->Values[1] * omc - axis->Values[2] * s;
    destination->Elements[5]    = axis->Values[1] * axis->Values[1] * omc + c;
    destination->Elements[9]    = axis->Values[1] * axis->Values[2] * omc + axis->Values[0] * s;
    destination->Elements[2]    = axis->Values[0] * axis->Values[2] * omc + axis->Values[1] * s;
    destination->Elements[6]    = axis->Values[1] * axis->Values[2] * omc - axis->Values[0] * s;
    destination->Elements[10]   = axis->Values[2] * axis->Values[2] * omc + c;
}
void mageMatrix4ScaleVector3(struct mageMatrix4 *destination, const struct mageVector3 *scale)
{
    mageMatrix4CreateDefault(destination);
    destination->Elements[0]    = scale->Values[0];
    destination->Elements[5]    = scale->Values[1];
    destination->Elements[10]   = scale->Values[2];
}
void mageMatrix4Inverse(struct mageMatrix4 *destination)
{
    float values[16];

    values[0] = destination->Elements[5] * destination->Elements[10] * destination->Elements[15] -
			destination->Elements[5] * destination->Elements[11] * destination->Elements[14] -
			destination->Elements[9] * destination->Elements[6] * destination->Elements[15] +
			destination->Elements[9] * destination->Elements[7] * destination->Elements[14] +
			destination->Elements[13] * destination->Elements[6] * destination->Elements[11] -
			destination->Elements[13] * destination->Elements[7] * destination->Elements[10];

    values[4] = -destination->Elements[4] * destination->Elements[10] * destination->Elements[15] +
        destination->Elements[4] * destination->Elements[11] * destination->Elements[14] +
        destination->Elements[8] * destination->Elements[6] * destination->Elements[15] -
        destination->Elements[8] * destination->Elements[7] * destination->Elements[14] -
        destination->Elements[12] * destination->Elements[6] * destination->Elements[11] +
        destination->Elements[12] * destination->Elements[7] * destination->Elements[10];

    values[8] = destination->Elements[4] * destination->Elements[9] * destination->Elements[15] -
        destination->Elements[4] * destination->Elements[11] * destination->Elements[13] -
        destination->Elements[8] * destination->Elements[5] * destination->Elements[15] +
        destination->Elements[8] * destination->Elements[7] * destination->Elements[13] +
        destination->Elements[12] * destination->Elements[5] * destination->Elements[11] -
        destination->Elements[12] * destination->Elements[7] * destination->Elements[9];

    values[12] = -destination->Elements[4] * destination->Elements[9] * destination->Elements[14] +
        destination->Elements[4] * destination->Elements[10] * destination->Elements[13] +
        destination->Elements[8] * destination->Elements[5] * destination->Elements[14] -
        destination->Elements[8] * destination->Elements[6] * destination->Elements[13] -
        destination->Elements[12] * destination->Elements[5] * destination->Elements[10] +
        destination->Elements[12] * destination->Elements[6] * destination->Elements[9];

    values[1] = -destination->Elements[1] * destination->Elements[10] * destination->Elements[15] +
        destination->Elements[1] * destination->Elements[11] * destination->Elements[14] +
        destination->Elements[9] * destination->Elements[2] * destination->Elements[15] -
        destination->Elements[9] * destination->Elements[3] * destination->Elements[14] -
        destination->Elements[13] * destination->Elements[2] * destination->Elements[11] +
        destination->Elements[13] * destination->Elements[3] * destination->Elements[10];

    values[5] = destination->Elements[0] * destination->Elements[10] * destination->Elements[15] -
        destination->Elements[0] * destination->Elements[11] * destination->Elements[14] -
        destination->Elements[8] * destination->Elements[2] * destination->Elements[15] +
        destination->Elements[8] * destination->Elements[3] * destination->Elements[14] +
        destination->Elements[12] * destination->Elements[2] * destination->Elements[11] -
        destination->Elements[12] * destination->Elements[3] * destination->Elements[10];

    values[9] = -destination->Elements[0] * destination->Elements[9] * destination->Elements[15] +
        destination->Elements[0] * destination->Elements[11] * destination->Elements[13] +
        destination->Elements[8] * destination->Elements[1] * destination->Elements[15] -
        destination->Elements[8] * destination->Elements[3] * destination->Elements[13] -
        destination->Elements[12] * destination->Elements[1] * destination->Elements[11] +
        destination->Elements[12] * destination->Elements[3] * destination->Elements[9];

    values[13] = destination->Elements[0] * destination->Elements[9] * destination->Elements[14] -
        destination->Elements[0] * destination->Elements[10] * destination->Elements[13] -
        destination->Elements[8] * destination->Elements[1] * destination->Elements[14] +
        destination->Elements[8] * destination->Elements[2] * destination->Elements[13] +
        destination->Elements[12] * destination->Elements[1] * destination->Elements[10] -
        destination->Elements[12] * destination->Elements[2] * destination->Elements[9];

    values[2] = destination->Elements[1] * destination->Elements[6] * destination->Elements[15] -
        destination->Elements[1] * destination->Elements[7] * destination->Elements[14] -
        destination->Elements[5] * destination->Elements[2] * destination->Elements[15] +
        destination->Elements[5] * destination->Elements[3] * destination->Elements[14] +
        destination->Elements[13] * destination->Elements[2] * destination->Elements[7] -
        destination->Elements[13] * destination->Elements[3] * destination->Elements[6];

    values[6] = -destination->Elements[0] * destination->Elements[6] * destination->Elements[15] +
        destination->Elements[0] * destination->Elements[7] * destination->Elements[14] +
        destination->Elements[4] * destination->Elements[2] * destination->Elements[15] -
        destination->Elements[4] * destination->Elements[3] * destination->Elements[14] -
        destination->Elements[12] * destination->Elements[2] * destination->Elements[7] +
        destination->Elements[12] * destination->Elements[3] * destination->Elements[6];

    values[10] = destination->Elements[0] * destination->Elements[5] * destination->Elements[15] -
        destination->Elements[0] * destination->Elements[7] * destination->Elements[13] -
        destination->Elements[4] * destination->Elements[1] * destination->Elements[15] +
        destination->Elements[4] * destination->Elements[3] * destination->Elements[13] +
        destination->Elements[12] * destination->Elements[1] * destination->Elements[7] -
        destination->Elements[12] * destination->Elements[3] * destination->Elements[5];

    values[14] = -destination->Elements[0] * destination->Elements[5] * destination->Elements[14] +
        destination->Elements[0] * destination->Elements[6] * destination->Elements[13] +
        destination->Elements[4] * destination->Elements[1] * destination->Elements[14] -
        destination->Elements[4] * destination->Elements[2] * destination->Elements[13] -
        destination->Elements[12] * destination->Elements[1] * destination->Elements[6] +
        destination->Elements[12] * destination->Elements[2] * destination->Elements[5];

    values[3] = -destination->Elements[1] * destination->Elements[6] * destination->Elements[11] +
        destination->Elements[1] * destination->Elements[7] * destination->Elements[10] +
        destination->Elements[5] * destination->Elements[2] * destination->Elements[11] -
        destination->Elements[5] * destination->Elements[3] * destination->Elements[10] -
        destination->Elements[9] * destination->Elements[2] * destination->Elements[7] +
        destination->Elements[9] * destination->Elements[3] * destination->Elements[6];

    values[7] = destination->Elements[0] * destination->Elements[6] * destination->Elements[11] -
        destination->Elements[0] * destination->Elements[7] * destination->Elements[10] -
        destination->Elements[4] * destination->Elements[2] * destination->Elements[11] +
        destination->Elements[4] * destination->Elements[3] * destination->Elements[10] +
        destination->Elements[8] * destination->Elements[2] * destination->Elements[7] -
        destination->Elements[8] * destination->Elements[3] * destination->Elements[6];

    values[11] = -destination->Elements[0] * destination->Elements[5] * destination->Elements[11] +
        destination->Elements[0] * destination->Elements[7] * destination->Elements[9] +
        destination->Elements[4] * destination->Elements[1] * destination->Elements[11] -
        destination->Elements[4] * destination->Elements[3] * destination->Elements[9] -
        destination->Elements[8] * destination->Elements[1] * destination->Elements[7] +
        destination->Elements[8] * destination->Elements[3] * destination->Elements[5];

    values[15] = destination->Elements[0] * destination->Elements[5] * destination->Elements[10] -
        destination->Elements[0] * destination->Elements[6] * destination->Elements[9] -
        destination->Elements[4] * destination->Elements[1] * destination->Elements[10] +
        destination->Elements[4] * destination->Elements[2] * destination->Elements[9] +
        destination->Elements[8] * destination->Elements[1] * destination->Elements[6] -
        destination->Elements[8] * destination->Elements[2] * destination->Elements[5];

    const float determinant = destination->Elements[0] * values[0] + destination->Elements[1] * values[4] + destination->Elements[2] * values[8] + destination->Elements[3] * values[12];
    uint32_t i;

    for (i = 0; i < 16; i++)
    {
        destination->Elements[i] = values[i] * determinant;
    }
}
void mageMatrix4Log(const struct mageMatrix4 *matrix, const char *name)
{
    const struct mageMatrix4 *m = matrix;
    MAGE_LOG_CORE_INFORM("Matrix4: %s \
        \n\t[%f %f %f %f] \
        \n\t[%f %f %f %f] \
        \n\t[%f %f %f %f] \
        \n\t[%f %f %f %f]\n", name, \
        m->Elements[0], m->Elements[1], m->Elements[2], m->Elements[3], \
        m->Elements[4], m->Elements[5], m->Elements[6], m->Elements[7], \
        m->Elements[8], m->Elements[9], m->Elements[10], m->Elements[11], \
        m->Elements[12], m->Elements[13], m->Elements[14], m->Elements[15] );
}