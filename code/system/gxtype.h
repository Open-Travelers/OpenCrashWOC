#ifndef GXTYPES_H
#define GXTYPES_H

#include "types.h"

///////////////////////////Gamecube SDK////////////////////////////

//void GXSetVtxAttrFmt (enum _GXVtxFmt vtxfmt, enum _GXAttr attr, enum _GXCompCnt cnt, enum _GXCompType type, u8 frac);

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



enum _GXChannelID
{
    GX_COLOR_NULL = 255,
    GX_ALPHA_BUMPN = 8,
    GX_ALPHA_BUMP = 7,
    GX_COLOR_ZERO = 6,
    GX_COLOR1A1 = 5,
    GX_COLOR0A0 = 4,
    GX_ALPHA1 = 3,
    GX_ALPHA0 = 2,
    GX_COLOR1 = 1,
    GX_COLOR0 = 0
};

enum _GXLightID
{
    GX_LIGHT_NULL,
    GX_MAX_LIGHT = 256,
    GX_LIGHT7 = 128,
    GX_LIGHT6 = 64,
    GX_LIGHT5 = 32,
    GX_LIGHT4 = 16,
    GX_LIGHT3 = 8,
    GX_LIGHT2 = 4,
    GX_LIGHT1 = 2,
    GX_LIGHT0 = 1
};

// Size: 0xC
struct _GXTlutObj
{
    unsigned int dummy[3]; // Offset: 0x0
};

// Size: 0x40, DWARF: 0xC20D10
struct _GXLightObj
{
    unsigned int dummy[16]; // Offset: 0x0, DWARF: 0xC20D30
};

// Size: 0x4, DWARF: 0xC20A7C
struct _GXColor
{
    unsigned char r; // Offset: 0x0, DWARF: 0xC20A99
    unsigned char g; // Offset: 0x1, DWARF: 0xC20ABD
    unsigned char b; // Offset: 0x2, DWARF: 0xC20AE1
    unsigned char a; // Offset: 0x3, DWARF: 0xC20B05
};


enum _GXTexWrapMode
{
    GX_MAX_TEXWRAPMODE = 3,
    GX_MIRROR = 2,
    GX_REPEAT = 1,
    GX_CLAMP = 0
};

// Size: 0x20
struct _GXTexObj
{
    u32 dummy[8]; // Offset: 0x0
};

enum _GXColorSrc
{
    GX_SRC_VTX = 1,
    GX_SRC_REG = 0
};

enum _GXDiffuseFn
{
    GX_DF_CLAMP = 2,
    GX_DF_SIGN = 1,
    GX_DF_NONE = 0
};

enum _GXAttnFn
{
    GX_AF_NONE = 2,
    GX_AF_SPOT = 1,
    GX_AF_SPEC = 0
};



enum _GXCompare
{
    GX_ALWAYS = 7,
    GX_GEQUAL = 6,
    GX_NEQUAL = 5,
    GX_GREATER = 4,
    GX_LEQUAL = 3,
    GX_EQUAL = 2,
    GX_LESS = 1,
    GX_NEVER = 0
};

enum _GXVtxFmt
{
    GX_MAX_VTXFMT = 8,
    GX_VTXFMT7 = 7,
    GX_VTXFMT6 = 6,
    GX_VTXFMT5 = 5,
    GX_VTXFMT4 = 4,
    GX_VTXFMT3 = 3,
    GX_VTXFMT2 = 2,
    GX_VTXFMT1 = 1,
    GX_VTXFMT0 = 0
};

enum _GXAttr
{
    GX_VA_NULL = 255,
    GX_VA_MAX_ATTR = 26,
    GX_VA_NBT = 25,
    GX_LIGHT_ARRAY = 24,
    GX_TEX_MTX_ARRAY = 23,
    GX_NRM_MTX_ARRAY = 22,
    GX_POS_MTX_ARRAY = 21,
    GX_VA_TEX7 = 20,
    GX_VA_TEX6 = 19,
    GX_VA_TEX5 = 18,
    GX_VA_TEX4 = 17,
    GX_VA_TEX3 = 16,
    GX_VA_TEX2 = 15,
    GX_VA_TEX1 = 14,
    GX_VA_TEX0 = 13,
    GX_VA_CLR1 = 12,
    GX_VA_CLR0 = 11,
    GX_VA_NRM = 10,
    GX_VA_POS = 9,
    GX_VA_TEX7MTXIDX = 8,
    GX_VA_TEX6MTXIDX = 7,
    GX_VA_TEX5MTXIDX = 6,
    GX_VA_TEX4MTXIDX = 5,
    GX_VA_TEX3MTXIDX = 4,
    GX_VA_TEX2MTXIDX = 3,
    GX_VA_TEX1MTXIDX = 2,
    GX_VA_TEX0MTXIDX = 1,
    GX_VA_PNMTXIDX = 0
};

enum _GXAttrType
{
    GX_INDEX16 = 3,
    GX_INDEX8 = 2,
    GX_DIRECT = 1,
    GX_NONE = 0
};

enum _GXCompCnt
{
    GX_TEX_ST = 1,
    GX_TEX_S = 0,
    GX_CLR_RGBA,
    GX_CLR_RGB = 0,
    GX_NRM_NBT3 = 2,
    GX_NRM_NBT = 1,
    GX_NRM_XYZ = 0,
    GX_POS_XYZ,
    GX_POS_XY = 0
};

enum _GXCompType
{
    GX_RGBA8 = 5,
    GX_RGBA6 = 4,
    GX_RGBA4 = 3,
    GX_RGBX8 = 2,
    GX_RGB8 = 1,
    GX_RGB565 = 0,
    GX_F32 = 4,
    GX_S16 = 3,
    GX_U16 = 2,
    GX_S8 = 1,
    GX_U8 = 0
};

/*
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
*/

void DCFlushRange(void*, u32);     //DCFlushRange(ram_addr, len)?
void GXInitTexObj(struct _GXTexObj * obj, void* image_ptr, u16 width, u16 height, enum _GXTexFmt texFormat, enum _GXTexWrapMode wrapModeS, enum _GXTexWrapMode wrapModeT, char mipmap);

/////////////////////////////////////////////////////////////////////

#endif
