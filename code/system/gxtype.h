#ifndef GXTYPES_H
#define GXTYPES_H

#include "types.h"

///////////////////////////Gamecube SDK////////////////////////////

typedef enum
{
    GX_CC_QUARTER = 14,
    GX_CC_TEXBBB = 18,
    GX_CC_TEXGGG = 17,
    GX_CC_TEXRRR = 16,
    GX_CC_ZERO = 15,
    GX_CC_KONST = 14,
    GX_CC_HALF = 13,
    GX_CC_ONE = 12,
    GX_CC_RASA = 11,
    GX_CC_RASC = 10,
    GX_CC_TEXA = 9,
    GX_CC_TEXC = 8,
    GX_CC_A2 = 7,
    GX_CC_C2 = 6,
    GX_CC_A1 = 5,
    GX_CC_C1 = 4,
    GX_CC_A0 = 3,
    GX_CC_C0 = 2,
    GX_CC_APREV = 1,
    GX_CC_CPREV = 0
} GXTevColorArg;

typedef struct
{
    GXTevColorArg a;
    GXTevColorArg b;
    GXTevColorArg c;
    GXTevColorArg d;
} GXTevColorInputCache;

enum GXTexWrapMode
{
    GX_MAX_TEXWRAPMODE = 3,
    GX_MIRROR = 2,
    GX_REPEAT = 1,
    GX_CLAMP = 0
};

enum _GXTevStageID
{
    GX_MAX_TEVSTAGE = 16,
    GX_TEVSTAGE15 = 15,
    GX_TEVSTAGE14 = 14,
    GX_TEVSTAGE13 = 13,
    GX_TEVSTAGE12 = 12,
    GX_TEVSTAGE11 = 11,
    GX_TEVSTAGE10 = 10,
    GX_TEVSTAGE9 = 9,
    GX_TEVSTAGE8 = 8,
    GX_TEVSTAGE7 = 7,
    GX_TEVSTAGE6 = 6,
    GX_TEVSTAGE5 = 5,
    GX_TEVSTAGE4 = 4,
    GX_TEVSTAGE3 = 3,
    GX_TEVSTAGE2 = 2,
    GX_TEVSTAGE1 = 1,
    GX_TEVSTAGE0 = 0
};

enum _GXTexFmt
{
    GX_TF_I4=0,
    GX_TF_I8=1,
    GX_TF_IA4=2,
    GX_TF_IA8=3,
    GX_TF_RGB565=4,
    GX_TF_RGB5A3=5,
    GX_TF_RGBA8=6,
    GX_TF_CMPR=14,
    GX_TF_Z8=17,
    GX_TF_Z16=19,
    GX_TF_Z24X8=22,
    GX_CTF_R4=32,
    GX_CTF_RA4=34,
    GX_CTF_RA8=35,
    GX_CTF_YUVA8=38,
    GX_CTF_A8=39,
    GX_TF_A8=39,
    GX_CTF_R8=40,
    GX_CTF_G8=41,
    GX_CTF_B8=42,
    GX_CTF_RG8=43,
    GX_CTF_GB8=44,
    GX_CTF_Z4=48,
    GX_CTF_Z8M=57,
    GX_CTF_Z8L=58,
    GX_CTF_Z16L=60
};

// Size: 0x20
struct _GXTexObj{
	UNKWORD mFlags; // at 0x0
	char UNK_0x4[0x4];
	UNKWORD mDimensions; // at 0x8 (Height/width are split between these bits)
	char UNK_0xC[0x4];
	UNKWORD mUserData; // at 0x10
	enum _GXTexFmt mFormat; // at 0x14
	UNKWORD mTLUT; // at 0x18
};


// Size: 0xC
struct _GXTlutObj{
    UNKTYPE* dummy[3]; // Offset: 0x0
};


void DCFlushRange(void*, u32);     //DCFlushRange(ram_addr, len)?
UNKTYPE GXInitTexObj(struct _GXTexObj * obj, UNKTYPE *image, u16 width, u32 height, enum _GXTexFmt texFormat, enum GXTexWrapMode wrapModeS, enum GXTexWrapMode wrapModeT, char mipmap); //correct?

/////////////////////////////////////////////////////////////////////

#endif
