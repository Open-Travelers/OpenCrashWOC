#include "system/gc/GXBump.h"

void __GXFlushTextureState()
{
    GXWGFifo.u8 = GX_LOAD_BP_REG; //0x61
    GXWGFifo.u32 = __GXContexts.main->x100_data[9];
    set_x2(GX_FALSE);
}
