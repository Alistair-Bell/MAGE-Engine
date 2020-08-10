#include <mageAPI.h>

mageEntity mageEntityCreate(struct mageScene *scene)
{
    mageEntity entity;
    MAGE_ASSERT(mageQueuePop(&scene->Pool.AvailableQueue, &entity, &scene->Allocater) == MAGE_RESULT_SUCCESS);
    scene->Pool.EntityPooledCount++;
    MAGE_ASSERT(scene->Pool.EntityPooledCount <= scene->Pool.EntityLimit);
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
    MAGE_ASSERT(found);
    struct mageComponentTable *table = &scene->Pool.ComponentTables[i];
    mageComponentHandle *handle = scene->Pool.ComponentHandles[entity];
    table->Count++;
    uint64_t highByte = table->ID;
    uint64_t lowByte;

    if (table->IndexQueue.Count <= 0)
    {
        lowByte = table->Count - 1;
        table->Components = scene->Allocater.Reallocater(table->Components, sizeof(table->ByteSize) * table->Count);
        table->Components[lowByte] = scene->Allocater.Allocate(table->ByteSize);
        memcpy(table->Components[lowByte], component, table->ByteSize);
    }
    else
    {   
        mageQueuePop(&table->IndexQueue, &lowByte, &scene->Allocater);
        memcpy(table->Components[lowByte], component, table->ByteSize);
    }
    uint64_t finalByte = (highByte << 32) + lowByte;
    
    handle[0]++;
    uint64_t index = handle[0];
    handle = scene->Allocater.Reallocater(handle, (handle[0]) * sizeof(mageComponentHandle));
    memcpy(&handle[index], &finalByte, sizeof(uint64_t));
}
void mageEntityDestroy(struct mageScene *scene, mageEntity entity)
{
    uint64_t newSize = sizeof(mageEntity) * scene->Pool.EntityPooledCount;
    scene->Pool.Pooled[entity] = 0;
    scene->Pool.EntityPooledCount--;
    uint32_t i;
    for (i = 1; i < scene->Pool.ComponentHandles[entity][0] + 1; i++)
    {
        uint64_t data = scene->Pool.ComponentHandles[entity][i];
        uint32_t high = (uint32_t)(data >> 32);
        uint32_t low  = (uint32_t)data;
        MAGE_LOG_CORE_INFORM("Table %d -> %s\n", high, scene->Pool.ComponentTables[high - 1].Tag);
    }


    memset(scene->Pool.ComponentHandles[entity], 0, sizeof(mageComponentHandle) * scene->Pool.ComponentHandles[entity][0]);
    mageQueuePush(&scene->Pool.AvailableQueue, &entity, &scene->Allocater);
    MAGE_LOG_CORE_INFORM("Destroying entity %d, pushing index to available queue\n", entity);
}