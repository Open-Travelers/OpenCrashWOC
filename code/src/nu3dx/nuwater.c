#include "nuwater.h"
#include "../system.h"

struct numtl_s* DebMat[8]; //debris.c

//NGC MATCH
static s32 Powr2(s32 v) {
    s32 p2;

    p2 = 1;
    while (p2 < v) {
        p2 = p2 << 1;
    }
    return p2;
}


//NGC MATCH
static void fixMaterials(void) {
    s32 nummtls;
    struct numtl_s *mtl;
    s32 i;

    nummtls = NuMtlNum();
        for (i = 0; i < nummtls; i++) {
            mtl = NuMtlGet(i);
            if ((mtl != NULL) && (mtl->fxid == '\x04')) {
                mtl->tid = water_copy_mtl->tid;
                //mtl->attrib & 0x3c30ffff | 0x1420000;
                mtl->attrib.alpha = 0;
                mtl->attrib.utc = 1;
                mtl->attrib.vtc = 1;
                mtl->attrib.lighting = 2;
                mtl->alpha_sort = 100;
                mtl->attrib.zmode = 0;
                NuMtlUpdate(mtl);
            }
        }
    return;
}


//NGC MATCH
s32 dynamicWaterGetNormalMap(void) {
    return dynamicWaterTextureIds[dynamicWaterForceStepOneTex];
}


//NGC MATCH
static void NuDynamicWaterClose(void) {
    s32 i;

    if (dynamicWaterInitialised != 0) {
        dynamicWaterInitialised = 0;
        for (i = 0; i < 6; i++) {
            NuTexDestroy(dynamicWaterTextureIds[i]);
        }
    }
    return;
}


