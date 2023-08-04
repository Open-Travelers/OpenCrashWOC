// This file is original in C++, but I'll try and make it in C for consistency.
#include "dxframe.h"

// If the backbuffer has been grabbed this frame.
s32 backbuffer_grabbed_this_frame = 0;
int iss3cmp;
char DebugText[256];

// Back buffer TID.
s32 backbuffer_tid = 0x207F;

void ResetFwGlobals()
{
	g_pddsZBuffer = NULL;
	g_dwRenderWidth = 0;
	g_dwRenderHeight = 0;
	g_pd3dDevice = NULL;
	g_pddsBackBuffer = NULL;
	InitRenderTargets();
  	return;
}


void InitRenderTargets(void)

{
  int last;
  int next;
  rendertargetlist_s *tglist;
  int i;

  memset(g_pRTArray,0,0x1c0);
  g_pRTArray[0].last = -1;
  g_pRTArray[0].next = 1;
  i = 0xe;
  next = 1;
  tglist = g_pRTArray;
  do {
    last = next;
    next = last + 1;
    tglist[1].last = last + -1;
    tglist[1].next = next;
    i = i + -1;
    tglist = tglist + 1;
  } while (i != 0);
  g_pRTArray[next].last = last;
  g_pRTArray[next].next = -1;
  g_dwNumRT = 0;
  g_dwCurrentRT = -1;
  g_dwFreeRT = 0;
  g_dwAllocRT = -1;
  return;
}


void initd3d(void)

{
  NudxFw_Init(NULL,NULL,0);
  return;
}

void NudxFw_DestroyBackBufferCopy(void)

{
  return;
}

void NudxFw_MakeBackBufferCopy(s32 force)
{
	if ((backbuffer_grabbed_this_frame == 0) || (force != 0))
	{
		//GS_CopyTextureFromFB(); This is a stub function. Is it supposed to do something?
		NudxFw_SetBackBufferCopied(1);
	}
	return;
}



s32 NudxFw_GetBackBufferCopyTID()
{
	return backbuffer_tid;
}


void NudxFw_SetBackBufferCopied(s32 copied)
{
	backbuffer_grabbed_this_frame = copied;
}


s32 NudxFw_Init()
{
	ResetFwGlobals();
	s32 code = CreateEnvironment(NULL);
	if (code > -1)
	{
		code = 0;
	}
	return code;
}


s32 NudxFw_SetRenderTargetSurface(struct D3DSurface *RenderTarget,struct D3DSurface *Zbuffer)

{
  return 0;
}


s32 NudxFw_BeginScene(s32 hRT)

{
  if ((hRT + -1 != g_dwCurrentRT) || (hRT != -1)) {
    SetRenderTarget(hRT);
  }
  //GS_BeginScene();
  return 0;
}

s32 SetRenderTarget(u32 hRT)

{
  return 0;
}

s32 NudxFw_Clear(s32 flags,s32 colour,float depth)

{
  //GS_RenderClear(flags,colour,depth,0);
  return 0;
}

int NudxFw_FlipScreen(int hRT,int ss)

{
  NudxFw_MakeBackBufferCopy(0);
  GS_FlipScreen();
  GS_RenderClear(3,0,1.0,0);
  if (hLoadScreenThread == NULL) {
    NuAnimUV();
  }
  return 0;
}


s32 NudxFw_SetRenderState(enum _D3DRENDERSTATETYPE state, u32 data)

{
  //GS_SetRenderState(state,data);	//Empty function
  return 0;
}

s32 NudxFw_SetTextureState(u32 stage, enum _D3DTEXTURESTAGESTATETYPE state, u32 data)
{
  //GS_SetTextureStageState(); //Empty function
  return 0;
}

struct D3DSurface * NudxFw_GetBackBuffer(void)

{
  return g_pddsBackBuffer;
}


struct D3DSurface * NudxFw_GetZBuffer(void)

{
  return g_pddsZBuffer;
}



