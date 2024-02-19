#include "system/gc/__GXInit.h"
#include "system/gc/OSAlloc.h"

#define OSRoundUp32B(x) (((u32) (x) + 32 - 1) & ~(32 - 1))
#define OSRoundDown32B(x) (((u32) (x)) & ~(32 - 1))
#define VI_DISPLAY_PIX_SZ 2

static GXRenderModeObj rmodeobj;
static GXRenderModeObj* rmode = NULL;
void* DemoFrameBuffer1;
static int GPHangWorkaround;
u32 totalprogramsize;
u32 totalramavailable;
static void* DefaultFifo;
__OSCurrHeap = 0xffffffff; //OSAlloc.c
 // Size: 0x80
static struct
{
    unsigned char pad[128]; // Offset: 0x0
}* DefaultFifoObj;


GXRenderModeObj * DEMOGetRenderModeObj(void) {
  return rmode;
}

void * DEMOGetCurrentBuffer(void) {
  return DemoFrameBuffer1;
}

void GXAdjustForOverscan(GXRenderModeObj *rmin, GXRenderModeObj *rmout, u16 hor, u16 ver) {
    if (rmin != rmout){
        memcpy(rmout, rmin, sizeof(GXRenderModeObj));
    }
    rmout->fbWidth   = rmin->fbWidth - (hor << 1);
    rmout->efbHeight = rmin->efbHeight - ((rmin->efbHeight * (ver << 1)) / rmin->xfbHeight);
    if (rmin->xFBmode == VI_XFBMODE_SF && (rmin->viTVmode & VI_PROGRESSIVE))
        rmout->xfbHeight = rmin->xfbHeight - (ver << 1);
    else
        rmout->xfbHeight = rmin->xfbHeight - ver;

    rmout->viWidth = rmin->viWidth - (hor << 1);
    if (rmin->viTVmode & VI_PROGRESSIVE)
        rmout->viHeight = rmin->viHeight - (ver << 2);
    else
        rmout->viHeight = rmin->viHeight - (ver << 1);

    rmout->viXOrigin += hor;
    rmout->viYOrigin += ver;
}

//GXPixel.c
void GXSetPixelFmt(GXPixelFmt pixelFmt, GXZFmt16 zFmt)
{
	GXBool isZ16;
	static u32 p2f[GX_MAX_PIXELFMT]
	    = { GX_PF_RGB8_Z24, GX_PF_RGBA6_Z24, GX_PF_RGB565_Z16, GX_PF_Z24, GX_PF_Y8, GX_PF_Y8, GX_PF_Y8, GX_PF_U8 };

	const u32 zControlRegOld = gx->peCtrl;

	GX_SET_REG(gx->peCtrl, p2f[pixelFmt], GX_BP_ZCONTROL_PIXEL_FMT_ST, GX_BP_ZCONTROL_PIXEL_FMT_END);
	GX_SET_REG(gx->peCtrl, zFmt, GX_BP_ZCONTROL_Z_FMT_ST, GX_BP_ZCONTROL_Z_FMT_END);

	if (zControlRegOld != gx->peCtrl) {
		GX_BP_LOAD_REG(gx->peCtrl);
		isZ16 = (pixelFmt == GX_PF_RGB565_Z16) ? GX_TRUE : GX_FALSE;
		GX_SET_REG(gx->genMode, isZ16, GX_BP_GENMODE_MULTISAMPLE_ST, GX_BP_GENMODE_MULTISAMPLE_END);
		gx->dirtyState |= GX_DIRTY_GEN_MODE;
	}

	if (p2f[pixelFmt] == GX_PF_Y8) {
		GX_SET_REG(gx->cmode1, pixelFmt - GX_PF_Y8, GX_BP_DSTALPHA_YUV_FMT_ST, GX_BP_DSTALPHA_YUV_FMT_END);
		GX_SET_REG(gx->cmode1, GX_BP_REG_DSTALPHA, 0, 7);
		GX_BP_LOAD_REG(gx->cmode1);
	}

	gx->bpSentNot = 0;
}
//

static void __DEMOInitGX(void) {
    //GXSetViewport(0.0f, 0.0f, (float)rmode->fbWidth, (float)rmode->efbHeight, 0.0f, 1.0f);
    //GXSetScissor(0, 0, rmode->fbWidth, rmode->efbHeight);
    //u16 r31 = GXSetDispCopyYScale(GXGetYScaleFactor(rmode->efbHeight, rmode->xfbHeight));
    //GXSetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
    //GXSetDispCopyDst(rmode->fbWidth, r31);
    //GXSetCopyFilter(rmode->aa, rmode->sample_pattern, 1, rmode->vfilter);
    if (rmode->aa != 0) {
        GXSetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
    } else {
        GXColor copyClrColor = {0, 0, 0, 0x7F};
        GXSetCopyClear(copyClrColor,0xffffff);
        GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
    }
    GXCopyDisp(DemoFrameBuffer1, 1);
    //GXSetDispCopyGamma(GX_GM_1_0);
}
/*
static void __DEMOInitMem(void) {
	void *arenaLo, *arenaHi;
	u32 fbSize;

	arenaLo = OSGetArenaLo();
	arenaHi = OSGetArenaHi();

	fbSize = (VIPadFrameBufferWidth(rmode->fbWidth) * rmode->xfbHeight * (u32)VI_DISPLAY_PIX_SZ);
	DemoFrameBuffer1 = (void*)OSRoundUp32B((u32)arenaLo);
    OSSetArenaLo((void *)(fbSize + (int)DemoFrameBuffer1));
    arenaLo = OSGetArenaLo();
    arenaHi = OSGetArenaHi();
    totalramavailable = (int)arenaHi - (int)arenaLo;
    totalprogramsize = 0x3000000 - totalramavailable;
    arenaLo = OSRoundUp32B((u32)arenaLo);
    arenaLo = (void *)OSInitAlloc(arenaLo,arenaHi,1);
    OSSetArenaLo(arenaLo);
    OSSetCurrentHeap(OSCreateHeap(OSRoundUp32B((u32)arenaLo),arenaHi)); //
    OSSetArenaLo(arenaHi);
}
*/