//NGC MATCH
static void NuDynamicWaterCalcShaderUVOffsets(s32 width, s32 height, float multiplier) {
    float hoff;
    float woff;

    woff = (1.0f / (float) width) * multiplier;
    hoff = (1.0f / (float) height)  * multiplier;

    dynamicWaterUVOffsets[0][0].x = woff * 0.5f;
    dynamicWaterUVOffsets[0][0].y = hoff * 0.5f;
    dynamicWaterUVOffsets[0][0].z = dynamicWaterUVOffsets[0][0].x;
    dynamicWaterUVOffsets[0][0].w = dynamicWaterUVOffsets[0][0].y;

    dynamicWaterUVOffsets[0][1].x = dynamicWaterUVOffsets[0][0].x;
    dynamicWaterUVOffsets[0][1].y = dynamicWaterUVOffsets[0][0].y;
    dynamicWaterUVOffsets[0][1].z = dynamicWaterUVOffsets[0][0].x;
    dynamicWaterUVOffsets[0][1].w = dynamicWaterUVOffsets[0][0].y;

    dynamicWaterUVOffsets[1][2].x = dynamicWaterUVOffsets[0][0].x - dynamicWaterUVOffsets[0][0].x * dynamicWaterBlurDist;
    dynamicWaterUVOffsets[1][2].y = dynamicWaterUVOffsets[0][0].y - hoff * dynamicWaterBlurDist;
    dynamicWaterUVOffsets[1][2].z = dynamicWaterUVOffsets[0][0].x + woff * dynamicWaterBlurDist;
    dynamicWaterUVOffsets[1][2].w = dynamicWaterUVOffsets[0][0].y - dynamicWaterUVOffsets[0][0].y * dynamicWaterBlurDist;

    dynamicWaterUVOffsets[0][2].x = dynamicWaterUVOffsets[0][0].x;
    dynamicWaterUVOffsets[0][2].y = dynamicWaterUVOffsets[0][0].y;
    dynamicWaterUVOffsets[0][2].z = 0.0f - woff;
    dynamicWaterUVOffsets[0][2].w = 0.0f - hoff;

    dynamicWaterUVOffsets[0][3].x = dynamicWaterUVOffsets[0][0].x + dynamicWaterUVOffsets[0][0].x + woff;
    dynamicWaterUVOffsets[0][3].y = 0.0f - hoff;
    dynamicWaterUVOffsets[0][3].z = dynamicWaterUVOffsets[0][0].x + dynamicWaterUVOffsets[0][0].x + woff;
    dynamicWaterUVOffsets[0][3].w = dynamicWaterUVOffsets[0][0].y + dynamicWaterUVOffsets[0][0].y + hoff;

    dynamicWaterUVOffsets[0][4].x = dynamicWaterUVOffsets[0][0].x;
    dynamicWaterUVOffsets[0][4].y = dynamicWaterUVOffsets[0][0].y;
    dynamicWaterUVOffsets[0][4].w = dynamicWaterUVOffsets[0][0].y + dynamicWaterUVOffsets[0][0].y + hoff;
    dynamicWaterUVOffsets[0][4].z = 0.0f - woff;

    dynamicWaterUVOffsets[0][5].x = dynamicWaterUVOffsets[0][0].x;
    dynamicWaterUVOffsets[0][5].y = dynamicWaterUVOffsets[0][0].y;
    dynamicWaterUVOffsets[0][5].z = dynamicWaterUVOffsets[0][0].x;
    dynamicWaterUVOffsets[0][5].w = dynamicWaterUVOffsets[0][0].y;

    dynamicWaterUVOffsets[1][3].x = dynamicWaterUVOffsets[0][0].x + dynamicWaterUVOffsets[0][0].x * dynamicWaterBlurDist;
    dynamicWaterUVOffsets[1][3].y = dynamicWaterUVOffsets[0][0].y + hoff * dynamicWaterBlurDist;
    dynamicWaterUVOffsets[1][3].z = dynamicWaterUVOffsets[0][0].x - woff * dynamicWaterBlurDist;
    dynamicWaterUVOffsets[1][3].w = dynamicWaterUVOffsets[0][0].y + dynamicWaterUVOffsets[0][0].y * dynamicWaterBlurDist;

    dynamicWaterUVOffsets[1][4].x = dynamicWaterUVOffsets[0][0].x - woff;
    dynamicWaterUVOffsets[1][4].z = dynamicWaterUVOffsets[0][0].x + woff;
    dynamicWaterUVOffsets[1][4].y = dynamicWaterUVOffsets[0][0].y;
    dynamicWaterUVOffsets[1][4].w = dynamicWaterUVOffsets[0][0].y;

    dynamicWaterUVOffsets[1][5].x = dynamicWaterUVOffsets[0][0].x;
    dynamicWaterUVOffsets[1][5].y = dynamicWaterUVOffsets[0][0].y + hoff;
    dynamicWaterUVOffsets[1][5].z = dynamicWaterUVOffsets[0][0].x;
    dynamicWaterUVOffsets[1][5].w = dynamicWaterUVOffsets[0][0].y - hoff;

    return;
}