s32 CreateEnvironment(struct HWND__ *hwnd)
{
  s32 RT;
  s32 next;

  next = CreateDirect3D(hwnd);
  if (-1 < next) {
    GetFullscreenBuffers(g_pd3dDevice);
    RT = g_dwNumRT;
    next = g_pRTArray[0].next;
    if (g_dwNumRT == 0) {
      g_pRTArray[0].pddsStencilBuffer = g_pddsStencilBuffer;
      g_pRTArray[0].width = g_dwRenderWidth;
      g_pRTArray[0].height = g_dwRenderHeight;
      g_pRTArray[0].pddsRenderTarget = g_pddsBackBuffer;
      g_pRTArray[0].pddsZBuffer = g_pddsZBuffer;
      g_pRTArray[g_pRTArray[0].next].last = -1;
      g_dwNumRT = 1;
      g_pRTArray[0].next = g_dwAllocRT;
      g_dwFreeRT = next;
      if (g_dwAllocRT != -1) {
        g_pRTArray[g_dwAllocRT].last = 0;
      }
      g_dwAllocRT = RT;
      next = 0;
    }
    else {
      next = -0x7e000000;
    }
  }
  return next;
}

s32 GetFullscreenBuffers(struct D3DDevice *lpdev)

{
  g_rcScreenRect.left = 0;
  g_rcScreenRect.right = 0x280;
  g_dwRenderWidth = 0x280;
  g_dwRenderHeight = 0x1e0;
  g_rcScreenRect.top = 0;
  g_rcScreenRect.bottom = 0x1e0;
  return 0;
}

s32 CreateDirect3D(struct HWND__ *hwnd)

{
  struct _GS_VIEWPORT gsvp;
  struct _D3DVIEWPORT8 vp;

  //GS_RenderClear(1,0,1.0,0);
  //GS_RenderClear(2,0,1.0,0);
  //GS_RenderClear(0xf0,0,0.0,0);
  GS_FlipScreen();
  //GS_RenderClear(0xf0,0,0.0,0);
  GS_FlipScreen();
  memset(&vp,0,0x18);
  vp.Height = 0x1c0;
  vp.Width = PHYSICAL_SCREEN_X;
  vp.MaxZ = 1.0;
  gsvp.X = vp.X;
  gsvp.Y = vp.Y;
  gsvp.width = PHYSICAL_SCREEN_X;
  gsvp.height = 0x1c0;
  gsvp.MaxZ = 1.0;
  gsvp.MinZ = vp.MinZ;
  //GS_SetViewport(&gsvp);
  return 0;
}


struct D3DTexture * NudxTx_Create(struct nutex_s *texture,int rendertargetflag)		//TODO!!!
{
    /*
    DWARF INFO


    long data;
    long data2;
    unsigned char* inbits24;
    unsigned short* inbits16;
    long* inbits32;
    long* outbits;
    long* outline;
    unsigned short* outline16;
    unsigned char* inbits8;
    int s;
    int t;
    unsigned char* b1;
    int imagesize;
    int mapix;
    int ix;
    int format;
    int solid;
    int blended;
    int transparent;
    nutextype_e type;
    int width;
    int height;
    void* bits;
    int* pal;
    float alphatest;

    */
  char *pbVar1;
  char *pbVar2;
  char cVar3;
  char bVar4;
  float fVar5;
  u32 uVar6;
  short sVar7;
  int tpid;
  u32 *bits_00;
  u32 uVar8;
  u32 *puVar9;
  u32 *puVar10;
  u32 uVar11;
  int *pal;
  enum nutextype_e type;
  u32 uVar12;
  int height;
  void *bits;
  int width;
  int iVar13;
  u32 unaff_r30;
  int mmcnt;
  int width_00;

