#ifndef GXTYPES_H
#define GXTYPES_H

#include "types.h"

static inline u32 ppcmask(u32 mb, u32 me)
{
   u32 maskmb = ~0u >> mb;
   u32 maskme = ~0u << (31 - me);
   return (mb <= me) ? maskmb & maskme : maskmb | maskme;
}

static inline u32 rotl32(u32 x, u32 amount)
{
   return (x << amount) | (x >> ((32 - amount) & 31));
}

static inline u32 __rlwinm(u32 rs, u32 sh, u32 mb, u32 me)
{
   return rotl32(rs, sh) & ppcmask(mb, me);
}

static inline u32 __rlwimi(u32 ra, u32 rs, u32 sh, u32 mb, u32 me)
{
   u32 mask = ppcmask(mb, me);
   return (ra & ~mask) | (rotl32(rs, sh) & mask);
}

///////////////////////////Gamecube SDK////////////////////////////
#define GX_BP_LOAD_REG(data)                \
	GXWGFifo.s8  = GX_FIFO_CMD_LOAD_BP_REG; \
	GXWGFifo.s32 = (data);
#define GX_GET_REG(reg, st, end)      GX_BITGET((reg), (st), ((end) - (st) + 1))
#define GX_SET_REG(reg, x, st, end)   GX_BITFIELD_SET((reg), (st), ((end) - (st) + 1), (x))
#define GX_BITFIELD(field, pos, size, value)       (__rlwimi((field), (value), 31 - (pos) - (size) + 1, (pos), (pos) + (size)-1))
#define GX_BITFIELD_SET(field, pos, size, value)   ((field) = GX_BITFIELD(field, pos, size, value))

#define GX_FIFO_OBJ_SIZE (128)
typedef u8 GXBool;
#define GX_FALSE ((GXBool) 0)
#define GX_TRUE ((GXBool) 1)

#define GX_ENABLE ((GXBool) 1)
#define GX_DISABLE ((GXBool) 0)
#define VI_TVMODE(format, interlace)  (((format) << 2) + (interlace))
#define VI_INTERLACE     0
#define VI_NON_INTERLACE 1
#define VI_PROGRESSIVE   2

#define VI_NTSC      0
#define VI_PAL       1
#define VI_MPAL      2
#define VI_DEBUG     3
#define VI_DEBUG_PAL 4
#define VI_EURGB60   5

//void GXSetVtxAttrFmt (enum _GXVtxFmt vtxfmt, enum _GXAttr attr, enum _GXCompCnt cnt, enum _GXCompType type, u8 frac);

#define VI_VERT_COUNT  (22) // vertical display position
#define VI_HORIZ_COUNT (23) // horizontal display position
vu16 __VIRegs[59];

typedef enum
{
    VI_TVMODE_NTSC_INT      = VI_TVMODE(VI_NTSC,        VI_INTERLACE),
    VI_TVMODE_NTSC_DS       = VI_TVMODE(VI_NTSC,        VI_NON_INTERLACE),
    VI_TVMODE_NTSC_PROG     = VI_TVMODE(VI_NTSC,        VI_PROGRESSIVE),
    VI_TVMODE_PAL_INT       = VI_TVMODE(VI_PAL,         VI_INTERLACE),
    VI_TVMODE_PAL_DS        = VI_TVMODE(VI_PAL,         VI_NON_INTERLACE),
    VI_TVMODE_EURGB60_INT   = VI_TVMODE(VI_EURGB60,     VI_INTERLACE),
    VI_TVMODE_EURGB60_DS    = VI_TVMODE(VI_EURGB60,     VI_NON_INTERLACE),
    VI_TVMODE_MPAL_INT      = VI_TVMODE(VI_MPAL,        VI_INTERLACE),
    VI_TVMODE_MPAL_DS       = VI_TVMODE(VI_MPAL,        VI_NON_INTERLACE),
    VI_TVMODE_DEBUG_INT     = VI_TVMODE(VI_DEBUG,       VI_INTERLACE),
    VI_TVMODE_DEBUG_PAL_INT = VI_TVMODE(VI_DEBUG_PAL,   VI_INTERLACE),
    VI_TVMODE_DEBUG_PAL_DS  = VI_TVMODE(VI_DEBUG_PAL,   VI_NON_INTERLACE)
} VITVMode;

typedef enum
{
    VI_XFBMODE_SF = 0,
    VI_XFBMODE_DF
} VIXFBMode;

typedef struct _GXRenderModeObj
{
    /*0x00*/ VITVMode viTVmode;
    /*0x04*/ u16 fbWidth;
    /*0x06*/ u16 efbHeight;
    /*0x08*/ u16 xfbHeight;
    /*0x0A*/ u16 viXOrigin;
    /*0x0C*/ u16 viYOrigin;
    /*0x0E*/ u16 viWidth;
    /*0x10*/ u16 viHeight;
    /*0x14*/ VIXFBMode xFBmode;
    /*0x18*/ u8 field_rendering;
    u8 aa;
    u8 sample_pattern[12][2];
    u8 vfilter[7];
} GXRenderModeObj;

GXRenderModeObj GXNtsc480IntDf;
GXRenderModeObj GXPal528IntDf;
GXRenderModeObj GXMpal480IntDf;

// Commands for interacting with the GXFifo pipe.
typedef enum _GXFifoCmd {
	GX_FIFO_CMD_NOOP = 0x00, // no operation

	GX_FIFO_CMD_LOAD_BP_REG = 0x61, // load blitting processor reg
	GX_FIFO_CMD_LOAD_CP_REG = 0x08, // load command processor reg
	GX_FIFO_CMD_LOAD_XF_REG = 0x10, // load transform unit reg

	GX_FIFO_CMD_LOAD_INDX_A = 0x20, // load index A
	GX_FIFO_CMD_LOAD_INDX_B = 0x28, // load index B
	GX_FIFO_CMD_LOAD_INDX_C = 0x30, // load index C
	GX_FIFO_CMD_LOAD_INDX_D = 0x38, // load index D

	GX_FIFO_CMD_CALL_DL   = 0x40, // call displaylist
	GX_FIFO_CMD_INVAL_VTX = 0x48, // invalid vertex

} GXFifoCmd;

// BP GenMode locators.
typedef enum _GXBPGenMode {
	// Active texture counts [28-31]
	GX_BP_GENMODE_NUMTEX_ST  = 28,
	GX_BP_GENMODE_NUMTEX_END = 31,

	// Color/channel counts [25-27]
	GX_BP_GENMODE_NUMCOLORS_ST  = 25,
	GX_BP_GENMODE_NUMCOLORS_END = 27,

	// Multisample mode [22-22]
	GX_BP_GENMODE_MULTISAMPLE_ST  = 22,
	GX_BP_GENMODE_MULTISAMPLE_END = 22,

	// Cull mode [16-17]
	GX_BP_GENMODE_CULLMODE_ST  = 16,
	GX_BP_GENMODE_CULLMODE_END = 17,

	// Indirect stage counts [13-15]
	GX_BP_GENMODE_NUMINDSTAGES_ST  = 13,
	GX_BP_GENMODE_NUMINDSTAGES_END = 15,

	// Toggle co-planar/Z-freeze [12-12]
	GX_BP_GENMODE_COPLANAR_ST  = 12,
	GX_BP_GENMODE_COPLANAR_END = 12,
} GXBPGenMode;

