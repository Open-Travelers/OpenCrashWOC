#include "nuwind.h"

extern struct numtx_s ropemat; //nubridge.c

//NGC MATCH
s32 NuWindRand(void) {
  NuWindQS = (NuWindQS * 0x24cd) + 1U & 0xffff;
  return NuWindQS;
}

//NGC MATCH
void NuWindInit(void) {
  NuWindDir = 0;
  NuWindDir2 = 0;
  NuWindWave = 0;
  NuWindGCount = 0;
  NuWindMtxIndex = 0;
}

//NGC MATCH
struct nuwindgrp_s* NuWindAllocateGrp(void) {
  if (NuWindGCount < 0x40) {
      NuWindGCount++;
      return (NuWindGCount * 0xa) + &ropemat._12; //NuWindMtxs??
  }
  return 0;
}

//NGC MATCH
void NuWindFreeGrp(struct nuwindgrp_s *grp) {
  if (grp != NULL) {
    NuWindGCount--;
  }
}

//NGC MATCH
struct numtx_s * NuWindAllocMtxs(s32 count) {
  if (count + NuWindMtxIndex < 0x200) {
      NuWindMtxIndex += count;
      return &NuWindMtxs[(NuWindMtxIndex - count)];
  }
  return NULL;
}

//NGC MATCH
void NuWindFreeMtxs(struct numtx_s *mtx,s32 count) {
   if (mtx != NULL) {
     NuWindMtxIndex -= count;
   }
}

//NGC MATCH
void NuWindDraw(struct nugscn_s *scn) {
    s32 lp;
    s32 i;
    float t2;
    float t3;
    struct nuwindgrp_s *grp;
    struct numtx_s *mtx;
    struct nugobj_s *gobj;
    char pad[25];
    
    grp = &NuWindGroup[0];
    for (lp = 0; lp < NuWindGCount; lp++) {
            if (grp->inrange != '\0') {
                grp->onscreen = '\0';
                mtx = grp->mtx;
                gobj = scn->gobjs[grp->instance->objid];
                    for (i = 0; i < grp->objcount; i++) {
                        t2 = mtx->_23;
                        t3 = mtx->_33;
                        mtx->_23 = 0.0f;
                        mtx->_33 = 1.0f;
                        if (NuRndrGrassGobj(gobj,mtx,NULL) != 0) {
                            grp->onscreen = '\x01';
                        }
                        mtx->_23 = t2;
                        mtx->_33 = t3;
                        mtx++;
                    }
            }
            grp++;
    }
    return;
}

//NGC 98%
s32* NuWindCreate(struct nuinstance_s* instance, struct nuvec4_s* pos, short count, 
                    float wind, float height, s32 collide) {
    struct nuwindgrp_s *grp;
    struct numtx_s *mtx;
    float minx;
    float maxx;
    float miny;
    float maxy;
    float minz;
    float maxz;
    s32 lp;
    
    lp = count;
    minx = height;
    grp = (struct nuwindgrp_s *)NuWindAllocateGrp();
    mtx = NuWindAllocMtxs(count);
    if ((grp != NULL) && (mtx != NULL)) {
        minx = -10000000.0f;
        minz = -10000000.0f; 
        miny = -10000000.0f;
        maxy = 10000000.0f;
        maxz =  10000000.0f;
        grp->height = height;
        grp->instance = instance;
        grp->collide = collide;
        grp->mtx = mtx;
        maxx =  10000000.0f;
        grp->objcount = count;
        grp->wind = wind;
        for (lp = 0; lp < count; lp++) {
                NuMtxSetIdentity(&mtx[lp]);
                NuMtxPreRotateY(&mtx[lp],NuWindRand());
                mtx[lp]._11 = pos[lp].w;
                mtx[lp]._00 = mtx[lp]._00 * pos[lp].w;
                mtx[lp]._02 = mtx[lp]._02 * pos[lp].w;
                mtx[lp]._20 = mtx[lp]._20 * pos[lp].w;
                mtx[lp]._22 = mtx[lp]._22 * pos[lp].w;
                mtx[lp]._30 = pos[lp].x;
                mtx[lp]._31 = pos[lp].y;
                mtx[lp]._32 = pos[lp].z;
                mtx[lp]._33 = (wind * pos[lp].w);
                if (pos[lp].x < minx) {
                    minx = pos[lp].x;
                }
                if (pos[lp].y < miny) {
                    miny = pos[lp].y;
                }
                if (pos[lp].z < minz) {
                    minz = pos[lp].z;
                }
                if (pos[lp].x > maxx) {
                    maxx = pos[lp].x;
                }
                if (pos[lp].y > maxy) {
                    maxy = pos[lp].y;
                }
                if (pos[lp].z > maxz) {
                    maxz = pos[lp].z;
                }
        }
        grp->center.x = (maxx  + minx)  / 2;
        grp->center.y = (maxy + miny) /2;
        grp->center.z = (maxz + minz) /2;
        grp->radius =  (grp->center.x * grp->center.x) + (grp->center.y * grp->center.y) + (grp->center.z * grp->center.z) + 1.0f;
        return grp;
    }
    else {
        NuWindFreeGrp(grp);
        NuWindFreeMtxs(mtx,count);
        return NULL;
    }
}

