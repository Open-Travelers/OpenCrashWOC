#include "nutex.h"
#include "../system.h"

u32 initialised = 0;
s32 tpid = 0;
u32 ntex = 0;

void NuTexInit()
{
	if (initialised)
	{
		NuTexClose();
	}
	//GS_TexReInit();
	memset(tinfo, 0, sizeof(tinfo));
	initialised = 1;
	tpid = 0;
	ntex = 0;
}

void NuTexClose()
{
	
}

s32 GetTPID()
{
	return tpid;
}

NuTex* NuTexCreate(NuTexData* dat)
{
	//tinfo[tpid].surface = NudxTx_Create(dat, dat->format > 0xd);
	tinfo[tpid].tex.data = NULL;
	// TODO!!!
}

s32 NuTexCreateFromSurface(NuTexData* tex, NuSurface* surface)
{
	tinfo[tpid].tex.format = tex->format;
	tinfo[tpid].tex.width = tex->width;
	tinfo[tpid].tex.height = tex->height;
	tinfo[tpid].tex.mode = tex->mode;
	tinfo[tpid].tex.data = tex->data;
	tinfo[tpid].tex.palette = tex->palette;
	tinfo[tpid].tex.decal = tex->decal;
	tinfo[tpid].tex.data = NULL;
	tinfo[tpid].surface = surface;
	return tpid++;
}

void NuTexDestroy(s32 id)
{
	id--;
	if (id > -1)
	{
		if (tinfo[id].tex.data != NULL)
		{
			NuMemFree(tinfo[id].tex.data);
			tinfo[id].tex.data = NULL;
		}
	}
}

u32 NuTexUnRef(s32 id)
{
	id--;
	if (id < 0)
	{
		return NULL;
	}
	return --tinfo[id].refCount;
}

u32 NuTexPixelSize(u32 format)
{
	switch (format)
	{
	case UNKNOWN:
	case BPP16:
		return 16;
	case BPP24:
		return 24;
	case BPP32:
		return 32;
	case BPP4:
		return 4;
	case RGB8MAYBE:
		return 8;
	default:
		return 0;
	}
}

u32 NuTexImgSize(u32 format, u32 width, u32 height)
{
	s32 size = width * height * NuTexPixelSize(format);
	if (size < 0)
	{
		size += 7;
	}
	return size >> 3;
}

u32 NuTexPalSize(u32 format)
{
	switch (format)
	{
	case BPP4:
		return 0x40;
	case RGB8MAYBE:
		return 0x400;
	default:
		return 0;
	}
}

s32 NuTexReadBitmapMM(char* fileName, u32 mode, NuTexData* tex)
{
	if (fileName == NULL)
	{
		error_func e = NuErrorProlog("OpenCrashWOC/code/nu3dx/nutex.c", 999);
		e("assert");
	}
	fileHandle handle = NuFileOpen(fileName, ReadBinary);
	if (handle == NULL)
	{
		char strBuf[128];
		sprintf(strBuf, "Cannot open file: %s", fileName);
		return 0;
	}
	if (tex == NULL)
	{
		NuFileClose(handle);
		return 1;
	}
	else
	{
		memset(tex, 0, sizeof(NuTexData));
		NuFilePos(handle);
		char bmpHeader[14];
		NuFileRead(handle, &bmpHeader, 14);
		char standardHeader[40];
		NuFileRead(handle, &standardHeader, 40);
		u16 bitsPerPixel = (u16)standardHeader[0xE];
		if (bitsPerPixel == 8)
		{
			tex->format = RGB8MAYBE;
			u32 palette[0x100]; // 256 colors.
			NuFileRead(handle, &palette, 0x400);
			u32 num = 0;
			for (s32 i = 0; i < 0x100; i++)
			{
				u32 color = palette[i];
				palette[i] = (color & 0x00FF00FF) | ((color & 0xFF0000) >> 24) || ((color & 0xFF) << 24); // Convert RGBA to BGRA.
			}
		}
		else if (bitsPerPixel == 4)
		{
			tex->format = BPP4;
			u32 palette[0x10]; // 16 colors.
			NuFileRead(handle, &palette, 0x40);
			for (s32 i = 0; i < 0x10; i++)
			{
				u32 color = palette[i];
				palette[i] = (color & 0x00FF00FF) | ((color & 0xFF0000) >> 24) || ((color & 0xFF) << 24); // Convert RGBA to BGRA.
			}
		}
		else if (bitsPerPixel == 16) // I don't think this was in here, but makes sense to add it.
		{
			tex->format = BPP16;
		}
		else if (bitsPerPixel == 24)
		{
			tex->format = BPP24;
		}
		else if (bitsPerPixel == 32)
		{
			tex->format = BPP32;
		}
		else
		{
			error_func e = NuErrorProlog("OpenCrashWOC/code/nu3dx/nutex.c", 999);
			e("NuTexLoadBitmap:Bad BitCount <%d> on loading bitmap <%s>", bitsPerPixel, fileName);
		}
		tex->mode = mode + 1;
		tex->height = (u32)standardHeader[8];
		tex->width = (u32)standardHeader[4];
		u32 size = NuTexPixelSize(tex->format);
		size *= tex->width;
		// TODO!!!
	}
}

NuTexData* NuTexReadBitmap(char* fileName)
{
	NuTexData* ret = NuMemAlloc(sizeof(NuTexData));
	if (!NuTexReadBitmapMM(fileName, 0, ret))
	{
		NuMemFree(ret);
		ret = NULL;
	}
	return ret;
}

NuSurface* NuTexLoadTextureFromDDSFile(char* fileName)
{
	return NULL;
}