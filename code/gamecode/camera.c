void ResetGameCameras(CamMtx *Gamecam,int n)

{
  if (0 < n) {
    do {
      Gamecam->mode = -1;
      n = n + -1;
      Gamecam->judder = 0.0;
      Gamecam->blend_time = 0.0;
      Gamecam->blend_duration = 0.0;
      Gamecam->distance = 0.0;
      Gamecam->ahead = 0.0;
      Gamecam->vertical = '\0';
      Gamecam = Gamecam + 1;
    } while (0 < n);
    return;
  }
  return;
}


void JudderGameCamera(CamMtx *cam,float time,nuvec_s *pos)

{
  double dVar1;
  float fVar2;
  
  dVar1 = (double)time;
  if ((double)cam->judder < dVar1) {
    if (pos == (nuvec_s *)0x0) {
      cam->judder = time;
    }
    else {
      fVar2 = NuVecDist(&(player->obj).pos,pos,(nuvec_s *)0x0);
      if (fVar2 < 10.0) {
        cam->judder = (float)(dVar1 * (double)((10.0 - fVar2) / 10.0));
      }
    }
  }
  return;
}

void BlendGameCamera(CamMtx *cam,float time)

{
  float y;
  float z;
  
  y = (cam->newpos).y;
  z = (cam->newpos).z;
  (cam->oldpos).x = (cam->newpos).x;
  (cam->oldpos).z = z;
  (cam->oldpos).y = y;
  cam->blend_duration = time;
  cam->old_xrot = cam->new_xrot;
  cam->old_yrot = cam->new_yrot;
  cam->old_zrot = cam->new_zrot;
  cam->blend_time = 0.0;
  return;
}


void InitRails(void)

