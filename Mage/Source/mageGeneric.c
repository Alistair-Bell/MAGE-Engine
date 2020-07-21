#include "mageAPI.h"

struct mageHeapAllocater mageHeapAllocaterDefault()
{
	return (struct mageHeapAllocater){ .Allocate = malloc, .Free = free, .ListAllocater = calloc, .Reallocater = realloc };
}
char *mageFileReadContents(const char *file, const char *readmode, uint32_t *fileSize)
{
	FILE *f = fopen(file, readmode);
	if (f == NULL)
	{
		MAGE_LOG_CORE_ERROR("Invalid file %s\n", file);
		return NULL;
	}
    fseek(f, 0, SEEK_END);
    uint64_t length = ftell(f);	
	char *foo = malloc(sizeof(char) * (length + 1));
	memset(foo, 0, length + 1);
    fseek(f, 0, SEEK_SET);
    fread(foo, 1, length, f);
    fclose(f);
	
	if (fileSize != NULL) *fileSize = length;
	return foo;
}
mageResult mageFileDumpContents(const char *file, const char *buffer, const uint8_t clean)
{
	FILE *f;
		switch(clean)
	{
		case 1:
			f = fopen(file, "w");
		default:
			f = fopen(file, "a");
	}
	if (f == NULL)
	{
		return MAGE_RESULT_INVALID_INPUT;
	}
	fprintf(f, "%s", buffer);
	fclose(f);

	return MAGE_RESULT_SUCCESS;
}

