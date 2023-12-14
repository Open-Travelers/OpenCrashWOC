#include"system/port.h"

void CloseShaders(void) {
    ResetShaders();
    RemoveWaterCausticTextures();
    return;
}

void ResetShaders(void) {
    SetupShaders(NULL);
    return;
}

//NGC MATCH
void InitialiseShaders(s32 Level) {
    NuMtxSetIdentity(&identmtx);
    currentLevel = Level;
    defaultShader = NO_SHADER;
    if (((Level == 2) || (Level == 0x22)) || (Level == 6)) {
        defaultShader = WATERCAUSTICS;
    }
    else if ((Level == 0x1b) && (IsLoadingScreen == 0)) {
        defaultShader = POINTLIGHT;
    }
    LoadWaterCausticTextures();
}

//NGC MATCH
void NuShaderSetBypassShaders(s32 flag) {
    bypassEffectShaders = flag;
    return;
}

//NGC MATCH
void NuShaderSetSkinningConstants(struct nugeomitem_s* item, struct nuprim_s* prim) {
  s32 i;
  s32 lookup;

  lookup = (s32)prim->skinmtxlookup;
  for(i = 0; i < 0x10; i++, lookup+= 2) {
    if (*(short *)lookup != -1) {
      NuMtxTranspose((struct numtx_s *)&CV_SKINMTX[i], &item->mtx[*(short *)lookup]);
    }
  }
  return;
}

//NGC MATCH
void NuShaderSetColourConstants(struct numtl_s *mtl) {
    if ((mtl != NULL) && (mtl->attrib.lighting != 2 || mtl->attrib.colour != 1)) {
        CV_MTL_COLOR = *(struct _GS_VECTOR4 *)makenuvec4((mtl->diffuse).r,(mtl->diffuse).g,(mtl->diffuse).b,mtl->alpha);
        NEW_CV_COLOR_TYPE = 1;
    }
    else {
        CV_MTL_COLOR = *(struct _GS_VECTOR4 *)makenuvec4(0.0f,0.0f,0.0f,0.0f);
        NEW_CV_COLOR_TYPE = 2;
    }
    return;
}

//NGC MATCH
void NuShaderSetLightConstants(struct numtl_s *mtl) {
    struct nulight_s *l1;
    struct nulight_s *l2;
    struct nulight_s *l3;

    if (mtl->attrib.lighting == 2) {
        CV_LIGHT1_COLOR.x = 0.0f;
        CV_LIGHT1_COLOR.y = 0.0f;
        CV_LIGHT1_COLOR.z = 0.0f;
        CV_LIGHT1_COLOR.w = 0.0f;
        CV_LIGHT2_COLOR.x = 0.0f;
        CV_LIGHT2_COLOR.y = 0.0f;
        CV_LIGHT2_COLOR.z = 0.0f;
        CV_LIGHT2_COLOR.w = 0.0f;
        CV_LIGHT3_COLOR.x = 0.0f;
        CV_LIGHT3_COLOR.y = 0.0f;
        CV_LIGHT3_COLOR.z = 0.0f;
        CV_LIGHT3_COLOR.w = 0.0f;
        CV_LIGHT1_POS.x = 0.0f;
        CV_LIGHT1_POS.y = 0.0f;
        CV_LIGHT1_POS.z = 0.0f;
        CV_LIGHT1_POS.w = 1.0f;
        CV_LIGHT2_POS.x = 0.0f;
        CV_LIGHT2_POS.y = 0.0f;
        CV_LIGHT2_POS.z = 0.0f;
        CV_LIGHT2_POS.w = 1.0f;
        CV_LIGHT3_POS.x = 0.0f;
        CV_LIGHT3_POS.y = 0.0f;
        CV_LIGHT3_POS.z = 0.0f;
        CV_LIGHT3_POS.w = 1.0f;
        CV_AMBIENT_COLOR.x = 0.0f;
        CV_AMBIENT_COLOR.y = 0.0f;
        CV_AMBIENT_COLOR.z = 0.0f;
        CV_AMBIENT_COLOR.w = 0.0f; 
        newCV_LIGHTS_OFF = 1;
    }
    else {
        l1 = NuLightGetLight(0);
        l2 = NuLightGetLight(1);
        l3 = NuLightGetLight(2);
        CV_LIGHT1_POS.x = (l1->mtx)._20;
        CV_LIGHT1_POS.y = (l1->mtx)._21;
        CV_LIGHT1_POS.z = (l1->mtx)._22;
        CV_LIGHT1_POS.w = 1.0f;
        CV_LIGHT2_POS.x = (l2->mtx)._20;
        CV_LIGHT2_POS.y = (l2->mtx)._21;
        CV_LIGHT2_POS.z = (l2->mtx)._22;
        CV_LIGHT2_POS.w = 1.0f;
        CV_LIGHT3_POS.x = (l3->mtx)._20;
        CV_LIGHT3_POS.y = (l3->mtx)._21;
        CV_LIGHT3_POS.z = (l3->mtx)._22;
        CV_LIGHT3_POS.w = 1.0f;
        CV_LIGHT1_COLOR.x = (l1->diffuse).r;
        CV_LIGHT1_COLOR.y = (l1->diffuse).g;
        CV_LIGHT1_COLOR.z = (l1->diffuse).b;
        CV_LIGHT1_COLOR.w = 0.0f;
        CV_LIGHT2_COLOR.x = (l2->diffuse).r;
        CV_LIGHT2_COLOR.y = (l2->diffuse).g;
        CV_LIGHT2_COLOR.z = (l2->diffuse).b;
        CV_LIGHT2_COLOR.w = 0.0f;
        CV_LIGHT3_COLOR.x = (l3->diffuse).r;
        CV_LIGHT3_COLOR.y = (l3->diffuse).g;
        CV_LIGHT3_COLOR.z = (l3->diffuse).b;
        CV_LIGHT3_COLOR.w = 0.0f;
        CV_AMBIENT_COLOR.x = (l1->ambient).r / 3.0f;
        CV_AMBIENT_COLOR.y = (l1->ambient).g / 3.0f;
        CV_AMBIENT_COLOR.z = (l1->ambient).b / 3.0f;
        CV_AMBIENT_COLOR.w = 1.0f;
        newCV_LIGHTS_OFF = 0;
    }

    GS_Set3Lights(&CV_LIGHT1_POS,&CV_LIGHT2_POS,&CV_LIGHT3_POS,&CV_LIGHT1_COLOR,&CV_LIGHT2_COLOR,
                  &CV_LIGHT3_COLOR,&CV_AMBIENT_COLOR);
}