{
  short sVar1;
  nugspline_s *spl;
  int cmp;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  Rail *rail;
  float fVar5;
  short len;
  char *tCmRail;
  
  nRAILS = 0;
  if (world_scene[0] != (nugscn_s *)0x0) {
    rail = Rail;
    iVar4 = 0;
    iVar3 = 8;
    do {
      tCmRail = tCamRail;
      uVar2 = *(undefined4 *)((int)tRailExt + iVar4);
      rail->out_distance = 25.0;
      rail->type = -1;
      rail->in_distance = 25.0;
      rail->pINPLAT = (nugspline_s *)0x0;
      rail->pINCAM = (nugspline_s *)0x0;
      rail->pLEFT = (nugspline_s *)0x0;
      rail->pCAM = (nugspline_s *)0x0;
      rail->pRIGHT = (nugspline_s *)0x0;
      rail->pOUTCAM = (nugspline_s *)0x0;
      rail->pOUTPLAT = (nugspline_s *)0x0;
      rail->circuit = '\0';
      sprintf(tbuf,"%s%s",tCmRail,uVar2);
      spl = NuSplineFind(world_scene[0],tbuf);
      rail->pCAM = spl;
      if (spl != (nugspline_s *)0x0) {
        len = spl->len;
        if (1 < len) {
          sprintf(tbuf,"%s%s",tLeftRail,*(undefined4 *)((int)tRailExt + iVar4));
          spl = NuSplineFind(world_scene[0],tbuf);
          rail->pLEFT = spl;
          if (spl != (nugspline_s *)0x0) {
            sVar1 = spl->len;
            if (sVar1 == len) {
              sprintf(tbuf,"%s%s",tRightRail,*(undefined4 *)((int)tRailExt + iVar4));
              spl = NuSplineFind(world_scene[0],tbuf);
              rail->pRIGHT = spl;
              tCmRail = tInPlatRail;
              if (spl != (nugspline_s *)0x0) {
                if (spl->len == sVar1) {
                  uVar2 = *(undefined4 *)((int)tRailExt + iVar4);
                  rail->edges = spl->len + -1;
                  sprintf(tbuf,"%s%s",tCmRail,uVar2);
                  spl = NuSplineFind(world_scene[0],tbuf);
                  rail->pINPLAT = spl;
                  if (spl != (nugspline_s *)0x0) {
                    fVar5 = SplineDistance(spl);
                    rail->in_distance = fVar5;
                  }
                  sprintf(tbuf,"%s%s",tInCamRail,*(undefined4 *)((int)tRailExt + iVar4));
                  spl = NuSplineFind(world_scene[0],tbuf);
                  rail->pINCAM = spl;
                  sprintf(tbuf,"%s%s",tOutPlatRail,*(undefined4 *)((int)tRailExt + iVar4));
                  spl = NuSplineFind(world_scene[0],tbuf);
                  rail->pOUTPLAT = spl;
                  if (spl != (nugspline_s *)0x0) {
                    fVar5 = SplineDistance(spl);
                    rail->out_distance = fVar5;
                  }
                  sprintf(tbuf,"%s%s",tOutCamRail,*(undefined4 *)((int)tRailExt + iVar4));
                  spl = NuSplineFind(world_scene[0],tbuf);
                  rail->pOUTCAM = spl;
                  cmp = strcmp(*(char **)((int)tRailExt + iVar4),"bonus");
                  if (cmp == 0) {
                    rail->type = '\x01';
                  }
                  else {
                    cmp = strcmp(*(char **)((int)tRailExt + iVar4),"death");
                    if (cmp == 0) {
                      rail->type = '\x02';
                    }
                    else {
                      cmp = strcmp(*(char **)((int)tRailExt + iVar4),"gem");
                      if (cmp == 0) {
                        rail->type = '\x03';
                      }
                      else {
                        rail->type = '\0';
                      }
                    }
                  }
                  nRAILS = nRAILS + 1;
                }
              }
            }
          }
        }
      }
      iVar4 = iVar4 + 4;
      rail = rail + 1;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
  }
  return;
}


float BestRailPosition(nuvec_s *pos,RPos_s *rpos,int iRAIL,int iALONG)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  short sVar4;
  int iVar5;
  char *pcVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int unaff_r20;
  uint uVar10;
  uint uVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  char *pcVar15;
  byte bVar16;
  double dVar17;
  double dVar18;
  double in_f31;
  float fVar19;
  nuvec_s local_a0;
  nuvec_s local_90;
  nuvec_s local_80;
  nuvec_s local_70;
  nuvec_s local_60;
  
  bVar2 = false;
  if ((Level == 6) || (Level == 0x22)) {
    bVar2 = true;
  }
  bVar2 = !bVar2;
  if (bVar2) {
    local_a0.x = pos->x;
    local_a0.z = pos->z;
    local_a0.y = pos->y;
  }
  else {
    local_a0.z = pos->z;
    local_a0.y = pos->x;
    local_a0.x = -pos->y;
  }
  dVar17 = 0.0;
  rpos->iRAIL = -1;
  rpos->iALONG = -1;
  rpos->fALONG = 0.0;
  rpos->fACROSS = 0.0;
  if (Rail[iRAIL].type != -1) {
    bVar16 = (iALONG == -1) << 1;
    if (iALONG == -1) {
      iALONG = (int)Rail[iRAIL].edges / 2;
    }
    uVar11 = 0;
    uVar10 = 0;
    iVar12 = iALONG + 1;
    iVar3 = iALONG + -1;
    dVar17 = in_f31;
LAB_80009f60:
    iVar7 = iVar12;
    iVar8 = iVar3;
    iVar14 = iALONG;
    if (uVar11 != 0) {
      if (uVar11 == 1) {
        iVar7 = iVar12 + 1;
        iVar14 = iVar12;
      }
      else {
        iVar8 = iVar3 + -1;
        iVar14 = iVar3;
      }
    }
    if ((iVar14 < 0) || (Rail[iRAIL].edges <= iVar14)) {
      uVar10 = uVar10 | uVar11;
    }
    else {
      iVar12 = iVar14 + 1;
      if ((iVar12 == Rail[iRAIL].edges) && (Rail[iRAIL].circuit != '\0')) {
        iVar12 = 0;
      }
      iVar3 = (int)(Rail[iRAIL].pLEFT)->ptsize;
      iVar9 = (int)(Rail[iRAIL].pRIGHT)->ptsize;
      pcVar15 = (Rail[iRAIL].pRIGHT)->pts;
      iVar5 = iVar12 * iVar3;
      pcVar6 = (Rail[iRAIL].pLEFT)->pts;
      iVar13 = iVar14 * iVar9;
      iVar3 = iVar14 * iVar3;
      iVar9 = iVar12 * iVar9;
      if (bVar2) {
        local_90.x = *(float *)(pcVar6 + iVar3);
        local_90.z = *(float *)(pcVar6 + iVar3 + 8);
        local_90.y = *(float *)(pcVar6 + iVar3 + 4);
        local_80.x = *(float *)(pcVar6 + iVar5);
        local_80.z = *(float *)(pcVar6 + iVar5 + 8);
        local_80.y = *(float *)(pcVar6 + iVar5 + 4);
        local_70.x = *(float *)(pcVar15 + iVar9);
        local_70.z = *(float *)(pcVar15 + iVar9 + 8);
        local_70.y = *(float *)(pcVar15 + iVar9 + 4);
        local_60.x = *(float *)(pcVar15 + iVar13);
        local_60.z = *(float *)(pcVar15 + iVar13 + 8);
        local_60.y = *(float *)(pcVar15 + iVar13 + 4);
      }
      else {
        local_90.x = -*(float *)(pcVar6 + iVar3 + 4);
        local_90.y = *(float *)(pcVar6 + iVar3);
        local_90.z = *(float *)(pcVar6 + iVar3 + 8);
        local_80.x = -*(float *)(pcVar6 + iVar5 + 4);
        local_80.y = *(float *)(pcVar6 + iVar5);
        local_80.z = *(float *)(pcVar6 + iVar5 + 8);
        local_70.x = -*(float *)(pcVar15 + iVar9 + 4);
        local_70.y = *(float *)(pcVar15 + iVar9);
        local_70.z = *(float *)(pcVar15 + iVar9 + 8);
        local_60.x = -*(float *)(pcVar15 + iVar13 + 4);
        local_60.y = *(float *)(pcVar15 + iVar13);
        local_60.z = *(float *)(pcVar15 + iVar13 + 8);
      }
      if ((((0.0 <= (local_a0.x - local_90.x) * (local_80.z - local_90.z) +
                    (local_a0.z - local_90.z) * (local_90.x - local_80.x)) &&
           (0.0 <= (local_a0.x - local_80.x) * (local_70.z - local_80.z) +
                   (local_a0.z - local_80.z) * (local_80.x - local_70.x))) &&
          (0.0 <= (local_a0.x - local_70.x) * (local_60.z - local_70.z) +
                  (local_a0.z - local_70.z) * (local_70.x - local_60.x))) &&
         (0.0 <= (local_a0.x - local_60.x) * (local_90.z - local_60.z) +
                 (local_a0.z - local_60.z) * (local_60.x - local_90.x))) {
        fVar19 = (local_90.y + local_80.y + local_70.y + local_60.y) * 0.25 - local_a0.y;
        dVar18 = (double)fVar19;
        NuFabs(fVar19);
        if ((bool)(bVar16 >> 1 & 1)) {
          if ((rpos->iALONG == -1) || (dVar18 < dVar17)) {
            unaff_r20 = 1;
            dVar17 = dVar18;
          }
        }
        else {
          unaff_r20 = 2;
          dVar17 = dVar18;
        }
        if (unaff_r20 != 0) {
          sVar4 = (short)iVar12 + 1;
          rpos->iALONG = (short)iVar14;
          rpos->iRAIL = (char)iRAIL;
          rpos->i1 = (short)iVar12;
          rpos->i2 = sVar4;
          if ((sVar4 == Rail[iRAIL].edges) && (Rail[iRAIL].circuit != '\0')) {
            rpos->i2 = 0;
          }
          fVar19 = RatioBetweenEdges(&local_a0,&local_60,&local_90,&local_70,&local_80);
          rpos->fALONG = fVar19;
          fVar19 = RatioBetweenEdges(&local_a0,&local_90,&local_80,&local_60,&local_70);
          rpos->fACROSS = fVar19;
          if (unaff_r20 == 2) goto LAB_8000a2b0;
        }
      }
    }
    if (uVar10 == 3) goto LAB_8000a2b0;
    bVar1 = uVar11 == 1;
    uVar11 = 1;
    iVar12 = iVar7;
    iVar3 = iVar8;
    if (bVar1) {
      uVar11 = 2;
    }
    goto LAB_80009f60;
  }
  goto LAB_8000a3d0;
LAB_8000a2b0:
  temp_iALONG = (int)rpos->iALONG;
  temp_fALONG = rpos->fALONG;
  temp_iRAIL = (int)rpos->iRAIL;
  temp_fACROSS = rpos->fACROSS;
  if ((rpos->iRAIL == -1) || (temp_iALONG == -1)) goto LAB_8000a3d0;
  iVar3 = (int)rpos->i2;
  iVar12 = (int)rpos->i1;
  rpos->vertical = '\0';
  if ((iVar3 == iVar12) || (Rail[iRAIL].edges <= iVar3)) {
LAB_8000a394:
    if (bVar2) {
      RailInfo(rpos,&rpos->pos,&rpos->angle,&rpos->cam_angle,&rpos->mode);
      goto LAB_8000a3d0;
    }
  }
  else if (bVar2) {
    iVar8 = (int)(Rail[iRAIL].pLEFT)->ptsize;
    pcVar6 = (Rail[iRAIL].pLEFT)->pts;
    iVar7 = iVar3 * iVar8;
    iVar8 = iVar12 * iVar8;
    if ((*(float *)(pcVar6 + iVar8) == *(float *)(pcVar6 + iVar7)) &&
       (*(float *)(pcVar6 + iVar8 + 8) == *(float *)(pcVar6 + iVar7 + 8))) {
      iVar7 = (int)(Rail[iRAIL].pRIGHT)->ptsize;
      pcVar6 = (Rail[iRAIL].pRIGHT)->pts;
      iVar3 = iVar3 * iVar7;
      iVar12 = iVar12 * iVar7;
      if ((*(float *)(pcVar6 + iVar12) == *(float *)(pcVar6 + iVar3)) &&
         (*(float *)(pcVar6 + iVar12 + 8) == *(float *)(pcVar6 + iVar3 + 8))) {
        rpos->vertical = '\x01';
      }
    }
    goto LAB_8000a394;
  }
  RailInfo(rpos,&rpos->pos,(ushort *)0x0,&rpos->cam_angle,(uchar *)0x0);
LAB_8000a3d0:
  return (float)dVar17;
}


