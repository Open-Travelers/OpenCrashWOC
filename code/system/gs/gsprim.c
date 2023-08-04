#include "gsprim.h"



void GS_DrawTriStrip(int nverts,float *vertlist,int stride)

{
  bool bVar1;
  int iVar2;
  uint uVar3;

  if (stride == 0x1c) {
    if (GS_CurrentVertDesc != 0x81) {
      GS_CurrentVertDesc = 0x81;
      GXClearVtxDesc();
      GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
      GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
      GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
    }
  }
  else if (GS_CurrentVertDesc != 0x83) {
    GS_CurrentVertDesc = 0x83;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS,GX_INDEX16);
    GXSetVtxDesc(GX_VA_NRM,GX_INDEX16);
    GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0,GX_INDEX16);
  }
  uVar3 = stride & 0xff;
  GXSetArray(GX_VA_POS,vertlist,uVar3);
  bVar1 = stride != 0x1c;
  if (bVar1) {
    GXSetArray(GX_VA_NRM,vertlist + 3,uVar3);
  }
  iVar2 = stride >> 2;
  GXSetArray(GX_VA_TEX0,vertlist + iVar2 + -2,uVar3);
  if (bVar1) {
    GXBegin(GX_TRIANGLESTRIP,GX_VTXFMT2,(ushort)nverts);
  }
  else {
    GXBegin(GX_TRIANGLESTRIP,GX_VTXFMT1,(ushort)nverts);
  }
  uVar3 = 0;
  if (nverts != 0) {
    do {
      if (bVar1) {
        ram0xcc008002 = SUB42(vertlist[uVar3 * iVar2 + iVar2 + -3],0);
        if ((IsStencil == 0) && (ShadowBodge == GX_TEVSTAGE0)) goto joined_r0x800ca7d8;
LAB_800ca7d0:
        ram0xcc008002 = (undefined2)ShadowColour;
      }
      else {
        ram0xcc008002 = SUB42(vertlist[uVar3 * 7 + 4],0);
        if ((IsStencil != 0) || (ShadowBodge != GX_TEVSTAGE0)) goto LAB_800ca7d0;
joined_r0x800ca7d8:
        if (GS_MaterialSourceEmissive == 0) {
          ram0xcc008002 = (undefined2)GS_CurrentMaterialEmissivergba;
        }
      }
      _DAT_cc008000 = CONCAT22((short)uVar3,ram0xcc008002);
      uVar3 = (uint)(short)((short)uVar3 + 1);
    } while (uVar3 < (uint)nverts);
  }
  return;
}


void GS_DrawTriList(int nverts,float *vertlist,int stride)

{
  _GSMATRIX GSmtx;

  DBTimerStart(0x15);
  if (stride == 0x1c) {
    C_MTXOrtho(0.0,448.0,0.0,640.0,0.0,-1.0,&GSmtx);
    GXSetProjection(&GSmtx,GX_ORTHOGRAPHIC );
    PSMTXIdentity((_GSMATRIX *)&stack0xffffffb8);
    GXSetCurrentMtx((uint)(_GSMATRIX *)&stack0xffffffb8);
    GXSetCurrentMtx(0);
    if (GS_CurrentVertDesc != 0x81) {
      GS_CurrentVertDesc = 0x81;
      GXClearVtxDesc();
      GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
      GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
      GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
    }
    GXBegin(GX_TRIANGLES,GX_VTXFMT1,(ushort)nverts);
    if (0 < nverts) {
      do {
        _DAT_cc008000 = vertlist[6];
        vertlist = vertlist + 7;
        nverts = nverts + -1;
      } while (nverts != 0);
    }
    GS_SetProjectionMatrix((_GSMATRIX *)&stack0xffffffb8);
  }
  else {
    if (GS_CurrentVertDesc != 0x82) {
      GS_CurrentVertDesc = 0x82;
      GXClearVtxDesc();
      GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
      GXSetVtxDesc(GX_VA_NRM,GX_DIRECT);
      GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
      GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
    }
    GXBegin(GX_TRIANGLES,GX_VTXFMT2,(ushort)nverts);
    if (0 < nverts) {
      do {
        _DAT_cc008000 = vertlist[8];
        vertlist = vertlist + 9;
        nverts = nverts + -1;
      } while (nverts != 0);
    }
  }
  DBTimerEnd(0x15);
  return;
}


void GS_DrawQuadList(uint nverts,float *vertlist,int stride)

{
  uint i;

  if (GS_CurrentVertDesc != 0x82) {
    GS_CurrentVertDesc = 0x82;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM,GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
  }
  GXBegin(GX_QUADS,GX_VTXFMT2,(ushort)nverts);
  i = 0;
  if (nverts != 0) {
    do {
      i = i + 1;
      _DAT_cc008000 = vertlist[8];
      vertlist = vertlist + 9;
    } while (i < nverts);
  }
  return;
}


