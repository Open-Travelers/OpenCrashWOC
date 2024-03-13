#include "gs.h"
#include "types.h"


s32 GS_ForceNoAlphaCompareFlag;
s32 GS_WorldMatIsIdentity;
s32 GS_IsNewFrame;
s32 fadeval;
s32 GS_CurrentVertDesc;
s32 GS_Parallax;
s32 PrintError;
char Lockupbuffer1[256];
char Lockupbuffer2[256];
char Lockupbuffer3[256];
char namesave[128];
char GS_CommandBuffer[128];
s32 GS_CurrentTexture;
static int GS_ZCompareMode;
static int GS_ZCmpUpdate;
static int GS_ZCmpEnable;
static int GS_BlendEnable;
static int GS_BlendDest;
static int GS_BlendSrc;
struct _GS_VECTOR3* GS_BlendSource;
float GS_MatProjection[4][4];
float GS_MatWorld[4][4];
float GS_MatView[4][4];
struct _GSMATRIX GS_ViewIdentity;
static struct _GS_VIEWPORT GS_ViewPort;

//NGC MATCH
void GS_SetFBCopyTexturePause(void) {
    GXLoadTexObj(&GS_FrameBufferCopyPause.tobj,GX_TEXMAP0);
}

//NGC MATCH
void GS_SetAlphaCompareForce(int arg0) {
  GS_ForceNoAlphaCompareFlag = arg0;
  if (arg0 != 0) {
    GXSetAlphaCompare(GX_GREATER,0,GX_AOP_OR,GX_GREATER,0);
  }
  return;
}

//NGC MATCH
void GS_SetAlphaCompare(int Func,int Ref) {
  if (GS_ForceNoAlphaCompareFlag != 0) {
    GXSetAlphaCompare(GX_ALWAYS,0,GX_AOP_OR,GX_ALWAYS,0);
  }
  else {
    GXSetAlphaCompare(Func,(u8)Ref,GX_AOP_OR,Func,(u8)Ref);
  }
  return;
}

//NGC MATCH
void GS_ARGBTORGBA(u32 *colors,int count) {
  if (count >= 1) {
      while (count != 0) {
        colors[6] = colors[6] << 8 | colors[6] >> 0x18;
        colors = colors + 9;
        count--;
      }
  }
  return;
}

//NGC MATCH
void GS_BeginScene(void) {
  GS_WorldMatIsIdentity = 0;
  GS_SetZCompare(1,1,GX_ALWAYS);
  GS_SetBlendSrc(1,1,0);
  if (GS_IsNewFrame != 0) {
    DEMOBeforeRender();
  }
  GS_IsNewFrame = 0;
  GXSetViewport(0.0f,0.0f,640.0f,448.0f,0.0f,1.0f);
  GXSetScissor(0,0,0x280,0x1c0);
  GXSetClipMode(GX_CLIP_ENABLE);
  GS_SetBlendSrc(0,1,0);
  GXSetZCompLoc(0);
  GXSetAlphaUpdate(0);
  GXSetCullMode(0);
  GS_SetZCompare(1,1,GX_LEQUAL);
  return;
}

void GS_EndScene(void) {
  //DEMODoneRender();
  return;
}

static s32 GS_ScreenHeight;
static s32 GS_ScreenWidth;
unsigned char DemoStatEnable; //DEMOStats.c

//NGC MATCH
s32 GS_Init(void) {
  struct _GXRenderModeObj *rmp;
  struct _GXColor col;

  GXInvalidateTexAll();
  GS_TexInit();
  col = GS_BgColour;
  //GXSetCopyClear(&col,0xffffff); //-1
  //GXCopyDisp(DEMOGetCurrentBuffer(), 1);
  //rmp = DEMOGetRenderModeObj();
  GS_ScreenWidth = (int)rmp->fbWidth;
  GS_ScreenHeight = (int)rmp->efbHeight;
  GS_InitVertexDescriptors();
  GS_InitXForm();
  DemoStatEnable = 0; //UNUSED
  GS_FrameBufferCopyPause.top = 0;
  GS_FrameBufferCopyPause.left = 0;
  GS_FrameBufferCopyPause.width = 0x140;
  GS_FrameBufferCopyPause.height = 0xe0;
  GS_FrameBufferCopyPause.data = NULL;
  //CARDInit();
  //GBA_Init();
  GS_FrameBufferCopySize = GXGetTexBufferSize(GS_FrameBufferCopyPause.width,GS_FrameBufferCopyPause.height,4,0,0);
  GS_FrameBufferCopyPause.data = (u8 *)malloc(GS_FrameBufferCopySize);
  if (GS_FrameBufferCopyPause.data == NULL) {
    DisplayErrorAndLockup("C:/source/crashwoc/code/system/gc/gs.c",0x182,"Out Of Memory - Frame Buffer Copy Pause");
  }
  GS_FrameBufferCopydataptr = GS_FrameBufferCopyPause.data;
  GS_LoadWorldMatrixIdentity();
  return 0;
}

