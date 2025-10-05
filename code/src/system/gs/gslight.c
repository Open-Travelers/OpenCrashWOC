#include "gslight.h"

struct _D3DMATERIAL8 GS_CurrentMaterial;
struct _GXColor GS_CurrentMaterialAmbient;
struct _GXColor GS_CurrentMaterialDiffuse;
struct _GXColor GS_CurrentMaterialDiffuseGX;
struct _GXColor GS_CurrentMaterialEmissive;
struct _GXColor GS_CurrentMaterialEmissiveGX;
struct _GXColor GS_CurrentMaterialEmissivergba;

//MATCH NGC
void GS_SetMaterial(struct _D3DMATERIAL8 *pMaterial) {
    u8 diff_b;
    u8 diff_r;
    u8 diff_g;
    u8 diff_a;
    u8 em_r;
    u8 em_g;
    u8 em_b;
    u8 em_a;

    GS_CurrentMaterial = *pMaterial;
    *(u32*)&GS_CurrentMaterialAmbient = (u32)(
        ((s32) (255.0f * GS_CurrentMaterial.Ambient.a) << 0x18) & 0xff000000 | 
        ((s32) (255.0f * GS_CurrentMaterial.Ambient.r) << 0x10) & 0xff0000| 
        ((s32) (255.0f * GS_CurrentMaterial.Ambient.g) << 8) & 0xff00| 
        (s32) (255.0f * GS_CurrentMaterial.Ambient.b) & 0xFF
    );

    diff_a = (s8) (GS_CurrentMaterial.Diffuse.a * 255.0f); 
    diff_r = (s8) (GS_CurrentMaterial.Diffuse.r * 255.0f);
    diff_g = (s8) (GS_CurrentMaterial.Diffuse.g * 255.0f);
    diff_b = (s8) (GS_CurrentMaterial.Diffuse.b * 255.0f);
    
    em_a = (s8) (GS_CurrentMaterial.Emissive.a * 255.0f);
    em_r = (s8) (GS_CurrentMaterial.Emissive.r * 255.0f);
    em_g = (s8) (GS_CurrentMaterial.Emissive.g * 255.0f);
    em_b = (s8) (GS_CurrentMaterial.Emissive.b * 255.0f);
    
     
    *(u32*)&GS_CurrentMaterialDiffuse =  (u32)((diff_a << 0x18) | (diff_r << 0x10) | (diff_g << 8) | diff_b << 0);
    GS_CurrentMaterialDiffuseGX.a = diff_a;
    GS_CurrentMaterialDiffuseGX.r = diff_r;
    GS_CurrentMaterialDiffuseGX.g = diff_g;
    GS_CurrentMaterialDiffuseGX.b = diff_b;
    *(u32*)&GS_CurrentMaterialEmissive = (u32)((em_a << 0x18) | (em_r << 0x10) | (em_g << 8) | em_b);
    *(u32*)&GS_CurrentMaterialEmissivergba = (u32)((em_r << 0x18) | (em_g << 0x10) | (em_b << 8) | em_a);
    GS_CurrentMaterialEmissiveGX.a = em_a; 
    GS_CurrentMaterialEmissiveGX.r = em_r; 
    GS_CurrentMaterialEmissiveGX.g = em_g;
    GS_CurrentMaterialEmissiveGX.b = em_b;
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

//NGC MATCH
void GS_SetLightingNone(void) {
    struct _GXColor col;

    GXSetChanAmbColor(GX_COLOR0A0,GX_White);
    col.a = (s8) (GS_CurrentMaterial.Diffuse.a * 255.0f); 
    col.r = (s8) (GS_CurrentMaterial.Diffuse.r * 255.0f);
    col.g = (s8) (GS_CurrentMaterial.Diffuse.g * 255.0f);
    col.b = (s8) (GS_CurrentMaterial.Diffuse.b * 255.0f);
    
    GXSetChanMatColor(GX_COLOR0A0,col);
    if ((GS_MaterialSourceEmissive != 0) || (ShadowBodge != GX_TEVSTAGE0)) {
      GXSetChanCtrl(GX_COLOR0A0,0,GX_SRC_REG,GX_SRC_VTX,0,GX_DF_NONE,GX_AF_NONE);
    }
    else {
      GXSetChanCtrl(GX_COLOR0A0,0,GX_SRC_REG,GX_SRC_REG,0,GX_DF_NONE,GX_AF_NONE);
    }
    GXSetNumChans(1);
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
void GXInitLightColor(struct _GXLightObj* lt_obj, struct _GXColor color);                   
void GXInitLightPos(struct _GXLightObj* lt_obj, float x, float y, float z);               
void GXLoadLightObjImm(struct _GXLightObj* lt_obj, enum _GXLightID light);                  
void GXSetChanAmbColor(enum _GXChannelID chan, struct _GXColor amb_color);               
void GXSetChanCtrl(enum _GXChannelID chan, unsigned char enable, enum _GXColorSrc amb_src, 
                        enum _GXColorSrc mat_src, unsigned long light_mask, 
                                            enum _GXDiffuseFn diff_fn, enum _GXAttnFn attn_fn);               
void GXSetChanMatColor(enum _GXChannelID chan, struct _GXColor mat_color);                      
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


//NGC MATCH
void GS_Set3Lights(struct _GS_VECTOR4 *LIGHT1_POS,struct _GS_VECTOR4 *LIGHT2_POS,struct _GS_VECTOR4 *LIGHT3_POS,
                  struct _GS_VECTOR4 *LIGHT1_COLOR,struct _GS_VECTOR4 *LIGHT2_COLOR,
                    struct _GS_VECTOR4 *LIGHT3_COLOR, struct _GXColor* AMB_COLOR) {
    
    struct _GXColor ambient;
    struct _GXColor colour;
    struct _GXColor mat_color;
    
    if (GS_EnableLightingFlag == 0) {
        GS_SetLightingNone();
        return;
    }
    //ambient = (_GXColor)((uint)AMB_COLOR & 0xff | 0x33333300);
    ambient.r = 0x33;
    ambient.g = 0x33;
    ambient.b = 0x33;
    GXSetChanAmbColor(GX_COLOR0A0,ambient);
    mat_color.a = (s32)(GS_CurrentMaterial.Diffuse.a * 255.0f);
    mat_color.r = (s32)(GS_CurrentMaterial.Diffuse.r * 255.0f);
    mat_color.g = (s32)(GS_CurrentMaterial.Diffuse.g * 255.0f);
    mat_color.b = (s32)(GS_CurrentMaterial.Diffuse.b * 255.0f);
    GXSetChanMatColor(GX_COLOR0A0,mat_color);
    if (IsTitleScreen() != 0) {
        GS_CurMat = GS_LightMat;
        MatReorder(&GS_CurMat);
        GS_SetLightingMatrix2(&GS_CurMat);
    } else {
        GS_CurMat = GS_LightMat;
        GS_SetLightingMatrix(&GS_CurMat);
        GS_CurMat = GS_LightViewMat;
    }
    GS_XFormLightVec(&XFLightPos,LIGHT1_POS,&GS_CurMat);
    GXInitLightPos(&GSLights[0],XFLightPos.x,XFLightPos.y,XFLightPos.z);
    colour.a = (s32)(LIGHT1_COLOR->w * 255.0f);
    colour.r = (s32)(LIGHT1_COLOR->x * 255.0f);
    colour.g = (s32)(LIGHT1_COLOR->y * 255.0f);
    colour.b = (s32)(LIGHT1_COLOR->z * 255.0f);
    GXInitLightColor(&GSLights[0],colour);
    GXLoadLightObjImm(&GSLights[0],GX_LIGHT0);
    GS_XFormLightVec(&XFLightPos,LIGHT2_POS,&GS_CurMat);
    GXInitLightPos(&GSLights[1],XFLightPos.x,XFLightPos.y,XFLightPos.z);
    colour.a = (s32)(LIGHT2_COLOR->w * 255.0f);
    colour.r = (s32)(LIGHT2_COLOR->x * 255.0f);
    colour.g = (s32)(LIGHT2_COLOR->y * 255.0f);
    colour.b = (s32)(LIGHT2_COLOR->z * 255.0f);
    GXInitLightColor(&GSLights[1],colour);
    GXLoadLightObjImm(&GSLights[1],GX_LIGHT1);
    GS_XFormLightVec(&XFLightPos,LIGHT3_POS,&GS_CurMat);
    GXInitLightPos(&GSLights[2],XFLightPos.x,XFLightPos.y,XFLightPos.z);
    colour.a = (s32)(LIGHT3_COLOR->w * 255.0f);
    colour.r = (s32)(LIGHT3_COLOR->x * 255.0f);
    colour.g = (s32)(LIGHT3_COLOR->y * 255.0f);
    colour.b = (s32)(LIGHT3_COLOR->z * 255.0f); 
    GXInitLightColor(&GSLights[2],colour);
    GXLoadLightObjImm(&GSLights[2],GX_LIGHT2);
    GXSetChanCtrl(GX_COLOR0,1,GX_SRC_REG,GX_SRC_REG,7,GX_DF_CLAMP,GX_AF_NONE);
    GXSetChanCtrl(GX_ALPHA0,0,GX_SRC_REG,GX_SRC_VTX,0,GX_DF_CLAMP,GX_AF_NONE);
    GXSetNumChans(1);
}