#include <mageAPI.h>

void mageSceneCreate(struct mageScene *scene, const char *sceneTag, const struct mageHeapAllocater *allocater)
{
    scene->Allocater                        = *(allocater);
    scene->Tag                              = sceneTag;
    scene->Pool.EntityPooledCount           = 0;
    scene->Pool.EntityPooledCount           = 0;
    scene->Pool.ComponentTableCount         = 0;
    scene->Pool.Pooled                      = scene->Allocater.ListAllocater(0, sizeof(mageEntity));
    scene->Pool.ComponentTables             = scene->Allocater.ListAllocater(0, sizeof(struct mageComponentTable));
    
    mageQueueCreate(&scene->Pool.AvailableQueue, sizeof(mageEntity), &scene->Allocater);

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
    uint32_t i;

    /* Freeing component tables */
    for (i = 0; i < scene->Pool.ComponentTableCount; i++)
    {
        mageComponentTableFree(&scene->Allocater, &scene->Pool.ComponentTables[i]);
    }
    scene->Allocater.Free(scene->Pool.Pooled);
    scene->Allocater.Free(scene->Pool.ComponentTables);
    mageQueueDestroy(&scene->Pool.AvailableQueue, &scene->Allocater);
    MAGE_LOG_CORE_INFORM("Scene %s destroyed\n", scene->Tag); 
}