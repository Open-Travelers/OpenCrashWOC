#pragma once
#include "system.h"
#include "system/gxtype.h"
#include "system/gc/GXBump.h"

typedef struct __GXGPFifo {
    u16 x0;
    s16 x2;
    s16 x4;
    s16 x6;
    u8 x8_pad[0x38 - 0x8];
} __GXGPFifo;
//typedef struct GXGPFifo GXGPFifo;
//typedef struct GXSettings GXSettings;
