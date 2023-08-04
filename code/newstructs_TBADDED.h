typedef struct __GXData_struct __GXData_struct, *P__GXData_struct;

typedef struct _GXTexRegion _GXTexRegion, *P_GXTexRegion;

typedef struct GXTlutRegion GXTlutRegion, *PGXTlutRegion;

typedef enum _GXAttrType {
    GX_NONE=0,
    GX_DIRECT=1,
    GX_INDEX8=2,
    GX_INDEX16=3
} _GXAttrType;

typedef enum _GXPerf0 {
    GX_PERF0_VERTICES=0,
    GX_PERF0_CLIP_VTX=1,
    GX_PERF0_CLIP_CLKS=2,
    GX_PERF0_XF_WAIT_IN=3,
    GX_PERF0_XF_WAIT_OUT=4,
    GX_PERF0_XF_XFRM_CLKS=5,
    GX_PERF0_XF_LIT_CLKS=6,
    GX_PERF0_XF_BOT_CLKS=7,
    GX_PERF0_XF_REGLD_CLKS=8,
    GX_PERF0_XF_REGRD_CLKS=9,
    GX_PERF0_CLIP_RATIO=10,
    GX_PERF0_TRIANGLES=11,
    GX_PERF0_TRIANGLES_CULLED=12,
    GX_PERF0_TRIANGLES_PASSED=13,
    GX_PERF0_TRIANGLES_SCISSORED=14,
    GX_PERF0_TRIANGLES_0TEX=15,
    GX_PERF0_TRIANGLES_1TEX=16,
    GX_PERF0_TRIANGLES_2TEX=17,
    GX_PERF0_TRIANGLES_3TEX=18,
    GX_PERF0_TRIANGLES_4TEX=19,
    GX_PERF0_TRIANGLES_5TEX=20,
    GX_PERF0_TRIANGLES_6TEX=21,
    GX_PERF0_TRIANGLES_7TEX=22,
    GX_PERF0_TRIANGLES_8TEX=23,
    GX_PERF0_TRIANGLES_0CLR=24,
    GX_PERF0_TRIANGLES_1CLR=25,
    GX_PERF0_TRIANGLES_2CLR=26,
    GX_PERF0_QUAD_0CVG=27,
    GX_PERF0_QUAD_NON0CVG=28,
    GX_PERF0_QUAD_1CVG=29,
    GX_PERF0_QUAD_2CVG=30,
    GX_PERF0_QUAD_3CVG=31,
    GX_PERF0_QUAD_4CVG=32,
    GX_PERF0_AVG_QUAD_CNT=33,
    GX_PERF0_CLOCKS=34,
    GX_PERF0_NONE=35
} _GXPerf0;

typedef enum _GXPerf1 {
    GX_PERF1_TEXELS=0,
    GX_PERF1_TX_IDLE=1,
    GX_PERF1_TX_REGS=2,
    GX_PERF1_TX_MEMSTALL=3,
    GX_PERF1_TC_CHECK1_2=4,
    GX_PERF1_TC_CHECK3_4=5,
    GX_PERF1_TC_CHECK5_6=6,
    GX_PERF1_TC_CHECK7_8=7,
    GX_PERF1_TC_MISS=8,
    GX_PERF1_VC_ELEMQ_FULL=9,
    GX_PERF1_VC_MISSQ_FULL=10,
    GX_PERF1_VC_MEMREQ_FULL=11,
    GX_PERF1_VC_STATUS7=12,
    GX_PERF1_VC_MISSREP_FULL=13,
    GX_PERF1_VC_STREAMBUF_LOW=14,
    GX_PERF1_VC_ALL_STALLS=15,
    GX_PERF1_VERTICES=16,
    GX_PERF1_FIFO_REQ=17,
    GX_PERF1_CALL_REQ=18,
    GX_PERF1_VC_MISS_REQ=19,
    GX_PERF1_CP_ALL_REQ=20,
    GX_PERF1_CLOCKS=21,
    GX_PERF1_NONE=22
} _GXPerf1;

struct GXTlutRegion {
    uint dummy[4];
};

struct _GXTexRegion {
    uint dummy[4];
};

struct __GXData_struct {
    ushort vNumNot;
    ushort bpSentNot;
    ushort vNum;
    ushort vLim;
    ulong cpEnable;
    ulong cpStatus;
    ulong cpClr;
    ulong vcdLo;
    ulong vcdHi;
    ulong vatA[8];
    ulong vatB[8];
    ulong vatC[8];
    ulong lpSize;
    ulong matIdxA;
    ulong matIdxB;
    ulong indexBase[4];
    ulong indexStride[4];
    ulong ambColor[2];
    ulong matColor[2];
    ulong suTs0[8];
    ulong suTs1[8];
    ulong suScis0;
    ulong suScis1;
    ulong tref[8];
    ulong iref;
    ulong bpMask;
    ulong IndTexScale0;
    ulong IndTexScale1;
    ulong tevc[16];
    ulong teva[16];
    ulong tevKsel[8];
    ulong cmode0;
    ulong cmode1;
    ulong zmode;
    ulong peCtrl;
    ulong cpDispSrc;
    ulong cpDispSize;
    ulong cpDispStride;
    ulong cpDisp;
    ulong cpTexSrc;
    ulong cpTexSize;
    ulong cpTexStride;
    ulong cpTex;
    uchar cpTexZ;
    uchar unk_[3];
    ulong genMode;
    struct _GXTexRegion TexRegions[8];
    struct _GXTexRegion TexRegionsCI[4];
    ulong nextTexRgn;
    ulong nextTexRgnCI;
    struct GXTlutRegion TlutRegions[20];
    struct _GXTexRegion * texRegion_Callback; /* (*texRegionCallback)(_GXTexObj*, _GXTexMapID) */
    struct GXTlutRegion * tlut_reg;
    enum _GXAttrType nrmType;
    uchar hasNrms;
    uchar hasBiNrms;
    uchar unk[2];
    ulong projType;
    float projMtx[6];
    float vpLeft;
    float vpTop;
    float vpWd;
    float vpHt;
    float vpNearz;
    float vpFarz;
    uchar fpRange;
    uchar unk2[3];
    float fgSideX;
    ulong tImage0[8];
    ulong tMode0[8];
    ulong texmapId[16];
    ulong tcsManEnab;
    ulong tevTcEnab;
    enum _GXPerf0 perf0;
    enum _GXPerf1 perf1;
    ulong perfSel;
    uchar inDispList;
    uchar dlSaveContext;
    uchar dirtyVat;
    uchar unk3;
    ulong dirtyState;
};

typedef struct _GS_VERTEX _GS_VERTEX, *P_GS_VERTEX;

struct _GS_VERTEX {
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
    uint diffuse;
    float u;
    float v;
};

typedef struct _GS_VERTEXSKIN _GS_VERTEXSKIN, *P_GS_VERTEXSKIN;

struct _GS_VERTEXSKIN {
    float x;
    float y;
    float z;
    float weights[2];
    float indexes[3];
    float nx;
    float ny;
    float nz;
    int diffuse;
    float u;
    float v;
};

typedef struct _GS_VERTEXTL _GS_VERTEXTL, *P_GS_VERTEXTL;

struct _GS_VERTEXTL {
    float x;
    float y;
    float z;
    float rhw;
    uint diffuse;
    float u;
    float v;
};

typedef struct _GSMATRIX33 _GSMATRIX33, *P_GSMATRIX33;

struct _GSMATRIX33 {
    float _11;
    float _12;
    float _13;
    float _14;
    float _21;
    float _22;
    float _23;
    float _24;
    float _31;
    float _32;
    float _33;
    float _34;
};

typedef enum _GXAlphaOp {
    GX_AOP_AND=0,
    GX_AOP_OR=1,
    GX_AOP_XOR=2,
    GX_AOP_XNOR=3,
    GX_AOP_ALPHAOP=4
} _GXAlphaOp;

typedef enum _GXAttnFn {
    GX_AF_SPEC=0,
    GX_AF_SPOT=1,
    GX_AF_NONE=2
} _GXAttnFn;

typedef enum _GXAttr {
    GX_VA_PNMTXIDX=0,
    GX_VA_TEX0MTXIDX=1,
    GX_VA_TEX1MTXIDX=2,
    GX_VA_TEX2MTXIDX=3,
    GX_VA_TEX3MTXIDX=4,
    GX_VA_TEX4MTXIDX=5,
    GX_VA_TEX5MTXIDX=6,
    GX_VA_TEX6MTXIDX=7,
    GX_VA_TEX7MTXIDX=8,
    GX_VA_POS=9,
    GX_VA_NRM=10,
    GX_VA_CLR0=11,
    GX_VA_CLR1=12,
    GX_VA_TEX0=13,
    GX_VA_TEX1=14,
    GX_VA_TEX2=15,
    GX_VA_TEX3=16,
    GX_VA_TEX4=17,
    GX_VA_TEX5=18,
    GX_VA_TEX6=19,
    GX_VA_TEX7=20,
    GX_POS_MTX_ARRAY=21,
    GX_NRM_MTX_ARRAY=22,
    GX_TEX_MTX_ARRAY=23,
    GX_LIGHT_ARRAY=24,
    GX_VA_NBT=25,
    GX_VA_MAX_ATTR=26,
    GX_VA_NULL=255
} _GXAttr;

typedef enum _GXBlendFactor {
    GX_BL_ZERO=0,
    GX_BL_ONE=1,
    GX_BL_SRCCLR=2,
    GX_BL_INVSRCCLR=3,
    GX_BL_SRCALPHA=4,
    GX_BL_INVSRCALPHA=5,
    GX_BL_DSTALPHA=6,
    GX_BL_INVDSTALPHA=7,
    GX_BL_DSTCLR=8,
    GX_BL_INVDSTCLR=9
} _GXBlendFactor;

typedef enum _GXBlendMode {
    GX_BM_NONE=0,
    GX_BM_BLEND=1,
    GX_BM_LOGIC=2,
    GX_BM_SUBTRACT=3,
    GX_MAX_BLENDMODE=4
} _GXBlendMode;

typedef enum _GXChannelID {
    GX_COLOR0=0,
    GX_COLOR1=1,
    GX_ALPHA0=2,
    GX_ALPHA1=3,
    GX_COLOR0A0=4,
    GX_COLOR1A1=5,
    GX_COLOR_ZERO=6,
    GX_ALPHA_BUMP=7,
    GX_ALPHA_BUMPN=8,
    GX_COLOR_NULL=255
} _GXChannelID;

typedef enum _GXClipMode {
    GX_CLIP_ENABLE=0,
    GX_CLIP_DISABLE=1
} _GXClipMode;

typedef struct _GXColor _GXColor, *P_GXColor;

struct _GXColor {
    uchar r;
    uchar g;
    uchar b;
    uchar a;
};

typedef enum _GXColorSrc {
    GX_SRC_REG=0,
    GX_SRC_VTX=1
} _GXColorSrc;

typedef enum _GXCompare {
    GX_NEVER=0,
    GX_LESS=1,
    GX_EQUAL=2,
    GX_LEQUAL=3,
    GX_GREATER=4,
    GX_NEQUAL=5,
    GX_GEQUAL=6,
    GX_ALWAYS=7
} _GXCompare;

typedef enum _GXCompCnt {
    GX_CLR_RGB=0,
    GX_CLR_RGBA=0,
    GX_NRM_XYZ=0,
    GX_POS_XY=0,
    GX_POS_XYZ=0,
    GX_TEX_S=0,
    GX_NRM_NBT=1,
    GX_TEX_ST=1,
    GX_NRM_NBT3=2
} _GXCompCnt;

typedef enum _GXCompType {
    GX_RGB565=0,
    GX_U8=0,
    GX_RGB8=1,
    GX_S8=1,
    GX_RGBX8=2,
    GX_U16=2,
    GX_RGBA4=3,
    GX_S16=3,
    GX_F32=4,
    GX_RGBA6=4,
    GX_RGBA8=5
} _GXCompType;

typedef enum _GXDiffuseFn {
    GX_DF_NONE=0,
    GX_DF_SIGN=1,
    GX_DF_CLAMP=2
} _GXDiffuseFn;

typedef struct _GXLightObj _GXLightObj, *P_GXLightObj;

struct _GXLightObj {
    uint dummy[16];
};

typedef enum _GXLogicOp {
    GX_LO_CLEAR=0,
    GX_LO_AND=1,
    GX_LO_REVAND=2,
    GX_LO_COPY=3,
    GX_LO_INVAND=4,
    GX_LO_NOOP=5,
    GX_LO_XOR=6,
    GX_LO_OR=7,
    GX_LO_NOR=8,
    GX_LO_EQUIV=9,
    GX_LO_INV=10,
    GX_LO_REVOR=11,
    GX_LO_INVCOPY=12,
    GX_LO_INVOR=13,
    GX_LO_NAND=14,
    GX_LO_SET=15
} _GXLogicOp;

typedef enum _GXPrimitive {
    GX_QUADS=128,
    GX_TRIANGLES=144,
    GX_TRIANGLESTRIP=152,
    GX_TRIANGLEFAN=160,
    GX_LINES=168,
    GX_LINESTRIP=176,
    GX_POINTS=184
} _GXPrimitive;

typedef struct _GXRenderModeObj _GXRenderModeObj, *P_GXRenderModeObj;

typedef enum viTVmode {
    VI_TVMODE_NTSC_INT=0,
    VI_TVMODE_NTSC_DS=1,
    VI_TVMODE_NTSC_PROG=2,
    VI_TVMODE_PAL_INT=3,
    VI_TVMODE_PAL_DS=4,
    VI_TVMODE_EURGB60_INT=5,
    VI_TVMODE_EURGB60_DS=6,
    VI_TVMODE_MPAL_INT=7,
    VI_TVMODE_MPAL_DS=8,
    VI_TVMODE_DEBUG_INT=9,
    VI_TVMODE_DEBUG_PAL_INT=16,
    VI_TVMODE_DEBUG_PAL_DS=17
} viTVmode;

typedef enum xFBmode {
    VI_XFBMODE_SF=0,
    VI_XFBMODE_DF=1
} xFBmode;

struct _GXRenderModeObj {
    enum viTVmode viTVmode;
    ushort fbWidth;
    ushort efbHeight;
    ushort xfbHeight;
    ushort viXOrigin;
    ushort viYOrigin;
    ushort viWidth;
    ushort viHeight;
    uchar unk[2];
    enum xFBmode xFBmode;
    uchar field_rendering;
    uchar aa;
    uchar sample_pattern[2][12];
    uchar vfilter[7];
    uchar unk2[3];
};

typedef enum _GXTevAlphaArg {
    GX_CA_APREV=0,
    GX_CA_A0=1,
    GX_CA_A1=2,
    GX_CA_A2=3,
    GX_CA_TEXA=4,
    GX_CA_RASA=5,
    GX_CA_KONST=6,
    GX_CA_ONE=6,
    GX_CA_ZERO=6
} _GXTevAlphaArg;

typedef enum _GXTevBias {
    GX_TB_ZERO=0,
    GX_TB_ADDHALF=1,
    GX_TB_SUBHALF=2,
    GX_MAX_TEVBIAS=3
} _GXTevBias;

typedef enum _GXTevColorArg {
    GX_CC_CPREV=0,
    GX_CC_APREV=1,
    GX_CC_C0=2,
    GX_CC_A0=3,
    GX_CC_C1=4,
    GX_CC_A1=5,
    GX_CC_C2=6,
    GX_CC_A2=7,
    GX_CC_TEXC=8,
    GX_CC_TEXA=9,
    GX_CC_RASC=10,
    GX_CC_RASA=11,
    GX_CC_ONE=12,
    GX_CC_HALF=13,
    GX_CC_KONST=14,
    GX_CC_QUARTER=14,
    GX_CC_ZERO=15,
    GX_CC_TEXRRR=16,
    GX_CC_TEXGGG=17,
    GX_CC_TEXBBB=18
} _GXTevColorArg;

typedef enum _GXTevMode {
    GX_MODULATE=0,
    GX_DECAL=1,
    GX_BLEND=2,
    GX_REPLACE=3,
    GX_PASSCLR=4
} _GXTevMode;

typedef enum _GXTevRegID {
    GX_TEVPREV=0,
    GX_TEVREG0=1,
    GX_TEVREG1=2,
    GX_TEVREG2=3,
    GX_MAX_TEVREG=4
} _GXTevRegID;

typedef enum _GXTevStageID {
    GX_TEVSTAGE0=0,
    GX_TEVSTAGE1=1,
    GX_TEVSTAGE2=2,
    GX_TEVSTAGE3=3,
    GX_TEVSTAGE4=4,
    GX_TEVSTAGE5=5,
    GX_TEVSTAGE6=6,
    GX_TEVSTAGE7=7,
    GX_TEVSTAGE8=8,
    GX_TEVSTAGE9=9,
    GX_TEVSTAGE10=10,
    GX_TEVSTAGE11=11,
    GX_TEVSTAGE12=12,
    GX_TEVSTAGE13=13,
    GX_TEVSTAGE14=14,
    GX_TEVSTAGE15=15,
    GX_MAX_TEVSTAGE=16
} _GXTevStageID;

typedef enum _GXTevSwapSel {
    GX_TEV_SWAP0=0,
    GX_TEV_SWAP1=1,
    GX_TEV_SWAP2=2,
    GX_TEV_SWAP3=3,
    GX_MAX_TEVSWAP=4
} _GXTevSwapSel;

typedef enum _GXTexCoordID {
    GX_TEXCOORD0=0,
    GX_TEXCOORD1=1,
    GX_TEXCOORD2=2,
    GX_TEXCOORD3=3,
    GX_TEXCOORD4=4,
    GX_TEXCOORD5=5,
    GX_TEXCOORD6=6,
    GX_TEXCOORD7=7,
    GX_MAX_TEXCOORD=8,
    GX_TEXCOORD_NULL=255
} _GXTexCoordID;

typedef enum _GXTexFmt {
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
} _GXTexFmt;

typedef enum _GXTexGenSrc {
    GX_TG_POS=0,
    GX_TG_NRM=1,
    GX_TG_BINRM=2,
    GX_TG_TANGENT=3,
    GX_TG_TEX0=4,
    GX_TG_TEX1=5,
    GX_TG_TEX2=6,
    GX_TG_TEX3=7,
    GX_TG_TEX4=8,
    GX_TG_TEX5=9,
    GX_TG_TEX6=10,
    GX_TG_TEX7=11,
    GX_TG_TEXCOORD0=12,
    GX_TG_TEXCOORD1=13,
    GX_TG_TEXCOORD2=14,
    GX_TG_TEXCOORD3=15,
    GX_TG_TEXCOORD4=16,
    GX_TG_TEXCOORD5=17,
    GX_TG_TEXCOORD6=18,
    GX_TG_COLOR0=19,
    GX_TG_COLOR1=20
} _GXTexGenSrc;

typedef enum _GXTexGenType {
    GX_TG_MTX3x4=0,
    GX_TG_MTX2x4=1,
    GX_TG_BUMP0=2,
    GX_TG_BUMP1=3,
    GX_TG_BUMP2=4,
    GX_TG_BUMP3=5,
    GX_TG_BUMP4=6,
    GX_TG_BUMP5=7,
    GX_TG_BUMP6=8,
    GX_TG_BUMP7=9,
    GX_TG_SRTG=10
} _GXTexGenType;

typedef enum _GXTexMapID {
    GX_TEXMAP0=0,
    GX_TEXMAP1=1,
    GX_TEXMAP2=2,
    GX_TEXMAP3=3,
    GX_TEXMAP4=4,
    GX_TEXMAP5=5,
    GX_TEXMAP6=6,
    GX_TEXMAP7=7,
    GX_MAX_TEXMAP=8,
    GX_TEXMAP_NULL=255,
    GX_TEX_DISABLE=256
} _GXTexMapID;

typedef enum _GXTexMtx {
    GX_TEXMTX0=30,
    GX_TEXMTX1=33,
    GX_TEXMTX2=36,
    GX_TEXMTX3=39,
    GX_TEXMTX4=42,
    GX_TEXMTX5=45,
    GX_TEXMTX6=48,
    GX_TEXMTX7=51,
    GX_TEXMTX8=54,
    GX_TEXMTX9=57,
    GX_IDENTITY=60
} _GXTexMtx;

typedef struct _GXTexObj _GXTexObj, *P_GXTexObj;

