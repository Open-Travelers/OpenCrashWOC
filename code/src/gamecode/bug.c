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

//NGC MATCH
void UpdateBugLight(struct creature_s *c) {
  struct nuvec_s oldpos;
  struct nuvec_s v;
  struct nuvec_s v0;
  struct nuvec_s v1;
  struct nuvec_s pos;
  float f;
  float r;
  float r0;
  float r1;
  float d;
  float d0;
  float d1;
  float dr;
  u16 a;
  s32 i;
  
  bug_fade = 0.0f;
  if ((LBIT & 0x88100000) == 0) {
    return;
  }
  if (((Level == 0x14) && (GemPath != 0)) || ((InBugArea((s32)(c->obj).RPos.iRAIL,(s32)(c->obj).RPos.iALONG,(c->obj).RPos.fALONG) != -1 &&
      (((LBIT & 0x80100000) == 0 || (VEHICLECONTROL == 1)))))) {
     f = BUGFADETIME;
  } else  {
        f = 0.0f;
  }
  buglight_ang[3] += qrand() / 0x40 * 5;
  if ((LBIT & 0x80100000) != 0) {
    if (mechlight_fade > f) {
      mechlight_fade -= 0.016666668f;
        if(mechlight_fade < f) {
            mechlight_fade = f;
        }
    }
    else if (mechlight_fade < f) {
        mechlight_fade += 0.016666668f;
        if(mechlight_fade > f) {
            mechlight_fade = f;
        }
    }
    if (mechlight_fade > 0.0f) {
      bug_pos.x = (player->obj).pos.x;
      bug_pos.y = (player->obj).top * (player->obj).SCALE + (player->obj).pos.y;
      bug_pos.z = (player->obj).pos.z;
      a = NuAtan2D(bug_pos.x - GameCam[0].pos.x,bug_pos.z - GameCam[0].pos.z);
      bug_pos.x = NuTrigTable[a & 0xffff] * mechlight_distance + bug_pos.x;
      bug_pos.z = NuTrigTable[((a & 0xffff) + 0x4000) & 0x2ffff] *  mechlight_distance + bug_pos.z;
      bug_scale = 1.5f;
    }
    bug_fade = mechlight_fade;
    qrand();
    return;
  }
  if (Level != 0x1b) {
    return;
  }
  f = 2.0f;
  oldpos = bug_pos;
  if (SplTab[70].spl != NULL) {
      pos = (player->obj).pos;
      if (best_cRPos != NULL) {
        r = buglight_distance;
        if ((best_cRPos->mode & 0xc) != 0) {
          r = buglight_distance * 0.333f;
        }
        pos.x = NuTrigTable[best_cRPos->angle] * r + pos.x;
        pos.z = NuTrigTable[(best_cRPos->angle + 0x4000) & 0x2ffff] * r + pos.z;
      }
      d0 = bug_splratio;
      PointAlongSpline(SplTab[70].spl,d0,&v,NULL,NULL);
      r = NuVecDistSqr(&pos,&v,NULL);
      dr = 0.001f;
      if (c->sprint != 0) {
        dr += dr;
      }
      r0 = (bug_splratio - dr);
      if (r0 < 0.0f) {
        r0 = 0.0f;
      }
      PointAlongSpline(SplTab[70].spl,r0,&v0,NULL,NULL);
      d = NuVecDistSqr(&pos,&v0,NULL);
      r1 = (bug_splratio + dr);
      if (r1 > 1.0f) {
        r1 = 1.0f;
      }
      PointAlongSpline(SplTab[70].spl,r1,&v1,NULL,NULL);
      d1 = NuVecDistSqr(&pos,&v1,NULL);
      if ((((d < r) && (d < d1)))) {
        d0 = r0;
      } else {
        if ((d1 < r) && (d1 < d)) {
            d0 = r1;
        }
      }
      if (bug_splratio > d0) {
        bug_splratio = (bug_splratio - ((dr * 0.016666668f) * 10.0f));
        if (bug_splratio < d0) {
          bug_splratio = d0;
        }
      }
      else if (bug_splratio < d0) {
          bug_splratio = (dr * 0.016666668f) * 10.0f + bug_splratio;
              if (bug_splratio > d0) {
                  bug_splratio = d0;
              }
      }
      PointAlongSpline(SplTab[70].spl,bug_splratio,&pos,NULL,NULL);
      bug_splpos.x = (pos.x - bug_splpos.x) * 0.25f + bug_splpos.x;
      bug_splpos.y = (pos.y - bug_splpos.y) * 0.25f + bug_splpos.y;
      bug_splpos.z = (pos.z - bug_splpos.z) * 0.25f + bug_splpos.z;
      bug_pos = bug_splpos;
  } else {
    bug_pos.x = (player->obj).pos.x;
    bug_pos.y = (player->obj).top * (player->obj).SCALE + (player->obj).pos.y;
    bug_pos.z = (player->obj).pos.z;
    a = NuAtan2D(bug_pos.x - GameCam[0].pos.x,bug_pos.z - GameCam[0].pos.z);
    bug_pos.x = NuTrigTable[a & 0xffff] * buglight_distance + bug_pos.x;
    bug_pos.z = NuTrigTable[((a & 0xffff) + 0x4000) & 0x2ffff] * buglight_distance + bug_pos.z;
  }
  buglight_ang[0] = (buglight_ang[0] + 0x20d);
  buglight_ang[1] = (buglight_ang[1] + 0x127);
  buglight_ang[2] = (buglight_ang[2] + 0x366);
  bug_pos.x = NuTrigTable[buglight_ang[0] & 0x2ffff] * 0.45f + bug_pos.x;
  bug_pos.y = NuTrigTable[buglight_ang[1] & 0x2ffff] * 0.45f + bug_pos.y;
  bug_pos.z = NuTrigTable[buglight_ang[2] & 0x2ffff] * 0.45f + bug_pos.z;

  NuVecSub(&v,&bug_pos,&oldpos);
  a = NuAtan2D(v.y,NuFsqrt(v.x * v.x + v.z * v.z));
  bug_xrot = SeekRot(bug_xrot,a,3);
  a = NuAtan2D(v.x,v.z);
  bug_yrot = SeekRot(bug_yrot,a,3);
  if (CRemap[174] != -1) {
    BugAnim.oldaction = BugAnim.action;
    UpdateAnimPacket(&CModel[CRemap[174]],&BugAnim,0.5f,0.0f);
  }
  if (buglight_fade > f) {
      buglight_fade -= 0.016666668f;
      if (buglight_fade < f) {
          buglight_fade = f;
      }
  } else if (buglight_fade < f) {
      buglight_fade += 0.016666668f;
      if (buglight_fade > f) {
          buglight_fade = f;
      }
  }    
  buglight_fade = 2.0f;
  bug_scale = 2.0f;
  bug_fade = 2.0f;
  if (0x7fff < qrand()) {
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
