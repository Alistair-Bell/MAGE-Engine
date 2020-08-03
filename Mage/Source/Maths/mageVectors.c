#include <mageAPI.h>

void mageVector2CreateDefault(struct mageVector2 *vector)
{
    memset(vector, 0, sizeof(struct mageVector2));
}
void mageVector2CreateFromFloats(struct mageVector2 *vector, const float x, const float y)
{
    vector->Values[0] = x;
    vector->Values[1] = y;
}
void mageVector2CreateFromCopy(struct mageVector2 *destination, struct mageVector2 *source)
{
    memcpy(destination, source, sizeof(struct mageVector2));
}
void mageVector2Add(struct mageVector2 *destination, struct mageVector2 *other)
{
    destination->Values[0] += other->Values[0];
    destination->Values[1] += other->Values[1];
}
void mageVector2Subtract(struct mageVector2 *destination, struct mageVector2 *other)
{
    destination->Values[0] -= other->Values[0];
    destination->Values[1] -= other->Values[1];
}
void mageVector2Multiply(struct mageVector2 *destination, struct mageVector2 *other)
{
    destination->Values[0] *= other->Values[0];
    destination->Values[1] *= other->Values[1];
}
void mageVector2Divide(struct mageVector2 *destination, struct mageVector2 *other)
{
    destination->Values[0] /= other->Values[0];
    destination->Values[1] /= other->Values[1];
}
void mageVector2Log(const struct mageVector2 *vector, const char *name)
{
    MAGE_LOG_CORE_INFORM("Vector2: %s [%f %f]\n", name, vector->Values[0], vector->Values[1]);
}

void mageVector3CreateDefault(struct mageVector3 *vector)
{
    memset(vector, 0, sizeof(struct mageVector3));
}
void mageVector3CreateFromFloats(struct mageVector3 *vector, const float x, const float y, const float z)
{
    vector->Values[0] = x;
    vector->Values[1] = y;
    vector->Values[2] = z;
}
void mageVector3CreateFromVector2AndFloat(struct mageVector3 *vector, const struct mageVector2 *xy, const float z)
{
    memcpy(vector, xy, sizeof(struct mageVector2));
    vector->Values[2] = z;
}
void mageVector3CreateFromCopy(struct mageVector3 *destination, struct mageVector3 *source)
{
    memcpy(destination, source, sizeof(struct mageVector3));
}
void mageVector3Add(struct mageVector3 *destination, struct mageVector3 *other)
{
    destination->Values[0] += other->Values[0];
    destination->Values[1] += other->Values[1];
    destination->Values[2] += other->Values[2];
}
void mageVector3Subtract(struct mageVector3 *destination, struct mageVector3 *other)
{
    destination->Values[0] -= other->Values[0];
    destination->Values[1] -= other->Values[1];
    destination->Values[2] -= other->Values[2];
}
void mageVector3Multiply(struct mageVector3 *destination, struct mageVector3 *other)
{
    destination->Values[0] *= other->Values[0];
    destination->Values[1] *= other->Values[1];
    destination->Values[2] *= other->Values[2];
}
void mageVector3Divide(struct mageVector3 *destination, struct mageVector3 *other)
{
    destination->Values[0] /= other->Values[0];
    destination->Values[1] /= other->Values[1];
    destination->Values[2] /= other->Values[2];
}
void mageVector3Log(const struct mageVector3 *vector, const char *name)
{
    MAGE_LOG_CORE_INFORM("Vector3: %s [%f %f %f]\n", name, vector->Values[0], vector->Values[1], vector->Values[2]);
}


void mageVector4CreateDefault(struct mageVector4 *vector)
{
    memset(vector, 0, sizeof(struct mageVector4));
}
void mageVector4CreateFromFloats(struct mageVector4 *vector, const float x, const float y, const float z, const float w)
{
    vector->Values[0] = x;
    vector->Values[1] = y;
    vector->Values[2] = z;
    vector->Values[3] = w;
}
void mageVector4CreateFrom2Vector2(struct mageVector4 *vector, const struct mageVector2 *xy, const struct mageVector2 *zw)
{
    memcpy(vector, xy, sizeof(struct mageVector2));
    memcpy(vector + sizeof(struct mageVector2), zw, sizeof(struct mageVector2));
}
void mageVector4CreateFromVector3AndFloat(struct mageVector4 *vector, const struct mageVector3 *xyz, const float w)
{
    memcpy(vector, xyz, sizeof(struct mageVector3));
    vector->Values[3] = w;
}
void mageVector4CreateFromCopy(struct mageVector4 *destination, struct mageVector4 *source)
{
    memcpy(destination, source, sizeof(struct mageVector4));
}
void mageVector4Add(struct mageVector4 *destination, struct mageVector4 *other)
{
    destination->Values[0] += other->Values[0];
    destination->Values[1] += other->Values[1];
    destination->Values[2] += other->Values[2];
    destination->Values[3] += other->Values[3];
}
void mageVector4Subtract(struct mageVector4 *destination, struct mageVector4 *other)
{
    destination->Values[0] -= other->Values[0];
    destination->Values[1] -= other->Values[1];
    destination->Values[2] -= other->Values[2];
    destination->Values[3] -= other->Values[3];
}
void mageVector4Multiply(struct mageVector4 *destination, struct mageVector4 *other)
{
    destination->Values[0] *= other->Values[0];
    destination->Values[1] *= other->Values[1];
    destination->Values[2] *= other->Values[2];
    destination->Values[3] *= other->Values[3];
}
void mageVector4Divide(struct mageVector4 *destination, struct mageVector4 *other)
{
    destination->Values[0] /= other->Values[0];
    destination->Values[1] /= other->Values[1];
    destination->Values[2] /= other->Values[2];
    destination->Values[3] /= other->Values[3];
}
void mageVector4Log(const struct mageVector4 *vector, const char *name)
{
    MAGE_LOG_CORE_INFORM("Vector4: %s [%f %f %f %f]\n", name, vector->Values[0], vector->Values[1], vector->Values[2], vector->Values[3]);
}