//vi.c
/////////////

static unsigned long CurrTvMode;

// Size: 0x26
static struct
{
    unsigned char equ; // Offset: 0x0, DWARF: 0xE10D0E
    unsigned short acv; // Offset: 0x2, DWARF: 0xE10D2E
    unsigned short prbOdd; // Offset: 0x4, DWARF: 0xE10D4E
    unsigned short prbEven; // Offset: 0x6, DWARF: 0xE10D71
    unsigned short psbOdd; // Offset: 0x8, DWARF: 0xE10D95
    unsigned short psbEven; // Offset: 0xA, DWARF: 0xE10DB8
    unsigned char bs1; // Offset: 0xC, DWARF: 0xE10DDC
    unsigned char bs2; // Offset: 0xD, DWARF: 0xE10DFC
    unsigned char bs3; // Offset: 0xE, DWARF: 0xE10E1C
    unsigned char bs4; // Offset: 0xF, DWARF: 0xE10E3C
    unsigned short be1; // Offset: 0x10, DWARF: 0xE10E5C
    unsigned short be2; // Offset: 0x12, DWARF: 0xE10E7C
    unsigned short be3; // Offset: 0x14, DWARF: 0xE10E9C
    unsigned short be4; // Offset: 0x16, DWARF: 0xE10EBC
    unsigned short nhlines; // Offset: 0x18, DWARF: 0xE10EDC
    unsigned short hlw; // Offset: 0x1A, DWARF: 0xE10F00
    unsigned char hsy; // Offset: 0x1C, DWARF: 0xE10F20
    unsigned char hcs; // Offset: 0x1D, DWARF: 0xE10F40
    unsigned char hce; // Offset: 0x1E, DWARF: 0xE10F60
    unsigned char hbe640; // Offset: 0x1F, DWARF: 0xE10F80
    unsigned short hbs640; // Offset: 0x20, DWARF: 0xE10FA3
    unsigned char hbeCCIR656; // Offset: 0x22, DWARF: 0xE10FC6
    unsigned short hbsCCIR656; // Offset: 0x24, DWARF: 0xE10FED
}* CurrTiming;

// Size: 0x58
static  struct
{
    unsigned short DispPosX; // Offset: 0x0, DWARF: 0xE1102A
    unsigned short DispPosY; // Offset: 0x2, DWARF: 0xE1104F
    unsigned short DispSizeX; // Offset: 0x4, DWARF: 0xE11074
    unsigned short DispSizeY; // Offset: 0x6, DWARF: 0xE1109A
    unsigned short AdjustedDispPosX; // Offset: 0x8, DWARF: 0xE110C0
    unsigned short AdjustedDispPosY; // Offset: 0xA, DWARF: 0xE110ED
    unsigned short AdjustedDispSizeY; // Offset: 0xC, DWARF: 0xE1111A
    unsigned short AdjustedPanPosY; // Offset: 0xE, DWARF: 0xE11148
    unsigned short AdjustedPanSizeY; // Offset: 0x10, DWARF: 0xE11174
    unsigned short FBSizeX; // Offset: 0x12, DWARF: 0xE111A1
    unsigned short FBSizeY; // Offset: 0x14, DWARF: 0xE111C5
    unsigned short PanPosX; // Offset: 0x16, DWARF: 0xE111E9
    unsigned short PanPosY; // Offset: 0x18, DWARF: 0xE1120D
    unsigned short PanSizeX; // Offset: 0x1A, DWARF: 0xE11231
    unsigned short PanSizeY; // Offset: 0x1C, DWARF: 0xE11256
    // DWARF: 0xE12379
    VIXFBMode FBMode; // Offset: 0x20, DWARF: 0xE1127B
    unsigned long nonInter; // Offset: 0x24, DWARF: 0xE112A0
    unsigned long tv; // Offset: 0x28, DWARF: 0xE112C5
    unsigned char wordPerLine; // Offset: 0x2C, DWARF: 0xE112E4
    unsigned char std; // Offset: 0x2D, DWARF: 0xE1130C
    unsigned char wpl; // Offset: 0x2E, DWARF: 0xE1132C
    unsigned long bufAddr; // Offset: 0x30, DWARF: 0xE1134C
    unsigned long tfbb; // Offset: 0x34, DWARF: 0xE11370
    unsigned long bfbb; // Offset: 0x38, DWARF: 0xE11391
    unsigned char xof; // Offset: 0x3C, DWARF: 0xE113B2
    int black; // Offset: 0x40, DWARF: 0xE113D2
    int threeD; // Offset: 0x44, DWARF: 0xE113F4
    unsigned long rbufAddr; // Offset: 0x48, DWARF: 0xE11417
    unsigned long rtfbb; // Offset: 0x4C, DWARF: 0xE1143C
    unsigned long rbfbb; // Offset: 0x50, DWARF: 0xE1145E
    // Size: 0x26, DWARF: 0xE10CFC
    struct CurrTiming* timing; // Offset: 0x54, DWARF: 0xE11480
} HorVer;

