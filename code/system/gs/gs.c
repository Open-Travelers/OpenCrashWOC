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

void GS_SetFBCopyTexturePause(void)

{
  //GXLoadTexObj(&GS_FrameBufferCopyPause.tobj,GX_TEXMAP0);
  return;
}

void GS_SetAlphaCompareForce(int param_1)

{
  GS_ForceNoAlphaCompareFlag = param_1;
  if (param_1 != 0) {
    //GXSetAlphaCompare(GX_GREATER,'\0',GX_AOP_OR,GX_GREATER,'\0');
  }
  return;
}

void GS_SetAlphaCompare(int Func,int Ref)

{
  if (GS_ForceNoAlphaCompareFlag == 0) {
    //GXSetAlphaCompare(Func,(uchar)Ref,GX_AOP_OR,Func,(uchar)Ref);
  }
  else {
    //GXSetAlphaCompare(GX_ALWAYS,'\0',GX_AOP_OR,GX_ALWAYS,'\0');
  }
  return;
}

void GS_ARGBTORGBA(u32 *colors,int count)

{
  if (count < 1) {
    return;
  }
  do {
    colors[6] = colors[6] << 8 | colors[6] >> 0x18;
    colors = colors + 9;
    count = count + -1;
  } while (count != 0);
  return;
}


void GS_BeginScene(void)

{
  u64 field;

  GS_WorldMatIsIdentity = 0;
  GS_SetZCompare(1,1,7);
  field = 1;
  GS_SetBlendSrc(1,1,0);
  if (GS_IsNewFrame != 0) {
    //DEMOBeforeRender();
  }
  GS_IsNewFrame = 0;
  //GXSetViewport(0.0,0.0,640.0,448.0,0.0,1.0,field);
  //GXSetScissor(0,0,0x280,0x1c0);
  //GXSetClipMode(GX_CLIP_ENABLE);
  GS_SetBlendSrc(0,1,0);
  //GXSetZCompLoc('\0');
  //GXSetAlphaUpdate('\0');
  //GXSetCullMode(0);
  GS_SetZCompare(1,1,3);
  return;
}


void GS_EndScene(void)

{
  //DEMODoneRender();
  return;
}

static int GS_ScreenHeight;
static int GS_ScreenWidth;


int GS_Init(void)

{
  int i;
  //_GXRenderModeObj *rmp;
  //_GXColor GXCol [3];

  //GXInvalidateTexAll();
    //GS_TexInit();
  //GXCol[0] = GS_BgColour;
  //GXSetCopyClear(GXCol,0xffffff);
  //i = DEMOGetCurrentBuffer();
  //GXCopyDisp((void *)i,'\x01');
  //rmp = DEMOGetRenderModeObj();
  //GS_ScreenHeight = (int)rmp->efbHeight;
  //GS_ScreenWidth = (int)rmp->fbWidth;
  GS_InitVertexDescriptors();
  GS_InitXForm();
  //DemoStatEnable = '\0';
  /*GS_FrameBufferCopyPause.width = 0x140;
  GS_FrameBufferCopyPause.height = 0xe0;
  GS_FrameBufferCopyPause.data = NULL;
  GS_FrameBufferCopyPause.top = 0;
  GS_FrameBufferCopyPause.left = 0;*/
  //CARDInit();
  //GBA_Init();
  //GS_FrameBufferCopySize =
       //GXGetTexBufferSize(GS_FrameBufferCopyPause.width._2_2_,GS_FrameBufferCopyPause.height._2_2 _,4
                        //  ,'\0','\0');
  //GS_FrameBufferCopyPause.data = (u8 *)malloc(GS_FrameBufferCopySize);
  /*if (GS_FrameBufferCopyPause.data == NULL) {
    DisplayErrorAndLockup
              ("C:/source/crashwoc/code/system/gc/gs.c",0x182,
               "Out Of Memory - Frame Buffer Copy Pause");
  }*/
  //GS_FrameBufferCopydataptr = GS_FrameBufferCopyPause.data;
  GS_LoadWorldMatrixIdentity();
  return 0;
}

//static _GXColor GS_BgColour;

void GS_RenderClear(u64 Flags,u64 Color,float Z,u64 Stencil)

