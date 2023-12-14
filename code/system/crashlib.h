#ifndef CRASHLIB_H
#define CRASHLIB_H

#include "gs.h"
#include "nuraster/dxframe.h"
#include "gamecode/main.h"

// Size: 0x14
struct _PERFTIMER
{
    char* name; // Offset: 0x0, DWARF: 0xD3852E
    unsigned long start; // Offset: 0x4, DWARF: 0xD38558
    unsigned long stop; // Offset: 0x8, DWARF: 0xD38580
    unsigned long elapsed; // Offset: 0xC, DWARF: 0xD385A7
    unsigned long average; // Offset: 0x10, DWARF: 0xD385D1
};

struct _PERFTIMER DBTimers[40];

#endif // !GS2_H
