#include "gamecode/camera.h"

/*
	MoveGameCamera	    77.48%*
*/

//MATCH NGC
void ResetGameCameras(struct cammtx_s *Gamecam,s32 n) {
       while (n > 0) {
            Gamecam->mode = -1;
            Gamecam->judder = 0.0f;
            Gamecam->blend_time = 0.0f;
            Gamecam->blend_duration = 0.0f;
            Gamecam->distance = 0.0f;
            Gamecam->ahead = 0.0f;
            Gamecam->vertical = '\0';
            Gamecam++;
            n--;
        }
    return;
}

//MATCH NGC
void JudderGameCamera(struct cammtx_s *cam,float time,struct nuvec_s *pos) {
    float d;

    if (time > cam->judder) {
        if (pos != NULL) {
            d = NuVecDist(&(player->obj).pos,pos,NULL);
            if (d < 10.0f) {
                cam->judder = time * ((10.0f - d) / 10.0f);
            }
        }
        else {
            cam->judder = time;
        }
    }
    return;
}

//MATCH NGC
void BlendGameCamera(struct cammtx_s *cam,float time) {
    cam->oldpos = cam->newpos;
    cam->old_xrot = cam->new_xrot;
    cam->old_yrot = cam->new_yrot;
    cam->old_zrot = cam->new_zrot;
    cam->blend_time = 0.0f;
    cam->blend_duration = time;
    return;
}

