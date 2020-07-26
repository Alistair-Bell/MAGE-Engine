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
    uint32_t i;
    uint8_t found = 0;
    for (i = 0; i < scene->Pool.ComponentTableCount; i++)
    {
        if (strcmp(scene->Pool.ComponentTables[i].Tag, componentReference) == 0)
        {
            MAGE_LOG_CORE_INFORM("Binding entity %d with component %s\n", entity, componentReference);
            found = 1;
            break;
        }
    }
    assert(found);
    struct mageComponentTable *table = &scene->Pool.ComponentTables[i];
    mageComponentHandle *handle = scene->Pool.ComponentHandles[entity];
    table->Count++;
    
    if (table->IndexQueue.Count == 0)
    {
        uint32_t index = table->Count - 1;
        table->Components = scene->Allocater.Reallocater(table->Components, sizeof(table->ByteSize) * table->Count);
        table->Components[index] = scene->Allocater.Allocate(table->ByteSize);
        memcpy(table->Components[index], component, table->ByteSize);
    }
    else
    {   
        uint32_t index;
        mageQueuePop(&table->IndexQueue, &index, &scene->Allocater);
        memcpy(table->Components[index], component, table->ByteSize);
    }
    

    uint32_t highByte = table->ID;
    uint32_t lowByte;
    mageComponentHandle finalByte = highByte + lowByte;
    handle[0]++;
    uint64_t index = handle[0];
    handle = scene->Allocater.Reallocater(handle, handle[0] * sizeof(mageComponentHandle));
    memcpy(&handle[index], &finalByte, sizeof(uint64_t));

}
void mageEntityDestroy(struct mageScene *scene, mageEntity entity)
{
    uint64_t newSize = sizeof(mageEntity) * scene->Pool.EntityPooledCount;
    scene->Pool.Pooled[entity] = 0;
    scene->Pool.EntityPooledCount--;
    mageQueuePush(&scene->Pool.AvailableQueue, &entity, &scene->Allocater);
    MAGE_LOG_CORE_INFORM("Destroying entity %d, pushing index to available queue\n", entity);
}