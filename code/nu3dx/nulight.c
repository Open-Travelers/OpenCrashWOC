#include "nulight.h"

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
f32 NuRndrFogNear;
f32 NuRndrFogFar;
u32 NuRndrFogBlur;
u32 NuRndrFogCol;

void NuLightClose()
{
    initialised = 0;
    numlights = 0;
}

void NuLightFog(float pnear, float pfar, u32 colour, int blur, int haze)

{
    NuRndrFogCol = (colour & 0xff000000) + (colour & 0xff) * 0x10000 + (colour & 0xff00) +
        (colour >> 0x10 & 0xff);
    NuRndrFogBlur = blur << 0x18;
    HazeValue = (u32)haze >> 0x18;
    NuRndrFogNear = pnear;
    NuRndrFogFar = pfar;
    if (pfar == pnear) {
        GS_SetupFog(0, 0.0, 0.0, 0);
    }
    else {
        GS_SetupFog(1, pnear, pfar, NuRndrFogCol);
    }
    return;
}


void NuLightInit(void)

{
    int i;
    int iVar1;
    int* next;

    if (initialised != 0) {
        NuLightClose();
    }
    numlights = 0;
    maxlights = 3;
    memset(light, 0, 0x12c);
    light[0].last = -1;
    next = &light[1].next;
    light[0].next = 1;
    i = 1;
    do {
        iVar1 = i;
        i = iVar1 + 1;
        next[1] = iVar1 + -1;
        *next = i;
        next = next + 0x19;
    } while (i < 2);
    light[i].last = iVar1;
    light[i].next = -1;
    freelight = 0;
    alloclight = -1;
    initialised = 1;
    return;
}

struct nulight_s* NuLightCreate(void) {
    struct nusyslight_s* pnVar1;
    struct nusyslight_s* ret;
    s32 alloc;
    s32 free;

    alloc = alloclight;
    free = freelight;
    if (numlights < maxlights) {
        alloclight = freelight;
        pnVar1 = light + freelight;
        ret = light + freelight;
        freelight = pnVar1->next;
        light[pnVar1->next].last = -1;
        light[alloc].last = free;
        light[free].next = alloc;
        memset(ret, 0, 100);
        NuMtxSetIdentity(&light[free].light.mtx);
        numlights = numlights + 1;
    }
    else {
        ret = NULL;
    }
    return &ret->light;
}


void NuLightUpdate(struct nulight_s* l)	//Need Fix

{
    int Index;
    struct _D3DLIGHT8 d3dlight;

      d3dlight.Direction.z = -(l->mtx)._22;
  d3dlight.Diffuse.r = (l->diffuse).b;
  d3dlight.Diffuse.g = (l->diffuse).g;
  d3dlight.Direction.x = -(l->mtx)._20;
  d3dlight.Diffuse.b = (l->diffuse).r;
  d3dlight.Direction.y = -(l->mtx)._21;
  d3dlight.Ambient.r = (l->ambient).b;
  d3dlight.Ambient.g = (l->ambient).g;
  d3dlight.Ambient.b = (l->ambient).r;
  d3dlight.Position.x = (l->mtx)._30;
  d3dlight.Position.y = (l->mtx)._31;
  d3dlight.Position.z = (l->mtx)._32;
  Index = (int)(l + 0x1741a7c) * -0x3d70a3d7 >> 2;  //?
  d3dlight.Type = D3DLIGHT_DIRECTIONAL;
  d3dlight.Diffuse.a = 1.0;
  d3dlight.Specular.r = 0.0;
  d3dlight.Specular.g = 0.0;
  d3dlight.Specular.b = 0.0;
  d3dlight.Specular.a = 1.0;
  d3dlight.Ambient.a = 1.0;
  d3dlight.range = 1000.0;
  d3dlight.Phi = 0.0;
  d3dlight.falloff = 0.0;
  d3dlight.attenuation0 = 1.0;
  d3dlight.attenuation1 = 0.0;
  d3dlight.attenuation2 = 0.0;
  d3dlight.Theta = 0.0;
  if (((d3dlight.Direction.x == 0.0) && (d3dlight.Direction.y == 0.0)) &&
     (d3dlight.Direction.z == 0.0)) {
    d3dlight.Direction.x = 1.0;
  }
    //GS_SetLight(Index, &d3dlight);
    //GS_LightEnable(Index, 1);
    return;
}

struct nulight_s* NuLightGetLight(int index)

{
    if (numlights <= index) {
        return NULL;
    }
    return &light[index].light;
}

