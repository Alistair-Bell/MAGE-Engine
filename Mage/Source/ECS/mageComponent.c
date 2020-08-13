#include <mageAPI.h>

uint32_t mageSceneRegisterComponent(struct mageScene *scene, const char *component, const uint32_t dataSize, const mageComponentConstructer constructer, const mageComponentDeconstructer deconstructer, const mageComponentRegisteringMode mode)
{
    assert(scene != NULL);
    uint64_t tableIndex = scene->TableCount;

    struct mageComponentTable table;
    table.ByteSize          = dataSize;
    table.Constructer       = constructer;
    table.Deconstructer     = deconstructer;
    table.StoredCount       = 0;
    table.Stored            = scene->Allocater.ListAllocater(0, sizeof(struct mageComponent));
    table.Identifier        = component;
    table.ID                = tableIndex;
    mageQueueCreate(&table.IndexQueues, dataSize, &scene->Allocater);

    scene->TableCount++;
    scene->ComponentTables  = scene->Allocater.Reallocater(scene->ComponentTables, sizeof(struct mageComponentTable) * scene->TableCount);
    memcpy(&scene->ComponentTables[tableIndex], &table, sizeof(struct mageComponentTable));

    MAGE_LOG_CORE_INFORM("Creating %s component table with index of %d\n", component, tableIndex);
    return tableIndex;
}
void mageSceneComponentTableFree(struct mageComponentTable *table, const struct mageHeapAllocater *allocater)
{
    mageMemoryFreeMethod f = allocater->Free;
    mageQueueDestroy(&table->IndexQueues, allocater);
    f(table->Stored);
    MAGE_LOG_CORE_INFORM("Destroying component table %d\n", table->ID);
}