#include <mageAPI.h>

/* Default ecs data methods */



/* Orthographic controller */
void mageOrthographicCameraCreate(struct mageOrthographicCamera *camera, const float left, const float right, const float bottom, const float top)
{
    mageMatrix4CreateDiagonal(&camera->ViewMatrix, 1.0f);
    mageOrthographicCameraSetProjection(camera, left, right, bottom, top);
}
void mageOrthographicCameraCalculateViewMatrix(struct mageOrthographicCamera *camera, struct mageTransform *transform)
{
    struct mageMatrix4 rotation     = (struct mageMatrix4) { .Elements[0] = 1.0f, .Elements[5] = 1.0f, .Elements[10] = 1.0f, .Elements[15] = 1.0f };
    struct mageMatrix4 translation  = (struct mageMatrix4) { .Elements[0] = 1.0f, .Elements[5] = 1.0f, .Elements[10] = 1.0f, .Elements[15] = 1.0f };

    mageMatrix4TranslateVector3(&translation, &transform->Position);
    mageMatrix4RotateAxis(&rotation, mageDegreesToRadians(transform->Rotation.X), &(const struct mageVector3){ .X = 0.0f, .Y = 0.0f, .Z = 1.0f});

    mageMatrix4Multiply(&translation, &rotation);
    mageMatrix4Inverse(&translation);
    
    
    camera->ViewMatrix = translation;
    translation = camera->ViewMatrix;
    mageMatrix4Multiply(&translation, &camera->ProjectionMatrix);
    camera->ViewProjectionMatrix = translation;
}
void mageOrthographicCameraSetProjection(struct mageOrthographicCamera *camera, const float left, const float right, const float bottom, const float top)
{
    mageMatrix4ApplyOrthographicProjection(&camera->ProjectionMatrix, left, right, bottom, top, -1.0f, 1.0f);
    struct mageMatrix4 viewProjection = camera->ViewMatrix;
    mageMatrix4Multiply(&viewProjection, &camera->ProjectionMatrix);
    camera->ViewProjectionMatrix = viewProjection;
}
