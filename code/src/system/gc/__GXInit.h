#pragma once
#include "system.h"
#include "system/gxtype.h"

extern GXContexts __GXContexts;

static inline void set_x2(GXBool value)
{
    __GXContexts.main->x0.u16[1] = value;
}

