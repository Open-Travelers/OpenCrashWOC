#include "nulight.h"

#define POW2(x) ((x) * (x))

//s32 initialised = 0;
s32 maxlights = 3;
s32 numlights = 0;
static s32 freelight = -1;
static s32 alloclight = -1;
static s32 current_lights_stored = 0;
f32 buglight_distance;

struct nusyslight_s light[3];
struct _LIGHTLIST GS_LightList[3];
struct nulight_s* currentlight1;
struct nulight_s* currentlight2;
struct nulight_s* currentlight3;
struct numtl_s* NuLightAddMat;

//Ext var
s32 HazeValue = 0;

//NGC MATCH
void NuLightClose(void) {
  numlights = 0;
  initialised = 0;
  return;
}

//NGC MATCH
void NuLightFog(float pnear,float pfar,u32 colour,s32 blur,s32 haze) {
  NuRndrFogBlur = blur << 0x18;
  HazeValue = (u32)haze >> 0x18;
  NuRndrFogNear = pnear;
  NuRndrFogFar = pfar;
  NuRndrFogCol = (colour & 0xff000000) + (colour & 0xff) * 0x10000 + (colour & 0xff00) + (colour >> 0x10 & 0xff);
  if (pfar != pnear) {
    GS_SetupFog(1,pnear,pfar,NuRndrFogCol);
  }
  else {
    GS_SetupFog(0,0.0f,0.0f,0);
  }
  return;
}

//NGC MATCH
void NuLightInit(void) {
  s32 i;

  if (initialised != 0) {
    NuLightClose();
  }
  numlights = 0;
  maxlights = 3;
  memset(light,0,0x12c);
  light[0].last = -1;
  light[0].next = 1;
  for (i = 1; i <= 1; i++) {
    light[i].last = i - 1;
    light[i].next = i + 1;
  }
  light[i].last = i - 1;
  light[i].next = -1;
  freelight = 0;
  alloclight = -1;
  initialised = 1;
  return;
}

//NGC MATCH
void* NuLightCreate(void) {
    s32 id;
    s32 next;
    s32 tempalloclight;

  if (numlights < maxlights) {
    next = light[freelight].next;
    tempalloclight = alloclight;
    alloclight = freelight;
    id = alloclight;
    freelight = next;
    light[freelight].last = -1;
    light[tempalloclight].last = alloclight;
    light[alloclight].next = tempalloclight;
    memset(&light[id],0,100);
    NuMtxSetIdentity(&light[id].light.mtx);
    numlights++;
    return &light[id].light;
  }
    return NULL;
}

//MATCH NGC
void NuLightUpdate(struct nulight_s* l) {
    struct _D3DLIGHT8 d3dlight;
    s32 id;

    id = (struct nusyslight_s*)l - &light[0];
    d3dlight.Direction.x = -(l->mtx)._20;
    d3dlight.Type = D3DLIGHT_DIRECTIONAL;
    d3dlight.Diffuse.r = (l->diffuse).b;
    d3dlight.Diffuse.g = (l->diffuse).g;
    d3dlight.Diffuse.b = (l->diffuse).r;
    d3dlight.Ambient.r = (l->ambient).b;
    d3dlight.Ambient.g = (l->ambient).g;
    d3dlight.Ambient.b = (l->ambient).r;
    d3dlight.Diffuse.a = 1.0f;
    d3dlight.Position.x = (l->mtx)._30;
    d3dlight.Position.y = (l->mtx)._31;
    d3dlight.Position.z = (l->mtx)._32;
    d3dlight.Specular.r = 0.0f;
    d3dlight.Specular.g = 0.0f;
    d3dlight.Specular.b = 0.0f;
    d3dlight.Specular.a = 1.0f;
    d3dlight.Ambient.a = 1.0f;
    d3dlight.Direction.y = -(l->mtx)._21;
    d3dlight.Direction.z = -(l->mtx)._22;
    d3dlight.range = 1000.0f;
    d3dlight.falloff = 0.0f;
    d3dlight.attenuation0 = 1.0f;
    d3dlight.attenuation1 = 0.0f;
    d3dlight.attenuation2 = 0.0f;
    d3dlight.theta = 0.0f;
    d3dlight.phi = 0.0f;
    if (((d3dlight.Direction.x == 0.0) && (d3dlight.Direction.y == 0.0)) && (d3dlight.Direction.z == 0.0)) {
        d3dlight.Direction.x = 1.0f;
    }
    GS_SetLight(id, &d3dlight);
    GS_LightEnable(id, 1);
    return;
}

