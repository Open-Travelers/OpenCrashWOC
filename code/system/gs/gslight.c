#include "gslight.h"




void GS_SetMaterial(_D3DMATERIAL8 *pMaterial)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  _D3DMATERIAL8 *pMat;
  _D3DMATERIAL8 *p_Var9;
  _D3DMATERIAL8 *curmat;
  int iVar10;
  
  iVar10 = 0x30;
  curmat = &GS_CurrentMaterial;
  do {
    p_Var9 = curmat;
    pMat = pMaterial;
    iVar10 = iVar10 + -0x18;
    (p_Var9->Diffuse).r = (pMat->Diffuse).r;
    (p_Var9->Diffuse).g = (pMat->Diffuse).g;
    (p_Var9->Diffuse).b = (pMat->Diffuse).b;
    (p_Var9->Diffuse).a = (pMat->Diffuse).a;
    (p_Var9->Ambient).r = (pMat->Ambient).r;
    pMaterial = (_D3DMATERIAL8 *)&(pMat->Ambient).b;
    (p_Var9->Ambient).g = (pMat->Ambient).g;
    curmat = (_D3DMATERIAL8 *)&(p_Var9->Ambient).b;
  } while (iVar10 != 0);
  *(float *)curmat = *(float *)pMaterial;
  (p_Var9->Ambient).a = (pMat->Ambient).a;
  (p_Var9->Specular).r = (pMat->Specular).r;
  (p_Var9->Specular).g = (pMat->Specular).g;
  (p_Var9->Specular).b = (pMat->Specular).b;
  uVar1 = (int)(GS_CurrentMaterial.Diffuse.a * 255.0) & 0xff;
  uVar2 = (int)(GS_CurrentMaterial.Emissive.a * 255.0) & 0xff;
  uVar3 = (int)(GS_CurrentMaterial.Emissive.r * 255.0) & 0xff;
  uVar4 = (int)(GS_CurrentMaterial.Diffuse.r * 255.0) & 0xff;
  uVar5 = (int)(GS_CurrentMaterial.Emissive.g * 255.0) & 0xff;
  uVar6 = (int)(GS_CurrentMaterial.Emissive.b * 255.0) & 0xff;
  uVar7 = (int)(GS_CurrentMaterial.Diffuse.g * 255.0) & 0xff;
  uVar8 = (int)(GS_CurrentMaterial.Diffuse.b * 255.0) & 0xff;
  GS_CurrentMaterialAmbient =
       (int)(GS_CurrentMaterial.Ambient.a * 255.0) << 0x18 |
       ((int)(GS_CurrentMaterial.Ambient.r * 255.0) & 0xffU) << 0x10 |
       ((int)(GS_CurrentMaterial.Ambient.g * 255.0) & 0xffU) << 8 |
       (int)(GS_CurrentMaterial.Ambient.b * 255.0) & 0xffU;
  GS_CurrentMaterialDiffuse = uVar1 << 0x18 | uVar4 << 0x10 | uVar7 << 8 | uVar8;
  GS_CurrentMaterialDiffuseGX._3_1_ = (char)uVar1;
  GS_CurrentMaterialDiffuseGX._0_1_ = (char)uVar4;
  GS_CurrentMaterialDiffuseGX._1_1_ = (char)uVar7;
  GS_CurrentMaterialDiffuseGX._2_1_ = (char)uVar8;
  GS_CurrentMaterialEmissive = uVar2 << 0x18 | uVar3 << 0x10 | uVar5 << 8 | uVar6;
  GS_CurrentMaterialEmissivergba = uVar3 << 0x18 | uVar5 << 0x10 | uVar6 << 8 | uVar2;
  GS_CurrentMaterialEmissiveGX._3_1_ = (char)uVar2;
  GS_CurrentMaterialEmissiveGX._0_1_ = (char)uVar3;
  GS_CurrentMaterialEmissiveGX._1_1_ = (char)uVar5;
  GS_CurrentMaterialEmissiveGX._2_1_ = (char)uVar6;
  return;
}


s32 GS_SetLight(s32 Index,struct _D3DLIGHT8 *pLight)

{
  struct _D3DLIGHT8 *pLg;
  struct _D3DLIGHT8 *ptr;
  struct _D3DLIGHT8 *light;
  s32 size;
  
  size = 0x60;
  light = &GS_LightList[Index].Light;
  do {
    ptr = light;
    pLg = pLight;
    size = size + -0x18;
    ptr->Type = pLg->Type;
    (ptr->Diffuse).r = (pLg->Diffuse).r;
    (ptr->Diffuse).g = (pLg->Diffuse).g;
    (ptr->Diffuse).b = (pLg->Diffuse).b;
    (ptr->Diffuse).a = (pLg->Diffuse).a;
    pLight = (_D3DLIGHT8 *)&(pLg->Specular).g;
    (ptr->Specular).r = (pLg->Specular).r;
    light = (_D3DLIGHT8 *)&(ptr->Specular).g;
  } while (size != 0);
  *(float *)light = *(float *)pLight;
  (ptr->Specular).b = (pLg->Specular).b;
  return 0;
}