void GS_DrawPrimitiveQuad(_GS_VERTEX *vertlist)

{
  if (GS_CurrentVertDesc != 0x82) {
    GS_CurrentVertDesc = 0x82;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM,GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
  }
  GXBegin(GX_QUADS,GX_VTXFMT2,4);
  _DAT_cc008000 = vertlist[3].v;
  return;
}


void GS_DrawIndexedTriStrip(uint VertexCount,short *pIndexData,float *vertlist,int stride)

{
  int iVar1;

  DBTimerStart(0x19);
  if (GS_CurrentVertDesc != 0x82) {
    GS_CurrentVertDesc = 0x82;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM,GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
  }
  if (VertexCount != 0) {
    do {
      GXBegin(GX_TRIANGLESTRIP,GX_VTXFMT2,*pIndexData);
      for (iVar1 = (int)*pIndexData; pIndexData = (short *)((ushort *)pIndexData + 1), 0 < iVar1;
          iVar1 = iVar1 + -1) {
        VertexCount = VertexCount - 1;
        _DAT_cc008000 = vertlist[*pIndexData * 9 + 8];
      }
      VertexCount = VertexCount - 1;
    } while (VertexCount != 0);
  }
  DBTimerEnd(0x19);
  return;
}


void GS_DrawIndexedTriList(uint nverts,short *indexlist,float *vertlist,int stride)

{
  short sVar1;
  uint uVar2;

  DBTimerStart(0x18);
  if (GS_CurrentVertDesc != 0x82) {
    GS_CurrentVertDesc = 0x82;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM,GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
  }
  GXBegin(GX_TRIANGLES,GX_VTXFMT2,(ushort)nverts);
  uVar2 = 0;
  if (nverts != 0) {
    do {
      sVar1 = *indexlist;
      uVar2 = uVar2 + 1;
      indexlist = indexlist + 1;
      _DAT_cc008000 = vertlist[sVar1 * 9 + 8];
    } while (uVar2 < nverts);
  }
  DBTimerEnd(0x18);
  return;
}


void TTLLights(void)

{
  _GXColor GXCol [2];

  GXCol[0] = GXWhite;
  GXSetChanAmbColor(GX_COLOR0A0,GXCol);
  GXCol[0] = GXWhite;
  GXSetChanMatColor(GX_COLOR0A0,GXCol);
  GXSetChanCtrl(GX_COLOR0A0,'\0',GX_SRC_VTX,GX_SRC_REG,0,GX_DF_NONE,GX_AF_NONE);
  GXSetNumChans(1);
  return;
}


void GS_DrawTriStripTTL(_GS_VERTEXTL *vertlist,int nverts)

{
  if (GS_CurrentVertDesc != 0x81) {
    GS_CurrentVertDesc = 0x81;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
  }
  TTLLights();
  GXBegin(GX_TRIANGLESTRIP,GX_VTXFMT1,(ushort)nverts);
  if (0 < nverts) {
    do {
      _DAT_cc008000 = vertlist->v;
      vertlist = vertlist + 1;
      nverts = nverts + -1;
    } while (nverts != 0);
  }
  return;
}

void GS_DrawTriListTTL(nuvtx_tltc1_s *vtx,int nverts)

{
  uint uVar1;
  int unaff_r13;
  int iVar2;

  if (GS_CurrentVertDesc != 1) {
    GS_CurrentVertDesc = 1; //0x81
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
  }
  TTLLights();
  GXBegin(GX_TRIANGLES,GX_VTXFMT1,(ushort)nverts);
  if (0 < nverts) {
    iVar2 = 0;
    do {
      GXPosition3f32(*(float *)((int)vtx->tc + iVar2 + -0x14),
                     *(float *)((int)vtx->tc + iVar2 + -0x10),
                     *(float *)((int)vtx->tc + iVar2 + -0xc));
      uVar1 = *(uint *)((int)vtx->tc + iVar2 + -4);
      GXColor4u8(uVar1 >> 0x10 != 0,uVar1 >> 8 != 0,uVar1 != 0,uVar1 >> 0x18 != 0);
      GXTexCoord2f32(*(float *)((int)vtx->tc + iVar2),*(float *)((int)vtx->tc + iVar2 + 4));
      nverts = nverts + -1;
      iVar2 = iVar2 + 0x1c;
    } while (nverts != 0);
  }
  if (*(char *)(unaff_r13 + -0x5ef8) == '\0') {
    OSPanic("C:/DolphinSDK1.0/include/dolphin/gx/GXGeometry.h",0x6d,
            "GXEnd: called without a GXBegin");
  }
  *(undefined *)(unaff_r13 + -0x5ef8) = 0;
  return;
}