//MATCH NGC
struct nulight_s * NuLightGetLight(s32 index) {
  if (index < numlights) {
      return &light[index].light;
  }
  return NULL;
}

//MATCH NGC
void NuLightSetAmbientLight(struct nuvec_s *c0) {
  struct nulight_s *curr;

  curr = currentlight1;
  if (currentlight1 != NULL) {
    (currentlight1->ambient).r = c0->x;
    (curr->ambient).g = c0->y;
    (curr->ambient).b = c0->z;
  }
  curr = currentlight2;
  if (currentlight2 != NULL) {
    (currentlight2->ambient).r = c0->x;
    (curr->ambient).g = c0->y;
    (curr->ambient).b = c0->z;
  }
  curr = currentlight3;
  if (currentlight3 != NULL) {
    (currentlight3->ambient).r = c0->x;
    (curr->ambient).g = c0->y;
    (curr->ambient).b = c0->z;
  }
  current_lights_stored = 0;
  return;
}

//NGC MATCH
void NuLightSetDirectionalLights (struct nuvec_s *d0,struct nucolour3_s *c0,struct nuvec_s *d1,struct nucolour3_s *c1,struct nuvec_s *d2,struct nucolour3_s *c2) {
  if ((currentlight1 != NULL) || (currentlight1 = NuLightCreate(), currentlight1 != NULL)) {
    (currentlight1->ambient).r = 0.6f;
    (currentlight1->ambient).g = 0.6f;
    (currentlight1->ambient).b = 0.6f;
    (currentlight1->diffuse).r = (1.0f < c0->r) ? 1.0f : c0->r;
    (currentlight1->diffuse).g = (1.0f < c0->g) ? 1.0f : c0->g;
    (currentlight1->diffuse).b = (1.0f < c0->b) ? 1.0f : c0->b;
    NuMtxSetIdentity(&currentlight1->mtx);
    NuMtxAlignZ(&currentlight1->mtx,d0);
  }
  if ((currentlight2 != NULL) || (currentlight2 = NuLightCreate(), currentlight2 != NULL)) {
    (currentlight2->ambient).r = 0.6f;
    (currentlight2->ambient).g = 0.6f;
    (currentlight2->ambient).b = 0.6f;
    (currentlight2->diffuse).r = (1.0f < c1->r) ? 1.0f : c1->r;
    (currentlight2->diffuse).g = (1.0f < c1->g) ? 1.0f : c1->g;
    (currentlight2->diffuse).b = (1.0f < c1->b) ? 1.0f : c1->b;
    NuMtxSetIdentity(&currentlight2->mtx);
    NuMtxAlignZ(&currentlight2->mtx,d1);
  }
  if ((currentlight3 != NULL) || (currentlight3 = NuLightCreate())) {
      (currentlight3->ambient).r = 0.6f;
      (currentlight3->ambient).g = 0.6f;
      (currentlight3->ambient).b = 0.6f;
      (currentlight3->diffuse).r = (1.0f < c2->r) ? 1.0f : c2->r;
      (currentlight3->diffuse).g = (1.0f < c2->g) ? 1.0f : c2->g;
      (currentlight3->diffuse).b = (1.0f < c2->b) ? 1.0f : c2->b;
      NuMtxSetIdentity(&currentlight3->mtx);
      NuMtxAlignZ(&currentlight3->mtx,d2);
  }
  current_lights_stored = 0;
  return;
}