//73% NGC
void NuWindUpdate(struct nuvec_s *pos) {
  float fVar1; //nuvec_s ??
  float fVar2; //nuvec_s ??
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  s32 i;
  s32 lp;
  float dVar14;
  float dVar15;
  float dVar16;
  float dVar17;
  float dVar19;
  float dVar20; //temp
  float dVar26;
  float dVar27;
  float dVar29;
  float dVar31;
  float fVar32; //nuvec_s ??
  struct numtx_s *mtx;
  struct nuwindgrp_s *grp;
  
  NuWindDir = NuWindDir + 0x85;
  fVar4 = NuTrigTable[NuWindDir * 4 & 0x3fffcU] * 8192.0f;
  fVar3 = NuTrigTable[(s32)(fVar4 + 16384.0f) & 0xffff] * 0.75f + NuTrigTable[(NuWindDir2 + 0x4179) & 0x3fffcU] * 0.15f;
  fVar4 = NuTrigTable[(s32)fVar4 & 0xffff] * 0.75f - NuTrigTable[NuWindDir2 * 4 & 0x3fffcU] * 0.15f;
  NuWindWave = NuWindWave + 0x1f5;
  NuWindDir2 = NuWindDir2 + 0x179;
    grp = &NuWindGroup[i];
    for (i = 0; i < NuWindGCount; i++) {
      fVar32 = (grp->center).y;
      fVar1 = (grp->center).x;
      fVar5 = fVar32 - global_camera.mtx._31;
      fVar2 = (grp->center).z;
      fVar6 = fVar1 - global_camera.mtx._30;
      fVar7 = fVar2 - global_camera.mtx._32;
      if (global_camera.farclip * global_camera.farclip + grp->radius < fVar7 * fVar7 + fVar6 * fVar6 + fVar5 * fVar5) {
            grp->inrange = '\x01';
            if (grp->onscreen != '\0') {
              mtx = grp->mtx;
              if ((grp->collide == 0) || (fVar32 = fVar32 - pos->y, fVar1 = fVar1 - pos->x, 
                  fVar2 = fVar2 - pos->z, grp->radius < fVar2 * fVar2 + fVar1 * fVar1 + fVar32 * fVar32)) {
                  for (lp = 0; lp < grp->objcount; lp++) {
                    dVar15 = (mtx->_30 + mtx->_32) * 8192.0f + NuWindWave;
                    fVar1 = NuTrigTable[(s32)dVar15 & 0xffff];
                    fVar32 = (((mtx->_33 * fVar4) *(NuTrigTable[(s32)(dVar15 + 16384.0f) & 0xffff] * 0.5f + 1.0f) - mtx->_12) * 0.2f + mtx->_12);
                    mtx->_12 = fVar32;
                    fVar1 = (((mtx->_33 * fVar3) * (fVar1 * 0.5f + 1.0f) - mtx->_10) * 0.2f + mtx->_10);
                    mtx->_10 = fVar1;
                    fVar32 = NuFsqrt(fVar1 * fVar1 + fVar32 * fVar32);
                    dVar15 = (1.0f - (fVar32 * 0.34999999f));
                    if (dVar15 < 0.05f) {
                      dVar15 = 0.05f;
                    }
                    mtx->_23 = (1.0f / dVar15);
                    mtx->_10 = (mtx->_10 * dVar15);
                    mtx->_12 = (mtx->_12 * dVar15);
                    if (dVar15 < 0.19f) {
                      dVar15 = 0.19f;
                    }
                    mtx->_11 = ((mtx->_33 / grp->wind) * dVar15);
                    mtx = mtx + 1;
                  }
              }
              else {
                  for (lp = 0; lp < grp->objcount; lp++) {
                    dVar14 = mtx->_33;
                    fVar32 = grp->wind;
                    dVar19 = (pos->y - mtx->_31);
                    dVar26 = 0.2f;
                    if (dVar19 > ((grp->height * dVar14) / fVar32)) {
                      fVar1 = 0.0f;
                    }
                    else {
                      fVar1 = 0.44999999f;
                      if (0.0f > dVar19) {
                        dVar26 = (dVar19 * 0.5f + 0.2f);
                      }
                    }
                    dVar19 = ((dVar14 * dVar26) / fVar32);
                    dVar26 = fVar1;
                    fVar32 = (dVar14 * 0.2f) / fVar32;
                    if (fVar32 > mtx->_11) {
                      fVar32 = mtx->_10 * mtx->_23;
                      dVar16 = fVar32;
                      fVar1 = mtx->_12 * mtx->_23;
                      dVar17 = fVar1;
                      mtx->_10 = fVar32;
                      mtx->_12 = fVar1;
                      dVar31 = ((dVar17 * dVar19 + mtx->_32) - pos->z);
                      dVar29 = ((dVar16 * dVar19 + mtx->_30) - pos->x);
                      fVar32 = (dVar29 * dVar29 + (dVar31 * dVar31));
                      if (fVar32 > (dVar26 * dVar26)) {
                        dVar26 = (mtx->_30 + mtx->_32) * 8192.0f  + NuWindWave;
                        fVar32 = NuTrigTable[(s32)dVar26 & 0xffff];
                        mtx->_12 = (((dVar14 * fVar4)  * (NuTrigTable[(s32)(dVar26 + 16384.0f) & 0xffff] * 0.5f + 1.0f) - dVar17) * 0.2f + dVar17);
                        mtx->_10 = (((dVar14 * fVar3)  * (fVar32 * 0.5f + 1.0f) - dVar16) *  0.2f + dVar16);
                      }
                      else {
                        fVar1 = 0.0f;
                        if (fVar32 == 0.0f) {
                          dVar31 = fVar32;
                          fVar32 = NuFsqrt(fVar32);
                          fVar1 = (dVar31 * (dVar26 / fVar32));
                          dVar26 = (dVar29 * (dVar26 / fVar32));
                        }
                        dVar17 = mtx->_30;
                        dVar29 = mtx->_32;
                        fVar32 = (((((dVar26 + pos->x) - dVar17) / dVar19) -  mtx->_10) * 0.2f + mtx->_10);
                        mtx->_10 = fVar32;
                        dVar14 = (dVar17 + dVar29) * 8192.0f + NuWindWave;
                        fVar32 = (((((fVar1 + pos->z) - dVar29) / dVar19) - mtx->_12) * 0.2f + mtx->_12);
                        dVar20 = fVar32;
                        mtx->_12 = fVar32;
                        dVar16 = (mtx->_33 * fVar3 * (NuTrigTable[(s32)dVar14 & 0xffff] * 0.5f + 1.0f));
                        dVar14 = (mtx->_33 * fVar4 * (NuTrigTable[(s32)(dVar14 + 16384.0f) & 0xffff] * 0.5f + 1.0f));
                        fVar32 = ((dVar14 - dVar20) * 0.05f + dVar20);
                        fVar2 = ((dVar16 - dVar31) * 0.05f + dVar31);
                        mtx->_12 = fVar32;
                        mtx->_10 = fVar2;
                        dVar26 = (dVar26 * ((dVar14 * dVar19 + dVar29) - pos->z) - (fVar1 * ((dVar16 * dVar19 + dVar17) -
                                                                 pos->x))) * -2048.0f;
                        fVar1 = fVar2 * NuTrigTable[(s32)(dVar26 + 16384.0f) & 0xffff] - fVar32 * NuTrigTable[(s32)dVar26 & 0xffff];
                        mtx->_10 = fVar1;
                        mtx->_12 = mtx->_10 * NuTrigTable[(s32)dVar26 & 0xffff] + fVar32 * NuTrigTable[(s32)(dVar26 + 16384.0f) & 0xffff];
                      }
                      fVar32 = NuFsqrt(mtx->_10 * mtx->_10 + mtx->_12 * mtx->_12);
                      fVar32 = 1.0f - (fVar32 * 0.34999999f);
                      if (fVar32 < 0.05f) {
                        fVar32 = 0.05f;
                      }
                      mtx->_23 = 1.0f / fVar32;
                      mtx->_10 = mtx->_10 * fVar32;
                      mtx->_12 = mtx->_12 * fVar32;
                      if (fVar32 < 0.19f) {
                        fVar32 = 0.19f;
                      }
                      fVar32 = (mtx->_33 / grp->wind) * fVar32;
                    }
                    mtx->_11 = fVar32;
                    //mtx++;
                  }
              }
            }
      }
      else {
        grp->inrange = 0;
      }
    }
  return;
}