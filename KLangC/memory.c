#include <stdlib.h>

#include "common.h"
#include "memory.h"

void* reallocate(void* previous, size_t oldSize, size_t newSize)
{
	// Check if size is 0 and free memory
	if (newSize == 0)
	{
		free(previous);
		return NULL;
	}

	return realloc(previous, newSize);
}