void NuShaderSetGlassMix(float mix) {
    glassmix = mix;
    return;
}

//EMPTY FUNCTION
void SetPixelShader(u32 Handle) {
    return;
}

//NGC MATCH
void SetVertexShader(u32 Handle) {
    
    switch (Handle) {
    case 0x144:
    case 0x53:
        shaderselected = 1;
        xytype = 1;
        ShaderHasNormals = 0;
        return;
    case 0x152:
    case 0x59:
        shaderselected = 2;
        xytype = 0;
        ShaderHasNormals = 1;
        return;
    case 0x142:
        shaderselected = 3;
        xytype = 0;
        ShaderHasNormals = 0;
        return;
    case 0x5D:
        shaderselected = 4;
        xytype = 0;
        ShaderHasNormals = 1;
        return;
    case 0x11:
        shaderselected = 5;
        xytype = 2;
        ShaderHasNormals = 0;
        return;
    default:
            DisplayErrorAndLockup
                      ("C:/source/crashwoc/code/system/port.c",0x70,"D3DDevice_SetVertexShader");
        return;
    }
}

//NGC MATCH
void NuSetShaderState(enum shadertypes_e type, enum nuvtxtype_e vtxtype) {

    currentshader = type;
    currentvtxtype = vtxtype;
    if (type == -1) { 
        gcc2_compiled__N92(0x152);
        SetPixelShader(0);
    } 
    else if (type == 0) {
        switch (vtxtype) {                             
        case 0:   //NUVT_TC1                               
        case 1:
        case 0x59:
            NUVT_TC1:
            gcc2_compiled__N92(0x152);
            break;
        case 0x53:  //NUVT_TLTC1
            gcc2_compiled__N92(0x144);
            break;
        case 0x51: //NUVT_LC1
            gcc2_compiled__N92(0x142);
            break;
        }
        SetPixelShader(0);
    } 
    else if (type == 0x80) {
        gcc2_compiled__N92(0x59);
    }
    else {
        if (Shaders[type].vertshader) {
            gcc2_compiled__N92(Shaders[type].vertshader);
        } else {
            gcc2_compiled__N92(0);
        }
        
        if (Shaders[type].pixelShaderHandle) {
            SetPixelShader(Shaders[type].pixelShaderHandle);
        } else {
            SetPixelShader(0);
        }    
    }
}

//NGC MATCH
struct NuVec* GetPlayerPosition(void) {
    if (player == NULL) {
        NuErrorProlog("C:/source/crashwoc/code/system/port.c",0xfc)("assert");
    }
    return &(player->obj).pos;
}