//NGC MATCH
s32 NuLightStoreCurrentLights(void) {
  s32 i;
  s32 lightsleft;

  if (1000 - num_stored_lights < 1) {
    NuErrorProlog("C:/source/crashwoc/code/nu3dx/nulight.c",0x187)("NuLight: Out of light storage space!");
  }
  if (current_lights_stored == 0) {
      for (i = 0; i < numlights; i++) {
        StoredLights[num_stored_lights].light[i] = *NuLightGetLight(i);
      }
    current_lights_stored = 1;
    return num_stored_lights++;
  }

  lightsleft = num_stored_lights - 1;
  return lightsleft;
}

//NGC MATCH
void NuLightSetStoredLights(s32 index) {

  if (currentlight1 == NULL) {
    currentlight1 = NuLightCreate();
  }
  if (currentlight2 == NULL) {
    currentlight2 = NuLightCreate();
  }
  if (currentlight3 == NULL) {
    currentlight3 = NuLightCreate();
  }
  *currentlight1 = StoredLights[index].light[0];
  NuLightUpdate(currentlight1);
  *currentlight2 = StoredLights[index].light[1];
  NuLightUpdate(currentlight2);
  *currentlight3 = StoredLights[index].light[2];
  NuLightUpdate(currentlight3);
  return;
}

//NGC MATCH
void NuLightClearStoredLights(void) {
    num_stored_lights = 0;
    current_lights_stored = 0;
}

//NGC MATCH
void NuLightAddSpotXSpanFade(struct nuvec_s* pos, float XSpan, float fade) {
    buglight_distance = (2.0f / XSpan) * 3.0f;
    return;
}

//NGC MATCH
void NuLightMatInit(void) {
  NuLightAddMat = NuMtlCreate(1);
  (NuLightAddMat->diffuse).r = 1.0f;
  (NuLightAddMat->diffuse).g = 1.0f;
  (NuLightAddMat->diffuse).b = 1.0f;
  //(NuLightAddMat->attrib & 0x3c00fff9 | 0x81660002);
    NuLightAddMat->attrib.cull = 2;
    NuLightAddMat->attrib.zmode = 1;
    NuLightAddMat->alpha = 0.999f;
    NuLightAddMat->tid = 0;
    NuLightAddMat->alpha_sort = 0x100;
    NuLightAddMat->attrib.alpha = 2;
    NuLightAddMat->attrib.afail = 1;
    NuLightAddMat->attrib.utc = 1;
    NuLightAddMat->attrib.vtc = 1;
    NuLightAddMat->attrib.lighting = 2;
  NuMtlUpdate(NuLightAddMat);
  return;
}

//MATCH NGC
s32 NuLgtRand(void) {
  NuLgtSeed = NuLgtSeed * 0x24cd + 1U & 0xffff;
  return NuLgtSeed;
}

//MATCH NGC
void NuLgtArcLaser(s32 type,struct nuvec_s *start,struct nuvec_s *target,struct nuvec_s *lasdir,float sizew,float sizel,float sizewob,float arcsize,s32 col) {
    s32 lp;
    u32 col2;
    u32 a;
    u32 r;
    u32 g;
    u32 b;
    
    a = (col >> 0x17) & 0x1fe;
    
    if (0xff < a) {
        a = 0xff;
    }
    
    b = (col & 0xff) << 1;
    
    col2 = a << 0x18;
    
    if (0xff < b) {
        b = 0xff;
    }
    
    col2 |= 0xFFFF;
    col2 |= (b << 0x10);
    
    if (NuLgtArcLaserCnt < 16) {
        NuLgtArcLaserData[NuLgtArcLaserCnt].type = type;
        NuLgtArcLaserData[NuLgtArcLaserCnt].start = *start;
        NuLgtArcLaserData[NuLgtArcLaserCnt].target = *target;
        NuLgtArcLaserData[NuLgtArcLaserCnt].lasdir = *lasdir;
        NuLgtArcLaserData[NuLgtArcLaserCnt].sizew = sizew;
        NuLgtArcLaserData[NuLgtArcLaserCnt].sizel = sizel;
        NuLgtArcLaserData[NuLgtArcLaserCnt].sizewob = sizewob;
        NuLgtArcLaserData[NuLgtArcLaserCnt].arcsize = arcsize;
        NuLgtArcLaserData[NuLgtArcLaserCnt].col = col2;
        if (((NuLgtArcLaserFrame & 1U) == 0) || (NuLgtArcLaserData[NuLgtArcLaserCnt].seed == 0)) {
            NuLgtArcLaserData[NuLgtArcLaserCnt].seed = NuLgtRand();
        }
        for (lp = 0; lp < 6; lp++) {
            NuLgtRand();
        }
        NuLgtArcLaserCnt++;
    }
    return;
}

