#include "system/gxtype.h"
#include "system/gc/GXTexture.h"



#define INSERT_FIELD(reg, value, nbits, shift)                                \
    (reg) = ((u32) (reg) & ~(((1 << (nbits)) - 1) << (shift))) |              \
            ((u32) (value) << (shift));
#define GX_WRITE_U32(ui) WGPIPE.u32 = (*(u32*) &ui);

void GXSetTevColorIn(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b,
                     GXTevColorArg c, GXTevColorArg d)
{
    GXContext* gx = __GXContexts.main;
    u32* temp_r9 = &gx->x130_data[stage];

    INSERT_FIELD(*temp_r9, a, 4, 12);
    INSERT_FIELD(*temp_r9, b, 4, 8);
    INSERT_FIELD(*temp_r9, c, 4, 4);
    INSERT_FIELD(*temp_r9, d, 4, 0);
    WGPIPE.u8 = GX_LOAD_BP_REG;
    GX_WRITE_U32(*temp_r9);
    set_x2(GX_FALSE);
}


void GXSetTevAlphaIn(GXTevStageID stage, GXTevAlphaArg a, GXTevAlphaArg b,
                     GXTevAlphaArg c, GXTevAlphaArg d)
{
    u32* temp_r9 = &__GXContexts.main->x170_data[stage];

    INSERT_FIELD(*temp_r9, a, 3, 13);
    INSERT_FIELD(*temp_r9, b, 3, 10);
    INSERT_FIELD(*temp_r9, c, 3, 7);
    INSERT_FIELD(*temp_r9, d, 3, 4);
    WGPIPE.u8 = GX_LOAD_BP_REG;
    GX_WRITE_U32(*temp_r9);
    set_x2(GX_FALSE);
}
