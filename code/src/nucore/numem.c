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

//MATCH NGC
void NuMemSetExternal(union variptr_u* ptr, union variptr_u* end) {
	if (ptr != NULL) {
		memexternal = &memext; // Is it the reference to it or not?
		memext.ptr = ptr;
		memext.end = end;
        return;
	}
	memexternal = ptr;
    return;
}

//MATCH NGC
void* NuMemAlloc(s32 size) {
    void* ret;

    // Alloc from main heap if possible
    if (memexternal != NULL) {
        memexternal->ptr->intaddr = (void*) ROUND_UP((u32) memexternal->ptr->voidptr, 16);
        if (memexternal->end != NULL && (u32)memexternal->end->intaddr - (u32)memexternal->ptr->intaddr < size) {
            return NULL;
        }
        ret = memexternal->ptr->intaddr;
        memexternal->ptr->intaddr = ((s8*)memexternal->ptr->intaddr) + size;
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
            NuErrorProlog("OpenCrashWOC/code/nucore/numem.c", 57,"NuMemAlloc : Failed to alloc %d bytes!", size);
        }

        // Clear buffer
        memset(ret, 0, size);

        // Resize heap?
        end = (s32)ret + size;
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
	//printf("\nmalloced: %d\n", malloced);
	return malloc(size);
}

void free_x(void* data)
{
	malloced--;
	free(data);
}