//NGC MATCH
void GS_RenderClear(unsigned long Flags,unsigned long Color,float Z,unsigned long Stencil) {
  struct _GXColor bgcol;
  
  if ((Flags & 0xf0) != 0) {
    if (fadeval == 0xff) {
      GS_BgColour.a = 0;
      GS_BgColour.r = 0;
      GS_BgColour.g = 0;
      GS_BgColour.b = 0;
    }
    else {
      GS_BgColour.a = Color >> 0x18;
      GS_BgColour.r = (Color >> 0x10);
      GS_BgColour.g = (Color >> 0x8);
      GS_BgColour.b = (Color);
    }
    bgcol = GS_BgColour;
    GXSetCopyClear(&bgcol,0xffffff);
  }
  return;
}

//NGC MATCH
void GS_RenderClearBlack(void) {
  struct _GXColor clr_col;
  
  clr_col = GS_BgColourBlack;
  GXSetCopyClear(&clr_col,0xffffff);
  return;
}

//NGC MATCH
void GS_SetOrthMatrix(void) {
  //DEMOSetupScrnSpc(GS_ScreenWidth,GS_ScreenHeight,100.0f);
  return;
}

//NGC MATCH
void GS_DrawFade(int fadecol) {
  u8 fadebytes[4];
    u8* ptr = fadebytes;

  *(s32*)fadebytes = fadecol;
  GS_SetOrthMatrix();
  GS_SetZCompare(0,0,GX_ALWAYS);
  GS_SetBlendSrc(1,0,3);
  GS_CurrentVertDesc = 0;
  GXClearVtxDesc();
  GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
  GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
  GXSetChanCtrl(GX_COLOR0A0,0,GX_SRC_VTX,GX_SRC_VTX,0,GX_DF_NONE,GX_AF_NONE);
  GXSetNumChans(1);
  GXSetTevOrder(GX_TEVSTAGE0,GX_TEXCOORD_NULL,GX_TEXMAP_NULL,GX_COLOR0A0);
  GXSetTevOp(GX_TEVSTAGE0,GX_PASSCLR);
  GXSetNumTexGens(0);
  GXSetNumTevStages(1);
  GXBegin(GX_QUADS,GX_VTXFMT7,4);
  GXPosition3f32(0.0f,480.0f,0.0f);
  GXColor4u8(ptr[3],ptr[2],ptr[1],ptr[0]); 
  GXPosition3f32(0.0f,0.0f,0.0f);
  GXColor4u8(ptr[3],ptr[2],ptr[1],ptr[0]);
  GXPosition3f32(640.0f,0.0f,0.0f);
  GXColor4u8(ptr[3],ptr[2],ptr[1],ptr[0]);
  GXPosition3f32(640.0f,480.0f,0.0f);
  GXColor4u8(ptr[3],ptr[2],ptr[1],ptr[0]);
  return;
}