// Flags for setting GXData dirtyState.
typedef enum _GXDirtyFlag {
	GX_DIRTY_SU_TEX   = (1 << 0), // 0x1
	GX_DIRTY_BP_MASK  = (1 << 1), // 0x2
	GX_DIRTY_GEN_MODE = (1 << 2), // 0x4
	GX_DIRTY_VCD      = (1 << 3), // 0x8
	GX_DIRTY_VAT      = (1 << 4), // 0x10
	// . . .
	GX_DIRTY_AMB_COLOR0  = (1 << 8),  // 0x100
	GX_DIRTY_AMB_COLOR1  = (1 << 9),  // 0x200
	GX_DIRTY_MAT_COLOR0  = (1 << 10), // 0x400
	GX_DIRTY_MAT_COLOR1  = (1 << 11), // 0x800
	GX_DIRTY_CHAN_COLOR0 = (1 << 12), // 0x1000
	GX_DIRTY_CHAN_COLOR1 = (1 << 13), // 0x2000
	GX_DIRTY_CHAN_ALPHA0 = (1 << 14), // 0x4000
	GX_DIRTY_CHAN_ALPHA1 = (1 << 15), // 0x8000
	GX_DIRTY_TEX0        = (1 << 16), // 0x10000
	GX_DIRTY_TEX1        = (1 << 17), // 0x20000
	GX_DIRTY_TEX2        = (1 << 18), // 0x40000
	GX_DIRTY_TEX3        = (1 << 19), // 0x80000
	GX_DIRTY_TEX4        = (1 << 20), // 0x100000
	GX_DIRTY_TEX5        = (1 << 21), // 0x200000
	GX_DIRTY_TEX6        = (1 << 22), // 0x400000
	GX_DIRTY_TEX7        = (1 << 23), // 0x800000
	GX_DIRTY_NUM_COLORS  = (1 << 24), // 0x1000000
	GX_DIRTY_NUM_TEX     = (1 << 25), // 0x2000000
	GX_DIRTY_MTX_IDX     = (1 << 26), // 0x4000000
	GX_DIRTY_PROJECTION  = (1 << 27), // 0x8000000
	GX_DIRTY_VIEWPORT    = (1 << 28), // 0x10000000

	GX_AMB_MAT_MASK = GX_DIRTY_AMB_COLOR0 | GX_DIRTY_AMB_COLOR1 | GX_DIRTY_MAT_COLOR0 | GX_DIRTY_MAT_COLOR1, // 0xF00

	GX_LIGHT_CHAN_MASK
	= GX_DIRTY_CHAN_COLOR0 | GX_DIRTY_CHAN_COLOR1 | GX_DIRTY_CHAN_ALPHA0 | GX_DIRTY_CHAN_ALPHA1 | GX_DIRTY_NUM_COLORS, // 0x100F000

	GX_TEX_GEN_MASK = 0x2FF0000, // all GX_DIRTY_TEXs | GX_DIRTY_NUM_TEX
} GXDirtyFlag;

// BP locators for setup size.
typedef enum _GXBPSUSSize {
	// Use line offsets [13-13]
	GX_BP_SU_SSIZE_USELINEOFS_ST  = 13,
	GX_BP_SU_SSIZE_USELINEOFS_END = 13,

	// Use point offsets [12-12]
	GX_BP_SU_SSIZE_USEPTOFS_ST  = 12,
	GX_BP_SU_SSIZE_USEPTOFS_END = 12,
} GXBPSUSSize;

// BP locators for Z mode.
typedef enum _GXBPZMode {
	// Test enable [31-31]
	GX_BP_ZMODE_TEST_ENABLE_ST  = 31,
	GX_BP_ZMODE_TEST_ENABLE_END = 31,

	// Compare [28-30]
	GX_BP_ZMODE_COMPARE_ST  = 28,
	GX_BP_ZMODE_COMPARE_END = 30,

	// Update enable [27-27]
	GX_BP_ZMODE_UPDATE_ENABLE_ST  = 27,
	GX_BP_ZMODE_UPDATE_ENABLE_END = 27,
} GXBPZMode;

// BP locators for blend mode.
typedef enum _GXBPBlendMode {
	// Blend enable [31-31]
	GX_BP_BLENDMODE_ENABLE_ST  = 31,
	GX_BP_BLENDMODE_ENABLE_END = 31,

	// Logic operation enable [30-30]
	GX_BP_BLENDMODE_LOGIC_OP_ST  = 30,
	GX_BP_BLENDMODE_LOGIC_OP_END = 30,

	// Dither [29-29]
	GX_BP_BLENDMODE_DITHER_ST  = 29,
	GX_BP_BLENDMODE_DITHER_END = 29,

	// Color update [28-28]
	GX_BP_BLENDMODE_COLOR_UPDATE_ST  = 28,
	GX_BP_BLENDMODE_COLOR_UPDATE_END = 28,

	// Alpha update [27-27]
	GX_BP_BLENDMODE_ALPHA_UPDATE_ST  = 27,
	GX_BP_BLENDMODE_ALPHA_UPDATE_END = 27,

	// Destination factor [24-26]
	GX_BP_BLENDMODE_DSTFACTOR_ST  = 24,
	GX_BP_BLENDMODE_DSTFACTOR_END = 26,

	// Source factor [21-23]
	GX_BP_BLENDMODE_SRCFACTOR_ST  = 21,
	GX_BP_BLENDMODE_SRCFACTOR_END = 23,

	// Subtract [20-20]
	GX_BP_BLENDMODE_SUBTRACT_ST  = 20,
	GX_BP_BLENDMODE_SUBTRACT_END = 20,

	// Logic mode [16-19]
	GX_BP_BLENDMODE_LOGICMODE_ST  = 16,
	GX_BP_BLENDMODE_LOGICMODE_END = 19,
} GXBPBlendMode;

// BP locators for destination alpha.
typedef enum _GXBPDstAlpha {
	// Alpha [24-31]
	GX_BP_DSTALPHA_ALPHA_ST  = 24,
	GX_BP_DSTALPHA_ALPHA_END = 31,

	// Enable [23-23]
	GX_BP_DSTALPHA_ENABLE_ST  = 23,
	GX_BP_DSTALPHA_ENABLE_END = 23,

	// YUV format [21-22]
	GX_BP_DSTALPHA_YUV_FMT_ST  = 21,
	GX_BP_DSTALPHA_YUV_FMT_END = 22,
} GXBPDstAlpha;

// Texture register fields for XF (transform) unit.
typedef enum _GXXfTexReg {
	GX_XF_TEX_PROJ_ST  = 0, // (s,t) (2x4)
	GX_XF_TEX_PROJ_STQ = 1, // (s,t,q) (3x4)

	GX_XF_TEX_FORM_AB11 = 0, // (A, B, 1.0f, 1.0f), used for regular tex src
	GX_XF_TEX_FORM_ABC1 = 1, // (A, B, C, 1.0f), used for geometry/normal src
} GXXfTexReg;