//96%
static void NuDynamicWaterInit() {
    s32 i;
    struct nutex_s tex;

    if (dynamicWaterEnabled != 0) {
        if (dynamicWaterInitialised != 0) {
            NuDynamicWaterClose();
        }
        tex.type = NUTEX_RT32;
        tex.width = 0x80;
        tex.height = 0x80;
        tex.mmcnt = 0;
        for (i = 0; i < 6; i++) {
            tex.type = NUTEX_RT32;
            tex.width = 0x80;
            tex.height = 0x80;
            dynamicWaterTextureIds[i] = NuTexCreate(&tex);
        }
        dynamicWaterHeightTargetTex = 4;
        dynamicWaterForceTex = 5;
        dynamicWaterForceStepOneTex = 3;
        dynamicWaterNormalTex = 3;
        dynamicWaterHeightSourceTex = 0;
        dynamicWaterVelocitySourceTex = 1;
        dynamicWaterVelocityTargetTex = 2;
        dynamicWaterBlurTex = 0;
        dynamicWaterFlipState = 0;
        pDropletTex = NuTexLoadTextureFromDDSFile("stuff\\Droplet1.dds");
        DropletTex.type = NUTEX_RGBA32;
        DropletTex.height = 0x20;
        DropletTex.width = 0x20;
        DropletTex.linear = 0;
        DropletTex.pal = 0;
        DropletTex.mmcnt = 1;
        DropletTID = NuTexCreateFromSurface(&DropletTex,pDropletTex);
        NudxFw_SetRenderTargetSurface(dynamicWaterSurfaces[dynamicWaterHeightSourceTex],NULL);
        NuRndrClear(0xb,-0x7f7f80,1.0f);
        NudxFw_SetRenderTargetSurface(dynamicWaterSurfaces[dynamicWaterVelocitySourceTex],NULL);
        NuRndrClear(0xb,-0x7f7f80,1.0f);
        dynamicWaterMaterial = NuMtlCreate(1);
        dynamicWaterMaterial->alpha = 0.5f;
        dynamicWaterMaterial->attrib.zmode = 1;
        dynamicWaterMaterial->attrib.utc = 0;
        dynamicWaterMaterial->attrib.vtc = 0;
        dynamicWaterMaterial->attrib.filter = 1;
        dynamicWaterMaterial->attrib.lighting = 2;
        dynamicWaterMaterial->attrib.colour = 0;
        dynamicWaterMaterial->attrib.alpha = 1;
        dynamicWaterMaterial->tid = dynamicWaterTextureIds[dynamicWaterNormalTex];
        dynamicWaterMaterial->alpha_sort = 0xff;
        dynamicWaterMaterial->fxid = 0x5c;
        NuMtlUpdate(dynamicWaterMaterial);
        NuDynamicWaterCalcShaderUVOffsets(0x80,0x80,1.0f);
        dynamicWaterInitialised = 1;
        if ((((Level == 7) || (Level == 0x17)) || (Level == 8)) || (Level == 0xc)) {
            dynamicWaterDropFrequency = 0.4f;
        }
        else if (((Level == 0xe) || (Level == 0)) || ((Level == 0x25 || (Level == 0x1d)))) {
            dynamicWaterDropFrequency = 0.5f;
        }
        else {
            dynamicWaterDropFrequency = 0.0f;
        }
        if (((Level == 0xc) || (Level == 2)) || (Level == 1)) {
            dynamicWaterBlend = 0.11f;
        }
        else if (Level == 0x12) {
            dynamicWaterBlend = 0.3f;
        }
        else if (Level == 0x1d) {
            dynamicWaterBlend = 0.52f;
        }
        else if ((Level == 0x13) || (Level == 0x17)) {
            dynamicWaterBlend = 0.13f;
        }
        else if ((Level == 5) || (Level == 3)) {
            dynamicWaterBlend = 0.25f;
        }
        else {
            dynamicWaterBlend = 0.1f;
        }
        if ((Level == 8) || (Level == 0x25)) {
            dynamicWaterScale = 0.25f;
        }
        else if (((Level == 2) || (((Level == 0xe || (Level == 0)) || (Level == 0x1c)))) ||
                (Level == 5)) {
            dynamicWaterScale = 0.2f;
        }
        else if ((Level == 0x12) || (Level == 3)) {
            dynamicWaterScale = 0.06f;
        }
        else if (Level == 0x1d) {
            dynamicWaterScale = 0.2f;
        }
        else if (((Level == 1) || (Level == 0x13)) || (Level == 0x10)) {
            dynamicWaterScale = 0.15f;
        }
        else {
            dynamicWaterScale = 0.3f;
        }
        for (i = 0; i < 0x104; i++) {
            NuDynamicWaterUpdate(1);
        }
    }
    return;
}

