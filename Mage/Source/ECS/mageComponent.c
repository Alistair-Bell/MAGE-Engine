#include <mageAPI.h>

static uint32_t MaskGeneratedCount = 0;

static uint64_t mageGenerateBitMask(const struct mageScene *scene)
{
    return (1UL << scene->Pool.ComponentTableCount);
}
uint64_t mageComponentRegister(struct mageScene *scene, const char *id, const uint64_t size)
{
    assert(scene != NULL);
    uint64_t mask = mageGenerateBitMask(scene);
    assert(scene->Pool.ComponentTableCount < 64);
    scene->Pool.ComponentTableCount++;
    scene->Pool.ComponentTables = scene->Allocater.Reallocater(scene->Pool.ComponentTables, sizeof(struct mageComponentTable) * scene->Pool.ComponentTableCount);
    
    struct mageComponentTable table;
    table.Tag           = id;
    table.ByteSize      = size;
    table.ComponentMask = mask;
    table.Count         = 0;
    table.Components    = scene->Allocater.ListAllocater(0, table.ByteSize);
    
    scene->Pool.ComponentTables[scene->Pool.ComponentTableCount - 1] = table;
    MAGE_LOG_CORE_INFORM("Registerning component %s, created table\n", id, mageGenerateBitMask(scene));
    return mask;
}
void mageComponentTableFree(struct mageComponentTable *table, struct mageHeapAllocater *allocater)
{
    uint32_t i;
    for (i = 0; i < table->Count; i++)
    {
        allocater->Free(table->Components[i]);
    }
    MAGE_LOG_CORE_INFORM("Destroying table %p (%s) with %lu components totalling %lu bytes\n", table, table->Tag, table->Count, table->Count * table->ByteSize);
    allocater->Free(table->Components);
}