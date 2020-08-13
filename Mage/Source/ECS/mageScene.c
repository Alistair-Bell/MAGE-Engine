#include <mageAPI.h>

void mageSceneCreate(struct mageScene *scene, const struct mageSceneCreateInfo *info)
{
    scene->Allocater                = *(info->Allocater);
    scene->Entities                 = scene->Allocater.Allocate(sizeof(struct mageEntityPool));
    scene->SceneTag                 = info->SceneTag;
    scene->Systems                  = scene->Allocater.ListAllocater(0, sizeof(struct mageSystemTable));
    scene->ComponentTables          = scene->Allocater.ListAllocater(0, sizeof(struct mageComponentTable));

    scene->SystemCount              = 0;
    scene->TableCount               = 0;

    scene->Entities->ActiveCount    = 0;
    scene->Entities->Pooled         = scene->Allocater.ListAllocater(0, sizeof(mageEntity));
    mageQueueCreate(&scene->Entities->AvaliableQueue, sizeof(mageEntity), info->Allocater);

}
mageEntity mageSceneEntityCreate(struct mageScene *scene)
{
    assert(scene != NULL);
    


    
}
void mageSceneDestroy(struct mageScene *scene)
{
    uint32_t i;
    mageQueueDestroy(&scene->Entities->AvaliableQueue, &scene->Allocater);
    mageMemoryFreeMethod f = scene->Allocater.Free;

    /* Entities */
    f(scene->Entities->Pooled);

    /* System tables */

    /* Component tables */
    for (i = 0 ; i < scene->TableCount; i++)
    {
        mageSceneComponentTableFree(&scene->ComponentTables[i], &scene->Allocater);
    }

    /* Scene */
    f(scene->Entities);
    f(scene->Systems);
    f(scene->ComponentTables);
}