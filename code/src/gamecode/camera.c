#include "gamecode/camera.h"

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

//96%
void InitRails(void) {
    s32 n;
    s32 i;
    s16 camlen;
    s16 leftlen; 
    s16 rightlen;
    struct rail_s* rail;

    
    nRAILS = 0;
    if (world_scene[0] != NULL) {
        for (i = 0, n = 8; n != 0;  n--, i++) {
            rail = &Rail[i];
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
            rail->circuit = '\0';
            sprintf(tbuf,"%s%s",tCamRail,tRailExt[i]);
            rail->pCAM = NuSplineFind(world_scene[0],tbuf);
            if (rail->pCAM != NULL) {
                camlen = rail->pCAM->len;
                if (1 < camlen) {
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
                                    rail->edges = rightlen + -1;
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
                                        rail->type = '\x01';
                                    }
                                    else {
                                        if (strcmp(tRailExt[i],"death") == 0) {
                                            rail->type = '\x02';
                                        }
                                        else {
                                            if (strcmp(tRailExt[i],"gem") == 0) {
                                                rail->type = '\x03';
                                            }
                                            else {
                                                rail->type = '\0';
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


//86%
float BestRailPosition(struct NuVec* pos, struct RPos_s* rpos, s32 iRAIL, s32 iALONG) {
    struct NuVec local_a0;
    struct NuVec local_90;
    struct NuVec local_80;
    struct NuVec local_70;
    struct NuVec local_60;
    struct NuVec* pcVar6;
    struct NuVec* pcVar15;
    BOOL bVar2;
    s32 iVar3;
    s32 iVar5;
    s32 iVar7;
    s32 iVar8;
    s32 iVar9;
    s32 unaff_r20;
    u32 uVar10;
    u32 uVar11;
    s32 iVar12;
    s32 iVar13;
    s32 iVar14;
    float dbest;
    float d;
    float y;
    struct rail_s* rail;
    int r28;
    int r24;
    int r23;
    int r22;
    int r21;
    int r20;
    int r19;
    int r18;

    bVar2 = 0;
    if ((Level == 6) || (Level == 0x22)) {
        bVar2 = 1;
    }
    if (bVar2) {
        local_a0.x = -pos->y;
        local_a0.y = pos->x;
        local_a0.z = pos->z;
    } else {
        local_a0 = *pos;
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
        r23 = (s32)rail->edges / 2;
    } else {
        r23 = iALONG;
    }
    r18 = r23 + 1;
    r19 = r23 - 1;
    r22 = 0;
    r21 = 0;
    uVar11 = 0;
    uVar10 = 0;
    while (1) {
        if (r22 == 0) {
            r28 = r23;
        } else if (r22 == 1) {
            r28 = r18;
            r18++;
        } else {
            r28 = r19;
            r19--;
        }
        if (r28 >= 0 && r28 < rail->edges) {
            struct NuVec* r5;
            struct NuVec* r12;
            struct NuVec* r9;
            struct NuVec* r30;
            r24 = r28 + 1;
            if ((r24 == rail->edges) && (rail->circuit != 0)) {
                r24 = 0;
            }
            r12 = (char*)rail->pRIGHT->pts + (r28 * rail->pRIGHT->ptsize);
            r30 = (char*)rail->pRIGHT->pts + (r24 * rail->pRIGHT->ptsize);
            r5 = (char*)rail->pLEFT->pts + (r24 * rail->pLEFT->ptsize);
            r9 = (char*)rail->pLEFT->pts + (r28 * rail->pLEFT->ptsize);
            if (bVar2) {
                local_90.x = -r9->y;
                local_90.y = r9->x;
                local_90.z = r9->z;

                local_80.x = -r5->y;
                local_80.y = r5->x;
                local_80.z = r5->z;

                local_70.x = -r30->y;
                local_70.y = r30->x;
                local_70.z = r30->z;

                local_60.x = -r12->y;
                local_60.y = r12->x;
                local_60.z = r12->z;
            } else {
                local_90 = *r9;
                local_80 = *r5;
                local_70 = *r30;
                local_60 = *r12;
            }
            if ((((0.0f <= (local_a0.x - local_90.x) * (local_80.z - local_90.z)
                       + (local_a0.z - local_90.z) * (local_90.x - local_80.x))
                  && (0.0f <= (local_a0.x - local_80.x) * (local_70.z - local_80.z)
                          + (local_a0.z - local_80.z) * (local_80.x - local_70.x)))
                 && (0.0f <= (local_a0.x - local_70.x) * (local_60.z - local_70.z)
                         + (local_a0.z - local_70.z) * (local_70.x - local_60.x)))
                && (0.0f <= (local_a0.x - local_60.x) * (local_90.z - local_60.z)
                        + (local_a0.z - local_60.z) * (local_60.x - local_90.x)))
            {
                d = NuFabs(((local_90.y + local_80.y + local_70.y + local_60.y) * 0.25f - local_a0.y));
                if (iALONG == -1 && (rpos->iALONG == -1 || d < dbest)) {
                    dbest = d;
                    r20 = 1;
                } else {
                    dbest = d;
                    r20 = 2;
                }
                if (r20 != 0) {
                    rpos->iALONG = r28;
                    rpos->iRAIL = iRAIL;
                    rpos->i1 = r24;
                    rpos->i2 = r24 + 1;
                    if ((rpos->i2 == rail->edges) && (rail->circuit != 0)) {
                        rpos->i2 = 0;
                    }
                    rpos->fALONG = RatioBetweenEdges(&local_a0, &local_60, &local_90, &local_70, &local_80);
                    rpos->fACROSS = RatioBetweenEdges(&local_a0, &local_90, &local_80, &local_60, &local_70);
                    if (r20 == 2) {
                        break;
                    }
                }
            }
        } else {
            r21 |= r22;
        }
        if (r21 == 3) {
            break;
        }
        r22 = (r22 == 1) ? 2 : 1;
    }
    temp_iRAIL = rpos->iRAIL;
    temp_iALONG = rpos->iALONG;
    temp_fALONG = rpos->fALONG;
    temp_fACROSS = rpos->fACROSS;

    if ((rpos->iRAIL != -1) && (temp_iALONG != -1)) {
        iVar3 = (s32)rpos->i2;
        iVar12 = (s32)rpos->i1;
        rpos->vertical = '\0';
        if ((iVar3 == iVar12) || (rail->edges <= iVar3)) {
            iVar8 = (s32)(rail->pLEFT)->ptsize;
            iVar7 = iVar3 * iVar8;
            iVar8 = iVar12 * iVar8;
            if ((pcVar6[iVar8].x == pcVar6[iVar7].x) && (pcVar6[iVar8].z == pcVar6[iVar7].z)) {
                iVar7 = (s32)(rail->pRIGHT)->ptsize;
                iVar3 = iVar3 * iVar7;
                iVar12 = iVar12 * iVar7;
                if ((pcVar6[iVar12].x == pcVar6[iVar3].x) && (pcVar6[iVar12].z == pcVar6[iVar3].z)) {
                    rpos->vertical = '\x01';
                }
            }
        }
        if (bVar2) {
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

void MoveRailPosition(struct nuvec_s *dst,struct RPos_s *rpos,float distance,int direction)

{

//TODO

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


//58% NGC
s32 InSplineArea(struct NuVec* pos, struct nugspline_s* spl) {
    struct NuVec* p0;
    struct NuVec* p1;
    s32 i;
    s32 j;


    for (i = 1; i < (s32)spl->len; i++) {
        p0 = (struct NuVec*)spl->pts;
        p1 = (struct NuVec*)((int)spl->pts + (i * spl->ptsize));
        j = i + 1;
        if (i == (s32)spl->len) {
            j = 1;
        }
        j = j * spl->ptsize;
        if ((pos[i].x - (p1->x + p0->x)) * (p0[j].z - (p1->z + p0->x))
             + (pos[i].z - (p1->z + p0->x)) * ((p1->x + p0->x) - p0[j].x) >= 0.0f)
        {
            return 0;
        }
    }
    return 1;
}*/

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

/*
void InitCameraTargetMaterial(void)

{
  numtlattrib_s attrib;
  numtl_s *mtl;

  mtl = NuMtlCreate(1);
  attrib = mtl->attrib;
  ctmtl = mtl;
  (mtl->diffuse).b = 0.5;
  mtl->alpha = 1.0;
  mtl->attrib = (numtlattrib_s)((uint)attrib & 0x3fc3ffff | 0x200000);
  (mtl->diffuse).r = 0.5;
  (mtl->diffuse).g = 0.5;
  NuMtlUpdate(mtl);
  return;
}*/
