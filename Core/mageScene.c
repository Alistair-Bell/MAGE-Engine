#include "mageAPI.h"

void *mageCameraAllocate()
{
    return malloc(sizeof(struct MAGE_CAMERA_STRUCT));
}
void mageCameraInitialise(mageCamera *camera, const mageVector3 position, const float rotation, const uint8_t flags)
{
    camera->Position = position;
    camera->Rotation = rotation;
    camera->Flags = flags;   
}
void mageCameraOrthographicSetProjection(mageCamera *camera, const float left, const float right, const float bottom, const float top)
{
    mageMatrix4x4 foo, bar;
    mageMatrix4x4Orthographic(&foo, left, right, bottom, top, -1.0f, 1.0f);
    mageMatrix4x4InitialiseDiagonal(&bar, 1.0f);

    memcpy(camera->ProjectionMatrix.Elements, foo.Elements, sizeof(float) * 16);
    memcpy(camera->ViewMatrix.Elements, foo.Elements, sizeof(float) * 16);

    mageMatrix4x4Multiply(&foo, &bar, &foo);

    memcpy(camera->ViewProjectionMatrix.Elements, foo.Elements, sizeof(float) * 16);
}
void mageCameraOrthographicRecalculateViewMatrix(mageCamera *camera)
{
    /* Variables */
    mageMatrix4x4 transform, translation, identity, rotation;
    mageVector3 angle;
    float radians;

    /* Setup */
    radians = mageToRadians(camera->Rotation);
    mageVector3Initialise(&angle, 0.0f, 0.0f, 1.0f);
    mageMatrix4x4InitialiseDiagonal(&identity, 1.0f);

    /* Matrix maths */
    mageMatrix4x4Translation(&translation, &camera->Position);
    mageMatrix4x4Rotation(&rotation, radians, &angle);
    
    mageMatrix4x4Invert(&transform);

    /* Memory copying to the camera locals */
    memcpy(camera->ViewMatrix.Elements, transform.Elements, sizeof(float) * 16);

    /* Final steps */
    mageMatrix4x4Multiply(&camera->ProjectionMatrix, &camera->ViewMatrix,  &camera->ViewProjectionMatrix);
}
void mageCameraGetFlag(mageCamera *camera, const uint8_t bit, uint8_t *value)
{
    uint8_t temp = camera->Flags >> bit;
    temp &= 0x01;
    mageTryDumpSuccess(temp, value);
}
void mageCameraSetFlag(mageCamera *camera, const uint8_t bit, uint8_t value)
{
    camera->Flags |= value << bit; 
}
void mageSceneInitialise(mageScene *scene)
{
    scene->Cameras = mageResizableListAllocate();
    mageResizableListInitialise(scene->Cameras, sizeof(struct MAGE_CAMERA_STRUCT));
    scene->MasterCamera = NULL;
}
void mageSceneBindMasterCamera(mageScene *scene, mageCamera *camera)
{
    if (scene->MasterCamera != NULL)
    {
        mageCameraSetFlag(scene->MasterCamera, MAGE_CAMERA_BIT_MASTER, 0);
        scene->MasterCamera = NULL;
    }
    mageCameraSetFlag(camera, MAGE_CAMERA_BIT_MASTER, 1);
    scene->MasterCamera = camera;
}