int GS_LightEnable(int Index,int Enable)

{
  GS_LightList[Index].EnableLight = Enable;
  return 0;
}


void GS_XFormLightVec(_GS_VECTOR3 *XFlight_pos,_GS_VECTOR4 *light_pos,_GSMATRIX *curmat)

{
  XFlight_pos->x =
       light_pos->z * curmat->mtx[2] + light_pos->x * curmat->mtx[0] + light_pos->y * curmat->mtx[ 1]
       + curmat->mtx[3];
  XFlight_pos->y =
       light_pos->z * curmat->mtx[1][2] +
       light_pos->x * curmat->mtx[1][0] + light_pos->y * curmat->mtx[1][1] + curmat->mtx[1][3];
  XFlight_pos->z =
       light_pos->z * curmat->mtx[2][2] +
       light_pos->x * curmat->mtx[2][0] + light_pos->y * curmat->mtx[2][1] + curmat->mtx[2][3];
  return;
}

void GS_SetLightingNone(void)

{
  _GXColor col;
  
  col = GX_White;
  GXSetChanAmbColor(GX_COLOR0A0,&col);
  col = (_GXColor)
        (((int)(GS_CurrentMaterial.Diffuse.b * 255.0) & 0xffU) << 8 |
        ((int)(GS_CurrentMaterial.Diffuse.g * 255.0) & 0xffU) << 0x10 |
        (int)(GS_CurrentMaterial.Diffuse.r * 255.0) << 0x18 |
        (int)(GS_CurrentMaterial.Diffuse.a * 255.0) & 0xffU);
  GXSetChanMatColor(GX_COLOR0A0,&col);
  if ((GS_MaterialSourceEmissive == 0) && (ShadowBodge == GX_TEVSTAGE0)) {
    GXSetChanCtrl(GX_COLOR0A0,'\0',GX_SRC_REG,GX_SRC_REG,0,GX_DF_NONE,GX_AF_NONE);
  }
  else {
    GXSetChanCtrl(GX_COLOR0A0,'\0',GX_SRC_REG,GX_SRC_VTX,0,GX_DF_NONE,GX_AF_NONE);
  }
  GXSetNumChans(1);
  return;
}

void GS_SetPointLighting(void)

{
  return;
}

void GS_EnableLighting(int flag)

{
  GS_EnableLightingFlag = flag;
  return;
}

void GS_SetMaterialSourceAmbient(int src)

{
  if (src != 0) {
    GS_MaterialSourceAmbient = 1;
    return;
  }
  GS_MaterialSourceAmbient = src;
  return;
}

void GS_SetMaterialSourceEmissive(int src)

{
  GS_MaterialSourceEmissive = src;
  return;
}

void GS_EnableColorVertex(int flag)

{
  GS_EnableColorVertexFlag = flag;
  return;
}

void GS_EnableSpecular(int flag)

{
  GS_EnableSpecularFlag = flag;
  return;
}


void GS_Set3Lights(_GS_VECTOR4 *LIGHT1_POS,_GS_VECTOR4 *LIGHT2_POS,_GS_VECTOR4 *LIGHT3_POS,
                  _GS_VECTOR4 *LIGHT1_COLOR,_GS_VECTOR4 *LIGHT2_COLOR,_GS_VECTOR4 *LIGHT3_COLOR,
                  _GS_VECTOR4 *AMB_COLOR)