// General texture commands.
typedef enum _GXXfTexGen {
	GX_XF_TG_REGULAR = 0, // Regular; transform incoming data.
	GX_XF_TG_BUMP    = 1, // Texgen bump mapping.
	GX_XF_TG_CLR0    = 2, // Color texgen for color 0 (s,t) = (r, g:b)
	GX_XF_TG_CLR1    = 3, // Color texgen for color 1 (s,t) = (r, g:b)
} GXXfTexGen;

// Transform memory types.
typedef enum _GXXfMem {
	GX_XF_MEM_POSMTX     = 0x000, // position coord matrix
	GX_XF_MEM_NRMMTX     = 0x400, // normal coord matrix
	GX_XF_MEM_DUALTEXMTX = 0x500, // dual texture matrix
	GX_XF_MEM_LIGHTOBJ   = 0x600, // light object
} GXXfMem;

// Blitting processor registers.
typedef enum _GXBPRegs {
	// gen mode
	GX_BP_REG_GENMODE = 0x0, // gen mode

	// display copy filters
	GX_BP_REG_DISPCOPYFILTER0 = 0x1, // display copy filter 0
	GX_BP_REG_DISPCOPYFILTER1 = 0x2, // display copy filter 1
	GX_BP_REG_DISPCOPYFILTER2 = 0x3, // display copy filter 2
	GX_BP_REG_DISPCOPYFILTER3 = 0x4, // display copy filter 3

	// indirect matrices
	GX_BP_REG_INDMTX0A = 0x6, // indirect matrix 0A
	GX_BP_REG_INDMTX0B = 0x7, // indirect matrix 0B
	GX_BP_REG_INDMTX0C = 0x8, // indirect matrix 0C
	GX_BP_REG_INDMTX1A = 0x9, // indirect matrix 1A
	GX_BP_REG_INDMTX1B = 0xA, // indirect matrix 1B
	GX_BP_REG_INDMTX1C = 0xB, // indirect matrix 1C
	GX_BP_REG_INDMTX2A = 0xC, // indirect matrix 2A
	GX_BP_REG_INDMTX2B = 0xD, // indirect matrix 2B
	GX_BP_REG_INDMTX2C = 0xE, // indirect matrix 2C
	GX_BP_REG_INDIMASK = 0xF, // indirect mask

	// indirect TEV stages
	GX_BP_REG_INDTEVSTAGE0  = 0x10, // indirect TEV stage 0
	GX_BP_REG_INDTEVSTAGE1  = 0x11, // indirect TEV stage 1
	GX_BP_REG_INDTEVSTAGE2  = 0x12, // indirect TEV stage 2
	GX_BP_REG_INDTEVSTAGE3  = 0x13, // indirect TEV stage 3
	GX_BP_REG_INDTEVSTAGE4  = 0x14, // indirect TEV stage 4
	GX_BP_REG_INDTEVSTAGE5  = 0x15, // indirect TEV stage 5
	GX_BP_REG_INDTEVSTAGE6  = 0x16, // indirect TEV stage 6
	GX_BP_REG_INDTEVSTAGE7  = 0x17, // indirect TEV stage 7
	GX_BP_REG_INDTEVSTAGE8  = 0x18, // indirect TEV stage 8
	GX_BP_REG_INDTEVSTAGE9  = 0x19, // indirect TEV stage 9
	GX_BP_REG_INDTEVSTAGE10 = 0x1A, // indirect TEV stage 10
	GX_BP_REG_INDTEVSTAGE11 = 0x1B, // indirect TEV stage 11
	GX_BP_REG_INDTEVSTAGE12 = 0x1C, // indirect TEV stage 12
	GX_BP_REG_INDTEVSTAGE13 = 0x1D, // indirect TEV stage 13
	GX_BP_REG_INDTEVSTAGE14 = 0x1E, // indirect TEV stage 14
	GX_BP_REG_INDTEVSTAGE15 = 0x1F, // indirect TEV stage 15

	// performance manips
	GX_BP_REG_SCISSORTL   = 0x20, // scissor top left
	GX_BP_REG_SCISSORBR   = 0x21, // scissor bottom right
	GX_BP_REG_LINEPTWIDTH = 0x22, // line point width
	GX_BP_REG_PERF0TRI    = 0x23, // performance 0 (triangle)
	GX_BP_REG_PERF0QUAD   = 0x24, // performance 0 (quad)

	// rasters
	GX_BP_REG_RAS1_SS0   = 0x25,
	GX_BP_REG_RAS1_SS1   = 0x26,
	GX_BP_REG_RAS1_IREF  = 0x27,
	GX_BP_REG_RAS1_TREF0 = 0x28,
	GX_BP_REG_RAS1_TREF1 = 0x29,
	GX_BP_REG_RAS1_TREF2 = 0x2A,
	GX_BP_REG_RAS1_TREF3 = 0x2B,
	GX_BP_REG_RAS1_TREF4 = 0x2C,
	GX_BP_REG_RAS1_TREF5 = 0x2D,
	GX_BP_REG_RAS1_TREF6 = 0x2E,
	GX_BP_REG_RAS1_TREF7 = 0x2F,

	// setup sizes
	GX_BP_REG_SU_SSIZE0 = 0x30,
	GX_BP_REG_SU_TSIZE0 = 0x31,
	GX_BP_REG_SU_SSIZE1 = 0x32,
	GX_BP_REG_SU_TSIZE1 = 0x33,
	GX_BP_REG_SU_SSIZE2 = 0x34,
	GX_BP_REG_SU_TSIZE2 = 0x35,
	GX_BP_REG_SU_SSIZE3 = 0x36,
	GX_BP_REG_SU_TSIZE3 = 0x37,
	GX_BP_REG_SU_SSIZE4 = 0x38,
	GX_BP_REG_SU_TSIZE4 = 0x39,
	GX_BP_REG_SU_SSIZE5 = 0x3A,
	GX_BP_REG_SU_TSIZE5 = 0x3B,
	GX_BP_REG_SU_SSIZE6 = 0x3C,
	GX_BP_REG_SU_TSIZE6 = 0x3D,
	GX_BP_REG_SU_SSIZE7 = 0x3E,
	GX_BP_REG_SU_TSIZE7 = 0x3F,

	// Z and blend controls
	GX_BP_REG_ZMODE      = 0x40,
	GX_BP_REG_BLENDMODE  = 0x41,
	GX_BP_REG_DSTALPHA   = 0x42,
	GX_BP_REG_ZCONTROL   = 0x43,
	GX_BP_REG_FIELDMASK  = 0x44,
	GX_BP_REG_DRAWDONE   = 0x45,
	GX_BP_REG_PETOKEN    = 0x47,
	GX_BP_REG_PETOKENINT = 0x48,

	// copying
	GX_BP_REG_TEXCOPYSRCXY   = 0x49,
	GX_BP_REG_TEXCOPYSRCWH   = 0x4A,
	GX_BP_REG_TEXCOPYDST     = 0x4B,
	GX_BP_REG_DISPCOPYSTRIDE = 0x4D,
	GX_BP_REG_DISPCOPYSCALEY = 0x4E,
	GX_BP_REG_COPYCLEARAR    = 0x4F,
	GX_BP_REG_COPYCLEARGB    = 0x50,
	GX_BP_REG_COPYCLEARZ     = 0x51,
	GX_BP_REG_COPYFILTER0    = 0x53,
	GX_BP_REG_COPYFILTER1    = 0x54,

	//
	GX_BP_REG_BOUNDINGBOX0 = 0x55,
	GX_BP_REG_BOUNDINGBOX1 = 0x56,

	GX_BP_REG_SCISSOROFFSET = 0x59,

	// texture memory
	GX_BP_REG_TMEMPRELOADADDR   = 0x60,
	GX_BP_REG_TMEMPRELOADEVEN   = 0x61,
	GX_BP_REG_TMEMPRELOADODD    = 0x62,
	GX_BP_REG_TMEMPRELOADMODE   = 0x63,
	GX_BP_REG_TMEMTLUTSRC       = 0x64,
	GX_BP_REG_TMEMTLUTDST       = 0x65,
	GX_BP_REG_TMEMTEXINVALIDATE = 0x66,

	// performance 1
	GX_BP_REG_PERF1     = 0x67,
	GX_BP_REG_FIELDMODE = 0x68,

	// set modes
	GX_BP_REG_SETMODE0_TEX0 = 0x80,
	GX_BP_REG_SETMODE0_TEX1 = 0x81,
	GX_BP_REG_SETMODE0_TEX2 = 0x82,
	GX_BP_REG_SETMODE0_TEX3 = 0x83,
	GX_BP_REG_SETMODE1_TEX0 = 0x84,
	GX_BP_REG_SETMODE1_TEX1 = 0x85,
	GX_BP_REG_SETMODE1_TEX2 = 0x86,
	GX_BP_REG_SETMODE1_TEX3 = 0x87,

	// set images
	GX_BP_REG_SETIMAGE0_TEX0 = 0x88,
	GX_BP_REG_SETIMAGE0_TEX1 = 0x89,
	GX_BP_REG_SETIMAGE0_TEX2 = 0x8A,
	GX_BP_REG_SETIMAGE0_TEX3 = 0x8B,
	GX_BP_REG_SETIMAGE1_TEX0 = 0x8C,
	GX_BP_REG_SETIMAGE1_TEX1 = 0x8D,
	GX_BP_REG_SETIMAGE1_TEX2 = 0x8E,
	GX_BP_REG_SETIMAGE1_TEX3 = 0x8F,
	GX_BP_REG_SETIMAGE2_TEX0 = 0x90,
	GX_BP_REG_SETIMAGE2_TEX1 = 0x91,
	GX_BP_REG_SETIMAGE2_TEX2 = 0x92,
	GX_BP_REG_SETIMAGE2_TEX3 = 0x93,
	GX_BP_REG_SETIMAGE3_TEX0 = 0x94,
	GX_BP_REG_SETIMAGE3_TEX1 = 0x95,
	GX_BP_REG_SETIMAGE3_TEX2 = 0x96,
	GX_BP_REG_SETIMAGE3_TEX3 = 0x97,

	// set texture lookups
	GX_BP_REG_SETTLUT_TEX0 = 0x98,
	GX_BP_REG_SETTLUT_TEX1 = 0x99,
	GX_BP_REG_SETTLUT_TEX2 = 0x9A,
	GX_BP_REG_SETTLUT_TEX3 = 0x9B,

	// set modes continued
	GX_BP_REG_SETMODE0_TEX4 = 0xA0,
	GX_BP_REG_SETMODE0_TEX5 = 0xA1,
	GX_BP_REG_SETMODE0_TEX6 = 0xA2,
	GX_BP_REG_SETMODE0_TEX7 = 0xA3,
	GX_BP_REG_SETMODE1_TEX4 = 0xA4,
	GX_BP_REG_SETMODE1_TEX5 = 0xA5,
	GX_BP_REG_SETMODE1_TEX6 = 0xA6,
	GX_BP_REG_SETMODE1_TEX7 = 0xA7,

	// set images continued
	GX_BP_REG_SETIMAGE0_TEX4 = 0xA8,
	GX_BP_REG_SETIMAGE0_TEX5 = 0xA9,
	GX_BP_REG_SETIMAGE0_TEX6 = 0xAA,
	GX_BP_REG_SETIMAGE0_TEX7 = 0xAB,
	GX_BP_REG_SETIMAGE1_TEX4 = 0xAC,
	GX_BP_REG_SETIMAGE1_TEX5 = 0xAD,
	GX_BP_REG_SETIMAGE1_TEX6 = 0xAE,
	GX_BP_REG_SETIMAGE1_TEX7 = 0xAF,
	GX_BP_REG_SETIMAGE2_TEX4 = 0xB0,
	GX_BP_REG_SETIMAGE2_TEX5 = 0xB1,
	GX_BP_REG_SETIMAGE2_TEX6 = 0xB2,
	GX_BP_REG_SETIMAGE2_TEX7 = 0xB3,
	GX_BP_REG_SETIMAGE3_TEX4 = 0xB4,
	GX_BP_REG_SETIMAGE3_TEX5 = 0xB5,
	GX_BP_REG_SETIMAGE3_TEX6 = 0xB6,
	GX_BP_REG_SETIMAGE3_TEX7 = 0xB7,

	// set texture lookups continued
	GX_BP_REG_SETTLUT_TEX4 = 0xB8,
	GX_BP_REG_SETTLUT_TEX5 = 0xB9,
	GX_BP_REG_SETTLUT_TEX6 = 0xBA,
	GX_BP_REG_SETTLUT_TEX7 = 0xBB,

	// TEV color manips
	GX_BP_REG_TEVCOLORCOMBINER0  = 0xC0,
	GX_BP_REG_TEVALPHACOMBINER0  = 0xC1,
	GX_BP_REG_TEVCOLORCOMBINER1  = 0xC2,
	GX_BP_REG_TEVALPHACOMBINER1  = 0xC3,
	GX_BP_REG_TEVCOLORCOMBINER2  = 0xC4,
	GX_BP_REG_TEVALPHACOMBINER2  = 0xC5,
	GX_BP_REG_TEVCOLORCOMBINER3  = 0xC6,
	GX_BP_REG_TEVALPHACOMBINER3  = 0xC7,
	GX_BP_REG_TEVCOLORCOMBINER4  = 0xC8,
	GX_BP_REG_TEVALPHACOMBINER4  = 0xC9,
	GX_BP_REG_TEVCOLORCOMBINER5  = 0xCA,
	GX_BP_REG_TEVALPHACOMBINER5  = 0xCB,
	GX_BP_REG_TEVCOLORCOMBINER6  = 0xCC,
	GX_BP_REG_TEVALPHACOMBINER6  = 0xCD,
	GX_BP_REG_TEVCOLORCOMBINER7  = 0xCE,
	GX_BP_REG_TEVALPHACOMBINER7  = 0xCF,
	GX_BP_REG_TEVCOLORCOMBINER8  = 0xD0,
	GX_BP_REG_TEVALPHACOMBINER8  = 0xD1,
	GX_BP_REG_TEVCOLORCOMBINER9  = 0xD2,
	GX_BP_REG_TEVALPHACOMBINER9  = 0xD3,
	GX_BP_REG_TEVCOLORCOMBINER10 = 0xD4,
	GX_BP_REG_TEVALPHACOMBINER10 = 0xD5,
	GX_BP_REG_TEVCOLORCOMBINER11 = 0xD6,
	GX_BP_REG_TEVALPHACOMBINER11 = 0xD7,
	GX_BP_REG_TEVCOLORCOMBINER12 = 0xD8,
	GX_BP_REG_TEVALPHACOMBINER12 = 0xD9,
	GX_BP_REG_TEVCOLORCOMBINER13 = 0xDA,
	GX_BP_REG_TEVALPHACOMBINER13 = 0xDB,
	GX_BP_REG_TEVCOLORCOMBINER14 = 0xDC,
	GX_BP_REG_TEVALPHACOMBINER14 = 0xDD,
	GX_BP_REG_TEVCOLORCOMBINER15 = 0xDE,
	GX_BP_REG_TEVALPHACOMBINER15 = 0xDF,

	// TEV registers
	GX_BP_REG_TEVREG0LO = 0xE0,
	GX_BP_REG_TEVREG0HI = 0xE1,
	GX_BP_REG_TEVREG1LO = 0xE2,
	GX_BP_REG_TEVREG1HI = 0xE3,
	GX_BP_REG_TEVREG2LO = 0xE4,
	GX_BP_REG_TEVREG2HI = 0xE5,
	GX_BP_REG_TEVREG3LO = 0xE6,
	GX_BP_REG_TEVREG3HI = 0xE7,

	// fog registers
	GX_BP_REG_FOGRANGE   = 0xE8,
	GX_BP_REG_FOGRANGEK0 = 0xE9,
	GX_BP_REG_FOGRANGEK1 = 0xEA,
	GX_BP_REG_FOGRANGEK2 = 0xEB,
	GX_BP_REG_FOGRANGEK3 = 0xEC,
	GX_BP_REG_FOGRANGEK4 = 0xED,
	GX_BP_REG_FOGPARAM0  = 0xEE,
	GX_BP_REG_FOGPARAM1  = 0xEF,
	GX_BP_REG_FOGPARAM2  = 0xF0,
	GX_BP_REG_FOGPARAM3  = 0xF1,
	GX_BP_REG_FOGCOLOR   = 0xF2,

	// performance manip registers
	GX_BP_REG_ALPHACOMPARE = 0xF3,
	GX_BP_REG_ZTEXTURE0    = 0xF4,
	GX_BP_REG_ZTEXTURE1    = 0xF5,

	// TEV K selectors
	GX_BP_REG_TEVKSEL0 = 0xF6,
	GX_BP_REG_TEVKSEL1 = 0xF7,
	GX_BP_REG_TEVKSEL2 = 0xF8,
	GX_BP_REG_TEVKSEL3 = 0xF9,
	GX_BP_REG_TEVKSEL4 = 0xFA,
	GX_BP_REG_TEVKSEL5 = 0xFB,
	GX_BP_REG_TEVKSEL6 = 0xFC,
	GX_BP_REG_TEVKSEL7 = 0xFD,

	// SS mask
	GX_BP_REG_SSMASK = 0xFE,
} GXBPRegs;