void GS_DrawQuadListBeginBlock(int nverts)

{
  int in_r4;

  if (in_r4 == 0) {
    TTLLights();
  }
  else {
    GS_SetLightingNone();
  }
  GXSetChanCtrl(GX_COLOR0A0,'\0',GX_SRC_REG,GX_SRC_VTX,0,GX_DF_NONE,GX_AF_NONE);
  QBlockverts = nverts;
  if (GS_CurrentVertDesc != 0x81) {
    GS_CurrentVertDesc = 0x81;
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
  }
  GXBegin(GX_QUADS,GX_VTXFMT1,(ushort)nverts);
  return;
}


void GS_DrawQuadListSetVert(nuvec_s *pos,float u,float v)

{
  _DAT_cc008000 = v;
  return;
}


void GS_DrawQuadListEndBlock(void)

{
  return;
}


void GS_DrawTriListTSkin(undefined4 param_1,int param_2,int param_3,int param_4)		//TODO

{
  short *psVar1;
  int iVar2;

  DBTimerStart(0x1a);
  if (GS_EnableLightingFlag == 0) {
    if (GS_CurrentVertDesc != 0x80) {
      GS_CurrentVertDesc = 0x80;
      GXClearVtxDesc();
      GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
      GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
    }
    GXBegin(GX_TRIANGLES,GX_VTXFMT7,(ushort)param_2);
    if (0 < param_2) {
      do {
        _DAT_cc008000 = ShadowColour;
        param_2 = param_2 + -1;
      } while (param_2 != 0);
    }
  }
  else {
    if (GS_CurrentVertDesc != 0x82) {
      GS_CurrentVertDesc = 0x82;
      GXClearVtxDesc();
      GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
      GXSetVtxDesc(GX_VA_NRM,GX_DIRECT);
      GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);
      GXSetVtxDesc(GX_VA_TEX0,GX_DIRECT);
    }
    GXBegin(GX_TRIANGLES,GX_VTXFMT2,(ushort)param_2);
    if (0 < param_2) {
      iVar2 = 0;
      do {
        psVar1 = (short *)(iVar2 + param_4);
        iVar2 = iVar2 + 2;
        _DAT_cc008000 = *(undefined4 *)(*psVar1 * 0x38 + param_3 + 0x34);
        param_2 = param_2 + -1;
      } while (param_2 != 0);
    }
  }
  DBTimerEnd(0x1a);
  return;
}




/*********from melee decomp***************/

/*

void C_MTXPerspective(Mtx44 m, f32 fovY, f32 aspect, f32 n, f32 f)
{
    f32 temp_f3;
    f32 temp_f4;

    fovY = 0.5F * fovY;

    temp_f4 = 1.0F / tanf(0.017453292F * (fovY));
    temp_f3 = 1.0F / (f - n);

    m[0][0] = temp_f4 / aspect;
    m[0][1] = 0.0F;
    m[0][2] = 0.0F;
    m[0][3] = 0.0F;

    m[1][0] = 0.0F;
    m[1][1] = temp_f4;
    m[1][2] = 0.0F;
    m[1][3] = 0.0F;

    m[2][0] = 0.0F;
    m[2][1] = 0.0F;
    m[2][2] = -n * temp_f3;
    m[2][3] = temp_f3 * -(f * n);

    m[3][0] = 0.0F;
    m[3][1] = 0.0F;
    m[3][2] = -1.0F;
    m[3][3] = 0.0F;
}


void C_MTXOrtho(Mtx44 m, f32 t, f32 b, f32 l, f32 r, f32 n, f32 f)
{
    f32 temp_f8;
    f32 temp_f10;
    f32 temp_f4;

    temp_f10 = 1.0F / (r - l);
    m[0][0] = 2.0F * temp_f10;
    m[0][1] = 0.0F;
    m[0][2] = 0.0F;
    m[0][3] = temp_f10 * -(r + l);

    temp_f8 = 1.0F / (t - b);
    m[1][0] = 0.0F;
    m[1][1] = 2.0F * temp_f8;
    m[1][2] = 0.0F;
    m[1][3] = temp_f8 * -(t + b);

    temp_f4 = 1.0F / (f - n);
    m[2][0] = 0.0F;
    m[2][1] = 0.0F;
    m[2][2] = -1.0F * temp_f4;
    m[2][3] = -f * temp_f4;

    m[3][0] = 0.0F;
    m[3][1] = 0.0F;
    m[3][2] = 0.0F;
    m[3][3] = 1.0F;
}

*/
