// This file is original in C++, but I'll try and make it in C for consistency.
#include "dxframe.h"

// If the backbuffer has been grabbed this frame.
s32 backbuffer_grabbed_this_frame = 0;
int iss3cmp;

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