//NGC MATCH
void InitRails(void) {
    struct rail_s* rail;
    s32 i;
    s16 camlen;
    s16 leftlen; 
    s16 rightlen;
    
    nRAILS = 0;
    if (world_scene[0] != NULL) {
        rail = Rail;
        for (i = 0; i < 8;  i++, rail++) {
            rail->in_distance = 25.0f;
            rail->out_distance = 25.0f;
            rail->type = -1;
            rail->pINPLAT = NULL;
            rail->pINCAM = NULL;
            rail->pLEFT = NULL;
            rail->pCAM = NULL;
            rail->pRIGHT = NULL;
            rail->pOUTCAM = NULL;
            rail->pOUTPLAT = NULL;
            rail->circuit = 0;
            sprintf(tbuf,"%s%s",tCamRail,tRailExt[i]);
            rail->pCAM = NuSplineFind(world_scene[0],tbuf);
            if (rail->pCAM != NULL) {
                camlen = rail->pCAM->len;
                if (camlen > 1) {
                    sprintf(tbuf,"%s%s",tLeftRail,tRailExt[i]);
                    rail->pLEFT = NuSplineFind(world_scene[0],tbuf);
                    if (rail->pLEFT != NULL) {
                        leftlen = rail->pLEFT->len;
                        if (leftlen == camlen) {
                            sprintf(tbuf,"%s%s",tRightRail,tRailExt[i]) ;
                            rail->pRIGHT = NuSplineFind(world_scene[0],tbuf);
                            if (rail->pRIGHT != NULL) {
                                rightlen = rail->pRIGHT->len; 
                                if (rightlen == leftlen) {
                                    rail->edges = rightlen - 1;
                                    sprintf(tbuf,"%s%s",tInPlatRail,tRailExt[i]);
                                    rail->pINPLAT = NuSplineFind(world_scene[0],tbuf);
                                    if (rail->pINPLAT != NULL) {
                                        rail->in_distance = SplineDistance(rail->pINPLAT);
                                    }
                                    sprintf(tbuf,"%s%s",tInCamRail,tRailExt[i]);
                                    rail->pINCAM = NuSplineFind(world_scene[0],tbuf);
                                    sprintf(tbuf,"%s%s",tOutPlatRail,tRailExt[i]);
                                    rail->pOUTPLAT = NuSplineFind(world_scene[0],tbuf);;
                                    if (rail->pOUTPLAT != NULL) {
                                        rail->out_distance = SplineDistance(rail->pOUTPLAT);
                                    }
                                    sprintf(tbuf,"%s%s",tOutCamRail,tRailExt[i]);
                                    rail->pOUTCAM = NuSplineFind(world_scene[0],tbuf);
                                    if (strcmp(tRailExt[i],"bonus") == 0) {
                                        rail->type = 1;
                                    }
                                    else {
                                        if (strcmp(tRailExt[i],"death") == 0) {
                                            rail->type = 2;
                                        }
                                        else {
                                            if (strcmp(tRailExt[i],"gem") == 0) {
                                                rail->type = 3;
                                            }
                                            else {
                                                rail->type = 0;
                                            }
                                        }
                                    }
                                    nRAILS++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return;
}

//NGC MATCH
float BestRailPosition(struct nuvec_s* pos, struct RPos_s* rpos, s32 iRAIL, s32 iALONG) {
    struct nuvec_s v;
    struct nuvec_s v0;
    struct nuvec_s v1;
    struct nuvec_s v2;
    struct nuvec_s v3;
    struct nuvec_s* p0;
    struct nuvec_s* p1;
    struct nuvec_s* p3;
    struct nuvec_s* p2;
    s32 iVar1;
    s32 iVar2;
    s32 iVar3;
    s32 iVar4;
    s32 iVar5;
    s32 iVar6;
    s32 iVar7;
    s32 iVar8;
    s32 bVar2;
    float d;
    float dbest;
    float y;
    struct rail_s* rail;

    bVar2 = 0;
    if ((Level == 6) || (Level == 0x22)) {
        bVar2 = 1;
    }
    if (bVar2 != 0) {
        v.x = -pos->y;
        v.y = pos->x;
        v.z = pos->z;
    } else {
        v = *pos;
    }
    rail = &Rail[iRAIL];
    // dbest = 0.0f;
    rpos->iRAIL = -1;
    rpos->iALONG = -1;
    rpos->fALONG = 0.0f;
    rpos->fACROSS = 0.0f;
    if (rail->type == -1) {
        return 0.0f;
    }

    if (iALONG == -1) {
        iVar3 = (s32)rail->edges / 2;
    } else {
        iVar3 = iALONG;
    }
    iVar8 = iVar3 + 1;
    iVar7 = iVar3 - 1;
    iVar4 = 0;
    iVar5 = 0;
Loop:
    if (iVar4 == 0) {
        iVar1 = iVar3;
    } else if (iVar4 == 1) {
        iVar1 = iVar8;
        iVar8++;
    } else {
         iVar1 = iVar7;
        iVar7--;
    }
    if (iVar1 >= 0 && iVar1 < rail->edges) {
        iVar2 = iVar1 + 1;
        if ((iVar2 == rail->edges) && (rail->circuit != 0)) {
            iVar2 = 0;
        }
        p0 =  (struct nuvec_s*) (rail->pLEFT->pts + (iVar1 * rail->pLEFT->ptsize));
        p1 =  (struct nuvec_s*) (rail->pLEFT->pts + (iVar2 * rail->pLEFT->ptsize));
        p2 = (struct nuvec_s*) (rail->pRIGHT->pts + (iVar2 * rail->pRIGHT->ptsize));
        p3 = (struct nuvec_s*) (rail->pRIGHT->pts + (iVar1 * rail->pRIGHT->ptsize));
        if (bVar2 != 0) {
            v0.x = -p0->y;
             v0.y = p0->x;
             v0.z = p0->z;

               v1.x = -p1->y;
               v1.y = p1->x;
               v1.z = p1->z;

               v2.x = -p2->y;
               v2.y = p2->x;
               v2.z = p2->z;

              v3.x = -p3->y;
              v3.y = p3->x;
              v3.z = p3->z;
          } else {
               v0 = *p0;
               v1 = *p1;
              v2 = *p2;
              v3 = *p3;
          }
          if ((((0.0f <= (v.x - v0.x) * (v1.z - v0.z)
                     + (v.z - v0.z) * (v0.x - v1.x))
                && (0.0f <= (v.x - v1.x) * (v2.z - v1.z)
                        + (v.z - v1.z) * (v1.x - v2.x)))
                && (0.0f <= (v.x - v2.x) * (v3.z - v2.z)
                        + (v.z - v2.z) * (v2.x - v3.x)))
             && (0.0f <= (v.x - v3.x) * (v0.z - v3.z)
                    + (v.z - v3.z) * (v3.x - v0.x)))
        {
            d = NuFabs(((v0.y + v1.y + v2.y + v3.y) * 0.25f - v.y));
            if (iALONG == -1) {
                if  (rpos->iALONG == -1 || d < dbest) {
                    dbest = d;
                    iVar6 = 1;
                }
             } else {
                dbest = d;
                 iVar6 = 2;
            }
            if (iVar6 != 0) {
                rpos->iALONG = iVar1;
                rpos->iRAIL = iRAIL;
                rpos->i1 = iVar2;
                rpos->i2 = iVar2 + 1;
                 if ((rpos->i2 == rail->edges) && (rail->circuit != 0)) {
                    rpos->i2 = 0;
                }
                rpos->fALONG = RatioBetweenEdges(&v, &v3, &v0, &v2, &v1);
                rpos->fACROSS = RatioBetweenEdges(&v, &v0, &v1, &v3, &v2);
                if (iVar6 == 2) {
                    goto Finish;
                }
            }
        }
    } else {
        iVar5 |= iVar4;
    }
    if (iVar5 == 3) {
        goto Finish;
    }
    iVar4 = (iVar4 == 1) ? 2 : 1;
    goto Loop;

Finish:
    temp_iRAIL = rpos->iRAIL;
    temp_iALONG = rpos->iALONG;
    temp_fALONG = rpos->fALONG;
    temp_fACROSS = rpos->fACROSS;

    if ((rpos->iRAIL != -1) && (temp_iALONG != -1)) {
        rpos->vertical = 0;
        if ((rpos->i2 != rpos->i1) && (rpos->i2 < rail->edges) && (bVar2 == 0)) {
            p0 = (struct nuvec_s*) (rail->pLEFT->pts + (rpos->i1 * rail->pLEFT->ptsize));
            p1 = (struct nuvec_s*) (rail->pLEFT->pts + (rpos->i2 * rail->pLEFT->ptsize));
            if ((p0->x == p1->x) && (p0->z == p1->z)) {
                p0 = (struct nuvec_s*) (rail->pRIGHT->pts + (rpos->i1 * rail->pRIGHT->ptsize));
                p1 = (struct nuvec_s*) (rail->pRIGHT->pts + (rpos->i2 * rail->pRIGHT->ptsize));
                if ((p0->x == p1->x) && (p0->z == p1->z)) {
                    rpos->vertical = 1;
                }
            }
        }
        if (bVar2 != 0) {
            RailInfo(rpos, &rpos->pos, NULL, &rpos->cam_angle, NULL);
        } else {
            RailInfo(rpos, &rpos->pos, &rpos->angle, &rpos->cam_angle, &rpos->mode);
        }
    }
    return dbest;
}

//MATCH NGC
void ComplexRailPosition(struct nuvec_s *pos,s32 iRAIL,s32 iALONG,struct RPos_s *rpos,s32 set) {

    struct RPos_s *list;
    s32 i;
    s32 bonus;
    s32 death;
    s32 gempath;
    s32 count;
    struct RPos_s *best;
    float d;
    float dbest;
    
    if (set != 0) {
        list = cRPos;
    }
    else {
        list = temp_cRPos;
    }
    if (set != 0) {
        bonus = (Bonus - 1U < 3);
        death = (Death - 1U < 3);
        gempath = (GemPath - 1U < 3);
    }
    count = 0;
    best = NULL;
    if (iRAIL == -1) {
        iRAIL = 0;
        iALONG = -1;
    }
    for (i = 0; i < 8; i++) {
        if ((set == 0) ||
           (((((bonus == 0 || (Rail[iRAIL].type == '\x01')) &&
              ((death == 0 || (Rail[iRAIL].type == '\x02')))) &&
             ((gempath == 0 || (Rail[iRAIL].type == '\x03')))) &&
            ((bonus != 0 ||
             (((death != 0 || (gempath != 0)) || (Rail[iRAIL].type == '\0')))))))) {
            dbest = BestRailPosition(pos,&list[count],iRAIL,iALONG);
            if ((list[count].iRAIL != -1) && (list[count].iALONG != -1)) {
                if ((best == NULL) || (dbest < d)) {
                    best = &list[count];
                    d = dbest;
                }
                count++;
                if (count == 3) break;
            }
        }
        iRAIL = (iRAIL + 1) % 8;
        iALONG = -1;
    }
    if (best != 0) {
        *rpos = *best;
    }
    else {
        rpos->iRAIL = rpos->iALONG = -1;
    }
    if (set != 0) {
        best_cRPos = best;
        cRPosCOUNT = count;
    }
    else{
        temp_best_cRPos = best;
        temp_cRPosCOUNT = count;
    }
        temp_iRAIL = (s32)rpos->iRAIL;
        temp_iALONG = (s32)rpos->iALONG;
        temp_fALONG = rpos->fALONG;
        temp_fACROSS = rpos->fACROSS;
    return;
}

//NGC MATCH
void MoveRailPosition(struct nuvec_s *dst,struct RPos_s *rpos,float distance,s32 direction) {
    struct rail_s* rail; 
    struct RPos_s RPos;
    struct nuvec_s *p0;
    struct nuvec_s *p1;
    struct nuvec_s *p;
    float f;
    float d;
    s32 i0;
    s32 i1;
    
    d = distance;
    if (d == 0.0f) {
        d = 0.5f;
    }
    temp_rail_end = 0;
    TempRPos = RPos = *rpos;
    if ((RPos.iRAIL == -1) || ((s32)RPos.iALONG == -1)) {
        return;
    }
    *dst = RPos.pos;
    rail = &Rail[RPos.iRAIL];
Loop: 
    i1 = RPos.iALONG; 
    i0 = i1 + 1;
    if ((i0 == rail->edges) && (rail->circuit != 0)) {
        i0 = 0;
    }
    p0 = (struct nuvec_s *)rail->pCAM->pts;
    p1 = (struct nuvec_s *)(rail->pCAM->pts + (i1 * rail->pCAM->ptsize));
    p  = (struct nuvec_s *)(rail->pCAM->pts + (i0 * rail->pCAM->ptsize));
    if (direction == 0) {
        p1 = p;
    }
    f = NuVecDist(dst,p1,NULL);
    if (d > f) {
        d = (d - f);
        *dst = *p1;
        if (direction != 0) {
            if (rail->circuit != 0) {
                RPos.iALONG++;
                if (RPos.iALONG == rail->edges) {
                    RPos.iALONG = direction;
                }
                goto Loop;
             }
            else {
                if (RPos.iALONG < rail->edges) {
                    RPos.iALONG++;
                    goto Loop;
                }
                temp_rail_end = 1;
                goto LAB_8000a950;
            }
         }
        if (rail->circuit != 0) {
            RPos.iALONG--;
            if (RPos.iALONG == -1) {
                RPos.iALONG = rail->edges;
                RPos.iALONG--;
            }
            goto Loop;
        }
        if (RPos.iALONG > 0) {
            RPos.iALONG--;
            goto Loop;
        }
        temp_rail_end = 2;
    } else {
        dst->x = (p1->x - dst->x) * (d / f) + dst->x;
        dst->y = (p1->y - dst->y) * (d / f) + dst->y;
        dst->z = (p1->z - dst->z) * (d / f) + dst->z;
    }
LAB_8000a950:
    TempRPos = RPos;
    return;
}

//MATCH NGC
void RailInfo(struct RPos_s* RPos, struct nuvec_s* pos, u16* yrot, u16* cam_yrot, u8* mode) {
    struct rail_s* rail;
    struct nuvec_s* pL;
    struct nuvec_s* pR;
    struct nuvec_s* pC;
    struct nuvec_s* p0;
    struct nuvec_s* p1;
    struct nuvec_s v0;
    struct nuvec_s v1;
    struct nuvec_s mid;
    struct nuvec_s vC;
    s32 i0;
    s32 iVar7;
    s32 iVar11;
    s32 iVar12;
    float l;
    float dp;
    s32 a;

    if (RPos->iRAIL != -1 && RPos->iALONG != -1) {
        iVar12 = RPos->iALONG;
        iVar11 = RPos->iALONG + 1;
        rail = &Rail[RPos->iRAIL];
        
        if ((iVar11 == rail->edges) && (rail->circuit != 0)) {
            iVar11 = 0;
        }
        
        if (pos != NULL) {
            p0 = (struct nuvec_s*)(rail->pCAM->pts + (iVar12 * rail->pCAM->ptsize));
            p1 = (struct nuvec_s*)(rail->pCAM->pts + (iVar11 * rail->pCAM->ptsize));
            pos->x = (p1->x - p0->x) * RPos->fALONG + p0->x;
            pos->y = (p1->y - p0->y) * RPos->fALONG + p0->y;
            pos->z = (p1->z - p0->z) * RPos->fALONG + p0->z;
        }
        
        if (yrot != NULL) {
            pL = (struct nuvec_s*)(rail->pLEFT->pts + (iVar12 * rail->pLEFT->ptsize));
            pR = (struct nuvec_s*)(rail->pRIGHT->pts + (iVar12 * rail->pRIGHT->ptsize));
            v0.x = (pL->x + pR->x) * 0.5f;
            v0.z = (pL->z + pR->z) * 0.5f;
            a = NuAtan2D(pR->x - pL->x, pR->z - pL->z);
            
            pL = (struct nuvec_s*)(rail->pLEFT->pts + (iVar11 * rail->pLEFT->ptsize));
            pR = (struct nuvec_s*)(rail->pRIGHT->pts + (iVar11 * rail->pRIGHT->ptsize));
            v1.x = (pL->x + pR->x) * 0.5f;
            v1.z = (pL->z + pR->z) * 0.5f;
            iVar7 = NuAtan2D(pR->x - pL->x, pR->z - pL->z);
            
            *yrot = (a + (s32)(RPos->fALONG * RotDiff(a, iVar7))) - 0x4000;
            if (mode != NULL) {
                if (pos != NULL) {
                    float dx;
                    float dz;
                    
                    mid.x = (v0.x + (v1.x - v0.x) * RPos->fALONG);
                    mid.z = (v0.z + (v1.z - v0.z) * RPos->fALONG);
                    
                    v0.x = NuTrigTable[*yrot];
                    v0.z = NuTrigTable[(*yrot + 0x4000) & 0xFFFF];
                    
                    dx = mid.x - pos->x;
                    dz = mid.z - pos->z;

                    l = 1.0f / NuFsqrt(dx * dx + dz * dz);
                    v1.x = dx * l;
                    v1.z = dz * l;
                    dp = (v0.x * v1.x) + (v0.z * v1.z);
                    if (dp > 0.866f) {
                        *mode = 1;
                    } else if (-0.866f > dp) {
                        *mode = 2;
                    } else if (NuFabs(dp) < 0.5f) {
                        if (RotDiff(NuAtan2D(dx, dz), *yrot) > 0) {
                            *mode = 4;
                        } else {
                            *mode = 8;
                        }
                    } else {
                        *mode = 0;
                    }
                } else {
                    *mode = 0;
                }
            }
        }
        
        if (cam_yrot != NULL) {
            pC = (struct nuvec_s*)(rail->pCAM->pts + (iVar12 * rail->pCAM->ptsize));
            pL = (struct nuvec_s*)(rail->pLEFT->pts + (iVar12 * rail->pLEFT->ptsize));
            pR = (struct nuvec_s*)(rail->pRIGHT->pts + (iVar12 * rail->pRIGHT->ptsize));
            
            v0.x = (pL->x + pR->x) * 0.5f;
            v0.z = (pL->z + pR->z) * 0.5f;
            
            vC = *pC;
            
            if ((Level == 6) || (Level == 0x22)) {
                vC.x += 5.0f;
            }
            
            a = NuAtan2D(v0.x - vC.x, v0.z - vC.z);
            
            pC = (struct nuvec_s*)(rail->pCAM->pts + (iVar11 * rail->pCAM->ptsize));
            pL = (struct nuvec_s*)(rail->pLEFT->pts + (iVar11 * rail->pLEFT->ptsize));
            pR = (struct nuvec_s*)(rail->pRIGHT->pts + (iVar11 * rail->pRIGHT->ptsize));
            
            v1.x = (pL->x + pR->x) * 0.5f;
            v1.z = (pL->z + pR->z) * 0.5f;
            
            vC = *pC;
            
            if ((Level == 6) || (Level == 0x22)) {
                vC.x += 5.0f;
            }
            
            i0 = NuAtan2D(v1.x - vC.x, v1.z - vC.z);
            *cam_yrot = (a + (s32)(RPos->fALONG * RotDiff(a, i0)));
        }
    }
}

//MATCH NGC
float LookUpDownRail(struct obj_s *obj,u16 yrot,s32 mode) {
  s32 dir;
  s32 dyrot;
  float pos;
  
  if (((Level == 8) && ((player->obj).RPos.iRAIL == '\0')) && ((u16)(player->obj).RPos.iALONG - 0x2d <= 3U)) {
        pos = 1.0f;
  }
else if ((Level == 0x11) && ((mode & 2U) != 0)) {
        pos = 1.0f;
    }
    else if (((Level == 1) && ((player->obj).RPos.iRAIL == '\0')) &&
       (((u32)(u16)(player->obj).RPos.iALONG < 200 || ((u32)(u16)(player->obj).RPos.iALONG - 0xea < 0x20)))) {
      pos = -1.25f;
    }
    else if ((Level == 0xc) && (((player->obj).RPos.iRAIL == '\0' && ((u16)(player->obj).RPos.iALONG <= 0xf)))) {
        pos = 0.0f;
    }
    else if (((Level == 7) && ((player->obj).RPos.iRAIL == '\0')) &&
	     (((u32)(u16)(player->obj).RPos.iALONG - 0x9e < 0xc || ((u32)(u16)(player->obj).RPos.iALONG - 0x6a < 0x24)))) {
      pos = -1.35f;
    }
    else {
      if ((Level == 0x13) && (Death == 2))
      pos = 0.0f;
        else{
              pos = -1.0f; 
        }
    }

  dyrot = RotDiff(yrot,obj->hdg);
  if ((dyrot >> 0x1f ^ dyrot) - (dyrot >> 0x1f) < 0x2aab) {
    dir = 0;
  }
  else {
    if ((dyrot >> 0x1f ^ dyrot) - (dyrot >> 0x1f) < 0x5556) {
        return pos; 
    }
    dir = 1;
  }
  if (((mode & 3U) != 0) && (dir == 1)) {
    pos = (pos - 2.0f);
  }
  return pos;
}

//MATCH NGC
s32 InSplineArea(struct nuvec_s *pos, struct nugspline_s *spl) {
    struct nuvec_s *p0;
    struct nuvec_s *p1;
    s32 i;
    s32 j;
    
    for (i = 1; i < spl->len; i++) {
        p1 = (struct nuvec_s*)((s32)spl->pts + (i * spl->ptsize));
        j = i + 1;
        if (j == spl->len) {
            j = 1;
        }
        p0 = (struct nuvec_s*)((s32)spl->pts + (j * spl->ptsize));
        if (!((pos->x - p1->x) * (p0->z - p1->z)
              + (pos->z - p1->z) * (p1->x - p0->x) >= 0.0f)) {
            return 0;
        }
    }
    return 1;
}

//77.48% NGC
void MoveGameCamera(struct cammtx_s *GameCamera,struct obj_s *obj) {
  static float Blend_183;
  float fVar3;
  float dVar39;
  s32 old_vertical;
  s32 iVar13;
  s32 uVar14;
  s32 character;
  s32 axSEEK;
  s32 aySEEK;
  s32 iVar17;
  struct nuvec_s *p0;
  struct nuvec_s *p1;
  s32 unaff_r14;
  s32 uVar27;
  s32 uVar28;
  s32 unaff_r22;
  s32 iVar30;
  s32 uVar31;
  s32 vehicle;
  u16 a;
  s32 iVar34;
  struct RPos_s RPos;
  struct nuvec_s vec;
  struct nuvec_s local_150;
  struct nuvec_s local_140;
  struct nuvec_s dst;
  float fVar43;
  float fVar44;
  struct nuvec_s local_120;
  struct nuvec_s local_114;
  struct nuvec_s local_108;
  struct nuvec_s local_fc;
  struct nuvec_s local_f0;
  struct nuvec_s local_e4;
  s32 oldmode;
  float local_d8;
  float local_d4;
  float local_d0;
  struct nuvec_s local_c8;
  float local_b8;
  float local_b0;
  float dVar40;
  struct anim_s *anim;
  struct creature_s *c;
  float dVar41;
  float dVar42;
  
  pVIS = NULL;
  vehicle = -1;
  cut_on = set_cutscenecammtx;
  if (set_cutscenecammtx != 0) {
    GameCamera->m = cutscenecammtx;
    set_cutscenecammtx = 0;
    goto Finish;
  }
  if (((PLAYERCOUNT != 0) && (VEHICLECONTROL == 1)) && (player->obj.vehicle != -1)) {
    vehicle = player->obj.vehicle;
  }
  oldmode = GameCamera->mode;
  c = NULL;
  GameCamera->mode = 0;
  if (Level == 0x23) {
    GameCamera->mode = 8;
    if ((((SplTab[12].spl != NULL) && (SplTab[13].spl != NULL)) &&
        (CRemap[96] != -1)) && (CModel[CRemap[96]].anmdata[0x2b] != NULL)) {
      GameCamera->mode = 0x18;
      anim = &TempAnim;
    }
  } else if (Level == 0x26) {
    GameCamera->mode = 10;
    if (SplTab[1].spl == NULL) {
      GameCamera->mode = 0x19;
    }
  } else if (Level == 0x28) {
      GameCamera->mode = 0x20;
  } else if (Level == 0x2b) {
      GameCamera->mode = 0x22;
  } else if (Level == 0x19) {
    character = 2;
    GameCamera->mode = 0x1a;
    if (CrunchTime_Intro != 0) {
      character = 0x7f;
    }
    if (FindNearestCreature(&vec,character,&dst) < 1000000.0f) {
        c = &Character[temp_creature_i];
        if (((CrunchTime_Intro != 0 || (c->obj.anim.newaction == 0x49)) ||
         (c->obj.anim.newaction == 0x46)) && (SpaceCortex_Defeated == 0)) {
            GameCamera->mode = 0x1b;
        }
    }
    if ((oldmode != -1) && (GameCamera->mode != oldmode)) {
      BlendGameCamera(GameCam,1.0f);
    }
  } else if (((Level == 0xd) || (Level == 0x12)) || (Level == 0x1a)) {
        GameCamera->mode = 0xe;
  } else if ((Level == 0x18) || (Level == 0x24)) {
        GameCamera->mode = 0xe;
  } else if ((Level == 0x1e) && (level_part_2 != 0)) {
        GameCamera->mode = 0xe;
  }
  else if (Level == 0x25) {
    if (((Cursor.menu != 0x15) &&
        (((Cursor.wait == 0 || (Cursor.new_menu != 0x15)) && (Cursor.menu != 0x1a)) )
        && (((Cursor.menu != 0x2f && (Cursor.menu != 0x21)) && (Cursor.menu != 0x20))) &&
          (Cursor.menu != 0x30) && (Cursor.menu != 0x31) && ((Cursor.menu != 0x16 &&
            (((Cursor.menu != 0x19 && (Cursor.menu != 0x1d)) && (Cursor.menu != 0x18)))) )
           && (Cursor.menu != 0x1c) && (Cursor.menu != 0x1b) &&
        ((Cursor.menu != 0x17 && (((Cursor.menu != 0x1e && (Cursor.menu != 0x20)) &&
          (Cursor.menu != 0x1f && (Cursor.menu != 0x25) && (Cursor.menu != 0x26))))))) ||
       (SplTab[65].spl == NULL || (SplTab[66].spl != NULL))) {
      GameCamera->mode = 0x1f;
    } else if (GameMode == 1) {
        GameCamera->mode = 0x21;
    } else if ((Death == 1) || (Death == 3)) {
        GameCamera->mode = 6;
    } else if ((nRAILS != 0) && (best_cRPos != NULL)) {
        GameCamera->mode = 5;
    } else if (Death != 2) {
          GameCamera->mode = 9;
    }
  } else if (PLAYERCOUNT != 0) {
    if (((((Bonus == 1) || (Bonus == 3)) || (Death == 1)) || ((Death == 3 || (GemPath == 1))))
       || (GemPath == 3)) {
      GameCamera->mode = 6;
    }
    else if ((!(vtog_duration < vtog_time) || (vtog_blend == 0)) || ((pVTog == NULL || (pVTog->pCAM != NULL)))) {
      GameCamera->mode = 0x1c;
    }
    else if ((Level == 6) || (Level == 0x22)) {
        GameCamera->mode = 5;
    } else if ((LBIT & 0x00100210801) != 0) {
        GameCamera->mode = 0x10;
    }
    else if (((obj->flags & 1) == 0) || (((obj->dead == 4 || obj->dead == 5) || ((char)obj->dead == 8)))) {
        GameCamera->mode = 0x1e;
    } else if ((nRAILS == 0) || (best_cRPos != NULL)) {
        GameCamera->mode = 5;
    } else if (SplTab[1].spl == NULL) {
        GameCamera->mode = 4;
    } else {
        GameCamera->mode = 10;
    }
  } else {
      GameCamera->mode = 4;
  }
  if ((SplTab[8].spl != NULL) && (InSplineArea(&obj->pos,SplTab[8].spl) != 0)) {
    GameCamera->mode = 0x16;
  } else if ((SplTab[9].spl != NULL) && (InSplineArea(&obj->pos,SplTab[9].spl) == 0)) {
    GameCamera->mode = 0x17;
    unaff_r14 = 0;
  } else if ((SplTab[10].spl != NULL) && (InSplineArea(&obj->pos,SplTab[10].spl) == 0)) {
    GameCamera->mode = 0x17;
    unaff_r14 = 1;
  } else if ((SplTab[11].spl != NULL) && (InSplineArea(&obj->pos,SplTab[11].spl) != 0)) {
    GameCamera->mode = 0x17;
    unaff_r14 = 2;
  }
  if ((Level == 0x17) && (GameCamera->mode == 5)) {
    if (FindNearestCreature(&vec,0x7f,&dst) < 1000000.0f) {
      c = &Character[temp_creature_i];
      if (DrainDamage_Intro != 0) {
        GameCamera->mode = 0x1b;
      }
    }
    if (oldmode == -1) goto LAB_8000b830;
    if (GameCamera->mode != oldmode) {
      BlendGameCamera(GameCam,1.0f);
      goto LAB_8000b830;
    }
  }
  else {
LAB_8000b830:
    if (GameCamera->mode != oldmode) {
      if ((GameCamera->mode == 0x17) || (oldmode == 0x17)) {
        BlendGameCamera(GameCamera,0.5f);
      } else if (((((GameCamera->mode == 0x16) || (oldmode == 0x16)) || (oldmode == 0x1c)) ||
               (((oldmode == 0x15 && (GameCamera->mode == 5)) ||
                ((GameCamera->mode == 0x1f && (oldmode == 9)))))) ||
              ((GameCamera->mode == 9 && (oldmode == 0x1f)))) {
        BlendGameCamera(GameCamera,1.0f);
      }
    }
  }
  local_140.x = 0.125f;
  local_140.y = 0.125f;
  local_140.z = 0.125f;
  if (pos_START != NULL) {
    vec = *pos_START;
  } else {
    vec = v000;
  }
  if (GameCamera->mode != oldmode) {
    GameCamera->iRAIL = -1;
    GameCamera->iALONG = -1;
  }
  //fVar43 = obj->max.y;
  //fVar44 = obj->SCALE;
  uVar27 = -1;
  uVar28 = -1;
  local_150.x = obj->pos.x;
  local_150.y = obj->max.y * obj->SCALE * 0.5f + obj->pos.y;
  local_150.z = obj->pos.z;
  axSEEK = 3;
  aySEEK = 4;
switch(GameCamera->mode) {
    case 0x14:
        local_150.y = (obj->min.y + obj->max.y) * obj->SCALE * 0.5f + obj->pos.y;
        p0 = (struct nuvec_s *)SplTab[8].spl->pts;
        iVar30 = (s32)SplTab[8].spl->ptsize;
        p1 = (struct nuvec_s *)(SplTab[8].spl->pts + iVar30);
        vec.x = local_150.x + (p1->x - p0->x);
        vec.y = local_150.y + (p1->y - p0->y);
        vec.z = local_150.z + (p1->z - p0->z);
    break;
    case 0x17:
      aySEEK = 3;
      local_150.y = (obj->min.y + obj->max.y) * obj->SCALE * 0.5f + obj->pos.y;
      p0 = (struct nuvec_s *)(SplTab[unaff_r14 + 9].spl)->pts;
      vec = *p0;
    break;
    case 0x10:
          if (Level == 0x15) {
            RumbleCam(GameCamera);
            pNuCam->mtx = GameCamera->m;
            NuCameraSet(pNuCam);
            goto Finish;
          }
          if (GameTimer.frame < 0x78) {
            fVar43 = 2.5f;
          } else {
            fVar43 = 5.0f;
          }
          if (ATLASCAMHEIGHT > fVar43) {
            ATLASCAMHEIGHT -= 0.03333334f;
            if (ATLASCAMHEIGHT < fVar43) {
              ATLASCAMHEIGHT = fVar43;
            }
          } else if (ATLASCAMHEIGHT < fVar43) {
            ATLASCAMHEIGHT += 0.03333334f;
            if (ATLASCAMHEIGHT > fVar43) {
              ATLASCAMHEIGHT = fVar43;
            }
          }
          local_150.x = obj->pos.x;
          local_150.y = (obj->min.y + obj->max.y) * obj->SCALE * 0.5f + obj->pos.y;
          local_150.z = obj->pos.z;
          vec.y = local_150.y + ATLASCAMHEIGHT;
          vec.x = local_150.x + ATLASCAMHEIGHT;
          vec.z = local_150.z - ATLASCAMHEIGHT;
          if (ResetAtlasCamera != 0) {
            ResetAtlasCamera = 0;
            GameCamera->pos = vec;
            GameCamera->blend_time = GameCamera->blend_duration;
          }
    break;
    case 0xe:
        GliderCam(GameCamera);
        pNuCam->mtx = GameCamera->m;
        NuCameraSet(pNuCam);
        goto Finish;
    break;
    case 0x16:
        JeepCamFollowAng(GameCamera,0);
        pNuCam->mtx = GameCamera->m;
        NuCameraSet(pNuCam);
        goto Finish;
    break;
    case 0x15:
        JeepCamIntro(GameCamera);
        pNuCam->mtx = GameCamera->m;
        NuCameraSet(pNuCam);
        goto Finish;
    break;
    case 0x1d:
    case 0xf:
      JeepCam(GameCamera);
      GameCamera->seek.y = GameCamera->seek.x = GameCamera->seek.z = 1.0f;
      local_140.x = local_140.z = local_140.y = 1.0f;
      local_150 = JeepvObj;
      GameCamera->pos = vec = JeepvPos;
    break;
    case 13: // > '\v'
        uVar31 = RotDiff(-GameCamera->yrot,obj->hdg);
        if ((s32)(uVar31 >= 0 ? uVar31 : -uVar31) > 0x4000) {
          //fVar43 = 0.0f;
            Blend_183 -= 0.025f;
          if (Blend_183 < 0.0f) {
             Blend_183 = 0.0f;
          }
        }
        else {
          //fVar43 = 1.0f;
          if (Blend_183 > 1.0f) {
            Blend_183 -= 0.025f;
            if ((Blend_183 < 1.0f)) {
              if (Blend_183 < 1.0f) {
                 Blend_183 = 1.0f; 
              }   
            }
          }
          else {
            Blend_183 += 0.025f;
            if (!(Blend_183 > 1.0f)) {
             Blend_183 = 1.0f;
            }
          }
        }
        RPos = obj->RPos;
        if ((RPos.iRAIL != -1) && (RPos.iALONG != -1)) {
          //dVar39 = 5.0f;
          if (Blend_183 != 1.0f) {
            MoveRailPosition(&local_108,&RPos,3.0f,1);
            GameCamera->iRAIL = TempRPos.iRAIL;
            GameCamera->iALONG = TempRPos.iALONG;
            MoveRailPosition(&local_f0,&RPos,3.0f,0);
            local_d8 = local_108.x - local_f0.x;
            local_d4 = local_108.y - local_f0.y;
            local_d0 = local_108.z - local_f0.z;
            fVar43 = sqrt(local_d0 * local_d0 + local_d8 * local_d8 + local_d4 * local_d4);
            local_f0.x = obj->pos.x;
            local_f0.y = obj->top * obj->SCALE + obj->pos.y + 0.5f;
            local_f0.z = obj->pos.z;
            dVar40 = 2.0f;
            local_108.x = local_f0.x + (local_d8 * 5.0f) / fVar43;
            local_108.y = local_f0.y + (local_d4 * 5.0f) / fVar43;
            local_108.z = local_f0.z + (local_d0 * 5.0f) / fVar43;
            MoveRailPosition(&local_c8,&RPos,3.0f,1);
            local_d0 = local_108.z - local_c8.z;
            local_d8 = local_108.x - local_c8.x;
            dVar39 = sqrt(local_d8 * local_d8 + local_d0 * local_d0);
            if (dVar39 > dVar40) {
              local_b0 = ((local_d0 * (dVar39 - dVar40)) / dVar39);
              local_b8 = ((local_d8 * (dVar39 - dVar40)) / dVar39);
              local_f0.x = local_f0.x - local_b8;
              local_f0.z = local_f0.z - local_b0;
              local_108.x = local_108.x - local_b8;
              local_108.z = local_108.z - local_b0;
            }
          }
          if (Blend_183 != 0.0f) {
            local_e4.x = obj->pos.x;
            local_e4.z = obj->pos.z;
            local_e4.y = obj->top * obj->SCALE + obj->pos.y + 0.5f;
            MoveRailPosition(&local_fc,&RPos,7.0f,1);
            GameCamera->iRAIL = TempRPos.iRAIL;
            GameCamera->iALONG = TempRPos.iALONG;
          }
          local_150.x = (1.0f - Blend_183) * local_f0.x + Blend_183 * local_e4.x;
          local_150.y = (1.0f - Blend_183) * local_f0.y + Blend_183 * local_e4.y;
          local_150.z = (1.0f - Blend_183) * local_f0.z + Blend_183 * local_e4.z;
          vec.x = (1.0f - Blend_183) * local_108.x + Blend_183 * local_fc.x;
          vec.y = (1.0f - Blend_183) * local_108.y + Blend_183 * local_fc.y;
          vec.z = (1.0f - Blend_183) * local_108.z + Blend_183 * local_fc.z;
        }
        else {
          vec = GameCamera->pos;
        }
    break;
    case 17:
        vec.x = local_150.x;
        vec.y = local_150.y + 10.0f;
        vec.z = local_150.z + 1.0f;
    break;
    case 0x1e:
        local_150.y = (obj->bot + obj->top) * obj->SCALE * 0.5f + obj->pos.y;
        NuVecSub(&dst,&GameCamera->pos,&local_150);
        NuVecNorm(&dst,&dst);
        vec.x = dst.x * 3.0f + local_150.x;
        vec.y = dst.y * 3.0f + local_150.y;
        vec.z = dst.z * 3.0f + local_150.z;
        if (obj->dead == 3) {
          local_150.y += 1.0f;
        }
    break;
    case 5:
    //case 6:
    //case 7:
    //case 8:
    case 9:
            if ((best_cRPos != NULL) && (cRPosCOUNT != 0)) {
              old_vertical = GameCamera->vertical;
              GameCamera->vertical = best_cRPos->vertical;
              if (GameCamera->vertical != old_vertical) {
                BlendGameCamera(GameCamera,1.0f);
              }
              if (((Level == 0x25) || (Level == 6)) || (Level == 0x22)) {
                GameCamera->distance = 0.0f;
              }
              else {
                c = (struct creature_s *)obj->parent;
                if (((c != player) || (c->slam != 2)) || (c->slam_wait != 0 )) {
                  if (best_cRPos->vertical == 0) {
                    fVar43 = LookUpDownRail(obj,best_cRPos->angle,(s32)best_cRPos->mode);
                  } else {
                    fVar43 = 0.0f;
                  }
                  if ((Level == 8) && (fVar43 < -2.0f)) {
                    fVar43 = -2.0f;
                  }
                  //fVar44 = GameCamera->distance;
                  if (GameCamera->distance > fVar43) {
                    GameCamera->distance -= 0.03333334f;
                    if (GameCamera->distance < fVar43) {
                      GameCamera->distance = fVar43;
                    }
                  } else if (GameCamera->distance < fVar43) {
                        GameCamera->distance += 0.03333334f;
                      if (GameCamera->distance > fVar43) {
                        GameCamera->distance = fVar43;
                      }
                  }
                }
              }
              iVar30 = 0;
              vec = v000;
              for(iVar30 = 0; iVar30 < cRPosCOUNT; iVar30++) {
                  dVar40 = NuFabs(GameCamera->distance);
                  MoveRailPosition(&dst,&cRPos[iVar30],dVar40,(GameCamera->distance >= 0.0f) ? 0 : 1);
                  if (best_cRPos == &cRPos[iVar30]) {
                    pVIS = Rail[cRPos[iVar30].iRAIL].pCAM;
                    iVIS = (s32)TempRPos.iALONG;
                  }
                  if (cRPos[iVar30].vertical != 0) {
                    iVar13 = (s32)(Rail[cRPos[iVar30].iRAIL].pCAM)->ptsize;
                    p0 = (struct nuvec_s *)((Rail[cRPos[iVar30].iRAIL].pCAM)->pts + cRPos[iVar30].i1 * iVar13);
                    p1 = (struct nuvec_s *)((Rail[cRPos[iVar30].iRAIL].pCAM)->pts + cRPos[iVar30].i2 * iVar13);
                    dst.y = obj->max.y * obj->SCALE + obj->pos.y;
                    if ((dst.y < p1->y) || (dst.y > p1->y)) {
                      dst.y = p1->y;
                    }
                  }
                  NuVecAdd(&vec,&vec,&dst);
              }
              //fVar43 = 1.0f / (cRPosCOUNT);
              unaff_r22 = (s32)cRPos[0].cam_angle;
              best_railangle = cRPos[0].angle;
              vec.x *= (1.0f / (cRPosCOUNT));
              vec.y *= (1.0f / (cRPosCOUNT));
              vec.z *= (1.0f / (cRPosCOUNT));
              if (cRPosCOUNT > 1) {
                iVar30 = 0;
                iVar13 = 0;
                for(iVar34 = 1; iVar34 < cRPosCOUNT; iVar34++) {
                    iVar30 += RotDiff(cRPos[iVar34].cam_angle,cRPos[iVar34].cam_angle);
                    iVar13 += RotDiff(best_railangle,cRPos[iVar34].angle);
                }
                best_railangle = best_railangle + (short)(iVar13 / (cRPosCOUNT - 1));
                unaff_r22 += iVar30 / (cRPosCOUNT - 1) & 0xffff;
              }
            }
            else {
              vec = GameCamera->pos;
            }
            local_150.x = obj->pos.x;
            local_150.z = obj->pos.z;
            local_150.y = (obj->min.y + obj->max.y) * obj->SCALE * 0.5f + obj->pos.y;
            dVar40 = 0.0f;
            dVar39 = 0.033333335f;
            if ((VEHICLECONTROL != 2) &&
               ((((VEHICLECONTROL != 1 || (obj->vehicle != 0x20)) && (best_cRPos != NULL))
                && ((best_cRPos->mode & 0xc) != 0)))) {
              if (Level == 0x17) {
                dVar40 = -2.0f;
              } else {
                iVar30 = RotDiff(obj->hdg,best_cRPos->angle);
                dVar40 = NuTrigTable[(iVar30 + 0x4000) & 0xFFFF];
              }
              dVar39 = (NuFabs(dVar40) * 0.016666668f);
            }
            if (GameCamera->ahead > dVar40) {
              GameCamera->ahead -= dVar39;
              if (GameCamera->ahead < dVar40) {
                GameCamera->ahead = dVar40;
              }
            }
            else if (GameCamera->ahead < dVar40) {
                GameCamera->ahead += dVar39;
                if (GameCamera->ahead > dVar40) {
                    GameCamera->ahead = dVar40;
                }
            }
            if (GameCamera->ahead != 0.0f) {
              fVar44 = NuTrigTable[(best_cRPos->angle + 0x4000) & 0xFFFF]  * GameCamera->ahead;
              dVar42 = NuTrigTable[best_cRPos->angle] * GameCamera->ahead;
              vec.x += dVar42;
              vec.z += fVar44;
              local_150.x += dVar42;
              local_150.z += fVar44;
            }
            if (vehicle == 0xa1) {
              aySEEK = 3;
              axSEEK = 2;
            }
            if (((Level == 0x1d) && (SplTab[68].spl != NULL)) &&
               (SplTab[69].spl != NULL)) {
              if (GameTimer.ftime < 3.0f) {
                dVar39 = (GameTimer.ftime / 3.0f);
                PointAlongSpline(SplTab[68].spl,GameTimer.ftime / 3.0f,&local_120,NULL,NULL);
                PointAlongSpline(SplTab[69].spl,dVar39,&local_114,NULL,NULL);
                dVar40 = (1.0f - dVar39);
                NuVecScale(&local_120,&local_120,dVar40);
                NuVecScale(&local_114,&local_114,dVar40);
                NuVecScaleAccum(&local_120,&vec,dVar39);
                NuVecScaleAccum(&local_114,&local_150,dVar39);
                vec = local_120;
                local_150 = local_114;
              }
            }
            else if (((Level == 3) && (SplTab[68].spl != NULL)) &&
                    (SplTab[69].spl != NULL)) {
              dVar40 = 6.0f;
              if (WesternCountdown < 1.0f) {
                dVar39 = 1.0f;
              } else if (WesternCountdown > 6.0f) {
                  dVar39 = 0.0f;
              }
              else {
                  dVar39 = ((5.0f - (WesternCountdown - 1.0f)) / 5.0f);
              }
              if (dVar39 > 0.94999999f) {
                dVar39 = 0.94999999f;
              }
              PointAlongSpline(SplTab[68].spl,dVar39,&local_120,NULL,NULL );
              PointAlongSpline(SplTab[69].spl,dVar39,&local_114,NULL,NULL );
              if (WesternCountdown < 1.0f) {
                fVar43 = 1.0f;
              } else if (WesternCountdown > dVar40) {
                  fVar43 = 0.0f;
              } else {
                  fVar43 = ((dVar40 - 1.0f) - (WesternCountdown - 1.0f)) /
                           (dVar40 - 1.0f);
              }
              if (fVar43 != 0.0f) {
                dVar39 = (fVar43 * fVar43);
                dVar40 = (1.0f - dVar39);
                NuVecScale(&local_120,&local_120,(1.0f - dVar39));
                NuVecScale(&local_114,&local_114,dVar40);
                NuVecScaleAccum(&local_120,&vec,dVar39);
                NuVecScaleAccum(&local_114,&local_150,dVar39);
                vec = local_120;
                local_150 = local_114;
              }
            }
    break;
    case 0x1f:
        //uVar31 = (s32)Cursor.wait;
        if (Cursor.wait != 0) {
          if (Cursor.new_menu == 0x15) {
            dVar39 = (s32)(Cursor.wait_frames - Cursor.wait);
          }
          else {
            dVar39 = Cursor.wait;
          }
          dVar39 = (dVar39 / Cursor.wait_frames);
        }
        else {
          if (Cursor.menu != -1) {
            dVar39 = 1.0f;
          }
          else {
            dVar39 = 0.0f;
          }
        }
        PointAlongSpline(SplTab[65].spl,dVar39,&vec,NULL,NULL);
        pVIS = death_pCAM;
        iVIS = iTEMP;
        vec.x += NuTrigTable[(s32)((unsigned long long)((GlobalTimer.frame % 0xf7) * 0x10000) * 0x10953f391 >>
                                 0x26) & 0xFFFF] * 0.1f;
        vec.y += NuTrigTable[(s32)((unsigned long long)((GlobalTimer.frame % 0xbf) * 0x10000) * 0xab8f69e3 >> 0x25
                                 ) & 0xFFFF] * 0.1f;
        vec.z += NuTrigTable[(s32)((unsigned long long)((GlobalTimer.frame % 0xd5) * 0x10000) * 0x99d722db >> 0x25
                                 ) & 0xFFFF] * 0.1f;
        PointAlongSpline(SplTab[66].spl,dVar39,&local_150,NULL,NULL);
    break;
    case 6:
          local_140.z = 0.1f;
          local_150.y = (obj->min.y + obj->max.y) * obj->SCALE * 0.5f + obj->pos.y;
          local_140.x = 0.1f;
          local_140.y = 0.1f;
          if ((Bonus == 1) || (Bonus == 3)) {
            uVar31 = (s32)((bonus_time / bonus_duration) * 32768.0f + 16384.0f);
            PointAlongSpline(bonus_pCAM,(-NuTrigTable[uVar31] + 1.0f) * 0.5f,&vec,NULL,NULL);
            pVIS = bonus_pCAM;
          }
          else if ((Death == 1) || (Death == 3)) {
            fVar43 = death_time / death_duration;
            if (Death == 3) {
              fVar43 = 1.0f - fVar43;
            }
            uVar31 = (fVar43 * 32768.0f + 16384.0f);
            PointAlongSpline(death_pCAM,(-NuTrigTable[uVar31] + 1.0f) * 0.5f,&vec,NULL,NULL);
            pVIS = death_pCAM;
          }
          else {
            fVar43 = gempath_time / gempath_duration;
            if (GemPath == 3) {
              fVar43 = 1.0f - fVar43;
            }
            uVar31 = (fVar43 * 32768.0f + 16384.0f);
            PointAlongSpline(gempath_pCAM,(-NuTrigTable[uVar31] + 1.0f) * 0.5f,&vec,NULL,NULL);
            pVIS = gempath_pCAM;
          }
          iVIS = iTEMP;
    break;
    case 0x1c:
        dVar39 = (vtog_time / vtog_duration);
        local_140.z = 0.2f;
        local_140.x = 0.2f;
        local_140.y = 0.2f;
        PointAlongSpline(pVTog->pCAM,vtog_time / vtog_duration,&vec,NULL,NULL);
        if (pVTog->pLOOK != NULL) {
           PointAlongSpline(pVTog->pLOOK,dVar39,&local_150,NULL,NULL); 
        }
    break;
    case 0x18:
        fVar43 = anim->anim_time - 1.0f;
        if (fVar43 > 30.0f) {
          fVar43 = 30.0f;
        }
        dVar39 = (fVar43 * 0.03333334f);
        PointAlongSpline(SplTab[12].spl,dVar39,&vec,NULL,NULL);
        PointAlongSpline(SplTab[13].spl,dVar39,&local_150,NULL,NULL);
    break;
    case 10:
        p0 = (struct nuvec_s *)(SplTab[1].spl)->pts;
        vec = *p0;
        iVar30 = (s32)(SplTab[1].spl)->ptsize;
        p1 = (struct nuvec_s *)(SplTab[8].spl->pts + iVar30);
        local_150 = *p1;
        if (Level == 0x26) {
          vec.x += NuTrigTable[((unsigned long long)((GlobalTimer.frame % 0x26e) * 0x10000) * 0x34ae820f
                                   >> 0x25) & 0xFFFF] * 0.05f;
          vec.y += NuTrigTable[((unsigned long long)((GlobalTimer.frame % 0x1eb) * 0x10000) * 0x10af2f723
                                   >> 0x27) & 0xFFFF] * 0.05f;
          vec.z += NuTrigTable[((GlobalTimer.frame % 0x1dc) * 0x10000) / 0x77 & 0xFFFF] * 0.05f;
        }
    break;
    case 0x20:
        oldmode = -1;
        vec = campos_SPACE;
        local_150 = v000;
    break;
    case 0x22:
        oldmode = -1;
        vec.x = NuTrigTable[32768] * 3.0f + 0.0f;
        vec.y = (CData[187].min.y + CData[187].max.y) * 0.5f;
        vec.z = NuTrigTable[49152] * 3.0f + 0.0f;
        local_150.x = 0.0f;
        local_150.y = vec.y;
        local_150.z = 0.0f;
    break;
    case 0x21:
        vec = cutcamlook_FRONTEND;
        vec.x += NuTrigTable[(s32)((unsigned long long)((GlobalTimer.frame % 0xf7) * 0x10000) * 0x10953f391 >>
                                 0x26) & 0xFFFF] * 0.1f;
        vec.y += NuTrigTable[(s32)((unsigned long long)((GlobalTimer.frame % 0xbf) * 0x10000) * 0xab8f69e3 >> 0x25
                                 ) & 0xFFFF] * 0.1f;
        vec.z += NuTrigTable[(s32)((unsigned long long)((GlobalTimer.frame % 0xd5) * 0x10000) * 0x99d722db >> 0x25
                                 ) & 0xFFFF] * 0.1f;
    break;
    case 0x1a:
        vec = BOSSCAMPOS;
        local_150.y = (obj->min.y + obj->max.y) * obj->SCALE * 0.5f + obj->pos.y;
    break;
    case 0x1b:
        if (Level == 0x17) {
          vec = BOSSCAMPOS2;
        }
        else {
          vec = BOSSCAMPOS;
        }
        //dVar39 = 0.5f;
        local_150.x = obj->pos.x;
        local_150.y = (obj->min.y + obj->max.y) * obj->SCALE * 0.5f + obj->pos.y + 0.666f;
        local_150.z = obj->pos.z;
        dst.x = c->obj.pos.x;
        dst.y = (c->obj.min.y + c->obj.max.y) * c->obj.SCALE * 0.5f +
                c->obj.pos.y + 0.666f;
        dst.z = c->obj.pos.z;
        NuVecAdd(&local_150,&local_150,&dst);
        NuVecScale(&local_150,&local_150,0.5f);
        if (Level == 0x19) {
          NuVecSub(&dst,&local_150,&vec);
          NuVecNorm(&dst,&dst);
          NuVecScale(&dst,&dst,3.0f);
          NuVecAdd(&vec,&vec,&dst);
        }
    break;
    case 11: // else
          dVar40 = (0.0f - local_150.x);
          dVar39 = (0.0f - local_150.z);
          vec.x = 0.0f;
          vec.y = 9.3f;
          vec.z = 0.0f;
          local_150.y = (obj->min.y + obj->max.y) * obj->SCALE * 0.5f + obj->pos.y;
          fVar43 = NuFsqrt((dVar40 * dVar40 + (dVar39 * dVar39))) + 0.1f;
          if (fVar43 < 1.0f) {
            uVar31 = (s32)GameCamera->angle;
          }
          else {
            uVar31 = NuAtan2D(dVar40,dVar39) & 0xffff;
          }
          if (GameCamera->mode == oldmode) {
            uVar31 = TurnRot(GameCamera->angle,(u16)uVar31,0x444);
          }
          GameCamera->angle = (u16)uVar31;
          dVar40 = 4.0f;
          vec.x = NuTrigTable[uVar31] * 4.0f + 0.0f;
          vec.z = NuTrigTable[(uVar31 + 0x4000) & 0xFFFF] * 4.0f + 0.0f;
          dVar39 = (local_150.z - vec.z);
          dVar42 = (local_150.x - vec.x);
          dVar41 = (NuFsqrt((dVar42 * dVar42 + (dVar39 * dVar39))) + 0.1f);
          if (dVar41 != dVar40) {
            dVar40 = (-(dVar40 - dVar41) / dVar41);
            vec.z = (dVar39 * dVar40 + vec.z);
            vec.x = (dVar42 * dVar40 + vec.x);
          }
          aySEEK = 2;
    break;
    case 8:
          uVar27 = 0x4000;
          uVar28 = 0;
          vec.x = LOGOCAMX;
          vec.y = LOGOCAMY;
          vec.z = LOGOCAMZ;
    break;
    case 0x19:
        uVar27 = 0;
        vec.y = NuTrigTable[(s32)((unsigned long long)((GlobalTimer.frame % 0xbf) * 0x10000) * 0xab8f69e3 >>
                                 0x25) & 0xFFFF] * 0.1f + 0.5f;
        vec.z = NuTrigTable[(s32)((unsigned long long)((GlobalTimer.frame % 0xd5) * 0x10000) * 0x99d722db >>
                                 0x25) & 0xFFFF] * 0.1f - 1.0f;
        vec.x = NuTrigTable[(s32)((unsigned long long)((GlobalTimer.frame % 0xf7) * 0x10000) * 0x10953f391 >>
                                 0x26) & 0xFFFF] * 0.1f + 1.0f;
        uVar28 = 0;
    break;
    case 4:
        if (oldmode == GameCamera->mode) {
          dVar40 = (GameCamera->pos.z - local_150.z);
          dVar42 = (GameCamera->pos.x - local_150.x);
          dVar39 = NuFsqrt((dVar42 * dVar42 + (dVar40 * dVar40)));
          uVar31 = NuAtan2D(dVar42,dVar40) & 0xffff;
          if (dVar39 > CAMSTOPNEAR) {
            local_140.x = local_140.x * (CAMSTOPNEAR - dVar39);
            local_140.z = local_140.z * (CAMSTOPNEAR - dVar39);
            local_140.x = ((local_140.x + local_140.x) / CAMSTOPNEAR);
            local_140.z = ((local_140.z + local_140.z) / CAMSTOPNEAR);
          } else if (dVar39 < CAMSTOPFAR) {
              local_140.x = (local_140.x * (dVar39 - CAMSTOPNEAR)) / (CAMSTOPFAR - CAMSTOPNEAR);
              local_140.z = (local_140.z * (dVar39 - CAMSTOPNEAR)) / (CAMSTOPFAR - CAMSTOPNEAR);
          }
        } else {
          uVar31 = obj->hdg - 0x8000 & 0xffff;
        }
        vec.x = NuTrigTable[uVar31] * CAMSTOPNEAR + obj->pos.x;
        vec.y = obj->max.y * obj->SCALE + obj->pos.y;
        vec.z = NuTrigTable[(uVar31 + 0x4000) * 4 & 0xFFFF] * CAMSTOPNEAR  +
                obj->pos.z;
        if (GameCamera->mode == oldmode) {
          dst.x = GameCamera->pos.x;
          dst.y = GameCamera->pos.y;
          dst.z = GameCamera->pos.z;
        } else {
          dst = vec;
        }
        fVar44 = NewShadowMask(&dst,0.0f,-1);
        if (fVar44 == 2000000.0f) {
            fVar43 = vec.y;
        } else {
            fVar43 = obj->max.y * obj->SCALE + fVar44;
        }
        if (vec.y < fVar43) {
            vec.y = fVar43;
        }
    break;
    case 12:
        //dVar39 = 0.5f;
        uVar31 = ((GameTimer.frame % 600 << 0x10) / 600 + 0x4000) * 4 & 0xFFFF;
        PointAlongSpline(Rail[1].pINCAM,NuTrigTable[uVar31] * 0.5f + 0.5f,&vec,NULL,NULL);
        dVar39 = (NuTrigTable[uVar31] * 0.5f + 0.5f);
        PointAlongSpline(Rail[1].pINPLAT,dVar39,&local_150,NULL,NULL);
    break;
    case 7: // < 9
        if ((Pad[0] != NULL) && ((Pad[0]->rdy & 4) != 0)) {
          SIDECAMDISTANCE = 6.0f;
        }
        vec.x = local_150.x + SIDECAMDISTANCE;
        vec.y = (obj->min.y + obj->max.y) * obj->SCALE * 0.5f + obj->pos.y;
        vec.z = obj->pos.z;
        uVar27 = 0;
        uVar28 = 0xc000;
        local_150.x = obj->pos.x;
        local_150.y = vec.y;
        local_150.z = vec.z;
    break;
}
  if ((PLAYERCOUNT != 0) && (obj->finished != 0)) {
    vec = GameCamera->pos;
  }
  //fVar43 = GameCamera->blend_duration;
  if (GameCamera->blend_time < GameCamera->blend_duration) {
    GameCamera->blend_time += 0.01666667f;    
    if (GameCamera->blend_duration > GameCamera->blend_time) {
        GameCamera->blend_time = GameCamera->blend_duration;
    }
  }
  if ((GameCamera->mode != oldmode) && (GameCamera->blend_time >= GameCamera->blend_duration)) {
    GameCamera->pos = vec;
    GameCamera->seek = local_140;
  }
  else {
    if (GameCamera->blend_time < GameCamera->blend_duration) {
      local_140.y = local_140.y * 0.25f;
    }
    if (GameCamera->seek.x > local_140.x) {
      GameCamera->seek.x -= 0.0009999999f;
      if (GameCamera->seek.x < local_140.x) {
        GameCamera->seek.x = local_140.x;
      }
    } else if (GameCamera->seek.x < local_140.x) {
            GameCamera->seek.x += 0.0009999999f;
            if (GameCamera->seek.x > local_140.x) {
                GameCamera->seek.x = local_140.x;
            }
    }
    if (GameCamera->seek.y > local_140.y) {
      GameCamera->seek.y -= 0.0009999999f;
      if (GameCamera->seek.y < local_140.y) {
        GameCamera->seek.y = local_140.y;
      }
    } else if (GameCamera->seek.y < local_140.y) {
            GameCamera->seek.y += 0.0009999999f;
            if (GameCamera->seek.y > local_140.y) {
                GameCamera->seek.y = local_140.y;
            }
    }
    if (GameCamera->seek.z > local_140.z) {
      GameCamera->seek.z -= 0.0009999999f;
      if (GameCamera->seek.z < local_140.z) {
        GameCamera->seek.z = local_140.z;
      }
    } else if (GameCamera->seek.z < local_140.z) {
            GameCamera->seek.z += 0.0009999999f;
            if (GameCamera->seek.z > local_140.z) {
               GameCamera->seek.z = local_140.z;
            }
    }
    if (GameCamera->blend_time < GameCamera->blend_duration) {
      fVar3 = GameCamera->blend_time / GameCamera->blend_duration;
      vec.x -= (GameCamera->oldpos.x) * fVar3 + GameCamera->oldpos.x;
      vec.y -= (GameCamera->oldpos.y) * fVar3 + GameCamera->oldpos.y;
      vec.z -= (GameCamera->oldpos.z) * fVar3 + GameCamera->oldpos.z;
    }
    GameCamera->pos.x = (vec.x - GameCamera->pos.x) * GameCamera->seek.x + GameCamera->pos.x;
    GameCamera->pos.y = (vec.y - GameCamera->pos.y) * GameCamera->seek.y + GameCamera->pos.y;
    GameCamera->pos.z = (vec.z - GameCamera->pos.z) * GameCamera->seek.z + GameCamera->pos.z;
  }
  GameCamera->newpos = vec;
  if ((((GameCamera->mode == 5) || (GameCamera->mode == 9)) || (GameCamera->mode == 6)) ||
     ((GameCamera->mode == 0x1a || (GameCamera->mode == 0x17)))) {
    iVar30 = 4;
  } else {
    iVar30 = 0;
  }
  if (iVar30 != 0) {
    if (((Level == 0x10) || (Level == 0)) || ((Level == 0xb || (Level == 9)))) {
      iVar30 = 0;
    }
    if ((iVar30 != 0) && (vehicle == 0xa1)) {
      iVar30 = 6;
    }
  }
  dVar40 = (local_150.x - GameCamera->pos.x);
  dVar42 = (local_150.y - GameCamera->pos.y);
  dVar39 = (local_150.z - GameCamera->pos.z);
  uVar31 = uVar27;
  //bVar37 = (uVar27 == -1) ? 0 : 1;
  if (uVar27 == -1) {
    uVar31 = -(NuAtan2D(dVar42,NuFsqrt((dVar40 * dVar40 + (dVar39 * dVar39)))));
    if (iVar30 != 0) {
      iVar13 = uVar31 * iVar30;
      if (iVar13 < 0) {
        iVar13 = iVar13 + 7;
      }
      uVar31 = iVar13 >> 3;
    }
    uVar31 = uVar31 & 0xffff;
  }
  if (GameCamera->blend_time < GameCamera->blend_duration) {
    uVar31 = (s32)GameCamera->old_xrot + (s32)((RotDiff(GameCamera->old_xrot,(u16)uVar31)) * (GameCamera->blend_time / GameCamera->blend_duration));
  }
  GameCamera->new_xrot = (u16)uVar31;
  if ((iVar30 != 0) && ((Level == 3 || ((Level == 0x1d && (GameTimer.ftime < 3.0f)))))) {
    iVar30 = 0;
  }
  uVar14 = uVar28;
  if (uVar28 == -1) {
    if ((GameCamera->mode == 0x1a) && (iVar30 != 0)) {
      iVar13 = RotDiff(0,(u16)NuAtan2D(dVar40,dVar39));
      iVar13 *= iVar30;
      if (iVar13 < 0) {
        iVar13 = iVar13 + 7;
      }
      uVar14 = iVar13 >> 3;
    }
    else if ((GameCamera->mode == 0x17) && (iVar30 != 0)) {
      iVar34 = 0;
      iVar13 = NuAtan2D(dVar40,dVar39);
      dst = v000;
      for(uVar14 = (s32)(SplTab[unaff_r14 + 9].spl)->len; iVar34 < uVar14; iVar34++) {
          iVar17 = iVar34 * (SplTab[unaff_r14 + 9].spl)->ptsize;
          p0 = (struct nuvec_s *)((SplTab[unaff_r14 + 9].spl)->pts + iVar17);
          NuVecAdd(&dst,&dst,p0);
      }
      NuVecScale(&dst,&dst,(1.0f / (uVar14)));
      iVar34 = NuAtan2D(dst.x - GameCamera->pos.x,dst.z - GameCamera->pos.z);
      iVar13 = RotDiff(iVar34,iVar13) * iVar30;
      if (iVar13 < 0) {
        iVar13 += 7;
      }
      uVar14 = iVar34 + (iVar13 >> 3);
    }
    else if (((GameCamera->mode == 5) && (iVar30 != 0)) && (cRPosCOUNT == 1)) {
      uVar14 = NuAtan2D(dVar40,dVar39) & 0xffff;
      if (unaff_r22 != -1) {
        iVar13 = RotDiff((u16)unaff_r22,uVar14) * iVar30;
        if (iVar13 < 0) {
          iVar13 += 7;
        }
        uVar14 = unaff_r22 + (iVar13 >> 3);
      }
    }
    else {
      uVar14 = NuAtan2D(dVar40,dVar39);
    }
  }
  if (GameCamera->blend_time < GameCamera->blend_duration) {
    iVar30 = ((s32)(RotDiff(GameCamera->old_yrot,(u16)uVar14)) * (GameCamera->blend_time / GameCamera->blend_duration));
    uVar14 = GameCamera->old_yrot + iVar30;
  }
  GameCamera->new_yrot = (u16)uVar14;
  iVar30 = 0;
  if (GameCamera->blend_time < GameCamera->blend_duration) {
    iVar30 = ((s32)(RotDiff(GameCamera->old_zrot,0)) * (GameCamera->blend_time / GameCamera->blend_duration));
    iVar30 = GameCamera->old_zrot + iVar30;
  }
  GameCamera->new_zrot = (u16)iVar30;
  if ((GameCamera->mode != oldmode) && (GameCamera->blend_time >= GameCamera->blend_duration)) {
    GameCamera->xrot = uVar31;
    GameCamera->yrot = uVar14;
    GameCamera->zrot = iVar30;
  }
  else {
    if (uVar27 != -1) {
      GameCamera->xrot = uVar27;
    }
    else {
      GameCamera->xrot = SeekRot(GameCamera->xrot,(u16)uVar31,axSEEK);
    }
    if (uVar28 != -1) {
      GameCamera->yrot = uVar28;
    }
    else {
      GameCamera->yrot = SeekRot(GameCamera->yrot,(u16)uVar14,aySEEK);
    }
    if (iVar30 != -1) {
      GameCamera->zrot = 0;
    }
    else {
      GameCamera->zrot = SeekRot(GameCamera->zrot,(u16)iVar30,5);
    }
  }
  a = GameCamera->xrot;
  iVar30 = GameCamera->yrot;
  if (GameCamera->judder > 0.0f) {
    GameCamera->judder -= 0.01666667f;
    if (GameCamera->judder > 0.0f) {
      iVar13 = (s32)((GameCamera->judder + GameCamera->judder) * 256.0f);
      if (gcc2_compiled__N102(GameCamera->judder,0.06666667f) < 0.03333334f) {
        iVar13 = -iVar13;
      }
      a += iVar13;
    } else {
      GameCamera->judder = 0.0f;
    }
  }
  NuMtxSetIdentity(&GameCamera->m);
  NuMtxTranslate(&GameCamera->m,&GameCamera->pos);
  NuMtxPreRotateY(&GameCamera->m,iVar30);
  NuMtxPreRotateX(&GameCamera->m,a);
  NuMtxPreRotateZ(&GameCamera->m,GameCamera->zrot);
Finish:
  GameCamera->vX.x = GameCamera->m._00;
  GameCamera->vX.y = GameCamera->m._01;
  GameCamera->vX.z = GameCamera->m._02;
  GameCamera->vY.x = GameCamera->m._10;
  GameCamera->vY.y = GameCamera->m._11;
  GameCamera->vY.z = GameCamera->m._12;
  GameCamera->vZ.x = GameCamera->m._20;
  GameCamera->vZ.y = GameCamera->m._21;
  GameCamera->vZ.z = GameCamera->m._22;
  pNuCam->mtx = GameCamera->m;
  NuCameraSet(pNuCam);
  if (player->used != 0) {
    GameCamera->hdg_to_player = NuAtan2D(player->obj.pos.x - GameCamera->pos.x,player->obj.pos.z - GameCamera->pos.z);
  }
}

//MATCH NGC
void GetALONG(struct nuvec_s *pos,struct RPos_s *rpos,s32 iRAIL,s32 iALONG,s32 info) {
  if (rpos == NULL) {
    rpos = &TempRPos;
  }
  if (nRAILS != 0) {
    ComplexRailPosition(pos,iRAIL,iALONG,rpos,0);
  }
  else {
    temp_iALONG = -1;
    temp_iRAIL = -1;
  }
  return;
}

//MATCH NGC
s32 FurtherALONG(s32 iRAIL0,s32 iALONG0,float fALONG0,s32 iRAIL1,s32 iALONG1,float fALONG1) {
    if (iRAIL0 == -1) {
        return 0;
    }
  if ((((iRAIL0 == -1) || (iRAIL1 == -1)) || (Rail[iRAIL0].type != Rail[iRAIL1].type)) || (iRAIL0 < iRAIL1)) {
            return 0;
  }
    if (iRAIL0 > iRAIL1) { 
        return 1; 
    } 
    if (iALONG0 < iALONG1) { 
        return 0; 
    } else if (iALONG0 <= iALONG1) {
        return (fALONG0 > fALONG1);
    } else if (iALONG0 <= iALONG1) {
        return (fALONG0 > fALONG1);
    } 
  return 1;
}

//MATCH NGC
s32 FurtherBEHIND(s32 iRAIL0,s32 iALONG0,float fALONG0,s32 iRAIL1,s32 iALONG1,float fALONG1) {
    if (iRAIL0 == -1) {
        return 0;
    }
  if ((((iRAIL0 == -1) || (iRAIL1 == -1)) || (Rail[iRAIL0].type != Rail[iRAIL1].type)) || (iRAIL0 > iRAIL1)) {
            return 0;
  }
    if (iRAIL0 < iRAIL1) { 
        return 1; 
    }
    if (iALONG0 > iALONG1) { 
        return 0; 
    } else if (iALONG0 >= iALONG1) {
        return (fALONG0 < fALONG1);
    } else if (iALONG0 >= iALONG1) {
        return (fALONG0 < fALONG1);
    } 
  return 1;
}

//NGC MATCH
void InitCameraTargetMaterial() {
  ctmtl = NuMtlCreate(1);
  //mtl->attrib = (numtlattrib_s)((uint)attrib & 0x3fc3ffff | 0x200000);
  ctmtl->attrib.cull = 2;
  ctmtl->attrib.zmode = 0;
  ctmtl->diffuse.r = 0.5f;
  ctmtl->diffuse.g = 0.5f;
  ctmtl->diffuse.b = 0.5f;
  ctmtl->alpha = 1.0f;
  ctmtl->attrib.alpha = 0; 

  NuMtlUpdate(ctmtl);
  return;
}
