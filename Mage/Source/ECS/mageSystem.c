#include "../mageAPI.h"

static uint32_t mageSceneFindComponentHandleByTag(const struct mageScene *scene, const char *tag)
{
    uint32_t i;
    uint8_t found = MAGE_FALSE;
    MAGE_ASSERT(0 <= scene->Pool.ComponentTableCount);
    for (i = 0; i < scene->Pool.ComponentTableCount; i++)
    {
        if (strcmp(tag, scene->Pool.ComponentTables[i].Tag) == 0)
        {
            return scene->Pool.ComponentTables[i].ID;
        }
    }
    MAGE_ASSERT(found != MAGE_FALSE);
    return 0;
}
static uint8_t mageSystemTableCreateFlags(const mageSystemType type, const mageSystemThreadPriority threadPriority)
{
    uint8_t temporary = 0x0;
    temporary += type;
    temporary += (threadPriority << 4);
    MAGE_SET_BIT(temporary, 7, 1);
    return temporary;
}

void mageSceneSystemRegister(struct mageScene *scene, const mageSystemType type, const mageSystemThreadPriority threadPriority, mageSystemCallback system, const uint32_t componentCount, ...)
{
    uint32_t i = 0;
    /* Getting vardic arguments */
    va_list vardicArgumemts;
    va_start(vardicArgumemts, componentCount);
    const char *f = va_arg(vardicArgumemts, const char *);
    char *arguments = malloc(strlen(f));
    strcpy(arguments, f);
    va_end(vardicArgumemts);

    MAGE_ASSERT(2 <= strlen(arguments));
    
    /* Parsing the vardic argument string */
    const char *splitter = ",";
    char *iterator = strtok(arguments, splitter);
    const char *tokens[componentCount];
    uint32_t tablesID[componentCount];


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
        
		iterator = strtok(NULL, splitter);
        i++;
	}
    /* Finding handles */
    for (i = 0; i < componentCount; i++)
    { 
        tablesID[i] = mageSceneFindComponentHandleByTag(scene, tokens[i]);
    }
    scene->Pool.SystemTableCount++;
    uint32_t count = scene->Pool.SystemTableCount;
    struct mageSystemTable table;
    memset(&table, 0, sizeof(struct mageSystemTable));
    
    table.Callback              = system;
    table.ComponentCount        = componentCount;
    table.Flags                 = mageSystemTableCreateFlags(type, threadPriority);
    table.ComponentID           = scene->Allocater.ListAllocater(componentCount, sizeof(uint32_t));
    
    switch (threadPriority)
    {
        case MAGE_SYSTEM_THREAD_PRIORITY_NONE:
            table.SystemThread = NULL;
            break;
        case MAGE_SYSTEM_THREAD_PRIORITY_FORCE:
            table.SystemThread = mageThreadCreate();
            break;
        default:
            MAGE_LOG_CORE_ERROR("Unknown thread priority!, using MAGE_SYSTEM_THREAD_PRIORITY_NONE", NULL);
            table.SystemThread = NULL;
            break;
    }
    memcpy(table.ComponentID, tablesID, sizeof(uint32_t) * componentCount);
    
    scene->Pool.SystemTables = scene->Allocater.Reallocater(scene->Pool.SystemTables, sizeof(struct mageSystemTable) * count);
    memcpy(&scene->Pool.SystemTables[count - 1], &table, sizeof(struct mageSystemTable));
    
    MAGE_LOG_CORE_INFORM("Registering system, requiring %d components %s\n", componentCount, f);
    free(arguments);
    free(iterator);
}
struct mageSystemData mageSceneSystemGetEntityComponents(struct mageScene *scene, const mageEntity entity, const uint32_t *components, const uint32_t count)
{
    struct mageSystemData data;
    uint32_t i;
    return data;

}
void mageSceneSystemFree(struct mageSystemTable *table, struct mageHeapAllocater *allocater)
{
    if (table->SystemThread != NULL)
    {
        allocater->Free(table->SystemThread);
    }
    free(table->ComponentID);
}
