#include "nutex.h"
#include "../system.h"

struct nusystex_s tinfo[0x400];

initialised = 0;
s32 tpid = 0;
s32 ntex = 0;
struct nutex_s tex;

void NuTexInit()
{
    static s32 initialised = 0;
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

void NuTexClose(void)
{
    s32 i;
    void* temp_r3;
    void** data;

    //NudxFw_DestroyBackBufferCopy();   empty function
    i = 0x400;
    data = &tinfo->tex.bits; //data = &tinfo[0].tex.bits;
    do {
        temp_r3 = *data;
        if (temp_r3 != NULL) {
            NuMemFree(temp_r3);
            *data = NULL;
        }
        data += 0x28;
        i -= 1;
    } while (i != 0);
    initialised = i;
    tpid = i;
    ntex = i;
	return;
}

s32 GetTPID()
{
	return tpid;
}

s32 NuTexCreate(struct nutex_s *nutex)
{
  struct D3DTexture *surface;   //replace with SDL_Surface *surface;

  //surface = NudxTx_Create(nutex,(uint)(NUTEX_BB < nutex->type));  //look inside this function
  tinfo[tpid].dds = surface;
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

s32 NuTexGetDecalInfo(s32 tid)

{
  return (s32)tinfo[tid + -1].tex.decal;
}

s32 NuTexCreateFromSurface(struct nutex_s *tex, struct D3DTexture *surface)	//CHECK
{
  s32 width;
  s32 cnt;
  s32 *pal;
  s32 height;
  void *bits;

  width = tex->width;
  height = tex->height;
  cnt = tex->mmcnt;
  tinfo[tpid].tex.type = tex->type;
  tinfo[tpid].tex.width = width;
  tinfo[tpid].tex.height = height;
  tinfo[tpid].tex.mmcnt = cnt;
  bits = tex->bits;
  pal = tex->pal;
  //&tinfo[tpid].tex.decal = &tex->decal;
  tinfo[tpid].tex.bits = bits;
  tinfo[tpid].tex.pal = pal;
  tinfo[tpid].tex.bits = NULL;
  tinfo[tpid].dds = surface;
  return tpid++;
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


void NuTexSetTextureStates(struct numtl_s *mtl)
{
  u32 flag_or_float;

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
    //flag_or_float = (u32)mtl->attrib >> 0x18 & 3;     FIX IT
    flag_or_float = (mtl->attrib)._word >> 0x18 & 3;
    if (flag_or_float == 0) {
      //GS_TexSetWrapModes(0,1);
    }
    else if (flag_or_float == 2) {
      //GS_TexSetWrapModes(0,2);
    }
    else if (flag_or_float == 3) {
      //GS_TexSetWrapModes(0,0);
    }
    else {
      //GS_TexSetWrapModes(0,0);
    }
    //flag_or_float = (u32)mtl->attrib >> 0x16 & 3;     FIX IT
    flag_or_float = (mtl->attrib)._word >> 0x16 & 3;
    if (flag_or_float == 0) {
      //GS_TexSetWrapModet(0,1);
    }
    else if (flag_or_float == 2) {
      //GS_TexSetWrapModet(0,2);
    }
    else if (flag_or_float == 3) {
      //GS_TexSetWrapModet(0,0);
    }
    else {
      //GS_TexSetWrapModet(0,0);
    }
    NudxFw_SetTextureState(0,D3DTSS_MAGFILTER,2);
    NudxFw_SetTextureState(0,D3DTSS_MINFILTER,2);
    NudxFw_SetTextureState(0,D3DTSS_MIPFILTER,2);
  }
  return;
}


s32 NuTexReadBitmapMM(char* fileName, s32 mmlevel, struct nutex_s* tex)
{
    s32 palette[256];
    s32 imgsize;

	if (fileName == NULL)
	{
		error_func e = NuErrorProlog("OpenCrashWOC/code/nu3dx/nutex.c", 999);
		e("assert");
	}
	fileHandle handle = NuFileOpen(fileName, NUFILE_READ);
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
		memset(tex, 0, sizeof(struct nutex_s));
		NuFilePos(handle);
		struct tagBITMAPFILEHEADER bmpHeader;
		NuFileRead(handle, &bmpHeader.bfType, 14);
		struct tagBITMAPINFOHEADER bmi;
		NuFileRead(handle, &bmi, 40);
		u16 bitsPerPixel = bmi.biBitCount;
		if (bitsPerPixel == 8)
		{
			tex->type = NUTEX_PAL8;
			s32 palette[0x100]; // 256 colors.
			NuFileRead(handle, &palette, 0x400);
			u32 num = 0;
			for (s32 i = 0; i < 0x100; i++)
			{
				u32 color = palette[i];
				palette[i] = (color & 0x00FF00FF) | ((color & 0xFF0000) >> 24) || ((color & 0xFF) << 24); // Convert RGBA to BGRA.

				//palette[i] = (color >> 0x18) << 8 | (color >> 8) << 0x18 | color & 0xff00ff
			}
		}
		else if (bitsPerPixel == 4)
		{
			tex->type = NUTEX_PAL4;
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
			tex->type = NUTEX_RGBA16;
			//palette = 0;
		}
		else if (bitsPerPixel == 24)
		{
			tex->type = NUTEX_RGB24;
			//palette = 0;
		}
		else if (bitsPerPixel == 32)
		{
			tex->type = NUTEX_RGBA32;
			//palette = 0;
		}
		else
		{
			error_func e = NuErrorProlog("OpenCrashWOC/code/nu3dx/nutex.c", 999);
			e("NuTexLoadBitmap:Bad BitCount <%d> on loading bitmap <%s>", bitsPerPixel, fileName);
		}
		tex->mmcnt = mmlevel + 1;
		tex->height = bmi.biHeight;
		tex->width = bmi.biWidth;
		s32 size = NuTexPixelSize(tex->type);
		size *= tex->width;


		 /********************new part**********************/

		s32 var1 = size + 7;
		if(var1 < 0)
		{
			var1 = size + 0xe;
		}

		s32 h = tex->height;
  		s32 ___N = var1 >> 3;
  		var1 = 0;
  		s32 w = tex->width;

		s32 k = 0; //counter

		  do {
                    imgsize = NuTexImgSize(tex->type,w,h);
    				h = h >> 1;
    				var1 = var1 + imgsize;
    				k = k + -1;
    				w = w >> 1;

  			} while (k != 0);

		void* bits = NuMemAlloc(var1);
  		tex->bits = bits;

		  if (palette == 0)
            {

    			tex->pal = NULL;
  			}

		else {
    				s32 palsize = (int *)NuMemAlloc(palette);
    				tex->pal = palsize;
  			}
  		bits = tex->bits;

		imgsize = NuTexImgSize(tex->type,tex->width,tex->height);
        NuFileRead(handle,bits,imgsize);
		union variptr_u dst;
		dst.voidptr = malloc_x(___N);
		void * __src = tex->bits;
		imgsize = NuTexImgSize(tex->type,tex->width,tex->height);
		bits = (void *)((int)__src + (imgsize - ___N));

		  if (__src < bits)
		  {

			  void* ptr1 = NULL;
			  void* ptr2 = NULL;
             do{
					memcpy(dst.voidptr,__src,___N);
					ptr2 = (void *)((int)__src + ___N);
					memcpy(__src,bits,___N);
					ptr1 = (void *)((int)bits - ___N);
					memcpy(bits,dst.voidptr,___N);
					bits = ptr1;
					__src = ptr2;
               } while (ptr2 < ptr1);
		  }

		free_x(dst.voidptr);
		NuTexImgSize(tex->type,tex->width,tex->height);

		if (palette !=0)
		{
			memcpy(tex->pal,palette,sizeof(palette));
		}

	}
}

void NuTexSetTexture(u32 stage,int tid)

{
  if (tid < 1) {
    //GS_TexSelect(stage,0);
  }
  else {
    //GS_TexSelect(stage,tid);
  }
  return;
}

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
