//NGC MATCH
void InitBugAreas(void) {
  s32 index;
  s32 i;
  struct nuvec_s *vec;
  
  for(i = 0; i < 4; i++) {
    BugArea[i].in_iRAIL = -1;
    BugArea[i].out_iRAIL = -1;
  }
  if ((SplTab[67].spl != NULL)) {
    for(i = 0; i < (SplTab[67].spl)->len; i++) {
      vec = (struct nuvec_s *)(SplTab[67].spl->pts + (i * (s32)(SplTab[67].spl)->ptsize));
      GetALONG(vec,NULL,-1,-1,1);
      if ((temp_iRAIL != -1) && (Rail[temp_iRAIL].type == 0)) {
        if ((i & 1) != 0) {
          index = ((i + ((u32)i >> 0x1f)) & 0xFFFFFFFE) >> 1;
          BugArea[index].out_iRAIL = temp_iRAIL;
          BugArea[index].out_iALONG = temp_iALONG;
          BugArea[index].out_fALONG = temp_fALONG;
        }
        else {
          index = ((i + ((u32)i >> 0x1f)) & 0xFFFFFFFE) >> 1;
          BugArea[index].in_iRAIL = temp_iRAIL;
          BugArea[index].in_iALONG = temp_iALONG;
          BugArea[index].in_fALONG = temp_fALONG;
        }
      }
    }
  }
  bug_splratio = 0.0f;
  return;
}

//NGC MATCH
s32 InBugArea(s32 iRAIL,s32 iALONG,float fALONG) {
  s32 i;
  
  if ((iRAIL != -1) && (Rail[iRAIL].type == 0)) {
    for(i = 0; i < 4; i++) {
      if ((BugArea[i].in_iRAIL != -1) &&
         (FurtherALONG(iRAIL,iALONG,fALONG,BugArea[i].in_iRAIL,BugArea[i].in_iALONG,BugArea[i].in_fALONG) != 0)) {
        if (BugArea[i].out_iRAIL == -1) {
          return i;
        }
        if (FurtherBEHIND(iRAIL,iALONG,fALONG,BugArea[i].out_iRAIL,BugArea[i].out_iALONG,BugArea[i].out_fALONG) != 0) {
          return i;
        }
      }
    }
  }
  return -1;
}

//NGC MATCH
void ResetBug(void) {
  s32 i;
  
  ResetAnimPacket(&BugAnim,0x22);
  bug_fade = 0.0f;
  i = NearestSplinePoint(&(player->obj).pos,SplTab[70].spl);
  if (i != -1) {
    bug_splratio = ((float)i / (float)(s32)(SplTab[70].spl->len - 1U));
  }
  else {
    bug_splratio = 0.0f;
  }
  PointAlongSpline(SplTab[70].spl,bug_splratio,&bug_splpos,NULL,NULL);
  bug_splpos = bug_pos;
  buglight_enable = 0;
  return;
}


void UpdateBugLight(creature_s *c)		//TODO

