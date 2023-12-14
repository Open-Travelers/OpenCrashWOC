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


s32 GS_LightEnable(s32 Index, s32 Enable) {
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

/*
void GS_SetLightingMatrix(struct _GSMATRIX * mtx);                         
//void GS_SetLightingMatrix2 (struct _GSMATRIX * m);                    
void GS_SetLightingNone ();                             
void GS_XFormLightVec(struct _GS_VECTOR3 * XFlight_pos, struct _GS_VECTOR4 * light_pos, struct _GSMATRIX * curmat);                    
void GXInitLightColor(struct _GXLightObj* lt_obj, struct _GXColor* color);                   
void GXInitLightPos(struct _GXLightObj* lt_obj, float x, float y, float z);               
void GXLoadLightObjImm(struct _GXLightObj* lt_obj, enum _GXLightID light);                  
void GXSetChanAmbColor(enum _GXChannelID chan, struct _GXColor* amb_color);               
void GXSetChanCtrl(enum _GXChannelID chan, unsigned char enable, enum _GXColorSrc amb_src, 
                        enum _GXColorSrc mat_src, unsigned long light_mask, 
                                            enum _GXDiffuseFn diff_fn, enum _GXAttnFn attn_fn);               
void GXSetChanMatColor(enum _GXChannelID chan, struct _GXColor* mat_color);                      
void GXSetNumChans(unsigned char nChans);                                
s32 IsTitleScreen();                               
//void MatReorder(struct _GSMATRIX* MatrixA);                               
extern struct _GXLightObj GSLights[3];
extern struct _GSMATRIX GS_CurMat;
extern struct _D3DMATERIAL8 GS_CurrentMaterial;
extern s32 GS_EnableLightingFlag;
extern struct _GSMATRIX GS_LightMat;
extern struct _GSMATRIX GS_LightViewMat;
extern struct _GS_VECTOR3 XFLightPos;
*/


//55%
void GS_Set3Lights(struct _GS_VECTOR4 *LIGHT1_POS,struct _GS_VECTOR4 *LIGHT2_POS,struct _GS_VECTOR4 *LIGHT3_POS,
                  struct _GS_VECTOR4 *LIGHT1_COLOR,struct _GS_VECTOR4 *LIGHT2_COLOR,
                    struct _GS_VECTOR4 *LIGHT3_COLOR, struct _GS_VECTOR4 *AMB_COLOR) 
{
    struct _GSMATRIX *p_Var1;
    struct _GSMATRIX *p_Var2;
    int iVar3;
    struct _GSMATRIX *p_Var4;
    struct _GSMATRIX *p_Var5;
    struct _GXColor col;
    
    if (GS_EnableLightingFlag == 0) {
        GS_SetLightingNone();
    }
    else {
        //col = (struct _GXColor)((uint)AMB_COLOR & 0xff | 0x33333300);
        GXSetChanAmbColor(GX_COLOR0A0,&col);
    /*    col = (struct _GXColor)
              (((int)(GS_CurrentMaterial.Diffuse.b * 255.0f) & 0xffU) << 8 |
              ((int)(GS_CurrentMaterial.Diffuse.g * 255.0f) & 0xffU) << 0x10 |
              (int)(GS_CurrentMaterial.Diffuse.r * 255.0f) << 0x18 |
              (int)(GS_CurrentMaterial.Diffuse.a * 255.0f) & 0xffU);    */
        GXSetChanMatColor(GX_COLOR0A0,&col);
        iVar3 = IsTitleScreen();
        if (iVar3 != 0) {
            GS_CurMat = GS_LightMat;
            MatReorder(&GS_CurMat);
            GS_SetLightingMatrix2(&GS_CurMat);
        }
        else {
            GS_LightMat = GS_CurMat;
            GS_SetLightingMatrix(&GS_CurMat);
            GS_CurMat = GS_LightViewMat;
        }
        GS_XFormLightVec(&XFLightPos,LIGHT1_POS,&GS_CurMat);
        GXInitLightPos(&GSLights[1],XFLightPos.x,XFLightPos.y,XFLightPos.z);
        //dVar6 = 255.0;
        //col = (struct _GXColor)
         //     (((int)(LIGHT1_COLOR->z * 255.0f) & 0xffU) << 8 |
        //      ((int)(LIGHT1_COLOR->y * 255.0f) & 0xffU) << 0x10 |
          //    (int)(LIGHT1_COLOR->x * 255.0f) << 0x18 | (int)(LIGHT1_COLOR->w * 255.0f) & 0xffU);
        GXInitLightColor(GSLights,&col);
        GXLoadLightObjImm(GSLights,GX_LIGHT0);
        GS_XFormLightVec(&XFLightPos,LIGHT2_POS,&GS_CurMat);
        GXInitLightPos((GSLights + 1),XFLightPos.x,XFLightPos.y,XFLightPos.z);
        //col = (struct _GXColor)
         //     (((int)((double)LIGHT2_COLOR->z * 255.0f) & 0xffU) << 8 |
         //     ((int)((double)LIGHT2_COLOR->y * 255.0f) & 0xffU) << 0x10 |
         //     (int)((double)LIGHT2_COLOR->x * 255.0f) << 0x18 |
          //    (int)((double)LIGHT2_COLOR->w * 255.0f) & 0xffU);
        GXInitLightColor(GSLights + 1,&col);
        GXLoadLightObjImm(GSLights + 1,GX_LIGHT1);
        GS_XFormLightVec(&XFLightPos,LIGHT3_POS,&GS_CurMat);
        GXInitLightPos((GSLights + 2),XFLightPos.x,XFLightPos.y,XFLightPos.z);
        //col = (struct _GXColor)
         //     (((int)((double)LIGHT3_COLOR->z * 255.0f) & 0xffU) << 8 |
          //    ((int)((double)LIGHT3_COLOR->y * 255.0f) & 0xffU) << 0x10 |
          //    (int)((double)LIGHT3_COLOR->x * 255.0f) << 0x18 |
           //   (int)((double)LIGHT3_COLOR->w * 255.0f) & 0xffU);
        GXInitLightColor(GSLights + 2,&col);
        GXLoadLightObjImm(GSLights + 2,GX_LIGHT2);
        GXSetChanCtrl(GX_COLOR0,'\x01',GX_SRC_REG,GX_SRC_REG,7,GX_DF_CLAMP,GX_AF_NONE);
        GXSetChanCtrl(GX_ALPHA0,'\0',GX_SRC_REG,GX_SRC_VTX,0,GX_DF_CLAMP,GX_AF_NONE);
        GXSetNumChans('\x01');
    }
    return;
}