typedef enum _GXGamma {
	GX_GM_1_0 = 0,
	GX_GM_1_7 = 1,
	GX_GM_2_2 = 2,
} GXGamma;

// BP locators for Z control.
typedef enum _GXBPZControl {
	// Pixel format [29-31]
	GX_BP_ZCONTROL_PIXEL_FMT_ST  = 29,
	GX_BP_ZCONTROL_PIXEL_FMT_END = 31,

	// Z format [26-28]
	GX_BP_ZCONTROL_Z_FMT_ST  = 26,
	GX_BP_ZCONTROL_Z_FMT_END = 28,

	// Whether to do Z-buffering before or after texturing [25-25]
	GX_BP_ZCONTROL_BEFORE_TEX_ST  = 25,
	GX_BP_ZCONTROL_BEFORE_TEX_END = 25,
} GXBPZControl;

// Frame buffer pixel formats.
typedef enum _GXPixelFmt {
	GX_PF_RGB8_Z24   = 0, // Non-antialiased (RGB 888).
	GX_PF_RGBA6_Z24  = 1, // Non-antialiased (RGBA 6666).
	GX_PF_RGB565_Z16 = 2, // Anti-aliasing.
	GX_PF_Z24        = 3,
	GX_PF_Y8         = 4,
	GX_PF_U8         = 5,
	GX_PF_V8         = 6,
	GX_PF_YUV420     = 7,
	GX_MAX_PIXELFMT, // 8
} GXPixelFmt;