//NGC MATCH
void GS_FlipScreen(void) {
  int i;
  double fps;
  
  if (GS_IsNewFrame != 0) {
    DEMOBeforeRender();
  }
  GS_IsNewFrame = 0;
  if (PrintError != 0) {
    TTLLights();
    GS_SetZCompare(1,1,GX_ALWAYS);
    GS_SetBlendSrc(1,1,0);
    DEMOInitCaption(0,0x280,0x1c0);
    i = strlen(Lockupbuffer1);
    DEMOPrintf(0x140 - (short)(i << 2),0x91,0,Lockupbuffer1);
    i = strlen(Lockupbuffer2);
    DEMOPrintf(0x140 - (short)(i << 2),0x9b,0,Lockupbuffer2);
    i = strlen(Lockupbuffer3);
    DEMOPrintf((0x140 - (short)(i << 2)),0xa5,0,Lockupbuffer3);
  }
  else if (Lockupbuffer1[0] != 0) {
      TTLLights();
      GS_SetZCompare(1,1,GX_ALWAYS);
      GS_SetBlendSrc(1,1,0);
      DEMOInitCaption(0,0x280,0x1c0);
      DEMOPrintf(10,0xf,0,"FATAL ERROR - GAME HALTED");
      DEMOPrintf(10,0x19,0,"REPORT THE FOLLOWING MESSAGE AND HOW TO REPRODUCE");
      DEMOPrintf(10,0x2d,0,Lockupbuffer1);
      DEMOPrintf(10,0x37,0,Lockupbuffer2);
      DEMOPrintf(10,0x41,0,Lockupbuffer3);
      DEMOPrintf(10,0x4b,0,namesave);
    } else {
        TTLLights();
        GS_SetZCompare(1,1,GX_ALWAYS);
    }
  
    DBTimerReset();
    DEMODoneRender();
    DEMOSwapBuffers();
    fps = TimerGetFPS();
    avfps = (avfps + (float)fps) * 0.5f;
    GS_IsNewFrame = 1;
    nuvideo_global_vbcnt++;
    GS_WorldMatIsIdentity = 0;
    GS_CurrentVertDesc = -1;
    GS_CurrentTexture = -1;
    cufps = (float)fps;
  return;
}

//NGC MATCH
void GS_SetViewport(struct _GS_VIEWPORT *pViewport) {
  GS_ViewPort = *pViewport;
  return;
}

//NGC MATCH
void GS_GetViewport(struct _GS_VIEWPORT* pViewport) {
  *pViewport = GS_ViewPort;
  return;
}

//77% NGC
void GS_SetupFog(s32 type,float startz,float endz,u32 colour) {
    volatile union { struct _GXColor c; u32 u; } dumb;
    struct _GXColor local_8;
    u32 unused;
    float var1, var2;

    dumb.u = colour;
      local_8.a = colour >> 0x18;
      local_8.r = (colour >> 0x10);
      local_8.g = (colour >> 0x8);
      local_8.b = (colour);
    
  var1 = 0.3f;
    var2 = 1000.0f;
 /* local_8 = *(struct _GXColor *)
            ((colour & 0xff) << 8 |
            (colour >> 8 & 0xff) << 0x10 | (colour >> 0x10) << 0x18 | colour >> 0x18);*/
  if (type == 0) {
      GXSetFog(GX_FOG_NONE,0.0f,0.0f,0.0f,0.0f,local_8);
      return;
  }
  GXSetFog(GX_FOG_LIN,startz,endz,var1,var2,local_8);
  return;
}

void GS_SetRenderState(s32 state,s32 data) {
  return;
}

//NGC MATCH
void GS_SetZCompare(int enable,int upd,enum _GXCompare mode) {
  sprintf(GS_CommandBuffer,"ZCMP En %d Up %d Mo %d\n",enable,upd,mode);
  if (GS_ZCompareMode != mode) {
    GS_ZCompareMode = mode;
  }
  if (GS_ZCmpUpdate != upd) {
    GS_ZCmpUpdate = upd;
  }
  if (GS_ZCmpEnable != enable) {
    GS_ZCmpEnable = enable;
  }
  if (GS_Parallax != 0) {
    //GXSetZMode(0,GX_NEVER,0);
  }
  else {
    //GXSetZMode(GS_ZCmpEnable,GS_ZCompareMode,GS_ZCmpUpdate);
  }
  return;
}

//NGC MATCH
void GS_SetBlendSrc(s32 enable,s32 src,s32 dest) {
  sprintf(GS_CommandBuffer,"BLEND En %d Sr %d Ds %d\n",enable,src,dest);
  if (GS_BlendEnable != enable) {
    GS_BlendEnable = enable;
  }
  if (GS_BlendSrc != src) {
    GS_BlendSrc = src;
  }
  if (GS_BlendDest != dest) {
    GS_BlendDest = dest;
  }
  if (GS_BlendEnable == 1) {
    //GXSetBlendMode(GX_BM_BLEND,GS_BlendSrc,GS_BlendDest,GX_LO_COPY);
  }
  else {
    //GXSetBlendMode(GX_BM_NONE,GX_BL_ONE,GX_BL_ZERO,GX_LO_COPY);
  }
  return;
}