//90% NGC
void UnSetupShaders(enum shadertypes_e shader) {

    NuTexSetTexture(1,0);
    NuTexSetTexture(2,0);
    NuTexSetTexture(3,0);
    NudxFw_SetTextureState(1,D3DTSS_COLOROP,1);
    NudxFw_SetTextureState(2,D3DTSS_COLOROP,1);
    NudxFw_SetTextureState(3,D3DTSS_COLOROP,1);
    switch (shader) {     
    case 3:
    //case 0x10:
        NudxFw_SetRenderState(0x5D, 0);
        GS_SetBlendSrc(1, 1, 0);
        return;
    case 5:
        NudxFw_SetRenderState(0x6C, 0);
        NudxFw_SetRenderState(0x6D, 0);
        return;
    case 0x10:
    case 0x15:
    case 0x16:
    case 7:
        NudxFw_SetRenderState(0x5D, 0);
        return;
    case 33:
        NudxFw_SetRenderState(0x5D, 0);
        GS_SetBlendSrc(1, 1, 0);
        return;
    default:
        return;
    }
}


//96% NGC
void SetupShaders(struct nugeomitem_s* geomitem) {

    
    struct nurndritem_s* rndritem;
    struct numtx_s *cammtx;
    struct NuVec eyepos;
    struct NuVec lightpos;
    struct nucolour3_s lightcol;
    enum shadertypes_e shader;
    enum nuvtxtype_e vtxtype;
    struct numtl_s *mtl;
    s32 lights;
    s32 num_bumplights;
    s32 i;

    u32 uVar15; //check temp
    u32 uVar16; //check temp

    rndritem = &geomitem->hdr;
    lightcol = lbl_80123138;
    uVar16 = 0;
    uVar15 = 2;
    
    if (currentLevel == 2) {
        if (VEHICLECONTROL == 0) {
            defaultShader = 0;
        }
        else {
            defaultShader = WATERCAUSTICS;
        }
    }
    
    uVar16 = 0;
    IsGlassObj = 0;
    IsWaterObj = 0;
    DBTimerStart(0x24);
    xytype = 0;
    if (geomitem == NULL) {
        UnSetupShaders(currentshader);
        NuSetShaderState(~NO_SHADER, NUVT_TC1);
        return;
    }
    shader = (enum shadertypes_e)geomitem->hShader;
    mtl = geomitem->geom->mtl;
    vtxtype = geomitem->geom->vtxtype;
    if (mtl != NULL) {
        uVar15 = mtl->attrib.lighting;
    }
    if (uVar15 != 2) {
        NuLightSetStoredLights((s32)(geomitem->hdr).lights_index);
    }
    if ((shader != 0x80) && (vtxtype != Shaders[shader].vertshader)) {
        shader = NO_SHADER;
    }
    if ((shader != currentshader) || (vtxtype != currentvtxtype)) {
        UnSetupShaders(currentshader);
        NuSetShaderState(shader, vtxtype);
    }
 
    matView = *NuCameraGetViewMtx();
    matProj = *NuCameraGetProjectionMtx();
    if ((geomitem->mtx == NULL) || (shader == SNOWCLOUD)) {
        matWorld = identmtx;
    }
    else {
        matWorld = *geomitem->mtx;
    }

    NuMtxInv(&invWorld, &matWorld);
    NuMtxMulH(&matWorldView, &matWorld, &matView);
    NuMtxInv(&invWorldView, &matWorldView);
    NuMtxMulH(&matWorldViewProj, &matWorldView, &matProj);
    NuMtxMulH(&matViewProj, &matView, &matProj);
    
    GS_LightViewMat = *(struct _GSMATRIX*) &matView;
    GS_LightMat = *(struct _GSMATRIX*) &matWorldView;
        
    NuMtxTranspose(&matWorldViewProj, &matWorldViewProj);
    NuMtxTranspose(&matViewProj, &matViewProj);
    NuMtxTranspose(&matWorldView, &matWorldView);
    NuMtxTranspose(&matWorld, &matWorld);
    GS_SetLightingNone();
    currentshader = shader;
    // matWorldViewProj = matWorldViewProj;

    switch(shader) {
        case NO_SHADER:
        break;
        case GLASS: // 3
            IsGlassObj = 1;
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matWorldViewProj;
            CV_WORLDVIEW = *(struct _GSMATRIX*) &matWorldView;
            CV_WORLDVIEWIT = *(struct _GSMATRIX33*) &invWorldView;
            CV_CONSTANTS = *(struct _GS_VECTOR4 *)makenuvec4(0.0f, 1.0f, 0.0f, 0.0f);
            CV_SHADER_PARAMS = *(struct _GS_VECTOR4 *)makenuvec4(0.2f, 0.8f, refract, 0.5f);
            CV_SCREEN_PARAMS = *(struct _GS_VECTOR4 *)makenuvec4(PHYSICAL_SCREEN_X - 1, PHYSICAL_SCREEN_Y - 1, 0.0f, 0.0f);;
            NuShaderSetColourConstants(mtl);
            NuShaderSetLightConstants(mtl);
            GS_TexSetWrapModes(0, GX_CLAMP);
            GS_TexSetWrapModet(0, GX_CLAMP);
            GS_TexSetWrapModes(1, GX_CLAMP);
            GS_TexSetWrapModet(1, GX_CLAMP);
            
            NuTexSetTexture(0, GetGlassSpecularTexId());
            if (mtl != NULL) {
                NuTexSetTexture(1, (mtl->tid < 1) ? GetGlassSpecularTexId() : mtl->tid);
            }
            else {
                NuTexSetTexture(1, GetGlassSpecularTexId());
            }
            GS_SetBlendSrc(1,4,5);
            GS_SetTevBlend(1);
        break;
        case WATER: // 1
            IsWaterObj = WATER;
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matWorldViewProj;
            CV_SHADER_PARAMS = *(struct _GS_VECTOR4 *)makenuvec4((mtl->fx2.f32 / mtl->fx4.f32) * 0.02083333f, mtl->fx3.f32 / mtl->fx4.f32, mtl->fx4.f32, 0.0f);
            CV_SCREEN_PARAMS = *(struct _GS_VECTOR4 *)makenuvec4(SWIDTH,SHEIGHT, 0.0f, 0.0f);
            NuWaterSetup();
            GS_SetBlendSrc(1, 4, 5);
            NuTexSetTexture(1, CausticTextures[CurrentCausticTexture]);
            GS_TexSetWrapModes(1, GX_REPEAT);
            GS_TexSetWrapModet(1, GX_REPEAT);
            GS_SetTevModulate(GX_TEVSTAGE1);
        break;
        case BRDFGOLD: // 10
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*)&matWorldViewProj;
            cammtx = NuCameraGetMtx();
            memcpy(&eyepos, &cammtx->_30, sizeof(struct NuVec));
            NuVecMtxTransform(&eyepos, &eyepos, &invWorld);
            CV_EYE_POSITION = *(struct _GS_VECTOR4 *)makenuvec4(eyepos.x, eyepos.y, eyepos.z, 0.0f);
            lightpos = *GetPlayerPosition();
            CV_CONSTANTS = *(struct _GS_VECTOR4*)makenuvec4(0.0f, 1.0f, 0.5f, 0.0f);
            lightpos.y += 0.5f;
            NuVecMtxTransform(&lightpos, &lightpos, &invWorld);
            CV_LIGHT_POSITION = *(struct _GS_VECTOR4 *)makenuvec4(lightpos.x, lightpos.y, lightpos.z, 0.0f);
            NudxFw_SetTextureState(1, D3DTSS_TEXCOORDINDEX, 1);
            NudxFw_SetTextureState(2, D3DTSS_TEXCOORDINDEX, 2);
            GS_TexSetWrapModes(1, GX_CLAMP);
            GS_TexSetWrapModet(1, GX_CLAMP);
            GS_TexSetWrapModes(2, GX_CLAMP);
            GS_TexSetWrapModet(2, GX_CLAMP);
            NudxFw_SetTextureState(1, D3DTSS_MAGFILTER, 2);
            NudxFw_SetTextureState(1, D3DTSS_MINFILTER, 2);
            NudxFw_SetTextureState(1, D3DTSS_MIPFILTER, 2);
            NudxFw_SetTextureState(2, D3DTSS_MAGFILTER, 2);
            NudxFw_SetTextureState(2, D3DTSS_MINFILTER, 2);
            NudxFw_SetTextureState(2, D3DTSS_MIPFILTER, 2);
        break;
        case LIGHTHAZE: // 8
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matWorldViewProj;
            CV_SCREEN_PARAMS = *(struct _GS_VECTOR4 *)makenuvec4(PHYSICAL_SCREEN_X - 1,PHYSICAL_SCREEN_Y - 1, 0.0f, 0.0f);
            CV_CONSTANTS = *(struct _GS_VECTOR4*)makenuvec4(0.0f, 0.0f, 0.0f, 0.0f);   
            NuTexSetTexture(1, haze_mtl->tid);
            GS_TexSetWrapModes(1, GX_CLAMP);
            GS_TexSetWrapModet(1, GX_CLAMP);
            NudxFw_SetTextureState(1, D3DTSS_TEXCOORDINDEX, 1);
        break;
        case DEPTHBLEND: // 6
            NuTexSetTexture(1, depthMtl->tid);
            GS_TexSetWrapModes(1, GX_CLAMP);
            GS_TexSetWrapModet(1, GX_CLAMP);
            NudxFw_SetTextureState(1, D3DTSS_TEXCOORDINDEX, 0);
        break;
        case SNOWCLOUD: // 5
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matWorldViewProj;
            memcpy(&CV_LIGHT_POSITION, &geomitem->mtx->_30, sizeof(struct _GS_VECTOR4));
            memcpy(&CV_EYE_POSITION, &(NuCameraGetMtx())->_30, sizeof(struct _GS_VECTOR4));
            CV_SHADER_PARAMS = *(struct _GS_VECTOR4 *)makenuvec4((mtl)->fx1.f32, (mtl)->fx1.f32, (mtl)->fx1.f32, 0.0f);
            CV_CONSTANTS = *(struct _GS_VECTOR4 *)makenuvec4(mtl->fx2.f32, 0.0f, 1.0f, 0.0f);
            NudxFw_SetRenderState(D3DRS_POINTSPRITEENABLE, 1);
            NudxFw_SetRenderState(D3DRS_POINTSCALEENABLE, 1);
            NuTexSetTexture(3, mtl->tid);
            NudxFw_SetTextureState(3, D3DTSS_COLORARG1, 2);
            NudxFw_SetTextureState(3, D3DTSS_COLOROP, 2);
            NuMtxSetIdentity(geomitem->mtx);
        break;
        case WATERCAUSTICS: // 4
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matWorldViewProj;
            CV_WORLDVIEW = *(struct _GSMATRIX*) &matWorld;
            CV_CONSTANTS = *(struct _GS_VECTOR4*)makenuvec4(0.0f, 1.0f, 0.3, 0.0f);
            CV_LIGHT_DIRECTION = *(struct _GS_VECTOR4*)makenuvec4(0.0f, 1.0f, 0.0f, 0.0f);
            NuShaderSetColourConstants(mtl);
            NuShaderSetLightConstants(mtl);
            NuTexSetTexture(1,CausticTextures[CurrentCausticTexture]);
            NudxFw_SetRenderState(D3DRS_SPECULARENABLE, 1);
            NudxFw_SetTextureState(1, D3DTSS_MINFILTER, 2);
            NudxFw_SetTextureState(1, D3DTSS_MAGFILTER, 2);
            NudxFw_SetTextureState(1, D3DTSS_MIPFILTER, 2);
            GS_TexSetWrapModes(1, GX_REPEAT);
            GS_TexSetWrapModet(1, GX_REPEAT);
            GS_SetTevModulate(GX_TEVSTAGE1);
        break;
        case HEATHAZE: // 2
            CV_SHADER_PARAMS = *(struct _GS_VECTOR4 *)makenuvec4(mtl->fx1.f32, 0.0f, 0.0f, 0.0f);
            CV_SCREENSPACE_OFFSET = *(struct _GS_VECTOR4 *)makenuvec4(0.53125f, 0.53125f, 0.53125f, 0.0f);
            NudxFw_SetTextureState(0, D3DTSS_COLORARG1, 2);
            NudxFw_SetTextureState(0, D3DTSS_COLOROP, 2);
            NuHazeSetHazeConstants();
            GS_SetTevModulate(GX_TEVSTAGE1);
        break;
        case SPECULAR: // 7
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matWorldViewProj;
            CV_WORLDVIEW = *(struct _GSMATRIX*) &matWorld;
            CV_CONSTANTS = *(struct _GS_VECTOR4*)makenuvec4(0.0f, 1.0f, 0.0f, 0.0f);
            CV_SHADER_PARAMS = *(struct _GS_VECTOR4 *)makenuvec4(0.2f, 0.8f, 0.03f, 0.5f);
            NuShaderSetColourConstants(mtl);
            NuShaderSetLightConstants(mtl);
            /* Fallthrough */

        case LIGHTS: // 11
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matWorldViewProj;
            CV_WORLDVIEW = *(struct _GSMATRIX*) &matWorld;
            CV_CONSTANTS = *(struct _GS_VECTOR4*)makenuvec4(0.0f, 1.0f, 0.0f, 0.0f);
            NuShaderSetColourConstants(mtl);
            NuShaderSetLightConstants(mtl);
        break;
        case BLENDSKINFUR: // 19
        case BLENDSKINFUR2: // 20
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matViewProj;
            CV_WORLDVIEW = *(struct _GSMATRIX*) &matWorld;
            CV_CONSTANTS = *(struct _GS_VECTOR4*)makenuvec4(0.0f, 1.0f, 3.0f, 3.0f);
            NuShaderSetColourConstants(mtl);
            NuShaderSetLightConstants(mtl);
        break;
        case BLENDSKINGLASS: // 17
        case BLENDSKINGLASS2: // 18
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matViewProj;
            CV_WORLDVIEW = *(struct _GSMATRIX*) &matWorld;
            memcpy(&CV_WORLDVIEWIT, &invWorldView, sizeof(struct _GSMATRIX));
            CV_CONSTANTS = *(struct _GS_VECTOR4*) makenuvec4(0.0f, 1.0f, 3.0f, 3.0f);
            CV_SHADER_PARAMS = *(struct _GS_VECTOR4*)makenuvec4(0.2f, 0.8f, refract, 0.5f);
            CV_SCREEN_PARAMS = *(struct _GS_VECTOR4*)makenuvec4(PHYSICAL_SCREEN_X - 1, PHYSICAL_SCREEN_Y - 1, 0.0f, 0.0f);
            NuShaderSetColourConstants(mtl);
            NuShaderSetLightConstants(mtl);
            GS_TexSetWrapModes(0, GX_CLAMP);
            GS_TexSetWrapModet(0, GX_CLAMP);
            GS_TexSetWrapModes(1, GX_CLAMP);
            GS_TexSetWrapModet(1, GX_CLAMP);
            NuTexSetTexture(0, 0);
            GS_SetBlendSrc(1, 2, 4);
        break;
        case BUMPMAP: // 21 
            if (geomitem->instancelights_index[0] == -1) {
                ResetShaders();
                return;
            }
            
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matWorldViewProj;
            NuShaderSetColourConstants(mtl);
            NudxFw_SetTextureState(1, D3DTSS_TEXCOORDINDEX, 0);
            NudxFw_SetTextureState(1, D3DTSS_MINFILTER, 2);
            NudxFw_SetTextureState(1, D3DTSS_MAGFILTER, 2);
            NudxFw_SetTextureState(1, D3DTSS_MIPFILTER, 2);
            GS_TexSetWrapModes(1, GX_REPEAT);
            GS_TexSetWrapModet(1, GX_REPEAT);
            NuTexSetTexture(1, (mtl->next)->tid);
            CV_CONSTANTS = *(struct _GS_VECTOR4*) makenuvec4(0.0f, 1.0f, 2.0, 0.0f);
            NudxFw_SetRenderState(0x5d, 1);
            lightcol = mtl->next->diffuse;
            lights = 0;
            
            for (i = 0; i < 3; i++) {
                if (geomitem->instancelights_index[i] != -1) {
                    lights++;
                }
            }
            
            if (lights == 0) {
                lightpos.x = 10.0;
                lightpos.y = 10.0;
                lightpos.z = 10.0;
                
                matView = *NuCameraGetViewMtx();
                NuVecMtxTransform(&lightpos, (struct NuVec *)&matView._30, &invWorld);
                CV_LIGHT_POSITION = *(struct _GS_VECTOR4*)makenuvec4(lightpos.x, lightpos.y, lightpos.z, 0.0f);
            }
            else {
                for (i = 0; i < lights; i++) {
                    lightpos = *(struct NuVec *) GetLightPosition((s32)geomitem->instancelights_index[i]);
                    NuVecMtxTransform(&lightpos, &lightpos, &invWorld);
                    CV_LIGHT_POSITION = *(struct _GS_VECTOR4*)makenuvec4(lightpos.x, lightpos.y, lightpos.z, 0.0f);
                    
                    if (i != lights) {
                        NuRndrItem(rndritem);
                    }
                    
                    if (i == 0) {
                        GS_SetBlendSrc(1, 4, 5);
                    }
                }
            }
        break;
        case BUMPMAPPOINTLIGHT: // 22
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matWorldViewProj;
            NuShaderSetColourConstants(mtl);
            NudxFw_SetTextureState(1, D3DTSS_TEXCOORDINDEX, 0);
            NudxFw_SetTextureState(1, D3DTSS_MINFILTER, 2);
            NudxFw_SetTextureState(1, D3DTSS_MAGFILTER, 2);
            NudxFw_SetTextureState(1, D3DTSS_MIPFILTER, 2);
            GS_TexSetWrapModes(1, GX_REPEAT);
            GS_TexSetWrapModet(1, GX_REPEAT);
            NuTexSetTexture(1, (mtl->next)->tid);
            CV_CONSTANTS = *(struct _GS_VECTOR4*)makenuvec4(0.0f, 1.0f, 2.0, 0.0f);
            CV_SHADER_PARAMS = *(struct _GS_VECTOR4 *)makenuvec4(3.0f, 2.5, 0.0f, 3.0f);
            NudxFw_SetRenderState(D3DRS_SPECULARENABLE, 1);
            lightcol = mtl->next->diffuse;
            currentLevel = 0x1b;
            lightpos = *(struct NuVec *)GetBugPosition();
            GS_SetPointLighting(&invWorld, &lightpos, &lightcol);
            NuVecMtxTransform(&lightpos, &lightpos, &invWorld);
            CV_LIGHT_POSITION = *(struct _GS_VECTOR4 *)makenuvec4(lightpos.x, lightpos.y, lightpos.z, 0.0f);
        break;

        case BLENDSKIN: // 15
        case BLENDSKIN2: // 16
        break;
    
        case POINTLIGHT: // 24
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matWorldViewProj;
            NuShaderSetColourConstants(mtl);
            CV_CONSTANTS = *(struct _GS_VECTOR4*)makenuvec4(0.0f, 1.0f, 2.0, 0.0f);
            CV_SHADER_PARAMS = *(struct _GS_VECTOR4 *)makenuvec4(3.0f, 2.5, 0.0f, 3.0f);
            currentLevel = 0x1b;
            lightpos = *(struct NuVec *)GetBugPosition();
            NuVecMtxTransform(&lightpos, &lightpos, &invWorld);
            GS_SetPointLighting(&invWorld, &lightpos, &lightcol);
            CV_LIGHT_POSITION = *(struct _GS_VECTOR4 *)makenuvec4(lightpos.x, lightpos.y, lightpos.z, 0.0f);
            CV_AMBIENT_COLOR = *(struct _GS_VECTOR4 *)makenuvec4(0.2f, 0.2f, 0.2f, 0.2f);
        break;
        case BLENDSKINPOINTLIGHT: // 25
        case BLENDSKIN2POINTLIGHT: // 26
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matViewProj;
            CV_WORLDVIEW = *(struct _GSMATRIX*) &matWorld;
            CV_CONSTANTS = *(struct _GS_VECTOR4*)makenuvec4(0.0f, 1.0f, 3.0f, 3.0f);
            NuShaderSetColourConstants(mtl);
            NuShaderSetLightConstants(mtl);
            CV_SHADER_PARAMS = *(struct _GS_VECTOR4 *)makenuvec4(3.0f, 2.5, 2.0, 3.0f);
            currentLevel = 0x1b;
            lightpos = *(struct NuVec *)GetBugPosition();
        // break;
    
            CV_LIGHT_POSITION = *(struct _GS_VECTOR4 *)makenuvec4(lightpos.x, lightpos.y, lightpos.z, 0.0f);
            CV_AMBIENT_COLOR = *(struct _GS_VECTOR4 *)makenuvec4(0.2f, 0.2f, 0.2f, 0.2f);
        break;
        
        case DYNAMICWATER: // 27
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matWorldViewProj;
            CV_WORLD_0 = *(struct _GSMATRIX*) &matWorld;
            CV_SHADER_PARAMS = *(struct _GS_VECTOR4 *)makenuvec4((mtl->fx2.f32 / mtl->fx4.f32) * 0.02083333f,
                                         mtl->fx3.f32 / mtl->fx4.f32, mtl->fx4.f32, 0.0f);
            CV_CONSTANTS = *(struct _GS_VECTOR4*)makenuvec4(0.0f, 1.0f, 2.0f, dynamicWaterScale);
            CV_MTL_COLOR = *(struct _GS_VECTOR4*)makenuvec4(mtl->diffuse.r, mtl->diffuse.g, mtl->diffuse.b, dynamicWaterBlend);
            cammtx = NuCameraGetMtx();
            memcpy(&eyepos, &cammtx->_30, sizeof(struct NuVec));
            NuVecMtxTransform(&eyepos, &eyepos, &invWorld);
            CV_EYE_POSITION = *(struct _GS_VECTOR4*)makenuvec4(eyepos.x, eyepos.y, eyepos.z, 0.0f);
            NuWaterSetup();
            NuTexSetTexture(0, dynamicWaterGetNormalMap());
            NuTexSetTexture(1, 0);
            NuTexSetTexture(2, 0);
            GS_SetBlendSrc(1, 4, 1);
            NudxFw_SetTextureState(0, D3DTSS_MINFILTER, 2);
            NudxFw_SetTextureState(0, D3DTSS_MAGFILTER, 2);
            NudxFw_SetTextureState(0, D3DTSS_MIPFILTER, 0);
            GS_TexSetWrapModes(0, GX_REPEAT);
            GS_TexSetWrapModet(0, GX_REPEAT);
            NudxFw_SetTextureState(3, D3DTSS_MINFILTER, 2);
            NudxFw_SetTextureState(3, D3DTSS_MAGFILTER, 2);
            NudxFw_SetTextureState(3, D3DTSS_MIPFILTER, 2);
        break;
        case XRAYGLASS: // 33
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matWorldViewProj;
            CV_CONSTANTS = *(struct _GS_VECTOR4*)makenuvec4(0.0f, 0.5f, 0.8f, 1.0f);
            GS_TexSetWrapModes(0,GX_CLAMP);
            GS_TexSetWrapModet(0,GX_CLAMP);
            GS_TexSetWrapModes(1,GX_CLAMP);
            GS_TexSetWrapModet(1,GX_CLAMP);
            
            if (mtl != NULL && mtl->tid > 0) {
                NuTexSetTexture(0, mtl->tid);
            }
            else {
                NuTexSetTexture(0, 0);
            }
            
            NudxFw_SetRenderState(D3DRS_SPECULARENABLE, 0);
            GS_SetAlphaCompare(7, 0);
            GS_SetBlendSrc2(1, 4, 1);
        break;
        case 0x80:
            CV_WORLDVIEWPROJ = *(struct _GSMATRIX*) &matWorldViewProj;
            CV_WORLDVIEW = *(struct _GSMATRIX*) &matWorld;
            CV_CONSTANTS = *(struct _GS_VECTOR4*)makenuvec4(0.0f, 1.0f, 0.0f, 0.0f);
            CV_SHADER_PARAMS = *(struct _GS_VECTOR4 *)makenuvec4(0.2f, 0.8f, 0.03f, 0.5f);
            NuShaderSetColourConstants(mtl);
            NuShaderSetLightConstants(mtl);
            NuTexSetTexture(1, GetGlassSpecularTexId());
            NudxFw_SetTextureState(1, D3DTSS_TEXCOORDINDEX, 1);
            NudxFw_SetTextureState(1, D3DTSS_COLORARG1, 2);
            NudxFw_SetTextureState(1, D3DTSS_COLORARG2, 1);
            NudxFw_SetTextureState(1, D3DTSS_COLOROP, 7);
            GS_SetBlendSrc(1, 4, 1);
            GS_SetTevBlend(1);
        break;
    }
    
    DBTimerEnd(0x24);
    return;   
}

