#include <mageAPI.h>

static void mageDefaultConstructer(void *data, const uint64_t size)
{

}
static void mageDefaultDeconstructer(void *data)
{
    
}

uint32_t mageSceneRegisterComponent(struct mageScene *scene, const char *component, const uint32_t dataSize, mageComponentConstructer constructer, mageComponentDeconstructer deconstructer, const mageComponentRegisteringMode mode)
{
    MAGE_ASSERT(scene != NULL);
    uint64_t tableIndex = scene->TableCount;

    struct mageComponentTable table;
    table.ByteSize          = dataSize;

    if (constructer == NULL)    constructer = mageDefaultConstructer;
    if (deconstructer == NULL)  deconstructer = mageDefaultDeconstructer;
    table.Constructer       = constructer;
    table.Deconstructer     = deconstructer;
    table.StoredCount       = 0;
    table.Stored            = MAGE_MEMORY_ARRAY_ALLOCATE(0, sizeof(struct mageComponent));
    table.Identifier        = component;
    table.ID                = tableIndex;
    table.TableMode         = mode;

    mageQueueCreate(&table.IndexQueues, dataSize);

    scene->TableCount++;
    if (mode == MAGE_ECS_COMPONENT_REGISTERING_MODE_REQUIRED)
    {
       scene->RequiredTableCount++;
    }
    scene->ComponentTables  = MAGE_MEMORY_REALLOCATE(scene->ComponentTables, sizeof(struct mageComponentTable) * scene->TableCount);
    memcpy(&scene->ComponentTables[tableIndex], &table, sizeof(struct mageComponentTable));

    MAGE_LOG_CORE_INFORM("Creating %s component table with index of %d\n", component, tableIndex);
    return tableIndex;
}
void mageSceneComponentTableFree(struct mageComponentTable *table)
{
    uint64_t i;
    mageQueueDestroy(&table->IndexQueues);
    for (i = 0; i < table->StoredCount; i++)
    {
        table->Deconstructer(table->Stored[i].Data);
        MAGE_MEMORY_FREE(table->Stored[i].Data);
    }
    MAGE_MEMORY_FREE(table->Stored);
    MAGE_LOG_CORE_INFORM("Destroying component table %s totaling %d bytes\n", table->Identifier, table->StoredCount * table->ByteSize);
}