//NGC MATCH
static void NuDynamicWaterRender2dRect(s32 width,s32 height) {
    struct nuvtx_tltc1_s vtx [6];
    s32 diffuse = -1;

    vtx[0].pnt.x = 0.0;
    vtx[0].pnt.y = 0.0;
    vtx[0].pnt.z = 0.0;
    vtx[0].rhw = 1.0;
    vtx[0].diffuse = diffuse;
    vtx[0].tc[0] = 0.0;
    vtx[0].tc[1] = 0.0;
    vtx[1].pnt.x = (float)width;
    vtx[1].pnt.y = 0.0;
    vtx[1].pnt.z = 0.0;
    vtx[1].rhw = 1.0;
    vtx[1].diffuse = diffuse;
    vtx[1].tc[0] = 1.0;
    vtx[1].tc[1] = 0.0;
    vtx[2].pnt.x = 0.0;
    vtx[2].pnt.y = (float)height;
    vtx[2].pnt.z = 0.0;
    vtx[2].rhw = 1.0;
    vtx[2].diffuse = diffuse;
    vtx[2].tc[0] = 0.0;
    vtx[2].tc[1] = 1.0;
    vtx[3].pnt.x = vtx[1].pnt.x;
    vtx[3].pnt.y = vtx[2].pnt.y;
    vtx[3].pnt.z = 0.0;
    vtx[3].diffuse = diffuse;
    vtx[3].tc[1] = 1.0;
    vtx[3].rhw = 1.0;
    vtx[3].tc[0] = 1.0f;
    vtx[3].tc[1] = 1.0f;
    //GS_DrawTriStripTTL((struct _GS_VERTEXTL *)vtx,4);
    return;
}


//NGC MATCH
static void NuDynamicWaterSetVertexShaderUVOffsets(s32 type) {
    return;
}

//NGC MATCH
static void NuDynamicWaterCycleTextures(void) {
    s32 copy;

    switch (dynamicWaterFlipState) {
    case 0:
        copy = dynamicWaterHeightTargetTex;
        dynamicWaterVelocitySourceTex = 2;
        dynamicWaterHeightTargetTex = dynamicWaterHeightSourceTex;
        dynamicWaterVelocityTargetTex = 1;
        dynamicWaterHeightSourceTex = copy;
        dynamicWaterBlurTex = copy;
        break;
    case 1:
        copy = dynamicWaterHeightTargetTex;
        dynamicWaterHeightTargetTex = dynamicWaterHeightSourceTex;
        dynamicWaterVelocitySourceTex = 1;
        dynamicWaterVelocityTargetTex = 2;
        dynamicWaterHeightSourceTex = copy;
        dynamicWaterBlurTex = copy;
        break;
    }
    dynamicWaterFlipState = !dynamicWaterFlipState;
}

//97.40%
static void NuDynamicWaterExcite() {
    char pad[54];
    float random;
    float x;
    float y;
    float scale;
    float copy;
    float temp;
    float i;
    int col;

    copy = randyfloat();
    if ((dynamicWaterDropFrequency > copy) || (dynamicWaterTurbulenceFrequency != 0)) {
        NudxFw_SetRenderTargetSurface(dynamicWaterSurfaces[dynamicWaterVelocityTargetTex], NULL);
        if (dynamicWaterDropFrequency > copy) {
            x = randyfloat() * 128.0f;
            y = randyfloat() * 128.0f;
            if (dynamicWaterDropMinScale > randyfloat() * dynamicWaterDropMaxScale) {
                scale = dynamicWaterDropMinScale;
            } else {
                scale = randyfloat() * dynamicWaterDropMaxScale;
            }
            temp = DebMat[0]->tid;
            DebMat[0]->tid = DropletTID;
            NuRndrRectUV2di((int)x, (int)y, scale * 32.0f, scale * 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, -1, DebMat[0]);
            DebMat[0]->tid = temp;
        }
        if (dynamicWaterTurbulenceFrequency != 0) {
            col = (dynamicWaterTurbulenceStrength << 0x18) + (dynamicWaterTurbulenceStrength << 0x10)
                + (dynamicWaterTurbulenceStrength << 0x8) + dynamicWaterTurbulenceStrength;
            scale = dynamicWaterTurbulenceScale * 32.0f;
            for (i = 0.0f; i < (float)dynamicWaterTurbulenceFrequency; i += (1.0f / 128.0f)) {
                x = randyfloat() * 128.0f;
                y = randyfloat() * 128.0f;
                temp = DebMat[0]->tid;
                DebMat[0]->tid = DropletTID;
                NuRndrRectUV2di((int)x, (int)y, (int)scale, (int)scale, 0.0f, 0.0f, 1.0f, 1.0f, col, DebMat[0]);
                DebMat[0]->tid = temp;
            }
        }
    }
    return;
}