{
  int iVar1;
  uint uVar2;
  double dVar3;
  double dVar4;
  double dVar5;
  double dVar6;
  double dVar7;
  double dVar8;
  double dVar9;
  nuvec_s oldpos;
  nuvec_s local_c8;
  nuvec_s nStack_b8;
  nuvec_s nStack_a8;
  nuvec_s pos;
  float fadetime;
  
  bug_fade = 0.0;
  if ((LBIT._4_4_ & 0x88100000) == 0) {
    bug_fade = 0.0;
    return;
  }
  if (((Level != 0x14) || (fadetime = BUGFADETIME, GemPath == 0)) &&
     ((iVar1 = InBugArea((int)(c->obj).RPos.iRAIL,(int)(c->obj).RPos.iALONG,(c->obj).RPos.fALONG),
      iVar1 == -1 ||
      ((fadetime = BUGFADETIME, (LBIT._4_4_ & 0x80100000) != 0 && (VEHICLECONTROL != 1)))))) {
    fadetime = 0.0;
  }
  dVar6 = (double)fadetime;
  iVar1 = qrand();
  if (iVar1 < 0) {
    iVar1 = iVar1 + 0x3f;
  }
  buglight_ang[3] = buglight_ang[3] + (short)((iVar1 >> 6) << 2) + (short)(iVar1 >> 6);
  if ((LBIT._4_4_ & 0x80100000) != 0) {
    dVar5 = (double)mechlight_fade;
    if (dVar5 <= dVar6) {
      if ((dVar6 <= dVar5) ||
         (mechlight_fade = (float)(dVar5 + 0.01666666753590107), (double)mechlight_fade <= dVar6))
      goto LAB_800093bc;
    }
    else {
      mechlight_fade = (float)(dVar5 - 0.01666666753590107);
      if (dVar6 <= (double)mechlight_fade) goto LAB_800093bc;
    }
    mechlight_fade = (float)dVar6;
LAB_800093bc:
    if (0.0 < mechlight_fade) {
      bug_pos.x = (player->obj).pos.x;
      bug_pos.y = (player->obj).top * (player->obj).SCALE + (player->obj).pos.y;
      bug_pos.z = (player->obj).pos.z;
      uVar2 = NuAtan2D(bug_pos.x - GameCam.pos.x,bug_pos.z - GameCam.pos.z);
      bug_pos.x = NuTrigTable[uVar2 & 0xffff] * mechlight_distance + bug_pos.x;
      bug_pos.z = *(float *)((int)NuTrigTable + (((uVar2 & 0xffff) + 0x4000) * 4 & 0x3fffc)) *
                  mechlight_distance + bug_pos.z;
      bug_scale = 1.5;
    }
    bug_fade = mechlight_fade;
    qrand();
    return;
  }
  if (Level != 0x1b) {
    return;
  }
  oldpos.x = bug_pos.x;
  oldpos.z = bug_pos.z;
  oldpos.y = bug_pos.y;
  if (SplTab[70].spl == (nugspline_s *)0x0) {
    bug_pos.x = (player->obj).pos.x;
    bug_pos.y = (player->obj).top * (player->obj).SCALE + (player->obj).pos.y;
    bug_pos.z = (player->obj).pos.z;
    uVar2 = NuAtan2D(bug_pos.x - GameCam.pos.x,bug_pos.z - GameCam.pos.z);
    bug_pos.x = NuTrigTable[uVar2 & 0xffff] * buglight_distance + bug_pos.x;
    bug_pos.z = *(float *)((int)NuTrigTable + (((uVar2 & 0xffff) + 0x4000) * 4 & 0x3fffc)) *
                buglight_distance + bug_pos.z;
    goto LAB_800097dc;
  }
  pos.x = (player->obj).pos.x;
  pos.z = (player->obj).pos.z;
  pos.y = (player->obj).pos.y;
  if (best_cRPos != (RPos_s *)0x0) {
    fadetime = buglight_distance;
    if ((best_cRPos->mode & 0xc) != 0) {
      fadetime = buglight_distance * 0.333;
    }
    pos.x = NuTrigTable[best_cRPos->angle] * fadetime + pos.x;
    pos.z = *(float *)((int)NuTrigTable + ((best_cRPos->angle + 0x4000) * 4 & 0x3fffc)) * fadetime  +
            pos.z;
  }
  dVar8 = (double)bug_splratio;
  PointAlongSpline(SplTab[70].spl,bug_splratio,&local_c8,(ushort *)0x0,(ushort *)0x0);
  fadetime = NuVecDistSqr(&pos,&local_c8,(nuvec_s *)0x0);
  dVar6 = (double)fadetime;
  dVar5 = 0.001000000047497451;
  if (c->sprint != '\0') {
    dVar5 = 0.002000000094994903;
  }
  dVar7 = (double)(float)((double)bug_splratio - dVar5);
  if ((double)(float)((double)bug_splratio - dVar5) < 0.0) {
    dVar7 = 0.0;
  }
  PointAlongSpline(SplTab[70].spl,(float)dVar7,&nStack_b8,(ushort *)0x0,(ushort *)0x0);
  fadetime = NuVecDistSqr(&pos,&nStack_b8,(nuvec_s *)0x0);
  dVar3 = (double)fadetime;
  dVar9 = (double)(float)((double)bug_splratio + dVar5);
  if (1.0 < (double)(float)((double)bug_splratio + dVar5)) {
    dVar9 = 1.0;
  }
  PointAlongSpline(SplTab[70].spl,(float)dVar9,&nStack_a8,(ushort *)0x0,(ushort *)0x0);
  fadetime = NuVecDistSqr(&pos,&nStack_a8,(nuvec_s *)0x0);
  dVar4 = (double)fadetime;
  if ((((dVar6 <= dVar3) || (dVar4 <= dVar3)) && (dVar7 = dVar8, dVar4 < dVar6)) && (dVar4 < dVar3 ))
  {
    dVar7 = dVar9;
  }
  dVar6 = (double)bug_splratio;
  if (dVar6 <= dVar7) {
    if ((dVar6 < dVar7) &&
       (bug_splratio = (float)((double)(float)(dVar5 * 0.01666666753590107) * 10.0 + dVar6),
       dVar7 < (double)bug_splratio)) goto LAB_800096c8;
  }
  else {
    bug_splratio = (float)(dVar6 - (double)((float)(dVar5 * 0.01666666753590107) * 10.0));
    if ((double)bug_splratio < dVar7) {
LAB_800096c8:
      bug_splratio = (float)dVar7;
    }
  }
  PointAlongSpline(SplTab[70].spl,bug_splratio,&pos,(ushort *)0x0,(ushort *)0x0);
  bug_pos.z = (pos.z - bug_splpos.z) * 0.25 + bug_splpos.z;
  bug_pos.x = (pos.x - bug_splpos.x) * 0.25 + bug_splpos.x;
  bug_pos.y = (pos.y - bug_splpos.y) * 0.25 + bug_splpos.y;
  bug_splpos.x = bug_pos.x;
  bug_splpos.y = bug_pos.y;
  bug_splpos.z = bug_pos.z;
LAB_800097dc:
  bug_pos.x = *(float *)((int)NuTrigTable + ((buglight_ang[0] + 0x20d) * 4 & 0x3fffc)) * 0.45 +
              bug_pos.x;
  bug_pos.y = *(float *)((int)NuTrigTable + ((buglight_ang[1] + 0x127) * 4 & 0x3fffc)) * 0.45 +
              bug_pos.y;
  buglight_ang[0] = (ushort)(buglight_ang[0] + 0x20d);
  bug_pos.z = *(float *)((int)NuTrigTable + ((buglight_ang[2] + 0x366) * 4 & 0x3fffc)) * 0.45 +
              bug_pos.z;
  buglight_ang[1] = (ushort)(buglight_ang[1] + 0x127);
  buglight_ang[2] = (ushort)(buglight_ang[2] + 0x366);
  NuVecSub(&local_c8,&bug_pos,&oldpos);
  dVar6 = (double)local_c8.y;
  fadetime = NuFsqrt(local_c8.x * local_c8.x + local_c8.z * local_c8.z);
  iVar1 = NuAtan2D((float)dVar6,fadetime);
  bug_xrot = SeekRot(bug_xrot,(ushort)iVar1,3);
  iVar1 = NuAtan2D(local_c8.x,local_c8.z);
  bug_yrot = SeekRot(bug_yrot,(ushort)iVar1,3);
  if (CRemap[174] != -1) {
    BugAnim.oldaction = BugAnim.action;
    UpdateAnimPacket(CModel + CRemap[174],&BugAnim,0.5,0.0);
  }
  bug_fade = 2.0;
  buglight_fade = 2.0;
  bug_scale = 2.0;
  iVar1 = qrand();
  if (0x7fff < iVar1) {
    return;
  }
  AddGameDebris(0x9c,&bug_pos);
  return;
}

//NGC MATCH
void AddBugLight(void) {
  if (bug_fade > 0.0f) {
    if (FRAME == FRAMES - 1) {
        NuLightAddSpotXSpanFade(&bug_pos,bug_scale,bug_fade);
        buglight_enable = 1;
    }
    return;
  }
  buglight_enable = 0;
  return;
}


void DrawBug(void)

{
  ushort xrot;
  
  if ((Level == 0x1b) && (CRemap[174] != -1)) {
    Draw3DCharacter(&bug_pos,xrot,(ushort *)(uint)bug_xrot,(ushort *)(bug_yrot - 0x8000 & 0xffff),
                    1.25,(CharacterModel *)0x0,(int)(CModel + CRemap[174]),BugAnim.anim_time,
                    (int)BugAnim.action);
  }
  return;
}

//NGC MATCH
struct nuvec_s * GetBugPosition(void) {
  return &bug_pos;
}
