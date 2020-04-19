#include "mageAPI.h"
#include <string.h>

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
		if (resizableList->Elements[i] != NULL)	
			freeMethod(resizableList->Elements[i]); 
	}
	freeMethod(resizableList);
}
void mageResizableListBasicDestroyHandler(void *item)
{
	/* TODO */ 
}


void mageFileReadContents(const char *file, char *buffer, const uint8 reallocatable, uint8 *success)
{
	FILE *f = fopen(file, "rt");
	if (f == NULL)
	{
		if (success != NULL)
			*success = 0;	
		return;
	}
    fseek(f, 0, SEEK_END);
    uint64 length = ftell(f);	
	char *foo = malloc(sizeof(char) * (length + 1));
	memset(foo, 0, length + 1);
    fseek(f, 0, SEEK_SET);
    fread(foo, 1, length, f);
    fclose(f);
	
	if (reallocatable)
	{
		buffer = realloc(buffer, sizeof(foo));
	}
	strcpy(buffer, foo);
	free(foo);
	if (success != NULL)
		*success = 1;
}
void mageFileDumpContents(const char *file, const char *buffer, const uint8 clean, uint8 *success)
{
	FILE *f;
	switch(clean)
	{
		case 0:
			f = fopen(file, "w");
		default:
			f = fopen(file, "w+");
	}
	if (f == NULL)
	{
		if (success != NULL)
			*success = 0;	
		return;
	}
	fprintf(f, "%s", buffer);
	fclose(f);

	if (success != NULL)
		*success = 1;

}
void mageGLClearError()
{
	#ifdef __MAGE_OPENGL_
		while (glGetError() != GL_NO_ERROR);
	#endif
}
void mageGLLogError(const char *function, const char *file, const sint32 line)
{
	#ifdef __MAGE_OPENGL_
		GLenum error;
		while ((error = glGetError()))
		{
			printf("[OpenGL error] -> %u\n", error);
		}
	#endif
}
