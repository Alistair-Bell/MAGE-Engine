#include <mageAPI.h>

static uint64_t mageGenerateBitMask(const struct mageScene *scene)
{
    return (1UL << scene->Pool.ComponentTableCount);
}
void mageComponentTableFree(struct mageHeapAllocater *allocater, struct mageComponentTable *table)
{
    uint32_t i;
    for (i = 0; i < table->Count; i++)
    {
        allocater->Free(table->Components[i]);
    }
    MAGE_LOG_CORE_INFORM("Destroying table %p (%s) with %lu components totalling %lu bytes\n", table, table->Tag, table->Count, table->Count * table->ByteSize);
    allocater->Free(table->Components);
}
void mageEntityRegisterComponent(struct mageScene *scene, const char *id, const uint64_t size)
{
    assert(scene != NULL);

    scene->Pool.ComponentTableCount++;
    scene->Pool.ComponentTables = scene->Allocater.Reallocater(scene->Pool.ComponentTables, sizeof(struct mageComponentTable) * scene->Pool.ComponentTableCount);
    
    struct mageComponentTable table;
    table.Tag           = id;
    table.ByteSize      = size;
    table.ComponentMask = mageGenerateBitMask(scene);
    table.Count         = 0;
    table.Components    = scene->Allocater.ListAllocater(0, table.ByteSize);
    
    scene->Pool.ComponentTables[scene->Pool.ComponentTableCount - 1] = table;
    MAGE_LOG_CORE_INFORM("Registerning component %s, created table\n", id, mageGenerateBitMask(scene));
}
mageEntity mageEntityCreate(struct mageScene *scene)
{
    mageEntity entity;
    scene->Pool.EntityPooledCount++;
    scene->Pool.Pooled = realloc(scene->Pool.Pooled, sizeof(mageEntity) * scene->Pool.EntityPooledCount);
    uint32_t index = scene->Pool.EntityPooledCount - 1;
    scene->Pool.Pooled[index] = entity;
    entity = index;
    
    return entity;
}
void mageEntityBindComponents(struct mageScene *scene, mageEntity entity, ...)
{
    
}