  type = texture->type;
  width = texture->width;
  height = texture->height;
  bits = texture->bits;
  pal = texture->pal;
  if (iss3cmp != 0) {
    mmcnt = texture->mmcnt;
    tpid = GetTPID();
    //GS_TexCreateNU(type,width,height,bits,mmcnt,rendertargetflag,tpid);
    return NULL;
  }
  if (type == 0x82) {
    mmcnt = texture->mmcnt;
    tpid = GetTPID();
    //GS_TexCreateNU(0x82,width,height,bits,mmcnt,rendertargetflag,tpid);
    return NULL;
  }
  if (type == 0x81) {
    mmcnt = texture->mmcnt;
    tpid = GetTPID();
    //GS_TexCreateNU(0x81,width,height,bits,mmcnt,rendertargetflag,tpid);
    return NULL;
  }
  if (type == 0x80) {
    mmcnt = texture->mmcnt;
    tpid = GetTPID();
    //GS_TexCreateNU(0x80,width,height,bits,mmcnt,rendertargetflag,tpid);
    return NULL;
  }
  tpid = width * height;
  uVar12 = 0;
  uVar11 = 0;
  mmcnt = 0;
  bits_00 = (u32 *)malloc_x(tpid * 4);
  texture->decal = 0;
  texture->linear = 0;
  if (type == NUTEX_RGB24) {
    sprintf(DebugText,"RGB24");
    tpid = 0;
    puVar10 = bits_00;
    if (0 < height) {
      do {
        tpid = tpid + 1;
        puVar9 = puVar10;
        iVar13 = width;
        if (0 < width) {
          do {
                    /* WARNING: Load size is inaccurate */
            //bVar4 = *bits;
            uVar12 = uVar12 + 1;
            pbVar1 = (char *)((int)bits + 1);
            pbVar2 = (char *)((int)bits + 2);
            bits = (void *)((int)bits + 3);
            *puVar9 = ((u32)bVar4 + (u32)*pbVar1 * 0x100 + (u32)*pbVar2 * 0x10000) - 0x1000000;
            cVar3 = *(char *)((int)puVar9 + 3);
            *(char *)((int)puVar9 + 3) = *(char *)((int)puVar9 + 1);
            *(char *)((int)puVar9 + 1) = cVar3;
            iVar13 = iVar13 + -1;
            puVar9 = puVar9 + 1;
          } while (iVar13 != 0);
        }
        puVar10 = puVar10 + width;
      } while (tpid < height);
    }
  }
  else if (type < NUTEX_RGB24) {
    sprintf(DebugText,"RGB16/RGBA16");
    tpid = 0;
    puVar10 = bits_00;
    if (0 < height) {
      do {
        tpid = tpid + 1;
        puVar9 = puVar10;
        iVar13 = width;
        if (0 < width) {
          do {
                    /* WARNING: Load size is inaccurate */
            //uVar8 = (u32)*bits;
            uVar8 = (uVar8 & 0x1f) << 3 | (uVar8 & 0x3e0) << 6 | (uVar8 & 0x7c00) << 9;
            /*if ((*bits & 1) != 0) {
              uVar8 = uVar8 | 0xff000000;
            }*/
            *puVar9 = uVar8;
            bits = (void *)((int)bits + 2);
            cVar3 = *(char *)((int)puVar9 + 3);
            *(char *)((int)puVar9 + 3) = *(char *)((int)puVar9 + 1);
            *(char *)((int)puVar9 + 1) = cVar3;
            iVar13 = iVar13 + -1;
            puVar9 = puVar9 + 1;
          } while (iVar13 != 0);
        }
        puVar10 = puVar10 + width;
      } while (tpid < height);
    }
  }
  else if (type == NUTEX_RGBA32) {
    sprintf(DebugText,"RGB32");
    tpid = 0;
    puVar10 = bits_00;
    if (0 < height) {
      do {
        tpid = tpid + 1;
        puVar9 = puVar10;
        iVar13 = width;
        if (0 < width) {
          do {
                    /* WARNING: Load size is inaccurate */
            //*puVar9 = *bits;
            /*fVar5 = (float)((double)CONCAT44(0x43300000,(int)*(char *)((int)puVar9 + 3) ^ 0x800000 00
                                            ) - 4503601774854144.0) / 255.0;*/
            if (0.063 <= fVar5) {
              if (fVar5 <= 0.9) {
                uVar11 = uVar11 + 1;
              }
              else {
                uVar12 = uVar12 + 1;
              }
            }
            else {
              mmcnt = mmcnt + 1;
            }
            bits = (void *)((int)bits + 4);
            iVar13 = iVar13 + -1;
            puVar9 = puVar9 + 1;
          } while (iVar13 != 0);
        }
        puVar10 = puVar10 + width;
      } while (tpid < height);
    }
  }
  else if (type < NUTEX_PAL4_S) {
    if (type != NUTEX_PAL4) {
      if (type == NUTEX_PAL8) {
        sprintf(DebugText,"PAL8");
      }
    }
    else {
      sprintf(DebugText,"PAL4");
    }
    uVar8 = 0;
    if (0 < tpid) {
      puVar10 = bits_00;
      do {
        if (type != NUTEX_PAL4) {
          if (type == NUTEX_PAL8) {
            //uVar6 = (u32)*(char *)((int)bits + uVar8);
            goto LAB_800cf184;
          }
        }
        else {
          if ((uVar8 & 1) == 0) {
            //uVar6 = *(char *)((int)bits + (int)uVar8 / 2) & 0xf;
          }
          else {
            //uVar6 = (u32)(*(char *)((int)bits + (int)uVar8 / 2) >> 4);
          }
LAB_800cf184:
          unaff_r30 = pal[uVar6];
        }
        *puVar10 = unaff_r30;
        /*fVar5 = (float)((double)CONCAT44(0x43300000,(int)*(char *)puVar10 ^ 0x80000000) -
                       4503601774854144.0) / 255.0;*/
        if (0.063 <= fVar5) {
          if (fVar5 <= 0.9) {
            uVar11 = uVar11 + 1;
          }
          else {
            uVar12 = uVar12 + 1;
          }
        }
        else {
          mmcnt = mmcnt + 1;
        }
        uVar8 = uVar8 + 1;
        puVar10 = puVar10 + 1;
      } while ((int)uVar8 < tpid);
    }
  }
  else {
    //e = NuErrorProlog("C:/source/crashwoc/code/system/crashlib.c",0x187);
    //(*e)("NudxTx_Create:\tUnknown texture type!");
  }
  if (((uVar12 == 0) || (uVar11 == 0)) || (mmcnt == 0)) {
    sVar7 = 0;
  }
  else {
   /* if (0.6 <= (float)((double)CONCAT44(0x43300000,uVar11 ^ 0x80000000) - 4503601774854144.0) /
               (float)((double)CONCAT44(0x43300000,uVar12 ^ 0x80000000) - 4503601774854144.0))*/
    goto LAB_800cf290;
    sVar7 = 1;
  }
  texture->decal = sVar7;
LAB_800cf290:
  mmcnt = texture->mmcnt;
  width_00 = texture->width;
  iVar13 = texture->height;
  tpid = GetTPID();
  //GS_TexCreateNU(type,width_00,iVar13,bits_00,mmcnt,rendertargetflag,tpid);
  free_x(bits_00);
  DebugText[0] = '\0';
  return NULL;
}

