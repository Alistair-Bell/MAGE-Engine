#include <mageAPI.h>

void mageSystemTableDestroy(struct mageSystemTable *table)
{
    if (table->ThreadHandle != NULL)
    {
        MAGE_MEMORY_FREE(table->ThreadHandle);
    }
    MAGE_MEMORY_FREE(table->ComponentIDs);
}