//NGC MATCH
void GS_SetBlendSrc2(s32 enable,s32 src,s32 dest) {
  //GXSetBlendMode(GX_BM_BLEND,GX_BL_SRCCLR,GX_BL_SRCCLR,GX_LO_COPY);
  return;
}

//NGC MATCH
void GS_SetProjectionMatrix(struct _GSMATRIX *pMatrix) {
    float pMtx[4][4]; // 0x8(r1)
    float mMtx[4][3]; // 0x48(r1)

    memcpy(GS_MatProjection, pMatrix, sizeof(float[4][4]));
    C_MTXPerspective((float **)pMtx,40.0f,1.428571f,0.3f,1000.0f);
    GXSetProjection((float **)pMtx,GX_PERSPECTIVE);
    return;
}

//NGC MATCH
void GS_SetLightingMatrix(struct _GSMATRIX *mtx) {
  float local_b0 [6][4];
  float local_50 [4][4];
  
  memcpy(&local_50, mtx, sizeof(struct _GSMATRIX));
  MatReorder(&local_50);
  memcpy(&local_b0, &local_50, sizeof(float) * 12);
  PSMTXInverse(&local_b0,&local_b0[3][0]);
  PSMTXTranspose(&local_b0[3][0],&local_b0);
  GXLoadNrmMtxImm(&local_b0,0);
  return;
}

//void GXLoadNrmMtxImm (void* mtx, unsigned long id);
//NGC MATCH
void GS_SetLightingMatrix2(struct _GSMATRIX *m) {
  struct _GSMATRIX local_a0;
  float pad[24];

  local_a0 = *m;
  GXLoadNrmMtxImm(&local_a0,0);
  return;
}

//NGC MATCH
void GS_LoadMatrix(struct _GSMATRIX *Matrix) {
  float M[4][3];
  
  memcpy(M, Matrix, sizeof(float[4][3]));
  GXLoadPosMtxImm(&M,0);
  GXSetCurrentMtx(0);
  return;
}

//NGC MATCH
void GS_SetViewMatrix(struct _GSMATRIX *a) {
  memcpy(GS_MatView, a, sizeof(float[4][4])); // *(struct _GSMATRIX *)GS_MatView = *a;
  SetIdentity(&GS_ViewIdentity);
  MatMult(&GS_ViewIdentity,a);
  MatReorder(&GS_ViewIdentity);
  return;
}

//NGC MATCH
void GS_LoadWorldMatrixIdentity(void) {
  if (GS_WorldMatIsIdentity == 0) {
    GS_WorldMatIsIdentity = 1;
    GS_LoadMatrix(&GS_ViewIdentity);
  }
  return;
}

//NGC MATCH
void GS_SetWorldMatrix(struct _GSMATRIX *pMatrix) {
  struct _GSMATRIX local_48;

  memcpy(GS_MatWorld, pMatrix, sizeof(float[4][4]));
  memcpy(&local_48, GS_MatWorld, sizeof(float[4][4]));
  MatMult(&local_48,&GS_MatView);
  MatReorder(&local_48);
  GS_LoadMatrix(&local_48);
  GS_WorldMatIsIdentity = 0;
  return;
}

void GS_CopyTextureFromFB(void) {
    return;
}

//NGC MATCH
void GS_CopyFBToPause(void) {
  GXSetTexCopySrc(GS_FrameBufferCopyPause.left,GS_FrameBufferCopyPause.top,0x280,0x1c0);
  GXSetTexCopyDst(GS_FrameBufferCopyPause.width,GS_FrameBufferCopyPause.height,GX_TF_RGB565,1);
  GXCopyTex(GS_FrameBufferCopyPause.data,0);
  GXPixModeSync();
  GXDrawDone();
  GXInitTexObj(&GS_FrameBufferCopyPause.tobj,GS_FrameBufferCopyPause.data,GS_FrameBufferCopyPause.width,GS_FrameBufferCopyPause.height,
  GX_TF_RGB565,GX_CLAMP,GX_CLAMP,0);
  return;
}

