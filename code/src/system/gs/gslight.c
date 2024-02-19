#include "gslight.h"

//56% NGC
void GS_SetMaterial(const struct _D3DMATERIAL8 *pMaterial) { 
    u32 uVar1;
    u32 uVar2;
    u32 uVar3;
    u32 uVar4;

    GS_CurrentMaterial = *pMaterial;

    uVar2 = (int)(GS_CurrentMaterial.Emissive.r * 255.0f) & 0xff;
    uVar3 = (int)(GS_CurrentMaterial.Emissive.g * 255.0f) & 0xff;
    uVar4 = (int)(GS_CurrentMaterial.Emissive.b * 255.0f) & 0xff;
    uVar1 = (int)(GS_CurrentMaterial.Emissive.a * 255.0f) & 0xff;
    GS_CurrentMaterialAmbient =
         (struct _GXColor *)
         ((int)(GS_CurrentMaterial.Ambient.a * 255.0f) << 0x18 |
          ((int)(GS_CurrentMaterial.Ambient.r * 255.0f) & 0xffU) << 0x10 |
          ((int)(GS_CurrentMaterial.Ambient.g * 255.0f) & 0xffU) << 8 |
         (int)(GS_CurrentMaterial.Ambient.b * 255.0f) & 0xffU);
    GS_CurrentMaterialDiffuse =
         (struct _GXColor *)
         ((int)(GS_CurrentMaterial.Diffuse.a * 255.0f) << 0x18 |
          ((int)(GS_CurrentMaterial.Diffuse.r * 255.0f) & 0xffU) << 0x10 |
          ((int)(GS_CurrentMaterial.Diffuse.g * 255.0f) & 0xffU) << 8 |
         (int)(GS_CurrentMaterial.Diffuse.b * 255.0f) & 0xffU);

    GS_CurrentMaterialDiffuseGX->r = (s32)(GS_CurrentMaterial.Diffuse.r * 255.0f) << 0x18;
    GS_CurrentMaterialDiffuseGX->g = (s32)(GS_CurrentMaterial.Diffuse.g * 255.0f) << 0x10;
    GS_CurrentMaterialDiffuseGX->b = (s32)(GS_CurrentMaterial.Diffuse.b * 255.0f) << 8;
    GS_CurrentMaterialDiffuseGX->a = (s32)(GS_CurrentMaterial.Diffuse.a * 255.0f);
    GS_CurrentMaterialEmissive = (struct _GXColor *)(uVar1 << 0x18 | uVar2 << 0x10 | uVar3 << 8 | uVar4);
     /*     GS_CurrentMaterialEmissive->r = (s32)(GS_CurrentMaterial.Emissive.r * 255.0f) << 0x18;
          GS_CurrentMaterialEmissive->g = (s32)(GS_CurrentMaterial.Emissive.g * 255.0f) << 0x10;
          GS_CurrentMaterialEmissive->b = (s32)(GS_CurrentMaterial.Emissive.b * 255.0f) << 8;
          GS_CurrentMaterialEmissive->a = (s32)(GS_CurrentMaterial.Emissive.a * 255.0f);*/
    
    GS_CurrentMaterialEmissivergba =
         (struct _GXColor *)(uVar2 << 0x18 | uVar3 << 0x10 | uVar4 << 8 | uVar1);
    GS_CurrentMaterialEmissiveGX->r = uVar2;
    GS_CurrentMaterialEmissiveGX->g = uVar3;
    GS_CurrentMaterialEmissiveGX->b = uVar4;
    GS_CurrentMaterialEmissiveGX->a = uVar1;
    return;
}

//NGC MATCH
s32 GS_SetLight(s32 Index,const struct _D3DLIGHT8 *pLight) {
  struct _D3DLIGHT8 *ptr = &GS_LightList[Index].Light;

    *ptr = *pLight;

  return 0;
}

//NGC MATCH
s32 GS_LightEnable(s32 Index, s32 Enable) {
    GS_LightList[Index].EnableLight = Enable;
    return 0;
}

//NGC MATCH
void GS_XFormLightVec(struct _GS_VECTOR3 *XFlight_pos,struct _GS_VECTOR4 *light_pos,struct _GSMATRIX *curmat) {
  XFlight_pos->x = light_pos->x * curmat->_11 + light_pos->y * curmat->_12 + light_pos->z * curmat->_13 + curmat->_14;
  XFlight_pos->y = light_pos->x * curmat->_21 + light_pos->y * curmat->_22 + light_pos->z * curmat->_23 + curmat->_24;
  XFlight_pos->z = light_pos->x * curmat->_31 + light_pos->y * curmat->_32 + light_pos->z * curmat->_33 + curmat->_34;
  return;
}

