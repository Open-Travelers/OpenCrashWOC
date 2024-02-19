#ifndef NURASTERTYPES_H
#define NURASTERTYPES_H

#include "nu3dx/nu3dxtypes.h"
#include "types.h"
#include"system/gxtype.h"
#include"system/gs.h"

// TODO: TYPES!!!


//static struct rendertargetlist_s g_pRTArray[16];
static struct D3DSurface* g_pddsStencilBuffer;
static unsigned int g_dwRenderHeight;
static unsigned int g_dwRenderWidth;
static struct D3DSurface* g_pddsBackBuffer;
static struct D3DSurface* g_pddsZBuffer;
static int g_dwNumRT;
static int g_dwCurrentRT;
static int g_dwAllocRT;
static int g_dwFreeRT;
static struct D3DDevice* g_pd3dDevice;
int PHYSICAL_SCREEN_X;
static struct _GS_VIEWPORT GS_ViewPort;
static struct _GXColor GS_BgColour;
static struct D3DSurface* g_pddsZBuffer;
void* volatile hLoadScreenThread;



struct tagRECT1{
    int left;
    int top;
    int right;
    int bottom;
};

static struct tagRECT1 g_rcScreenRect;

// Size: 0x1
struct D3DDevice
{
    /*static inline int SetMaterial(const _D3DMATERIAL8* pMaterial);
    static inline int SetViewport(const _D3DVIEWPORT8* pViewport);
    static inline int SetPixelShaderConstant(unsigned int Register, const void* pConstantData, unsigned int ConstantCount);
    static inline int CreateTexture(unsigned int Width, unsigned int Height, unsigned int Levels, unsigned int Usage, _D3DFORMAT Format, unsigned int Pool, D3DTexture** ppTexture);
    static inline int SetPixelShader(unsigned int Handle);
    static inline int CreateVertexShader(const unsigned long* pDeclaration, const unsigned long* pFunction, unsigned long* pHandle, unsigned int Usage);
    static inline int SetShaderConstantMode(unsigned int Mode);
    static inline unsigned int Release();
    static inline int CreateVertexBuffer(unsigned int Length, unsigned int Usage, unsigned int FVF, unsigned int Pool, D3DVertexBuffer** ppVertexBuffer);
    static inline int Clear(unsigned int Count, const _D3DRECT* pRects, unsigned int Flags, unsigned int Color, float Z, unsigned int Stencil);
    static inline int LightEnable(unsigned int Index, int Enable);
    static inline int SelectVertexShader(unsigned int Handle, unsigned int Address);
    static inline int CopyRects(D3DSurface* pSourceSurface, const tagRECT* pSourceRectsArray, unsigned int cRects, D3DSurface* pDestinationSurface, const tagPOINT* pDestPointsArray);
    static inline int SetTransform(_D3DTRANSFORMSTATETYPE State, const _D3DMATRIX* pMatrix);
    static inline int DeletePixelShader(unsigned int Handle);
    static inline int CreatePalette(_D3DPALETTESIZE Size, D3DPalette** ppPalette);
    static inline int SetLight(unsigned int Index, const _D3DLIGHT8* pLight);
    static inline int GetViewport(_D3DVIEWPORT8* pViewport);
    static inline int DeleteVertexShader(unsigned int Handle);
    static inline int SetTextureStageState(unsigned int Stage, _D3DTEXTURESTAGESTATETYPE Type, unsigned int Value);
    static inline int CreateIndexBuffer(unsigned int Length, unsigned int Usage, _D3DFORMAT Format, unsigned int Pool, D3DIndexBuffer** ppIndexBuffer);
    static inline int GetBackBuffer(int BackBuffer, unsigned int Type, D3DSurface** ppBackBuffer);
    static inline int GetDepthStencilSurface(D3DSurface** ppZStencilSurface);
    static inline int SetRenderTarget(D3DSurface* pRenderTarget, D3DSurface* pNewZStencil);
    static inline int SetRenderState(_D3DRENDERSTATETYPE State, unsigned int Value);*/
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

// Size: 0x18
struct _D3DVIEWPORT8
{
    unsigned int X;
    unsigned int Y;
    unsigned int Width;
    unsigned int Height;
    float MinZ;
    float MaxZ;
};

/*struct HWND__
{
    int unused;
};*/


enum _D3DRENDERSTATETYPE
{
    D3DRS_FORCE_DWORD = 2147483647,
    D3DRS_MAX = 146,
    D3DRS_DONOTCULLUNCOMPRESSED = 145,
    D3DRS_ROPZREAD = 144,
    D3DRS_ROPZCMPALWAYSREAD = 143,
    D3DRS_STENCILCULLENABLE = 142,
    D3DRS_OCCLUSIONCULLENABLE = 141,
    D3DRS_YUVENABLE = 140,
    D3DRS_DXT1NOISEENABLE = 139,
    D3DRS_LINEWIDTH = 138,
    D3DRS_SHADOWFUNC = 137,
    D3DRS_EDGEANTIALIAS = 132,
    D3DRS_LOGICOP = 131,
    D3DRS_ZBIAS = 130,
    D3DRS_TEXTUREFACTOR = 129,
    D3DRS_CULLMODE = 128,
    D3DRS_FRONTFACE = 127,
    D3DRS_STENCILFAIL = 126,
    D3DRS_STENCILENABLE = 125,
    D3DRS_ZENABLE = 124,
    D3DRS_NORMALIZENORMALS = 123,
    D3DRS_TWOSIDEDLIGHTING = 122,
    D3DRS_BACKFILLMODE = 121,
    D3DRS_FILLMODE = 120,
    D3DRS_FOGCOLOR = 119,
    D3DRS_VERTEXBLEND = 118,
    D3DRS_PSTEXTUREMODES = 117,
    D3DRS_DEFERRED_MAX = 117,
    D3DRS_SWAPFILTER = 116,
    D3DRS_PATCHSEGMENTS = 115,
    D3DRS_PATCHEDGESTYLE = 114,
    D3DRS_POINTSIZE_MAX = 113,
    D3DRS_POINTSCALE_C = 112,
    D3DRS_POINTSCALE_B = 111,
    D3DRS_POINTSCALE_A = 110,
    D3DRS_POINTSCALEENABLE = 109,
    D3DRS_POINTSPRITEENABLE = 108,
    D3DRS_POINTSIZE_MIN = 107,
    D3DRS_POINTSIZE = 106,
    D3DRS_AMBIENT = 105,
    D3DRS_BACKAMBIENT = 104,
    D3DRS_EMISSIVEMATERIALSOURCE = 103,
    D3DRS_AMBIENTMATERIALSOURCE = 102,
    D3DRS_DIFFUSEMATERIALSOURCE = 101,
    D3DRS_SPECULARMATERIALSOURCE = 100,
    D3DRS_BACKEMISSIVEMATERIALSOURCE = 99,
    D3DRS_BACKAMBIENTMATERIALSOURCE = 98,
    D3DRS_BACKDIFFUSEMATERIALSOURCE = 97,
    D3DRS_BACKSPECULARMATERIALSOURCE = 96,
    D3DRS_COLORVERTEX = 95,
    D3DRS_LOCALVIEWER = 94,
    D3DRS_SPECULARENABLE = 93,
    D3DRS_LIGHTING = 92,
    D3DRS_WRAP3 = 91,
    D3DRS_WRAP2 = 90,
    D3DRS_WRAP1 = 89,
    D3DRS_WRAP0 = 88,
    D3DRS_RANGEFOGENABLE = 87,
    D3DRS_FOGDENSITY = 86,
    D3DRS_FOGEND = 85,
    D3DRS_FOGSTART = 84,
    D3DRS_FOGTABLEMODE = 83,
    D3DRS_FOGENABLE = 82,
    D3DRS_SIMPLE_MAX = 82,
    D3DRS_SOLIDOFFSETENABLE = 81,
    D3DRS_WIREFRAMEOFFSETENABLE = 80,
    D3DRS_POINTOFFSETENABLE = 79,
    D3DRS_POLYGONOFFSETZOFFSET = 78,
    D3DRS_POLYGONOFFSETZSLOPESCALE = 77,
    D3DRS_SWATHWIDTH = 76,
    D3DRS_BLENDCOLOR = 75,
    D3DRS_BLENDOP = 74,
    D3DRS_STENCILWRITEMASK = 73,
    D3DRS_STENCILMASK = 72,
    D3DRS_STENCILREF = 71,
    D3DRS_STENCILFUNC = 70,
    D3DRS_STENCILPASS = 69,
    D3DRS_STENCILZFAIL = 68,
    D3DRS_COLORWRITEENABLE = 67,
    D3DRS_SHADEMODE = 66,
    D3DRS_DITHERENABLE = 65,
    D3DRS_ZWRITEENABLE = 64,
    D3DRS_DESTBLEND = 63,
    D3DRS_SRCBLEND = 62,
    D3DRS_ALPHAREF = 61,
    D3DRS_ALPHATESTENABLE = 60,
    D3DRS_ALPHABLENDENABLE = 59,
    D3DRS_ALPHAFUNC = 58,
    D3DRS_ZFUNC = 57,
    D3DRS_PS_MAX = 57,
    D3DRS_PSINPUTTEXTURE = 56,
    D3DRS_PSDOTMAPPING = 55,
    D3DRS_PSCOMBINERCOUNT = 53,
    D3DRS_PSRGBOUTPUTS7 = 52,
    D3DRS_PSRGBOUTPUTS6 = 51,
    D3DRS_PSRGBOUTPUTS5 = 50,
    D3DRS_PSRGBOUTPUTS4 = 49,
    D3DRS_PSRGBOUTPUTS3 = 48,
    D3DRS_PSRGBOUTPUTS2 = 47,
    D3DRS_PSRGBOUTPUTS1 = 46,
    D3DRS_PSRGBOUTPUTS0 = 45,
    D3DRS_PSFINALCOMBINERCONSTANT1 = 44,
    D3DRS_PSFINALCOMBINERCONSTANT0 = 43,
    D3DRS_PSCOMPAREMODE = 42,
    D3DRS_PSRGBINPUTS7 = 41,
    D3DRS_PSRGBINPUTS6 = 40,
    D3DRS_PSRGBINPUTS5 = 39,
    D3DRS_PSRGBINPUTS4 = 38,
    D3DRS_PSRGBINPUTS3 = 37,
    D3DRS_PSRGBINPUTS2 = 36,
    D3DRS_PSRGBINPUTS1 = 35,
    D3DRS_PSRGBINPUTS0 = 34,
    D3DRS_PSALPHAOUTPUTS7 = 33,
    D3DRS_PSALPHAOUTPUTS6 = 32,
    D3DRS_PSALPHAOUTPUTS5 = 31,
    D3DRS_PSALPHAOUTPUTS4 = 30,
    D3DRS_PSALPHAOUTPUTS3 = 29,
    D3DRS_PSALPHAOUTPUTS2 = 28,
    D3DRS_PSALPHAOUTPUTS1 = 27,
    D3DRS_PSALPHAOUTPUTS0 = 26,
    D3DRS_PSCONSTANT1_7 = 25,
    D3DRS_PSCONSTANT1_6 = 24,
    D3DRS_PSCONSTANT1_5 = 23,
    D3DRS_PSCONSTANT1_4 = 22,
    D3DRS_PSCONSTANT1_3 = 21,
    D3DRS_PSCONSTANT1_2 = 20,
    D3DRS_PSCONSTANT1_1 = 19,
    D3DRS_PSCONSTANT1_0 = 18,
    D3DRS_PSCONSTANT0_7 = 17,
    D3DRS_PSCONSTANT0_6 = 16,
    D3DRS_PSCONSTANT0_5 = 15,
    D3DRS_PSCONSTANT0_4 = 14,
    D3DRS_PSCONSTANT0_3 = 13,
    D3DRS_PSCONSTANT0_2 = 12,
    D3DRS_PSCONSTANT0_1 = 11,
    D3DRS_PSCONSTANT0_0 = 10,
    D3DRS_PSFINALCOMBINERINPUTSEFG = 9,
    D3DRS_PSFINALCOMBINERINPUTSABCD = 8,
    D3DRS_PSALPHAINPUTS7 = 7,
    D3DRS_PSALPHAINPUTS6 = 6,
    D3DRS_PSALPHAINPUTS5 = 5,
    D3DRS_PSALPHAINPUTS4 = 4,
    D3DRS_PSALPHAINPUTS3 = 3,
    D3DRS_PSALPHAINPUTS2 = 2,
    D3DRS_PSALPHAINPUTS1 = 1,
    D3DRS_PSALPHAINPUTS0 = 0,
    D3DRS_PS_MIN = 0
};




#endif // !NURASTERTYPES_H
