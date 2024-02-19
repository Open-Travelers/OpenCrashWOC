#include "system/gc/GXTexture.h"


void GXInvalidateTexAll(GXContext* arg0, s32 arg1, u8* arg2)
{
    __GXFlushTextureState();
    GXWGFifo.u8 = GX_LOAD_BP_REG;
    GXWGFifo.u32 = 0x66001000;
    GXWGFifo.u8 = GX_LOAD_BP_REG;
    GXWGFifo.u32 = 0x66001100;
    __GXFlushTextureState();
}

#define GET_TILE_COUNT(a, b) (((a) + (1 << (b)) - 1) >> (b))

inline void __GXGetTexTileShift(GXTexFmt format, u32* widthTiles, u32* heightTiles)
{
	switch (format) {
	case GX_TF_I4:
	case GX_TF_C4:
	case GX_TF_CMPR:
	case GX_CTF_R4:
	case GX_CTF_Z4:
		*widthTiles  = 3;
		*heightTiles = 3;
		break;

	case GX_TF_I8:
	case GX_TF_IA4:
	case GX_TF_C8:
	case GX_TF_Z8:
	case GX_CTF_RA4:
	case GX_CTF_R8:
	case GX_CTF_G8:
	case GX_CTF_B8:
	case GX_CTF_RG8:
	case GX_CTF_Z8M:
	case GX_CTF_Z8L:
		*widthTiles  = 3;
		*heightTiles = 2;
		break;

	case GX_TF_IA8:
	case GX_TF_RGB565:
	case GX_TF_RGB5A3:
	case GX_TF_RGBA8:
	case GX_TF_C14X2:
	case GX_TF_Z16:
	case GX_TF_Z24X8:
	case GX_CTF_RA8:
	case GX_CTF_GB8:
	case 44:
	case GX_CTF_Z16L:
		*widthTiles  = 2;
		*heightTiles = 2;
		break;

	default:
		*heightTiles = 0;
		*widthTiles  = 0;
		break;
	}
}

u32 GXGetTexBufferSize(u16 width, u16 height, GXTexFmt format, GXBool mipmap, u8 max_lod)
{
	u32 widthTiles, heightTiles, tileSize, bufferSize, numX, numY, i;

	__GXGetTexTileShift(format, &widthTiles, &heightTiles);

	if (format == GX_TF_RGBA8 || format == GX_TF_Z24X8) {
		tileSize = 0x40;
	} else {
		tileSize = 0x20;
	}

	if (mipmap == GX_TRUE) {

		bufferSize = 0;
		for (i = 0; i < max_lod; i++) {
			numX = GET_TILE_COUNT(width, widthTiles);
			numY = GET_TILE_COUNT(height, heightTiles);

			bufferSize += numX * numY * tileSize;
			if (width == 1 && height == 1) {
				break;
			}

			width  = (width > 1) ? (width >> 1) : 1;
			height = (height > 1) ? (height >> 1) : 1;
		}

	} else {
		numX       = GET_TILE_COUNT(width, widthTiles);
		numY       = GET_TILE_COUNT(height, heightTiles);
		bufferSize = numX * numY * tileSize;
	}

	return bufferSize;
}

