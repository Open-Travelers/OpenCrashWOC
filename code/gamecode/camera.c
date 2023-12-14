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

void RailInfo(RPos_s *RPos,nuvec_s *pos,ushort *yrot,ushort *cam_yrot,uchar *mode)	//TODO

{
  float fVar1;
  float fVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  char *pcVar6;
  int iVar7;
  char *pcVar8;
  char *pcVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  double dVar13;
  double dVar14;
  double dVar15;
  double dVar16;
  float fVar17;
  nuvec_s local_70;
  nuvec_s local_48;

  iVar3 = (int)RPos->iRAIL;
  if ((iVar3 != -1) && (iVar12 = (int)RPos->iALONG, iVar12 != -1)) {
    iVar11 = iVar12 + 1;
    if ((iVar11 == Rail[iVar3].edges) && (Rail[iVar3].circuit != '\0')) {
      iVar11 = 0;
    }
    if (pos != (nuvec_s *)0x0) {
      iVar7 = (int)(Rail[iVar3].pCAM)->ptsize;
      pcVar9 = (Rail[iVar3].pCAM)->pts;
      iVar5 = iVar11 * iVar7;
      iVar7 = iVar12 * iVar7;
      pos->x = (*(float *)(pcVar9 + iVar5) - *(float *)(pcVar9 + iVar7)) * RPos->fALONG +
               *(float *)(pcVar9 + iVar7);
      pos->y = (*(float *)(pcVar9 + iVar5 + 4) - *(float *)(pcVar9 + iVar7 + 4)) * RPos->fALONG +
               *(float *)(pcVar9 + iVar7 + 4);
      pos->z = (*(float *)(pcVar9 + iVar5 + 8) - *(float *)(pcVar9 + iVar7 + 8)) * RPos->fALONG +
               *(float *)(pcVar9 + iVar7 + 8);
    }
    if (yrot != (ushort *)0x0) {
      iVar7 = iVar12 * (Rail[iVar3].pLEFT)->ptsize;
      pcVar9 = (Rail[iVar3].pLEFT)->pts;
      pcVar6 = (Rail[iVar3].pRIGHT)->pts;
      iVar5 = iVar12 * (Rail[iVar3].pRIGHT)->ptsize;
      dVar14 = 0.5;
      fVar17 = (*(float *)(pcVar9 + iVar7) + *(float *)(pcVar6 + iVar5)) * 0.5;
      fVar1 = (*(float *)(pcVar9 + iVar7 + 8) + *(float *)(pcVar6 + iVar5 + 8)) * 0.5;
      iVar5 = NuAtan2D(*(float *)(pcVar6 + iVar5) - *(float *)(pcVar9 + iVar7),
                       *(float *)(pcVar6 + iVar5 + 8) - *(float *)(pcVar9 + iVar7 + 8));
      iVar7 = iVar11 * (Rail[iVar3].pRIGHT)->ptsize;
      pcVar6 = (Rail[iVar3].pRIGHT)->pts;
      pcVar9 = (Rail[iVar3].pLEFT)->pts;
      iVar10 = iVar11 * (Rail[iVar3].pLEFT)->ptsize;
      dVar16 = (double)(*(float *)(pcVar9 + iVar10) + *(float *)(pcVar6 + iVar7)) * dVar14;
      dVar15 = (double)(*(float *)(pcVar9 + iVar10 + 8) + *(float *)(pcVar6 + iVar7 + 8)) * dVar14 ;
      iVar7 = NuAtan2D(*(float *)(pcVar6 + iVar7) - *(float *)(pcVar9 + iVar10),
                       *(float *)(pcVar6 + iVar7 + 8) - *(float *)(pcVar9 + iVar10 + 8));
      uVar4 = RotDiff((ushort)iVar5,(ushort)iVar7);
      local_48._0_8_ = (double)CONCAT44(0x43300000,uVar4 ^ 0x80000000);
      uVar4 = (iVar5 + (int)(RPos->fALONG * (float)(local_48._0_8_ - 4503601774854144.0))) - 0x400 0;
      *yrot = (ushort)uVar4;
      if (mode != (uchar *)0x0) {
        if (pos == (nuvec_s *)0x0) {
          *mode = '\0';
        }
        else {
          uVar4 = uVar4 & 0xffff;
          dVar15 = (double)((((float)dVar15 - fVar1) * RPos->fALONG + fVar1) - pos->z);
          fVar1 = NuTrigTable[uVar4];
          fVar2 = *(float *)((int)NuTrigTable + ((uVar4 + 0x4000) * 4 & 0x3fffc));
          dVar16 = (double)((((float)dVar16 - fVar17) * RPos->fALONG + fVar17) - pos->x);
          fVar17 = NuFsqrt((float)(dVar16 * dVar16 + (double)(float)(dVar15 * dVar15)));
          fVar17 = fVar1 * (float)(dVar16 * (double)(1.0 / fVar17)) +
                   fVar2 * (float)(dVar15 * (double)(1.0 / fVar17));
          if (fVar17 <= 0.866) {
            if (-0.866 <= fVar17) {
              dVar13 = (double)fVar17;
              NuFabs(fVar17);
              if (dVar14 <= dVar13) {
                *mode = '\0';
              }
              else {
                iVar5 = NuAtan2D((float)dVar16,(float)dVar15);
                iVar5 = RotDiff((ushort)iVar5,*yrot);
                if (iVar5 < 1) {
                  *mode = '\b';
                }
                else {
                  *mode = '\x04';
                }
              }
            }
            else {
              *mode = '\x02';
            }
          }
          else {
            *mode = '\x01';
          }
        }
      }
    }
    if (cam_yrot != (ushort *)0x0) {
      iVar5 = iVar12 * (Rail[iVar3].pLEFT)->ptsize;
      pcVar8 = (Rail[iVar3].pLEFT)->pts;
      pcVar9 = (Rail[iVar3].pRIGHT)->pts;
      iVar7 = iVar12 * (Rail[iVar3].pRIGHT)->ptsize;
      pcVar6 = (Rail[iVar3].pCAM)->pts;
      iVar12 = iVar12 * (Rail[iVar3].pCAM)->ptsize;
      local_70.x = *(float *)(pcVar6 + iVar12);
      if ((Level == 6) || (Level == 0x22)) {
        local_70.x = local_70.x + 5.0;
      }
      iVar12 = NuAtan2D((*(float *)(pcVar8 + iVar5) + *(float *)(pcVar9 + iVar7)) * 0.5 - local_70 .x
                        ,(*(float *)(pcVar8 + iVar5 + 8) + *(float *)(pcVar9 + iVar7 + 8)) * 0.5 -
                         *(float *)(pcVar6 + iVar12 + 8));
      iVar5 = iVar11 * (Rail[iVar3].pRIGHT)->ptsize;
      pcVar9 = (Rail[iVar3].pLEFT)->pts;
      pcVar8 = (Rail[iVar3].pRIGHT)->pts;
      iVar7 = iVar11 * (Rail[iVar3].pLEFT)->ptsize;
      pcVar6 = (Rail[iVar3].pCAM)->pts;
      iVar11 = iVar11 * (Rail[iVar3].pCAM)->ptsize;
      local_70.x = *(float *)(pcVar6 + iVar11);
      if ((Level == 6) || (Level == 0x22)) {
        local_70.x = local_70.x + 5.0;
      }
      iVar3 = NuAtan2D((*(float *)(pcVar9 + iVar7) + *(float *)(pcVar8 + iVar5)) * 0.5 - local_70. x,
                       (*(float *)(pcVar9 + iVar7 + 8) + *(float *)(pcVar8 + iVar5 + 8)) * 0.5 -
                       *(float *)(pcVar6 + iVar11 + 8));
      uVar4 = RotDiff((ushort)iVar12,(ushort)iVar3);
      local_48._0_8_ = (double)CONCAT44(0x43300000,uVar4 ^ 0x80000000);
      *cam_yrot = (ushort)iVar12 +
                  (short)(int)(RPos->fALONG * (float)(local_48._0_8_ - 4503601774854144.0));
    }
  }
  return;
}



