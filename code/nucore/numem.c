#include "numem.h"
#include "nuerror.h"

void* memexternal = NULL;
void* highallocaddr = NULL;
void* peakallocaddr = NULL;
u32 totalloc = 0;
u32 malloced = 0;

void NuMemSetExternal(void* ptr, void* end)
{
	if (ptr != NULL)
	{
		memexternal = memext; // Is it the reference to it or not?
		memext[0] = ptr;
		memext[1] = end;
	}
	else
	{
		memexternal = ptr;
	}
}

void* NuMemAlloc(size_t size)
{
	void* mem = memexternal;
	if (memexternal == NULL)
	{
		totalloc += size;
		mem = malloc(size);
		if (mem == NULL)
		{
			error_func e = NuErrorProlog("OpenCrashWOC/code/nucore/numem.c", 30);
			e("NuMemAlloc : Failed to alloc %d bytes!", size);
			return;
		}
		memset(mem, 0, size);
		void* end = (void*)((u32)mem + size);
		if ((u32)highallocaddr < (u32)end)
		{
			highallocaddr = end;
		}
		if ((u32)peakallocaddr < (u32)end) {
			peakallocaddr = end;
		}
	}
	else
	{
		// HUH???
	}
	return mem;
}

void NuMemFree(void* data)
{
	free(data);
}

void* malloc_x(size_t size)
{
	malloced++;
	return malloc(size);
}

void free_x(void* data)
{
	malloced--;
	free(data);
}