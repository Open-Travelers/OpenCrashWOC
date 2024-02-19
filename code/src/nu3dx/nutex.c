#include "nutex.h"
#include "../system.h"

struct nusystex_s tinfo[0x400];

//static s32 initialised;
static s32 palette_128[256];
static char strBuf[128];
static s32 ntex;
struct nutex_s tex;

//MATCH NGC
void NuTexInit() {
	if (initialised != 0) {
		NuTexClose();
	}
	GS_TexReInit();
	memset(tinfo, 0, sizeof(tinfo));
	ntex = 0;
	tpid = 0;
	initialised = 1;
}

//MATCH NGC
void NuTexClose(void)
{
    s32 i;

    NudxFw_DestroyBackBufferCopy();   //empty function
    for (i = 0; i < 0x400; i++) {
        if (tinfo[i].tex.bits != NULL) {
            NuMemFree(tinfo[i].tex.bits);
            tinfo[i].tex.bits = 0;
        }
    }
    initialised = ntex = tpid = 0;
	return;
}

s32 GetTPID()
{
	return tpid;
}

//MATCH NGC
s32 NuTexCreate(struct nutex_s *nutex) {
    //replace struct D3DTexture with SDL_Surface *surface;

    if (nutex->type > NUTEX_BB) {
        tinfo[tpid].dds = NudxTx_Create(nutex, 1);
    } else {
        tinfo[tpid].dds = NudxTx_Create(nutex, 0);
    }
  tinfo[tpid].tex.bits = NULL;
  tinfo[tpid].tex.type = nutex->type;
  tinfo[tpid].tex.decal = nutex->decal;
  tinfo[tpid].tex.linear = nutex->linear;
  tinfo[tpid].tex.width = nutex->width;
  tinfo[tpid].tex.height = nutex->height;
  if (force_decal != 0) {
    tinfo[tpid].tex.decal = 1;
  }
  tpid++;
  return tpid;
}

//MATCH NGC
s32 NuTexGetDecalInfo(s32 tid) {
  return (s32)tinfo[tid + -1].tex.decal;
}

//MATCH NGC
s32 NuTexCreateFromSurface(struct nutex_s *tex, struct D3DTexture *surface) {
  tinfo[tpid].tex = *tex;

  tinfo[tpid].dds = surface;
  tinfo[tpid].tex.bits = NULL;
  tpid++;
  return tpid;
}