float LookUpDownRail(obj_s *obj,ushort yrot,int mode)		//TODO

{
  bool bVar1;
  int drot;
  int iVar2;
  uint uVar3;
  double dVar4;

  if (((Level == 8) && ((player->obj).RPos.iRAIL == '\0')) &&
     ((ushort)(player->obj).RPos.iALONG - 0x2d < 4)) {
LAB_8000afb8:
    dVar4 = 1.0;
  }
  else {
    if ((Level == 0x11) && ((mode & 2U) != 0)) goto LAB_8000afb8;
    if (((Level == 1) && ((player->obj).RPos.iRAIL == '\0')) &&
       ((uVar3 = (uint)(ushort)(player->obj).RPos.iALONG, uVar3 < 200 || (uVar3 - 0xea < 0x20))))  {
      dVar4 = -1.25;
    }
    else if ((Level == 0xc) &&
            (((player->obj).RPos.iRAIL == '\0' && ((ushort)(player->obj).RPos.iALONG < 0x10)))) {
LAB_8000b078:
      dVar4 = -1.0;
    }
    else if (((Level == 7) && ((player->obj).RPos.iRAIL == '\0')) &&
            ((uVar3 = (uint)(ushort)(player->obj).RPos.iALONG, uVar3 - 0x9e < 0xc ||
             (uVar3 - 0x6a < 0x24)))) {
      dVar4 = -1.350000023841858;
    }
    else {
      if ((Level == 0x13) && (Death == 2)) goto LAB_8000b078;
      dVar4 = 0.0;
    }
  }
  drot = RotDiff(yrot,obj->hdg);
  iVar2 = (drot >> 0x1f ^ drot) - (drot >> 0x1f);
  if (iVar2 < 0x2aab) {
    bVar1 = false;
  }
  else {
    if (iVar2 < 0x5556) goto LAB_8000b0dc;
    bVar1 = true;
  }
  if (((mode & 3U) != 0) && (bVar1)) {
    dVar4 = (double)(float)(dVar4 - 2.0);
  }
LAB_8000b0dc:
  return (float)dVar4;
}