// Compressed Z format.
typedef enum _GXZFmt16 {
	GX_ZC_LINEAR = 0, // 16-bit linear.
	GX_ZC_NEAR   = 1, // Compressed format (14e2) for smaller far/near ratio.
	GX_ZC_MID    = 2, // Compressed format (13e3) for medium far/near ratio.
	GX_ZC_FAR    = 3, // Compressed format (12e4) for large far/near ratio.
} GXZFmt16;

// Texture Coordinate IDs
typedef enum _GXTexCoordID {
	GX_TEXCOORD0 = 0, // Generated tex coord 0.
	GX_TEXCOORD1 = 1, // Generated tex coord 1.
	GX_TEXCOORD2 = 2, // Generated tex coord 2.
	GX_TEXCOORD3 = 3, // Generated tex coord 3.
	GX_TEXCOORD4 = 4, // Generated tex coord 4.
	GX_TEXCOORD5 = 5, // Generated tex coord 5.
	GX_TEXCOORD6 = 6, // Generated tex coord 6.
	GX_TEXCOORD7 = 7, // Generated tex coord 7.

	GX_MAX_TEXCOORD, // Max num texture coords (8).

	GX_TEXCOORD_NULL = 0xFF, // Null coordinate.
} GXTexCoordID;

// Texture map names.
typedef enum _GXTexMapID {
	GX_TEXMAP0 = 0, // Texture map ID 0.
	GX_TEXMAP1 = 1, // Texture map ID 1.
	GX_TEXMAP2 = 2, // Texture map ID 2.
	GX_TEXMAP3 = 3, // Texture map ID 3.
	GX_TEXMAP4 = 4, // Texture map ID 4.
	GX_TEXMAP5 = 5, // Texture map ID 5.
	GX_TEXMAP6 = 6, // Texture map ID 6.
	GX_TEXMAP7 = 7, // Texture map ID 7.

	GX_MAX_TEXMAP, // Max num texture map IDs (8).

	GX_TEXMAP_NULL = 0xFF,  // No textures used.
	GX_TEX_DISABLE = 0x100, // No texture map look-up.
} GXTexMapID;

// Color index formats.
typedef enum _GXCITexFmt {
	GX_TF_C4    = 0x8,
	GX_TF_C8    = 0x9,
	GX_TF_C14X2 = 0xA,
} GXCITexFmt;