{
  //_GXColor bgcol [2];

  if ((Flags & 0xf0) != 0) {
    if (fadeval == 0xff) {
      //GS_BgColour = NULL;
    }
    else {
      //GS_BgColour._2_2_ = CONCAT11((char)Color,(char)(Color >> 0x18));
      //GS_BgColour = (_GXColor)((Color >> 8) << 0x10 | (uint)GS_BgColour._2_2_);
    }
    //bgcol[0] = GS_BgColour;
    //GXSetCopyClear(bgcol,0xffffff);
  }
  return;
}


void GS_RenderClearBlack(void)

{
  //_GXColor clr_col [2];

  //clr_col[0] = GS_BgColourBlack;
  //GXSetCopyClear(clr_col,0xffffff);
  return;
}

void GS_SetOrthMatrix(void)

{
  //DEMOSetupScrnSpc(GS_ScreenWidth,GS_ScreenHeight,100.0);
  return;
}


void GS_DrawFade(int fadecol)

{
  GS_SetOrthMatrix();
  GS_SetZCompare(0,0,7);
  GS_SetBlendSrc(1,0,3);
  GS_CurrentVertDesc = 0;
  //GXClearVtxDesc();
  //GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
  //GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
  //GXSetChanCtrl(GX_COLOR0A0,'\0',GX_SRC_VTX,GX_SRC_VTX,0,GX_DF_NONE,GX_AF_NONE);
  //GXSetNumChans(1);
  //GXSetTevOrder(GX_TEVSTAGE0,GX_TEXCOORD_NULL,GX_TEXMAP_NULL,GX_COLOR0A0);
  //GXSetTevOp(GX_TEVSTAGE0,GX_PASSCLR);
  //GXSetNumTexGens('\0');
  //GXSetNumTevStages('\x01');
  //GXBegin(GX_QUADS,GX_VTXFMT7,4);
  //_DAT_cc008000 = fadecol & 0xff000000;
  return;
}

void GS_FlipScreen(void)

{
  s8 sVar1;
  int iVar2;
  //undefined4 uVar3;
  char *pcVar4;
  double fps;

  if (GS_IsNewFrame != 0) {
    //DEMOBeforeRender();
  }
  GS_IsNewFrame = 0;
  if (PrintError == 0) {
    if (Lockupbuffer1[0] == '\0') {
      //TTLLights();
      GS_SetZCompare(1,1,7);
      goto LAB_800c9244;
    }
    //TTLLights();
    GS_SetZCompare(1,1,7);
    GS_SetBlendSrc(1,1,0);
    /*DEMOInitCaption(0,0x280,0x1c0);
    DEMOPrintf(10,0xf,0,"FATAL ERROR - GAME HALTED");
    DEMOPrintf(10,0x19,0,"REPORT THE FOLLOWING MESSAGE AND HOW TO REPRODUCE");
    DEMOPrintf(10,0x2d,0,Lockupbuffer1);
    DEMOPrintf(10,0x37,0,Lockupbuffer2);
    DEMOPrintf(10,0x41,0,Lockupbuffer3);*/
    iVar2 = 10;
    pcVar4 = namesave;
    //uVar3 = 0x4b;
  }
  else {
    //TTLLights();
    GS_SetZCompare(1,1,7);
    GS_SetBlendSrc(1,1,0);
    //DEMOInitCaption(0,0x280,0x1c0);
    sVar1 = strlen(Lockupbuffer1);
    //DEMOPrintf((int)(short)(0x140 - (short)(sVar1 << 2)),0x91,0,Lockupbuffer1);
    sVar1 = strlen(Lockupbuffer2);
    //DEMOPrintf((int)(short)(0x140 - (short)(sVar1 << 2)),0x9b,0,Lockupbuffer2);
    sVar1 = strlen(Lockupbuffer3);
    pcVar4 = Lockupbuffer3;
    //uVar3 = 0xa5;
    iVar2 = (int)(short)(0x140 - (short)(sVar1 << 2));
  }
  //DEMOPrintf(iVar2,uVar3,0,pcVar4);
LAB_800c9244:
  //DBTimerReset();
  //DEMODoneRender();
  //DEMOSwapBuffers();
  //fps = TimerGetFPS();
  //avfps = (avfps + (float)fps) * 0.5;
  GS_IsNewFrame = 1;
  nuvideo_global_vbcnt = nuvideo_global_vbcnt + 1;
  GS_WorldMatIsIdentity = 0;
  GS_CurrentTexture = -1;
  //cufps = (float)fps;
  GS_CurrentVertDesc = -1;
  return;
}