{
  float *pfVar1;
  float *pfVar2;
  int iVar3;
  float *pfVar4;
  float *pfVar5;
  double dVar6;
  _GXColor col;
  
  if (GS_EnableLightingFlag == 0) {
    GS_SetLightingNone();
  }
  else {
    col = (_GXColor)((uint)AMB_COLOR & 0xff | 0x33333300);
    GXSetChanAmbColor(GX_COLOR0A0,&col);
    col = (_GXColor)
          (((int)(GS_CurrentMaterial.Diffuse.b * 255.0) & 0xffU) << 8 |
          ((int)(GS_CurrentMaterial.Diffuse.g * 255.0) & 0xffU) << 0x10 |
          (int)(GS_CurrentMaterial.Diffuse.r * 255.0) << 0x18 |
          (int)(GS_CurrentMaterial.Diffuse.a * 255.0) & 0xffU);
    GXSetChanMatColor(GX_COLOR0A0,&col);
    iVar3 = IsTitleScreen();
    if (iVar3 == 0) {
      iVar3 = 0x30;
      pfVar1 = (float *)&GS_CurMat;
      pfVar2 = (float *)&GS_LightMat;
      do {
        pfVar5 = pfVar2;
        pfVar4 = pfVar1;
        iVar3 = iVar3 + -0x18;
        *pfVar4 = *pfVar5;
        pfVar4[1] = pfVar5[1];
        pfVar4[2] = pfVar5[2];
        pfVar4[3] = pfVar5[3];
        (*(float (*) [4])(pfVar4 + 4))[0] = (*(float (*) [4])(pfVar5 + 4))[0];
        pfVar4[5] = pfVar5[5];
        pfVar1 = pfVar4 + 6;
        pfVar2 = pfVar5 + 6;
      } while (iVar3 != 0);
      pfVar4[6] = pfVar5[6];
      pfVar4[7] = pfVar5[7];
      pfVar4[8] = pfVar5[8];
      pfVar4[9] = pfVar5[9];
      GS_SetLightingMatrix(&GS_CurMat);
      iVar3 = 0x30;
      pfVar1 = (float *)&GS_LightViewMat;
      pfVar2 = (float *)&GS_CurMat;
      do {
        pfVar5 = pfVar2;
        pfVar4 = pfVar1;
        iVar3 = iVar3 + -0x18;
        *pfVar5 = *pfVar4;
        pfVar5[1] = pfVar4[1];
        pfVar5[2] = pfVar4[2];
        pfVar5[3] = pfVar4[3];
        (*(float (*) [4])(pfVar5 + 4))[0] = (*(float (*) [4])(pfVar4 + 4))[0];
        pfVar5[5] = pfVar4[5];
        pfVar1 = pfVar4 + 6;
        pfVar2 = pfVar5 + 6;
      } while (iVar3 != 0);
      pfVar5[6] = pfVar4[6];
      pfVar5[7] = pfVar4[7];
      pfVar5[8] = pfVar4[8];
      pfVar5[9] = pfVar4[9];
    }
    else {
      iVar3 = 0x30;
      pfVar1 = (float *)&GS_CurMat;
      pfVar2 = (float *)&GS_LightMat;
      do {
        pfVar5 = pfVar2;
        pfVar4 = pfVar1;
        iVar3 = iVar3 + -0x18;
        *pfVar4 = *pfVar5;
        pfVar4[1] = pfVar5[1];
        pfVar4[2] = pfVar5[2];
        pfVar4[3] = pfVar5[3];
        (*(float (*) [4])(pfVar4 + 4))[0] = (*(float (*) [4])(pfVar5 + 4))[0];
        pfVar4[5] = pfVar5[5];
        pfVar1 = pfVar4 + 6;
        pfVar2 = pfVar5 + 6;
      } while (iVar3 != 0);
      pfVar4[6] = pfVar5[6];
      pfVar4[7] = pfVar5[7];
      pfVar4[8] = pfVar5[8];
      pfVar4[9] = pfVar5[9];
      MatReorder(&GS_CurMat);
      GS_SetLightingMatrix2(&GS_CurMat);
    }
    GS_XFormLightVec(&XFLightPos,LIGHT1_POS,&GS_CurMat);
    GXInitLightPos((GXLightObj *)GSLights,XFLightPos.x,XFLightPos.y,XFLightPos.z);
    dVar6 = 255.0;
    col = (_GXColor)
          (((int)(LIGHT1_COLOR->z * 255.0) & 0xffU) << 8 |
          ((int)(LIGHT1_COLOR->y * 255.0) & 0xffU) << 0x10 |
          (int)(LIGHT1_COLOR->x * 255.0) << 0x18 | (int)(LIGHT1_COLOR->w * 255.0) & 0xffU);
    GXInitLightColor((GXLightObj *)GSLights,(GXColorOLD *)&col);
    GXLoadLightObjImm(GSLights);
    GS_XFormLightVec(&XFLightPos,LIGHT2_POS,&GS_CurMat);
    GXInitLightPos((GXLightObj *)(GSLights + 1),XFLightPos.x,XFLightPos.y,XFLightPos.z);
    col = (_GXColor)
          (((int)((double)LIGHT2_COLOR->z * dVar6) & 0xffU) << 8 |
          ((int)((double)LIGHT2_COLOR->y * dVar6) & 0xffU) << 0x10 |
          (int)((double)LIGHT2_COLOR->x * dVar6) << 0x18 |
          (int)((double)LIGHT2_COLOR->w * dVar6) & 0xffU);
    GXInitLightColor((GXLightObj *)(GSLights + 1),(GXColorOLD *)&col);
    GXLoadLightObjImm(GSLights + 1);
    GS_XFormLightVec(&XFLightPos,LIGHT3_POS,&GS_CurMat);
    GXInitLightPos((GXLightObj *)(GSLights + 2),XFLightPos.x,XFLightPos.y,XFLightPos.z);
    col = (_GXColor)
          (((int)((double)LIGHT3_COLOR->z * dVar6) & 0xffU) << 8 |
          ((int)((double)LIGHT3_COLOR->y * dVar6) & 0xffU) << 0x10 |
          (int)((double)LIGHT3_COLOR->x * dVar6) << 0x18 |
          (int)((double)LIGHT3_COLOR->w * dVar6) & 0xffU);
    GXInitLightColor((GXLightObj *)(GSLights + 2),(GXColorOLD *)&col);
    GXLoadLightObjImm(GSLights + 2);
    GXSetChanCtrl(GX_COLOR0,'\x01',GX_SRC_REG,GX_SRC_REG,7,GX_DF_CLAMP,GX_AF_NONE);
    GXSetChanCtrl(GX_ALPHA0,'\0',GX_SRC_REG,GX_SRC_VTX,0,GX_DF_CLAMP,GX_AF_NONE);
    GXSetNumChans(1);
  }
  return;
}