static void GetCurrentDisplayPosition(u32* hct, u32* vct)
{
	u32 hcount, vcount0, vcount;
	vcount = __VIRegs[VI_VERT_COUNT] & 0x7FF;

	do {
		vcount0 = vcount;
		hcount  = __VIRegs[VI_HORIZ_COUNT] & 0x7FF;
		vcount  = __VIRegs[VI_VERT_COUNT] & 0x7FF;
	} while (vcount0 != vcount);

	*hct = hcount;
	*vct = vcount;
}

static u32 getCurrentHalfLine(void)
{
	u32 hcount, vcount;
	GetCurrentDisplayPosition(&hcount, &vcount);

	return ((vcount - 1) << 1) + ((hcount - 1) / CurrTiming->hlw);
}

static u32 getCurrentFieldEvenOdd() { return (getCurrentHalfLine() < CurrTiming->nhlines) ? 1 : 0; }

u32 VIGetNextField(void)
{
	u32 nextField;
	int interrupt;

	interrupt = OSDisableInterrupts();
	nextField = getCurrentFieldEvenOdd() ^ 1;
	OSRestoreInterrupts(interrupt);
	return nextField ^ (HorVer.DispPosY & 1);
}

#define VI_NTSC      (0)
#define VI_PAL       (1)
#define VI_MPAL      (2)
#define VI_DEBUG     (3)
#define VI_DEBUG_PAL (4)
#define VI_EURGB60   (5)
#define VI_GCA       (6)

u32 VIGetTvFormat(void)
{
	u32 fmt;
	int interrupt;

	interrupt = OSDisableInterrupts();

	switch (CurrTvMode) {
	case VI_NTSC:
	case VI_DEBUG:
	case VI_GCA:
		fmt = VI_NTSC;
		break;
	case VI_PAL:
	case VI_DEBUG_PAL:
		fmt = VI_PAL;
		break;
	case VI_EURGB60:
	case VI_MPAL:
		fmt = CurrTvMode;
		break;
	}

	OSRestoreInterrupts(interrupt);
	return fmt;
}
//


static void  __DEMOInitRenderMode(GXRenderModeObj* pRMode) {
    if (pRMode != NULL) {
        rmode = pRMode;
    } else {
        switch (VIGetTvFormat()) {
            case 0:
                rmode = &GXNtsc480IntDf;
                break;
            case 1:
                rmode = &GXPal528IntDf;
                break;
            case 2:
                rmode = &GXMpal480IntDf;
                break;
            default:
                DisplayErrorAndLockup("C:/source/crashwoc/code/system/gc/gcInit.c",0x107,"DEMOInit: invalid TV format\n");
                //OSPanic("C:/source/crashwoc/code/system/gc/gcInit.c",0x107,"DEMOInit: invalid TV format\n");
        }
        rmode->fbWidth = 0x200;
        GXAdjustForOverscan(rmode, &rmodeobj, 0, 0x10);
        rmode = &rmodeobj;
    }
}


void DEMOInit(GXRenderModeObj* mode) {
	//OSInit();
	//DVDInit();
	//VIInit();
	//DEMOPadInit();
	__DEMOInitRenderMode(mode);
	//__DEMOInitMem();
	//VIConfigure(rmode);
	DefaultFifo = OSAllocFromHeap(__OSCurrHeap, 0x10000);
	//DefaultFifoObj = GXInit(DefaultFifo, 0x10000);
	__DEMOInitGX();
	//__DEMOInitVI();
	//VISetPostRetraceCallback(VIPostCallback);
}

void DEMOBeforeRender(void) {
	if (GPHangWorkaround) {
		//GXSetDrawSync(sysGetToken());
		//GXClearGPMetric();
	}

	//if (rmode->field_rendering)
		//GXSetViewportJitter(0.0f, 0.0f, (f32)rmode->fbWidth, (f32)rmode->efbHeight, 0.0f, 1.0f, VIGetNextField());
	//else
		//GXSetViewport(0.0f, 0.0f, (f32)rmode->fbWidth, (f32)rmode->efbHeight, 0.0f, 1.0f);

	//GXInvalidateVtxCache();
	GXInvalidateTexAll();
}
