#ifndef NUMEM_H
#define NUMEM_H

#include "types.h"
#include "nucore/nucoretypes.h"
//#include <stdlib.h>
//#include <memory.h>


#ifndef FIRST
    extern struct memexternal_s memext; //extern ?
#endif

extern struct memexternal_s* memexternal;
extern s32 highallocaddr;
static s32 peakallocaddr;
static s32 totalloc;
extern s32 malloced;



//void memset(void*, int, int, ...); // the crclr at 24 means memset takes varargs

// Set memory to be external, with a pointer and ending point.
void NuMemSetExternal(union variptr_u* ptr, union variptr_u* end);

// Allocate memory.
void* NuMemAlloc(s32 size);

// Free memory.
void NuMemFree(void* data);

// Allocate memory.
void* malloc_x(s32 size);

// Free memory.
void free_x(void* data);

#endif // !NUMEM_H