// clang-format off
typedef enum _GXTexFmt {
    // Intensities (I) and RGB/RGBA.
	GX_TF_I4     = 0x0, // 4-bit I
	GX_TF_I8     = 0x1, // 8-bit I
	GX_TF_IA4    = 0x2, // 8-bit I + alpha (4+4).
	GX_TF_IA8    = 0x3, // 16-bit I + alpha (8+8).
	GX_TF_RGB565 = 0x4, // 16-bit RGB.
	GX_TF_RGB5A3 = 0x5, // MSB=1, RGB555 (opaque). MSB=0, RGBA4443 (transparent).
	GX_TF_RGBA8  = 0x6, // 32-bit RGB.
	GX_TF_CMPR   = 0xE, // Compressed 4-bit texel.

    // Z-texture format.
	GX_TF_Z8    = 0x11, // Unsigned 8-bit Z. For texture copies, specify the upper 8 bits of Z.
	GX_TF_Z16   = 0x13, // Unsigned 16-bit Z. For texture copies, specify the upper 16 bits of Z.
	GX_TF_Z24X8 = 0x16, // Unsigned 24-bit (32-bit texture) Z. For texture copies, copy the 24-bit Z and 0xff.

    // Copy-texture format.
	GX_CTF_R4    = 0x20, // 4-bit red. For copying 4 bits from red.
	GX_CTF_RA4   = 0x22, // 4-bit red + 4-bit alpha. For copying 4 bits from red, 4 bits from alpha.
	GX_CTF_RA8   = 0x23, // 8-bit red + 8-bit alpha. For copying 8 bits from red, 8 bits from alpha.
    GX_CTF_YUVA8 = 0x26, // 8-bit YUV + alpha. For copying 8 bits from YUV, 8 bits from alpha.
	GX_CTF_A8    = 0x26, // 8-bit alpha. For copying 8 bits from alpha.
	GX_CTF_R8    = 0x27, // 8-bit red. For copying 8 bits from red.
	GX_CTF_G8    = 0x28, // 8-bit green. For copying 8 bits from green.
	GX_CTF_B8    = 0x29, // 8-bit blue. For copying 8 bits from blue.
	GX_CTF_RG8   = 0x2A, // 8-bit red +8-bit green. For copying 8 bits from red, 8 bits from green.
	GX_CTF_GB8   = 0x2B, // 8-bit green +8-bit blue. For copying 8 bits from green, 8 bits from blue.

    // Copy-Z-texture format.
	GX_CTF_Z4   = 0x30, // 4-bit Z. For copying the 4 upper bits from Z.
	GX_CTF_Z8M  = 0x39, // 8-bit Z (median byte). For copying the middle 8 bits of Z.
	GX_CTF_Z8L  = 0x3A, // 8-bit Z (lower byte). For copying the lower 8 bits of Z.
	GX_CTF_Z16L = 0x3C, // 16-bit Z (lower portion). For copying the lower 16 bits of Z.
} GXTexFmt;
// clang-format on

typedef enum {
GX_NOP = 0x00,
GX_DRAW_QUADS = 0x80,
GX_DRAW_TRIANGLES = 0x90,
GX_DRAW_TRIANGLE_STRIP = 0x98,
GX_DRAW_TRIANGLE_FAN = 0xA0,
GX_DRAW_LINES = 0xA8,
GX_DRAW_LINE_STRIP = 0xB0,
GX_DRAW_POINTS = 0xB8,
GX_LOAD_BP_REG = 0x61,
GX_LOAD_CP_REG = 0x08,
GX_LOAD_XF_REG = 0x10,
GX_LOAD_INDX_A = 0x20,
GX_LOAD_INDX_B = 0x28,
GX_LOAD_INDX_C = 0x30,
GX_LOAD_INDX_D = 0x38,
GX_CMD_CALL_DL = 0x40,
GX_CMD_INVL_VC = 0x48,
GX_OPCODE_MASK = 0xF8,
GX_VAT_MASK = 0x07,
} GXCommand;

// Generic struct for texture objects.
typedef struct _GXTexObj {
	u32 pad[8]; //dummy
} GXTexObj;       // size 0x20

// Internal struct for texture objects.
typedef struct _GXTexObjPriv {
	u32 mode0;       // _00
	u32 mode1;       // _04
	u32 image0;      // _08
	u32 image3;      // _0C
	void* userData;  // _10
	GXTexFmt format; // _14
	u32 tlutName;    // _18
	u16 loadCount;   // _1C
	u8 loadFormat;   // _1E
	u8 flags;        // _1F
} GXTexObjPriv;

typedef enum _GXTlut {
	GX_TLUT0  = 0, // TLUT (256 16-bit entries) ID 0.
	GX_TLUT1  = 1,
	GX_TLUT2  = 2,
	GX_TLUT3  = 3,
	GX_TLUT4  = 4,
	GX_TLUT5  = 5,
	GX_TLUT6  = 6,
	GX_TLUT7  = 7,
	GX_TLUT8  = 8,
	GX_TLUT9  = 9,
	GX_TLUT10 = 10,
	GX_TLUT11 = 11,
	GX_TLUT12 = 12,
	GX_TLUT13 = 13,
	GX_TLUT14 = 14,
	GX_TLUT15 = 15,

	GX_MAX_TLUT = 16,

	GX_BIGTLUT0 = 16, // BIGTLUT (1024 16-bit entries) ID 0.
	GX_BIGTLUT1 = 17,
	GX_BIGTLUT2 = 18,
	GX_BIGTLUT3 = 19,

	GX_MAX_BIGTLUT = 4,

	GX_MAX_TLUT_ALL = GX_MAX_TLUT + GX_MAX_BIGTLUT, // 20
} GXTlut;

// Generic struct for texture memory storage.
typedef struct _GXTexRegion {
	u8 padding[0x10]; // _00
} GXTexRegion;

typedef struct _GXTexRegionPriv {
	u32 unk0;      // _00
	u32 unk4;      // _04
	u32 unk8;      // _08
	u8 unkC;       // _0C
	u8 unkD;       // _0D
	u8 padding[2]; // _0E
} GXTexRegionPriv;

// Generic struct for texture look-up table objects.
typedef struct _GXTlutObj {
	u8 padding[0xc]; // _00
} GXTlutObj;

typedef struct _GXTlutObjPriv {
	u32 unk0;        // _00
	u32 unk4;        // _04
	u16 numEntries;  // _08
	u8 padding[0x2]; // _0A
} GXTlutObjPriv;

// Generic struct for texture look-up table memory storage.
typedef struct _GXTlutRegion {
	u8 padding[0x10]; // _00
} GXTlutRegion;

typedef struct _GXTlutRegionPriv {
	u32 unk0;              // _00
	GXTlutObjPriv tlutObj; // _04
} GXTlutRegionPriv;

// Generic struct for light information.
typedef struct _GXLightObj {
	s32 padding[16]; // _00
} GXLightObj;         // size 0x40

// Generic 8-bit-component colors.
typedef struct _GXColor {
	u8 r, g, b, a; // _00, _01, _02, _03
} GXColor;

typedef struct __GXLightObjPriv {
	u32 reserved[3]; // _00
	GXColor color;   // _0C, light color
	f32 a[3];        // _10, angle-attenuation coefficients
	f32 k[3];        // _1C, distance-attenuation coefficients
	f32 lpos[3];     // _28, diffuse: position;  specular: direction
	f32 ldir[3];     // _34, diffuse: direction; specular: half-angle
} GXLightObjPriv;

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

typedef enum
{
    GX_CA_APREV,
    GX_CA_A0,
    GX_CA_A1,
    GX_CA_A2,
    GX_CA_TEXA,
    GX_CA_RASA,
    GX_CA_KONST,
    GX_CA_ZERO
} GXTevAlphaArg;