// Size: 0x20
typedef struct _GXTexObj
{
	UNKWORD mFlags; // at 0x0
	char UNK_0x4[0x4];
	UNKWORD mDimensions; // at 0x8 (Height/width are split between these bits)
	char UNK_0xC[0x4];
	UNKWORD mUserData; // at 0x10
	enum _GXTexFmt mFormat; // at 0x14
	UNKWORD mTLUT; // at 0x18
} GXTexObj;

typedef struct _GXTlutObj _GXTlutObj, *P_GXTlutObj;

struct _GXTlutObj {
    ulong dummy[3];
};

typedef enum _GXVtxFmt {
    GX_VTXFMT0=0,
    GX_VTXFMT1=1,
    GX_VTXFMT2=2,
    GX_VTXFMT3=3,
    GX_VTXFMT4=4,
    GX_VTXFMT5=5,
    GX_VTXFMT6=6,
    GX_VTXFMT7=7,
    GX_MAX_VTXFMT=8
} _GXVtxFmt;

typedef enum _GXZTexOp {
    GX_ZT_DISABLE=0,
    GX_ZT_ADD=1,
    GX_ZT_REPLACE=2,
    GX_MAX_ZTEXOP=3
} _GXZTexOp;

typedef struct _PERFTIMER _PERFTIMER, *P_PERFTIMER;

struct _PERFTIMER {
    char * name;
    ulong start;
    ulong stop;
    ulong elapsed;
    ulong average;
};

typedef enum DMTexFlt {
    DMTF_POINTSAMPLE=0,
    DMTF_BILERP=1
} DMTexFlt;

typedef enum GXAnisotropy {
    GX_ANISO_1=0,
    GX_ANISO_2=1,
    GX_ANISO_4=2,
    GX_MAX_ANISOTROPY=3
} GXAnisotropy;

typedef uchar GXBool;

typedef struct GXColorOLD GXColorOLD, *PGXColorOLD;

typedef uchar uint8_t;

