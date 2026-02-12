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

//NGC MATCH
void NuWindUpdate(struct nuvec_s* pos) {
    s32 lp;
    s32 i;
    s32 plrinrange;
    float d;
    float yd;
    float radius;
    float t10;
    float t12;
    float tx;
    float tz;
    float tx2;
    float tz2;
    struct numtx_s* mtx;
    struct nuvec_s WindVec;
    struct nuwindgrp_s* grp;
    float rot_angle;
    float wind_scale_div;

    grp = NuWindGroup;
    NuWindDir += 0x85;
    NuWindDir2 += 0x179;
    NuWindWave += 0x1f5;
    WindVec.x =
        NuTrigTable[(s32)((NuTrigTable[NuWindDir & 0xffff] * 8192.0f) + 16384.0f) & 0xffff] * 0.75f + NuTrigTable[(NuWindDir2 + 0x4000) & 0xffff] * 0.15f;
    WindVec.z = NuTrigTable[(s32)(NuTrigTable[NuWindDir & 0xffff] * 8192.0f) & 0xffff] * 0.75f - NuTrigTable[NuWindDir2 & 0xffff] * 0.15f;

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
                        < grp->radius)
                    {
                        plrinrange = 1;
                    }
                }     
                if ((grp->collide == 0) || (plrinrange == 0)) {
                    for (lp = 0; lp < grp->objcount; lp++, mtx++) {
                        t10 = ((mtx->_33 * WindVec.x) * (NuTrigTable[(s32)((mtx->_30 + mtx->_32) * 8192.0f + NuWindWave) & 0xffff] * 0.5f + 1.0f)) - mtx->_10;
                        t12 = ((mtx->_33 * WindVec.z) * (NuTrigTable[(s32)(((mtx->_30 + mtx->_32) * 8192.0f + NuWindWave) + 16384.0f) & 0xffff] * 0.5f + 1.0f)) - mtx->_12;

                        mtx->_10 += t10 * 0.2f;
                        mtx->_12 += t12 * 0.2f;

                        d = 1.0f - NuFsqrt(mtx->_10 * mtx->_10 + mtx->_12 * mtx->_12) * 0.35f;

                        if (d < 0.05f) {
                            d = 0.05f;
                        }

                        mtx->_10 *= d;
                        mtx->_12 *= d;
                        mtx->_23 = 1.0f / d;

                        if (d < 0.19f) {
                            d = 0.19f;
                        }

                        mtx->_11 = (mtx->_33 / grp->wind) * d;
                    }
                } else {
                    // Collision loop
                    for (lp = 0; lp < grp->objcount; lp++, mtx++) {
                        yd = pos->y - mtx->_31;

                        if (yd > (grp->height * mtx->_33) / grp->wind) {
                            radius = 0.0f;
                            wind_scale_div = (mtx->_33 * 0.2f) / grp->wind;
                        } else {
                            radius = 0.45f;
                            if (yd < 0.0f) {
                                wind_scale_div = mtx->_33 * 0.2f / grp->wind;
                            } else {
                                wind_scale_div = ((yd * 0.5f + 0.2f) * mtx->_33) / grp->wind;
                            }
                        }

                        if (mtx->_11 > (mtx->_33 * 0.2f) / grp->wind) {
                            mtx->_10 *= mtx->_23;
                            mtx->_12 *= mtx->_23;

                            tx = (mtx->_10 * wind_scale_div + mtx->_30) - pos->x;
                            tz = (mtx->_12 * wind_scale_div + mtx->_32) - pos->z;

                            d = tx * tx + tz * tz;

                            if (d > radius * radius) {
                                t10 = (mtx->_33 * WindVec.x) * (NuTrigTable[(s32)((mtx->_30 + mtx->_32) * 8192.0f + NuWindWave) & 0xffff] * 0.5f + 1.0f);
                                t12 = (mtx->_33 * WindVec.z) * (NuTrigTable[(s32)(((mtx->_30 + mtx->_32) * 8192.0f + NuWindWave) + 16384.0f) & 0xffff] * 0.5f + 1.0f);

                                mtx->_10 += (t10 - mtx->_10) * 0.2f;
                                mtx->_12 += (t12 - mtx->_12) * 0.2f;
                            } else {
                                if (d == 0.0f) {
                                    tx = radius;
                                    tz = 0.0f;
                                } else {
                                    d = NuFsqrt(d);
                                    tx = tx * (radius / d);
                                    tz = tz * (radius / d);
                                }

                                mtx->_10 += ((((tx + pos->x) - mtx->_30) / wind_scale_div) - mtx->_10) * 0.2f;
                                mtx->_12 += ((((tz + pos->z) - mtx->_32) / wind_scale_div) - mtx->_12) * 0.2f;

                                tx2 = (mtx->_33 * WindVec.x) * (NuTrigTable[(s32)((mtx->_30 + mtx->_32) * 8192.0f + NuWindWave) & 0xffff] * 0.5f + 1.0f);
                                tz2 = (mtx->_33 * WindVec.z) * (NuTrigTable[(s32)(((mtx->_30 + mtx->_32) * 8192.0f + NuWindWave) + 16384.0f) & 0xffff] * 0.5f + 1.0f);

                                mtx->_10 += (tx2 - mtx->_10) * 0.05f;
                                mtx->_12 += (tz2 - mtx->_12) * 0.05f;
                                rot_angle = (tx * ((tz2 * wind_scale_div + mtx->_32) - pos->z)
                                             - tz * ((tx2 * wind_scale_div + mtx->_30) - pos->x))
                                    * -2048.0f;

                                mtx->_10 = mtx->_10 * NuTrigTable[(s32)(rot_angle + 16384.0f) & 0xffff]
                                    - mtx->_12 * NuTrigTable[(s32)rot_angle & 0xffff];

                                mtx->_12 = mtx->_10 * NuTrigTable[(s32)rot_angle & 0xffff]
                                    + mtx->_12 * NuTrigTable[(s32)(rot_angle + 16384.0f) & 0xffff];
                            }

                            d = 1.0f - NuFsqrt(mtx->_10 * mtx->_10 + mtx->_12 * mtx->_12) * 0.35f;

                            if (d < 0.05f) {
                                d = 0.05f;
                            }

                            mtx->_10 *= d;
                            mtx->_12 *= d;
                            mtx->_23 = 1.0f / d;

                            if (d < 0.19f) {
                                d = 0.19f;
                            }

                            mtx->_11 = (mtx->_33 / grp->wind) * d;
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