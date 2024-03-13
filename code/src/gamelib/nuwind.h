#ifndef NUWIND_H
#define NUWIND_H

#include "../types.h"
#include "../nu.h"

// Size: 0x28
struct nuwindgrp_s
{
    struct nuinstance_s* instance; // Offset: 0x0, DWARF: 0x759D7C
    struct numtx_s* mtx; // Offset: 0x4, DWARF: 0x759DAC
    struct nuvec_s center; // Offset: 0x8, DWARF: 0x759DD7
    char onscreen; // Offset: 0x14, DWARF: 0x759E02
    char inrange; // Offset: 0x15, DWARF: 0x759E2D
    short objcount; // Offset: 0x16, DWARF: 0x759E57
    s32 collide; // Offset: 0x18, DWARF: 0x759E82
    float wind; // Offset: 0x1C, DWARF: 0x759EAC
    float height; // Offset: 0x20, DWARF: 0x759ED3
    float radius; // Offset: 0x24, DWARF: 0x759EFC
}; 

s32 NuWindGCount;
struct nuwindgrp_s NuWindGroup[64];
struct numtx_s NuWindMtxs[512];
s32 NuWindQS;
s32 NuWindDir;
s32 NuWindDir2;
s32 NuWindGCount;
s32 NuWindMtxIndex;
s32 NuWindWave;

#endif // !NUWIND_H