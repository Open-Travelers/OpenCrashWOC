#include "system/gc/__GXInit.h"


void GXSetDispCopySrc(u16 left, u16 top, u16 width, u16 height)
{
	gx->cpDispSrc = 0;
	GX_SET_REG(gx->cpDispSrc, left, 22, 31);
	GX_SET_REG(gx->cpDispSrc, top, 12, 21);
	GX_SET_REG(gx->cpDispSrc, 0x49, 0, 7);

	gx->cpDispSize = 0;
	GX_SET_REG(gx->cpDispSize, width - 1, 22, 31);
	GX_SET_REG(gx->cpDispSize, height - 1, 12, 21);
	GX_SET_REG(gx->cpDispSize, 0x4A, 0, 7);
}

void GXSetTexCopySrc(u16 left, u16 top, u16 width, u16 height)
{
	gx->cpTexSrc = 0;
	GX_SET_REG(gx->cpTexSrc, left, 22, 31);
	GX_SET_REG(gx->cpTexSrc, top, 12, 21);
	GX_SET_REG(gx->cpTexSrc, 0x49, 0, 7);

	gx->cpTexSize = 0;
	GX_SET_REG(gx->cpTexSize, width - 1, 22, 31);
	GX_SET_REG(gx->cpTexSize, height - 1, 12, 21);
	GX_SET_REG(gx->cpTexSize, 0x4A, 0, 7);
}

void GXSetCopyClear(GXColor clearColor, u32 clearZ)
{
	u32 reg = 0;
	GX_SET_REG(reg, clearColor.r, 24, 31);
	GX_SET_REG(reg, clearColor.a, 16, 23);
	GX_SET_REG(reg, 0x4F, 0, 7);
	GX_BP_LOAD_REG(reg);

	reg = 0;
	GX_SET_REG(reg, clearColor.b, 24, 31);
	GX_SET_REG(reg, clearColor.g, 16, 23);
	GX_SET_REG(reg, 0x50, 0, 7);
	GX_BP_LOAD_REG(reg);

	reg = 0;
	GX_SET_REG(reg, clearZ, 8, 31);
	GX_SET_REG(reg, 0x51, 0, 7);
	GX_BP_LOAD_REG(reg);

	gx->bpSentNot = GX_FALSE;
}

void GXCopyDisp(void* dest, GXBool doClear)
{
	u32 reg;
	u32 newDest;
	GXBool check;

	if (doClear) {
		reg = gx->zmode;
		GX_SET_REG(reg, 1, 31, 31);
		GX_SET_REG(reg, 7, 28, 30);
		GX_BP_LOAD_REG(reg);

		reg = gx->cmode0;
		GX_SET_REG(reg, 0, 31, 31);
		GX_SET_REG(reg, 0, 30, 30);
		GX_BP_LOAD_REG(reg);
	}

	check = GX_FALSE;
	if ((doClear || (gx->peCtrl & 0x7) == 3) && (gx->peCtrl >> 6 & 0x1) == 1) {
		check = GX_TRUE;
		reg   = gx->peCtrl;
		GX_SET_REG(reg, 0, 25, 25);
		GX_BP_LOAD_REG(reg);
	}

	GX_BP_LOAD_REG(gx->cpDispSrc);
	GX_BP_LOAD_REG(gx->cpDispSize);
	GX_BP_LOAD_REG(gx->cpDispStride);

	newDest = (u32)dest & 0x3FFFFFFF;
	reg     = 0;
	GX_SET_REG(reg, newDest >> 5, 11, 31);
	GX_SET_REG(reg, 0x4B, 0, 7);
	GX_BP_LOAD_REG(reg);

	GX_SET_REG(gx->cpDisp, doClear, 20, 20);
	GX_SET_REG(gx->cpDisp, 1, 17, 17);
	GX_SET_REG(gx->cpDisp, 0x52, 0, 7);
	GX_BP_LOAD_REG(gx->cpDisp);

	if (doClear) {
		GX_BP_LOAD_REG(gx->zmode);
		GX_BP_LOAD_REG(gx->cmode0);
	}

	if (check) {
		GX_BP_LOAD_REG(gx->peCtrl);
	}

	gx->bpSentNot = GX_FALSE;
}