int InSplineArea(nuvec_s *pos,nugspline_s *spl)

{
  float fVar1;
  float fVar2;
  int len;
  nuvec_s *p0;
  int j;
  nuvec_s *p1;
  int i;

  len = (int)spl->len;
  i = 1;
  if (1 < len) {
    p0 = (nuvec_s *)spl->pts;
    do {
      p1 = (nuvec_s *)(i * spl->ptsize);
      i = i + 1;
      j = i;
      if (i == len) {
        j = 1;
      }
      j = j * spl->ptsize;
      fVar2 = *(float *)((int)&p1->x + (int)&p0->x);
      fVar1 = *(float *)((int)&p1->z + (int)&p0->x);
      if ((pos->x - fVar2) * (*(float *)((int)&p0->z + j) - fVar1) +
          (pos->z - fVar1) * (fVar2 - *(float *)((int)&p0->x + j)) < 0.0) {
        return 0;
      }
    } while (i < len);
  }
  return 1;
}
*/

void MoveGameCamera(struct cammtx_s *GameCamera,struct obj_s *obj)

{

//TODO

}


void GetALONG(struct nuvec_s *pos,struct RPos_s *rpos,int iRAIL,int iALONG,int info)

{
  if (rpos == NULL) {
    rpos = &TempRPos;
  }
  if (nRAILS == 0) {
    temp_iRAIL = -1;
    temp_iALONG = -1;
  }
  else {
    //ComplexRailPosition(pos,iRAIL,iALONG,rpos,0);
  }
  return;
}


int FurtherALONG(int iRAIL0,int iALONG0,float fALONG0,int iRAIL1,int iALONG1,float fALONG1)

{
  if ((((iRAIL0 != -1) && (iRAIL1 != -1)) && (Rail[iRAIL0].type == Rail[iRAIL1].type)) &&
     (iRAIL1 <= iRAIL0)) {
    if (iRAIL1 < iRAIL0) {
      return 1;
    }
    if (iALONG1 <= iALONG0) {
      if (iALONG0 <= iALONG1) {
        return (fALONG1 < fALONG0);
      }
      return 1;
    }
  }
  return 0;
}


int FurtherBEHIND(int iRAIL0,int iALONG0,float fALONG0,int iRAIL1,int iALONG1,float fALONG1)

{
  if ((((iRAIL0 != -1) && (iRAIL1 != -1)) && (Rail[iRAIL0].type == Rail[iRAIL1].type)) &&
     (iRAIL0 <= iRAIL1)) {
    if (iRAIL0 < iRAIL1) {
      return 1;
    }
    if (iALONG0 <= iALONG1) {
      if (iALONG1 <= iALONG0) {
        return (fALONG0 < fALONG1);
      }
      return 1;
    }
  }
  return 0;
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
