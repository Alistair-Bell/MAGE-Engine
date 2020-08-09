#include <mageAPI.h>

uint64_t mageComponentRegister(struct mageScene *scene, const char *id, const uint64_t size)
{
    assert(scene != NULL);
    scene->Pool.ComponentTableCount++;
    scene->Pool.ComponentTables = scene->Allocater.Reallocater(scene->Pool.ComponentTables, sizeof(struct mageComponentTable) * scene->Pool.ComponentTableCount);
    
    struct mageComponentTable table;
    table.Tag           = id;
    table.ByteSize      = size;
    table.ID            = scene->Pool.ComponentTableCount - 1;
    table.Count         = 0;
    table.Components    = scene->Allocater.ListAllocater(0, table.ByteSize);
    mageQueueCreate(&table.IndexQueue, sizeof(uint32_t), &scene->Allocater);

    memcpy(&scene->Pool.ComponentTables[scene->Pool.ComponentTableCount - 1], &table, sizeof(struct mageComponentTable));
    
    MAGE_LOG_CORE_INFORM("Registerning component %s, created table with id %d\n", id, table.ID);
    return table.ID;
}
void mageComponentTableFree(struct mageComponentTable *table, struct mageHeapAllocater *allocater)
{
    uint32_t i;
    for (i = 0; i < table->Count; i++)
    {
        allocater->Free(table->Components[i]);
    }
    mageQueueDestroy(&table->IndexQueue, allocater);
    MAGE_LOG_CORE_INFORM("Destroying table %p (%s) with %lu components totalling %lu bytes\n", table, table->Tag, table->Count, table->Count * table->ByteSize);
    allocater->Free(table->Components);
}