//75% NGC
void GS_SetLightingNone(void) {
  struct _GXColor col;
  
  col = GX_White;
  GXSetChanAmbColor(GX_COLOR0A0,&col);
  col.r = (s32)(GS_CurrentMaterial.Diffuse.r * 255.0f);
  col.g = (s32)(GS_CurrentMaterial.Diffuse.g * 255.0f);
  col.b = (s32)(GS_CurrentMaterial.Diffuse.b * 255.0f);
  col.a = (s32)(GS_CurrentMaterial.Diffuse.a * 255.0f);
 /* col = //(struct _GXColor)
        (((int)(GS_CurrentMaterial.Diffuse.b * 255.0) & 0xffU) << 8 |
        ((int)(GS_CurrentMaterial.Diffuse.g * 255.0) & 0xffU) << 0x10 |
        (int)(GS_CurrentMaterial.Diffuse.r * 255.0) << 0x18 |
        (int)(GS_CurrentMaterial.Diffuse.a * 255.0) & 0xffU);*/
  GXSetChanMatColor(GX_COLOR0A0,&col);
  if ((GS_MaterialSourceEmissive != 0) || (ShadowBodge != GX_TEVSTAGE0)) {
    GXSetChanCtrl(GX_COLOR0A0,0,GX_SRC_REG,GX_SRC_VTX,0,GX_DF_NONE,GX_AF_NONE);
  }
  else {
    GXSetChanCtrl(GX_COLOR0A0,0,GX_SRC_REG,GX_SRC_REG,0,GX_DF_NONE,GX_AF_NONE);
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

//NGC MATCH
void GS_SetMaterialSourceAmbient(s32 src) {
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


//78% NGC
void GS_Set3Lights(struct _GS_VECTOR4 *LIGHT1_POS,struct _GS_VECTOR4 *LIGHT2_POS,struct _GS_VECTOR4 *LIGHT3_POS,
                  struct _GS_VECTOR4 *LIGHT1_COLOR,struct _GS_VECTOR4 *LIGHT2_COLOR,
                    struct _GS_VECTOR4 *LIGHT3_COLOR, struct _GS_VECTOR4 *AMB_COLOR) {
    struct _GXColor col;
    struct _GXColor ambient;
    //struct _GXColor mat_color;
    
    if (GS_EnableLightingFlag == 0) {
        GS_SetLightingNone();
    }
    else {
        col.r = ((s32)AMB_COLOR->x & 0x33333300);
        col.g = ((s32)AMB_COLOR->y & 0x33333300);
        col.b = ((s32)AMB_COLOR->z & 0x33333300);
        col.a = ((s32)AMB_COLOR->w & 0x33333300);
        GXSetChanAmbColor(GX_COLOR0A0,&col);
          col.r = (s32)(GS_CurrentMaterial.Diffuse.r * 255.0f);
          col.g = (s32)(GS_CurrentMaterial.Diffuse.g * 255.0f);
          col.b = (s32)(GS_CurrentMaterial.Diffuse.b * 255.0f);
          col.a = (s32)(GS_CurrentMaterial.Diffuse.a * 255.0f);
    /*    col = (struct _GXColor)
              (((int)(GS_CurrentMaterial.Diffuse.b * 255.0f) & 0xffU) << 8 |
              ((int)(GS_CurrentMaterial.Diffuse.g * 255.0f) & 0xffU) << 0x10 |
              (int)(GS_CurrentMaterial.Diffuse.r * 255.0f) << 0x18 |
              (int)(GS_CurrentMaterial.Diffuse.a * 255.0f) & 0xffU);    */
        GXSetChanMatColor(GX_COLOR0A0,&col);
        if (IsTitleScreen() != 0) {
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
          col.r = (s32)(LIGHT1_COLOR->w * 255.0f);
          col.g = (s32)(LIGHT1_COLOR->x * 255.0f);
          col.b = (s32)(LIGHT1_COLOR->y * 255.0f);
          col.a = (s32)(LIGHT1_COLOR->z * 255.0f);
        GXInitLightColor(&GSLights[0],&col);
        GXLoadLightObjImm(&GSLights[0],GX_LIGHT0);
        GS_XFormLightVec(&XFLightPos,LIGHT2_POS,&GS_CurMat);
        GXInitLightPos(&GSLights[1],XFLightPos.x,XFLightPos.y,XFLightPos.z);
          col.r = (s32)(LIGHT2_COLOR->w * 255.0f);
          col.g = (s32)(LIGHT2_COLOR->x * 255.0f);
          col.b = (s32)(LIGHT2_COLOR->y * 255.0f);
          col.a = (s32)(LIGHT2_COLOR->z * 255.0f);
        //col = (struct _GXColor)
         //     (((int)((double)LIGHT2_COLOR->z * 255.0f) & 0xffU) << 8 |
         //     ((int)((double)LIGHT2_COLOR->y * 255.0f) & 0xffU) << 0x10 |
         //     (int)((double)LIGHT2_COLOR->x * 255.0f) << 0x18 |
          //    (int)((double)LIGHT2_COLOR->w * 255.0f) & 0xffU);
        GXInitLightColor(&GSLights[1],&col);
        GXLoadLightObjImm(&GSLights[1],GX_LIGHT1);
        GS_XFormLightVec(&XFLightPos,LIGHT3_POS,&GS_CurMat);
        GXInitLightPos(&GSLights[2],XFLightPos.x,XFLightPos.y,XFLightPos.z);
          col.r = (s32)(LIGHT3_COLOR->w * 255.0f);
          col.g = (s32)(LIGHT3_COLOR->x * 255.0f);
          col.b = (s32)(LIGHT3_COLOR->y * 255.0f);
          col.a = (s32)(LIGHT3_COLOR->z * 255.0f);
        //col = (struct _GXColor)
         //     (((int)((double)LIGHT3_COLOR->z * 255.0f) & 0xffU) << 8 |
          //    ((int)((double)LIGHT3_COLOR->y * 255.0f) & 0xffU) << 0x10 |
          //    (int)((double)LIGHT3_COLOR->x * 255.0f) << 0x18 |
           //   (int)((double)LIGHT3_COLOR->w * 255.0f) & 0xffU);
        GXInitLightColor(&GSLights[2],&col);
        GXLoadLightObjImm(&GSLights[2],GX_LIGHT2);
        GXSetChanCtrl(GX_COLOR0,1,GX_SRC_REG,GX_SRC_REG,7,GX_DF_CLAMP,GX_AF_NONE);
        GXSetChanCtrl(GX_ALPHA0,0,GX_SRC_REG,GX_SRC_VTX,0,GX_DF_CLAMP,GX_AF_NONE);
        GXSetNumChans(1);
    }
    return;
}