/*  WIP
struct D3DTexture* NudxTx_Create(struct nutex_s* texture, s32 rendertargetflag) {
  u8 *pbVar1;
  u8 *pbVar2;
  char cVar3;
  char bVar4;
  float fVar5;
  u32 uVar6;
  short sVar7;
  s32 tpid;
  long* outbits;
  long* px_buffer;
  u32 uVar8;
  char *puVar9;
  long *puVar10;
  long *puVar11;
  long *piVar9;
  u32 uVar10;
  u32 uVar11;
  u32 uVar12;
  s32 *pal;
  enum nutextype_e type;
  s32 height;
  u8 *bits;
  s32 width;
  s32 iVar12;
  s32 iVar14;
  u32 unaff_r30;
  s32 mmcnt;
  s32 width_00;

  type = texture->type;
  width = texture->width;
  height = texture->height;
  bits = texture->bits;
  pal = texture->pal;
  if (iss3cmp != 0) {
    mmcnt = texture->mmcnt;
    tpid = GetTPID();
    GS_TexCreateNU(type,width,height,bits,mmcnt,rendertargetflag,tpid);
    return NULL;
  }
  if (type == 0x82) {
    mmcnt = texture->mmcnt;
    tpid = GetTPID();
    GS_TexCreateNU(0x82,width,height,bits,mmcnt,rendertargetflag,tpid);
    return NULL;
  }
  if (type == 0x81) {
    mmcnt = texture->mmcnt;
    tpid = GetTPID();
    GS_TexCreateNU(0x81,width,height,bits,mmcnt,rendertargetflag,tpid);
    return NULL;
  }
  if (type == 0x80) {
    mmcnt = texture->mmcnt;
    tpid = GetTPID();
    GS_TexCreateNU(0x80,width,height,bits,mmcnt,rendertargetflag,tpid);
    return NULL;
  }
  tpid = width * height;
  uVar11 = 0;
  uVar10 = 0;
  mmcnt = 0;
  outbits = (long*)malloc_x(tpid * 4);
  texture->decal = 0;
  texture->linear = 0;
  if (type == NUTEX_RGB24) {
    sprintf(DebugText,"RGB24"); // print texture type we're decoding
    tpid = 0;
    px_buffer = outbits; // px_buffer is the output buffer for the texture
    if (0 < height) {
        //loading 24 bits of texture data at a time and storing them into a pointer, it then advances this pointer 3 bytes?
      do {
        tpid = tpid + 1; // increment current row
        puVar9 = (char *)px_buffer; // puVar9 is output buffer for this row of pixels
        iVar12 = width;
        if (0 < width) {
          do {
               // NUTEXes seems to be little-endian based on this code
            bVar4 = *bits; // the blue component
            uVar11 = uVar11 + 1;
            pbVar1 = bits + 1; // the green component
            pbVar2 = bits + 2; // the red component
            bits = bits + 3; // advance bits to the next pixel
            // Combine bits into a single color, subtracting 0x1000000 sets the upper two bytes to FF which means solid alpha
            *(uint *)puVar9 = (uint)bVar4 + (uint)*pbVar1 * 0x100 + (uint)*pbVar2 * 0x10000 + -0x1000000;
            cVar3 = puVar9[3]; // gets red component for swap
            puVar9[3] = puVar9[1]; // swaps blue and red components
            puVar9[1] = cVar3; // finishes swap, resulting in a big-endian ARBG color
            iVar12 = iVar12 + -1;
            puVar9 = puVar9 + 4; // move puVar9 forward one pixel
          } while (iVar12 != 0);
        }
        px_buffer = px_buffer + width;  // move px_buffer forward by width pixels (to next row)
      } while (tpid < height);
    }
  }
    else if (type < NUTEX_RGB24) {
    sprintf(DebugText,"RGB16/RGBA16");
    tpid = 0;
    px_buffer = outbits;
    if (0 < height) {
      do {
        tpid = tpid + 1;
        puVar10 = px_buffer;
        iVar12 = width;
        if (0 < width) {
          do {
            uVar8 = (u32)*(u16 *)bits;
            uVar8 = (uVar8 & 0x1f) << 3 | (uVar8 & 0x3e0) << 6 | (uVar8 & 0x7c00) << 9;
            if ((*(u16 *)bits & 1) != 0) {
              uVar8 = uVar8 | 0xff000000;
            }
            *(u32 *)puVar10 = uVar8;
            bits = (u8 *)((int)bits + 2);
            cVar3 = puVar10[3];
            puVar10[3] = puVar10[1];
            puVar10[1] = cVar3;
            iVar12 = iVar12 + -1;
            puVar10 = puVar10 + 4;
          } while (iVar12 != 0);
        }
        px_buffer = px_buffer + width;
      } while (tpid < height);
    }
  }
    else if (type == NUTEX_RGBA32) {
    sprintf(DebugText,"RGB32");
    tpid = 0;
    px_buffer = outbits;
    if (0 < height) {
      do {
        tpid = tpid + 1;
        piVar9 = px_buffer;
        iVar12 = width;
        if (0 < width) {
          do {
            *piVar9 = *(int *)bits;
                          //float conversion fVar5 = (float)*(puVar9 + 3) / 255.0f;
            fVar5 = (float)((double)CONCAT44(0x43300000,(int)*(char *)((int)piVar9 + 3) ^ 0x80000000
                                            ) - 4503601774854144.0) / 255.0;
            if (0.063 <= fVar5) {
              if (fVar5 <= 0.9) {
                uVar10 = uVar10 + 1;
              }
              else {
                uVar11 = uVar11 + 1;
              }
            }
            else {
              mmcnt = mmcnt + 1;
            }
            bits = (u8 *)((int)bits + 4);
            iVar12 = iVar12 + -1;
            piVar9 = piVar9 + 1;
          } while (iVar12 != 0);
        }
        px_buffer = px_buffer + width;
      } while (tpid < height);
    }
  }
  else if (type < NUTEX_PAL4_S) {
    if (type != NUTEX_PAL4) {
      if (type == NUTEX_PAL8) {
        sprintf(DebugText,"PAL8");
      }
    }
    else {
      sprintf(DebugText,"PAL4");
    }
        uVar8 = 0;
    if (0 < tpid) {
      piVar9 = outbits;
      do {
        if (type != NUTEX_PAL4) {
          if (type == NUTEX_PAL8) {
            uVar6 = (u32)bits[uVar8];
            goto LAB_800cf184;
          }
        }
        else {
          if ((uVar8 & 1) == 0) {
            uVar6 = bits[(int)uVar8 / 2] & 0xf;
          }
          else {
            uVar6 = (u32)(bits[(int)uVar8 / 2] >> 4);
          }
LAB_800cf184:
          unaff_r30 = pal[uVar6];
        }
        *piVar9 = unaff_r30;
        fVar5 = (float)((double)CONCAT44(0x43300000,(int)*(char *)piVar9 ^ 0x80000000) -
                       4503601774854144.0) / 255.0;
        if (0.063 <= fVar5) {
          if (fVar5 <= 0.9) {
            uVar10 = uVar10 + 1;
          }
          else {
            uVar11 = uVar11 + 1;
          }
        }
        else {
          mmcnt = mmcnt + 1;
        }
        uVar8 = uVar8 + 1;
        piVar9 = piVar9 + 1;
      } while ((int)uVar8 < tpid);
    }
  }
  else {
      NuErrorProlog("C:/source/crashwoc/code/system/crashlib.c",0x187)("NudxTx_Create:\tUnknown texture type!");
  }
  if (((uVar11 == 0) || (uVar10 == 0)) || (mmcnt == 0)) {
    sVar7 = 0;
  }
  else {
       //float conversion if (0.6 <= (float)uVar11/(float)uVar12)
    if (0.6 <= (float)((double)CONCAT44(0x43300000,uVar10 ^ 0x80000000) - 4503601774854144.0) /
               (float)((double)CONCAT44(0x43300000,uVar11 ^ 0x80000000) - 4503601774854144.0))
        //
    goto LAB_800cf290;
    sVar7 = 1;
  }
  texture->decal = sVar7;
LAB_800cf290:
  mmcnt = texture->mmcnt;
  width_00 = texture->width;
  iVar12 = texture->height;
  tpid = GetTPID();
  GS_TexCreateNU(type,width_00,iVar12,&outbits,mmcnt,rendertargetflag,tpid);
  free_x(&outbits);
  DebugText[0] = '\0';
  return NULL;
}
*/
