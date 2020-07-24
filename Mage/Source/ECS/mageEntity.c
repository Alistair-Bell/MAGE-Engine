#include <mageAPI.h>

mageEntity mageEntityCreate(struct mageScene *scene)
{
    mageEntity entity;
    assert(mageQueuePop(&scene->Pool.AvailableQueue, &entity, &scene->Allocater) == MAGE_RESULT_SUCCESS);
    scene->Pool.EntityPooledCount++;
    assert(scene->Pool.EntityPooledCount <= scene->Pool.EntityLimit);
    scene->Pool.Pooled[entity] = entity;
    MAGE_LOG_CORE_INFORM("Entity %d registerd\n", entity);
    return entity;
}
void mageEntityBindComponent(struct mageScene *scene, mageEntity entity, const char *componentReference, void *component)
{
    /* Updating the entity's component handle */ 
    uint64_t i;
    uint8_t found = 0;
    for (i = 0; i < scene->Pool.ComponentTableCount; i++)
    {
        if (strcmp(componentReference, scene->Pool.ComponentTables[i].Tag) == 0)
        {
            scene->Pool.ComponentHandles[entity] += scene->Pool.ComponentTables[i].ComponentMask;
            found = 1;
            break;
        }
    }
    struct mageComponentTable *table = &scene->Pool.ComponentTables[i];

    assert(found);
    table->Count++;

    uint32_t size  =  table->ByteSize * table->Count;
    uint32_t index =  table->Count - 1;
    table->Components = scene->Allocater.Reallocater(table->Components, size);
    table->Components[index] = scene->Allocater.Allocate(table->ByteSize);
    MAGE_LOG_CORE_INFORM("Binding entity %d with %s, component handle of %lu\n", entity, componentReference, scene->Pool.ComponentHandles[entity]);
}
void mageEntityDestroy(struct mageScene *scene, mageEntity entity)
{
    uint64_t newSize = sizeof(mageEntity) * scene->Pool.EntityPooledCount;
    scene->Pool.Pooled[entity] = 0;
    scene->Pool.EntityPooledCount--;
    mageQueuePush(&scene->Pool.AvailableQueue, &entity, &scene->Allocater);
    MAGE_LOG_CORE_INFORM("Destroying entity %d, pushing index to available queue\n", entity);
}