//91% NGC (struct problem??)
 void NuLgtArcLaserDraw(s32 paused) {
    s32 id;
    struct nuvec4_s *pnts;
    struct nuvec_s norm;
    struct nuvec_s norm2;   
    struct numtx_s *vpcsmtx;
    float fVar1;
    float fVar4;
    s32 iVar10;
    u8 uVar11;
    float dVar12;
    float dVar13;
    float dVar19;
    float dVar21;
    float dVar22;
    float local_188;
    struct nuvtx_tltc1_s dxpnts [4];
     
    dxpnts[0].diffuse = NuLgtArcV0;
    dxpnts[0].tc[0] = NuLgtArcV0;
    dxpnts[0].tc[1] = NuLgtArcV1;
    dxpnts[0].tc[2] = NuLgtArcV1;
     
    if (paused != 0) {
        NuLgtArcLaserCnt = NuLgtArcLaserOldCnt;
    }
    if ((NuLgtArcMtl == NULL) || (NuLgtArcLaserCnt == 0)) {
        NuLgtArcLaserOldCnt = 0;
        return;
    }
     
    pnts = (struct nuvec4_s *)NuScratchAlloc32(0xc0);
    vpcsmtx = NuCameraGetVPCSMtx();
    if (NuRndrBeginScene(1) != 0) {
        for (iVar10 = 0; iVar10 < NuLgtArcLaserCnt; iVar10++) {
            pnts[4].x = NuLgtArcLaserData[iVar10].start.x;
            pnts[4].y = NuLgtArcLaserData[iVar10].start.y;
            pnts[4].z = NuLgtArcLaserData[iVar10].start.z;
            pnts[4].w = 1.0f;
            NuVec4MtxTransformVU0(&pnts[4], &pnts[4], vpcsmtx);
            
            pnts[5].x = NuLgtArcLaserData[iVar10].target.x;
            pnts[5].y = NuLgtArcLaserData[iVar10].target.y;
            pnts[5].z = NuLgtArcLaserData[iVar10].target.z;
            pnts[5].w = 1.0f;
            
            NuVec4MtxTransformVU0(&pnts[5], &pnts[5] ,vpcsmtx);
            
            if (0.5f <= pnts[4].w) {
                pnts[6].x = NuLgtArcLaserData[iVar10].target.x + ((NuLgtArcLaserData[iVar10].start.x - NuLgtArcLaserData[iVar10].target.x) * (pnts[5].w - 0.5f)) / (pnts[5].w - pnts[4].w);
                pnts[6].y = NuLgtArcLaserData[iVar10].target.y + ((NuLgtArcLaserData[iVar10].start.y - NuLgtArcLaserData[iVar10].target.y) * (pnts[5].w - 0.5f)) / (pnts[5].w - pnts[4].w);
                pnts[6].z = NuLgtArcLaserData[iVar10].target.z + ((NuLgtArcLaserData[iVar10].start.z - NuLgtArcLaserData[iVar10].target.z) * (pnts[5].w - 0.5f)) / (pnts[5].w - pnts[4].w);
                pnts[6].w = 1.0f;
            }
            else {
                pnts[6].x = NuLgtArcLaserData[iVar10].start.x;
                pnts[6].y = NuLgtArcLaserData[iVar10].start.y;
                pnts[6].z = NuLgtArcLaserData[iVar10].start.z;
                pnts[6].w = 1.0f;
            }
            
            sqrt(POW2(pnts[6].x - NuLgtArcLaserData[iVar10].target.x) + POW2(pnts[6].y - NuLgtArcLaserData[iVar10].target.y) + POW2(pnts[6].z - NuLgtArcLaserData[iVar10].target.z));
            if (0.5f > pnts[5].w) {
                pnts[7].x = NuLgtArcLaserData[iVar10].start.x + ((NuLgtArcLaserData[iVar10].target.x - NuLgtArcLaserData[iVar10].start.x) * (pnts[4].w - 0.5f)) / (pnts[4].w - pnts[5].w);
                pnts[7].y = NuLgtArcLaserData[iVar10].start.y + ((NuLgtArcLaserData[iVar10].target.y - NuLgtArcLaserData[iVar10].start.y) * (pnts[4].w - 0.5f)) / (pnts[4].w - pnts[5].w);
                pnts[7].z = NuLgtArcLaserData[iVar10].start.z + ((NuLgtArcLaserData[iVar10].target.z - NuLgtArcLaserData[iVar10].start.z) * (pnts[4].w - 0.5f)) / (pnts[4].w - pnts[5].w);
                pnts[7].w = 1.0f;
            }
            else {
                pnts[7].x = NuLgtArcLaserData[iVar10].target.x;
                pnts[7].y = NuLgtArcLaserData[iVar10].target.y;
                pnts[7].z = NuLgtArcLaserData[iVar10].target.z;
                pnts[7].w = 1.0f;
            }
            
            NuVec4MtxTransformVU0(&pnts[4], &pnts[6], vpcsmtx);
            NuVec4Scale(&pnts[4], &pnts[4], 1.0f / pnts[4].w);
            NuVec4MtxTransformVU0(&pnts[5], &pnts[7], vpcsmtx);
            NuVec4Scale(&pnts[5], &pnts[5], 1.0f / pnts[5].w);
            
            dVar13 = 0.0f;
            norm.x = (pnts[5].y - pnts[4].y) * (((float)SWIDTH) / 240.0f);
            norm.y = pnts[4].x - pnts[5].x;
            norm.z = 0.0f;
            
            NuVecNorm(&norm, &norm);
            norm.x = norm.x * (NuLgtArcLaserData[iVar10].sizew *((((float)SWIDTH) * 150.0f) / 240.0f));
            norm.y = norm.y * (NuLgtArcLaserData[iVar10].sizew * 150.0f);
            
            pnts[7].x = pnts[7].x - pnts[6].x;
            pnts[7].y = pnts[7].y - pnts[6].y;
            pnts[7].z = pnts[7].z - pnts[6].z;
            pnts[7].w = pnts[7].w - pnts[6].w;
            
            dVar19 = sqrt(POW2(pnts[7].x) + POW2(pnts[7].y) + POW2(pnts[7].z));
            if (0.0f < dVar19) {
                pnts[10].x = NuLgtArcLaserData[iVar10].lasdir.x;
                pnts[10].y = NuLgtArcLaserData[iVar10].lasdir.y;
                pnts[10].z = NuLgtArcLaserData[iVar10].lasdir.z;
                dVar21 = POW2(pnts[10].x) + POW2(pnts[10].y) + POW2(pnts[10].z);
                if (dVar21 > 0.0f) {
                    dVar21 = (NuLgtArcLaserData[iVar10].arcsize / NuFsqrt(pnts[6].w));
                }
                pnts[10].x = (pnts[10].x * dVar21);
                pnts[10].y = (pnts[10].y * dVar21);
                pnts[10].z = (pnts[10].z * dVar21);
                NuLgtSeed = NuLgtArcLaserData[iVar10].seed;
                dVar19 = (NuLgtArcLaserData[iVar10].sizel / dVar19);
                if (1.0f > dVar19) {
                    dVar19 = 1.0f;
                }
                dxpnts[0].pnt.x = dxpnts[0].pnt.z = NuLgtArcU0;
                dxpnts[0].pnt.y = dxpnts[0].rhw = NuLgtArcU1;
                pnts[4].x = pnts[6].x;
                pnts[4].y = pnts[6].y;
                pnts[4].z = pnts[6].z;
                pnts[4].w = 1.0f;
                
                NuVec4MtxTransformVU0(&pnts[4], &pnts[4], vpcsmtx);
                dVar21 = (1.0f / pnts[4].w);
                NuVec4Scale(&pnts[4], &pnts[4], dVar21);
                norm2.x = (norm.x * dVar21);
                norm2.y = (norm.y * dVar21);
                pnts[1].x = pnts[4].x;
                pnts[1].y = pnts[4].y;
                pnts[1].z = pnts[4].z;
                pnts[3].x = pnts[4].x;
                pnts[3].y = pnts[4].y;
                pnts[3].z = pnts[4].z;
                    
                for (; dVar13 < 1.0f; dVar13 = dVar12) {
                    dVar12 = (dVar13 + dVar19);
                    if (dVar12 >= 1.0f) {
                        pnts[5].x = pnts[6].x + pnts[7].x;
                        pnts[5].y = pnts[6].y + pnts[7].y;
                        pnts[5].z = pnts[6].z + pnts[7].z;
                        dxpnts[0].pnt.x = dxpnts[0].pnt.z = NuLgtArcU1 + (((NuLgtArcU0 - NuLgtArcU1) * (1.0f - dVar13)) / dVar19);
                    }
                    else {
                        pnts[5].x = pnts[10].x * NuTrigTable[(u16)(dVar13 + dVar19)] + (pnts[7].x * dVar12 + pnts[6].x);
                        pnts[5].y = pnts[10].y * NuTrigTable[(u16)(dVar13 + dVar19)] + (pnts[7].y * dVar12 + pnts[6].y);
                        pnts[5].z = pnts[10].z * NuTrigTable[(u16)(dVar13 + dVar19)] + (pnts[7].z * dVar12 + pnts[6].z);
                    }
                    pnts[5].w = 1.0f;
                    NuVec4MtxTransformVU0(&pnts[5], &pnts[5], vpcsmtx);
                    dVar22 = (1.0f / pnts[5].w);
                    NuVec4Scale(&pnts[5], &pnts[5], dVar22);
                    if (((1.0f - (dVar19 * 1.5f)) < dVar13) && (NuLgtSeed != 0)) {
                        uVar11 = NuLgtRand();
                        fVar1 = ((norm.x * (((uVar11) - 0x80)) * NuLgtArcLaserData[iVar10].sizewob) * dVar22);
                        fVar4 = ((norm.y * (((uVar11) - 0x80)) * NuLgtArcLaserData[iVar10].sizewob) * dVar22);
                    }
                    else {
                        fVar1 = 0.0f;
                        fVar4 = 0.0f;
                    }
                    norm2.x = (norm.x * dVar22);
                    norm2.y = (norm.y * dVar22);
                    pnts->x = (pnts[5].x - norm2.x) + fVar1;
                    pnts->y = (pnts[5].y - norm2.y) + fVar4;
                    pnts[2].x = pnts[5].x + norm2.x + fVar1;
                    pnts[2].y = pnts[5].y + norm2.y + fVar4;
                    pnts->z = pnts[5].z;
                    pnts[2].z = pnts[5].z;
                    
                    if (((-50.0f > pnts->x) && (50.0f < pnts[1].x)) 
                        || ((pnts->x > 700.0f) && (pnts[1].x > 700.0f))
                        || ((-50.0f > pnts->y) && (-50.0f > pnts[1].y))
                        || (!(pnts->y > 530.0f) || !(pnts[1].y > 530.0f))
                    ) 
                    {
                        // dxpnts[0].pnt.x = pnts->x;
                        // dxpnts[0].pnt.y = pnts->y;
                        // dxpnts[0].pnt.z = pnts->z;
                        *(s32*)&dxpnts[1].tc[0] = *(s32*)&dxpnts[2].tc[0] = *(s32*)&dxpnts[3].tc[0] = NuLgtArcLaserData[iVar10].col;
                        dxpnts[0].rhw = 0.1f;
                        // dxpnts[0].diffuse = NuLgtArcLaserData[iVar10].col;
                        
                        dxpnts[1].pnt.x = pnts[1].x;
                        dxpnts[1].pnt.y = pnts[1].y;
                        dxpnts[1].pnt.z = pnts[1].z;
                        
                        // dxpnts[2].pnt.x = dxpnts[0].diffuse;
                        dxpnts[2].pnt.y = pnts[2].y;
                        dxpnts[2].pnt.z = pnts[2].z;
                        
                        dxpnts[0].tc[0] = dxpnts[0].pnt.x;
                        dxpnts[0].tc[1] = NuLgtArcV0;
                        dxpnts[1].rhw = 0.1f;
                        dxpnts[1].tc[1] = dxpnts[0].pnt.x;
                        dxpnts[2].rhw = 0.1f;
                        dxpnts[2].tc[0] = dxpnts[0].pnt.x;
                        dxpnts[2].tc[1] = NuLgtArcV1;
                        
                        dxpnts[1].diffuse = dxpnts[2].diffuse = dxpnts[3].diffuse = dVar12;
                        NuRndrTri2d(dxpnts, NuLgtArcMtl);
                        
                        dxpnts[0].pnt.x = pnts[1].x;
                        dxpnts[0].pnt.y = pnts[1].y;
                        dxpnts[0].pnt.z = pnts[1].z;
                        
                        dxpnts[1].pnt.x = pnts[3].x;
                        dxpnts[1].pnt.y = pnts[3].y;
                        dxpnts[1].pnt.z = pnts[3].z;
                        
                        dxpnts[2].pnt.x = pnts[2].x;
                        dxpnts[2].pnt.y = pnts[2].y;
                        dxpnts[2].pnt.z = pnts[2].z;
                        dxpnts[0].tc[0] = NuLgtArcU1;
                        dxpnts[0].tc[1] = NuLgtArcV0;
                        dxpnts[1].tc[0] = NuLgtArcU1;
                        dxpnts[1].tc[1] = NuLgtArcV1;
                        dxpnts[2].tc[0] = dxpnts[0].pnt.x;
                        dxpnts[2].tc[1] = NuLgtArcV1;
                        
                        dxpnts[1].diffuse = 0.1f;
                        dxpnts[2].diffuse = 0.1f;
                        dxpnts[3].diffuse = 0.1f;
                        
                        NuRndrTri2d(dxpnts, NuLgtArcMtl);
                    }
                    pnts[1].x = pnts->x;
                    pnts[1].y = pnts->y;
                    pnts[3].x = pnts[2].x;
                    pnts[3].y = pnts[2].y;
                    pnts[1].z = pnts->z;
                    pnts[3].z = pnts[2].z;
                }
            }
        }
        NuRndrEndScene();
        NuLgtArcLaserOldCnt = NuLgtArcLaserCnt;
        NuLgtArcLaserCnt = 0;
        if (paused == 0) {
            NuLgtArcLaserFrame++;
        }
    }
    NuScratchRelease();
  return;
}

//MATCH NGC
void NuLgtSetArcMat(struct numtl_s *mtl,float u0,float v0,float u1,float v1) {
  NuLgtArcMtl = mtl;
  NuLgtArcU0 = u0;
  NuLgtArcV0 = v0;
  NuLgtArcU1 = u1;
  NuLgtArcV1 = v1;
  return;
}
