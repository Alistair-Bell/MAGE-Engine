#include <mageAPI.h>


void mageOrthographicCameraCreate(struct mageOrthographicCamera *camera, const float left, const float right, const float bottom, const float top)
{
    mageMatrix4CreateDiagonal(&camera->ViewMatrix, 1.0f);
    mageOrthographicCameraSetProjection(camera, left, right, bottom, top);
}
void mageOrthographicCameraCalculateViewMatrix(struct mageOrthographicCamera *camera)
{
    struct mageMatrix4 translation, rotation;
    mageMatrix4CreateDiagonal(&translation, 1.0f);
    mageMatrix4CreateDiagonal(&rotation, 1.0f);

    mageMatrix4TranslateVector3(&translation, &camera->Transform.Position);
    mageMatrix4RotateAxis(&rotation, mageDegreesToRadians(camera->Transform.Position.Values[0]), &(struct mageVector3){ .X = 0.0f, .Y = 0.0f, .Z = 1.0f});

    mageMatrix4Multiply(&translation, &rotation);

    mageMatrix4Inverse(&translation);

    memcpy(&camera->ViewMatrix, &translation, sizeof(struct mageMatrix4));
    

}
void mageOrthographicCameraSetProjection(struct mageOrthographicCamera *camera, const float left, const float right, const float bottom, const float top)
{
    mageMatrix4ApplyOrthographicProjection(&camera->ProjectionMatrix, left, right, bottom, top, -1.0f, 1.0f);

    struct mageMatrix4 result = camera->ProjectionMatrix;
    mageMatrix4Multiply(&result, &camera->ViewMatrix);
    memcpy(&camera->ViewProjectionMatrix, &result, sizeof(struct mageMatrix4));
}