struct GXColorOLD {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

typedef struct GXFifoObj GXFifoObj, *PGXFifoObj;

struct GXFifoObj {
    uint8_t pad[25];
};

typedef struct GXFogAdjTable GXFogAdjTable, *PGXFogAdjTable;

struct GXFogAdjTable {
    ushort _00[10];
};

typedef enum GXFogType {
    GX_FOG_NONE=0,
    GX_FOG_LINEAR=2,
    GX_FOG_EXPONENT=4,
    GX_FOG_EXPONENT2=5,
    GX_FOG_REVERSEEXP=6,
    GX_FOG_REVERSEXP2=7
} GXFogType;

typedef enum GXIndTexScale {
    GX_IND_TEX_SCALE_0=0
} GXIndTexScale;

typedef enum GXIndTexStageID {
    GX_IND_TEX_STAGE_ID_0=0,
    GX_IND_TEX_STAGE_ID_1=1,
    GX_IND_TEX_STAGE_ID_2=2,
    GX_IND_TEX_STAGE_ID_3=3
} GXIndTexStageID;

typedef struct GXInit GXInit, *PGXInit;

struct GXInit {
    char field0_0x0[128];
};

typedef struct GXLightObj GXLightObj, *PGXLightObj;

struct GXLightObj {
    float z;
    undefined field1_0x4;
    undefined field2_0x5;
    undefined field3_0x6;
    undefined field4_0x7;
    float x;
    float y;
};

typedef enum GXProjectionType {
    GX_PERSPECTIVE=0,
    GX_ORTHOGRAPHIC =1
} GXProjectionType;

typedef enum GXSpotFn {
    GX_SP_OFF=0,
    GX_SP_FLAT=1,
    GX_SP_COS=2,
    GX_SP_COS2=3,
    GX_SP_SHARP=4,
    GX_SP_RING1=5,
    GX_SP_RING2=6
} GXSpotFn;

typedef enum GXTevOp {
    GX_TEV_ADD=0,
    GX_TEV_SUB=1,
    GX_TEV_COMP_R8_GT=8,
    GX_TEV_COMP_R8_EQ=9,
    GX_TEV_COMP_GR16_GT=10,
    GX_TEV_COMP_GR16_EQ=11,
    GX_TEV_COMP_BGR24_GT=12,
    GX_TEV_COMP_BGR24_EQ=13,
    GX_TEV_COMP_A8_GT=14,
    GX_TEV_COMP_RGB8_GT=14,
    GX_TEV_COMP_A8_EQ=15,
    GX_TEV_COMP_RGB8_EQ=15
} GXTevOp;

typedef enum GXTevScale {
    GX_CS_SCALE_1=0,
    GX_CS_SCALE_2=1,
    GX_CS_SCALE_4=2,
    GX_CS_DIVIDE_2=3,
    GX_MAX_TEVSCALE=4
} GXTevScale;

typedef enum GXTexCacheSize {
    GX_TEXCACHE_32K=1,
    GX_TEXCACHE_128K=2,
    GX_TEXCACHE_512K=3,
    GX_TEXCACHE_NONE=4
} GXTexCacheSize;

typedef enum GXTexFilter {
    GX_NEAR=0,
    GX_LINEAR=1,
    GX_NEAR_MIP_NEAR=2,
    GX_LIN_MIP_NEAR=3,
    GX_NEAR_MIP_LIN=4,
    GX_LIN_MIP_LIN=5
} GXTexFilter;

typedef enum GXTexFmt {
    GX_TF_I4=0,
    GX_TF_I8=1,
    GX_TF_A8=2,
    GX_TF_IA4=3,
    GX_TF_IA8=4,
    GX_TF_RGB565=5,
    GX_TF_RGB5A3=6,
    GX_TF_RGBA8=7,
    GX_TF_CMPR=8,
    GX_TF_Z8=9,
    GX_TF_Z16=10,
    GX_TF_Z24X8=11,
    GX_CTF_R4=12,
    GX_CTF_RA4=13,
    GX_CTF_RA8=14,
    GX_CTF_A8=15,
    GX_CTF_R8=16,
    GX_CTF_G8=17,
    GX_CTF_B8=18,
    GX_CTF_RG8=19,
    GX_CTF_GB8=20,
    GX_CTF_Z4=21,
    GX_CTF_Z8M=22,
    GX_CTF_Z8L=23,
    GX_CTF_Z16L=24
} GXTexFmt;

typedef enum GXTexMtxType {
    GX_MTX3x4=0,
    GX_MTX2x4=1
} GXTexMtxType;

typedef struct GXTexObj GXTexObj, *PGXTexObj;

struct GXTexObj {
    uint field0_0x0;
    uint field1_0x4;
    uint field2_0x8;
    uint field3_0xc;
    undefined field4_0x10;
    undefined field5_0x11;
    undefined field6_0x12;
    undefined field7_0x13;
    uint field8_0x14;
    undefined field9_0x18;
    undefined field10_0x19;
    undefined field11_0x1a;
    undefined field12_0x1b;
    ushort field13_0x1c;
    undefined field14_0x1e;
    byte field15_0x1f;
};

typedef struct GXTexRegion GXTexRegion, *PGXTexRegion;

struct GXTexRegion {
    char padding[14];
};

typedef enum GXTexWrapMode {
    GX_CLAMP=0,
    GX_REPEAT=1,
    GX_MIRROR=2,
    GX_MAX_TEXWRAPMODE=3
} GXTexWrapMode;

typedef enum GXTlutSize {
    GX_TLUT_16=0,
    GX_TLUT_32=1,
    GX_TLUT_64=2,
    GX_TLUT_128=3,
    GX_TLUT_256=4,
    GX_TLUT_512=5,
    GX_TLUT_1K=6,
    GX_TLUT_2K=7,
    GX_TLUT_4K=8,
    GX_TLUT_8K=9,
    GX_TLUT_16K=10
} GXTlutSize;

typedef struct Heap Heap, *PHeap;

typedef struct HeapCell HeapCell, *PHeapCell;

struct HeapCell {
    struct HeapCell * prev;
    struct HeapCell * next;
    uint size;
};

struct Heap {
    int size;
    struct HeapCell * free;
    struct HeapCell * allocated;
};

typedef int OSHeapHandle;

typedef struct _AXPB _AXPB, *P_AXPB;

typedef struct _AXPBMIX _AXPBMIX, *P_AXPBMIX;

typedef struct _AXPBITD _AXPBITD, *P_AXPBITD;

typedef struct _AXPBUPDATE _AXPBUPDATE, *P_AXPBUPDATE;

typedef struct _AXPBDPOP _AXPBDPOP, *P_AXPBDPOP;

typedef struct _AXPBVE _AXPBVE, *P_AXPBVE;

typedef struct _AXPBFIR _AXPBFIR, *P_AXPBFIR;

typedef struct _AXPBADDR _AXPBADDR, *P_AXPBADDR;

typedef struct _AXPBADPCM _AXPBADPCM, *P_AXPBADPCM;

typedef struct _AXPBSRC _AXPBSRC, *P_AXPBSRC;

typedef struct _AXPBADPCMLOOP _AXPBADPCMLOOP, *P_AXPBADPCMLOOP;

struct _AXPBADPCM {
    ushort a[2][8];
    ushort gain;
    ushort pred_scale;
    ushort yn1;
    ushort yn2;
};

struct _AXPBVE {
    ushort currentVolume;
    short currentDelta;
};

struct _AXPBSRC {
    ushort ratioHi;
    ushort ratioLo;
    ushort currentAddressFrac;
    ushort last_samples[4];
};

struct _AXPBITD {
    ushort flag;
    ushort bufferHi;
    ushort bufferLo;
    ushort shiftL;
    ushort shiftR;
    ushort targetShiftL;
    ushort targetShiftR;
};

struct _AXPBFIR {
    ushort numCoefs;
    ushort coefsHi;
    ushort coefsLo;
};

struct _AXPBMIX {
    ushort vL;
    ushort vDeltaL;
    ushort vR;
    ushort vDeltaR;
    ushort vAuxAL;
    ushort vDeltaAuxAL;
    ushort vAuxAR;
    ushort vDeltaAuxAR;
    ushort vAuxBL;
    ushort vDeltaAuxBL;
    ushort vAuxBR;
    ushort vDeltaAuxBR;
    ushort vAuxBS;
    ushort vDeltaAuxBS;
    ushort vS;
    ushort vDeltaS;
    ushort vAuxAS;
    ushort vDeltaAuxAS;
};

struct _AXPBDPOP {
    short aL;
    short aAuxAL;
    short aAuxBL;
    short aR;
    short aAuxAR;
    short aAuxBR;
    short aS;
    short aAuxAS;
    short aAuxBS;
};

struct _AXPBADDR {
    ushort loopFlag;
    ushort format;
    ushort loopAddressHi;
    ushort loopAddressLo;
    ushort endAddressHi;
    ushort endAddressLo;
    ushort currentAddressHi;
    ushort currentAddressLo;
};

struct _AXPBUPDATE {
    ushort updNum[5];
    ushort dataHi;
    ushort dataLo;
};

struct _AXPBADPCMLOOP {
    ushort loop_pred_scale;
    ushort loop_yn1;
    ushort loop_yn2;
};

struct _AXPB {
    ushort nextHi;
    ushort nextLo;
    ushort currHi;
    ushort currLo;
    ushort srcSelect;
    ushort coefSelect;
    ushort mixerCtrl;
    ushort state;
    ushort type;
    struct _AXPBMIX mix;
    struct _AXPBITD itd;
    struct _AXPBUPDATE update;
    struct _AXPBDPOP dpop;
    struct _AXPBVE ve;
    struct _AXPBFIR fir;
    struct _AXPBADDR addr;
    struct _AXPBADPCM adpcm;
    struct _AXPBSRC src;
    struct _AXPBADPCMLOOP adpcmLoop;
    ushort pad[3];
};

typedef struct _AXPBITDBUFFER _AXPBITDBUFFER, *P_AXPBITDBUFFER;

struct _AXPBITDBUFFER {
    short data[32];
};

typedef struct _AXPBU _AXPBU, *P_AXPBU;

struct _AXPBU {
    ushort data[128];
};

typedef struct _AXVPB _AXVPB, *P_AXVPB;

struct _AXVPB {
    void * next;
    void * prev;
    void * next1;
    uint priority;
    void * callback;
    uint userContext;
    uint index;
    uint sync;
    uint depop;
    uint updateMS;
    uint updateCounter;
    uint updateTotal;
    ushort * updateWrite;
    ushort updateData[128];
    void * itdBuffer;
    struct _AXPB pb;
};

typedef struct adpcm adpcm, *Padpcm;

struct adpcm {
    struct _AXPBADPCM adpcm;
    struct _AXPBADPCMLOOP adpcmloop;
};

typedef struct sp_entry sp_entry, *Psp_entry;

struct sp_entry {
    uint type;
    uint sampleRate;
    uint loopAddr;
    uint loopEndAddr;
    uint endAddr;
    uint currentAddr;
    struct adpcm * adpcm;
};

typedef struct SS_Voices SS_Voices, *PSS_Voices;

struct SS_Voices {
    struct _AXVPB * ax_voice;
    struct adpcm * sp_entry;
    int sfxid;
};

typedef struct bootInfo bootInfo, *PbootInfo;

struct bootInfo { /* DVDInit */
    uint x0[8];
    uint boot_magic;
};

typedef struct DiskInfo DiskInfo, *PDiskInfo;

typedef struct DVDDiskID DVDDiskID, *PDVDDiskID;

struct DVDDiskID {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
    undefined field24_0x18;
    undefined field25_0x19;
    undefined field26_0x1a;
    undefined field27_0x1b;
    undefined field28_0x1c;
    undefined field29_0x1d;
    undefined field30_0x1e;
    undefined field31_0x1f;
};

struct DiskInfo {
    struct DVDDiskID diskID;
    uint8_t filler20[32];
    void * FSTLocationInRam;
    uint FSTMaxLength;
};

typedef struct DVDQueue_sub DVDQueue_sub, *PDVDQueue_sub;

struct DVDQueue_sub {
    struct DVDQueue_sub * unk0;
    struct DVDQueue_sub * unk4;
};

typedef struct GS_FrameBufferCopy GS_FrameBufferCopy, *PGS_FrameBufferCopy;

struct GS_FrameBufferCopy {
    struct _GXTexObj tobj;
    uchar * data;
    int top;
    int left;
    int width;
    int height;
};

typedef struct instNUGCUTCAM_s instNUGCUTCAM_s, *PinstNUGCUTCAM_s;

struct instNUGCUTCAM_s {
    uchar flags;
    uchar tgt_ix;
    char pad[2];
};

typedef struct instNUGCUTCAMSYS_s instNUGCUTCAMSYS_s, *PinstNUGCUTCAMSYS_s;

typedef struct NUGCUTCAMTGT_s NUGCUTCAMTGT_s, *PNUGCUTCAMTGT_s;

typedef struct nuvec_s nuvec_s, *Pnuvec_s;

struct nuvec_s {
    float x;
    float y;
    float z;
};

struct NUGCUTCAMTGT_s {
    struct nuvec_s * tgt;
    float frame;
    float nframes;
    char camid;
    char pad[3];
};

struct instNUGCUTCAMSYS_s {
    struct NUGCUTCAMTGT_s * tgts;
    struct instNUGCUTCAM_s * icutcams;
    uchar next_switch;
    char current_camera;
    uchar next_tgt_ix;
    uchar tgtarraysize;
    uchar ntgts;
    char pad[3];
};

typedef struct instNUGCUTCHAR_s instNUGCUTCHAR_s, *PinstNUGCUTCHAR_s;

struct instNUGCUTCHAR_s {
    void * character;
    float blend;
    float mtxblend;
    float frame1;
    float frame2;
    char flags;
    uchar prev_animix;
    uchar current_animix;
    uchar blendto_animix;
};

typedef struct instNUGCUTCHARSYS_s instNUGCUTCHARSYS_s, *PinstNUGCUTCHARSYS_s;

struct instNUGCUTCHARSYS_s {
    struct instNUGCUTCHAR_s * ichars;
};

typedef struct instNUGCUTLOCATOR_s instNUGCUTLOCATOR_s, *PinstNUGCUTLOCATOR_s;

struct instNUGCUTLOCATOR_s {
    float time;
    void * data;
};

typedef struct instNUGCUTLOCATORSYS_s instNUGCUTLOCATORSYS_s, *PinstNUGCUTLOCATORSYS_s;

struct instNUGCUTLOCATORSYS_s {
    struct instNUGCUTLOCATOR_s * ilocators;
};

typedef struct instNUGCUTRIGID_s instNUGCUTRIGID_s, *PinstNUGCUTRIGID_s;

typedef struct nuhspecial_s nuhspecial_s, *Pnuhspecial_s;

typedef struct nugscn_s nugscn_s, *Pnugscn_s;

typedef struct nuspecial_s nuspecial_s, *Pnuspecial_s;

typedef struct numtl_s numtl_s, *Pnumtl_s;

typedef struct nugobj_s nugobj_s, *Pnugobj_s;

typedef struct nuinstance_s nuinstance_s, *Pnuinstance_s;

typedef struct nugspline_s nugspline_s, *Pnugspline_s;

typedef struct nuinstanim_s nuinstanim_s, *Pnuinstanim_s;

typedef struct nuAnimData_s nuAnimData_s, *PnuAnimData_s;

typedef struct nutexanim_s nutexanim_s, *Pnutexanim_s;

typedef struct numtx_s numtx_s, *Pnumtx_s;

typedef struct nuspecialflags nuspecialflags, *Pnuspecialflags;

typedef struct numtlattrib_s numtlattrib_s, *Pnumtlattrib_s;

typedef struct nucolour3_s nucolour3_s, *Pnucolour3_s;

typedef union nufx_u nufx_u, *Pnufx_u;

typedef enum gobjtype_s {
    NUGOBJ_MESH=0,
    NUGOBJ_FACEON=1
} gobjtype_s;

typedef struct nugeom_s nugeom_s, *Pnugeom_s;

typedef struct nufaceongeom_s nufaceongeom_s, *Pnufaceongeom_s;

typedef struct nuinstflags_s nuinstflags_s, *Pnuinstflags_s;

typedef struct nuanimdatachunk_s nuanimdatachunk_s, *Pnuanimdatachunk_s;

typedef struct nutexanimenv_s nutexanimenv_s, *Pnutexanimenv_s;

typedef struct nusysmtl_s nusysmtl_s, *Pnusysmtl_s;

typedef enum nuvtxtype_e {
    NUVT_PS=17,
    NUVT_LC1=81,
    NUVT_TLTC1=83,
    NUVT_TC1=89,
    NUVT_SK3TC1=93
} nuvtxtype_e;

typedef struct nuprim_s nuprim_s, *Pnuprim_s;

typedef struct nuskin_s nuskin_s, *Pnuskin_s;

typedef struct NUVTXSKININFO_s NUVTXSKININFO_s, *PNUVTXSKININFO_s;

typedef struct NUBLENDGEOM_s NUBLENDGEOM_s, *PNUBLENDGEOM_s;

typedef enum nufaceontype_s {
    NUFACEON_FACEON=0,
    NUFACEON_FACEY=1
} nufaceontype_s;

typedef struct nufaceon_s nufaceon_s, *Pnufaceon_s;

typedef struct nuanimcurveset_s nuanimcurveset_s, *Pnuanimcurveset_s;

typedef struct nuanimkey_s nuanimkey_s, *Pnuanimkey_s;

typedef struct nuanimcurve_s nuanimcurve_s, *Pnuanimcurve_s;

typedef struct nutexanimprog_s nutexanimprog_s, *Pnutexanimprog_s;

typedef struct nurndritem_s nurndritem_s, *Pnurndritem_s;

typedef struct nugeomitem_s nugeomitem_s, *Pnugeomitem_s;

typedef enum nuprimtype_e {
    NUPT_POINT=0,
    NUPT_LINE=1,
    NUPT_TRI=2,
    NUPT_TRISTRIP=3,
    NUPT_NDXLINE=4,
    NUPT_NDXTRI=5,
    NUPT_NDXTRISTRIP=6,
    NUPT_BEZPATCH=7,
    NUPT_BEZTRI=8,
    NUPT_FACEON=9,
    NUPT_QUADLIST=10
} nuprimtype_e;

typedef struct nuplane_s nuplane_s, *Pnuplane_s;

typedef enum nurndritemtype_s {
    NURNDRITEM_GEOM3D=0,
    NURNDRITEM_GEOM2D=1,
    NURNDRITEM_SKIN3D2=2,
    NURNDRITEM_GEOMFACE=3
} nurndritemtype_s;

struct nugspline_s {
    short len;
    short ptsize;
    char * name;
    char * pts;
};

struct NUBLENDGEOM_s {
    int nblends;
    struct nuvec_s * * blend_offsets;
    int * ix;
    struct nuvec_s * offsets;
    struct nuvec_s * ooffsets;
    int hVB; /* GS_Buffer * */
    int blendindex[10];
};

struct nuanimdatachunk_s {
    int numnodes;
    int num_valid_animcurvesets;
    struct nuanimcurveset_s * * animcurveset;
    struct nuanimkey_s * keys;
    struct nuanimcurve_s * curves;
};

struct nurndritem_s {
    struct nurndritem_s * next;
    enum nurndritemtype_s type;
    int flags;
    short lights_index;
};

struct nugeomitem_s {
    struct nurndritem_s hdr;
    struct numtx_s * mtx;
    struct nugeom_s * geom;
    float * * blendvals;
    short instancelights_index[3];
    short hShader;
};

struct nutexanimenv_s {
    struct nutexanimprog_s * prog;
    int pc;
    int rep_count[16];
    int rep_start[16];
    int rep_ix;
    int ra[16];
    int ra_ix;
    int pause;
    int pause_r;
    int pause_cnt;
    struct numtl_s * mtl;
    short * tids;
    int tex_ix;
    int dynalloc:1;
};

struct nuplane_s {
    float a;
    float b;
    float c;
    float d;
};

struct nufaceon_s {
    struct nuvec_s point;
    float width;
    float height;
    int colour;
};

struct numtx_s {
    float _00;
    float _01;
    float _02;
    float _03;
    float _10;
    float _11;
    float _12;
    float _13;
    float _20;
    float _21;
    float _22;
    float _23;
    float _30;
    float _31;
    float _32;
    float _33;
};

struct nuinstflags_s {
    int visible:1;
    int onscreen:1;
    int visitest:1;
    int isanimated:1;
};

struct nuinstance_s {
    struct numtx_s matrix;
    int objid; /* Created by retype action */
    struct nuinstflags_s flags;
    struct nuinstanim_s * anim;
    short room_group;
    char special_flag;
    char pad[1];
};

struct nugscn_s { /* used in nusceneload */
    short * tids;
    int numtid;
    struct numtl_s * * mtls;
    int nummtl;
    int numgobj;
    struct nugobj_s * * gobjs;
    int numinstance;
    struct nuinstance_s * instances;
    int numspecial;
    struct nuspecial_s * specials;
    void * splinedata;
    int numsplines;
    struct nugspline_s * splines;
    char * nametable;
    int numexspecials;
    struct nuspecial_s * exspecials;
    int exallocix;
    struct nuinstanim_s * instanimblock;
    struct nuAnimData_s * * instanimdata;
    int numinstanims;
    int numinstanceixs;
    short * instanceixs;
    short crossfade;
    short crossfaderate;
    int crossfadegobj;
    int crossfadefirst;
    int numtexanims;
    struct nutexanim_s * texanims;
    short * texanim_tids;
    short * instancelightix;
};

struct nuspecialflags {
    int ext_onscreen:1;
    int ext_vis:1;
    int ext:1;
};

struct nuspecial_s {
    struct numtx_s mtx;
    struct nuinstance_s * instance;
    char * name;
    struct nuspecialflags flags;
    int pad;
};

struct nuAnimData_s {
    float time;
    char * node_name;
    int nchunks;
    struct nuanimdatachunk_s * * chunks;
};

struct nuhspecial_s {
    struct nugscn_s * scene;
    struct nuspecial_s * special;
};

struct instNUGCUTRIGID_s {
    struct nuhspecial_s special;
    char visibleframeix;
    char pad[3];
};

struct nuanimcurve_s {
    uint mask;
    struct nuanimkey_s * animkeys;
    uint numkeys;
    uint flags;
};

struct nutexanim_s {
    struct nutexanim_s * succ;
    struct nutexanim_s * prev;
    short * tids;
    short numtids;
    short dynalloc:1;
    struct numtl_s * mtl;
    struct nutexanimenv_s * env;
    char * ntaname;
    char * scriptname;
};

union nufx_u {
    void * voidptr;
    uchar u8;
    char s8;
    ushort u16;
    short s16;
    uint u32;
    int s32;
    float f32;
};

struct nutexanimprog_s {
    struct nutexanimprog_s * succ;
    struct nutexanimprog_s * prev;
    char name[32];
    int on_sig[32];
    int off_sig[32];
    uint on_mask;
    uint off_mask;
    short xdef_ids[32];
    short xdef_addrs[32];
    int xdef_cnt;
    short eop;
    short dynalloc:1;
    short code[1];
};

struct nuanimcurveset_s {
    int flags;
    float * constants;
    struct nuanimcurve_s * * set;
    char ncurves;
    char pad[3];
};

struct nufaceongeom_s {
    struct nufaceongeom_s * next;
    struct numtl_s * mtl;
    int mtl_id;
    enum nufaceontype_s faceon_type;
    struct nufaceon_s * faceons;
    int nfaceons;
    float faceon_radius;
};

struct nucolour3_s {
    float r;
    float g;
    float b;
};

struct nugobj_s {
    struct nugobj_s * sysnext;
    struct nugobj_s * syslast;
    enum gobjtype_s type;
    struct nugeom_s * geom;
    struct nufaceongeom_s * faceon_geom;
    float bounding_radius_from_origin;
    float bounding_rsq_from_origin;
    struct nuvec_s bounding_box_min;
    struct nuvec_s bounding_box_max;
    struct nuvec_s bounding_box_center;
    float bounding_radius_from_center;
    float bounding_rsq_from_center;
    int ngobjs;
    struct nugobj_s * next_gobj;
    struct nuvec_s origin;
    int ignore;
    int culltype;
};

struct numtlattrib_s {
    uint uvmode:1;
    uint afail:2;
    uint aref:8;
    uint atst:3;
    uint fill:1;
    uint colour:1;
    uint lighting:2;
    uint zmode:2;
    uint cull:2;
    uint vtc:2;
    uint utc:2;
    uint fx:2;
    uint filter:2;
    uint alpha:2;
};

struct numtl_s {
    struct numtl_s * next;
    struct numtlattrib_s attrib;
    struct nucolour3_s ambient;
    struct nucolour3_s diffuse;
    union nufx_u fx1;
    union nufx_u fx2;
    union nufx_u fx3;
    union nufx_u fx4;
    float power;
    float alpha;
    int tid;
    short alpha_sort;
    uchar fxid;
    uchar special_id;
    short K;
    uchar L;
    uchar vanmmode:4;
    uchar uanmmode:4;
    float du;
    float dv;
    float su;
    float sv;
};

struct nusysmtl_s {
    struct numtl_s mtl;
    struct nurndritem_s * rndrlist;
    struct nugeomitem_s * geom2d;
    struct nugeomitem_s * geom3d;
    struct nusysmtl_s * next;
    struct nusysmtl_s * last;
    int hShader;
};

struct nuprim_s {
    struct nuprim_s * next;
    enum nuprimtype_e type;
    ushort vertexCount;
    ushort max;
    ushort * vid;
    struct nuplane_s * pln;
    int idxbuff; /* GS_Buffer * */
    int cached;
    short skinmtxlookup[16];
};

struct NUVTXSKININFO_s {
    float wts[3];
    uchar joint_ixs[3];
    uchar pad;
};

struct nuinstanim_s {
    struct numtx_s Mtx;
    float tfactor;
    float tfirst;
    float tinterval;
    float ltime;
    uint oscillate:1;
    uint repeating:1;
    uint backwards:1;
    uint waiting:1;
    uint playing:1;
    int ipad[2];
    uchar anim_ix;
    char pad[3];
};

struct nuskin_s {
    struct nuskin_s * next;
    int vtxoffset;
    int vtxcnt;
    int mtxcnt;
    int * mtxid;
    float * weights;
};

struct nugeom_s {
    struct nugeom_s * next;
    struct nusysmtl_s * mtls;
    int mtl_id;
    enum nuvtxtype_e vertex_type;
    int vtxcount;
    int vtxmax;
    int hVB; /* GS_Buffer * */
    void * basisvbptr;
    struct nuprim_s * prim;
    struct nuskin_s * skin;
    struct NUVTXSKININFO_s * vtxskininfo;
    struct NUBLENDGEOM_s * blendgeom;
};

struct nuanimkey_s {
    float time;
    float dtime;
    float c;
    float d;
};

typedef struct instNUGCUTRIGIDSYS_s instNUGCUTRIGIDSYS_s, *PinstNUGCUTRIGIDSYS_s;

struct instNUGCUTRIGIDSYS_s {
    struct instNUGCUTRIGID_s * irigids;
};

typedef struct instNUGCUTSCENE_s instNUGCUTSCENE_s, *PinstNUGCUTSCENE_s;

typedef struct NUGCUTSCENE_s NUGCUTSCENE_s, *PNUGCUTSCENE_s;

typedef struct instNUGCUTTRIGGERSYS_s instNUGCUTTRIGGERSYS_s, *PinstNUGCUTTRIGGERSYS_s;

typedef struct NUGCUTCAMSYS_s NUGCUTCAMSYS_s, *PNUGCUTCAMSYS_s;

typedef struct NUGCUTRIGIDSYS_s NUGCUTRIGIDSYS_s, *PNUGCUTRIGIDSYS_s;

typedef struct NUGCUTCHARSYS_s NUGCUTCHARSYS_s, *PNUGCUTCHARSYS_s;

typedef struct NUGCUTLOCATORSYS_s NUGCUTLOCATORSYS_s, *PNUGCUTLOCATORSYS_s;

typedef struct NUGCUTBBOX_s NUGCUTBBOX_s, *PNUGCUTBBOX_s;

typedef struct NUGCUTTRIGGERSYS_S NUGCUTTRIGGERSYS_S, *PNUGCUTTRIGGERSYS_S;

typedef struct instNUTRIGGERSYS_s instNUTRIGGERSYS_s, *PinstNUTRIGGERSYS_s;

typedef struct instNUGCUTTRIGGER_s instNUGCUTTRIGGER_s, *PinstNUGCUTTRIGGER_s;

typedef struct NUGCUTCAM_s NUGCUTCAM_s, *PNUGCUTCAM_s;

typedef struct nuanimdata2_s nuanimdata2_s, *Pnuanimdata2_s;

typedef struct NUSTATEANIM_s NUSTATEANIM_s, *PNUSTATEANIM_s;

typedef struct NUGCUTRIGID_s NUGCUTRIGID_s, *PNUGCUTRIGID_s;

typedef struct NUGCUTCHAR_s NUGCUTCHAR_s, *PNUGCUTCHAR_s;

typedef struct NUGCUTLOCATOR_s NUGCUTLOCATOR_s, *PNUGCUTLOCATOR_s;

typedef struct NUGCUTLOCATORTYPE_s NUGCUTLOCATORTYPE_s, *PNUGCUTLOCATORTYPE_s;

typedef struct NUGCUTTRIGGER_s NUGCUTTRIGGER_s, *PNUGCUTTRIGGER_s;

typedef struct NUTRIGGERSYS_s NUTRIGGERSYS_s, *PNUTRIGGERSYS_s;

typedef struct instNUTRIGGER_s instNUTRIGGER_s, *PinstNUTRIGGER_s;

typedef struct nuanimcurve2_s nuanimcurve2_s, *Pnuanimcurve2_s;

typedef enum locdir {
    LOCATOR_NO_DIRECTION=0,
    LOCATOR_UP=1,
    LOCATOR_DOWN=2,
    LOCATOR_X=3,
    LOCATOR_NEGX=4,
    LOCATOR_Y=5,
    LOCATOR_NEGY=6,
    LOCATOR_Z=7,
    LOCATOR_NEGZ=8
} locdir;

typedef union adata adata, *Padata;

typedef struct nuanimcurvedata_s nuanimcurvedata_s, *Pnuanimcurvedata_s;

struct NUGCUTTRIGGER_s {
    short ix;
    short pad;
    char * triggername;
    struct NUSTATEANIM_s * enableflag1anim;
};

struct instNUGCUTTRIGGER_s {
    char next_ix;
    char pad[3];
};

struct NUGCUTSCENE_s {
    int version;
    int address_offset;
    float nframes;
    char * name_table;
    struct NUGCUTCAMSYS_s * cameras;
    struct NUGCUTRIGIDSYS_s * rigids;
    struct NUGCUTCHARSYS_s * chars;
    struct NUGCUTLOCATORSYS_s * locators;
    struct NUGCUTBBOX_s * bbox;
    struct NUGCUTTRIGGERSYS_S * triggersys;
};

struct nuanimdata2_s {
    float endframe;
    short nnodes;
    short ncurves;
    short nchunks;
    short pad;
    struct nuanimcurve2_s * curves;
    char * curveflags;
    char * curvesetflags;
};

union adata {
    float constant;
    struct nuanimcurvedata_s * curvedata;
};

struct NUGCUTBBOX_s {
    struct nuvec_s min;
    struct nuvec_s max;
};

struct NUGCUTTRIGGERSYS_S {
    int ntriggers;
    struct NUGCUTTRIGGER_s * triggers;
};

struct NUGCUTCAMSYS_s {
    uint ncutcams;
    struct NUGCUTCAM_s * cutcams;
    struct nuanimdata2_s * camanim;
    struct NUSTATEANIM_s * camchanges;
    char initial_camid;
    char remap_table[10];
    uchar pad;
};

struct NUGCUTCHAR_s {
    struct numtx_s mtx;
    char * name;
    struct nuanimdata2_s * char_anim;
    struct nuanimdata2_s * joint_anim;
    struct nuanimdata2_s * face_anim;
    void * character;
    struct NUGCUTLOCATOR_s * locators;
    float animrate;
    uchar flags;
    uchar animix;
    uchar nlocators;
    uchar type;
    uchar first_locator;
    uchar blendframes;
    char pad[2];
};

struct NUSTATEANIM_s {
    int nchanges;
    float * frames;
    uchar * states;
};

struct instNUGCUTSCENE_s {
    struct instNUGCUTSCENE_s * next;
    struct instNUGCUTSCENE_s * prev;
    char name[16];
    struct numtx_s mtx;
    struct NUGCUTSCENE_s * cutscene;
    struct nuvec_s centre;
    float maxdistsqr;
    int is_visible:1;
    int checkmaxdist:1;
    int checkbboxclip:1;
    int has_mtx:1;
    int autostart:1;
    int looping:1;
    uint is_playing:1;
    int played_first_frame:1;
    int is_disabled:16;
    int been_updated_this_frame:1;
    float cframe;
    float rate;
    struct instNUGCUTCAMSYS_s * icameras;
    struct instNUGCUTRIGIDSYS_s * irigids;
    struct instNUGCUTCHARSYS_s * ichars;
    struct instNUGCUTLOCATORSYS_s * ilocators;
    struct instNUGCUTTRIGGERSYS_s * itriggersys;
    struct instNUGCUTSCENE_s * next_to_play;
    void * endfn;
};

struct NUTRIGGERSYS_s {
    int ntriggers;
    struct NUGCUTTRIGGER_s triggers;
};

struct nuanimcurve2_s {
    union adata data;
};

struct instNUGCUTTRIGGERSYS_s {
    struct instNUTRIGGERSYS_s * itriggersys;
    struct instNUGCUTTRIGGER_s * itriggers;
};

struct instNUTRIGGERSYS_s {
    struct instNUTRIGGERSYS_s * next;
    struct instNUTRIGGERSYS_s * prev;
    struct NUTRIGGERSYS_s * triggersys;
    struct instNUTRIGGER_s * itriggers;
    void * scenemanager;
    int is_disabled:1;
};

struct NUGCUTLOCATORTYPE_s {
    char * name;
    uchar flags[2];
    ushort ix;
    char pad[4];
};

struct NUGCUTLOCATORSYS_s {
    struct NUGCUTLOCATOR_s * locators;
    struct NUGCUTLOCATORTYPE_s * locator_types;
    uchar nlocators;
    uchar ntypes;
    char pad[2];
};

struct nuanimcurvedata_s {
    uint * mask;
    ushort * key_ixs;
    void * key_array;
};

struct NUGCUTRIGIDSYS_s {
    struct NUGCUTRIGID_s * rigids;
    ushort nrigids;
    char pad[2];
};

struct NUGCUTCAM_s {
    struct numtx_s mtx;
    uchar flags;
    uchar id;
    uchar anim_ix;
    uchar pad[13];
};

struct NUGCUTLOCATOR_s {
    struct numtx_s mtx;
    struct nuvec_s pivot;
    float rate;
    struct nuanimdata2_s * anim;
    enum locdir direction;
    uchar flags;
    uchar locator_type;
    uchar joint_ix;
    char pad;
    float param1;
    float param2;
};

struct NUGCUTRIGID_s {
    struct numtx_s mtx;
    char * name;
    struct nuhspecial_s special;
    struct nuanimdata2_s * anim;
    struct NUSTATEANIM_s * visibiltyanim;
    struct NUGCUTLOCATOR_s * locators;
    uchar flags;
    uchar nlocators;
    uchar first_locator;
    char pad;
};

struct instNUTRIGGER_s {
    short hitpoints;
    uchar enableflags;
    char flags;
};

struct NUGCUTCHARSYS_s {
    struct NUGCUTCHAR_s * chars;
    ushort nchars;
    char pad[2];
};

typedef struct locdataFN locdataFN, *PlocdataFN;

struct locdataFN {
    struct instNUGCUTSCENE_s * field0_0x0;
    struct NUGCUTLOCATORSYS_s * field1_0x4;
    struct instNUGCUTLOCATOR_s * field2_0x8;
    struct NUGCUTLOCATOR_s * field3_0xc;
    float field4_0x10;
    struct numtx_s * field5_0x14;
};

typedef union BMPtmp BMPtmp, *PBMPtmp;

union BMPtmp {
    uint u32;
    uchar u8[4];
};

typedef struct tagBITMAPFILEHEADER tagBITMAPFILEHEADER, *PtagBITMAPFILEHEADER;

struct tagBITMAPFILEHEADER {
    ushort pad;
    ushort bfType;
    uint bfSize;
    ushort bfReserved1;
    ushort bfReserved2;
    uint bfOffBits;
};

typedef struct tagBITMAPINFOHEADER tagBITMAPINFOHEADER, *PtagBITMAPINFOHEADER;

struct tagBITMAPINFOHEADER {
    uint biSize;
    uint biWidth;
    uint biHeight;
    ushort biPlanes;
    ushort biBitCount;
    uint biCompression;
    uint biSizeImage;
    uint biXPelsPerMeter;
    uint biYPelsPerMeter;
    uint biClrUsed;
    uint biClrImportant;
};

typedef struct _REMAPTABLE _REMAPTABLE, *P_REMAPTABLE;

struct _REMAPTABLE {
    char * name;
    uchar * remap;
};

typedef struct nusound_filename_info_s nusound_filename_info_s, *Pnusound_filename_info_s;

struct nusound_filename_info_s {
    char Filename[32];
    int Pitch;
    int LoopInfo;
    int Type;
    int ID;
};

typedef struct _D3DCOLORVALUE _D3DCOLORVALUE, *P_D3DCOLORVALUE;

struct _D3DCOLORVALUE {
    float r;
    float g;
    float b;
    float a;
};

typedef struct _D3DLIGHT8 _D3DLIGHT8, *P_D3DLIGHT8;

typedef enum _D3DLIGHTTYPE {
    D3DLIGHT_POINT=1,
    D3DLIGHT_SPOT=2,
    D3DLIGHT_DIRECTIONAL=3,
    _D3DLIGHTTYPE=255
} _D3DLIGHTTYPE;

struct _D3DLIGHT8 {
    enum _D3DLIGHTTYPE Type;
    struct _D3DCOLORVALUE Diffuse;
    struct _D3DCOLORVALUE Specular;
    struct _D3DCOLORVALUE Ambient;
    struct nuvec_s Position;
    struct nuvec_s Direction;
    float range;
    float falloff;
    float attenuation0;
    float attenuation1;
    float attenuation2;
    float Theta;
    float Phi;
};

typedef struct _D3DMATERIAL8 _D3DMATERIAL8, *P_D3DMATERIAL8;

struct _D3DMATERIAL8 {
    struct _D3DCOLORVALUE Diffuse;
    struct _D3DCOLORVALUE Ambient;
    struct _D3DCOLORVALUE Specular;
    struct _D3DCOLORVALUE Emissive;
    float Power;
};

typedef enum _D3DTEXTURESTAGESTATETYPE {
    D3DTSS_ADDRESSU=0,
    D3DTSS_ADDRESSV=1,
    D3DTSS_ADDRESSW=2,
    D3DTSS_MAGFILTER=3,
    D3DTSS_MINFILTER=4,
    D3DTSS_MIPFILTER=5,
    D3DTSS_MIPMAPLODBIAS=6,
    D3DTSS_MAXMIPLEVEL=7,
    D3DTSS_MAXANISOTROPY=8,
    D3DTSS_COLORKEYOP=9,
    D3DTSS_COLORSIGN=10,
    D3DTSS_ALPHAKILL=11,
    D3DTSS_COLOROP=12,
    D3DTSS_DEFERRED_TEXTURE_STATE_MAX=12,
    D3DTSS_COLORARG0=13,
    D3DTSS_COLORARG1=14,
    D3DTSS_COLORARG2=15,
    D3DTSS_ALPHAOP=16,
    D3DTSS_ALPHAARG0=17,
    D3DTSS_ALPHAARG1=18,
    D3DTSS_ALPHAARG2=19,
    D3DTSS_RESULTARG=20,
    D3DTSS_TEXTURETRANSFORMFLAGS=21,
    D3DTSS_BUMPENVMAT00=22,
    D3DTSS_DEFERRED_MAX=22,
    D3DTSS_BUMPENVMAT01=23,
    D3DTSS_BUMPENVMAT11=24,
    D3DTSS_BUMPENVMAT10=25,
    D3DTSS_BUMPENVLSCALE=26,
    D3DTSS_BUMPENVLOFFSET=27,
    D3DTSS_TEXCOORDINDEX=28,
    D3DTSS_BORDERCOLOR=29,
    D3DTSS_COLORKEYCOLOR=30,
    D3DTSS_MAX=31,
    D3DTSS_FORCE_DWORD=2147483647
} _D3DTEXTURESTAGESTATETYPE;

typedef struct _D3DVIEWPORT8 _D3DVIEWPORT8, *P_D3DVIEWPORT8;

struct _D3DVIEWPORT8 {
    uint X;
    uint Y;
    uint Width;
    uint Height;
    float MinZ;
    float MaxZ;
};

typedef struct _GS_TEXTURE _GS_TEXTURE, *P_GS_TEXTURE;

typedef enum gs_texturetype_e {
    GS_TEX_RGB16=0,
    GS_TEX_RGBA16=1,
    GS_TEX_RGB24=2,
    GS_TEX_RGBA32=3,
    GS_TEX_PAL4=4,
    GS_TEX_PAL8=5,
    GS_TEX_PAL4_S=6,
    GS_TEX_PAL8_S=7,
    GS_TEX_DXT1=8,
    GS_TEX_DXT2=9,
    GS_TEX_DXT3=10,
    GS_TEX_DXT4=11,
    GS_TEX_DXT5=12,
    GS_TEX_BB=13,
    GS_TEX_RT8=14,
    GS_TEX_RTT8=15,
    GS_TEX_RT8H=16,
    GS_TEX_RT32=17,
    GS_TEX_RT24=18,
    GS_TEX_RT16=19,
    GS_TEX_RTT32=20,
    GS_TEX_RTT24=21,
    GS_TEX_RTT16=22,
    GS_TEX_RZT32=23,
    GS_TEX_RZT24=24,
    GS_TEX_RZT16=25,
    GS_TEX_NUMTEXTURETYPES=26
} gs_texturetype_e;

struct _GS_TEXTURE {
    int Flags;
    enum gs_texturetype_e Format;
    uint Width;
    uint Height;
    uint TexBits;
    uint PalBits;
    uint NUID;
    uint Pad;
    struct _GXTexObj Tex;
    struct _GXTlutObj Tlut;
};

typedef struct _GS_VECTOR3 _GS_VECTOR3, *P_GS_VECTOR3;

struct _GS_VECTOR3 {
    float x;
    float y;
    float z;
};

typedef struct _GS_VECTOR4 _GS_VECTOR4, *P_GS_VECTOR4;

struct _GS_VECTOR4 {
    float x;
    float y;
    float z;
    float w;
};

typedef struct _GS_VIEWPORT _GS_VIEWPORT, *P_GS_VIEWPORT;

struct _GS_VIEWPORT {
    ulong X;
    ulong Y;
    ulong width;
    ulong height;
    float MinZ;
    float MaxZ;
};

typedef struct _GSMATRIX _GSMATRIX, *P_GSMATRIX;

struct _GSMATRIX {
    float _00;
    float _01;
    float _02;
    float _03;
    float _10;
    float _11;
    float _12;
    float _13;
    float _20;
    float _21;
    float _22;
    float _23;
    float _30;
    float _31;
    float _32;
    float _33;
};

typedef struct _LIGHTLIST _LIGHTLIST, *P_LIGHTLIST;

struct _LIGHTLIST {
    int EnableLight;
    struct _D3DLIGHT8 Light;
};

typedef struct _sceDmaTag _sceDmaTag, *P_sceDmaTag;

struct _sceDmaTag {
    ushort qwc;
    uchar mark;
    uchar id;
    struct _sceDmaTag * next;
    uint p[2];
};

typedef struct _XINPUT_FEEDBACK _XINPUT_FEEDBACK, *P_XINPUT_FEEDBACK;

typedef struct _XINPUT_FEEDBACK_HEADER _XINPUT_FEEDBACK_HEADER, *P_XINPUT_FEEDBACK_HEADER;

struct _XINPUT_FEEDBACK_HEADER {
    uint dwStatus;
    void * hEvent;
    uchar Reserved[58];
    char unk[2];
};

struct _XINPUT_FEEDBACK {
    struct _XINPUT_FEEDBACK_HEADER header;
    char unk[4];
};

typedef struct _XINPUT_STATE _XINPUT_STATE, *P_XINPUT_STATE;

struct _XINPUT_STATE {
    uint dwPacketNumber;
    void * field1_0x4;
    void * field2_0x8;
    void * field3_0xc;
    void * field4_0x10;
    void * field5_0x14;
};

typedef enum Actions_ {
    ACT_ATTACK=0,
    ACT_ATTACK2=1,
    ACT_CRAWL=2,
    ACT_CROUCHDOWN=3,
    ACT_CROUCHIDLE=4,
    ACT_CROUCHUP=5,
    ACT_DIE=6,
    ACT_DIE2=7,
    ACT_DOWN=8,
    ACT_DROWN=9,
    ACT_EXPLODE=10,
    ACT_FALL=11,
    ACT_FILL=12,
    ACT_FLY=13,
    ACT_GETUP=14,
    ACT_HANG=15,
    ACT_HOP=16,
    ACT_IDLE=17,
    ACT_IDLE00=18,
    ACT_IDLE01=19,
    ACT_IDLE02=20,
    ACT_IDLE03=21,
    ACT_IDLE04=22,
    ACT_IDLE05=23,
    ACT_IDLE06=24,
    ACT_IDLE07=25,
    ACT_IN=26,
    ACT_JUMP=27,
    ACT_JUMP2=28,
    ACT_LAND=29,
    ACT_LAND2=30,
    ACT_LEFT=31,
    ACT_LOSE=32,
    ACT_OPEN=33,
    ACT_OUT=34,
    ACT_PULL=35,
    ACT_PUSH=36,
    ACT_RIGHT=37,
    ACT_RUN=38,
    ACT_RUN2=39,
    ACT_RUNNINGJUMP=40,
    ACT_SHAKE=41,
    ACT_SIT=42,
    ACT_SKATE=43,
    ACT_SKID=44,
    ACT_SLAM=45,
    ACT_SLEEP=46,
    ACT_SLIDE=47,
    ACT_SOMERSAULT=48,
    ACT_SPENT=49,
    ACT_SPIN=50,
    ACT_SPIN2=51,
    ACT_SPRINT=52,
    ACT_STARJUMP=53,
    ACT_SUPERSLAM=54,
    ACT_SWIM=55,
    ACT_SWING=56,
    ACT_TIPTOE=57,
    ACT_TRAPPED=58,
    ACT_TRAPPEDSHAKE=59,
    ACT_TURN=60,
    ACT_UP=61,
    ACT_WADE=62,
    ACT_WAIT=63,
    ACT_WALK=64,
    ACT_WIN=65,
    ACT_FLYZOFFA=66,
    MAXACTIONS=67
} Actions_;

typedef struct ADeb ADeb, *PADeb;

struct ADeb {
    short character;
    short action;
    float time;
    float time2;
    uint flags;
    ushort locators;
    uchar gdeb;
    char count;
    uchar random;
    uchar xrot;
    char type;
    char target;
    short sfx;
    uchar rumble;
    uchar buzz;
    char unk[4];
    ulonglong levbits;
    float min;
    float max;
};

typedef struct ai_s ai_s, *Pai_s;

struct ai_s {
    char ai_type;
    char status;
    char pad1;
    char pad2;
    char pad3;
    char iRAIL;
    short iALONG;
    float fALONG;
    float time;
    float delay;
    struct nuvec_s pos[8];
    struct nuvec_s origin;
};

typedef struct AI_tmp AI_tmp, *PAI_tmp;

struct AI_tmp {
    struct nuvec_s oldpos;
    struct nuvec_s newpos;
    struct nuvec_s wobblepos;
    float time;
    float duration;
    float spins;
    float speed;
    float radius;
    float distance;
    float height;
    float lateral;
    float scale;
    float wobble;
    ushort oldangle;
    ushort angle;
    ushort old_yrot;
    ushort movflags;
    ushort terflags;
    ushort rotflags;
    ushort rotspeed;
    ushort rotadjust;
    ushort wobble_ang;
    ushort wobble_speed;
    char direction;
    char i0;
    char i1;
    char idle_update;
    void * event_function[2];
    float event_distance[2];
    short event_branch[2];
    char event[2];
    char event_index[2];
    uchar event_flags[2];
    uchar event_occured[2];
    ushort attack_locator_bits;
    char pad1;
    char pad2;
    float attack_locator_radius;
    struct nuhspecial_s obj;
    char kill;
    char locator_kill;
    short force_action;
    short look_creature;
    uchar count;
    uchar hits;
};

typedef struct AIType AIType, *PAIType;

typedef struct creatcmd_s creatcmd_s, *Pcreatcmd_s;

struct creatcmd_s {
    int cmd;
    int i;
    float f;
};

struct AIType {
    short character;
    short points;
    struct creatcmd_s * cmd;
    char name[16];
    float delay;
};

typedef struct alpslot alpslot, *Palpslot;

struct alpslot {
    float t;
    float a;
};

typedef ushort angle;

typedef struct anim_parameters_s anim_parameters_s, *Panim_parameters_s;

struct anim_parameters_s {
    int objid;
    int usedpart;
    int usedsound;
    int trigger_type;
    int trigger_id;
    float trigger_var;
    float trigger_wait;
    char particle_name[16][8];
    int particle_type[8];
    int particle_rate[8];
    int particle_switch[8];
    struct nuvec_s particle_offset[8];
    short particle_emitrotz[8];
    short particle_emitroty[8];
    float sound_last_time;
    char sound_name[16][8];
    int sound_id[8];
    int sound_type[8];
    float sound_time[8];
    struct nuvec_s sound_offset[8];
    int terrplatid;
    float playergrav;
    float tension;
    float damping;
};

typedef struct anim_s anim_s, *Panim_s;

struct anim_s {
    float anim_time;
    float blend_src_time;
    float blend_dst_time;
    short action;
    short oldaction;
    short newaction;
    short blend_src_action;
    short blend_dst_action;
    short blend_frame;
    short blend_frames;
    char blend;
    uchar flags;
};

typedef struct animdatainst_s animdatainst_s, *Panimdatainst_s;

struct animdatainst_s {
    struct nuAnimData_s * ad;
    char name[256];
    int inst_cnt;
};

typedef struct AnimList AnimList, *PAnimList;

struct AnimList {
    char * file;
    short action;
    uchar blend_in_frames;
    uchar blend_out_frames;
    float speed;
    ushort flags;
    char pad1;
    char pad2;
    ulonglong levbits;
};

typedef struct ATLASSTRUCT ATLASSTRUCT, *PATLASSTRUCT;

typedef struct creature_s creature_s, *Pcreature_s;

typedef struct MYDRAW MYDRAW, *PMYDRAW;

typedef struct Quat Quat, *PQuat;

typedef struct obj_s obj_s, *Pobj_s;

typedef struct NEWBUGGY NEWBUGGY, *PNEWBUGGY;

typedef struct Moveinfo_s Moveinfo_s, *PMoveinfo_s;

typedef struct Nearest_Light_s Nearest_Light_s, *PNearest_Light_s;

typedef struct rumble_s rumble_s, *Prumble_s;

typedef struct CharacterModel CharacterModel, *PCharacterModel;

typedef struct NUJOINTANIM_s NUJOINTANIM_s, *PNUJOINTANIM_s;

typedef struct mask mask, *Pmask;

typedef struct RPos_s RPos_s, *PRPos_s;

typedef union type_s_name type_s_name, *Ptype_s_name;

typedef struct pdir_s pdir_s, *Ppdir_s;

typedef struct NUHGOBJ_s NUHGOBJ_s, *PNUHGOBJ_s;

typedef struct NUPOINTOFINTEREST_s NUPOINTOFINTEREST_s, *PNUPOINTOFINTEREST_s;

typedef struct NUJOINTDATA_s NUJOINTDATA_s, *PNUJOINTDATA_s;

typedef struct NULAYERDATA_s NULAYERDATA_s, *PNULAYERDATA_s;

typedef struct NUSHADOWDATA_s NUSHADOWDATA_s, *PNUSHADOWDATA_s;

typedef struct NUCOLLISIONDATA_s NUCOLLISIONDATA_s, *PNUCOLLISIONDATA_s;

typedef struct NUELLIPSOID_s NUELLIPSOID_s, *PNUELLIPSOID_s;

typedef struct NUCYLINDERS_s NUCYLINDERS_s, *PNUCYLINDERS_s;

typedef struct NUSHADOWMESH_s NUSHADOWMESH_s, *PNUSHADOWMESH_s;

typedef struct NUCOLLISIONHDR_s NUCOLLISIONHDR_s, *PNUCOLLISIONHDR_s;

typedef struct nuvec4_s nuvec4_s, *Pnuvec4_s;

typedef enum NUCOLLISIONTYPES {
    NUCOLLISIONSPHERETYPE=1
} NUCOLLISIONTYPES;

struct pdir_s {
    int Index;
    struct nuvec_s Direction;
    struct nucolour3_s Colour;
    float Distance;
};

struct NUELLIPSOID_s {
    struct nuvec_s centre;
    struct nuvec_s y_axis;
    struct nuvec_s x_axis;
    struct nuvec_s z_axis;
};

struct NUCOLLISIONHDR_s {
    enum NUCOLLISIONTYPES type;
    void * data;
};

union type_s_name {
    uchar chrs;
    ushort all;
};

struct nuvec4_s {
    float x;
    float y;
    float z;
    float w;
};

struct Quat {
    struct nuvec_s v;
    float w;
};

struct rumble_s {
    uchar buzz;
    uchar power;
    uchar frame;
    uchar frames;
};

struct NUSHADOWDATA_s {
    struct NUELLIPSOID_s * ellipsoids;
    struct NUCYLINDERS_s * cylinders;
    struct NUSHADOWMESH_s * shadow_meshes;
    uchar nellipsoids;
    uchar ncylinders;
    uchar nshadow_meshes;
    uchar joint;
};

struct NULAYERDATA_s {
    char * name;
    struct nugobj_s * * gobjs;
    struct nugobj_s * skin_gobj;
    struct nugobj_s * * blend_gobjs;
    struct nugobj_s * blend_skin_gobj;
};

struct Nearest_Light_s {
    int AmbIndex;
    struct nuvec_s AmbCol;
    float ambientdist;
    int CurLoopIndex;
    struct pdir_s dir1;
    struct pdir_s dir2;
    struct pdir_s dir3;
    struct pdir_s * pDir1st;
    struct pdir_s * pDir2nd;
    struct pdir_s * pDir3rd;
    int glbambindex;
    int negativeindex;
    float negativedist;
    struct pdir_s glbdirectional;
};

struct MYDRAW {
    struct anim_s Anim;
    struct CharacterModel * Model;
    int Character;
    int NumJoints;
    struct NUJOINTANIM_s * JointList;
    struct nuvec_s * Position;
    struct Nearest_Light_s Nearest_Light;
};

struct Moveinfo_s {
    float IDLESPEED;
    float TIPTOESPEED;
    float WALKSPEED;
    float RUNSPEED;
    float SPRINTSPEED;
    float SLIDESPEED;
    float CRAWLSPEED;
    float DANGLESPEED;
    float WADESPEED;
    float JUMPHEIGHT;
    float DANGLEGAP;
    short JUMPFRAMES0;
    short JUMPFRAMES1;
    short JUMPFRAMES2;
    short STARJUMPFRAMES;
    short SOMERSAULTFRAMES;
    short SPINFRAMES;
    short SPINRESETFRAMES;
    short SUPERSPINFRAMES;
    short SUPERSPINWAITFRAMES;
    short SLAMWAITFRAMES;
    short SLIDEFRAMES;
    short CROUCHINGFRAMES;
    short JUMPLANDFRAMES;
    short spad;
};

struct RPos_s {
    char iRAIL;
    char vertical;
    short iALONG;
    short i1;
    short i2;
    float fALONG;
    float fACROSS;
    ushort angle;
    ushort cam_angle;
    uchar mode;
    char pad1;
    char pad2;
    char pad3;
    struct nuvec_s pos;
};

struct mask {
    struct numtx_s mM;
    struct numtx_s mS;
    struct nuvec_s pos;
    struct nuvec_s newpos;
    struct Nearest_Light_s lights;
    struct anim_s anim;
    float scale;
    float shadow;
    short character;
    short active;
    short sfx;
    char pad1;
    char pad2;
    ushort xrot;
    ushort yrot;
    ushort angle;
    ushort surface_xrot;
    ushort surface_zrot;
    ushort wibble_ang[3];
    float idle_time;
    float idle_duration;
    char idle_mode;
    char reflect;
    uchar offset_frame;
    char hold;
};

struct obj_s {
    struct obj_s * parent;
    struct CharacterModel * model;
    struct mask * mask;
    void * contact;
    struct numtx_s * pLOCATOR;
    struct anim_s anim;
    short character;
    short vehicle;
    uint flags;
    uint frame;
    uint draw_frame;
    uint vehicle_frame;
    struct RPos_s RPos;
    struct nuvec_s pos;
    struct nuvec_s mom;
    struct nuvec_s oldpos;
    struct nuvec_s startpos;
    struct nuvec_s vSN;
    struct nuvec_s vLN;
    struct nuvec_s vRN;
    float shadow;
    float layer_shadow;
    float roof_y;
    float clearance;
    float forward;
    float abs_forward;
    float side;
    float abs_side;
    float xyz_distance;
    float xz_distance;
    float radius;
    struct nuvec_s min;
    struct nuvec_s max;
    float scale;
    float SCALE;
    float RADIUS;
    float old_SCALE;
    float objbot;
    float objtop;
    float bot;
    float top;
    float oldobjbot;
    float oldobjtop;
    float die_time;
    float die_duration;
    float reflect_y;
    float idle_gametime;
    float pad_speed;
    float pad_dx;
    float pad_dz;
    char i;
    char dead;
    ushort pad_angle;
    ushort attack;
    ushort vulnerable;
    short die_action;
    char old_ground;
    char finished;
    ushort xrot;
    ushort yrot;
    ushort zrot;
    ushort surface_xrot;
    ushort surface_zrot;
    ushort layer_xrot;
    ushort layer_zrot;
    ushort roof_xrot;
    ushort roof_zrot;
    short target_xrot;
    short target_yrot;
    short dyrot;
    union type_s_name gndflags;
    ushort hdg;
    ushort thdg;
    char ground;
    char surface_type;
    char layer_type;
    char roof_type;
    char invisible;
    uchar submerged;
    char transporting;
    char got_shadow;
    uchar boing;
    uchar contact_type;
    char die_model[2];
    uchar invincible;
    char pos_adjusted;
    char wade;
    char dangle;
    char ddsand;
    char ddsnow;
    char ddwater;
    char ddr;
    char ddg;
    char ddb;
    char last_ground;
    char direction;
    char kill_contact;
    uchar touch;
};

struct NUCOLLISIONDATA_s {
    uint nitems;
    struct NUCOLLISIONHDR_s * hdrs;
};

struct NUSHADOWMESH_s {
    struct nuvec4_s * normals;
    struct nuvec4_s * verts;
};

struct NEWBUGGY {
    void * a; /* struct TODO */
};

struct CharacterModel {
    struct NUHGOBJ_s * hobj;
    struct nuAnimData_s * anmdata[118];
    struct AnimList * animlist[118];
    struct nuAnimData_s * fanmdata[118];
    struct AnimList * fanimlist[118];
    int shadhdr;
    short character;
    char pad1;
    char pad2;
    struct NUPOINTOFINTEREST_s * pLOCATOR[16];
};

struct NUPOINTOFINTEREST_s {
    struct numtx_s offset;
    char * name;
    uchar parent_joint_ix;
    uchar pad[11];
};

struct ATLASSTRUCT {
    struct creature_s * Cre;
    int Whacko;
    int Type;
    int Dead;
    int HitPoints;
    int DestHitPoints;
    int HitPointCounter;
    int DrawCrunch;
    int DrawShell;
    float InhibitControlTimer;
    int NumAttacks;
    int Action;
    int LastAction;
    float ActionTimer;
    float ActionTimer2;
    int BeenHit;
    int CantBeHit;
    float RollSpeed;
    float RollAccTimer;
    struct MYDRAW Shell;
    struct MYDRAW Crunch;
    struct nuvec_s Position;
    struct nuvec_s OldPosition;
    struct nuvec_s LastPosition;
    struct nuvec_s TargetPosition;
    struct nuvec_s Velocity;
    struct nuvec_s OldVelocity;
    struct nuvec_s Resolved;
    struct nuvec_s Force;
    float Radius;
    float AngleX;
    float AngleY;
    float AngleZ;
    float InputAng;
    float InputMag;
    struct Quat Quat;
    struct Quat ThisQuat;
    struct Quat FrameQuat[4];
    struct Quat LastQuat;
    int LastHit;
    int Axis;
    struct nuvec_s LastNormal;
    struct nuvec_s StoreLastNormal;
    float DebugAxisTurn;
    float DebugAngY;
    float D[7];
    int OnGround;
    int PlatformId;
    struct nuvec_s PlatformNormal;
    float ShadowY;
    int SurfaceType;
    int TrailSurfaceType;
    int BigDrop;
    int Embedded;
    int Rock;
    int RockNum;
    float CrunchY;
    float DestCrunchY;
    short * TerrHandle;
    float BoostTimer;
    struct nuvec_s InterestPoint;
};

struct creature_s {
    char used;
    char on;
    char off_wait;
    char i_aitab;
    struct obj_s obj;
    struct AI_tmp ai;
    struct NEWBUGGY * Buggy;
    struct creatcmd_s * cmdtable;
    struct creatcmd_s * cmdcurr;
    struct Moveinfo_s * OnFootMoveInfo;
    struct numtx_s m;
    struct numtx_s mtxLOCATOR[16][2];
    struct nuvec_s momLOCATOR[16][2];
    struct Nearest_Light_s lights;
    struct rumble_s rumble;
    float idle_time;
    float idle_wait;
    short idle_action;
    short old_idle_action;
    char idle_mode;
    char idle_repeat;
    char jump;
    char jump_type;
    char jump_subtype;
    char ok_slam;
    char slam;
    char spin;
    char crawl;
    char crawl_lock;
    char tiptoe;
    char sprint;
    uchar somersault;
    uchar land;
    char pad_type;
    char jump_hack;
    uchar jump_hold;
    uchar allow_jump;
    short jump_frames;
    short jump_frame;
    short slam_wait;
    short spin_frames;
    short spin_frame;
    short spin_wait;
    short slide;
    short crouch_pos;
    ushort slam_frame;
    short fire_action;
    uchar fire;
    uchar tap;
    char target;
    char target_wait;
    char fire_lock;
    char idle_sigh;
    uchar hit_type;
    uchar freeze;
    char anim_processed;
    char pad1;
};

struct NUJOINTANIM_s {
    float rx;
    float ry;
    float rz;
    float tx;
    float ty;
    float tz;
    float sx;
    float sy;
    float sz;
    short max_rx;
    short max_ry;
    short max_rz;
    short min_rx;
    short min_ry;
    short min_rz;
    uchar joint_id;
    uchar flags;
    uchar pad[2];
};

struct NUHGOBJ_s {
    short * tids;
    int numtid;
    struct numtl_s * * mtls;
    int nummtl;
    struct NUJOINTDATA_s * joints;
    struct numtx_s * T;
    struct numtx_s * INV_WT;
    uchar * joint_ixs;
    struct NULAYERDATA_s * layers;
    struct NUPOINTOFINTEREST_s * points_of_interest;
    uchar * poi_ixs;
    char * string_table;
    int string_table_size;
    float tbrdist;
    struct NUSHADOWDATA_s * shadow_data;
    float sphere_radius;
    float sphere_yoff;
    struct nuvec_s min;
    struct nuvec_s max;
    float cylinder_yoff;
    float cylinder_height;
    float cylinder_radius;
    struct NUCOLLISIONDATA_s * collision_data;
    int numtexanims;
    struct nutexanim_s * texanims;
    short * texanim_tids;
    uchar num_joints;
    uchar num_joint_ixs;
    uchar num_layers;
    uchar num_points_of_interest;
    uchar num_poi_ixs;
    uchar shadowoff;
    char pad[2];
};

struct NUJOINTDATA_s {
    struct numtx_s orient;
    struct nuvec_s locator_offset;
    char * name;
    int parent_ix;
    char flags;
    char pad[11];
};

struct NUCYLINDERS_s {
    struct nuvec_s centre;
    struct nuvec_s y_axis;
    struct nuvec4_s x_axis;
    struct nuvec4_s z_axis;
    char pad[8];
};

typedef struct Award Award, *PAward;

struct Award {
    float time;
    ushort yrot;
    ushort got;
    char level;
    char wait;
    char pad1;
    char pad2;
    struct nuvec_s oldpos0;
    struct nuvec_s oldpos1;
    struct nuvec_s newpos;
};

typedef struct BATTLESHIPSTRUCT BATTLESHIPSTRUCT, *PBATTLESHIPSTRUCT;

struct BATTLESHIPSTRUCT {
    int Active;
    struct MYDRAW MainDraw;
    struct numtx_s Locator[16];
    struct nuvec_s Position;
    float AngleY;
    float TiltX;
    float TiltZ;
    float DestTiltX;
    float DestTiltZ;
    float DestY;
    float Seek;
    int Seen;
    int HitPoints;
    float FireTimer[2];
    struct nuvec_s GooScale;
    struct nuvec_s DestGooScale;
    struct nuvec_s BaseGooScale;
    float GooSpeed;
    struct nuvec_s GooTimer;
    int KillMeNow;
};

typedef struct BlockInfo BlockInfo, *PBlockInfo;

typedef struct nuiffhdr_s nuiffhdr_s, *Pnuiffhdr_s;

struct nuiffhdr_s {
    int blk;
    int size;
};

struct BlockInfo {
    struct nuiffhdr_s hdr;
    int pos;
};

typedef struct bridge bridge, *Pbridge;

struct bridge {
    struct nuvec_s pos[2][24];
    struct nuvec_s vel[2][24];
    struct numtx_s mtx[24];
    struct nuinstance_s * instance[24];
    struct nuinstance_s * ipost;
    struct nuvec_s center;
    float radius;
    short plat[24];
    char inrange;
    char onscreen;
    char sections;
    char postint;
    int colour;
    short hit;
    short yang;
    float width;
    float tension;
    float gravity;
    float damp;
    float plrweight;
    float postw;
    float posth;
};

typedef struct BugArea_s BugArea_s, *PBugArea_s;

struct BugArea_s {
    char in_pad;
    char in_iRAIL;
    short in_iALONG;
    float in_fALONG;
    char out_pad;
    char out_iRAIL;
    short out_iALONG;
    float out_fALONG;
};

typedef struct BUGSAVE BUGSAVE, *PBUGSAVE;

struct BUGSAVE {
    struct Quat Orientation;
    short x;
    short y;
    short z;
    short FrontWheelAng;
    float WheelLength; /* unk type */
};

typedef struct CamMtx CamMtx, *PCamMtx;

struct CamMtx {
    struct numtx_s m;
    struct numtx_s m0;
    struct nuvec_s vX;
    struct nuvec_s vY;
    struct nuvec_s vZ;
    struct nuvec_s pos;
    struct nuvec_s oldpos;
    struct nuvec_s newpos;
    struct nuvec_s pos2;
    struct nuvec_s seek;
    float distance;
    float ahead;
    float judder;
    float blend_time;
    float blend_duration;
    int xrot;
    int yrot;
    int zrot;
    ushort old_xrot;
    ushort new_xrot;
    ushort old_yrot;
    ushort new_yrot;
    ushort old_zrot;
    ushort new_zrot;
    ushort hdg_to_player;
    short route;
    short i0;
    short i1;
    char mode;
    char iRAIL;
    short iALONG;
    ushort angle;
    char vertical;
    char pad1;
};

typedef struct cdata_s cdata_s, *Pcdata_s;

struct cdata_s { /* CharacterData */
    char * path;
    char * file;
    char * name;
    struct AnimList * anim;
    float radius;
    struct nuvec_s min;
    struct nuvec_s max;
    float scale;
    float shadow_scale;
};

typedef struct chars chars, *Pchars;

struct chars {
    struct NUHGOBJ_s * obj;
    char * path;
    char * name;
};

typedef struct Chase Chase, *PChase;

typedef struct event event, *Pevent;

struct event {
    struct nugspline_s * spl;
    struct nuhspecial_s obj[24];
};

struct Chase {
    struct nugspline_s * spl_START;
    float time;
    float duration;
    struct nugspline_s * spl_CHASER[6];
    struct nuvec_s pos[6];
    struct RPos_s RPos[6];
    struct anim_s anim[6];
    struct nuhspecial_s obj[6];
    short character[6];
    short action[6];
    ushort xrot[6];
    ushort yrot[6];
    ushort zrot[6];
    struct event event[24][6];
    struct nugspline_s * spl_MISC[4][6];
    struct Nearest_Light_s lights[6];
    float scale[6];
    uchar misc_phase[6];
    char ok[6];
    uchar cuboid;
    char i;
    char i_last;
    char i_next;
    char status;
    char iRAIL;
    short iALONG;
    float fALONG;
};

typedef struct CocoMoveInfo CocoMoveInfo, *PCocoMoveInfo;

struct CocoMoveInfo {
    float IDLESPEED;
    float TIPTOESPEED;
    float WALKSPEED;
    float RUNSPEED;
    float SPRINTSPEED;
    float SLIDESPEED;
    float CRAWLSPEED;
    float DANGLESPEED;
    float WADESPEED;
    float JUMPHEIGHT;
    float DANGLEGAP;
    short JUMPFRAMES0;
    short JUMPFRAMES1;
    short JUMPFRAMES2;
    short STARJUMPFRAMES;
    short SOMERSAULTFRAMES;
    short SPINFRAMES;
    short SPINRESETFRAMES;
    short SUPERSPINFRAMES;
    short SUPERSPINWAITFRAMES;
    short SLAMWAITFRAMES;
    short SLIDEFRAMES;
    short CROUCHINGFRAMES;
    short JUMPLANDFRAMES;
    short spad;
};

typedef struct colslot colslot, *Pcolslot;

struct colslot {
    float t;
    float r;
    float g;
    float b;
};

typedef struct Cradat Cradat, *PCradat;

typedef struct crateA crateA, *PcrateA;

typedef struct crate_s crate_s, *Pcrate_s;

struct crate_s {
    int id;
    char type[4];
    struct nuvec_s pos;
    struct crate_s * linked;
    struct crate_s * trigger;
    ushort orientation;
    short offx;
    short offy;
    short offz;
    short ccindex;
    char draw;
    char cpad1;
};

struct Cradat {
    struct crateA * crate;
    char type1;
    char type2;
    char type3;
    char type4;
};

struct crateA {
    struct crate_s * model;
    struct nuvec_s pos0;
    struct nuvec_s pos;
    float oldy;
    float shadow;
    float mom;
    float timer;
    float duration;
    char on;
    char iRAIL;
    short iALONG;
    float fALONG;
    ushort flags;
    char type1;
    char type2;
    char type3;
    char type4;
    char newtype;
    char subtype;
    char i;
    char metal_count;
    char appeared;
    char in_range;
    short dx;
    short dy;
    short dz;
    short iU;
    short iD;
    short iN;
    short iS;
    short iE;
    short iW;
    short trigger;
    char counter;
    char anim_cycle;
    short index;
    float anim_time;
    float anim_duration;
    float anim_speed;
    ushort xrot0;
    ushort zrot0;
    ushort xrot;
    ushort zrot;
    ushort surface_xrot;
    ushort surface_zrot;
    short character;
    short action;
    struct nuvec_s colbox[2];
};

typedef struct CrashMoveInfo CrashMoveInfo, *PCrashMoveInfo;

struct CrashMoveInfo {
    float IDLESPEED;
    float TIPTOESPEED;
    float WALKSPEED;
    float RUNSPEED;
    float SPRINTSPEED;
    float SLIDESPEED;
    float CRAWLSPEED;
    float DANGLESPEED;
    float WADESPEED;
    float JUMPHEIGHT;
    float DANGLEGAP;
    short JUMPFRAMES0;
    short JUMPFRAMES1;
    short JUMPFRAMES2;
    short STARJUMPFRAMES;
    short SOMERSAULTFRAMES;
    short SPINFRAMES;
    short SPINRESETFRAMES;
    short SUPERSPINFRAMES;
    short SUPERSPINWAITFRAMES;
    short SLAMWAITFRAMES;
    short SLIDEFRAMES;
    short CROUCHINGFRAMES;
    short JUMPLANDFRAMES;
    short spad;
};

typedef struct CrateGroup_s CrateGroup_s, *PCrateGroup_s;

struct CrateGroup_s {
    struct nuvec_s origin;
    float radius;
    short iCrate;
    short nCrates;
    ushort angle;
    char pad1;
    char pad2;
    struct nuvec_s minclip;
    struct nuvec_s maxclip;
};

typedef struct Credit_s Credit_s, *PCredit_s;

struct Credit_s {
    char * txt;
    short colour;
    short size;
};

typedef struct csc_s csc_s, *Pcsc_s;

struct csc_s {
    struct NUHGOBJ_s * obj;
    char * path;
    char * name;
};

typedef union currLst currLst, *PcurrLst;

typedef struct nulnkhdr_s nulnkhdr_s, *Pnulnkhdr_s;

typedef struct nulsthdr_s nulsthdr_s, *Pnulsthdr_s;

union currLst {
    char * s8;
    struct nulnkhdr_s * lhdr;
};

struct nulsthdr_s {
    struct nulnkhdr_s * free;
    struct nulnkhdr_s * head;
    struct nulnkhdr_s * tail;
    short elcnt;
    short elsize;
};

struct nulnkhdr_s {
    struct nulsthdr_s * owner;
    struct nulnkhdr_s * succ;
    struct nulnkhdr_s * prev;
    ushort id;
    ushort used;
};

typedef struct Cursor Cursor, *PCursor;

typedef struct remember remember, *Premember;

struct remember {
    char x;
    char y;
};

struct Cursor {
    struct remember remember[48];
    char unk_60[8];
    uint menu_frame;
    uint item_frame;
    char x;
    char y;
    char x_min;
    char y_min;
    char x_max;
    char y_max;
    char menu; /* 0x76 */
    char new_menu; /* 0x77 */
    char new_level; /* 0x78 */
    uchar wait; /* 0x79 */
    uchar wait_frames; /* 0x7a */
    char wait_hack; /* 0x7b */
    char button_lock; /* 0x7c */
    char pad1;
    char pad2;
    char pad3;
};

typedef struct cutscenedesc_s cutscenedesc_s, *Pcutscenedesc_s;

struct cutscenedesc_s {
    char * gscname;
    char * cutname;
    int sfxid;
    int debgroup;
};

typedef struct D3DBaseTexture D3DBaseTexture, *PD3DBaseTexture;

struct D3DBaseTexture {
    int Common;
    int Data;
    int Lock;
    int Format;
    int Size;
};

typedef struct D3DDevice D3DDevice, *PD3DDevice;

struct D3DDevice {
    undefined field0_0x0;
};

typedef struct D3DSurface D3DSurface, *PD3DSurface;

struct D3DSurface {
    int Common;
    int Data;
    int Lock;
    int Format;
    int Size;
    struct D3DSurface * Parent;
};

typedef struct D3DTexture D3DTexture, *PD3DTexture;

struct D3DTexture {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
};

typedef struct D3DXVECTOR2 D3DXVECTOR2, *PD3DXVECTOR2;

struct D3DXVECTOR2 {
    float x;
    float y;
};

typedef struct datasetup_s datasetup_s, *Pdatasetup_s;

struct datasetup_s {
    struct nuvec_s vt[3];
    int colour;
};

typedef struct DatFile DatFile, *PDatFile;

typedef struct nudatinfo_s nudatinfo_s, *Pnudatinfo_s;

typedef struct nudfnode_s nudfnode_s, *Pnudfnode_s;

typedef int int32_t;

typedef int32_t filehandle;

struct DatFile {
    int ver;
    int nfiles;
    struct nudatinfo_s * finfo;
    int treecnt;
    struct nudfnode_s * filetree;
    int leafnames;
    int dfhandle;
    filehandle fh;
    short intalloc;
    short openmode;
    int start_lsn;
    void * memdataptr;
    char pad[4];
};

struct nudatinfo_s {
    int foffset;
    int flen;
    int uplen;
    int ppack;
};

struct nudfnode_s {
    short childix;
    short sibix;
    char * txt;
};

typedef struct deb3_s deb3_s, *Pdeb3_s;

typedef struct deb3info deb3info, *Pdeb3info;

struct deb3info {
    short type;
    short classid;
    short info;
    float timer;
    float size;
    short deb;
    short rate;
    void * impact; /* void(*impact)(deb3_s*) */
    void * end; /* void(*end)(deb3_s*) */
    int data;
};

struct deb3_s {
    struct numtx_s mtx;
    struct numtx_s invWorldInertiaTensor;
    struct nuvec_s velocity;
    struct nuvec_s angularvelocity;
    struct nuvec_s angularMomentum;
    struct nuvec_s impact;
    struct nuvec_s norm;
    struct nuvec_s diff;
    float shadow;
    float grav;
    short status;
    short timer;
    short check;
    short count;
    struct deb3info * info;
    int data;
};

typedef struct debkeydatatype_s debkeydatatype_s, *Pdebkeydatatype_s;

typedef struct rdata_s rdata_s, *Prdata_s;

typedef struct gensorttab gensorttab, *Pgensorttab;

typedef struct debrissphere_s debrissphere_s, *Pdebrissphere_s;

typedef struct debris_s debris_s, *Pdebris_s;

struct debrissphere_s {
    struct nuvec_s emit;
    float t;
};

struct debkeydatatype_s {
    struct rdata_s * chunks[128];
    struct numtx_s rotmtx;
    struct numtx_s emitrotmtx;
    short count;
    short type;
    short active;
    short debcount;
    short disposed;
    short pointer;
    short reqcount;
    short reqdebcount;
    float x;
    float y;
    float z;
    float toffx;
    float toffy;
    float toffz;
    struct gensorttab * genptr;
    void * gencode;
    struct debkeydatatype_s * next;
    struct debkeydatatype_s * prev;
    short rotory;
    short rotorz;
    int instances;
    short oncount;
    short delay;
    struct debrissphere_s spheres[8];
    short sphere_next;
    short sphere_next_emit;
    int trigger_type;
    int trigger_id;
    float trigger_var;
    short refrotz;
    short refroty;
    float refoff;
    float refbounce;
    short sound_next[4];
    short group_id;
    char unk[2];
};

struct gensorttab {
    float x;
    float y;
    float z;
    float time;
    float mx;
    float my;
    float mz;
    float etime;
};

struct debris_s {
    float x;
    float y;
    float z;
    float time;
    float mx;
    float my;
    float mz;
    float etime;
};

struct rdata_s {
    int dmadata[2];
    int unpackdata[2];
    struct debris_s debris[32];
};

typedef enum debris_chunk_control_e {
    DEBRIS_CHUNK_CONTROL_FREE=0,
    DEBRIS_CHUNK_CONTROL_FREE_AND_UNLINK=1,
    DEBRIS_CHUNK_CONTROL_SORT_DMA=2,
    DEBRIS_CHUNK_CONTROL_RETURN_TO_STACK=3,
    DEBRIS_CHUNK_CONTROL_CHANGE_COLOUR=4,
    DEBRIS_CHUNK_CONTROL_DO_BOUNCEY=5,
    DEBRIS_CHUNK_CONTROL_DO_BOUNCEXZ=6
} debris_chunk_control_e;

typedef struct debris_chunk_control_s debris_chunk_control_s, *Pdebris_chunk_control_s;

struct debris_chunk_control_s {
    struct rdata_s * chunk;
    int delay;
    enum debris_chunk_control_e action;
    struct debkeydatatype_s * owner;
    struct debris_chunk_control_s * next;
    int ivariable;
    float fvariable;
    short type;
    short refrotz;
    short refroty;
    float refoff;
    float refbounce;
};

typedef struct debtab debtab, *Pdebtab;

typedef struct wslot wslot, *Pwslot;

typedef struct hslot hslot, *Phslot;

typedef struct rotslot rotslot, *Protslot;

typedef struct jibxslot jibxslot, *Pjibxslot;

typedef struct jibyslot jibyslot, *Pjibyslot;

typedef struct setup_s setup_s, *Psetup_s;

typedef struct sphereslot sphereslot, *Psphereslot;

typedef struct sounds sounds, *Psounds;

struct sphereslot {
    float t;
    float r;
};

struct hslot {
    float t;
    float h;
};

struct jibyslot {
    float t;
    float y;
};

struct sounds {
    int id;
    int type;
    int delay;
};

struct wslot {
    float t;
    float w;
};

struct jibxslot {
    float t;
    float x;
};

struct rotslot {
    float t;
    float r;
};

struct debtab {
    char id[16];
    char listid;
    char pad1_unk;
    short generate;
    short debnum;
    short ival_offset;
    short ival_on;
    short ival_on_ran;
    short ival_off;
    short ival_off_ran;
    char gensort;
    char gencode;
    char mat;
    char efftab;
    float radius;
    float cuton;
    float cutoff;
    float drawcutoff;
    float emitmag;
    struct nuvec_s variable_emit;
    struct nuvec_s variable_emit_ranscale;
    struct nuvec_s variable_start;
    struct nuvec_s variable_start_ranscale;
    float rsx;
    float rsy;
    float rsz;
    float osx;
    float osy;
    float osz;
    float rvx;
    float rvy;
    float rvz;
    float ovx;
    float ovy;
    float ovz;
    float grav;
    float etime;
    short uw;
    char frames;
    char rate;
    float uvanim;
    float jibxfreq;
    float jibxamp;
    float jibyfreq;
    float jibyamp;
    struct colslot colslot[8];
    struct alpslot alpslot[8];
    float minsize;
    float maxsize;
    struct wslot wslot[8];
    struct hslot hslot[8];
    float minrot;
    float maxrot;
    struct rotslot rotslot[8];
    struct jibxslot jibxslot[8];
    struct jibyslot jibyslot[8];
    float u1;
    float v1;
    float u2;
    float v2;
    struct setup_s * DmaDebTypePointer;
    struct sphereslot sphereslot[8];
    char numspheres;
    char pad2_unk[3];
    int variable_key;
    struct sounds sounds[4];
};

struct setup_s {
    int DmaHeader[4];
    float grav;
    float gtime;
    int DmaBody[4];
    float u0;
    float v0;
    float u1;
    float v1;
    float u2;
    float v2;
    float u3;
    float v3;
    struct datasetup_s data[64];
};

typedef struct DmaDebTypePointer DmaDebTypePointer, *PDmaDebTypePointer;

struct DmaDebTypePointer {
    int DmaHeader[4];
    float grav;
    float gtime;
    int DmaBody[4];
    float u0;
    float v0;
    float u1;
    float v1;
    float u2;
    float v2;
    float u3;
    float v3;
    struct datasetup_s data[64];
};

typedef struct DVDCommandBlock DVDCommandBlock, *PDVDCommandBlock;

struct DVDCommandBlock {
    struct DVDCommandBlock * next;
    struct DVDCommandBlock * prev;
    uint command;
    int state;
    uint offset;
    uint length;
    void * addr;
    uint currTransferSize;
    uint transferredSize;
    struct DVDDiskID * id;
    struct DVDCommandBlock * DVDCommandBlock; /* void(*callback)(int, DVDCommandBlock*) */
    void * userData;
};


/* WARNING! conflicting data type names: /auto_structs/DVDDiskID - /auto_structs/GAMECUBE_SDK/DVDDiskID */

typedef struct DVDFileInfo DVDFileInfo, *PDVDFileInfo;

struct DVDFileInfo {
    struct DVDCommandBlock cb;
    uint startAddr;
    uint length;
    struct DVDFileInfo * DVDFileInfo;
};

typedef enum eCrashVehicleType {
    Submarine=32,
    Glider=54,
    Gyro=59,
    Mech=68,
    Atlasphere=83,
    Jeep=99,
    Scooter=107,
    Dropship=129,
    Minecart=137,
    Mosquito=139,
    Offroader=153,
    Snowboard=160,
    Minetub=161,
    FireEngine=178
} eCrashVehicleType;

typedef struct edpp_ptldesc_s edpp_ptldesc_s, *Pedpp_ptldesc_s;

struct edpp_ptldesc_s {
    struct nuvec_s pos;
    int type;
    int handle;
    short rotz;
    short roty;
    short emitrotz;
    short emitroty;
    int offset;
    char name[16];
    int trigger_type;
    int trigger_id;
    float trigger_var;
    short refrotz;
    short refroty;
    float refoff;
    float refbounce;
    short group_id;
    char unk[2];
};

typedef struct EnemyJeep EnemyJeep, *PEnemyJeep;

typedef struct SIMWHEEL SIMWHEEL, *PSIMWHEEL;

struct SIMWHEEL {
    struct nuvec_s Position;
    struct nuvec_s OldPosition;
    float TrailWidth;
    float Radius;
    int Platform;
    int SurfaceType;
};

struct EnemyJeep {
    struct nuvec_s Position;
    struct MYDRAW Draw;
    float PlayBackTime;
    int PlayBackMax;
    struct NUJOINTANIM_s Joints[8];
    struct numtx_s Locators[16];
    int LocatorValidFrame;
    struct SIMWHEEL TrailWheel[4];
    float TimeLine;
    char Active;
    char DrawOn;
    char TrailOn;
    char Pad;
};

typedef struct fileinfo_s fileinfo_s, *Pfileinfo_s;

struct fileinfo_s {
    filehandle handle;
    int read_pos;
    int file_pos;
    int file_length;
    int buff_start;
    int buff_length;
    int use_buff;
    int buff;
};

typedef struct FIREBOSSSTRUCT FIREBOSSSTRUCT, *PFIREBOSSSTRUCT;

typedef struct MYSPLINE MYSPLINE, *PMYSPLINE;

typedef struct JEEPROCK JEEPROCK, *PJEEPROCK;

struct MYSPLINE {
    struct nugspline_s * Spline;
    float Cur;
    float Nex;
    float Act;
    float Inc;
    struct nuvec_s CurPos;
    struct nuvec_s NexPos;
    float LookaheadDist;
};

struct FIREBOSSSTRUCT {
    struct numtx_s Locator[16];
    int DropSide;
    float FootTime;
    int HitPoints;
    int Pass;
    int Active;
    float AngleY;
    struct nuvec_s Position;
    struct MYDRAW MainDraw;
    struct MYDRAW ExplodeDraw;
    struct MYSPLINE MainSpline;
    int Seen;
    float Speed;
    int Action;
    int LastAction;
    float ActionTimer;
    float ActionTimer2;
    struct nuvec_s Vel;
    int Dead;
    struct numtx_s DrawMtx;
    int BeenHitThisPass;
    float BeenHitThisPassTimer;
    struct JEEPROCK * Rock;
};

struct JEEPROCK {
    struct nuvec_s Pos;
    struct nuvec_s Vel;
    int Active;
    int Seen;
    int Stuck;
    int Explode;
    int SmallDamage;
    float Life;
    struct ATLASSTRUCT Atlas;
    int Grabbed;
    int Mode;
    float FlameTimer;
    struct nuvec_s Scale;
    int FireBlob;
    int SmashMe;
};

typedef struct Font3DAccentTab Font3DAccentTab, *PFont3DAccentTab;

struct Font3DAccentTab {
    struct nuhspecial_s obj;
    char * name;
};

typedef struct Font3DObjTab Font3DObjTab, *PFont3DObjTab;

struct Font3DObjTab {
    short i;
    uchar flags;
    char action;
    float anim_time;
    float scale;
};

typedef struct Font3DTab Font3DTab, *PFont3DTab;

struct Font3DTab {
    char ascii;
    char pad1;
    char pad2;
    char pad3;
    struct nuhspecial_s obj;
    char * name;
};

typedef struct FootData FootData, *PFootData;

struct FootData {
    struct nuvec_s pnts[4];
    short brightness;
    char timer;
    char gfx;
};

typedef int fxi;

typedef struct Game_s Game_s, *PGame_s;

typedef struct Hub Hub, *PHub;

typedef struct level level, *Plevel;

typedef struct time time, *Ptime;

struct Hub {
    uchar flags;
    uchar crystals;
    char pad1;
    char pad2;
};

struct time {
    char name[4];
    uint itime;
};

struct level {
    ushort flags;
    char pad1;
    char pad2;
    struct time time[3];
};

struct Game_s {
    char name[9];
    uchar vibration;
    uchar surround;
    uchar sfx_volume;
    uchar music_volume;
    char screen_x;
    char screen_y;
    uchar language;
    struct Hub hub[6];
    struct level level[35];
    uchar lives;
    uchar wumpas;
    uchar mask;
    uchar percent;
    uchar crystals;
    uchar relics;
    uchar crate_gems;
    uchar bonus_gems;
    uchar gems;
    uchar gembits;
    uchar powerbits;
    uchar empty;
    uint cutbits;
    uchar year;
    uchar month;
    uchar day;
    uchar hours;
    uchar mins;
    uchar pad_[3];
};

typedef struct GDeb GDeb, *PGDeb;

struct GDeb {
    int i;
    char * name;
    longlong levbits;
};

typedef struct ghg_inst_s ghg_inst_s, *Pghg_inst_s;

struct ghg_inst_s {
    char * name;
    struct NUHGOBJ_s * scene;
};

typedef struct GLIDERSTRUCT GLIDERSTRUCT, *PGLIDERSTRUCT;

struct GLIDERSTRUCT {
    struct creature_s * Cre;
    struct nuvec_s vel;
    int Dead;
    int CocoDead;
    float CocoDeadTimer;
    float CocoDeathSpinX;
    float CocoDeathSpinZ;
    float NextEngRum;
    float FixVelTimer;
    float ImmuneAsteroidsTimer;
    struct nuvec_s Position;
    struct nuvec_s OldPosition;
    struct nuvec_s Velocity;
    struct nuvec_s Resolved;
    struct nuvec_s RailPoint;
    float RailAngle;
    float TiltX;
    float TiltZ;
    float DestTiltX;
    float DestTiltZ;
    float AngleY;
    float CamAngleY;
    float CamTiltX;
    float CamTornRecoverTimer;
    float InputX;
    float InputZ;
    int BarrelRoll;
    float BarrelDelta;
    float BarrelSpeedX;
    int TerminalDive;
    int TerminalDir;
    float FireTimer;
    float HitTimer;
    int AutoPilot;
    int ForceTurn;
    int HitPoints;
    char LocatorList[16];
    float LocatorTime[16];
    int InTornado;
    int LastInTornado;
    float InTornadoTime;
    float TornadoSpin;
    float InTornadoScale;
    struct nuvec_s ApparentPosition;
    struct nuvec_s ApparentVelocity;
    struct nuvec_s PositionStack[30];
    struct nuvec_s VelocityStack[30];
    int StackIndx;
    float Speed;
    float TargetSpeed;
    float NotInFrontTimer;
    float PullUpTimer;
    float OverideTiltZ;
    float TargetTimer;
    struct nuvec_s * MovingTargetPoint;
    struct nuvec_s * MovingTargetVel;
    int TargetOn;
    float TargetedTime;
    int TargetMoving;
    float NextHitSoundTimer;
};

typedef struct GlobalTimer GlobalTimer, *PGlobalTimer;

struct GlobalTimer {
    uint frame;
    uint itime;
    uint isec;
    float ftime;
    float fsec;
};

typedef struct glowlight glowlight, *Pglowlight;

struct glowlight {
    struct nucolour3_s HighColour;
    struct nucolour3_s LowColour;
    struct nucolour3_s CurColour;
    struct nucolour3_s ColDiff;
    ushort Step;
    ushort CurAngle;
};

typedef struct GS_Buffer GS_Buffer, *PGS_Buffer;

struct GS_Buffer {
    uint size;
    uint type;
};

typedef struct H2OElec H2OElec, *PH2OElec;

struct H2OElec {
    struct nuvec_s start;
    struct nuvec_s end;
    int time;
    int ang;
};

typedef struct hitdata hitdata, *Phitdata;

struct hitdata {
    float minx;
    float maxx;
    float miny;
    float maxy;
    float minz;
    float maxz;
    struct nuvec_s pnts[4];
    struct nuvec_s norm[2];
    uchar info[4];
};

typedef struct JEEPBALLOON JEEPBALLOON, *PJEEPBALLOON;

struct JEEPBALLOON {
    struct nuvec_s Pos;
    struct nuvec_s Vel;
    int Active;
    int Seen;
    int Explode;
    int SmallDamage;
    float Life;
    short AngY;
    short unk;
};

typedef struct JEEPSTRUCT JEEPSTRUCT, *PJEEPSTRUCT;

typedef struct POINTANG POINTANG, *PPOINTANG;

typedef struct VEHICLE VEHICLE, *PVEHICLE;

struct POINTANG {
    float x;
    float y;
    float z;
    int Ang;
};

struct VEHICLE {
    struct nuvec_s ActualWheelPosition[4];
    struct nuvec_s OldWheelPosition[4];
    int BigSpin[4];
    struct nuvec_s ActualPosition;
    struct nuvec_s Resolved;
    struct nuvec_s Velocity;
    struct nuvec_s WheelAxis[3];
    float FrontWheelSpeedAdj;
    short aTargetAngle;
    short aTarSurfRotX;
    short aTarSurfRotZ;
    short aActualAngle;
    short aActSurfRotX;
    short aActSurfRotZ;
    short ActFrontRotX;
    short ActRearRotX;
    short TarFrontRotX;
    short TarRearRotX;
    int AnyOnGroundBits;
    int AllOnGroundBits;
    int AllTouchingGroundBits;
    int AnyTouchingGroundBits;
    struct nuvec_s AirNormal;
    struct nuvec_s SurfaceNormal;
    short * TerrHandle;
    int FrontWheelGroundBits;
};

struct JEEPSTRUCT {
    struct creature_s * Cre;
    struct MYDRAW ChassisDraw;
    struct numtx_s ChassisLocators[16];
    struct numtx_s DrawMtx;
    struct NUJOINTANIM_s Joints[8];
    struct SIMWHEEL TrailWheel[4];
    struct POINTANG RestartPoint;
    float DownHoleTimer;
    int DownHole;
    int Dropped;
    struct nuvec_s RestartCamPos;
    struct nuvec_s RestartCamObj;
    float FireBossTurnTimer;
    float WheelHeight[4];
    float TimeLine;
    int FireBossDir;
    int CantMove;
    int Quick;
    float MaxSpeed;
    float MySpeed;
    float DefaultSpeed;
    float StartSpeed;
    float StartSpeedTimer;
    int Active;
    short aWRot[4];
    short aFrontWheelAng;
    short aOldFrontWheelAng;
    struct nuvec_s Pos;
    short aAngleY;
    short aMovementAng;
    short aSurfRotX;
    short aSurfRotZ;
    float TiltSeekTime;
    short aTiltX;
    short aTiltZ;
    short aDestTiltX;
    short aDestTiltZ;
    short aInputAng;
    char unk1[2];
    float InputSpeed;
    int WheelSpin;
    short aDeltaAng;
    short aLastDeltaAng;
    short aLastDeltaAngA;
    short aOldChassisAngleY;
    short aChassisAngleY;
    short aChassisTargetAngleY;
    int aChassisAngMom;
    float Accelerator;
    float AccelerationForce;
    float CentrefugalForce;
    float Traction;
    float GroundTractionAcc;
    float TurnSin;
    short aBaseMoveAng;
    char unk2[2];
    float CentRailDist;
    float BoostOnTimer;
    float BoostTimer;
    int Finished;
    float CarryOnRecordTime;
    float FloorHeight;
    int TerrainType;
    struct MYSPLINE Spline;
    float FireTimer;
    struct VEHICLE Move;
};

typedef struct JeepTrail JeepTrail, *PJeepTrail;

struct JeepTrail {
    struct nuvec_s pos1;
    struct nuvec_s pos2;
    int intensity;
    int RealIntensity;
};

typedef struct Kaboom Kaboom, *PKaboom;

struct Kaboom {
    ushort type;
    short i;
    struct nuvec_s pos;
    float radius0;
    float radius1;
    float time;
    float duration;
    ushort yrot;
    char pad2;
    char pad3;
};

typedef struct LDATA_s LDATA_s, *PLDATA_s;

typedef struct pCHASE pCHASE, *PpCHASE;

typedef struct pSFX pSFX, *PpSFX;

struct pSFX {
    char name[16];
    ushort pitch;
    ushort volume;
    uchar buzz;
    uchar rumble;
    uchar delay;
    uchar wait;
    char * path;
    ushort frequency;
    ushort stream;
    char type;
    char pad1;
    ushort id;
    struct nuvec_s Pos;
};

struct pCHASE {
    char i;
    char i_last;
    char i_next;
    uchar cuboid;
    short character[6];
    short action[6];
    float scale[6];
    float duration;
};

struct LDATA_s {
    char * filepath;
    uchar * cList; /* modelList */
    struct pCHASE * pChase;
    uint time[3];
    short music[2];
    struct pSFX * pSFX;
    short nSFX;
    char pad1;
    char hub;
    ushort flags;
    short character;
    short vehicle;
    ushort farclip;
    struct nuvec_s vSTART;
    struct nuvec_s vBONUS;
    float fognear;
    float fogfar;
    uchar fogr;
    uchar fogg;
    uchar fogb;
    uchar foga;
    uchar hazer;
    uchar hazeg;
    uchar hazeb;
    uchar hazea;
    int totalsize;
};

typedef struct LgtLaserData LgtLaserData, *PLgtLaserData;

struct LgtLaserData { /* NuLgtArcLaserData */
    struct nuvec_s start;
    struct nuvec_s target;
    struct nuvec_s lasdir;
    float sizew;
    float sizel;
    float sizewob;
    float arcsize;
    uchar type;
    uchar p1;
    uchar p2;
    uchar p3;
    int col;
    int seed;
};

typedef struct Lights Lights, *PLights;

struct Lights {
    int type;
    struct nuvec_s pos;
    struct nuvec_s radius_pos;
    float radius;
    uchar r;
    uchar g;
    uchar b;
    uchar glow;
    struct nucolour3_s colour;
    struct nuvec_s direction;
    uchar globalflag;
    uchar blendtype;
    uchar brightness;
    uchar pad1;
    struct glowlight * glowlight;
    char pad2[4];
};

typedef struct MaskFeathers MaskFeathers, *PMaskFeathers;

struct MaskFeathers {
    struct numtx_s mM;
    struct numtx_s mS;
    float time;
    float duration;
    float shadow;
    char pad1;
    char pad2;
    char pad3;
    char pad4;
};

typedef struct matchingslot_s matchingslot_s, *Pmatchingslot_s;

struct matchingslot_s {
    int batch;
    int slot;
};

typedef struct memexternal_s memexternal_s, *Pmemexternal_s;

typedef union variptr_u variptr_u, *Pvariptr_u;

typedef struct nuivec_s nuivec_s, *Pnuivec_s;

typedef struct nuivec4_s nuivec4_s, *Pnuivec4_s;

typedef struct scegiftag scegiftag, *Pscegiftag;

struct scegiftag {
    longlong NREG:4;
    longlong FLG:2;
    longlong PRIM:11;
    longlong PRE:1;
    longlong id:14;
    longlong pad16:16;
    longlong EOP:1;
    longlong NLOOP:15;
    longlong REGS15:4;
    longlong REGS14:4;
    longlong REGS13:4;
    longlong REGS12:4;
    longlong REGS11:4;
    longlong REGS10:4;
    longlong REGS9:4;
    longlong REGS8:4;
    longlong REGS7:4;
    longlong REGS6:4;
    longlong REGS5:4;
    longlong REGS4:4;
    longlong REGS3:4;
    longlong REGS2:4;
    longlong REGS1:4;
    longlong REGS0:4;
};

struct nuivec4_s {
    int x;
    int y;
    int z;
    int w;
};

struct nuivec_s {
    int x;
    int y;
    int z;
};

union variptr_u {
    void * voidptr;
    uchar * u8;
    ushort * u16;
    short * s16;
    uint * u32;
    longlong * u64;
    longlong * u128;
    float * f32;
    struct nuvec_s * vec3;
    struct nuvec4_s * vec4;
    struct nuivec_s * ivec3;
    struct nuivec4_s * ivec4;
    struct numtx_s * mtx44;
    uint * viftag;
    uint intaddr;
    struct _sceDmaTag * dmatag;
    struct scegiftag * gifTag;
};

struct memexternal_s {
    union variptr_u * ptr;
    union variptr_u * end;
};

typedef struct MemFile MemFile, *PMemFile;

typedef enum nufilemode_e {
    NUFILE_READ=0,
    NUFILE_WRITE=1,
    NUFILE_APPEND=2,
    NUFILE_MODE_CNT=3
} nufilemode_e;

struct MemFile {
    char * start;
    char * end;
    char * currpos;
    enum nufilemode_e mode;
    int used;
};

typedef struct NewWumpa NewWumpa, *PNewWumpa;

struct NewWumpa {
    struct nuvec_s world_pos;
    struct nuvec_s screen_pos;
    float screen_scale;
    uchar count;
    uchar delay;
    char transformed;
    char bonus;
    char active;
    char pad1;
    char pad2;
    char pad3;
};

typedef struct nuangvec_s nuangvec_s, *Pnuangvec_s;

struct nuangvec_s {
    int x;
    int y;
    int z;
};

typedef struct NUANIMDATAHDR_s NUANIMDATAHDR_s, *PNUANIMDATAHDR_s;

struct NUANIMDATAHDR_s {
    int version;
    int address_offset;
    struct nuAnimData_s * animdata;
};

typedef struct NUANIMKEYBIG_s NUANIMKEYBIG_s, *PNUANIMKEYBIG_s;

struct NUANIMKEYBIG_s {
    float time;
    float dtime;
    float val;
    float grad;
};

typedef struct NUANIMKEYINTEGER_s NUANIMKEYINTEGER_s, *PNUANIMKEYINTEGER_s;

struct NUANIMKEYINTEGER_s {
    float val;
    float time;
};

typedef enum NUANIMKEYTYPES_e {
    NUANIMKEYTYPE_NONE=0,
    NUANIMKEYTYPE_BIG=1,
    NUANIMKEYTYPE_SMALL=2,
    NUANIMKEYTYPE_INTEGER=3,
    NUANIMKEYTYPE_BOOLEAN=4
} NUANIMKEYTYPES_e;

typedef struct nuanimtime_s nuanimtime_s, *Pnuanimtime_s;

struct nuanimtime_s {
    float time;
    float time_offset;
    int chunk;
    uint time_mask;
    uint time_byte;
};

typedef struct nucamera_s nucamera_s, *Pnucamera_s;

struct nucamera_s {
    struct numtx_s mtx;
    float fov;
    float aspect;
    float nearclip;
    float farclip;
    uint portalnearclip;
    struct nuvec_s scale;
};

typedef enum nucamfxmode_s {
    NUCAMFX_NONE=0,
    NUCAMFX_REFLECT=1
} nucamfxmode_s;

typedef struct NUCUTSCENE_s NUCUTSCENE_s, *PNUCUTSCENE_s;

typedef struct NUCUTSCENECAMERA_s NUCUTSCENECAMERA_s, *PNUCUTSCENECAMERA_s;

typedef struct NUHINSTANCE_s NUHINSTANCE_s, *PNUHINSTANCE_s;

typedef struct NUCUTSCENEAUDIO_s NUCUTSCENEAUDIO_s, *PNUCUTSCENEAUDIO_s;

struct NUCUTSCENE_s { /* 0x1f13f */
    int * tids;
    int numtid;
    struct numtl_s * mtls;
    int nummtl;
    float length;
    char * string_table;
    struct NUCUTSCENECAMERA_s * cameras[10];
    struct nuAnimData_s * camera_anim;
    float * camera_change_times;
    uchar * camera_changes;
    int numgobj;
    struct nugobj_s * gobjs;
    int numinstance;
    struct nuinstance_s * instances;
    struct nuAnimData_s * instance_animdata;
    int numhgobj;
    struct NUHGOBJ_s * hgobj;
    int numhinstance;
    struct NUHINSTANCE_s * hinstances;
    struct NUCUTSCENEAUDIO_s * audio;
    float current_time;
    int flags;
    char audio_path[64];
    uchar num_camera_changes;
    uchar camera_change_ix;
    uchar camera_ix;
    uchar camera_anim_ix;
    uchar numaudio;
    uchar next_audioix;
    char pad[2];
};

struct NUHINSTANCE_s {
    struct numtx_s mtx;
    struct NUHGOBJ_s * hgobj;
    struct nuAnimData_s * animdata;
    struct nuAnimData_s * blendweightanimdata;
    char pad[4];
};

struct NUCUTSCENEAUDIO_s {
    float start_time;
    char * wav_time;
};

struct NUCUTSCENECAMERA_s {
    struct nucamera_s * camera;
};

typedef struct NuCutSceneCharacterCreateData NuCutSceneCharacterCreateData, *PNuCutSceneCharacterCreateData;

struct NuCutSceneCharacterCreateData {
    struct NUGCUTCHAR_s * field0_0x0;
    struct instNUGCUTCHAR_s * field1_0x4;
    union variptr_u * field2_0x8;
};

typedef struct nudathdr_s nudathdr_s, *Pnudathdr_s;

struct nudathdr_s {
    int ver;
    int nfiles;
    struct nudatinfo_s * finfo;
    int treecnt;
    struct nudfnode_s * filetree;
    int leafnamesize;
    char * leafnames;
    int dfhandle;
    float fh;
    short intalloc;
    short openmode;
    int start_lsn;
    void * memdatptr;
};

typedef struct nufpar_s nufpar_s, *Pnufpar_s;

typedef struct nufpcomjmp_s nufpcomjmp_s, *Pnufpcomjmp_s;

struct nufpcomjmp_s {
    char * fname;
    void (* func)(struct nufpar_s *); /* struct */
};

struct nufpar_s {
    filehandle handle; /* fH */
    uchar buffer[4096]; /* fbuff */
    char textBuffer[256]; /* lbuff */
    char wordBuffer[260];
    int line_num;
    int linepos;
    int cpos;
    int buffstart;
    int buffend;
    struct nufpcomjmp_s * comstack[8];
    int compos;
    int size;
};

typedef struct nufparpos_s nufparpos_s, *Pnufparpos_s;

struct nufparpos_s {
    int line_num;
    int line_pos;
    int cpos;
    int buffstart;
    int buffend;
};

typedef struct nufspecial_s nufspecial_s, *Pnufspecial_s;

struct nufspecial_s {
    struct numtx_s mtx;
    int instanceix;
    int nameix;
    struct nuspecialflags flags;
    int pad;
};

typedef struct NUGCUTLOCFNDATA_s NUGCUTLOCFNDATA_s, *PNUGCUTLOCFNDATA_s;

struct NUGCUTLOCFNDATA_s {
    char * name;
    struct locdataFN * void_fn;
};

typedef struct nulight_s nulight_s, *Pnulight_s;

struct nulight_s {
    struct nucolour3_s ambient;
    struct nucolour3_s diffuse;
    struct numtx_s mtx;
};

typedef struct nulights_s nulights_s, *Pnulights_s;

struct nulights_s {
    struct nulight_s light[3];
};

typedef struct NUNODE_s NUNODE_s, *PNUNODE_s;

struct NUNODE_s {
    char * name;
    uint id;
    uint type;
    int nchildren;
    struct NUNODE_s * parent;
    struct NUNODE_s * child;
    struct NUNODE_s * next;
    void * data;
};

typedef struct nunrand_s nunrand_s, *Pnunrand_s;

struct nunrand_s {
    long idum;
};

typedef struct nuotitem_s nuotitem_s, *Pnuotitem_s;

struct nuotitem_s {
    struct nuotitem_s * next;
    struct nurndritem_s * hdr;
    float dist;
    struct nusysmtl_s * mtl;
};

typedef struct nupad_s nupad_s, *Pnupad_s;

struct nupad_s {
    void * padhandle;
    struct _XINPUT_STATE xinputs;
    struct _XINPUT_STATE old_xinputs;
    struct _XINPUT_FEEDBACK feedback;
    int port;
    int slot;
    int stage;
    int id;
    int ldx;
    int ldy;
    int rdx;
    int rdy;
    float l_nx;
    float l_ny;
    float l_rx;
    float l_ry;
    float l_dx;
    float l_dy;
    float r_nx;
    float r_ny;
    float r_rx;
    float r_ry;
    float r_dx;
    float r_dy;
    uint paddata;
    uint old_paddata;
    uint paddata_db;
    int bit_;
    uint padvalue;
    uchar ok;
    uchar read_id;
    uchar buttons_hi;
    uchar buttons_lo;
    uchar r_alg_x;
    uchar r_alg_y;
    uchar l_alg_x;
    uchar l_alg_y;
    uchar l_algpad_r;
    uchar l_algpad_l;
    uchar l_algpad_u;
    uchar l_algpad_d;
    uchar r_algpad_u;
    uchar r_algpad_r;
    uchar r_algpad_d;
    uchar r_algpad_l;
    uchar l1_alg;
    uchar r1_alg;
    uchar l2_alg;
    uchar r2_alg;
};

typedef struct nureflect_s nureflect_s, *Pnureflect_s;

struct nureflect_s {
    float distortion;
    struct numtx_s mtx;
    struct numtx_s uvmtx;
};

typedef struct NuScene_old NuScene_old, *PNuScene_old;

struct NuScene_old {
    char __padding_0[4];
    char __padding_4[4];
    uint unsnum_8;
    char __padding_C[4];
    int word_10;
    struct nugobj_s * * gobj_ptr;
    int word_18;
    int word_1C;
    int word_20;
    int word_24;
    int word_28;
    int word_2C;
    int word_30;
    int word_34;
    struct NuScene_old * nextmaybe;
    char __padding_3C[8];
    char __padding_44[4];
    struct nuAnimData_s * * AnimData;
    int numAnim;
    char __padding_50[20];
    int word_64;
    int word_68;
    int word_6C;
    char __padding_70[16];
};

typedef struct nuscene_s nuscene_s, *Pnuscene_s;

typedef struct nuspline_s nuspline_s, *Pnuspline_s;

struct nuscene_s { /* Nus */
    int nnodes;
    char * * names;
    struct NUNODE_s * * nodes;
    char * strings;
    int numtids; /* tex_count? */
    short * tids;
    int nummtls;
    struct numtl_s * * mtls;
    int numgobjs;
    struct nugobj_s * * gobjs;
    int numsplines;
    struct nuspline_s * splines;
    struct nuvec_s * spline_cvs;
    struct NUNODE_s * root_node;
    struct nugscn_s * gscene;
};

struct nuspline_s {
    char * name;
    int ncvs;
    struct nuvec_s * cvs;
    float r;
    float r2;
    struct nuvec_s min;
    struct nuvec_s max;
    struct nuvec_s cntr;
    float cntr_r;
    float cntr_r2;
};

typedef enum nustencilmode_e {
    NUSTENCIL_NOSTENCIL=0,
    NUSTENCIL_REPLACE_NODRAW=1
} nustencilmode_e;

typedef struct nustenitem_s nustenitem_s, *Pnustenitem_s;

struct nustenitem_s {
    struct nustenitem_s * next;
    struct nurndritem_s * hdr;
    struct nusysmtl_s * mtl;
    int type;
};

typedef struct nusyslight_s nusyslight_s, *Pnusyslight_s;

struct nusyslight_s {
    struct nulight_s light;
    int index;
    int next;
    int last;
};

typedef struct nusystex_s nusystex_s, *Pnusystex_s;

typedef struct nutex_s nutex_s, *Pnutex_s;

typedef enum nutextype_e {
    NUTEX_RGB16=0,
    NUTEX_RGBA16=1,
    NUTEX_RGB24=2,
    NUTEX_RGBA32=3,
    NUTEX_PAL4=4,
    NUTEX_PAL8=5,
    NUTEX_PAL4_S=6,
    NUTEX_PAL8_S=7,
    NUTEX_DXT1=8,
    NUTEX_DXT2=9,
    NUTEX_DXT3=10,
    NUTEX_DXT4=11,
    NUTEX_DXT5=12,
    NUTEX_BB=13,
    NUTEX_RT8=14,
    NUTEX_RTT8=15,
    NUTEX_RT8H=16,
    NUTEX_RT32=17,
    NUTEX_RT24=18,
    NUTEX_RT16=19,
    NUTEX_RTT32=20,
    NUTEX_RTT24=21,
    NUTEX_RTT16=22,
    NUTEX_RZT32=23,
    NUTEX_RZT24=24,
    NUTEX_RZT16=25,
    NUTEX_NUMTEXTURETYPES=26
} nutextype_e;

struct nutex_s {
    enum nutextype_e type;
    int width;
    int height;
    int mmcnt;
    void * bits;
    int * pal;
    short decal;
    short linear;
};

struct nusystex_s {
    struct nutex_s tex;
    struct D3DTexture * dds;
    int ref;
    int rtid;
};

typedef struct nutexanimf_s nutexanimf_s, *Pnutexanimf_s;

struct nutexanimf_s {
    struct nutexanim_s * succ;
    struct nutexanim_s * prev;
    int tids;
    short numtids;
    short dynalloc:1;
    int mtl;
    struct nutexanimenv_s * env;
    int ntaname;
    int scriptname;
};

typedef struct nutexanimlist_s nutexanimlist_s, *Pnutexanimlist_s;

struct nutexanimlist_s {
    struct nutexanim_s * nta;
    struct nutexanimlist_s * succ;
    struct nutexanimlist_s * prev;
};

typedef struct NuTexData_OLD NuTexData_OLD, *PNuTexData_OLD;

typedef uint uint32_t;

typedef ushort uint16_t;

struct NuTexData_OLD {
    uint format;
    uint32_t width;
    uint32_t height;
    uint32_t mode;
    uint pixel_data;
    uint palette;
    uint16_t decal;
    short unk_1;
};

typedef struct nuviewport_s nuviewport_s, *Pnuviewport_s;

struct nuviewport_s {
    uint x;
    uint y;
    uint width;
    uint height;
    float zmin;
    float zmax;
    float centre_x;
    float centre_y;
    float clipmin_x;
    float clipmin_y;
    float clipmax_x;
    float clipmax_y;
    float clip_w;
    float clip_h;
    float scis_w;
    float scis_h;
};

typedef struct nuvtx_lc1_s nuvtx_lc1_s, *Pnuvtx_lc1_s;

struct nuvtx_lc1_s {
    struct nuvec_s pnt;
    int diffuse;
    float tc[2];
};

typedef struct nuvtx_ps_s nuvtx_ps_s, *Pnuvtx_ps_s;

struct nuvtx_ps_s {
    struct nuvec_s pnt;
    int diffuse;
};

typedef struct nuvtx_sk3tc1_s nuvtx_sk3tc1_s, *Pnuvtx_sk3tc1_s;

struct nuvtx_sk3tc1_s {
    struct nuvec_s pnt;
    float weights[2];
    float indexes[3];
    struct nuvec_s nrm;
    int diffuse;
    float tc[2];
};

typedef struct nuvtx_tc1_s nuvtx_tc1_s, *Pnuvtx_tc1_s;

struct nuvtx_tc1_s {
    struct nuvec_s pnt;
    struct nuvec_s nrm;
    int diffuse;
    float tc[2];
};

typedef struct nuvtx_tltc1_s nuvtx_tltc1_s, *Pnuvtx_tltc1_s;

struct nuvtx_tltc1_s {
    struct nuvec_s pnt;
    float rhw;
    int diffuse;
    float tc[2];
};

typedef struct nuvtx_ts_s nuvtx_ts_s, *Pnuvtx_ts_s;

struct nuvtx_ts_s {
    struct nuvec_s vTangent;
    struct nuvec_s vNormal;
};

typedef struct nuwateritem_s nuwateritem_s, *Pnuwateritem_s;

struct nuwateritem_s {
    struct nuwateritem_s * next;
    struct nurndritem_s * hdr;
    struct nusysmtl_s * mtl;
};

typedef struct object_path_s object_path_s, *Pobject_path_s;

struct object_path_s {
    int objid;
    float speed;
    int oscillate;
    int repeat;
    float pause;
    float usedway;
    float usedtime;
    float usedpart;
    float usedsound;
    float start_offset;
    float terrplatid;
    struct nuvec_s waypoint[8];
    float waypoint_speed[8];
    struct nuvec_s waypoint_rot[8];
    int waypoint_time[8];
    int trigger_type;
    int trigger_id;
    float trigger_var;
    float trigger_wait;
    char particle_name[16][8];
    int particle_type[8];
    int particle_rate[8];
    int particle_switch[8];
    struct nuvec_s particle_offset[8];
    short particle_emitrotz[8];
    short particle_emitroty[8];
    float sound_last_time;
    char sound_name[16][8];
    int sound_id[8];
    int sound_type[8];
    float sound_time[8];
    struct nuvec_s sound_offset[8];
    float playergrav;
    float tension;
    float damping;
};

typedef struct ObjTab ObjTab, *PObjTab;

struct ObjTab {
    struct nuhspecial_s obj;
    struct nugscn_s * * scene;
    char visible;
    char font3d_letter;
    char pad1;
    char pad2;
    char * name;
    char unk[4];
    ulonglong levbits; /* unk_type */
};

typedef struct offlist offlist, *Pofflist;

typedef struct TERRAINFLAGS_s TERRAINFLAGS_s, *PTERRAINFLAGS_s;

struct TERRAINFLAGS_s {
    uint rot;
};

struct offlist {
    int offset;
    struct nuvec_s translation;
    short type;
    short info;
    short rx;
    short ry;
    short rz;
    short pad;
    struct nuvec_s rotation;
    struct TERRAINFLAGS_s flags;
    short prim;
    short id;
    int datapos;
};

typedef struct OppTub OppTub, *POppTub;

struct OppTub {
    struct creature_s * c;
    char track;
    char count;
    uchar wait;
    char laps;
    float time;
    float duration;
    float lap_position;
    char place;
    uchar finished;
    uchar finishframes;
    uchar stall;
    ushort old_xrot;
    ushort old_yrot;
    ushort old_zrot;
    uchar boost;
    char spark;
};

typedef struct PadRecData PadRecData, *PPadRecData;

struct PadRecData {
    uchar ok;
    uchar read_id;
    uchar buttons_hi;
    uchar buttons_lo;
    uchar r_alg_x;
    uchar r_alg_y;
    uchar l_alg_x;
    uchar l_alg_y;
    uchar l_algpad_r;
    uchar l_algpad_l;
    uchar l_algpad_u;
    uchar l_algpad_d;
    uchar r_algpad_u;
    uchar r_algpad_r;
    uchar r_algpad_d;
    uchar r_algpad_l;
    uchar l1_alg;
    uchar r1_alg;
    uchar l2_alg;
    uchar r2_alg;
};

typedef struct PadRecInfo PadRecInfo, *PPadRecInfo;

struct PadRecInfo {
    int padpointer;
    int padmode;
    int padend;
    int padsize;
    struct PadRecData recdata[48000];
};

typedef struct PADStatus PADStatus, *PPADStatus;

struct PADStatus {
    uint button;
    short stickX;
    short stickY;
    short substickX;
    short substickY;
    uint triggerLeft;
    uint triggerRight;
    uint analogA;
    uint analogB;
    short err;
};

typedef struct ParticleChunkRenderStack ParticleChunkRenderStack, *PParticleChunkRenderStack;

struct ParticleChunkRenderStack {
    struct rdata_s * chunk;
    struct debtab * debinfo;
    struct debkeydatatype_s * debdata;
    struct numtx_s rotmtx;
    float x;
    float y;
    float z;
};

typedef struct pCutAnim_s pCutAnim_s, *PpCutAnim_s;

struct pCutAnim_s {
    short character;
    short action;
    short sfx;
    char i;
    char pad1;
};

typedef struct PData PData, *PPData;

struct PData {
    uchar character;
    uchar font3d_letter;
    char pad1;
    char pad2;
    char * name;
    int * description;
};

typedef struct PDeb PDeb, *PPDeb;

typedef struct PDebInfo PDebInfo, *PPDebInfo;

struct PDeb {
    struct PDebInfo * info;
    struct nuvec_s oldpos;
    struct nuvec_s pos;
    struct nuvec_s mom;
    float time;
    float oldscale;
    float scale;
    ushort locators;
    ushort xrot;
    ushort yrot;
    ushort zrot;
    char type;
    char active;
    char pad1;
    char pad2;
};

struct PDebInfo {
    int i_objtab;
    float duration;
    float gravity;
};

typedef struct PLANESTRUCT PLANESTRUCT, *PPLANESTRUCT;

struct PLANESTRUCT {
    struct nuvec_s Pos;
    struct nuvec_s Vel;
    float ActionTimer;
    int ActionStatus;
    int Active;
    struct MYDRAW MainDraw;
};

typedef struct platattrib platattrib, *Pplatattrib;

struct platattrib {
    uint hit:1;
    uint rotate:1;
};

typedef struct platdata platdata, *Pplatdata;

struct platdata {
    struct numtx_s oldmtx;
    struct numtx_s * curmtx;
    short terrno;
    short instance;
    struct platattrib status;
    short hitcnt;
    short pad;
    float plrgrav;
    float ypos;
    float yvel;
    float tension;
    float damp;
};

typedef struct plr_bonus_wumpas plr_bonus_wumpas, *Pplr_bonus_wumpas;

struct plr_bonus_wumpas {
    short count;
    short draw;
    char frame;
    char wait;
    uchar delay;
    uchar item;
};

typedef struct plr_item plr_item, *Pplr_item;

struct plr_item {
    short count;
    short draw;
    char frame;
    char wait;
    uchar delay;
    uchar item;
};

typedef struct PlrEvent PlrEvent, *PPlrEvent;

struct PlrEvent {
    struct nugspline_s * spl;
    struct nuhspecial_s obj[16];
    struct nuvec_s pos;
    char played;
    char iRAIL;
    short iALONG;
    float fALONG;
};

typedef struct pollist pollist, *Ppollist;

struct pollist {
    float x;
    float y;
    float z;
    float x2;
    float y2;
    float z2;
    short timer;
    short pad;
    struct hitdata * hitDat[512];
};

typedef struct Poly Poly, *PPoly;

struct Poly {
    float minx;
    float maxx;
    float miny;
    float maxy;
    float minz;
    float maxz;
    struct nuvec_s pnts[4];
    struct nuvec_s norm[2];
    uchar info[4];
};

typedef union prevLst prevLst, *PprevLst;

union prevLst {
    char * s8;
    struct nulnkhdr_s * lhdr;
};

typedef struct primdef_s primdef_s, *Pprimdef_s;

struct primdef_s {
    struct nuvtx_tc1_s vrts[3];
    int vid[3];
    float weights[15][3];
    int mtxid[15];
    int nummtx;
    int sorted;
    int baseid;
};

typedef struct Projectile Projectile, *PProjectile;

struct Projectile {
    struct obj_s obj;
    struct nuvec_s srcpos;
    struct nuvec_s dstpos;
    float time;
    float duration;
    char type;
    char active;
    short i_objtab;
    char kill;
    uchar owner_safety;
    char pad1;
    char pad2;
};

typedef struct pVTog pVTog, *PpVTog;

struct pVTog {
    struct nugspline_s * pTRIGGER;
    struct nugspline_s * pCAM;
    struct nugspline_s * pLOOK;
    struct numtx_s m;
    struct nuvec_s pos;
    struct anim_s anim;
    char type;
    char iRAIL;
    short iALONG;
    float fALONG;
    ushort xrot;
    ushort yrot;
    ushort zrot;
    char pad1;
    char pad2;
};

typedef struct Rail Rail, *PRail;

struct Rail {
    struct nugspline_s * pINPLAT;
    struct nugspline_s * pINCAM;
    struct nugspline_s * pLEFT;
    struct nugspline_s * pCAM;
    struct nugspline_s * pRIGHT;
    struct nugspline_s * pOUTCAM;
    struct nugspline_s * pOUTPLAT;
    float in_distance;
    float out_distance;
    short edges;
    char type;
    char circuit;
};

typedef struct rendertargetlist_s rendertargetlist_s, *Prendertargetlist_s;

struct rendertargetlist_s {
    int next;
    int last;
    uint width;
    uint height;
    struct D3DSurface * pddsRenderTarget;
    struct D3DSurface * pddsZBuffer;
    struct D3DSurface * pddsStencilBuffer;
};

typedef struct scene_inst_s scene_inst_s, *Pscene_inst_s;

struct scene_inst_s {
    char * name;
    struct nugscn_s * scene;
};

typedef struct sceneinst_s sceneinst_s, *Psceneinst_s;

struct sceneinst_s {
    struct nuscene_s * nus;
    char name[256];
    int inst_cnt;
};

typedef struct sceneptr sceneptr, *Psceneptr;

struct sceneptr {
    int count;
    struct offlist offlist[512];
};

typedef enum shadertypes_e {
    NO_SHADER=0,
    WATER=1,
    HEATHAZE=2,
    GLASS=3,
    WATERCAUSTICS=4,
    SNOWCLOUD=5,
    DEPTHBLEND=6,
    SPECULAR=7,
    LIGHTHAZE=8,
    PROJTEX=9,
    BRDFGOLD=10,
    LIGHTS=11,
    VOLUMESHADOW=12,
    VOLUMESHADOW_BLENDSKIN=13,
    VOLUMESHADOW_BLENDSKIN2=14,
    BLENDSKIN=15,
    BLENDSKIN2=16,
    BLENDSKINGLASS=17,
    BLENDSKINGLASS2=18,
    BLENDSKINFUR=19,
    BLENDSKINFUR2=20,
    BUMPMAP=21,
    BUMPMAPPOINTLIGHT=22,
    ENVIRONMAP=23,
    POINTLIGHT=24,
    BLENDSKINPOINTLIGHT=25,
    BLENDSKIN2POINTLIGHT=26,
    DYNAMICWATER=27,
    CALCNEIGHBOURFORCE=28,
    CALCNEIGHBOURFORCE2=29,
    APPLYFORCEORVELOCITY=30,
    BLURFILTER=31,
    CREATENORMALMAP=32,
    XRAYGLASS=33,
    DUMMY=34,
    NUM_SHADER_TYPES=35,
    RESET_SHADER=255
} shadertypes_e;

typedef struct ShadPolDat ShadPolDat, *PShadPolDat;

struct ShadPolDat {
    struct nuvec_s pos;
    float size;
    short shade;
    short xrot;
    short yrot;
    short zrot;
};

typedef struct space space, *Pspace;

struct space {
    int character;
    char unk[4];
    struct AnimList animlist[5];
};

typedef struct SphereData SphereData, *PSphereData;

struct SphereData {
    struct nuvec_s pos;
    float radius;
};

typedef struct SplTab SplTab, *PSplTab;

struct SplTab {
    struct nugspline_s * spl;
    char * name;
    short min;
    short max;
    char unk[4];
    ulonglong levbits; /* unk_type */
};

typedef struct tagRECT tagRECT, *PtagRECT;

struct tagRECT {
    int left;
    int top;
    int right;
    int bottom;
};

typedef struct TempTerr TempTerr, *PTempTerr;

typedef struct terr terr, *Pterr;

typedef struct TrackInfo TrackInfo, *PTrackInfo;

typedef struct terrgroup terrgroup, *Pterrgroup;

typedef enum terr_type {
    TERR_TYPE_NORMAL=0,
    TERR_TYPE_PLATFORM=1,
    TERR_TYPE_WALLSPL=2,
    TERR_TYPE_CRASHDATA=3,
    TERR_TYPE_EMPTY=255
} terr_type;

struct terr {
    struct nuvec_s Location;
    short * model;
    struct nuvec_s min;
    struct nuvec_s max;
    struct TERRAINFLAGS_s flags;
    enum terr_type type;
    short info;
    short id;
    float radius;
};

struct terrgroup {
    short tabindex;
    short count;
    float minx;
    float minz;
    float maxx;
    float maxz;
};

struct TrackInfo {
    void * ptrid;
    short platid;
    short platinf;
    short timer;
    short pad;
};

struct TempTerr {
    struct terr * terr;
    struct platdata platdat[128];
    void * wallinfo;
    struct TrackInfo Trackinfo[4];
    struct terrgroup group[257];
    int terrainlow;
    int terrgcnt;
    short terrlist[4096];
    struct pollist polllist[9];
};

typedef struct TerrI TerrI, *PTerrI;

struct TerrI {
    struct nuvec_s origpos;
    struct nuvec_s origvel;
    struct nuvec_s curpos;
    struct nuvec_s curvel;
    short id;
    short scanmode;
    float stopflag;
    float vellen;
    uchar * flags;
    float ax;
    float ay;
    float len;
    float size;
    float sizesq;
    float sizediv;
    float yscale;
    float yscalesq;
    float inyscale;
    float inyscalesq;
    short hitcnt;
    short hitterrno;
    float csx;
    float csy;
    float csz;
    float cex;
    float cey;
    float cez;
    short hittype;
    short plathit;
    short * platScanStart;
    struct hitdata * hitter;
    float hittime;
    float timeadj;
    float impactadj;
    struct nuvec_s hitnorm;
    struct nuvec_s uhitnorm;
    struct nuvec_s tempvec[2];
    struct hitdata rotter;
    struct hitdata * hitdat[512];
};

typedef struct TerrShape TerrShape, *PTerrShape;

struct TerrShape {
    struct nuvec_s offset;
    float ang;
    float size;
};

typedef struct TerSurface TerSurface, *PTerSurface;

struct TerSurface {
    float friction;
    short gdeb;
    ushort flags;
};

typedef struct Text3DOBJ Text3DOBJ, *PText3DOBJ;

struct Text3DOBJ { /* volatile */
    short i;
    uchar flags;
    char action;
    float anim_time;
    float scale;
};

typedef struct trail trail, *Ptrail;

struct trail {
    struct nuvec_s pos1;
    struct nuvec_s pos2;
    int intensity;
    int RealIntensity;
};

typedef struct VEHMASK VEHMASK, *PVEHMASK;

struct VEHMASK {
    int Active;
    int id;
    int KillAtEnd;
    struct nuvec_s Position;
    struct MYDRAW MainDraw;
    float DrawAngY;
    int Action;
    int LastAction;
    int EffectType;
    float Tween;
    float TweenInc;
    float Ang;
    struct nuvec_s Store[2];
    struct nuvec_s * Point[2];
    struct nuvec_s Offset[2];
    float AngInc[2];
    float Rad[2];
    float Scale[2];
    float TiltX[2];
    float DrawScale;
    int Seen;
};

typedef struct visidat_s visidat_s, *Pvisidat_s;

struct visidat_s {
    struct nugspline_s * sp;
    int numinstances;
    struct nuinstance_s * i[1];
};

typedef struct visidata_s visidata_s, *Pvisidata_s;

struct visidata_s {
    struct nugscn_s * sc;
    void * * * vspline; /* unk_type */
    int binfosize;
    int curspline;
    int curknot;
};

typedef struct WallSpl WallSpl, *PWallSpl;

struct WallSpl {
    ushort count;
    ushort val;
    struct nuvec_s spl[4096];
};

typedef struct WaterDat WaterDat, *PWaterDat;

struct WaterDat {
    struct nuvec_s pos;
    float size;
    float endsize;
    float cursize;
    uint shade;
    uint curshade;
    short timer;
    short otimer;
};

typedef struct WBBOLT WBBOLT, *PWBBOLT;

struct WBBOLT {
    int Mode;
    int Owner;
    struct nuvec_s Position;
    struct nuvec_s Velocity;
    float Life;
    int Type;
    float SeekSpeed;
    float Scale;
};

typedef struct WindGroup WindGroup, *PWindGroup;

struct WindGroup {
    struct nuinstance_s * instance;
    struct numtx_s * mtx;
    struct nuvec_s center;
    char onscreen;
    char inrange;
    short objcount;
    int collide;
    float wind;
    float height;
    float radius;
};

typedef struct WInfo WInfo, *PWInfo;

struct WInfo {
    struct numtx_s m;
    float scale;
    float dy;
    ushort angle;
    ushort spin;
    char pad1;
    char pad2;
    char pad3;
    char pad4;
};

typedef struct WScr WScr, *PWScr;

struct WScr {
    struct nuvec_s pos;
    float timer;
    float xs;
    float ys;
    float scale;
    char bonus;
    char pad1;
    char pad2;
    char pad3;
};

typedef struct Wumpa Wumpa, *PWumpa;

struct Wumpa {
    struct nuvec_s pos0;
    struct nuvec_s pos1;
    struct nuvec_s pos;
    struct nuvec_s mom;
    float shadow;
    ushort surface_xrot;
    ushort surface_zrot;
    float time;
    float duration;
    char active;
    char iRAIL;
    short iALONG;
    float fALONG;
    char destroy;
    char fired;
    char in_range;
    char surface_type;
};

typedef struct ZOFFASTART ZOFFASTART, *PZOFFASTART;

struct ZOFFASTART {
    float x;
    float y;
    float z;
    float angle;
};

#define __WORDSIZE 32

typedef struct HWND__ HWND__, *PHWND__;

struct HWND__ { /* PlaceHolder Structure */
};

typedef struct Elf32_Ehdr Elf32_Ehdr, *PElf32_Ehdr;

struct Elf32_Ehdr {
    byte e_ident_magic_num;
    char e_ident_magic_str[3];
    byte e_ident_class;
    byte e_ident_data;
    byte e_ident_version;
    byte e_ident_osabi;
    byte e_ident_abiversion;
    byte e_ident_pad[7];
    word e_type;
    word e_machine;
    dword e_version;
    dword e_entry;
    dword e_phoff;
    dword e_shoff;
    dword e_flags;
    word e_ehsize;
    word e_phentsize;
    word e_phnum;
    word e_shentsize;
    word e_shnum;
    word e_shstrndx;
};

typedef struct Elf32_Phdr Elf32_Phdr, *PElf32_Phdr;

typedef enum Elf_ProgramHeaderType_PPC {
    PT_NULL=0,
    PT_LOAD=1,
    PT_DYNAMIC=2,
    PT_INTERP=3,
    PT_NOTE=4,
    PT_SHLIB=5,
    PT_PHDR=6,
    PT_TLS=7,
    PT_GNU_EH_FRAME=1685382480,
    PT_GNU_STACK=1685382481,
    PT_GNU_RELRO=1685382482
} Elf_ProgramHeaderType_PPC;

struct Elf32_Phdr {
    enum Elf_ProgramHeaderType_PPC p_type;
    dword p_offset;
    dword p_vaddr;
    dword p_paddr;
    dword p_filesz;
    dword p_memsz;
    dword p_flags;
    dword p_align;
};

typedef struct Elf32_Shdr Elf32_Shdr, *PElf32_Shdr;

typedef enum Elf_SectionHeaderType_PPC {
    SHT_NULL=0,
    SHT_PROGBITS=1,
    SHT_SYMTAB=2,
    SHT_STRTAB=3,
    SHT_RELA=4,
    SHT_HASH=5,
    SHT_DYNAMIC=6,
    SHT_NOTE=7,
    SHT_NOBITS=8,
    SHT_REL=9,
    SHT_SHLIB=10,
    SHT_DYNSYM=11,
    SHT_INIT_ARRAY=14,
    SHT_FINI_ARRAY=15,
    SHT_PREINIT_ARRAY=16,
    SHT_GROUP=17,
    SHT_SYMTAB_SHNDX=18,
    SHT_ANDROID_REL=1610612737,
    SHT_ANDROID_RELA=1610612738,
    SHT_GNU_ATTRIBUTES=1879048181,
    SHT_GNU_HASH=1879048182,
    SHT_GNU_LIBLIST=1879048183,
    SHT_CHECKSUM=1879048184,
    SHT_SUNW_move=1879048186,
    SHT_SUNW_COMDAT=1879048187,
    SHT_SUNW_syminfo=1879048188,
    SHT_GNU_verdef=1879048189,
    SHT_GNU_verneed=1879048190,
    SHT_GNU_versym=1879048191
} Elf_SectionHeaderType_PPC;

struct Elf32_Shdr {
    dword sh_name;
    enum Elf_SectionHeaderType_PPC sh_type;
    dword sh_flags;
    dword sh_addr;
    dword sh_offset;
    dword sh_size;
    dword sh_link;
    dword sh_info;
    dword sh_addralign;
    dword sh_entsize;
};

typedef struct Elf32_Sym Elf32_Sym, *PElf32_Sym;

struct Elf32_Sym {
    dword st_name;
    dword st_value;
    dword st_size;
    byte st_info;
    byte st_other;
    word st_shndx;
};

typedef void _IO_lock_t;

typedef struct _IO_marker _IO_marker, *P_IO_marker;

typedef struct _IO_FILE _IO_FILE, *P_IO_FILE;

typedef long __off_t;

typedef longlong __quad_t;

typedef __quad_t __off64_t;

typedef ulong size_t;

struct _IO_marker {
    struct _IO_marker * _next;
    struct _IO_FILE * _sbuf;
    int _pos;
};

struct _IO_FILE {
    int _flags;
    char * _IO_read_ptr;
    char * _IO_read_end;
    char * _IO_read_base;
    char * _IO_write_base;
    char * _IO_write_ptr;
    char * _IO_write_end;
    char * _IO_buf_base;
    char * _IO_buf_end;
    char * _IO_save_base;
    char * _IO_backup_base;
    char * _IO_save_end;
    struct _IO_marker * _markers;
    struct _IO_FILE * _chain;
    int _fileno;
    int _flags2;
    __off_t _old_offset;
    ushort _cur_column;
    char _vtable_offset;
    char _shortbuf[1];
    _IO_lock_t * _lock;
    __off64_t _offset;
    void * __pad1;
    void * __pad2;
    void * __pad3;
    void * __pad4;
    size_t __pad5;
    int _mode;
    char _unused2[56];
};

typedef struct lconv lconv, *Plconv;

struct lconv {
    char * decimal_point;
    char * thousands_sep;
    char * grouping;
    char * int_curr_symbol;
    char * currency_symbol;
    char * mon_decimal_point;
    char * mon_thousands_sep;
    char * mon_grouping;
    char * positive_sign;
    char * negative_sign;
    char int_frac_digits;
    char frac_digits;
    char p_cs_precedes;
    char p_sep_by_space;
    char n_cs_precedes;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
    char int_p_cs_precedes;
    char int_p_sep_by_space;
    char int_n_cs_precedes;
    char int_n_sep_by_space;
    char int_p_sign_posn;
    char int_n_sign_posn;
};

typedef void * __gnuc_va_list;

typedef int intptr_t;

typedef ulonglong uint64_t;

typedef uchar uint_fast8_t;

typedef struct _IO_FILE FILE;

typedef struct nufaceonitem_s nufaceonitem_s, *Pnufaceonitem_s;

struct nufaceonitem_s {
    struct nufaceonitem_s * next;
    struct nurndritem_s * hdr;
    struct nusysmtl_s * mtl;
};

typedef struct txanimscripts txanimscripts, *Ptxanimscripts;

struct txanimscripts {
    char * path;
    int pad1;
    ulonglong levbits;
};

typedef struct Vert_s8 Vert_s8, *PVert_s8;

struct Vert_s8 {
    char field0_0x0[18];
};

typedef int __ssize_t;

typedef short int16_t;

typedef longlong int64_t;

typedef char int8_t;

typedef __ssize_t ssize_t;

typedef void * ARCallback;

typedef unsigned short    wchar16;
