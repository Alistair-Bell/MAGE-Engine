#include <mageAPI.h>

void mageSceneCreate(struct mageScene *scene, const struct mageSceneCreateInfo *info)
{
    uint32_t i;
    scene->MaxComponents            = info->ComponentLimit;
    scene->MaxEntities              = info->EntityLimit;

    scene->Entities                 = MAGE_MEMORY_ALLOCATE(sizeof(struct mageEntityPool));
    scene->SceneTag                 = info->SceneTag;
    scene->Systems                  = MAGE_MEMORY_ARRAY_ALLOCATE(0, sizeof(struct mageSystemTable));
    scene->ComponentTables          = MAGE_MEMORY_ARRAY_ALLOCATE(0, sizeof(struct mageComponentTable));

    scene->SystemCount              = 0;
    scene->TableCount               = 0;
    scene->RequiredTableCount       = 0;

    scene->Entities->ActiveCount    = 0;
    scene->Entities->Pooled         = MAGE_MEMORY_ARRAY_ALLOCATE(scene->MaxEntities, sizeof(mageEntity));
    scene->Entities->Handles        = MAGE_MEMORY_ARRAY_ALLOCATE(scene->MaxComponents, sizeof(uint64_t));

    for (i = 0; i < scene->MaxEntities; i++)
    {
        scene->Entities->Handles[i] = MAGE_MEMORY_ARRAY_ALLOCATE(0, sizeof(uint64_t));
    }

    mageQueueCreate(&scene->Entities->AvaliableQueue, sizeof(mageEntity));

}
void mageSceneBindEntityRequiredComponents(struct mageScene *scene, const mageEntity entity, const uint32_t *componentID, const uint32_t count)
{
    uint32_t i;
    uint64_t newSize = sizeof(uint64_t) * scene->RequiredTableCount;
    scene->Entities->Handles[entity] = MAGE_MEMORY_REALLOCATE(scene->Entities->Handles[entity], newSize);
    struct mageComponentTable *table;
    for (i = 0; i < count; i++)
    {
        uint32_t tableIndex             = componentID[i];
        table = &scene->ComponentTables[tableIndex];
        struct mageComponent stored;
        uint32_t componentIndex;
        stored.Data                     = MAGE_MEMORY_ALLOCATE(table->ByteSize);
        stored.SharedCount              = 1;
        
        table->Constructer(stored.Data, table->ByteSize);

        if (table->IndexQueues.Count <= 0)
        {
            componentIndex = table->StoredCount; 
            table->StoredCount++;
            table->Stored = MAGE_MEMORY_REALLOCATE(table->Stored, sizeof(struct mageComponent) * table->StoredCount);
        }

        else
        {
            MAGE_ASSERT(mageQueuePop(&table->IndexQueues, &componentIndex) != MAGE_RESULT_DATA_NOT_PRESENT);
        }
        memcpy(&table->Stored[componentIndex], &stored, sizeof(struct mageComponent));
    }
}
mageEntity mageSceneEntityCreate(struct mageScene *scene)
{
    MAGE_ASSERT(scene != NULL);
    uint32_t i;
    mageEntity returnValue;
    scene->Entities->ActiveCount++;
    mageEntity entity;
    
    if (scene->Entities->AvaliableQueue.Count <= 0)
    {
        entity = scene->Entities->ActiveCount - 1;
    }
    else
    {
        MAGE_ASSERT(mageQueuePop(&scene->Entities->AvaliableQueue, &entity) == MAGE_RESULT_INVALID_INPUT);
    }
    
    uint32_t *indexes = MAGE_MEMORY_ARRAY_ALLOCATE(scene->RequiredTableCount, sizeof(uint32_t));
    uint32_t count = 0;
    for (i = 0; i < scene->TableCount; i++)
    {
        if (scene->ComponentTables[i].TableMode == MAGE_COMPONENT_REGISTERING_MODE_REQUIRED)
        {
            indexes[count] = scene->ComponentTables[i].ID;
            count++;
        }
    }
    MAGE_ASSERT(count == scene->RequiredTableCount);
    mageSceneBindEntityRequiredComponents(scene, entity, indexes, scene->RequiredTableCount);
    MAGE_MEMORY_FREE(indexes);
    return entity;
}
void mageSceneDestroy(struct mageScene *scene)
{
    uint32_t i;
    mageQueueDestroy(&scene->Entities->AvaliableQueue);
    /* Entities */
    MAGE_MEMORY_FREE(scene->Entities->Pooled);
    
    for (i = 0; i < scene->MaxComponents; i++)
    {
        MAGE_MEMORY_FREE(scene->Entities->Handles[i]);
    }

    MAGE_MEMORY_FREE(scene->Entities->Handles);
    /* System tables */

    /* Component tables */
    for (i = 0 ; i < scene->TableCount; i++)
    {
        mageSceneComponentTableFree(&scene->ComponentTables[i]);
    }

    /* Scene */
    MAGE_MEMORY_FREE(scene->Entities);
    MAGE_MEMORY_FREE(scene->Systems);
    MAGE_MEMORY_FREE(scene->ComponentTables);
}