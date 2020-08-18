#include <mageAPI.h>

/* I would have liked to have this header public in mageCore.h but the compiler was having none of it, an unintended side effect of single header libraries and the macro template structure */
#define SORT_NAME sort_algorithms
#define SORT_TYPE uint64_t
#define SORT_CMP(x, y) ((x) - (y))
#include "../Externals/sort/sort.h"

static void mageEntitiesSortHandles(struct mageComponentHandle *handles, const uint64_t count)
{
    
}

static struct mageComponentHandle mageComponentHandleGenerate(uint64_t tableIndex, uint64_t index)
{
    struct mageComponentHandle handle;
    handle.TableIndex       = tableIndex;
    handle.ComponentIndex   = index;
    return handle;
}

static uint32_t mageAddComponentToTable(struct mageComponentTable *table, struct mageComponent component, const uint64_t maxComponents)
{
    uint32_t componentIndex;
    
    MAGE_ASSERT(table->StoredCount <= maxComponents);

    if (table->IndexQueues.Count <= 0)
    {
        componentIndex = table->StoredCount; 
        table->StoredCount++;
        table->Stored = MAGE_MEMORY_REALLOCATE(table->Stored, sizeof(struct mageComponent) * table->StoredCount);
    }
    else
    {
        mageQueuePop(&table->IndexQueues, &componentIndex);
    }
    if (table->Constructer != NULL)
        table->Constructer(component.Data, table->ByteSize);

    memcpy(&table->Stored[componentIndex], &component, sizeof(struct mageComponent));
    return componentIndex;
}
static void mageUpdateEntityComponentHandle(struct mageComponentHandle *handles, struct mageComponentHandle *addition, const uint32_t count)
{
    handles[0].Data += count;
    uint64_t newSize = (handles[0].Data + 1) * sizeof(struct mageComponentHandle);
    MAGE_ASSERT(newSize < UINT64_MAX);
    handles = MAGE_MEMORY_REALLOCATE(handles, newSize);
    memcpy(handles + 1, addition, sizeof(struct mageComponentHandle) * count);
    mageEntitiesSortHandles(handles, count);
}

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
        scene->Entities->Handles[i] = MAGE_MEMORY_ARRAY_ALLOCATE(10, sizeof(struct mageComponentHandle));
    }

    mageQueueCreate(&scene->Entities->AvaliableQueue, sizeof(mageEntity));

    if (info->RegisterDefaultComponents)
    {   
        MAGE_LOG_CORE_INFORM("Registering default MAGE components\n", NULL);
        MAGE_ECS_REGISTER_COMPONENT(scene, struct mageTransform, NULL, NULL, MAGE_COMPONENT_REGISTERING_MODE_REQUIRED);
    }
}
void mageSceneBindEntityRequiredComponents(struct mageScene *scene, const mageEntity entity, const uint32_t *componentID, const uint32_t count)
{
    uint32_t i;
    uint64_t newSize = sizeof(uint64_t) * scene->RequiredTableCount;
    struct mageComponentTable *table;
    for (i = 0; i < count; i++)
    {
        uint32_t index = componentID[i];
        table = &scene->ComponentTables[index];
        struct mageComponent component;
        component.Data              = MAGE_MEMORY_ALLOCATE(table->ByteSize);
        component.SharedCount       = 1;
        mageAddComponentToTable(table, component, scene->MaxComponents);
    }
}
struct mageComponentHandle mageSceneComponentFromTagBindEntities(struct mageScene *scene, const char *component, void *data, mageEntity *entities, const uint64_t count)
{
    uint32_t i;
    uint8_t found = MAGE_FALSE;
    uint64_t tableIndex;
    uint64_t componentIndex;
    struct mageComponentHandle returnValue;

    /* Finding table */
    
    for (i = 0; i < scene->TableCount; i++)
    {
        if (strcmp(component, scene->ComponentTables[i].Identifier) == 0)
        {
            found = MAGE_TRUE;
    
            break;
        }
    }
    MAGE_ASSERT(found != MAGE_FALSE);
    tableIndex = i;

    /* Allocating component */
    struct mageComponent c;
    c.Data = MAGE_MEMORY_ALLOCATE(scene->ComponentTables[tableIndex].ByteSize);
    c.SharedCount = count;
    componentIndex = mageAddComponentToTable(&scene->ComponentTables[tableIndex], c, scene->MaxComponents);
    
    /*  Generating handle */

    /* High 4 bytes = table id Low 4 bytes = index in table */
    returnValue = mageComponentHandleGenerate(tableIndex, componentIndex);
    
    /* Coppy to entity handle */
    for (i = 0; i < count; i++)
    {
        struct mageComponentHandle *handles = scene->Entities->Handles[entities[i]];
        mageUpdateEntityComponentHandle(handles, &returnValue, 1);
    }
    return returnValue;
}
void mageSceneComponentBindExistingToEntities(struct mageScene *scene, const struct mageComponentHandle componentHandle, mageEntity *entities, const uint32_t count)
{
    uint32_t i;
    struct mageComponentHandle handles[] = { componentHandle };
    scene->ComponentTables[componentHandle.TableIndex].Stored[componentHandle.ComponentIndex].SharedCount += count;
    for (i = 0; i < count; i++)
    {
        uint32_t entityIndex = entities[i];
        mageUpdateEntityComponentHandle(scene->Entities->Handles[entityIndex], handles, 1);
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
    if (0 < scene->Entities->ActiveCount)
    {
        MAGE_LOG_CORE_WARNING("Scene %s has %lu active entities in scene, destroying leftovers is not required it is good practice\n", scene->SceneTag, scene->Entities->ActiveCount);
    }
    uint32_t i;
    mageQueueDestroy(&scene->Entities->AvaliableQueue);
    /* Entities */
    MAGE_MEMORY_FREE(scene->Entities->Pooled);
    
    for (i = 0; i < scene->MaxEntities; i++)
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