//92% NGC
short NuShaderAssignShader(struct nugeom_s* geom) {
    enum shadertypes_e shader = NO_SHADER;
    s32 skinned; // ?
    s32 blended = 0; // r8
    struct numtl_s* mtl;
    u32 fxid;

    skinned = 0;

    if (nurndr_forced_mtl) {
        mtl = nurndr_forced_mtl;
    } else if (nurndr_forced_mtl_table && geom->mtl->special_id) {
        mtl = nurndr_forced_mtl_table[geom->mtl->special_id];
    } else {
        mtl = geom->mtl;
    }
    fxid = mtl->fxid;

    if (geom->vtxskininfo != 0) {
        skinned = 1;
    }
    if (geom->blendgeom && geom->blendgeom->hVB) {
        blended = 1;
    }

    if (skinned) {
        if (blended == 0)
            goto off248;
        switch (fxid) {
            case 1:
                shader = GLASS;
                break;
            case 3:
                shader = GLASS;
                break;
            case 2:
                shader = SPECULAR;
                break;
            case 4:
                shader = WATER;
                break;
            case 0x5c:
                shader = DYNAMICWATER;
                break;
            case 0x5d:
                shader = BRDFGOLD;
                break;
            case 0x5f:
                shader = LIGHTHAZE;
                break;
            case 0x60:
                shader = DEPTHBLEND;
                break;
            case 0x61:
                shader = SNOWCLOUD;
                break;
            case 0x62:
                shader = HEATHAZE;
                break;
            case 0x63:
                shader = HEATHAZE;
                break;
            case 0x87:
                if (currentLevel == 0x1b)
                    shader = BUMPMAPPOINTLIGHT;
                break;
            case 0x64:
                shader = XRAYGLASS;
                break;
        }
        if (shader == NO_SHADER) {
            shader = LIGHTS;
            if (geom->mtl->attrib.lighting == 2) {
                shader = defaultShader;
            }
            goto off248;
        }
        if (blended != 0) {
            switch (fxid) {
                case 1:
                    shader = 0x11;
                    break;
                case 3:
                    shader = 0x11;
                    break;
                case 0x88:
                    shader = 0x10;
                    break;
                default:
                    shader = 0xf;
                    break;
            }
        } else {
            switch (fxid) {
                case 1:
                    shader = 0x12;
                    break;
                case 3:
                    shader = 0x12;
                    break;
                case 0x88:
                    shader = 0x10;
                    break;
                default:
                    shader = 0x10;
                    break;
            }
        }
    off248:
        if ((defaultShader == 0) || (defaultShader == 0x18)) {
            switch (shader) {
                case BLENDSKIN:
                    shader = 0x19;
                    break;
                case BLENDSKIN2:
                    shader = 0x1a;
                    break;
            }
        }
    }
    if ((bypassEffectShaders != 0)    
        && (((
            ((((shader == 0xB) || (shader == 0x15)) || (shader == 0x16))
             || ((shader == 0x18 || (shader == 4))))
            || (((shader == 0x19)))
        )) || (shader == 0x1a)))
    {
        shader = NO_SHADER;
    } else {
        nobypass++;
    }
    if (bypassEffectShaders == 0) {
        nobypass++;
    }
    if ((ForceShader != RESET_SHADER) && (shader == SPECULAR)) {
        shader = ForceShader;
    }
    return shader;
}



/* //ASM GCN
void LinkDmaParticalSets(int **table,int count) {
    int **ppiVar1;
    int **lp;
    int *ptr;
    
    *table[count + -1] = 1;
    if (count + -2 < 0) {
        return;
    }
    lp = table + count + -2;
    do {
        ptr = *lp;
        *ptr = 0;
        ppiVar1 = lp + 1;
        lp = lp + -1;
        ptr[1] = (int)*ppiVar1;
    } while ((int)table <= (int)lp);
    return;
}
*/