//NGC MATCH
void NuDynamicWaterUpdate(s32 forceupdate) {
    struct D3DSurface *currentBackBuffer;
    struct D3DSurface *currentZBuffer;
    s32 i;

    if (((dynamicWaterInitialised != 0) && (dynamicWaterEnabled != 0)) && ((watervisible != 0 || (forceupdate != 0)))) {
        currentBackBuffer = NudxFw_GetBackBuffer();
        currentZBuffer = NudxFw_GetZBuffer();
        ResetShaders();
        GS_SetZCompare(0,0,GX_LEQUAL);
        NudxFw_SetRenderState(0x52,0);
        GS_SetAlphaCompare(7,0);
        GS_SetBlendSrc(0,0,1);
        NudxFw_SetRenderState(0x5d,0);
        NudxFw_SetRenderState(0x7d,0);
        GS_EnableLighting(0);
        GS_EnableColorVertex(0);
        //NuSetShaderState(CALCNEIGHBOURFORCE,NUVT_TLTC1);
        NudxFw_SetRenderTargetSurface(dynamicWaterSurfaces[dynamicWaterForceStepOneTex],NULL);
        GS_SetBlendSrc(1,1,0);
        NuTexSetTexture(0,dynamicWaterTextureIds[dynamicWaterHeightSourceTex]);
        NuTexSetTexture(1,dynamicWaterTextureIds[dynamicWaterHeightSourceTex]);
        NuTexSetTexture(2,dynamicWaterTextureIds[dynamicWaterHeightSourceTex]);
        NuTexSetTexture(3,dynamicWaterTextureIds[dynamicWaterHeightSourceTex]);
        for (i = 0; i < 4; i++) {
            NudxFw_SetTextureState(i,D3DTSS_MAGFILTER,2);
            NudxFw_SetTextureState(i,D3DTSS_MINFILTER,2);
            NudxFw_SetTextureState(i,D3DTSS_MIPFILTER,0);
            NudxFw_SetTextureState(i,D3DTSS_TEXTURETRANSFORMFLAGS,0);
            NudxFw_SetTextureState(i,D3DTSS_TEXCOORDINDEX,i);
            NudxFw_SetTextureState(i,D3DTSS_RESULTARG,1);
        }
        NuDynamicWaterSetVertexShaderUVOffsets(1);
        NuDynamicWaterRender2dRect(0x80,0x80);
        //NuSetShaderState(CALCNEIGHBOURFORCE2,NUVT_TLTC1);
        NudxFw_SetRenderTargetSurface(dynamicWaterSurfaces[dynamicWaterForceTex],NULL);
        NuTexSetTexture(0,dynamicWaterTextureIds[dynamicWaterHeightSourceTex]);
        NuTexSetTexture(1,dynamicWaterTextureIds[dynamicWaterHeightSourceTex]);
        NuTexSetTexture(2,dynamicWaterTextureIds[dynamicWaterForceStepOneTex]);
        NuDynamicWaterSetVertexShaderUVOffsets(2);
        NuDynamicWaterRender2dRect(0x80,0x80);
        //NuSetShaderState(APPLYFORCEORVELOCITY,NUVT_TLTC1);
        NudxFw_SetRenderTargetSurface(dynamicWaterSurfaces[dynamicWaterVelocityTargetTex],NULL);
        NuTexSetTexture(0,dynamicWaterTextureIds[dynamicWaterVelocitySourceTex]);
        NuTexSetTexture(1,dynamicWaterTextureIds[dynamicWaterForceTex]);
        NuTexSetTexture(2,0);
        NuTexSetTexture(3,0);
        NuDynamicWaterSetVertexShaderUVOffsets(0);
        NuDynamicWaterRender2dRect(0x80,0x80);
        //NuSetShaderState(APPLYFORCEORVELOCITY,NUVT_TLTC1);
        NudxFw_SetRenderTargetSurface(dynamicWaterSurfaces[dynamicWaterHeightTargetTex],NULL);
        NuTexSetTexture(0,dynamicWaterTextureIds[dynamicWaterHeightSourceTex]);
        NuTexSetTexture(1,dynamicWaterTextureIds[dynamicWaterVelocityTargetTex]);
        NuDynamicWaterRender2dRect(0x80,0x80);
        //NuSetShaderState(BLURFILTER,NUVT_TLTC1);
        NudxFw_SetRenderTargetSurface(dynamicWaterSurfaces[dynamicWaterBlurTex],NULL);
        i = dynamicWaterHeightTargetTex;
        dynamicWaterHeightTargetTex = dynamicWaterHeightSourceTex;
        dynamicWaterHeightSourceTex = i;
        NuTexSetTexture(0,dynamicWaterTextureIds[dynamicWaterHeightSourceTex]);
        NuTexSetTexture(1,dynamicWaterTextureIds[dynamicWaterHeightSourceTex]);
        NuTexSetTexture(2,dynamicWaterTextureIds[dynamicWaterHeightSourceTex]);
        NuTexSetTexture(3,dynamicWaterTextureIds[dynamicWaterHeightSourceTex]);
        NuDynamicWaterSetVertexShaderUVOffsets(4);
        NuDynamicWaterRender2dRect(0x80,0x80);
        //NuSetShaderState(CREATENORMALMAP,NUVT_TLTC1);
        NudxFw_SetRenderTargetSurface(dynamicWaterSurfaces[dynamicWaterNormalTex],NULL);
        NuTexSetTexture(0,dynamicWaterTextureIds[dynamicWaterHeightTargetTex]);
        NuTexSetTexture(1,dynamicWaterTextureIds[dynamicWaterHeightTargetTex]);
        NuTexSetTexture(2,dynamicWaterTextureIds[dynamicWaterHeightTargetTex]);
        NuTexSetTexture(3,dynamicWaterTextureIds[dynamicWaterHeightTargetTex]);
        NuDynamicWaterSetVertexShaderUVOffsets(5);
        NuDynamicWaterRender2dRect(0x80,0x80);
        NuDynamicWaterExcite();
        NuDynamicWaterCycleTextures();
        NudxFw_SetRenderTargetSurface(currentBackBuffer,currentZBuffer);
        ResetShaders();
        NudxFw_SetRenderState(0x52,fog_enabled);
    }
    return;
}

