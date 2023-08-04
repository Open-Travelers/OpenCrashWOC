#define FIRST 0

#include "numem.h"
#include "nuerror.h"
#include "nucoretypes.h";

struct memexternal_s memext;
extern s32 highallocaddr = 0;
extern s32 peakallocaddr = 0;
extern s32 totalloc = 0;
extern s32 malloced = 0;
struct memexternal_s *memexternal;


#define NuError(msg,line)                                                           \
    error_func e = NuErrorProlog(__FILE__, __LINE__);                          \
    e(msg);

#define ROUND_UP(x, align) (((x) + (align)-1) & (-(align)))

void NuMemSetExternal(union variptr_u* ptr, union variptr_u* end)
{
	if (ptr != NULL)
	{
		memexternal = &memext; // Is it the reference to it or not?
		memext.end = ptr;
		memext.ptr = end;
	}
	else
	{
		memexternal = ptr;
	}
}

void* NuMemAlloc(s32 size) //CHECK
{
    void* ret;

	// Alloc from main heap if possible
    if (memexternal != NULL) {


        //memexternal->ptr->intaddr = (u32*) ROUND_UP((s32) memexternal->ptr->vec4->w, 16);

        if (memexternal->ptr->voidptr != NULL && (s32)&memexternal->ptr->voidptr - (s32)&memexternal->ptr->intaddr < size) {
            return NULL;
        }
        ret = (void*)&memexternal->ptr;
        memexternal->ptr->voidptr = (void*) (((s32)&memexternal->ptr) + size);
        return ret;
    }
	// Main game heap is NULL, fallback on C malloc?
	else {
        s32 end;

		// Total used memory
        totalloc += size;

		// Attempt to allocate
        ret = malloc(size);
        if (ret == NULL) {
            NuError("NuMemAlloc : Failed to alloc %d bytes!", size);
			/*error_func e = NuErrorProlog("OpenCrashWOC/code/nucore/numem.c", 57);
			e("NuMemAlloc : Failed to alloc %d bytes!", size);*/
        }

		// Clear buffer
        memset(ret, 0, size);

		// Resize heap?
        end = size + (s32)ret;
        if (end > highallocaddr) {
            highallocaddr = end;
        }
        if (end > peakallocaddr) {
            peakallocaddr = end;
        }

        return ret;
    }
}

void NuMemFree(void* data)
{
	free(data);
}

void* malloc_x(s32 size)
{
	malloced++;
	printf("\nmalloced: %d\n", malloced);
	return malloc(size);
}

void free_x(void* data)
{
	malloced--;
	free(data);
}
