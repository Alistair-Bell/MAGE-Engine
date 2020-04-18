#include "mageAPI.h"

void *mageResizableListAllocate()
{
	return malloc(sizeof(struct MAGE_RESIZABLE_LIST_STRUCT));
}
void mageResizableListInitialise(mageResizableList *resizableList, const uint32 size)
{
	resizableList->Quantity = 0;
	resizableList->ElementSize = size;
}
void mageResizableListPush(mageResizableList *resizableList, void *item)
{
	resizableList->Quantity++;
	resizableList->Elements = realloc(resizableList->Elements, resizableList->ElementSize * resizableList->Quantity);
	resizableList->Elements[resizableList->Quantity - 1] = item;

}
void mageResizableListPop(mageResizableList *resizableList, void (*freeMethod)(void* item))
{
	if (freeMethod == NULL)
		freeMethod = free;
	
	if (0 < resizableList->Quantity)
	{
		freeMethod(resizableList->Elements[resizableList->Quantity - 1]);
		resizableList->Quantity--;	
	}		
}
void mageResizableListDestroy(mageResizableList *resizableList, void (*freeMethod)(void* item))
{
	if (freeMethod == NULL)
		freeMethod = free;
		
	uint32 i;
	for (i = 0; i < resizableList->Quantity - 1; i++)
	{	
		freeMethod(resizableList->Elements[i]); 
	}
	freeMethod(resizableList);
}
void mageResizableListBasicDestroyHandler(void *item)
{
	/* TODO */ 
}