//NGC MATCH
static s32 isWaterInstance(struct nugscn_s *gsc,struct nuinstance_s *inst) {
    struct nugeom_s *geom;

    geom = gsc->gobjs[inst->objid]->geom;
    while( geom != NULL ) {
        //if ((NuShaderAssignShader(geom) == 1) || (NuShaderAssignShader(geom) == 0x1b)) return 1;
        geom = geom->next;
    }
    return 0;
}

//NGC MATCH
static void initWaterGeom(struct nugeom_s* geom) {
    s32 i;
    s32 vtxcnt = geom->vtxcnt;
    struct nuvtx_tc1_s* vbptr = (struct nuvtx_tc1_s*)geom->hVB;
    int nrmY;

    for (i = 0; i < vtxcnt; i++) {
        nrmY = vbptr->nrm.y;
        vbptr->nrm.x = (nrmY > 0 ? nrmY : -nrmY) % 32;
        vbptr++;
    }

    return;
}

//NGC MATCH
static void initWater(struct nugscn_s* gsc) {
    s32 i;
    s32 num_water_insts;
    struct nugeom_s *geom;

    num_water_insts = 0;
    for (i = 0; i < gsc->numinstance; i++) {
            if (isWaterInstance(gsc,&gsc->instances[i]) != 0) {
                num_water_insts++;
            }
    }
    if (0xff < nwinst + num_water_insts) {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/nuwater.c",0x35b)("assert");
    }
    for (i = 0; i < gsc->numinstance; i++) {
            if (isWaterInstance(gsc,&gsc->instances[i]) != 0) {
                wgsc[nwinst] = gsc;
                winst[nwinst++] = &gsc->instances[i];
                gsc->instances[i].flags.visible = 0;
                for (geom = gsc->gobjs[gsc->instances[i].objid]->geom; geom != NULL;
                    geom = geom->next) {
                    initWaterGeom(geom);
                }
            }
    }
    return;
}