typedef struct
{
    GXTevColorArg a;
    GXTevColorArg b;
    GXTevColorArg c;
    GXTevColorArg d;
} GXTevColorInputCache;

typedef enum _GXTevStageID
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
} GXTevStageID;

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

enum _GXTexWrapMode
{
    GX_MAX_TEXWRAPMODE = 3,
    GX_MIRROR = 2,
    GX_REPEAT = 1,
    GX_CLAMP = 0
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

typedef enum _GXAttrType {
	GX_NONE    = 0, // No data to be sent.
	GX_DIRECT  = 1, // Value sent is value of data.
	GX_INDEX8  = 2, // Value sent is INDEX value of data array (8-bit).
	GX_INDEX16 = 3, // Value sent is INDEX value of data array (16-bit).
} GXAttrType;

// Perf-0 types.
typedef enum _GXPerf0 {
	GX_PERF0_VERTICES      = 0,
	GX_PERF0_CLIP_VTX      = 1,
	GX_PERF0_CLIP_CLKS     = 2,
	GX_PERF0_XF_WAIT_IN    = 3,
	GX_PERF0_XF_WAIT_OUT   = 4,
	GX_PERF0_XF_XFRM_CLKS  = 5,
	GX_PERF0_XF_LIT_CLKS   = 6,
	GX_PERF0_XF_BOT_CLKS   = 7,
	GX_PERF0_XF_REGLD_CLKS = 8,
	GX_PERF0_XF_REGRD_CLKS = 9,
	GX_PERF0_CLIP_RATIO    = 10,

	GX_PERF0_TRIANGLES           = 11,
	GX_PERF0_TRIANGLES_CULLED    = 12,
	GX_PERF0_TRIANGLES_PASSED    = 13,
	GX_PERF0_TRIANGLES_SCISSORED = 14,
	GX_PERF0_TRIANGLES_0TEX      = 15,
	GX_PERF0_TRIANGLES_1TEX      = 16,
	GX_PERF0_TRIANGLES_2TEX      = 17,
	GX_PERF0_TRIANGLES_3TEX      = 18,
	GX_PERF0_TRIANGLES_4TEX      = 19,
	GX_PERF0_TRIANGLES_5TEX      = 20,
	GX_PERF0_TRIANGLES_6TEX      = 21,
	GX_PERF0_TRIANGLES_7TEX      = 22,
	GX_PERF0_TRIANGLES_8TEX      = 23,
	GX_PERF0_TRIANGLES_0CLR      = 24,
	GX_PERF0_TRIANGLES_1CLR      = 25,
	GX_PERF0_TRIANGLES_2CLR      = 26,

	GX_PERF0_QUAD_0CVG    = 27,
	GX_PERF0_QUAD_NON0CVG = 28,
	GX_PERF0_QUAD_1CVG    = 29,
	GX_PERF0_QUAD_2CVG    = 30,
	GX_PERF0_QUAD_3CVG    = 31,
	GX_PERF0_QUAD_4CVG    = 32,
	GX_PERF0_AVG_QUAD_CNT = 33,

	GX_PERF0_CLOCKS = 34,
	GX_PERF0_NONE   = 35,
} GXPerf0;

// Perf-1 types.
typedef enum _GXPerf1 {
	GX_PERF1_TEXELS      = 0,
	GX_PERF1_TX_IDLE     = 1,
	GX_PERF1_TX_REGS     = 2,
	GX_PERF1_TX_MEMSTALL = 3,
	GX_PERF1_TC_CHECK1_2 = 4,
	GX_PERF1_TC_CHECK3_4 = 5,
	GX_PERF1_TC_CHECK5_6 = 6,
	GX_PERF1_TC_CHECK7_8 = 7,
	GX_PERF1_TC_MISS     = 8,

	GX_PERF1_VC_ELEMQ_FULL    = 9,
	GX_PERF1_VC_MISSQ_FULL    = 10,
	GX_PERF1_VC_MEMREQ_FULL   = 11,
	GX_PERF1_VC_STATUS7       = 12,
	GX_PERF1_VC_MISSREP_FULL  = 13,
	GX_PERF1_VC_STREAMBUF_LOW = 14,
	GX_PERF1_VC_ALL_STALLS    = 15,
	GX_PERF1_VERTICES         = 16,

	GX_PERF1_FIFO_REQ    = 17,
	GX_PERF1_CALL_REQ    = 18,
	GX_PERF1_VC_MISS_REQ = 19,
	GX_PERF1_CP_ALL_REQ  = 20,

	GX_PERF1_CLOCKS = 21,
	GX_PERF1_NONE   = 22,
} GXPerf1;

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

typedef GXTexRegion* (*GXTexRegionCallback)(const GXTexObj* t_obj, GXTexMapID id);
typedef GXTlutRegion* (*GXTlutRegionCallback)(u32 idx);