void NuLightSetAmbientLight(struct nuvec_s* c0)

{
    struct nulight_s* curr;

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


void NuLightSetDirectionalLights(struct nuvec_s* d0, struct nucolour3_s* c0, struct nuvec_s* d1, struct nucolour3_s* c1, struct nuvec_s* d2, struct nucolour3_s* c2)
{
    f32 fVar1;
    f32 fVar2;
    struct nulight_s* curr;

    currentlight1 = NuLightCreate();

    if ((currentlight1 != NULL) || (currentlight1 != NULL))
    {
        curr = currentlight1;
        fVar1 = 1.0;
        (currentlight1->ambient).b = 0.6;
        (curr->ambient).r = 0.6;
        (curr->ambient).g = 0.6;
        fVar2 = c0->r;

        if (1.0 < c0->r) {
            fVar2 = fVar1;
        }
        (curr->diffuse).r = fVar2;
        fVar2 = c0->g;
        if (1.0 < c0->g) {
            fVar2 = fVar1;
        }
        (curr->diffuse).g = fVar2;
        fVar2 = c0->b;
        if (1.0 < c0->b) {
            fVar2 = fVar1;
        }

        (curr->diffuse).b = fVar2;
        NuMtxSetIdentity(&curr->mtx);
        NuMtxAlignZ(&currentlight1->mtx, d0);
    }

    currentlight2 = NuLightCreate();


    if ((currentlight2 != NULL) || (currentlight2 != NULL))
    {
        curr = currentlight2;
        fVar1 = 1.0;
        (currentlight2->ambient).b = 0.6;
        (curr->ambient).r = 0.6;
        (curr->ambient).g = 0.6;
        fVar2 = c1->r;
        if (1.0 < c1->r) {
            fVar2 = fVar1;
        }
        (curr->diffuse).r = fVar2;
        fVar2 = c1->g;
        if (1.0 < c1->g) {
            fVar2 = fVar1;
        }
        (curr->diffuse).g = fVar2;
        fVar2 = c1->b;
        if (1.0 < c1->b) {
            fVar2 = fVar1;
        }
        (curr->diffuse).b = fVar2;
        NuMtxSetIdentity(&curr->mtx);
        NuMtxAlignZ(&currentlight2->mtx, d1);
    }

    currentlight3 = NuLightCreate();

    if ((currentlight3 == NULL) && (currentlight3 == NULL))
    {
        current_lights_stored = 0;
            return;
    }
    curr = currentlight3;
        fVar1 = 1.0;
        (currentlight3->ambient).b = 0.6;
        (curr->ambient).r = 0.6;
        (curr->ambient).g = 0.6;
        fVar2 = c2->r;
        if (1.0 < c2->r) {
            fVar2 = fVar1;
        }
    (curr->diffuse).r = fVar2;
    fVar2 = c2->g;
    if (1.0 < c2->g) {
        fVar2 = fVar1;
    }
    (curr->diffuse).g = fVar2;
    fVar2 = c2->b;
    if (1.0 < c2->b) {
        fVar2 = fVar1;
    }
    (curr->diffuse).b = fVar2;
    NuMtxSetIdentity(&curr->mtx);
    NuMtxAlignZ(&currentlight3->mtx, d2);
    current_lights_stored = 0;
    return;

}


int NuLightStoreCurrentLights(void)		//TODO

{
    s32 iVar1;
    struct nulight_s* pnVar2;
    struct nulight_s* pnVar3;
    struct nulight_s* pnVar4;
    s32 i;
    s32 iVar5;
    struct nulight_s* pnVar6;
    s32 lightsleft;

    if (1000 - num_stored_lights < 1) {
        /*NuErrorProlog("C:/source/crashwoc/code/nu3dx/nulight.c", 0x187);
        ("NuLight: Out of light storage space!");*/
    }
    if (current_lights_stored == 0) {
        lightsleft = 0;
        if (0 < numlights) {
            do {
                pnVar3 = NuLightGetLight(lightsleft);
                i = lightsleft + 1;
                iVar5 = 0x48;
                pnVar2 = StoredLights[num_stored_lights].light + lightsleft;
                do {
                    pnVar6 = pnVar3;
                    pnVar4 = pnVar2;
                    iVar5 = iVar5 + -0x18;
                    (pnVar4->ambient).r = (pnVar6->ambient).r;
                    (pnVar4->ambient).g = (pnVar6->ambient).g;
                    (pnVar4->ambient).b = (pnVar6->ambient).b;
                    (pnVar4->diffuse).r = (pnVar6->diffuse).r;
                    (pnVar4->diffuse).g = (pnVar6->diffuse).g;
                    (pnVar4->diffuse).b = (pnVar6->diffuse).b;
                    iVar1 = numlights;
                    pnVar2 = (struct nulight_s*)&pnVar4->mtx;
                    pnVar3 = (struct nulight_s*)&pnVar6->mtx;
                } while (iVar5 != 0);
                //NEED CORRECTION
                //(pnVar4->mtx).mtx[0] = (pnVar6->mtx).mtx[0];
                //(pnVar4->mtx).mtx[1] = (pnVar6->mtx).mtx[1];
                //(pnVar4->mtx).mtx[2] = (pnVar6->mtx).mtx[2];
                //(pnVar4->mtx).mtx[3] = (pnVar6->mtx).mtx[3];
                lightsleft = i;
            } while (i < iVar1);
        }
        current_lights_stored = 1;
        lightsleft = num_stored_lights;
        num_stored_lights = num_stored_lights + 1;
    }
    else {
        lightsleft = num_stored_lights + -1;
    }
    return lightsleft;
}


void NuLightSetStoredLights(s32 index)		//TODO

{
    struct nulight_s* pnVar1;
    struct nulight_s* pnVar2;
    struct nulight_s* pnVar3;
    struct nulight_s* pnVar4;
    struct nulight_s* pnVar5;
    s32 iVar6;
    struct nulight_s* pnVar7;

    if (currentlight1 == NULL) {
        currentlight1 = NuLightCreate();
    }
    if (currentlight2 == NULL) {
        currentlight2 = NuLightCreate();
    }
    if (currentlight3 == NULL) {
        currentlight3 = NuLightCreate();
    }
  iVar6 = 0x48;
  pnVar2 = (struct nulight_s *)(StoredLights + index);
  pnVar3 = currentlight1;
  do {
    pnVar5 = pnVar3;
    pnVar4 = pnVar2;
    iVar6 = iVar6 + -0x18;
    (pnVar5->ambient).r = (pnVar4->ambient).r;
    (pnVar5->ambient).g = (pnVar4->ambient).g;
    (pnVar5->ambient).b = (pnVar4->ambient).b;
    (pnVar5->diffuse).r = (pnVar4->diffuse).r;
    (pnVar5->diffuse).g = (pnVar4->diffuse).g;
    (pnVar5->diffuse).b = (pnVar4->diffuse).b;
    pnVar1 = currentlight1;
    pnVar2 = (struct nulight_s *)&pnVar4->mtx;
    pnVar3 = (struct nulight_s *)&pnVar5->mtx;
  } while (iVar6 != 0);
  (pnVar5->mtx)._00 = (pnVar4->mtx)._00;
  (pnVar5->mtx)._01 = (pnVar4->mtx)._01;
  (pnVar5->mtx)._02 = (pnVar4->mtx)._02;
  (pnVar5->mtx)._03 = (pnVar4->mtx)._03;
  NuLightUpdate(pnVar1);
  iVar6 = 0x48;
  pnVar2 = StoredLights[index].light + 1;
  pnVar3 = currentlight2;
  do {
    pnVar7 = pnVar3;
    pnVar5 = pnVar2;
    iVar6 = iVar6 + -0x18;
    (pnVar7->ambient).r = (pnVar5->ambient).r;
    (pnVar7->ambient).g = (pnVar5->ambient).g;
    (pnVar7->ambient).b = (pnVar5->ambient).b;
    (pnVar7->diffuse).r = (pnVar5->diffuse).r;
    (pnVar7->diffuse).g = (pnVar5->diffuse).g;
    (pnVar7->diffuse).b = (pnVar5->diffuse).b;
    pnVar1 = currentlight2;
    pnVar2 = (struct nulight_s *)&pnVar5->mtx;
    pnVar3 = (struct nulight_s *)&pnVar7->mtx;
  } while (iVar6 != 0);
  (pnVar7->mtx)._00 = (pnVar5->mtx)._00;
  (pnVar7->mtx)._01 = (pnVar5->mtx)._01;
  (pnVar7->mtx)._02 = (pnVar5->mtx)._02;
  (pnVar7->mtx)._03 = (pnVar5->mtx)._03;
  NuLightUpdate(pnVar1);
  iVar6 = 0x48;
  pnVar2 = StoredLights[index].light + 2;
  pnVar3 = currentlight3;
  do {
    pnVar7 = pnVar3;
    pnVar5 = pnVar2;
    iVar6 = iVar6 + -0x18;
    (pnVar7->ambient).r = (pnVar5->ambient).r;
    (pnVar7->ambient).g = (pnVar5->ambient).g;
    (pnVar7->ambient).b = (pnVar5->ambient).b;
    (pnVar7->diffuse).r = (pnVar5->diffuse).r;
    (pnVar7->diffuse).g = (pnVar5->diffuse).g;
    (pnVar7->diffuse).b = (pnVar5->diffuse).b;
    pnVar1 = currentlight3;
    pnVar2 = (struct nulight_s *)&pnVar5->mtx;
    pnVar3 = (struct nulight_s *)&pnVar7->mtx;
  } while (iVar6 != 0);
  (pnVar7->mtx)._00 = (pnVar5->mtx)._00;
  (pnVar7->mtx)._01 = (pnVar5->mtx)._01;
  (pnVar7->mtx)._02 = (pnVar5->mtx)._02;
  (pnVar7->mtx)._03 = (pnVar5->mtx)._03;
  NuLightUpdate(pnVar1);
}

void NuLightClearStoredLights(void)

{
    current_lights_stored = 0;
    num_stored_lights = 0;
    return;
}

void NuLightAddSpotXSpanFade(struct nuvec_s* pos, float XSpan, float fade)

{
    buglight_distance = (2.0 / XSpan) * 3.0;
    return;
}

void NuLightMatInit(void)

{
    struct numtl_s* pnVar1;
    //struct numtlattrib_s nVar2;

    pnVar1 = NuMtlCreate(1);
    //nVar2 = pnVar1->attrib;
    NuLightAddMat = pnVar1;
    (pnVar1->diffuse).b = 1.0;
    pnVar1->alpha = 0.999;
    pnVar1->tid = 0;
    pnVar1->alpha_sort = 0x100;
    (pnVar1->diffuse).r = 1.0;
    (pnVar1->diffuse).g = 1.0;
    //pnVar1->attrib = (struct numtlattrib_s)((u32)nVar2 & 0x3c00fff9 | 0x81660002);	//fix!
    //NuMtlUpdate();
    return;
}

int NuLgtRand(void)

{
  //NuLgtSeed = NuLgtSeed * 0x24cd + 1U & 0xffff;
  return NuLgtSeed;
}

void NuLgtArcLaser(s32 type,struct nuvec_s *start,struct nuvec_s *target,struct nuvec_s *lasdir,f32 sizew,f32 sizel,
                  f32 sizewob,f32 arcsize,s32 col)

{
  u32 uVar1;
  f32 fVar2;
  s32 seed;
  u32 uVar3;
  f32 fVar4;
  s32 laserframe;

  laserframe = NuLgtArcLaserFrame;
  seed = NuLgtArcLaserCnt;
  uVar3 = (u32)col >> 0x17 & 0x1fe;
  if (0xff < uVar3) {
    uVar3 = 0xff;
  }
  uVar1 = (col & 0xffU) << 1;
  if (0xff < uVar1) {
    uVar1 = 0xff;
  }
  if (NuLgtArcLaserCnt < 0x10) {
    NuLgtArcLaserData[NuLgtArcLaserCnt].type = (u8)type;
    fVar4 = start->z;
    fVar2 = start->y;
    NuLgtArcLaserData[seed].start.x = start->x;
    NuLgtArcLaserData[seed].start.y = fVar2;
    NuLgtArcLaserData[seed].start.z = fVar4;
    fVar4 = target->z;
    fVar2 = target->y;
    NuLgtArcLaserData[seed].target.x = target->x;
    NuLgtArcLaserData[seed].target.z = fVar4;
    NuLgtArcLaserData[seed].target.y = fVar2;
    fVar4 = lasdir->z;
    fVar2 = lasdir->y;
    NuLgtArcLaserData[seed].lasdir.x = lasdir->x;
    NuLgtArcLaserData[seed].lasdir.z = fVar4;
    NuLgtArcLaserData[seed].lasdir.y = fVar2;
    NuLgtArcLaserData[seed].sizew = sizew;
    NuLgtArcLaserData[seed].sizel = sizel;
    NuLgtArcLaserData[seed].sizewob = sizewob;
    NuLgtArcLaserData[seed].arcsize = arcsize;
    NuLgtArcLaserData[seed].col = uVar3 << 0x18 | 0xffff | uVar1 << 0x10;
    if (((laserframe & 1U) == 0) || (NuLgtArcLaserData[seed].seed == 0)) {
      seed = NuLgtRand();
      NuLgtArcLaserData[NuLgtArcLaserCnt].seed = seed;
    }
    seed = 6;
    do {
      NuLgtRand();
      seed = seed + -1;
    } while (seed != 0);
    NuLgtArcLaserCnt = NuLgtArcLaserCnt + 1;
    return;
  }
  return;
}


void NuLgtSetArcMat(struct numtl_s *mtl,f32 u0,f32 v0,f32 u1,f32 v1)

{
  NuLgtArcMtl = mtl;
  NuLgtArcU0 = u0;
  NuLgtArcV0 = v0;
  NuLgtArcU1 = u1;
  NuLgtArcV1 = v1;
  return;
}


s32 GS_LightEnable(s32 Index, s32 Enable)

{
    GS_LightList[Index].EnableLight = Enable;
    return 0;
}


/*void NuLgtArcLaserDraw(int paused)	//TODO

{
  float fVar1;
  float fVar2;
  float fVar3;
  float *pfVar4;
  numtx_s *b;
  int iVar5;
  uint uVar6;
  nuvec4_s *dest;
  nuvec4_s *dest_00;
  double dVar7;
  double dVar8;
  double dVar9;
  double dVar10;
  double dVar11;
  double dVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  double dVar16;
  double dVar17;
  float fVar18;
  nuvec_s local_1a8;
  float local_198;
  float local_194;
  float local_188;
  float local_184;
  float local_180;
  float local_17c;
  float v0_2;
  float v0;
  float v1_2;
  float v1;
  Nearest_Light_s light;
  double local_b0;

  v0 = NuLgtArcV0;
  v1 = NuLgtArcV1;
  v0_2 = NuLgtArcV0;
  v1_2 = NuLgtArcV1;
  if (paused != 0) {
    NuLgtArcLaserCnt = NuLgtArcLaserOldCnt;
  }
  if ((NuLgtArcMtl == (numtl_s *)0x0) || (NuLgtArcLaserCnt == 0)) {
    NuLgtArcLaserOldCnt = 0;
  }
  else {
    pfVar4 = (float *)NuScratchAlloc32(0xc0);
    b = NuCameraGetVPCSMtx();
    iVar5 = NuRndrBeginScene(1);
    if (iVar5 != 0) {
      iVar5 = 0;
      if (0 < NuLgtArcLaserCnt) {
        dVar9 = 240.0;
        dVar12 = 4503601774854144.0;
        dVar8 = 150.0;
        dVar11 = 1.0;
        dVar10 = 0.5;
        dest_00 = (nuvec4_s *)(pfVar4 + 0x10);
        dest = (nuvec4_s *)(pfVar4 + 0x14);
        do {
          pfVar4[0x10] = NuLgtArcLaserData[iVar5].start.x;
          pfVar4[0x11] = NuLgtArcLaserData[iVar5].start.y;
          fVar18 = NuLgtArcLaserData[iVar5].start.z;
          pfVar4[0x13] = (float)dVar11;
          pfVar4[0x12] = fVar18;
          NuVec4MtxTransformVU0(dest_00,dest_00,b);
          pfVar4[0x14] = NuLgtArcLaserData[iVar5].target.x;
          pfVar4[0x15] = NuLgtArcLaserData[iVar5].target.y;
          fVar18 = NuLgtArcLaserData[iVar5].target.z;
          pfVar4[0x17] = (float)dVar11;
          pfVar4[0x16] = fVar18;
          NuVec4MtxTransformVU0(dest,dest,b);
          dVar7 = (double)pfVar4[0x13];
          if (dVar10 <= dVar7) {
LAB_800b22a4:
            pfVar4[0x18] = NuLgtArcLaserData[iVar5].start.x;
            pfVar4[0x19] = NuLgtArcLaserData[iVar5].start.y;
            fVar18 = NuLgtArcLaserData[iVar5].start.z;
            pfVar4[0x1b] = (float)dVar11;
            pfVar4[0x1a] = fVar18;
LAB_800b22c0:
            fVar18 = pfVar4[0x19] - NuLgtArcLaserData[iVar5].target.y;
            fVar1 = pfVar4[0x18] - NuLgtArcLaserData[iVar5].target.x;
            fVar2 = pfVar4[0x1a] - NuLgtArcLaserData[iVar5].target.z;
            sqrt(fVar2 * fVar2 + fVar1 * fVar1 + fVar18 * fVar18);
            if (0.5 <= pfVar4[0x17]) {
              pfVar4[0x1c] = NuLgtArcLaserData[iVar5].target.x;
              pfVar4[0x1d] = NuLgtArcLaserData[iVar5].target.y;
              fVar18 = NuLgtArcLaserData[iVar5].target.z;
              pfVar4[0x1f] = (float)dVar11;
              pfVar4[0x1e] = fVar18;
            }
            else {
              fVar18 = NuLgtArcLaserData[iVar5].start.x;
              fVar3 = pfVar4[0x13] - 0.5;
              fVar2 = pfVar4[0x13] - pfVar4[0x17];
              pfVar4[0x1c] = fVar18 + ((NuLgtArcLaserData[iVar5].target.x - fVar18) * fVar3) / fVa r2
              ;
              fVar18 = NuLgtArcLaserData[iVar5].start.y;
              pfVar4[0x1d] = fVar18 + ((NuLgtArcLaserData[iVar5].target.y - fVar18) * fVar3) / fVa r2
              ;
              fVar18 = NuLgtArcLaserData[iVar5].start.z;
              fVar1 = NuLgtArcLaserData[iVar5].target.z;
              pfVar4[0x1f] = (float)dVar11;
              pfVar4[0x1e] = fVar18 + ((fVar1 - fVar18) * fVar3) / fVar2;
            }
            NuVec4MtxTransformVU0(dest_00,(nuvec4_s *)(pfVar4 + 0x18),b);
            dVar14 = 1.0;
            NuVec4Scale(1.0 / pfVar4[0x13],dest_00,dest_00);
            NuVec4MtxTransformVU0(dest,(nuvec4_s *)(pfVar4 + 0x1c),b);
            NuVec4Scale((float)(dVar14 / (double)pfVar4[0x17]),dest,dest);
            dVar7 = 0.0;
            local_b0 = (double)CONCAT44(0x43300000,SWIDTH ^ 0x80000000);
            local_1a8.x = (pfVar4[0x15] - pfVar4[0x11]) *
                          (float)((double)(float)(local_b0 - dVar12) / dVar9);
            local_1a8.z = 0.0;
            local_1a8.y = pfVar4[0x10] - pfVar4[0x14];
            NuVecNorm(&local_1a8,&local_1a8);
            local_b0 = (double)CONCAT44(0x43300000,SWIDTH ^ 0x80000000);
            local_1a8.y = local_1a8.y * (float)((double)NuLgtArcLaserData[iVar5].sizew * dVar8);
            local_1a8.x = local_1a8.x *
                          (float)((double)NuLgtArcLaserData[iVar5].sizew *
                                 (double)(float)((double)(float)((double)(float)(local_b0 - dVar12 )
                                                                * dVar8) / dVar9));
            fVar18 = pfVar4[0x1d] - pfVar4[0x19];
            fVar1 = pfVar4[0x1c] - pfVar4[0x18];
            fVar2 = pfVar4[0x1e] - pfVar4[0x1a];
            pfVar4[0x1c] = fVar1;
            pfVar4[0x1d] = fVar18;
            pfVar4[0x1f] = pfVar4[0x1f] - pfVar4[0x1b];
            pfVar4[0x1e] = fVar2;
            fVar18 = sqrt(fVar2 * fVar2 + fVar1 * fVar1 + fVar18 * fVar18);
            dVar13 = (double)fVar18;
            if (dVar7 < dVar13) {
              fVar18 = NuLgtArcLaserData[iVar5].lasdir.x;
              pfVar4[0x28] = fVar18;
              fVar1 = NuLgtArcLaserData[iVar5].lasdir.y;
              pfVar4[0x29] = fVar1;
              fVar2 = NuLgtArcLaserData[iVar5].lasdir.z;
              fVar18 = fVar2 * fVar2 + fVar18 * fVar18 + fVar1 * fVar1;
              dVar15 = (double)fVar18;
              pfVar4[0x2a] = fVar2;
              if (dVar7 < dVar15) {
                fVar18 = NuFsqrt(fVar18);
                dVar15 = (double)(NuLgtArcLaserData[iVar5].arcsize / fVar18);
              }
              pfVar4[0x28] = (float)((double)pfVar4[0x28] * dVar15);
              pfVar4[0x29] = (float)((double)pfVar4[0x29] * dVar15);
              pfVar4[0x2a] = (float)((double)pfVar4[0x2a] * dVar15);
              NuLgtSeed = NuLgtArcLaserData[iVar5].seed;
              dVar13 = (double)(float)((double)NuLgtArcLaserData[iVar5].sizel / dVar13);
              if (dVar14 < dVar13) {
                dVar13 = dVar14;
              }
              local_188 = NuLgtArcU0;
              local_184 = NuLgtArcU1;
              local_180 = NuLgtArcU0;
              local_17c = NuLgtArcU1;
              pfVar4[0x13] = (float)dVar14;
              pfVar4[0x10] = pfVar4[0x18];
              pfVar4[0x11] = pfVar4[0x19];
              pfVar4[0x12] = pfVar4[0x1a];
              dVar15 = dVar14;
              NuVec4MtxTransformVU0(dest_00,dest_00,b);
              dVar16 = (double)(float)(dVar15 / (double)pfVar4[0x13]);
              NuVec4Scale((float)(dVar15 / (double)pfVar4[0x13]),dest_00,dest_00);
              local_198 = (float)((double)local_1a8.x * dVar16);
              local_194 = (float)((double)local_1a8.y * dVar16);
              dVar15 = 50.0;
              pfVar4[0xc] = pfVar4[0x10];
              pfVar4[0xd] = pfVar4[0x11];
              pfVar4[0xe] = pfVar4[0x12];
              pfVar4[4] = pfVar4[0x10];
              pfVar4[5] = pfVar4[0x11];
              pfVar4[6] = pfVar4[0x12];
              do {
                fVar18 = NuLgtArcU1;
                dVar16 = (double)(float)(dVar7 + dVar13);
                if (dVar16 < dVar14) {
                  uVar6 = (int)(dVar7 + dVar13) & 0xffff;
                  pfVar4[0x14] = pfVar4[0x28] * NuTrigTable[uVar6] +
                                 (float)((double)pfVar4[0x1c] * dVar16 + (double)pfVar4[0x18]);
                  pfVar4[0x15] = pfVar4[0x29] * NuTrigTable[uVar6] +
                                 (float)((double)pfVar4[0x1d] * dVar16 + (double)pfVar4[0x19]);
                  pfVar4[0x16] = pfVar4[0x2a] * NuTrigTable[uVar6] +
                                 (float)((double)pfVar4[0x1e] * dVar16 + (double)pfVar4[0x1a]);
                }
                else {
                  fVar1 = NuLgtArcU0 - NuLgtArcU1;
                  pfVar4[0x14] = pfVar4[0x18] + pfVar4[0x1c];
                  pfVar4[0x15] = pfVar4[0x19] + pfVar4[0x1d];
                  local_188 = fVar18 + (float)((double)(fVar1 * (float)(dVar14 - dVar7)) / dVar13) ;
                  pfVar4[0x16] = pfVar4[0x1a] + pfVar4[0x1e];
                  local_180 = local_188;
                }
                pfVar4[0x17] = (float)dVar14;
                NuVec4MtxTransformVU0(dest,dest,b);
                dVar17 = (double)(float)(dVar14 / (double)pfVar4[0x17]);
                NuVec4Scale((float)(dVar14 / (double)pfVar4[0x17]),dest,dest);
                if (((double)(float)(dVar14 - (double)(float)(dVar13 * 1.5)) <= dVar7) ||
                   (NuLgtSeed == 0)) {
                  fVar18 = 0.0;
                  dVar7 = dVar16;
                  fVar1 = fVar18;
                }
                else {
                  uVar6 = NuLgtRand();
                  local_b0 = (double)CONCAT44(0x43300000,(uVar6 & 0xff) - 0x80 ^ 0x80000000);
                  fVar18 = (float)((double)(local_1a8.x * (float)(local_b0 - dVar12) *
                                           NuLgtArcLaserData[iVar5].sizewob) * dVar17);
                  dVar7 = dVar16;
                  fVar1 = (float)((double)(local_1a8.y * (float)(local_b0 - dVar12) *
                                          NuLgtArcLaserData[iVar5].sizewob) * dVar17);
                }
                local_198 = (float)((double)local_1a8.x * dVar17);
                local_194 = (float)((double)local_1a8.y * dVar17);
                fVar2 = (pfVar4[0x14] - local_198) + fVar18;
                *pfVar4 = fVar2;
                fVar3 = (pfVar4[0x15] - local_194) + fVar1;
                pfVar4[1] = fVar3;
                pfVar4[8] = pfVar4[0x14] + local_198 + fVar18;
                pfVar4[10] = pfVar4[0x16];
                pfVar4[2] = pfVar4[0x16];
                pfVar4[9] = pfVar4[0x15] + local_194 + fVar1;
                if (((((-50.0 <= fVar2) || (dVar15 <= (double)pfVar4[4])) &&
                     ((fVar2 <= 700.0 || (pfVar4[4] <= 700.0)))) &&
                    ((-50.0 <= fVar3 || (-50.0 <= pfVar4[5])))) &&
                   ((fVar3 <= 530.0 || (pfVar4[5] <= 530.0)))) {
                  light.ambientdist = (float)NuLgtArcLaserData[iVar5].col;
                  light.AmbIndex = (int)*pfVar4;
                  light.AmbCol.x = pfVar4[1];
                  light.AmbCol.y = pfVar4[2];
                  light.dir1.Direction.x = pfVar4[4];
                  light.dir1.Direction.y = pfVar4[5];
                  light.dir1.Direction.z = pfVar4[6];
                  light.dir2.Index = (int)pfVar4[8];
                  light.dir2.Direction.x = pfVar4[9];
                  light.dir2.Direction.y = pfVar4[10];
                  light.CurLoopIndex = (int)local_188;
                  light.dir1.Index = (int)v0_2;
                  light.dir1.Colour.b = local_184;
                  light.dir1.Distance = v0;
                  light.AmbCol.z = 0.1;
                  light.dir1.Colour.r = 0.1;
                  light.dir2.Direction.z = 0.1;
                  light.dir2.Colour.g = local_180;
                  light.dir2.Colour.b = v1_2;
                  light.dir1.Colour.g = light.ambientdist;
                  light.dir2.Colour.r = light.ambientdist;
                  NuRndrTri2d(&light,NuLgtArcMtl);
                  light.AmbIndex = (int)pfVar4[4];
                  light.AmbCol.x = pfVar4[5];
                  light.AmbCol.y = pfVar4[6];
                  light.dir1.Direction.x = pfVar4[0xc];
                  light.dir1.Direction.y = pfVar4[0xd];
                  light.dir1.Direction.z = pfVar4[0xe];
                  light.dir2.Index = (int)pfVar4[8];
                  light.dir2.Direction.x = pfVar4[9];
                  light.dir2.Direction.y = pfVar4[10];
                  light.CurLoopIndex = (int)local_184;
                  light.dir1.Index = (int)v0;
                  light.dir1.Colour.b = local_17c;
                  light.dir1.Distance = v1;
                  light.dir2.Colour.g = local_180;
                  light.dir2.Colour.b = v1_2;
                  light.AmbCol.z = 0.1;
                  light.dir1.Colour.r = 0.1;
                  light.dir2.Direction.z = 0.1;
                  NuRndrTri2d(&light,NuLgtArcMtl);
                }
                pfVar4[4] = *pfVar4;
                pfVar4[5] = pfVar4[1];
                pfVar4[0xc] = pfVar4[8];
                pfVar4[0xd] = pfVar4[9];
                pfVar4[6] = pfVar4[2];
                pfVar4[0xe] = pfVar4[10];
              } while (dVar7 < dVar14);
            }
          }
          else if (dVar10 <= (double)pfVar4[0x17]) {
            if (dVar10 <= dVar7) goto LAB_800b22a4;
            fVar2 = NuLgtArcLaserData[iVar5].target.x;
            fVar18 = (float)((double)pfVar4[0x17] - dVar10);
            fVar1 = (float)((double)pfVar4[0x17] - dVar7);
            pfVar4[0x18] = fVar2 + ((NuLgtArcLaserData[iVar5].start.x - fVar2) * fVar18) / fVar1;
            fVar2 = NuLgtArcLaserData[iVar5].target.y;
            pfVar4[0x19] = fVar2 + ((NuLgtArcLaserData[iVar5].start.y - fVar2) * fVar18) / fVar1;
            fVar2 = NuLgtArcLaserData[iVar5].target.z;
            fVar3 = NuLgtArcLaserData[iVar5].start.z;
            pfVar4[0x1b] = (float)dVar11;
            pfVar4[0x1a] = fVar2 + ((fVar3 - fVar2) * fVar18) / fVar1;
            goto LAB_800b22c0;
          }
          iVar5 = iVar5 + 1;
        } while (iVar5 < NuLgtArcLaserCnt);
      }
      NuRndrEndScene();
      NuLgtArcLaserOldCnt = NuLgtArcLaserCnt;
      NuLgtArcLaserCnt = 0;
      if (paused == 0) {
        NuLgtArcLaserFrame = NuLgtArcLaserFrame + 1;
      }
    }
    NuScratchRelease();
  }
  return;
}*/