void GS_SetViewport(struct _GS_VIEWPORT *pViewport)

{
  GS_ViewPort.X = pViewport->X;
  GS_ViewPort.Y = pViewport->Y;
  GS_ViewPort.width = pViewport->width;
  GS_ViewPort.height = pViewport->height;
  GS_ViewPort.MaxZ = pViewport->MaxZ;
  GS_ViewPort.MinZ = pViewport->MinZ;
  return;
}

void GS_GetViewport(struct nuviewport_s *vp)

{
  float MinZ;
  u64 Y;
  u64 height;
  u64 width;

  height = GS_ViewPort.height;
  width = GS_ViewPort.width;
  Y = GS_ViewPort.Y;
  vp->x = GS_ViewPort.X;
  vp->y = Y;
  vp->width = width;
  vp->height = height;
  MinZ = GS_ViewPort.MinZ;
  vp->zmax = GS_ViewPort.MaxZ;
  vp->zmin = MinZ;
  return;
}

void GS_SetupFog(int type,float startz,float endz,u32 colour)

{
  //_GXColor local_8;

  /*local_8 = (_GXColor)
            ((colour & 0xff) << 8 |
            (colour >> 8 & 0xff) << 0x10 | (colour >> 0x10) << 0x18 | colour >> 0x18);*/
  if (type == 0) {
    //GXSetFog(GX_FOG_NONE,0.0,0.0,0.0,0.0,(_GXColor)&local_8);
  }
  else {
    //GXSetFog(GX_FOG_LINEAR,startz,endz,0.3,1000.0,(_GXColor)&local_8);
  }
  return;
}


void GS_SetRenderState(int state,int data)
{
  return;
}

void GS_SetZCompare(int enable,int upd,enum _GXCompare mode)

{
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
  if (GS_Parallax == 0) {
    //GXSetZMode((uchar)GS_ZCmpEnable,GS_ZCompareMode,(uchar)GS_ZCmpUpdate);
  }
  else {
    //GXSetZMode('\0',GX_NEVER,'\0');
  }
  return;
}

void GS_SetBlendSrc(int enable,int src,int dest)