	// Texture lookup table regions
	GXTlutRegion TlutRegions[GX_MAX_TLUT_ALL]; // _388
	GXTexRegionCallback texRegionCallback;     // _4C8
	GXTlutRegionCallback tlutRegionCallback;   // _4CC

typedef struct _GXData {
	// Bypass and vertex info
	u16 vNumNot;   // _000, !(# flush verts to send)
	u16 bpSentNot; // _002, !(bypass reg sent last?)
	u16 vNum;      // _004, # flush verts to send
	u16 vLim;      // _006, max vert size

	// Command process (CP) regs
	u32 cpEnable; // _008
	u32 cpStatus; // _00C
	u32 cpClr;    // _010
	u32 vcdLo;    // _014
	u32 vcdHi;    // _018
	u32 vatA[8];  // _01C
	u32 vatB[8];  // _03C
	u32 vatC[8];  // _05C
	u32 lpSize;   // _07C
	u32 matIdxA;  // _080
	u32 matIdxB;  // _084

	// Index loading base/stride regs (pos, nrm, tex, light)
	u32 indexBase[4];   // _088
	u32 indexStride[4]; // _098

	// Transform and lighting regs
	u32 ambColor[2]; // _0A8
	u32 matColor[2]; // _0B0

	// Setup regs
	u32 suTs0[8]; // _0B8
	u32 suTs1[8]; // _0D8
	u32 suScis0;  // _0F8
	u32 suScis1;  // _0FC

	// Raster regs
	u32 tref[8]; // _100
	u32 iref;    // _120

	// Bump/Indirect texture regs
	u32 bpMask;       // _124
	u32 IndTexScale0; // _128
	u32 IndTexScale1; // _12C

	// Tev regs
	u32 tevc[16];   // _130
	u32 teva[16];   // _170
	u32 tevKsel[8]; // _1B0

	// Performance regs
	u32 cmode0; // _1D0
	u32 cmode1; // _1D4
	u32 zmode;  // _1D8
	u32 peCtrl; // _1DC

	// Display copy regs
	u32 cpDispSrc;    // _1E0
	u32 cpDispSize;   // _1E4
	u32 cpDispStride; // _1E8
	u32 cpDisp;       // _1EC

	// Texture copy regs
	u32 cpTexSrc;    // _1F0
	u32 cpTexSize;   // _1F4
	u32 cpTexStride; // _1F8
	u32 cpTex;       // _1FC
	GXBool cpTexZ;   // _200

	// General raster mode
	u32 genMode; // _204

	// Texture regions
	GXTexRegion TexRegions0[GX_MAX_TEXMAP]; // _208
	GXTexRegion TexRegions1[GX_MAX_TEXMAP]; // _288
	GXTexRegion TexRegions2[GX_MAX_TEXMAP]; // _308

	// Texture lookup table regions
	GXTlutRegion TlutRegions[GX_MAX_TLUT_ALL]; // _388
	GXTexRegionCallback texRegionCallback;     // _4C8
	GXTlutRegionCallback tlutRegionCallback;   // _4CC

	// Command processor vars
	GXAttrType nrmType; // _4D0
	GXBool hasNrms;     // _4D4
	GXBool hasBiNrms;   // _4D5
	u32 projType;       // _4D8
	f32 projMtx[6];     // _4DC

	// Viewport parms
	f32 vpLeft;  // _4F4
	f32 vpTop;   // _4F8
	f32 vpWd;    // _4FC
	f32 vpHt;    // _500
	f32 vpNearz; // _504
	f32 vpFarz;  // _508
	f32 zOffset; // _50C
	f32 zScale;  // _510

	// Texture regs
	u32 tImage0[8];   // _514
	u32 tMode0[8];    // _534
	u32 texmapId[16]; // _554
	u32 tcsManEnab;   // _594
	u32 tevTcEnab;    // _598

	// Performance metrics
	GXPerf0 perf0; // _59C
	GXPerf1 perf1; // _5A0
	u32 perfSel;   // _5A4

	// Flags
	GXBool inDispList;    // _5A8
	GXBool dlSaveContext; // _5A9
	GXBool abtWaitPECopy; // _5AA
	u8 dirtyVAT;          // _5AB
	u32 dirtyState;       // _5AC
} GXData;
GXData* const __GXData;
#define gx __GXData

typedef struct __GXFifoObj {
    unsigned char* base; // Offset: 0x0, DWARF: 0xE3AF8E
    unsigned char* top; // Offset: 0x4, DWARF: 0xE3AFB2
    unsigned long size; // Offset: 0x8, DWARF: 0xE3AFD5
    unsigned long hiWatermark; // Offset: 0xC, DWARF: 0xE3AFF6
    unsigned long loWatermark; // Offset: 0x10, DWARF: 0xE3B01E
    void* rdPtr; // Offset: 0x14, DWARF: 0xE3B046
    void* wrPtr; // Offset: 0x18, DWARF: 0xE3B068
    long count; // Offset: 0x1C, DWARF: 0xE3B08A
    unsigned char bind_cpu; // Offset: 0x20, DWARF: 0xE3B0AC
    unsigned char bind_gp; // Offset: 0x21, DWARF: 0xE3B0D1
} __GXFifoObj;

// Generic struct for FIFO access (size 0x80).
typedef struct _GXFifoObj {
	u8 padding[GX_FIFO_OBJ_SIZE]; // _00
} GXFifoObj;

// Size: 0x2C8, DWARF: 0xC1A3AD
typedef struct _OSContext
{
    unsigned int gpr[32]; // Offset: 0x0, DWARF: 0xC1A3CB
    unsigned int cr; // Offset: 0x80, DWARF: 0xC1A3F3
    unsigned int lr; // Offset: 0x84, DWARF: 0xC1A418
    unsigned int ctr; // Offset: 0x88, DWARF: 0xC1A43D
    unsigned int xer; // Offset: 0x8C, DWARF: 0xC1A463
    double fpr[32]; // Offset: 0x90, DWARF: 0xC1A489
    unsigned int fpscr_pad; // Offset: 0x190, DWARF: 0xC1A4B1
    unsigned int fpscr; // Offset: 0x194, DWARF: 0xC1A4DD
    unsigned int srr0; // Offset: 0x198, DWARF: 0xC1A505
    unsigned int srr1; // Offset: 0x19C, DWARF: 0xC1A52C
    unsigned short mode; // Offset: 0x1A0, DWARF: 0xC1A553
    unsigned short state; // Offset: 0x1A2, DWARF: 0xC1A57A
    unsigned int gqr[8]; // Offset: 0x1A4, DWARF: 0xC1A5A2
    double psf[32]; // Offset: 0x1C8, DWARF: 0xC1A5CA
} OSContext;

typedef struct GXContext {
        union {
            u32 u32;
            u16 u16[2];
        } x0;
        u16 x4;
        u16 x6;
        OSContext* x8;
        u32 xC;
        u32 x10;
        u32 x14;
        u32 x18;
        s32 x1C_data[(0x3C - 0x1C) / 4];
        s32 x3C_data[(0x5C - 0x3C) / 4];
        s32 x5C_data[(0x7C - 0x5C) / 4];
        s32 x7C;
        s32 x80;
        s32 x84;
        u32 x88_data[4];
        u32 x98_data[4];
        u32 ambColors[2];
        u32 matColors[2];
        s32 xB8[GX_MAX_TEXCOORD];
        u8 xD8_pad[0xF8 - 0xD8];
        s32 xF8;
        s32 xFC;
        u32 x100_data[(0x130 - 0x100) / 4];
        u32 x130_data[(0x170 - 0x130) / 4];
        u32 x170_data[(0x1AC - 0x170) / 4];
        u8 x1AC_pad[0x1B0 - 0x1AC];
        GXTexRegionCallback callbacks[(0x1D0 - 0x1B0) / 4];
        u32 x1D0[(0x204 - 0x1D0) / 4];
        u32 x204;
        u8 x208_pad[0x2D0 - 0x208];
        struct {
            u32 unk[4];
        } x2D0[(0x410 - 0x2D0) / 16];
        GXTexRegionCallback tex_region_cb;
        GXTexRegionCallback tlut_region_cb;
        u32 x418;
        GXBool x41C;
        GXBool x41D;
        u32 projection_type;
        f32 projection_v[6];
        f32 viewport_v[6];
        u8 x454_pad[4];
        f32 x458;
        s32 x45C_data[0x20 / 4];
        s32 x47C_data[0x20 / 4];
        u32 x49C_data[(0x4EC - 0x49C) / 4];
        u8 x4EC;
        GXBool x4ED;
        u8 x4EE;
        u32 x4F0_flags;
        GXFifoObj* fifo;
        u8 x4F8_pad[0x570 - 0x4F8];
        u32 dirtyFlags;
} GXContext;

typedef union {
	u8 u8;
	u16 u16;
	u32 u32;
	u64 u64;
	s8 s8;
	s16 s16;
	s32 s32;
	s64 s64;
	f32 f32;
	f64 f64;
} PPCWGPipe;

volatile PPCWGPipe WGPIPE;
volatile PPCWGPipe GXWGFifo;

typedef struct _GXContexts {
    GXContext* main;
    GXContext* null;
} GXContexts;

GXContext __GXContext;
GXContexts __GXContexts; // = {&__GXContext, NULL}

void DCFlushRange(void*, u32);     //DCFlushRange(ram_addr, len)?
void GXInitTexObj(GXTexObj * obj, void* image_ptr, u16 width, u16 height, GXTexFmt texFormat, enum _GXTexWrapMode wrapModeS, enum _GXTexWrapMode wrapModeT, char mipmap);

/////////////////////////////////////////////////////////////////////

#endif