void NuTexDestroy(s32 id)
{
	id--;
	if (id > -1)
	{
		if (tinfo[id].tex.bits != NULL)
		{
			NuMemFree(tinfo[id].tex.bits);
			tinfo[id].tex.bits = NULL;
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
	return --tinfo[id].ref;
}

s32 NuTexPixelSize(enum nutextype_e type) {
  if (type == NUTEX_RGB24) {
    return 0x18;
  }
  if (type < NUTEX_RGBA32) {
    if ((type == NUTEX_RGB16) || (type == NUTEX_RGBA16)) {
      return 0x10;
    }
  }
  else {
    if (type == NUTEX_PAL4) {
      return 4;
    }
    if (type < NUTEX_PAL4) {
      return 0x20;
    }
    if (type == NUTEX_PAL8) {
      return 8;
    }
  }
  return 0;
}

s32 NuTexImgSize(enum nutextype_e type, s32 width, s32 height)
{
	s32 size = width * height * NuTexPixelSize(type);
	if (size < 0)
	{
		size += 7;
	}
	return size >> 3;
}

s32 NuTexPalSize(enum nutextype_e type)

{
  if (type == NUTEX_PAL4) {
    return 0x40;
  }
  if (type == NUTEX_PAL8) {
    return 0x400;
  }
  return 0;
}

//NGC MATCH
void NuTexSetTextureStates(struct numtl_s *mtl) {

  if (mtl->tid == 0) {
    NuTexSetTexture(0,0);
    NudxFw_SetTextureState(0,D3DTSS_COLOROP,1);
    NudxFw_SetTextureState(0,D3DTSS_ALPHAOP,1);
  }
  else {
    NudxFw_SetTextureState(0,D3DTSS_COLOROP,4);
    NudxFw_SetTextureState(0,D3DTSS_COLORARG1,2);
    NudxFw_SetTextureState(0,D3DTSS_COLORARG2,0);
    NudxFw_SetTextureState(0,D3DTSS_ALPHAARG1,2);
    NudxFw_SetTextureState(0,D3DTSS_ALPHAARG2,0);
    NudxFw_SetTextureState(0,D3DTSS_ALPHAOP,4);
    NudxFw_SetTextureState(0,D3DTSS_TEXCOORDINDEX,0);
    NudxFw_SetTextureState(0,D3DTSS_TEXTURETRANSFORMFLAGS,0);
        if (mtl->attrib.utc == 0) {
            GS_TexSetWrapModes(0,GX_REPEAT);
        }
        else if (mtl->attrib.utc == 2) {
            GS_TexSetWrapModes(0,GX_MIRROR);
        }
        else if (mtl->attrib.utc == 3) {
            GS_TexSetWrapModes(0,GX_CLAMP);
        }
        else {
            GS_TexSetWrapModes(0,GX_CLAMP);
        }

        if (mtl->attrib.vtc == 0) {
            GS_TexSetWrapModet(0,GX_REPEAT);
        }
        else if (mtl->attrib.vtc == 2) {
            GS_TexSetWrapModet(0,GX_MIRROR);
        }
        else if (mtl->attrib.vtc == 3) {
            GS_TexSetWrapModet(0,GX_CLAMP);
        }
        else {
            GS_TexSetWrapModet(0,GX_CLAMP);
        }
    NudxFw_SetTextureState(0,D3DTSS_MAGFILTER,2);
    NudxFw_SetTextureState(0,D3DTSS_MINFILTER,2);
    NudxFw_SetTextureState(0,D3DTSS_MIPFILTER,2);
  }
  return;
}

//91% NGC
s32 NuTexReadBitmapMM(char *filename,s32 mmlevel,struct nutex_s *tex) {

    s32 fh;
    s32 imgsize;
    void *bits;
    s32 *palsize;
    union variptr_u dst;
    s32 color;
    s32 colortmp;
    s32 colortmp2;
    s32 rowsize;
    s32 ___N;
    s32 h;
    s32 w;
    s32 m;
    s32 i;
    void *__src;
    s32 maxI;
    s32 cnt;
    struct tagBITMAPFILEHEADER bmh;
    struct tagBITMAPINFOHEADER bmi;

    if (filename == NULL) {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nutex.c",0x1fa)("assert");
    }
    fh = NuFileOpen(filename,NUFILE_READ);
    if (fh != 0) {
            if (tex != NULL)
            {
                        memset(tex,0,0x1c);
                        NuFilePos(fh);
                        NuFileRead(fh,&bmh.bfType,sizeof(struct tagBITMAPFILEHEADER) - 2);
                        NuFileRead(fh,&bmi, sizeof(struct tagBITMAPINFOHEADER));
                        switch(bmi.biBitCount){
                            case 0x20:
                                tex->type = NUTEX_RGBA32;
                               rowsize = 0;
                        m = mmlevel + 1;
                                break;
                            case 0x18:
                                tex->type = NUTEX_RGB24;
                                rowsize = 0;
                        m = mmlevel + 1;

                            break;
                            case 4:
                                tex->type = NUTEX_PAL4;
                                rowsize = 0x40;
                                NuFileRead(fh, palette_128, 0x40);
                        m = mmlevel + 1;
                                // swapcolors(palette);
                                for(maxI = 0; maxI < 0x10; maxI++)
                                {
                                    color = palette_128[maxI];
                                    colortmp = color >> 0x18 & 0xFF;
                                    colortmp2 = (color & 0xff00) >> 8;
                                    color = (color & ~0xFF000000) | (colortmp2 << 24);
                                    color = (color & ~0xFF00) | (colortmp << 8);
                                    palette_128[maxI] = color;
                                }
                            break;
                            case 8:
                                tex->type = NUTEX_PAL8;
                                rowsize = 0x400;
                                NuFileRead(fh, palette_128, 0x400);
                        m = mmlevel + 1;
                                // swapcolors(palette_128);
                                for(maxI = 0; maxI < 0x100; maxI++)
                                {
                                    color = palette_128[maxI];
                                    colortmp = color >> 0x18 & 0xFF;
                                    colortmp2 = (color & 0xff00) >> 8;
                                    color = (color & ~0xFF000000) | (colortmp2 << 24);
                                    color = (color & ~0xFF00) | (colortmp << 8);
                                    palette_128[maxI] = color;
                                }
                            break;

                            default:
                            NuErrorProlog("C:/source/crashwoc/code/nu3dx/nutex.c",0x22d)
                                ("NuTexLoadBitmap:Bad BitCount <%d> on loading bitmap <%s>",bmi.biBitCount,filename);
                            m = mmlevel + 1;
                            break;

                        }
                        tex->mmcnt = mmlevel;
                        tex->height = bmi.biHeight;
                        tex->width = bmi.biWidth;
                        i = NuTexPixelSize(tex->type);
                        maxI = tex->width * i + 7;
                        if (maxI < 0) {
                            maxI = i + 0xe;
                        }
                        ___N = maxI >> 3;
                        h = tex->height;
                        w = tex->width;
                        cnt = 0;
                        for (m = 0; m < mmlevel + 1; m++) {
                            //m--;
                            imgsize = NuTexImgSize(tex->type, w, h);
                            cnt += imgsize;
                            w >>= 1;
                            h >>= 1;
                        }// while (m != 0);
                        bits = NuMemAlloc(cnt);
                        tex->bits = bits;
                        if (rowsize != 0) {
                            palsize = (s32 *)NuMemAlloc(rowsize);
                            tex->pal = palsize;
                        }
                        else {
                            tex->pal = NULL;
                        }
                        bits = tex->bits;
                        i = NuTexImgSize(tex->type,tex->width,tex->height);
                        NuFileRead(fh,bits,i);
                        dst.voidptr = malloc_x(___N);
                        __src = tex->bits;
                        i = NuTexImgSize(tex->type,tex->width,tex->height);
                        bits = &__src[i]  - ___N;
                        while(bits > __src) {
                            memcpy(dst.voidptr,__src,___N);
                            memcpy(__src,bits,___N);
                            memcpy(bits, dst.voidptr,___N);
                            __src = ((s32)__src + ___N);
                            bits = ((s32)bits - ___N);
                        }
                        free_x(dst.voidptr);
                        NuTexImgSize(tex->type,tex->width,tex->height);
                        if (rowsize != 0) {
                            memcpy(tex->pal,palette_128,rowsize);
                        }
            }
        NuFileClose(fh);
    }
    else {
        sprintf(strBuf,"Cannot open file: %s",filename);
        return 0;
    }

    return 1;
}

//MATCH GCN
void NuTexSetTexture(u32 stage,s32 tid) {
  if (tid > 0) {
    GS_TexSelect(stage,tid);
  }
  else {
    GS_TexSelect(stage,0);
  }
  return;
}

//MATCH GCN
struct nutex_s * NuTexReadBitmap(char* fileName)
{
	struct nutex_s* ret = NuMemAlloc(sizeof(struct nutex_s));
	if (!NuTexReadBitmapMM(fileName, 0, ret))
	{
		NuMemFree(ret);
		ret = NULL;
	}
	return ret;
}

struct D3DTexture* NuTexLoadTextureFromDDSFile(char* fileName)
{
	return NULL;
}