//NGC MATCH
static void animateWater(float scale_in, float theta_in, struct numtl_s *mtl) {
    int i;
    float scale;
    float theta;
    struct nuvec_s v;
    struct numtx_s mtx;
    struct nuvec_s scalevec;
    char pad[70];


    scalevec = *(struct nuvec_s *)makenuvec(0.5f,0.5f,0.5f);
    NuMtxInvR(&mtx,NuCameraGetMtx());
    NuMtxScale(&mtx,&scalevec);
    theta = (theta_in * (mtl->fx1).f32);
    scale = (scale_in * (mtl->fx4).f32);
    mtx._03 = mtx._13 = mtx._23 = 0.0f;
    NuRandSeed(0x11);
    for (i = 0; i < 32; i++) {
        v.x = NuTrigTable[(int)((theta * (NuRandFloat() * 0.4f + 0.8f ) + (NuRandFloat() * 6.2839999f)) *
                                    10430.378f) & 0xffff] * scale;

        v.y = 0.0f;
        v.z = (NuTrigTable[(int)((theta * (NuRandFloat() * 0.4f + 0.69999999f)
                               + (NuRandFloat() * 6.2839999f)) * 10430.378f) & 0xffff] * scale);
        NuVecMtxTransformH(&waterUVOffset[i],&v,&mtx);
        waterUVOffset[i].w = v.x;
    }
    return;
}

//NGC MATCH
void NuWaterLoad(struct nugscn_s *gsc) {
    if (gsc != NULL) {
        initWater(gsc);
        fixMaterials();
    }
    return;
}

//NGC MATCH
void NuWaterInit(void) {

    float p2w = Powr2(SWIDTH);
    float p2h = Powr2(SHEIGHT);
    NuRandSeed(0x11);
    water_copy_mtl = NuMtlCreate(1);

    //(attrib & 0xcc0cffff | 0x16e8000);
    water_copy_mtl->attrib.cull = 2;
    water_copy_mtl->attrib.zmode = 3;
    water_copy_mtl->attrib.filter = 0;
    water_copy_mtl->attrib.lighting = 2;
    water_copy_mtl->attrib.colour = 1;
    (water_copy_mtl->diffuse).r = 1.0f;
    (water_copy_mtl->diffuse).g = 1.0f;
    (water_copy_mtl->diffuse).b = 1.0f;
    water_copy_mtl->alpha = 0.999f;
    water_copy_mtl->attrib.utc = 1;
    water_copy_mtl->attrib.vtc = 1;
    water_copy_mtl->tid = NudxFw_GetBackBufferCopyTID();
    NuMtlUpdate(water_copy_mtl);
    if (dynamicWaterEnabled != 0) {
        NuDynamicWaterInit();
    }
    nwinst = 0;
    nwater_mtl = 0;
    return;
}

//NGC MATCH
void NuWaterClose(void) {
    nwinst = 0;
    nwater_mtl = 0;
    if (dynamicWaterEnabled != 0) {
        NuDynamicWaterClose();
    }
    return;
}

//NGC MATCH
void NuWaterSetup(void) {
    s32 i;

    for (i = 0; i < 0x20; i++) {

    }
    return;
}

//NGC MATCH
void NuWaterSpeed(float speed) {
    theta_step = speed * 0.1f;
    return;
}

//NGC MATCH
void NuWaterRender(void) {
    s32 outcode;
    s32 i;
    static float theta;

    watervisible = 0;
    if (nwinst != 0) {
        animateWater(0.1f,theta,wgsc[0]->gobjs[winst[0]->objid]->geom->mtl);
            for (i = 0; i < nwinst; i++) {
                if ((winst[i]->flags.visitest) != 0) {
                    NuRndrGobj(world_scene[0]->gobjs[winst[i]->objid],&winst[i]->mtx,NULL);
                    nurndr_forced_mtl = dynamicWaterMaterial;
                    outcode = NuRndrGobj(world_scene[0]->gobjs[winst[i]->objid],&winst[i]->mtx,NULL);
                    nurndr_forced_mtl = NULL;
                    if (outcode != 0) {
                        watervisible = 1;
                    }
                }
            }
        theta = theta + theta_step;
    }
    return;
}