{
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

void GS_SetBlendSrc2(int enable,int src,int dest)

{
  //GXSetBlendMode(GX_BM_BLEND,GX_BL_SRCCLR,GX_BL_SRCCLR,GX_LO_COPY);
  return;
}

void GS_SetProjectionMatrix(struct _GSMATRIX *pMatrix) {
    float pMtx[4][4]; // 0x8(r1)
    float mMtx[4][3]; // 0x48(r1)

    GS_MatProjection = *pMatrix;
    //C_MTXPerspective((float **)pMtx,40.0f,1.428571f,0.3f,1000.0f);
    //GXSetProjection((const float **)pMtx,GX_PERSPECTIVE);
    return;
}

/*
void GS_SetLightingMatrix(struct _GSMATRIX *mtx)

{
    float *pfVar1;
  struct _GSMATRIX *p_Var2;
  struct _GSMATRIX *MatrixA;
  struct _GSMATRIX *p_Var3;
  float *pfVar4;
  int iVar5;
  struct _GSMATRIX *p_Var6;
  //undefined local_b0 [96];
  struct _GSMATRIX local_50;

  MatrixA = &local_50;
  iVar5 = 0x30;
  //pfVar4 = (float *)local_b0;
  p_Var2 = MatrixA;
  do {
    p_Var6 = mtx;
    p_Var3 = p_Var2;
    iVar5 = iVar5 + -0x18;
    p_Var3->_00 = p_Var6->_00;
    p_Var3->_01 = p_Var6->_01;
    p_Var3->_02 = p_Var6->_02;
    p_Var3->_03 = p_Var6->_03;
    p_Var3->_10 = p_Var6->_10;
    p_Var3->_11 = p_Var6->_11;
    p_Var2 = (struct _GSMATRIX *)&p_Var3->_12;
    mtx = (struct _GSMATRIX *)&p_Var6->_12;
  } while (iVar5 != 0);
  p_Var3->_12 = p_Var6->_12;
  p_Var3->_13 = p_Var6->_13;
  p_Var3->_20 = p_Var6->_20;
  p_Var3->_21 = p_Var6->_21;
  //MatReorder(MatrixA);
  iVar5 = 0x30;
  do {
    iVar5 = iVar5 + -0x18;
    *pfVar4 = MatrixA->_00;
    pfVar4[1] = MatrixA->_01;
    pfVar4[2] = MatrixA->_02;
    pfVar4[3] = MatrixA->_03;
    pfVar4[4] = MatrixA->_10;
    pfVar1 = &MatrixA->_11;
    MatrixA = (struct _GSMATRIX *)&MatrixA->_12;
    pfVar4[5] = *pfVar1;
    pfVar4 = pfVar4 + 6;
  } while (iVar5 != 0);
  /*PSMTXInverse((_GSMATRIX *)local_b0,(_GSMATRIX *)(local_b0 + 0x30));
  PSMTXTranspose((float *(*) [4])(_GSMATRIX *)(local_b0 + 0x30),(float *(*) [4])local_b0);
  GXLoadNrmMtxImm(local_b0,0);*/
 /* return;
}

void GS_SetLightingMatrix2(struct _GSMATRIX *m)

{
  float *pfVar1;
  float *pfVar2;
  int iVar3;
  float *pfVar4;
  float local_a0 [40];

  iVar3 = 0x30;
  pfVar1 = local_a0;
  do {
    pfVar4 = (float *)m;
    pfVar2 = pfVar1;
    iVar3 = iVar3 + -0x18;
    *pfVar2 = *pfVar4;
    pfVar2[1] = pfVar4[1];
    pfVar2[2] = pfVar4[2];
    pfVar2[3] = pfVar4[3];
    pfVar2[4] = (*(float (*) [4])(pfVar4 + 4))[0];
    pfVar2[5] = pfVar4[5];
    pfVar1 = pfVar2 + 6;
    m = (struct _GSMATRIX *)(pfVar4 + 6);
  } while (iVar3 != 0);
  pfVar2[6] = pfVar4[6];
  pfVar2[7] = pfVar4[7];
  pfVar2[8] = pfVar4[8];
  pfVar2[9] = pfVar4[9];
  //GXLoadNrmMtxImm(local_a0,0);
  return;
}

void GS_LoadMatrix(struct _GSMATRIX *Matrix)

{
    float *pfVar1;
  struct _GSMATRIX *pfVar2;
  int iVar2;

  //pfVar2 = (struct _GSMATRIX *)&stack0xffffffd0;
  iVar2 = 0x30;
  do {
    iVar2 = iVar2 + -0x18;
    pfVar2->_00 = Matrix->_00;
    pfVar2->_01 = Matrix->_01;
    pfVar2->_02 = Matrix->_02;
    pfVar2->_03 = Matrix->_03;
    pfVar2->_10 = Matrix->_10;
    pfVar1 = &Matrix->_11;
    Matrix = (struct _GSMATRIX *)&Matrix->_12;
    pfVar2->_11 = *pfVar1;
    pfVar2 = (struct _GSMATRIX *)&pfVar2->_12;
  } while (iVar2 != 0);
  //GXLoadPosMtxImm((struct _GSMATRIX *)&stack0xffffffd0,0);
  //GXSetCurrentMtx(0);
  return;
}

void GS_SetViewMatrix(struct _GSMATRIX *a)

{
   struct _GSMATRIX *p_Var1;
  struct _GSMATRIX *p_Var2;
  struct _GSMATRIX *p_Var3;
  int iVar4;
  struct _GSMATRIX *p_Var5;

  iVar4 = 0x30;
  p_Var1 = &GS_MatView;
  p_Var2 = a;
  do {
    p_Var5 = p_Var2;
    p_Var3 = p_Var1;
    iVar4 = iVar4 + -0x18;
    p_Var3->_00 = p_Var5->_00;
    p_Var3->_01 = p_Var5->_01;
    p_Var3->_02 = p_Var5->_02;
    p_Var3->_03 = p_Var5->_03;
    p_Var3->_10 = p_Var5->_10;
    p_Var3->_11 = p_Var5->_11;
    p_Var1 = (struct _GSMATRIX *)&p_Var3->_12;
    p_Var2 = (struct _GSMATRIX *)&p_Var5->_12;
  } while (iVar4 != 0);
  p_Var3->_12 = p_Var5->_12;
  p_Var3->_13 = p_Var5->_13;
  p_Var3->_20 = p_Var5->_20;
  p_Var3->_21 = p_Var5->_21;
  //SetIdentity(&GS_ViewIdentity);
  //MatMult(&GS_ViewIdentity,a);
  //MatReorder(&GS_ViewIdentity);
  return;
}
*/
void GS_LoadWorldMatrixIdentity(void)

{
  if (GS_WorldMatIsIdentity == 0) {
    GS_WorldMatIsIdentity = 1;
    //GS_LoadMatrix(&GS_ViewIdentity);
  }
  return;
}

/*
void GS_SetWorldMatrix(struct _GSMATRIX *pMatrix)

{
  struct _GSMATRIX *p_Var1;
  struct _GSMATRIX *p_Var2;
  int iVar3;
  struct _GSMATRIX *p_Var4;
  struct _GSMATRIX *pfVar4;
  struct _GSMATRIX *p_Var5;
  struct _GSMATRIX local_48;

  iVar3 = 0x30;
  p_Var1 = &GS_MatWorld;
  do {
    pfVar4 = p_Var1;
    p_Var2 = pMatrix;
    iVar3 = iVar3 + -0x18;
    pfVar4->_00 = p_Var2->_00;
    pfVar4->_01 = p_Var2->_01;
    pfVar4->_02 = p_Var2->_02;
    pfVar4->_03 = p_Var2->_03;
    pfVar4->_10 = p_Var2->_10;
    pfVar4->_11 = p_Var2->_11;
    pMatrix = (struct _GSMATRIX *)&p_Var2->_12;
    p_Var1 = (struct _GSMATRIX *)&pfVar4->_12;
  } while (iVar3 != 0);
  iVar3 = 0x30;
  pfVar4->_12 = p_Var2->_12;
  pfVar4->_13 = p_Var2->_13;
  pfVar4->_20 = p_Var2->_20;
  pfVar4->_21 = p_Var2->_21;
  p_Var1 = &GS_MatWorld;
  p_Var2 = &local_48;
  do {
    p_Var5 = p_Var2;
    p_Var4 = p_Var1;
    iVar3 = iVar3 + -0x18;
    p_Var5->_00 = p_Var4->_00;
    p_Var5->_01 = p_Var4->_01;
    p_Var5->_02 = p_Var4->_02;
    p_Var5->_03 = p_Var4->_03;
    p_Var5->_10 = p_Var4->_10;
    p_Var5->_11 = p_Var4->_11;
    p_Var1 = (struct _GSMATRIX *)&p_Var4->_12;
    p_Var2 = (struct _GSMATRIX *)&p_Var5->_12;
  } while (iVar3 != 0);
  p_Var5->_12 = p_Var4->_12;
  p_Var5->_13 = p_Var4->_13;
  p_Var5->_20 = p_Var4->_20;
  p_Var5->_21 = p_Var4->_21;
  //MatMult(&local_48,&GS_MatView);
  //MatReorder(&local_48);
  GS_LoadMatrix(&local_48);
  GS_WorldMatIsIdentity = iVar3;
}
*/

void GS_CopyTextureFromFB(void)

{
  return;
}


void GS_CopyFBToPause(void)

{
  /*GXSetTexCopySrc(GS_FrameBufferCopyPause.left._2_2_,GS_FrameBufferCopyPause.top._2_2_,0x280,0x1c 0);
  GXSetTexCopyDst(GS_FrameBufferCopyPause.width._2_2_,GS_FrameBufferCopyPause.height._2_2_,
                  GX_TF_RGB565,'\x01');
  GXCopyTex(GS_FrameBufferCopyPause.data,'\0');
  GXPixModeSync();
  GXDrawDone();
  GXInitTexObj(&GS_FrameBufferCopyPause.tobj,GS_FrameBufferCopyPause.data,
               (uint)GS_FrameBufferCopyPause.width._2_2_,(uint)GS_FrameBufferCopyPause.height._2_ 2_,
               GX_TF_IA8,GX_CLAMP,GX_CLAMP,'\0');*/
  return;
}

