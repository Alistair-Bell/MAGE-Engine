#include <mageAPI.h>

/* I would have liked to have this header public in mageCore.h but the compiler was having none of it, an unintended side effect of single header libraries and the macro template structure, works as intended so I dont care */
#define SORT_NAME sort_algorithms
#define SORT_TYPE uint64_t
#define SORT_CMP(x, y) ((x) - (y))
#include "../Externals/sort/sort.h"



static struct mageComponentHandle mageComponentHandleGenerate(uint64_t tableIndex, uint64_t index)
{
    struct mageComponentHandle handle;
    handle.TableIndex       = tableIndex;
    handle.ComponentIndex   = index;
    return handle;
}


static void mageEntitiesSortHandles(struct mageScene *scene, const mageEntity entity, const uint64_t count)
{

    if (count < 3)
        return;
    uint64_t *raw = MAGE_MEMORY_ARRAY_ALLOCATE(count, sizeof(struct mageComponentHandle));
    memcpy(raw, scene->Entities->Handles[entity] + 1, sizeof(struct mageComponentHandle) * count);
    sort_algorithms_heap_sort(raw, count);
    MAGE_MEMORY_FREE(raw);
    MAGE_LOG_CORE_INFORM("Sorting entity %d with %d component handles using heap sort\n", entity, count - 1);
}
static void mageUpdateEntityComponentHandle(struct mageScene *scene, const struct mageComponentHandle *addition, const mageEntity entity, const uint32_t count)
{
    /* Index to add from after */
    uint64_t start = scene->Entities->Handles[entity][0].Data + 1;
    uint64_t oldSize = (scene->Entities->Handles[entity][0].Data + 1) * sizeof(struct mageComponentHandle);
    uint64_t newSize = count * sizeof(struct mageComponentHandle);
    scene->Entities->Handles[entity][0].Data += count;
    scene->Entities->Handles[entity] = MAGE_MEMORY_REALLOCATE(scene->Entities->Handles[entity], newSize + oldSize);
    memcpy(scene->Entities->Handles[entity] + start, addition, newSize);
    mageEntitiesSortHandles(scene, entity, scene->Entities->Handles[entity][0].Data);
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
    table->Constructer(component.Data, table->ByteSize);
    memcpy(&table->Stored[componentIndex], &component, sizeof(struct mageComponent));
    return componentIndex;
}
static int64_t mageBinarySearch(uint64_t *array, uint32_t low, uint32_t high, uint32_t key)
{
    /* Sourced -> https://github.com/vvs14/searching/blob/master/binary_search.c */
	if (low <= high)
	{
		uint32_t mid = low + (high - low) / 2;
		if (key == array[mid])
			return mid;
		
        else if (key < array[mid])
			return mageBinarySearch(array, low, mid - 1, key);
		
        else
			return mageBinarySearch(array, mid + 1, high, key);
	}
	return -1;
}
static void mageCallSystem(struct mageScene *scene, struct mageSystemTable *table, const mageEntity entity)
{
    /* Fetch data */
    if (scene->Entities->Handles[entity][0].Data < table->ComponentCount)
        return;
    
    uint64_t handlesCount = scene->Entities->Handles[entity][0].Data;
    struct mageComponentHandle handles[handlesCount];
    memcpy(handles, scene->Entities->Handles[entity] + 1, sizeof(struct mageComponentHandle) * handlesCount);
    
    MAGE_LOG_CORE_WARNING("HEre\n", NULL);
}
static uint32_t mageFindTableByTag(struct mageScene *scene, const char *tag)
{
    /* Need to find a better way to do this but it will do for now */
    uint32_t i;
    uint8_t found = MAGE_FALSE;
    for (i = 0; i < scene->TableCount; i++)
    {
        if (strcmp(scene->ComponentTables[i].Identifier, tag) == 0) return i;
    }
    MAGE_LOG_CORE_FATAL_ERROR("Unable to find table %s!\n", tag);
    MAGE_ASSERT(found != MAGE_TRUE);
    return MAGE_FALSE;
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
        MAGE_ECS_REGISTER_COMPONENT(scene, struct mageTransform, NULL, NULL, MAGE_ECS_COMPONENT_REGISTERING_MODE_REQUIRED);
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
    uint64_t tableIndex;
    uint64_t componentIndex;
    struct mageComponentHandle returnValue;
    uint32_t i = mageFindTableByTag(scene, component);
    tableIndex = i;

    /* Allocating component */
    struct mageComponent c;
    c.Data = MAGE_MEMORY_ALLOCATE(scene->ComponentTables[tableIndex].ByteSize);
    memcpy(c.Data, data, scene->ComponentTables[tableIndex].ByteSize);
    c.SharedCount = count;
    componentIndex = mageAddComponentToTable(&scene->ComponentTables[tableIndex], c, scene->MaxComponents);


    returnValue = mageComponentHandleGenerate(tableIndex, componentIndex);
    
    /* Coppy to entity handle */
    for (i = 0; i < count; i++)
    {
        mageUpdateEntityComponentHandle(scene, &returnValue, entities[i], 1);
    }
    return returnValue;
}
void mageSceneComponentBindExistingToEntities(struct mageScene *scene, const struct mageComponentHandle componentHandle, mageEntity *entities, const uint32_t count)
{
    uint32_t i;
    scene->ComponentTables[componentHandle.TableIndex].Stored[componentHandle.ComponentIndex].SharedCount += count;
    for (i = 0; i < count; i++)
    {
        uint32_t entityIndex = entities[i];
        mageUpdateEntityComponentHandle(scene, &componentHandle, entities[i], 1);
    }    
}
mageEntity mageSceneEntityCreate(struct mageScene *scene)
{
    MAGE_ASSERT(scene != NULL);
    uint32_t i;
    mageEntity returnValue;
    mageEntity entity;
    
    if (scene->Entities->AvaliableQueue.Count > 0)
    {
        mageQueuePop(&scene->Entities->AvaliableQueue, &entity);
    }
    else
    {
        entity = scene->Entities->ActiveCount;
    }
    
    
    scene->Entities->ActiveCount++;
    uint32_t *indexes = MAGE_MEMORY_ARRAY_ALLOCATE(scene->RequiredTableCount, sizeof(uint32_t));
    uint32_t count = 0;
    for (i = 0; i < scene->TableCount; i++)
    {
        if (scene->ComponentTables[i].TableMode == MAGE_ECS_COMPONENT_REGISTERING_MODE_REQUIRED)
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
void *mageSceneEntityFetchComponentByHandle(struct mageScene *scene, const char *component, const struct mageComponentHandle handle, const mageEntity entity)
{
    uint32_t tableIndex, componentIndex;
    tableIndex      = handle.TableIndex;
    componentIndex  = handle.ComponentIndex;
    MAGE_ASSERT(tableIndex <= scene->TableCount - 1);
    MAGE_ASSERT(componentIndex <= scene->ComponentTables[tableIndex].StoredCount - 1);

    void *data = scene->ComponentTables[tableIndex].Stored[componentIndex].Data;
    if (data == NULL)
    {
        MAGE_LOG_CORE_ERROR("Entity %lu has no current %s component!\n", entity, component);
        return data;
    }
    MAGE_LOG_CORE_INFORM("Found %s component attached to entity %lu\n", component, entity);
    return data;
}
void *mageSceneEntityFetchComponentByTableID(struct mageScene *scene, const uint32_t table, const mageEntity entity)
{
    return NULL;
}
uint32_t mageSceneSystemRegister(struct mageScene *scene, const mageSystemCallback callback, const mageSystemType type, const mageSystemThreadPriority threadPriority, const uint32_t count, ...)
{
    const uint32_t systemIndex = scene->SystemCount;
    scene->SystemCount++;
    scene->Systems = MAGE_MEMORY_REALLOCATE(scene->Systems, sizeof(struct mageSystemTable) * scene->SystemCount);
    
    va_list vardicArguments;
    va_start(vardicArguments, count);
    const char *f = va_arg(vardicArguments, const char *);
    va_end(vardicArguments);
    MAGE_ASSERT(0 != strlen(f));
    char *arguments = MAGE_MEMORY_ALLOCATE(strlen(f) * sizeof(char *));
    strcpy(arguments, f);
    
    const char *tokens[count];
    if (1 < count)
    {
        uint32_t i;
        const char *separator = ",";
        char *iterator = strtok(arguments, separator);     
    
        while(iterator != NULL)
	    {
		    if (iterator[0] == ' ')
            {
                tokens[i] = iterator + 1;
            }
            else
            {
                tokens[i] = iterator;

            }
		    iterator = strtok(NULL, separator);
            i++;
	    }
        MAGE_MEMORY_FREE(iterator);
    }
    else
    {
        tokens[0] = f;
    }

    /* Creating tables */
    struct mageSystemTable table;
    table.ComponentCount            = count;
    table.Identifer                 = systemIndex;
    table.Type                      = type;
    table.ComponentIDs              = MAGE_MEMORY_ARRAY_ALLOCATE(count, sizeof(uint32_t));
    memset(table.ComponentIDs, 0, sizeof(uint32_t) * count);

    uint32_t i, j;
    /* Find handles */
    /* Find a better way to find handles, this is not that efficient but that is a job for future me */
    for (i = 0; i < count; i++)
    {
        uint32_t index = mageFindTableByTag(scene, tokens[i]);
        table.ComponentIDs[i] = scene->ComponentTables[i].ID;
    }
    switch (threadPriority)
    {
        case MAGE_ECS_SYSTEM_THREAD_PRIORITY_NONE:
            table.ThreadHandle      = NULL;
            break;
        case MAGE_ECS_SYSTEM_THREAD_PRIORITY_FORCE:
            table.ThreadHandle      = mageThreadCreate();
            break;
        default:
            MAGE_LOG_CORE_ERROR("Unknown thread priority!, multithreaded disabled\n", NULL);
            table.ThreadHandle      = NULL;
            break;
    }
    memcpy(&scene->Systems[systemIndex], &table, sizeof(struct mageSystemTable));
    MAGE_LOG_CORE_INFORM("Registering system %lu requiring: %s\n", systemIndex + 1, f);
    MAGE_MEMORY_FREE(arguments);
    return systemIndex;
}
void mageSceneEntityDestroy(struct mageScene *scene, mageEntity entity)
{
    MAGE_ASSERT(scene != NULL);

    /* Add index to queue */
    mageQueuePush(&scene->Entities->AvaliableQueue, &entity);

    /* Update components */
    struct mageComponentHandle *handles = scene->Entities->Handles[entity];

    /* Update runtime count */
    uint32_t i, actualIndex;
    struct mageComponent *currentComponent;
    struct mageComponentTable *table;
    for (i = 0; i < handles[0].Data; i++)
    {   
        uint32_t actualIndex = i + 1;
        
        currentComponent = &scene->ComponentTables[handles[actualIndex].TableIndex].Stored[handles[actualIndex].ComponentIndex];
        table = &scene->ComponentTables[actualIndex];
        uint32_t tableIndex = handles[actualIndex].TableIndex;
        currentComponent->SharedCount--;

        if (currentComponent->SharedCount <= 0)
        {
            /* Call deconstructer */
            table->Deconstructer(currentComponent->Data);
            MAGE_LOG_CORE_INFORM("Destroying %s component %p %p\n", table->Identifier, currentComponent->Data);
            MAGE_MEMORY_FREE(currentComponent->Data);
            currentComponent->Data = NULL;
            table->StoredCount--;
        }
    }
    scene->Entities->ActiveCount--;

    mageQueuePush(&scene->Entities->AvaliableQueue, &entity);
    scene->Entities->Handles[entity] = MAGE_MEMORY_REALLOCATE(scene->Entities->Handles[entity], sizeof(struct mageComponentHandle));
    memset(scene->Entities->Handles[entity], 0, sizeof(struct mageComponentHandle));
}
mageResult mageSceneTick(struct mageScene *scene)
{
    uint32_t updateCount        = 0;
    uint32_t fixedCount         = 0;
    uint32_t lateCount          = 0;
 
    /* 
        Execution order
        (0) -> update
        (?) -> fixed update (based on a specified frame count per second)
        (2) -> latupdate 
    */
    struct mageSystemTable *updateTables[scene->SystemCount];
    struct mageSystemTable *fixedTables[scene->SystemCount];
    struct mageSystemTable *lateTables[scene->SystemCount];

    uint32_t i;
    for (i = 0; i < scene->SystemCount; i++)
    {
        switch (scene->Systems[i].Type)
        {
            case MAGE_ECS_SYSTEM_TYPE_UPDATE:
                updateTables[updateCount] = &scene->Systems[i];
                updateCount++;
                break;
            case MAGE_ECS_SYSTEM_TYPE_FIXED_UPDATE:
                fixedTables[fixedCount] = &scene->Systems[i];
                fixedCount++;
                break;
            case MAGE_ECS_SYSTEM_TYPE_LATE_UPDATE:
                lateTables[lateCount] = &scene->Systems[i];
                lateCount++;
                break;
            
            default:
                break;
        }
    }
    MAGE_LOG_CORE_INFORM("Using %d update systems, %d fixed update systems and %d late update systems\n", updateCount, fixedCount, lateCount);



    return MAGE_RESULT_SUCCESS;
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

    for (i = 0; i < scene->SystemCount; i++)
    {
        mageSystemTableDestroy(&scene->Systems[i]);
    }
    MAGE_MEMORY_FREE(scene->Systems);
    MAGE_MEMORY_FREE(scene->ComponentTables);
} 