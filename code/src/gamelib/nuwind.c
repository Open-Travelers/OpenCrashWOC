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
      return &NuWindGroup[NuWindGCount]; //(NuWindGCount * 0xa) + &ropemat._12
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

//NGC MATCH // PS2 MATCH
s32* NuWindCreate(struct nuinstance_s* instance, struct nuvec4_s* pos, short count, 
                    float wind, float height, s32 collide) {
    struct nuwindgrp_s *grp;
    struct numtx_s *mtx;
    s32 lp;
    float minx;
    float maxx;
    float miny;
    float maxy;
    float minz;
    float maxz;
    
    lp = count;
    minx = height;
    grp = (struct nuwindgrp_s *)NuWindAllocateGrp();
    mtx = NuWindAllocMtxs(count);

    if ((grp != NULL) && (mtx != NULL)) {
        minx = minz = miny = -10000000.0f;
        maxx = maxy = maxz =  10000000.0f;
        grp->instance = instance;
        grp->height = height;
        grp->collide = collide;
        grp->mtx = mtx;
        maxx =  10000000.0f;
        grp->objcount = count;
        grp->wind = wind;
        for (lp = 0; lp < count; lp++,mtx++) {
                NuMtxSetIdentity(mtx);
                NuMtxPreRotateY(mtx,NuWindRand());
                mtx->_11 = pos[lp].w;
                mtx->_00 = mtx->_00 * pos[lp].w;
                mtx->_02 = mtx->_02 * pos[lp].w;
                mtx->_20 = mtx->_20 * pos[lp].w;
                mtx->_22 = mtx->_22 * pos[lp].w;
                mtx->_30 = pos[lp].x;
                mtx->_31 = pos[lp].y;
                mtx->_32 = pos[lp].z;
                mtx->_33 = (wind * pos[lp].w);
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
        return (s32*)grp;
    }
    NuWindFreeGrp(grp);
    NuWindFreeMtxs(mtx,count);
    return NULL;
}

//92% NGC
void NuWindUpdate(struct nuvec_s* pos) {
    s32 lp;
    s32 i;
    s32 plrinrange;
    float d;
    float fVar1;
    float fVar4;
    float fVar5;
    float dVar14;
    float dVar15;
    float dVar16;
    float dVar17;
    float dVar18;
    float dVar19;
    float dVar26;
    float dVar29;
    float dVar31;
    float fVar32;
    struct numtx_s* mtx;
    struct nuvec_s WindVec;
    struct nuwindgrp_s* grp;

    grp = NuWindGroup;
    NuWindDir += 0x85;
    fVar4 = NuTrigTable[NuWindDir & 0xffff] * 8192.0f;
    NuWindDir2 += 0x179;
    NuWindWave += 0x1f5;
    WindVec.x = NuTrigTable[(s32)(fVar4 + 16384.0f) & 0xffff] * 0.75f + NuTrigTable[(NuWindDir2 + 0x4000) & 0xffff] * 0.15f;
    WindVec.z = NuTrigTable[(s32)fVar4 & 0xffff] * 0.75f - NuTrigTable[NuWindDir2 & 0xffff] * 0.15f;
    for (i = 0; i < NuWindGCount; i++, grp++) {
        if (((grp->center.x - global_camera.mtx._30) * (grp->center.x - global_camera.mtx._30)
               + (grp->center.y - global_camera.mtx._31) * (grp->center.y - global_camera.mtx._31)
               + (grp->center.z - global_camera.mtx._32) * (grp->center.z - global_camera.mtx._32))
            < global_camera.farclip * global_camera.farclip + grp->radius)
        {
            grp->inrange = 1;
            if (grp->onscreen != 0) {
                mtx = grp->mtx;
                if (grp->collide != 0) {
                    plrinrange = 0;
                   if (((grp->center.x - pos->x) * (grp->center.x - pos->x)
                        + (grp->center.y - pos->y) * (grp->center.y - pos->y)
                        + (grp->center.z - pos->z) * (grp->center.z - pos->z))
                        < grp->radius) {
                            plrinrange = 1;
                        }
                    
                } else {
                    goto jmp_1;
                }
                if (plrinrange == 0) {
jmp_1:
                    for (lp = 0; lp < grp->objcount; lp++) {
                        dVar18 = (mtx->_30 + mtx->_32) * 8192.0f + NuWindWave;
                        //dVar14 = NuTrigTable[(s32)dVar15 & 0xffff];
                        dVar16 = ((mtx->_33 * WindVec.x) * (NuTrigTable[(s32)dVar18 & 0xffff] * 0.5f + 1.0f));
                        dVar26 = ((mtx->_33 * WindVec.z) * (NuTrigTable[(s32)(dVar18 + 16384.0f) & 0xffff] * 0.5f + 1.0f));
                        mtx->_10 += (dVar16 - mtx->_10) * 0.2f;
                        mtx->_12 += (dVar26 - mtx->_12) * 0.2f;
                        dVar15 = (1.0f - (NuFsqrt(mtx->_10 * mtx->_10 + mtx->_12 * mtx->_12) * 0.34999999f));
                        if (dVar15 < 0.05f) {
                            dVar15 = 0.05f;
                        }
                        mtx->_10 *= dVar15;
                        mtx->_12 *= dVar15;
                        mtx->_23 = (1.0f / dVar15);
                        if (dVar15 < 0.19f) {
                            dVar15 = 0.19f;
                        }
                        mtx->_11 = ((mtx->_33 / grp->wind) * dVar15);
                        mtx++;
                    }
                } else {
                    for (lp = 0; lp < grp->objcount; lp++, mtx++) {
                        // dVar14 = mtx->_33;
                        //fVar32 = grp->wind;
                        dVar19 = (pos->y - mtx->_31);
                        //dVar26 = 0.2f;
                        if (dVar19 > ((grp->height * mtx->_33) / grp->wind)) {
                            fVar1 = 0.0f;
                            dVar26 = ((mtx->_33 * 0.2f));
                        } else {
                            fVar1 = 0.44999999f;
                            if (dVar19 < 0.0f) {
                                dVar26 = ((mtx->_33 * 0.2f));
                            } else {
                                dVar26 *= (dVar19 * 0.5f + 0.2f);
                            } 
                            dVar19 = (dVar26 / grp->wind);
                        }
                        //dVar26 = fVar1;
                        //fVar32 = (mtx->_33 * 0.2f) / grp->wind;
                        if (mtx->_11 > (mtx->_33 * 0.2f) / grp->wind) {
                            mtx->_10 *= mtx->_23;
                            mtx->_12 *= mtx->_23;
                            dVar29 = ((mtx->_10 * dVar19 + mtx->_30) - pos->x);
                            dVar31 = ((mtx->_12 * dVar19 + mtx->_32) - pos->z);
                            fVar32 = (dVar29 * dVar29 + (dVar31 * dVar31));
                            if (fVar32 > (fVar1 * fVar1)) {
                                dVar26 = (mtx->_30 + mtx->_32) * 8192.0f + NuWindWave;
                                fVar32 = NuTrigTable[(s32)dVar26 & 0xffff] * 0.5f;
                                dVar16 = ((mtx->_33 * WindVec.x) * (fVar32 + 1.0f));
                                dVar14 =
                                    (mtx->_33 * WindVec.z * (NuTrigTable[(s32)(dVar26 + 16384.0f) & 0xffff] * 0.5f + 1.0f));
                                mtx->_10 += (dVar16 - mtx->_10) * 0.2f;
                                mtx->_12 += (dVar14 - mtx->_12) * 0.2f;
                            } else {
                                if (fVar32 == 0.0f) {
                                    fVar1 = 0.0f;
                                    dVar26 = 0.0f;
                                } else {
                                    dVar17 = NuFsqrt(fVar32);
                                    fVar1 = (fVar1 * (dVar26 / dVar17));
                                    dVar26 = (dVar29 * (dVar26 / dVar17));
                                }
                                mtx->_10 += ((((fVar1 + pos->x) - mtx->_30) / dVar19) - mtx->_10) * 0.2f;
                                // mtx->_10 = fVar32;
                                mtx->_12 += ((((fVar1 + pos->z) - mtx->_32) / dVar19) - mtx->_12) * 0.2f;
                                dVar14 = (mtx->_30 + mtx->_32) * 8192.0f + NuWindWave;
                                // dVar20 = fVar32;
                                // mtx->_12 = fVar32;
                                dVar16 = (mtx->_33 * WindVec.z * (NuTrigTable[(s32)dVar14 & 0xffff] * 0.5f + 1.0f));
                                dVar14 =
                                    (mtx->_33 * WindVec.z * (NuTrigTable[(s32)(dVar14 + 16384.0f) & 0xffff] * 0.5f + 1.0f));
                                // fVar32 = ((dVar14 - mtx->_12) * 0.05f + mtx->_12);
                                mtx->_10 += (dVar16 - mtx->_10) * 0.05f;
                                mtx->_12 += (dVar14 - mtx->_12) * 0.05f;

                                dVar26 = (dVar26 * ((dVar14 * dVar19 + mtx->_32) - pos->z)
                                          - (fVar1 * ((dVar16 * dVar19 + mtx->_30) - pos->x)))
                                    * -2048.0f;
                                mtx->_10 = mtx->_10 * NuTrigTable[(s32)(dVar26 + 16384.0f) & 0xffff]
                                    - mtx->_12 * NuTrigTable[(s32)dVar26 & 0xffff];

                                mtx->_12 = mtx->_10 * NuTrigTable[(s32)dVar26 & 0xffff]
                                    + mtx->_12 * NuTrigTable[(s32)(dVar26 + 16384.0f) & 0xffff];
                            }
                            fVar32 = 1.0f - (NuFsqrt(mtx->_10 * mtx->_10 + mtx->_12 * mtx->_12) * 0.34999999f);
                            if (fVar32 < 0.05f) {
                                fVar32 = 0.05f;
                            }
                            mtx->_10 *= fVar32;
                            mtx->_12 *= fVar32;
                            mtx->_23 = 1.0f / fVar32;
                            if (fVar32 < 0.19f) {
                                fVar32 = 0.19f;
                            }
                            mtx->_11 = (mtx->_33 / grp->wind) * fVar32;
                        } else {
                            mtx->_11 = (mtx->_33 * 0.2f) / grp->wind;
                        }
                    }
                }
            }
        } else {
            grp->inrange = 0;
        }
    }
}