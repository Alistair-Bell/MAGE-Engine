#include "mageAPI.h"

void mageFreeMethod(void *item)
{
	free(item);
}
void mageTryDumpSuccess(uint8 contents, uint8 *state)
{
	if (state != NULL)
		*state = contents;
}

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
void mageResizableListPop(mageResizableList *resizableList, void *buffer, const uint8 reallocatable)
{	
	if (reallocatable)
		buffer = realloc(buffer, resizableList->ElementSize);
	
	memcpy(buffer, resizableList->Elements[resizableList->Quantity - 1], resizableList->ElementSize);
	mageFreeMethod(resizableList->Elements[resizableList->Quantity - 1]);
	resizableList->Quantity--;
	resizableList->Elements = realloc(resizableList->Elements, resizableList->ElementSize * resizableList->Quantity);
}
void mageResizableListDestroy(mageResizableList *resizableList)
{
	
}


void mageFileReadContents(const char *file, char *buffer, const uint8 reallocatable, uint8 *success)
{
	FILE *f = fopen(file, "rt");
	if (f == NULL)
	{
		mageTryDumpSuccess(0, success);	
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
	mageTryDumpSuccess(1, success);
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
		mageTryDumpSuccess(0, success);
		return;
	}
	fprintf(f, "%s", buffer);
	fclose(f);

	mageTryDumpSuccess(1, success);

}
void mageGLClearError()
{
	#ifdef MAGE_OPENGL
		while (glGetError() != GL_NO_ERROR);
	#endif
}
void mageGLLogError(const char *function, const char *file, const sint32 line)
{
	#ifdef MAGE_OPENGL
		GLenum error;
		while ((error = glGetError()))
		{
			printf("[OpenGL error] -> %u\n", error);
		}
	#endif
}