void ComplexRailPosition(nuvec_s *pos,int iRAIL,int iALONG,RPos_s *rpos,int set)

{
  bool bVar1;
  bool bVar2;
  RPos_s *pRVar3;
  uint uVar4;
  RPos_s *tmp;
  RPos_s *pRVar5;
  uint uVar6;
  RPos_s *pRVar7;
  int iVar8;
  uint unaff_r23;
  uint unaff_r24;
  uint unaff_r25;
  int iVar9;
  RPos_s *pRVar10;
  byte bVar11;
  double in_f31;
  float fVar12;
  
  if (set == 0) {
    tmp = temp_cRPos;
  }
  else {
    tmp = cRPos;
  }
  bVar11 = (set == 0) << 1;
  if (set != 0) {
    unaff_r23 = (uint)(Bonus - 1U < 3);
    unaff_r24 = (uint)(Death - 1U < 3);
    unaff_r25 = (uint)(GemPath - 1U < 3);
  }
  iVar9 = 0;
  pRVar10 = (RPos_s *)0x0;
  if (iRAIL == -1) {
    iRAIL = 0;
    iALONG = -1;
  }
  iVar8 = 0;
  bVar2 = true;
  do {
    if (((bool)(bVar11 >> 1 & 1)) ||
       (((((unaff_r23 == 0 || (Rail[iRAIL].type == '\x01')) &&
          ((unaff_r24 == 0 || (Rail[iRAIL].type == '\x02')))) &&
         ((unaff_r25 == 0 || (Rail[iRAIL].type == '\x03')))) &&
        ((unaff_r23 != 0 || (((unaff_r24 != 0 || (unaff_r25 != 0)) || (Rail[iRAIL].type == '\0'))) ))
        ))) {
      fVar12 = BestRailPosition(pos,tmp,iRAIL,iALONG);
      if ((tmp->iRAIL != -1) && (tmp->iALONG != -1)) {
        if ((pRVar10 == (RPos_s *)0x0) || ((double)fVar12 < in_f31)) {
          pRVar10 = tmp;
          in_f31 = (double)fVar12;
        }
        bVar1 = iVar9 == 2;
        tmp = tmp + 1;
        iVar9 = iVar9 + 1;
        bVar2 = pRVar10 == (RPos_s *)0x0;
        if (bVar1) break;
      }
    }
    uVar6 = iRAIL + 1;
    uVar4 = uVar6;
    if ((int)uVar6 < 0) {
      uVar4 = iRAIL + 8;
    }
    iVar8 = iVar8 + 1;
    iRAIL = uVar6 - (uVar4 & 0xfffffff8);
    iALONG = -1;
  } while (iVar8 < 8);
  if (bVar2) {
    rpos->iRAIL = -1;
    rpos->iALONG = -1;
  }
  else {
    iVar8 = 0x18;
    tmp = pRVar10;
    pRVar3 = rpos;
    do {
      pRVar7 = pRVar3;
      pRVar5 = tmp;
      iVar8 = iVar8 + -0x18;
      *(undefined4 *)pRVar7 = *(undefined4 *)pRVar5;
      *(undefined4 *)&pRVar7->i1 = *(undefined4 *)&pRVar5->i1;
      pRVar7->fALONG = pRVar5->fALONG;
      pRVar7->fACROSS = pRVar5->fACROSS;
      *(undefined4 *)&pRVar7->angle = *(undefined4 *)&pRVar5->angle;
      *(undefined4 *)&pRVar7->mode = *(undefined4 *)&pRVar5->mode;
      tmp = (RPos_s *)&pRVar5->pos;
      pRVar3 = (RPos_s *)&pRVar7->pos;
    } while (iVar8 != 0);
    (pRVar7->pos).x = (pRVar5->pos).x;
    (pRVar7->pos).y = (pRVar5->pos).y;
    (pRVar7->pos).z = (pRVar5->pos).z;
  }
  if (!(bool)(bVar11 >> 1 & 1)) {
    best_cRPos = pRVar10;
    cRPosCOUNT = iVar9;
    pRVar10 = temp_best_cRPos;
    iVar9 = temp_cRPosCOUNT;
  }
  temp_cRPosCOUNT = iVar9;
  temp_best_cRPos = pRVar10;
  temp_iRAIL = (int)rpos->iRAIL;
  temp_iALONG = (int)rpos->iALONG;
  temp_fALONG = rpos->fALONG;
  temp_fACROSS = rpos->fACROSS;
  return;
}


void MoveRailPosition(nuvec_s *dst,RPos_s *rpos,float distance,int direction)

{

//TODO

}


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


void MoveGameCamera(CamMtx *GameCamera,obj_s *obj)

{

//TODO

}


void GetALONG(nuvec_s *pos,RPos_s *rpos,int iRAIL,int iALONG,int info)

{
  if (rpos == (RPos_s *)0x0) {
    rpos = &TempRPos;
  }
  if (nRAILS == 0) {
    temp_iRAIL = -1;
    temp_iALONG = -1;
  }
  else {
    ComplexRailPosition(pos,iRAIL,iALONG,rpos,0);
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
        return (uint)(fALONG1 < fALONG0);
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
        return (uint)(fALONG0 < fALONG1);
      }
      return 1;
    }
  }
  return 0;
}


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
}
