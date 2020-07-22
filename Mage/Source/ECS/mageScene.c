#include <mageAPI.h>

void mageSceneCreate(struct mageScene *scene, const uint32_t enitityLimit, const char *sceneTag, const struct mageHeapAllocater *allocater)
{
    scene->Allocater                        = *(allocater);
    scene->Tag                              = sceneTag;
    scene->Pool.EntityLimit                 = enitityLimit;
    scene->Pool.EntityPooledCount           = 0;
    scene->Pool.EntityPooledCount           = 0;
    scene->Pool.ComponentTableCount         = 0;
    scene->Pool.Pooled                      = scene->Allocater.ListAllocater(enitityLimit, sizeof(mageEntity));
    scene->Pool.ComponentTables             = scene->Allocater.ListAllocater(0, sizeof(struct mageComponentTable));
    scene->Pool.ComponentHandles            = scene->Allocater.ListAllocater(enitityLimit, sizeof(mageComponent));
    
    memset(scene->Pool.ComponentHandles, 0, sizeof(mageComponent) * enitityLimit);

    uint64_t i;
    mageEntity availableIDs[enitityLimit];
    for (i = 0; i < enitityLimit; i++)
    {
        availableIDs[i] = ((enitityLimit - 1) - i);
    }

    mageQueueCreateFromSet(&scene->Pool.AvailableQueue, enitityLimit, sizeof(mageEntity), availableIDs, &scene->Allocater);
    MAGE_LOG_CORE_INFORM("Creating new scene located at %p with tag Of %s\n", scene, scene->Tag);
}
void mageSceneDisplayInformation(const struct mageScene *scene)
{
    MAGE_LOG_CORE_INFORM("Scene: %s (%p) \
        \n\tPooled objects -> %lu \
        \n\tQueue length -> %lu \
        \n\tComponents registered -> %lu \
        \n\tComponents pooled -> %lu\n", scene->Tag, scene, scene->Pool.EntityPooledCount, scene->Pool.AvailableQueue.Count, scene->Pool.ComponentTableCount, scene->Pool.EntityPooledCount);
}
void mageSceneDestroy(struct mageScene *scene)
{
    mageSceneDisplayInformation(scene);
    if (scene->Pool.EntityPooledCount)
    {
        MAGE_LOG_CORE_WARNING("Scene: %s (%p) has %lu entities still active, make sure they get destroyed\n", scene->Tag, scene, scene->Pool.EntityPooledCount);
    }

    /* Freeing component tables */
    uint32_t i;
    for (i = 0; i < scene->Pool.ComponentTableCount; i++)
    {
        mageComponentTableFree(&scene->Pool.ComponentTables[i], &scene->Allocater);
    }
    scene->Allocater.Free(scene->Pool.ComponentHandles);
    scene->Allocater.Free(scene->Pool.Pooled); 
    scene->Allocater.Free(scene->Pool.ComponentTables);
    mageQueueDestroy(&scene->Pool.AvailableQueue, &scene->Allocater);
    MAGE_LOG_CORE_INFORM("Scene %s destroyed\n", scene->Tag); 
}