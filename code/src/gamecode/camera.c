#include "gamecode/camera.h"

/*
	RailInfo	    73%*
	MoveGameCamera	    22%**
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
void JudderGameCamera(struct cammtx_s *cam,float time,struct NuVec *pos) {
    float d;
    char pad[3];

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
void ComplexRailPosition(struct NuVec *pos,s32 iRAIL,s32 iALONG,struct RPos_s *rpos,s32 set) {

    struct RPos_s *list;
    s32 i;
    s32 bonus;
    s32 death;
    s32 gempath;
    s32 count;
    struct RPos_s *best;
    float d;
    float dbest;
    char pad[7];
    
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

/*
//70% NGC
void RailInfo(struct RPos_s* RPos, struct nuvec_s* pos, u16* yrot, u16* cam_yrot, u8* mode) {
    struct rail_s* rail;
    struct nuvec_s* pL;
    struct nuvec_s* pR;
    struct nuvec_s* pC;
    struct nuvec_s* p0;
    struct nuvec_s* p1;
    struct nuvec_s v0; // r1+0x8
    struct nuvec_s v1; // r1+0x18
    struct nuvec_s mid; // r1+0x28
    struct nuvec_s vC; // r1+0x38
    float local_48;
    int i0;
    int iVar5;
    int iVar7;
    int iVar10;
    int iVar11;
    int iVar12;
    u16 a;
    float fVar1;
    float fVar2;
    float dVar13;
    float dVar15;
    float dVar16;
    float fVar17;

    if (RPos->iRAIL != -1 && RPos->iALONG != -1) {
        iVar12 = RPos->iALONG;
        rail = &Rail[RPos->iRAIL];
        iVar11 = iVar12 + 1;
        if ((iVar11 == rail->edges) && (rail->circuit != 0)) {
            iVar11 = 0;
        }
        if (pos != NULL) {
            p0 = rail->pCAM->pts + (iVar12 * rail->pCAM->ptsize);
            p1 = rail->pCAM->pts + (iVar11 * rail->pCAM->ptsize);
            pos->x = (p1->x - p0->x) * RPos->fALONG + p0->x;
            pos->y = (p1->y - p0->y) * RPos->fALONG + p0->y;
            pos->z = (p1->z - p0->z) * RPos->fALONG + p0->z;
        }
        if (yrot != NULL) {
            pL = (rail->pLEFT)->pts + (iVar12 * (rail->pLEFT)->ptsize);
            pR = (rail->pRIGHT)->pts + (iVar12 * (rail->pRIGHT)->ptsize);
            v0.x = (pL->x + pR->x) * 0.5f;
            v0.z = (pL->z + pR->z) * 0.5f;
            iVar5 = NuAtan2D(pR->x - pL->x, pR->z - pL->z);
            pL = (rail->pLEFT)->pts + (iVar11 * (rail->pLEFT)->ptsize);
            pR = (rail->pRIGHT)->pts + (iVar11 * (rail->pRIGHT)->ptsize);
            v1.x = (pL->x + pR->x) * 0.5f;
            v1.z = (pL->z + pR->z) * 0.5f;
            iVar7 = NuAtan2D(pR->x - pL->x, pR->z - pL->z);
            a = (iVar5 + (s32)(RPos->fALONG * RotDiff(iVar5, iVar7))) - 0x4000;
            *yrot = a;
            if (mode != NULL) {
                if (pos != NULL) {
                    dVar15 = (((dVar15 - fVar1) * RPos->fALONG + fVar1) - pos->z);
                    fVar1 = NuTrigTable[a];
                    fVar2 = NuTrigTable[(s32)(a + 0x4000) * 4 & 0x3fffc];
                    dVar16 = (((dVar16 - fVar17) * RPos->fALONG + fVar17) - pos->x);
                    fVar17 = NuFsqrt((dVar16 * dVar16 + (dVar15 * dVar15)));
                    fVar17 = fVar1 * (dVar16 * (1.0f / fVar17)) + fVar2 * (dVar15 * (1.0f / fVar17));
                    if (fVar17 > 0.866f) {
                        *mode = 1;
                    } else {
                        if (-0.866f > fVar17) {
                            *mode = 2;
                        } else {
                            if (NuFabs(fVar17) < 0.5f) {
                                if (RotDiff(NuAtan2D(dVar16, dVar15), *yrot) > 0) {
                                    *mode = 4;
                                } else {
                                    *mode = 8;
                                }
                            } else {
                                *mode = 0;
                            }
                        }
                    }
                } else {
                    *mode = 0;
                }
            }
        }
        if (cam_yrot != NULL) {
            iVar5 = iVar12 * (rail->pLEFT)->ptsize;
            iVar7 = iVar12 * (rail->pRIGHT)->ptsize;
            iVar12 = iVar12 * (rail->pCAM)->ptsize;
            pL = (rail->pLEFT)->pts + iVar5;
            pR = (rail->pRIGHT)->pts + iVar7;
            pC = (rail->pCAM)->pts + iVar12;
            vC = *pC;
            if ((Level == 6) || (Level == 0x22)) {
                vC.x += 5.0f;
            }
            iVar12 = NuAtan2D(v0.x - vC.x, v0.z - vC.z);
            pL = (rail->pLEFT)->pts + (iVar11 * (rail->pLEFT)->ptsize);
            pR = (rail->pRIGHT)->pts + (iVar11 * (rail->pRIGHT)->ptsize);
            pC = (rail->pCAM)->pts + (iVar11 * (rail->pCAM)->ptsize);
            vC = *pC;
            if ((Level == 6) || (Level == 0x22)) {
                vC.x += 5.0f;
            }
            i0 = NuAtan2D(v0.x - vC.x, v0.z - vC.z);
            *cam_yrot = (iVar12 + (s32)(RPos->fALONG * RotDiff(iVar12, i0)));
        }
    }
    return;
}
*/

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

void MoveGameCamera(struct cammtx_s *GameCamera,struct obj_s *obj)

{

//TODO

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
