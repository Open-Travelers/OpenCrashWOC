//NGC MATCH
void ResetAI(void) {
  LevelAI *ai;
  s32 i;
  s32 j;
  s32 gempath_reset;
  
  gempath_reset = 0;
  if ((Rail[7].type == 3) &&
     (AheadOfCheckpoint((s32)gempath_RPos.iRAIL,(s32)gempath_RPos.iALONG,gempath_RPos.fALONG) != 0)) {
    gempath_reset = 1;
  }
  
  ai = AITab;
  for(i = 0; i < LEVELAICOUNT; i++, ai++) {
      GetALONG(ai->pos,NULL,-1,-1,1);
      ai->iRAIL = (char)temp_iRAIL;
      ai->iALONG = temp_iALONG;
      ai->fALONG = temp_fALONG;
      if ((((s8)temp_iRAIL != -1) && (Rail[(s8)temp_iRAIL].type == 3))) {
          if (gempath_reset != 0) {
              goto jmp_1;
          }
      }
      else if ((bonus_restart == 0) && (AheadOfCheckpoint((s32)ai->iRAIL,(s32)ai->iALONG,ai->fALONG) != 0)) {
jmp_1:
        ai->status = 1;
      }
      ai->time = 0.0f;
      ai->delay = AIType[ai->ai_type].delay;
      (ai->origin) = ai->pos[0];
      if (AIType[ai->ai_type].points > 1) {
        for(j = 1; j < AIType[ai->ai_type].points; j++) {
            NuVecAdd(&ai->origin,&ai->origin,&ai->pos[j]);
        }
        NuVecScale(&ai->origin,&ai->origin, (1.0f / AIType[ai->ai_type].points));
      }
  }
  for(i = 1; i < 9; i++) {
    if (Character[i].used != 0) {
      RemoveGameObject(&Character[i].obj);
      Character[i].used = 0;
      Character[i].on = 0;
    }
  }
  return;
}

//NGC MATCH
s32 FindAIType(char *name,s32 points) {
  s32 i;
  
  for (i = 0; i < 0x6b; i++) {
      if ((strcmp(name,AIType[i].name) == 0 && (points == AIType[i].points)) && (points < 9)) {
          return i;
      }
  }
  return -1;
}

//NGC MATCH
void LoadAI(void) {
  s32 fh;
  s32 i;
  s32 j;
  s32 k;
  s32 count;
  s32 points;
  s32 type;
  char name[16];
  void* fbuff;
  s32 fsize;
  
  sprintf(tbuf,"%s.ai",LevelFileName);
  if (NuFileExists(tbuf) != 0) {
    fbuff = Chase;
    fsize = NuFileLoadBuffer(tbuf,fbuff,0x7fffffff);
    fh = NuMemFileOpen(fbuff,fsize,NUFILE_READ);
    if (fh != 0) {
      points = NuFileReadInt(fh);
      for(i = 0; i < points; i++) {
          for(j = 0; j < 0x10; j++) {
            name[j] = NuFileReadChar(fh);
          }
          count = NuFileReadInt(fh);
          type = FindAIType(name,count);
          for(j = 0; j < count; j++) {
              k = 0;
              if (type != -1) {
                k = j;
              }
              AITab[LEVELAICOUNT].pos[k].x = NuFileReadFloat(fh);
              AITab[LEVELAICOUNT].pos[k].y = NuFileReadFloat(fh);
              AITab[LEVELAICOUNT].pos[k].z = NuFileReadFloat(fh);
          }
          if (type != -1) {
            AITab[LEVELAICOUNT].ai_type = (char)type;
            LEVELAICOUNT++;
          }
      }
      NuFileClose(fh);
    }
  }
  return;
}

//NGC MATCH
void InitAI(void) {
  s32 i;
  
  LEVELAITYPES = 0;
  for(i = 0; i < 0x6b; i++) {
    if (CRemap[AIType[i].character] != -1) {
      LevelAIType[LEVELAITYPES++] = i;
    }
  }
  LEVELAICOUNT = 0;
  if (Level != 0x28) {
    LoadAI();
  }
  ResetAI();
  return;
}

//NGC MATCH
void ResetDRAINDAMAGE(void) {
  DrainDamage_Intro = 1;
  drain_wawa_ok = 0;
  drain_ang[0] = qrand();
  drain_ang[1] = qrand();
  drain_ang[2] = qrand();
}

//NGC MATCH
void UpdateDRAINDAMAGE(void) {
  struct creature_s *crunch;
  u16 xrot;
  u16 yrot;
  struct nuvec_s pos;
  struct nuvec_s v;
  s32 i;
  
  FindNearestCreature(&(player->obj).pos,0x7f,NULL);
  if (temp_creature_i != -1) {
    crunch = &Character[temp_creature_i];
  }
  else {
    crunch = NULL;
  }
  drain_wawa_model = NULL;
  i = (s32)CRemap[87];
  if (i != -1) {
    drain_wawa_model = &CModel[i];
    if ((drain_wawa_model->anmdata[0xc] != NULL) && (drain_wawa_model->anmdata[0xd] != NULL)) {
      drain_ang[0] = drain_ang[0] + 0x6bd;
      drain_ang[1] = drain_ang[1] + 0x492;
      drain_ang[2] = drain_ang[2] + 0x8e8;
      if (crunch == NULL) {
        if (drain_wawa_ok == 0) {
            goto jump;
        }
      }
      else {
        pos.x = crunch->obj.pos.x - (NuTrigTable[crunch->obj.hdg] * 3.0f);
        pos.y = crunch->obj.pos.y + DRAINWAWADY;
        pos.z = crunch->obj.pos.z - (NuTrigTable[(crunch->obj.hdg + 0x4000) & 0xFFFF] * 3.0f);
      }
      NuVecSub(&v,&GameCam[0].pos,&drain_wawa_pos);
      xrot = NuAtan2D(v.y,NuFsqrt(v.x * v.x + v.z * v.z));
      yrot = NuAtan2D(v.x,v.z);
      if (drain_wawa_ok == 0) {
        drain_wawa_ok = 1;
        drain_wawa_pos = pos;
        drain_wawa_xrot = xrot;
        drain_wawa_yrot = yrot;
        ResetAnimPacket(&drain_wawa_anim,0xc);
        GameSfx(0x1e,&drain_wawa_pos);
      }
      else {
        drain_wawa_xrot = SeekRot(drain_wawa_xrot,xrot,3);
        drain_wawa_yrot = SeekRot(drain_wawa_yrot,yrot,3);
      }
      drain_wawa_anim.oldaction = drain_wawa_anim.action;
      UpdateAnimPacket(CModel + i,&drain_wawa_anim,0.5f,0.0f);
      if ((drain_wawa_anim.flags & 1) == 0) {
          return;
      }
        if (drain_wawa_anim.action == 0xc) {
          if ((crunch == NULL) || (NuVecDistSqr(&drain_wawa_pos,&crunch->obj.pos,NULL) > 25.0f)) {
              drain_wawa_anim.newaction = 0xd;
              GameSfx(0x21,&drain_wawa_pos);
          }
          return;
        }
        drain_wawa_ok = 0;
        return;
jump:
        drain_wawa_ok = 0;
        return;
    }
      drain_wawa_model = NULL;
  }
  return;
}

void DrawDRAINDAMAGE(void)		//TODO

{
  if ((drain_wawa_model != (CharacterModel *)0x0) && (drain_wawa_ok != 0)) {
	/*
nuvec_s pos;

    pos.z = NuTrigTable[*(ushort *)(mod[-0xe].fanimlist + 0x22)] * 0.1 + drain_wawa_pos.z;
    pos.x = NuTrigTable[*(ushort *)(mod[-0xe].fanimlist + 0x21)] * 0.1 + drain_wawa_pos.x;
    pos.y = NuTrigTable[*(ushort *)((int)mod[-0xe].fanimlist + 0x86)] * 0.1 + drain_wawa_pos.y;
*/
    Draw3DCharacter((nuvec_s *)&stack0xffffffe8,
                    (ushort)((uint)(NuTrigTable[drain_ang[0]] * 0.1 + drain_wawa_pos.x) >> 0x10),
                    (ushort *)(uint)drain_wawa_xrot,(ushort *)(drain_wawa_yrot - 0x8000 & 0xffff),
                    3.0,(CharacterModel *)0x0,(int)drain_wawa_model,drain_wawa_anim.anim_time,
                    (int)drain_wawa_anim.action);
  }
  return;
}

//NGC MATCH
static s32 WaterCrunchFunction_Defeated(struct creature_s *c,struct nuvec_s *pos) {
  boss_dead = (Game.hub[Hub].flags & 4) ? 2 : 1;
  return 1;
}

//NGC MATCH
void ResetCRUNCHTIME(void) {
  struct nuvec_s v;
  s32 i;
  
  jcrunch = 0;
  crunch_vulnerable = 0;
  crunchtime_attack_phase = 0;
  earth_attack_on = 0;
  earth_attack_wait = 0;
  water_attack_on = 0;
  water_attack_wait = 0;
  water_last_chute = -1;
  fire_attack_on = -1;
  fire_attack_wait = 0;
  for(i = 0; i < 9; i++) {
    weather_attack_on[i] = 0;
  }
  weather_attack_wait = 0;
  CrunchTime_Intro = 1;
  InitVehMasks();
  InitVehMask(0,3);
  v.y = 5.0f;
  v.x = 0.0f;
  v.z = 0.0f;
  SetNewMaskStuff(0,&crunchtime_arena_midpos,&v,0.0f,-540.0f,1.0f,0,0,1.0f,0.0f);
  return;
}

int UpdateCRUNCHTIME(void)		//TODO

{
  bool bVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  short sVar8;
  int i;
  uint uVar9;
  ObjTab *iVar16;
  nuvec_s *buttonpos;
  float fVar10;
  nuspecial_s *pnVar11;
  nuinstance_s *pnVar12;
  creature_s *cortex;
  nuvec_s *p0;
  nuvec_s *p1;
  creature_s *crunch;
  nuvec_s *p2;
  int iVar13;
  int iVar14;
  int iVar15;
  byte bVar16;
  double dVar17;
  double dVar18;
  double dVar19;
  float fVar20;
  nuvec_s pos [2];
  nuvec_s plrmid;
  nuangvec_s ang;
  bool phase;
  
  jcrunch = 0;
  FindNearestCreature(&(player->obj).pos,0x7f,(nuvec_s *)0x0);
  if (temp_creature_i == -1) {
    crunch = (creature_s *)0x0;
  }
  else {
    crunch = Character + temp_creature_i;
  }
  FindNearestCreature(&(player->obj).pos,2,(nuvec_s *)0x0);
  if (temp_creature_i == -1) {
    cortex = (creature_s *)0x0;
  }
  else {
    cortex = Character + temp_creature_i;
  }
  plrmid.x = (player->obj).pos.x;
  spaceuka_angle[0] = spaceuka_angle[0] + 0x492;
  bVar16 = (cortex == (creature_s *)0x0) << 1;
  plrmid.y = ((player->obj).bot + (player->obj).top) * (player->obj).SCALE * 0.5 +
             (player->obj).pos.y;
  spaceuka_angle[1] = spaceuka_angle[1] + 0x6ae;
  spaceuka_angle[2] = spaceuka_angle[2] + 0x870;
  plrmid.z = (player->obj).pos.z;
  crunchtime_attack_phase = (uint)cortex;
  if (cortex != (creature_s *)0x0) {
    crunchtime_attack_phase = 4 - (cortex->ai).hits;
  }
  bVar1 = crunch == (creature_s *)0x0;
  if (((((((bVar1) || (3 < crunchtime_attack_phase)) || (i = (int)(crunch->ai).i0, i < 1)) ||
        ((4 < i || ((crunch->ai).terflags != 0)))) ||
       (((crunch->ai).movflags != 0 ||
        ((fVar20 = NuVecDistSqr(&(crunch->obj).pos,
                                (nuvec_s *)(crunch->i_aitab * 0x80 + -0x7fdcc84c + i * 0xc),
                                (nuvec_s *)0x0), 1.0 <= fVar20 ||
         (sVar8 = (crunch->obj).anim.newaction, sVar8 == 0x73)))))) || (sVar8 == 0x49)) ||
     ((((crunch->obj).anim.action == 0x2c && ((crunch->obj).anim.anim_time < 15.0)) ||
      ((sVar8 == 0x2c && ((crunch->obj).anim.blend != '\0')))))) {
    jcrunch = 0;
  }
  else {
    jcrunch = (int)(crunch->ai).i0;
  }
  i = -1;
  if (!bVar1) {
    dVar17 = 0.5;
    iVar14 = 0;
    iVar13 = 0xc;
    do {
      fVar20 = NuVecDistSqr(&(crunch->obj).pos,
                            (nuvec_s *)(crunch->i_aitab * 0x80 + -0x7fdcc84c + iVar13),
                            (nuvec_s *)0x0);
      iVar15 = iVar14;
      if ((double)fVar20 < dVar17) {
        iVar13 = 0x3c;
        iVar15 = 4;
        i = iVar14;
      }
      iVar14 = iVar15 + 1;
      iVar13 = iVar13 + 0xc;
    } while (iVar14 < 4);
  }
  iVar14 = 4;
  iVar16 = ObjTab + 0xb5;
  buttonpos = space_buttonpos;
  iVar13 = 0;
  do {
    pnVar11 = (iVar16->obj).special;
    if (pnVar11 != (nuspecial_s *)0x0) {
      pnVar12 = pnVar11->instance;
      fVar10 = buttonpos->y;
      fVar20 = buttonpos->z;
      (pnVar12->matrix)._30 = buttonpos->x;
      (pnVar12->matrix)._32 = fVar20;
      (pnVar12->matrix)._31 = fVar10;
      if (iVar13 == i) {
        pnVar12 = ((iVar16->obj).special)->instance;
        (pnVar12->matrix)._31 = (pnVar12->matrix)._31 - 0.125;
      }
    }
    buttonpos = buttonpos + 1;
    iVar16 = iVar16 + 1;
    iVar13 = iVar13 + 1;
    iVar14 = iVar14 + -1;
  } while (iVar14 != 0);
  if ((((!bVar1) && ((crunch->ai).i0 == '\x01')) && ((crunch->obj).anim.newaction == 0x22)) &&
     (crunchtime_attack_phase < 4)) {
    earth_attack_on = 6;
  }
  if (earth_attack_on != 0) {
    earth_attack_on = earth_attack_on - 1;
  }
  if (((earth_attack_wait == 0) ||
      (earth_attack_wait = earth_attack_wait - 1, earth_attack_wait == 0)) && (earth_attack_on !=  0)
     ) {
    i = qrand();
    AddProjectile((nuvec_s *)0x0,(nuvec_s *)0x0,(nuvec_s *)0x0,0xe,(ushort)i,(obj_s *)0x0);
    earth_attack_wait = 0x1e;
  }
  if (((!bVar1) && ((crunch->ai).i0 == '\x02')) &&
     (((crunch->obj).anim.newaction == 0x22 && (crunchtime_attack_phase < 4)))) {
    water_attack_on = 6;
  }
  if (water_attack_on != 0) {
    water_attack_on = water_attack_on - 1;
  }
  if (((water_attack_wait == 0) ||
      (water_attack_wait = water_attack_wait - 1, water_attack_wait == 0)) && (water_attack_on !=  0)
     ) {
    if ((uint)water_last_chute < 2) {
      i = qrand();
      if (i < 0) {
        i = i + 0x7fff;
      }
      water_last_chute = (i >> 0xf) + 2;
    }
    else {
      i = qrand();
      if (i < 0) {
        i = i + 0x7fff;
      }
      water_last_chute = i >> 0xf;
    }
    ang.x = 0;
    buttonpos = water_attack_pos + water_last_chute;
    ang.y = NuAtan2D(crunchtime_arena_midpos.x - water_attack_pos[water_last_chute].x,
                     crunchtime_arena_midpos.z - water_attack_pos[water_last_chute].z);
    ang.z = 0;
    AddDeb3(buttonpos,0x11,5,&ang);
    GameSfx(0x48,buttonpos);
    water_attack_wait = 0x4b;
  }
  phase = false;
  if (((!bVar1) && ((crunch->ai).i0 == '\x03')) && ((crunch->obj).anim.newaction == 0x22)) {
    phase = crunchtime_attack_phase < 4;
  }
  if ((fire_attack_wait != 0) && (fire_attack_wait = fire_attack_wait - 1, fire_attack_wait == 0x3 c)
     ) {
    GameSfx(0x4d,(nuvec_s *)0x0);
  }
  if (phase) {
    if (fire_attack_wait == 0) {
      do {
        i = qrand();
        if (i < 0) {
          i = i + 0x3fff;
        }
      } while (i >> 0xe == fire_attack_on);
      fire_attack_wait = 0x78;
      fire_attack_on = i >> 0xe;
      GameSfx(0x85,(nuvec_s *)0x0);
      goto LAB_80004568;
    }
  }
  else {
LAB_80004568:
    if (fire_attack_wait == 0) goto LAB_80004810;
  }
  i = fire_attack_on;
  uVar9 = qrand();
  dVar18 = 4503601774854144.0;
  fVar6 = fire_backpos[i].x;
  fVar7 = fire_backpos[i + 1].x;
  dVar17 = 1.525902189314365e-05;
  fVar5 = (float)((double)CONCAT44(0x43300000,uVar9 ^ 0x80000000) - 4503601774854144.0) *
          1.525902e-05;
  fVar10 = fire_backpos[i + 1].y;
  fVar2 = fire_backpos[i].y;
  fVar3 = fire_backpos[i + 1].z;
  pos[0].x = (fire_frontpos[i + 1].x - fire_frontpos[i].x) * fVar5 + fire_frontpos[i].x;
  fVar4 = fire_backpos[i].z;
  pos[0].z = (fire_frontpos[i + 1].z - fire_frontpos[i].z) * fVar5 + fire_frontpos[i].z;
  pos[0].y = (fire_frontpos[i + 1].y - fire_frontpos[i].y) * fVar5 + fire_frontpos[i].y;
  uVar9 = qrand();
  fVar20 = (float)((double)(float)((double)CONCAT44(0x43300000,uVar9 ^ 0x80000000) - dVar18) *
                  dVar17);
  pos[1].z = (((fVar3 - fVar4) * fVar5 + fVar4) - pos[0].z) * fVar20 + pos[0].z;
  pos[1].x = (((fVar7 - fVar6) * fVar5 + fVar6) - pos[0].x) * fVar20 + pos[0].x;
  pos[1].y = (((fVar10 - fVar2) * fVar5 + fVar2) - pos[0].y) * fVar20 + pos[0].y;
  if (fire_attack_wait < 0x3c) {
    if (fire_attack_wait != 0) {
      iVar13 = 4;
      do {
        AddGameDebrisRot(0x76,pos + 1,1,0,0);
        iVar13 = iVar13 + -1;
      } while (iVar13 != 0);
      if ((player->obj).bot * (player->obj).SCALE + (player->obj).pos.y <
          crunchtime_arena_midpos.y + 1.0) {
        fVar20 = fire_backpos[i].z;
        fVar10 = fire_backpos[i].x;
        fVar2 = fire_backpos[i + 1].x;
        fVar3 = (player->obj).pos.z;
        fVar4 = (player->obj).pos.x;
        fVar6 = fire_backpos[i + 1].z;
        if (0.0 <= (fVar4 - fVar10) * (fVar6 - fVar20) + (fVar3 - fVar20) * (fVar10 - fVar2)) {
          fVar7 = fire_frontpos[i + 1].x;
          fVar5 = fire_frontpos[i + 1].z;
          if (0.0 <= (fVar4 - fVar2) * (fVar5 - fVar6) + (fVar3 - fVar6) * (fVar2 - fVar7)) {
            fVar2 = fire_frontpos[i].x;
            fVar6 = fire_frontpos[i].z;
            if ((0.0 <= (fVar4 - fVar7) * (fVar6 - fVar5) + (fVar3 - fVar5) * (fVar7 - fVar2)) &&
               (0.0 <= (fVar4 - fVar2) * (fVar20 - fVar6) + (fVar3 - fVar6) * (fVar2 - fVar10))) {
              KillPlayer(&player->obj,0xd);
            }
          }
        }
      }
    }
  }
  else {
    i = 2;
    do {
      AddGameDebrisRot(0x75,pos + 1,1,0,0);
      i = i + -1;
    } while (i != 0);
  }
LAB_80004810:
  if ((((!bVar1) && ((crunch->ai).i0 == '\x04')) && ((crunch->obj).anim.newaction == 0x22)) &&
     ((crunchtime_attack_phase < 4 && (weather_attack_wait == 0)))) {
    do {
      i = qrand();
      i = i / 0x1c72;
    } while (weather_attack_on[i] != 0);
    weather_attack_on[i] = 0x78;
    weather_attack_wait = 0x28;
    GameSfx(0x32,weather_dstpos + i);
  }
  dVar17 = 0.3330000042915344;
  p0 = weather_dstpos;
  dVar18 = 1.0;
  p1 = weather_srcpos;
  phase = false;
  iVar13 = 0;
  iVar14 = 0;
  i = 0;
  buttonpos = p0;
  do {
    if (*(int *)((int)weather_attack_on + i) != 0) {
      iVar15 = *(int *)((int)weather_attack_on + i) + -1;
      *(int *)((int)weather_attack_on + i) = iVar15;
      if (iVar15 == 0x3c) {
        gamesfx_effect_volume = 0xbffd;
        GameSfx(0x4b,buttonpos);
      }
      if (*(int *)((int)weather_attack_on + i) != 0) {
        AddGameDebrisRot(0x78,p1,1,0,0);
        AddGameDebrisRot(0x78,p0,1,0,0);
      }
    }
    if (*(int *)((int)weather_attack_on + i) - 1U < 0x3b) {
      fVar20 = NuVecDistSqr(&plrmid,(nuvec_s *)((int)&weather_srcpos[0].x + iVar14),(nuvec_s *)0x0 );
      dVar19 = 0.1108890026807785;
      if (fVar20 <= 0.110889) {
        p2 = (nuvec_s *)((int)&weather_dstpos[0].x + iVar14);
        fVar20 = NuVecDistSqr(&plrmid,p2,(nuvec_s *)0x0);
        if (dVar19 <= (double)fVar20) {
          uVar9 = NuAtan2D(*(float *)((int)&weather_srcpos[0].x + iVar14) -
                           *(float *)((int)&weather_dstpos[0].x + iVar14),
                           *(float *)((int)&weather_srcpos[0].z + iVar14) -
                           *(float *)((int)&weather_dstpos[0].z + iVar14));
          NuVecSub(pos,&plrmid,p2);
          NuVecRotateY(pos,pos,-(uVar9 & 0xffff));
          dVar19 = (double)pos[0].x;
          NuFabs(pos[0].x);
          if ((dVar17 <= dVar19) || (dVar18 <= (double)pos[0].y)) goto LAB_80004a20;
        }
      }
      phase = true;
    }
LAB_80004a20:
    iVar13 = iVar13 + 1;
    iVar14 = iVar14 + 0xc;
    p0 = p0 + 1;
    p1 = p1 + 1;
    buttonpos = buttonpos + 1;
    i = i + 4;
    if (8 < iVar13) {
      if (weather_attack_wait != 0) {
        weather_attack_wait = weather_attack_wait - 1;
      }
      if (phase) {
        KillPlayer(&player->obj,0xd);
      }
      if (!(bool)(bVar16 >> 1 & 1)) {
        pos[1].y = (cortex->obj).top * (cortex->obj).SCALE + (cortex->obj).pos.y + SPACEUKAHACKY;
        if ((CrunchTime_Intro != 0) ||
           (((sVar8 = (cortex->obj).anim.newaction, sVar8 != 0x1c && (sVar8 != 0x46)) &&
            (((cortex->obj).anim.action != 0x49 ||
             ((cortex->obj).anim.anim_time <= SPACEUKAFRAMEHACK)))))) {
          pos[1].y = pos[1].y + 2.0;
        }
        pos[1].x = NuTrigTable[spaceuka_angle[0]] * 0.1 +
                   ((cortex->obj).pos.x - NuTrigTable[(cortex->obj).hdg] * SPACEUKADIST);
        pos[1].y = NuTrigTable[spaceuka_angle[1]] * 0.1 + pos[1].y;
        pos[1].z = NuTrigTable[spaceuka_angle[2]] * 0.1 +
                   ((cortex->obj).pos.z -
                   *(float *)((int)NuTrigTable + (((cortex->obj).hdg + 0x4000) * 4 & 0x3fffc)) *
                   SPACEUKADIST);
        space_ukapos.z = (pos[1].z - space_ukapos.z) * 0.0333 + space_ukapos.z;
        space_ukapos.x = (pos[1].x - space_ukapos.x) * 0.0333 + space_ukapos.x;
        space_ukapos.y = (pos[1].y - space_ukapos.y) * 0.0333 + space_ukapos.y;
      }
      uVar9 = 0;
      if (!bVar1) {
        uVar9 = (uint)((crunch->obj).anim.action == 0x73);
      }
      bVar1 = uVar9 != crunch_vulnerable;
      crunch_vulnerable = uVar9;
      if (bVar1) {
        if (uVar9 == 0) {
          pos[1].y = 5.0;
          pos[1].x = 0.0;
          pos[1].z = 0.0;
          SetNewMaskStuff(0,&crunchtime_arena_midpos,pos + 1,0.0,-540.0,1.0,0,0,1.0,0.0);
        }
        else {
          pos[1].x = 0.0;
          pos[1].y = 1.0;
          pos[1].z = 0.0;
          SetNewMaskStuff(0,&(crunch->obj).pos,pos + 1,1.0,-540.0,1.0,0,0,1.0,0.0);
        }
      }
      ProcessVehMasks();
      if (jcrunch == 1) {
        i = 0xb4;
      }
      else if (jcrunch == 2) {
        i = 0xb7;
      }
      else if (jcrunch == 3) {
        i = 0xb6;
      }
      else {
        i = 0xb5;
        if (jcrunch != 4) {
          i = 0xb3;
        }
      }
      return i;
    }
  } while( true );
}


void DrawCRUNCHTIME(void)	//TODO

{
  uint ptr;
  uint *attack;
  int i;
  nuvec_s *p0;
  nuvec_s *p1;
  int col;
  int iVar1;
  ushort xrot;
  
  p0 = weather_srcpos;
  p1 = weather_dstpos;
  attack = weather_attack_on;
  i = 9;
  do {
    ptr = *attack;
    attack = attack + 1;
    if ((ptr < 0x3c) && (ptr != 0)) {
      if (Paused == 0) {
        if ((int)ptr < 0x2d) {
          iVar1 = ptr << 4;
        }
        else {
          iVar1 = (0x3c - ptr) * 0x10;
        }
        if (0x80 < iVar1) {
          iVar1 = 0x80;
        }
        NuVecAdd((nuvec_s *)&stack0xffffffc8,p0,p1);
        NuVecScale((nuvec_s *)&stack0xffffffc8,(nuvec_s *)&stack0xffffffc8,0.5);
        col = iVar1 * 0x1000000 + 0xff7f3f;
        NuLgtArcLaser(0,p0,p1,(nuvec_s *)&stack0xffffffc8,0.1,0.2,0.01,1.65,col);
        NuLgtArcLaser(0,p0,p1,(nuvec_s *)&stack0xffffffc8,0.1,0.2,0.01,1.65,col);
        NuLgtArcLaser(0,p0,p1,(nuvec_s *)&stack0xffffffc8,0.4,0.3,0.001,1.65,
                      iVar1 * 0x1000000 + 0x800040);
      }
    }
    p1 = p1 + 1;
    p0 = p0 + 1;
    i = i + -1;
  } while (i != 0);
  if (CRemap[4] != -1) {
    Draw3DCharacter(&space_ukapos,xrot,(ushort *)0x0,(ushort *)0x0,1.0,(CharacterModel *)0x0,
                    (int)(CModel + CRemap[4]),1.0,-1);
  }
  DrawVehMasks();
  return;
}

//NGC MATCH
s32 GetTotalSpaceBossObjectives() {
    return 4;
}

//NGC MATCH
s32 GetCurrentSpaceBossObjectives(void) {
    return 4 - crunchtime_attack_phase;
}

//NGC MATCH
static s32 SpaceCortexFunction_CheckPunch(struct creature_s *c,struct nuvec_s *pos) {
  return SpaceCrunch_Punch;
}

//NGC MATCH
float RatioDifferenceAlongLine(float r0,float r1,struct nuvec_s *p0,struct nuvec_s *p1) {
  float dx;
  float dz;
  float r;
  float d;
  
  r = (r1 - r0);
  dx = ((p1->x - p0->x) * r);
  dz = ((p1->z - p0->z) * r);
  d = NuFsqrt(dx * dx + dz * dz);
  if (r < 0.0f) {
    d = -d;
  }
  return d;
}

//NGC MATCH
s32 FindAILabel(struct creatcmd_s *cmd,s32 i) {
  s32 j = 0;

  while(1) { 
    if (cmd->cmd == 0x90) {
        return 0; 
    }
    if((cmd->cmd == 0x8b) && (cmd->i == i)) {
        return j;
    }
    cmd++;
    j++;
  }
}



void FindGongBongerAnim(nuvec_s *pos,nuhspecial_s *obj)		//CHECK

{
  nuinstance_s *inst;
  int i;
  int cnt;
  nuspecial_s *spec;
  nuhspecial_s gong [3];
  float dx;
  float dz;
  
  memset(gong,0,0x18);
  if (world_scene[0] != (nugscn_s *)0x0) {
    NuSpecialFind(world_scene[0],gong,"gongbong1");
    NuSpecialFind(world_scene[0],gong + 1,"gongbong2");
    NuSpecialFind(world_scene[0],gong + 2,"gongbong3");
    i = 0;
    do {
      cnt = i + 1;
      if ((gong[i].special != (nuspecial_s *)0x0) &&
         (inst = (gong[i].special)->instance, dz = (inst->matrix)._32 - pos->z,
         dx = (inst->matrix)._30 - pos->x, dx * dx + dz * dz < 25.0)) {
        spec = gong[i].special;
        obj->scene = gong[i].scene;
        obj->special = spec;
        return;
      }
      i = cnt;
    } while (cnt < 3);
  }
  return;
}

//NGC MATCH
s32 PlayerLateralInRange(float lateral,struct nuvec_s *pos,struct nuvec_s *p0,struct nuvec_s *p1,float distance) {
  if(NuFabs(RatioDifferenceAlongLine(lateral,RatioAlongLine(pos,p0,p1),p0,p1)) < distance) {
      return 1;
  }
  return 0;
}

//NGC MATCH
s32 PlayerLateralOutOfRange(float lateral,struct nuvec_s *pos,struct nuvec_s *p0,struct nuvec_s *p1,float distance) {
  if(NuFabs(RatioDifferenceAlongLine(lateral,RatioAlongLine(pos,p0,p1),p0,p1)) > distance) {
      return 1;
  }
  return 0;
}

//NGC MATCH
float FindNearestCreature(struct nuvec_s *pos,s32 character,struct nuvec_s *dst) {
  struct creature_s* c;
  float d0;
  float d;
  s32 j;
  s32 i;
  
  d0 = 1000000.0f;
  c = Character;
  j = -1;
  for(i = 0; i < 9; i++, c++) {
    if ((((c->on != 0) && (c->used != 0)) && (c->obj.character == character)) && (pos != &c->obj.pos)) {
      d = NuVecDistSqr(pos,&c->obj.pos,NULL);
      if (d < d0) {
        j = i;
        d0 = d;
        if (dst != NULL) {
          *dst = c->obj.pos;
        }
      }
    }
  }
  temp_creature_i = j;
  return d0;
}

//68% NGC
void MoveCreature(struct creature_s* c) {
    u16 uVar12;
    u16 old_hdg;
    u16 uVar28;
    s32 bVar1; // check
    s32 bVar2; // check
    // short sVar7;
    // s8 bVar14;
    short sVar11;
    struct nuvec_s local_c8;
    struct nuvec_s local_b8;
    struct nuvec_s local_a8;
    struct nuvec_s local_98;
    struct nuvec_s* pnVar15;
    struct nuvec_s* pnVar26; // tmp
    s32 uVar5;
    s32 iVar16;
    s32 iVar18;
    u32 uVar19;
    s32 iVar20;
    s32 unaff_r14;
    s32 unaff_r17;
    s32 uVar33;
    s32 uVar34;
    s32 uVar36;
    s32 iVar37;
    s32 local_88;
    s32 i;
    s32 j;
    // struct CharacterModel *pCVar21;
    // struct nuanimdata_s *pnVar22;
    // float *pfVar35;
    float fVar10;
    float dVar39;
    float dVar40;
    float dVar41;
    float dVar42;
    float fVar6;
    float fVar23;
    float fVar24;
    float fVar29;
    float fVar30;
    struct creature_s* plr;

    struct creature_s* pcVar17;
    struct nuvec_s* pos;
    struct creatcmd_s* pcVar9; // tmp
    char cVar3;
    char cVar13;
    // char cVar4;
    // short *psVar25;
    // s32 *piVar32;
    char* pcVar31;
    char* pcVar27;
    // s32 local_84;
    // double local_68;

    plr = player;
    GetTopBot(player);
    NewTopBot(&plr->obj);
    switch (c->obj.dead) {
        case 1:
            NuVecAdd(&c->obj.pos, &c->obj.pos, &c->obj.mom);
            if (HitCreatures(&c->obj, 1, 4) || HitCrates(&c->obj, 1) || HitItems(&c->obj)) {
                c->obj.die_time = c->obj.die_duration;
            } else {
                HitWumpa(&c->obj, 0);
            }
        break;
        case 0:
            c->obj.oldpos = c->obj.pos;
            local_98.x = c->obj.pos.x;
            fVar10 = (c->obj.bot + c->obj.top) * c->obj.SCALE * 0.5f;
            local_98.y = c->obj.pos.y + fVar10;
            local_98.z = c->obj.pos.z;
            pnVar15 = &c->obj.pos;
            fVar10 = c->obj.top;
            fVar24 = c->obj.bot;
            // fVar29 = c->obj.pos.z;
            // fVar23 = c->obj.pos.y;
            fVar30 = c->obj.SCALE;
            // local_98.y = c->obj.pos.y;
            c->obj.anim.oldaction = c->obj.anim.action;
            OldTopBot(&c->obj);
            unaff_r17 = (u32)(s8)AITab[c->i_aitab].ai_type;
            pos = AITab[c->i_aitab].pos;
            // sVar7 = AIType[unaff_r17].points;
            cVar3 = (u32)AIType[unaff_r17].points;
            if (c->cmdcurr != NULL)
                goto LAB_80005684;
            pcVar9 = c->cmdtable;
            do {
                c->cmdcurr = pcVar9;
            LAB_80005684:
                pcVar17 = player;
                pcVar9 = c->cmdcurr;
                bVar2 = 1;
                // iVar16 = c->cmdcurr->cmd;
                switch (c->cmdcurr->cmd) {
                    case 0x20: // > 0x23 && > 0x26 && > 0x2a
                        bVar2 = 0; // goto LAB_80005a24;
                    break;
                    case -1:
                        bVar2 = 0;
                        // goto LAB_80005a24;
                        break;
                    case 0:
                        memset(&c->ai, 0, sizeof(struct AI_s));
                        
                        c->ai.scale = 1.0f;
                        for (i = 0; i < 2; i++) { 
                            c->ai.event[i] = -1;
                        }
                        if (c->obj.character == 0x3f) {
                            FindGongBongerAnim(pos, &c->ai.obj);
                        }
                        c->ai.locator_kill = -1;
                        c->ai.kill = -1;
                        c->ai.force_action = -1;
                        c->ai.look_creature = -1;
                        break;
                    case 0x2b:
                        GetALONG(pnVar15, &c->obj.RPos, (s32)c->obj.RPos.iRAIL, (s32)c->obj.RPos.iALONG, c->cmdcurr->i);
                        break;
                    case 1:
                        cVar13 = &c->cmdcurr->i;
                    //LAB_80005d30:
                        c->ai.i0 = cVar13;
                        // fVar24 = pos[cVar13].z;
                        // fVar10 = pos[cVar13].y;
                        c->obj.pos = pos[c->ai.i0];
                        break;
                    case 2:
                        iVar16 = c->cmdcurr->i;
                        // fVar24 = pos[iVar16].z;
                        // fVar10 = pos[iVar16].y;
                        player->obj.pos = pos[iVar16];
                        player->obj.oldpos = pos[iVar16];
                        // fVar24 = v000.z;
                        // fVar10 = v000.y;
                        player->obj.mom = v000;
                        c->obj.touch = 0;
                        c->obj.contact = NULL;
                        c->obj.kill_contact = 0;
                        break;
                    
                    case 5:
                        cVar13 = c->ai.i0;
                        // cVar4 = c->ai.i1;
                        // fVar10 = c->cmdcurr->f;
                        c->ai.lateral = c->cmdcurr->f;
                        c->obj.pos.x = (pos[c->ai.i1].x - pos[c->ai.i0].x) * c->ai.lateral + pos[c->ai.i0].x;
                        c->obj.pos.y = (pos[c->ai.i1].y - pos[c->ai.i0].y) * c->ai.lateral + pos[c->ai.i0].y;
                        c->obj.pos.z = (pos[c->ai.i1].z - pos[c->ai.i0].z) * c->ai.lateral + pos[c->ai.i0].z;
                        break;
                    case 6: // < 8
                        c->ai.lateral = RatioAlongLine(&(plr->obj).pos, &pos[c->ai.i0], &pos[c->ai.i1]);
                        cVar13 = c->ai.i0;
                        // cVar4 = c->ai.i1;
                        // c->ai.lateral = fVar10;
                        c->obj.pos.x = (pos[c->ai.i1].x - pos[c->ai.i0].x) * c->ai.lateral + pos[c->ai.i0].x;
                        c->obj.pos.y = (pos[c->ai.i1].y - pos[c->ai.i0].y) * c->ai.lateral + pos[c->ai.i0].y;
                        c->obj.pos.z = (pos[c->ai.i1].z - pos[c->ai.i0].z) * c->ai.lateral + pos[c->ai.i0].z;
                        break;
                    case 3:
                        if ((c->obj.flags & 0x10000) != 0) {
                            if (c->obj.roof_y != 2000000.0f) {
                                c->obj.pos.y = c->obj.roof_y - c->obj.top * c->obj.SCALE;
                            }
                        } else {
                            if (c->obj.shadow != 2000000.0f) {
                                c->obj.pos.y = c->obj.shadow - c->obj.bot * c->obj.SCALE;
                            }
                        }
                        break;
                    case 4:
                        c->obj.pos.y = c->obj.pos.y + c->cmdcurr->f;
                        break; 
                    case 7:
                        uVar34 = c->cmdcurr->i & 0xff;
                        iVar16 = c->cmdcurr->i >> 8;
                        c->obj.pos.x =  pos[iVar16].x + (pos[uVar34].x - pos[iVar16].x) * c->cmdcurr->f;
                        c->obj.pos.y =  pos[iVar16].y + (pos[uVar34].y - pos[iVar16].y) * c->cmdcurr->f;
                        c->obj.pos.z =  pos[iVar16].z + (pos[uVar34].z - pos[iVar16].z) * c->cmdcurr->f;
                        break;
                    case 8: //< 0xc && < 9
                        uVar33 = cVar3 - 1;
                        // goto LAB_80005d30;
                        c->ai.i0 = uVar33;
                        // fVar24 = pos[cVar13].z;
                        // fVar10 = pos[cVar13].y;
                        c->obj.pos = pos[c->ai.i0];
                        break;
                    case 9:
                        // pcVar9 = c->cmdcurr;
                        c->ai.newpos.x = pos[c->cmdcurr->i].x;
                        c->ai.newpos.y = pos[c->cmdcurr->i].y;
                        c->ai.newpos.z = pos[c->cmdcurr->i].z;
                        break;
                     case 0xd:
                        // cVar13 = c->ai.i0;
                        // cVar4 = c->ai.i1;
                        //fVar10 = c->cmdcurr->f;
                        c->ai.lateral = c->cmdcurr->f;
                        c->ai.newpos.x = pos[c->ai.i0].x + (pos[c->ai.i1].x - pos[c->ai.i0].x) * c->ai.lateral;
                        c->ai.newpos.y = pos[c->ai.i0].y + (pos[c->ai.i1].y - pos[c->ai.i0].y) * c->ai.lateral;
                        c->ai.newpos.z = pos[c->ai.i0].z + (pos[c->ai.i1].z - pos[c->ai.i0].z) * c->ai.lateral;
                        break;
                    case 0xe:
                        //fVar10 = RatioAlongLine(&(plr->obj).pos, &pos[c->ai.i0], &pos[c->ai.i1]);
                        // cVar13 = c->ai.i0;
                        // cVar4 = c->ai.i1;
                        c->ai.lateral = RatioAlongLine(&(plr->obj).pos, &pos[c->ai.i0], &pos[c->ai.i1]);
                        c->ai.newpos.x = pos[c->ai.i0].x + (pos[c->ai.i1].x - pos[c->ai.i0].x) * c->ai.lateral;
                        c->ai.newpos.y = pos[c->ai.i0].y + (pos[c->ai.i1].y - pos[c->ai.i0].y) * c->ai.lateral;
                        c->ai.newpos.z = pos[c->ai.i0].z + (pos[c->ai.i1].z - pos[c->ai.i0].z) * c->ai.lateral;
                        break;
                    case 0xa: //< 0xc && > 9
                        // pnVar26 = &c->ai.newpos;
                        // fVar24 = c->obj.pos.y;
                        // fVar10 = c->obj.pos.z;
                        c->ai.newpos = c->obj.pos;
                        // goto LAB_800062d0;
                        break;
                    case 0xb:
                        // fVar10 = c->obj.shadow;
                        if (c->obj.shadow != 2000000.0f) {
                            c->ai.newpos.y = c->obj.shadow;
                        }
                        break;
                    case 0xc: // < 0xe && < 0x10
                        c->ai.newpos.y = c->ai.newpos.y + c->cmdcurr->f;
                        break;
                    case 0xf:
                        iVar16 = c->cmdcurr->i >> 8;
                        uVar34 = c->cmdcurr->i & 0xff;
                        c->ai.newpos.x = pos[iVar16].x + (pos[uVar34].x - pos[iVar16].x) * c->cmdcurr->f;
                        c->ai.newpos.y = pos[iVar16].y + (pos[uVar34].y - pos[iVar16].y) * c->cmdcurr->f;
                        c->ai.newpos.z = pos[iVar16].z + (pos[uVar34].z - pos[iVar16].z) * c->cmdcurr->f;
                        break;
                    case 0x10: //< 0x12
                        // cVar13 = c->ai.i0 + 1;
                        c->ai.i0 = c->ai.i0 + 1;
                        if ((s32)c->ai.i0 == uVar33) {
                            c->ai.i0 = 0;
                        }
                        iVar16 = (s32)c->ai.i0;
                        c->ai.newpos = pos[iVar16];
                        break;
                    case 0x11:
                        // bVar14 = c->ai.i0 - 1;
                        c->ai.i0 = c->ai.i0 - 1;
                        if ((c->ai.i0 & 0x80) != 0) {
                            c->ai.i0 = c->ai.i0 + cVar3;
                        }
                        iVar16 = (s32)c->ai.i0;
                        c->ai.newpos = pos[iVar16];
                        break;
                    case 0x12:
                        iVar16 = (s32)(char)(cVar3 + -1);
                        c->ai.i0 = cVar3 + -1;
                        c->ai.newpos = pos[iVar16]; // < 0x13
                        break;
                    case 0x13: // >= 0x13
                        local_c8.x = 0.0f;
                        local_c8.y = 0.0f;
                        local_c8.z = c->cmdcurr->f;
                        if (c->cmdcurr->cmd == 0x13) {
                            // uVar34 = qrand();
                            local_c8.z *= (float)(qrand()) * 0.000015259022f;
                        }
                        // iVar16 = qrand();
                        // pnVar26 = &c->ai.newpos;
                        NuVecRotateX(&local_c8, &local_c8, qrand());
                        // iVar16 = qrand();
                        NuVecRotateY(&c->ai.newpos, &local_c8, qrand());
                        NuVecAdd(&c->ai.newpos, &c->ai.newpos, &c->ai.wobblepos);
                        break;
                    case 0x14: // < 0x18
                        // LAB_80005a24:
                        bVar2 = 0;
                        break;
                    case 0x15:
                        iVar16 = qrand();
                        iVar16 = iVar16 - (iVar16 / (s32)uVar33) * uVar33;
                        c->ai.newpos.x = pos[iVar16].x;
                        c->ai.newpos.y = pos[iVar16].y;
                        c->ai.newpos.z = pos[iVar16].z;
                        break;
                    case 0x21:
                        c->obj.hdg = *(u16*)((s32)&c->cmdcurr->i + 2);
                        // goto LAB_80007394;
                        break;
                    case 0x22:
                        // iVar16 = c->cmdcurr->i;
                        fVar10 = pos[c->cmdcurr->i].x - c->obj.pos.x;
                        fVar24 = pos[c->cmdcurr->i].z - c->obj.pos.z;
                        // iVar16 = NuAtan2D(fVar10,fVar24);
                        c->obj.hdg = (u16)NuAtan2D(fVar10, fVar24);
                        break;
                    case 0x23: // >= 0x23
                    case 0x28: // >= 0x23
                    LAB_800060f0:
                        //iVar16 = c->cmdcurr->cmd;
                        if (c->cmdcurr->cmd == 0x28) {
                            pnVar26 = &ai_lookpos;
                        } else if (c->cmdcurr->cmd == 0x25) {
                            pnVar26 = &c->ai.newpos;
                        } else if (c->cmdcurr->cmd == 0x24) {
                            pnVar26 = &GameCam[0].pos;
                        } else {
                            pnVar26 = &(plr->obj).pos;
                        }
                        fVar10 = pnVar26->x - c->obj.pos.x;
                        fVar24 = pnVar26->z - c->obj.pos.z;
                        /*
                        c->obj.hdg = (u16)NuAtan2D(ai_lookpos.x - c->obj.pos.x, ai_lookpos.z - c->obj.pos.z);
                        c->obj.hdg = (u16)NuAtan2D(c->ai.newpos.x - c->obj.pos.x, c->ai.newpos.z - c->obj.pos.z);
                        c->obj.hdg = (u16)NuAtan2D(GameCam[0].pos.x - c->obj.pos.x, GameCam[0].pos.z - c->obj.pos.z);
                        c->obj.hdg = (u16)NuAtan2D((plr->obj).pos.x - c->obj.pos.x, (plr->obj).pos.z - c->obj.pos.z);
                        */
                        // iVar16 = NuAtan2D(fVar10,fVar24);
                        c->obj.hdg = (u16)NuAtan2D(fVar10, fVar24);
                        break;
                     case 0x27:
                        //fVar10 = FindNearestCreature(pnVar15, (s32)c->ai.look_creature, &local_c8);
                        if (!(FindNearestCreature(pnVar15, (s32)c->ai.look_creature, &local_c8) < 1000000.0f))
                            break; // goto LAB_80007394;
    
                        fVar10 = local_c8.x - c->obj.pos.x;
                        fVar24 = local_c8.z - c->obj.pos.z;
                        // iVar16 = NuAtan2D(fVar10,fVar24);
                        c->obj.hdg = (u16)NuAtan2D(fVar10, fVar24);
                        break;
                    case 0x26: // >= 0x26
                        local_c8 = v000;
                        // uVar34 = uVar33;
                        // pnVar26 = pos;
                        for (uVar34 = 0; uVar34 < uVar33; uVar34++) {
                            NuVecAdd(&local_c8, &local_c8, &pos[uVar34]);
                            // pnVar26 = pnVar26 + 1;
                        }
                        NuVecScale(&local_c8, &local_c8, 1.0f / (float)(uVar33));
                        fVar10 = local_c8.x - c->obj.pos.x;
                        fVar24 = local_c8.z - c->obj.pos.z;
                        // iVar16 = NuAtan2D(fVar10,fVar24);
                        c->obj.hdg = (u16)NuAtan2D(fVar10, fVar24);
                        break;
                    case 0x29:
                        // iVar16 = qrand();
                        c->obj.hdg = (u16)qrand();
                        break;
                    case 0x2a: // < 0x2c
                        uVar12 = c->obj.hdg;
                        c->obj.hdg = uVar12 + c->cmdtable->i;
                        break;
                    case 0x2d:
                        c->ai.rotspeed = c->cmdcurr->i;
                        break;
                    case 0x2e: //< 0x30
                        c->ai.rotspeed = c->cmdcurr->i * qrand() / 0x10000;
                        break;
                    case 0x32: // < 0x34
                        iVar16 = qrand();
                        if (iVar16 < (s32)(u32) * (u16*)((s32)&c->cmdcurr->i + 2)) {
                            c->ai.rotspeed = -c->ai.rotspeed;
                        }
                        break;
                    case 0x2c: //>= 0x2c
                        if (c->obj.RPos.iRAIL == -1)
                            break; // goto LAB_80007394;
                        // pcVar9 = c->cmdcurr;
                        //uVar12 = c->obj.RPos.angle;
                        c->obj.hdg = c->obj.RPos.angle + c->cmdcurr->i;
                        break;
                    case 0x2f:
                        // pnVar26 = &c->ai.wobblepos;
                        // fVar24 = c->obj.pos.y;
                        // fVar10 = c->obj.pos.z;
                        c->ai.wobblepos = c->obj.pos;
                    // LAB_800062d0:
                        // pnVar26->z = fVar10;
                        // pnVar26->y = fVar24;
                        break;
                    case 0x30: // >= 0x30
                        // iVar16 = qrand();
                        c->ai.wobble_ang = (u16)qrand();
                        // pcVar9 = c->cmdcurr;
                        c->ai.wobble_speed = c->cmdcurr->i;
                        c->ai.wobble = c->cmdcurr->f;
                        break; 
                    case 0x74:
                    case 0x75:
                    case 0x76:
                    case 0x77:
                        if ((c->obj.dead == 0) && ((plr->obj).dead != 0)) {
                            for (i = 0; i < 2; i++) { 
                                c->ai.event[i] = -1;
                            }
                        } else {
                            int x;
                            if (c->cmdcurr->cmd == 0x76 || c->cmdcurr->cmd == 0x77) {
                                x = c->cmdcurr->cmd - 0x76;
                                c->ai.event[x] = 0x14;
                                c->ai.event_function[x] = c->cmdcurr->i;
                                // print(&c->cmdcurr);
                            } else {
                                x = c->cmdcurr->cmd - 0x74;
                                c->ai.event[x] = c->cmdcurr->i;
                                switch (c->ai.i0) {
                                    case 0:
                                    case 1:
                                    case 2:
                                    case 3:
                                    case 4:
                                    case 5:
                                    case 6:
                                    case 7:
                                    case 0xa:
                                    case 0xb:
                                    case 0xc:
                                    case 0xd:
                                    c->ai.event_distance[x] = c->cmdcurr->f;
                                    break;
                                }
                                // if ((c->ai.i0 >= 0) && (c->ai.i0 <= 7) || (c->ai.i0 >= 0xA) && (c->ai.i0 <= 0xD)) {
                                // }
                            }
                            c->ai.event_branch[x] = -1;
                            c->ai.event_flags[x] = 0;
                            c->ai.event_occured[x] = 0;
                        }
                        break;
                    case 0x78:
                    case 0x79:
                        c->ai.event_branch[c->cmdcurr->cmd - 0x78] = FindAILabel(c->cmdtable, c->cmdcurr->i);
                        break;
                    case 0x4b:
                        // uVar12 = *(u16 *)((s32)&c->cmdcurr->i + 2);
                        c->ai.angle = c->cmdcurr->i;
                        c->ai.oldangle = c->cmdcurr->i;
                        break;
                    case 0x4c:
                        iVar16 = qrand();
                        c->ai.oldangle = (u16)iVar16;
                        c->ai.angle = (u16)iVar16;
                        break;
                    case 0x53:
                        c->ai.radius = c->cmdcurr->f;
                        break;
                    case 0x54:
                        c->ai.height = c->cmdcurr->f;
                        break;
                    case 0x4d:
                        c->ai.speed = c->cmdcurr->f;
                        break;
                    case 0x4e:
                        c->ai.speed = (float)(qrand()) * 0.000015259022f * c->cmdcurr->f;
                        break;
                    case 0x4f:
                        c->ai.speed = c->ai.speed + c->cmdcurr->f;
                        break;
                    case 0x52:
                        c->ai.distance = c->cmdcurr->f;
                        break;
                    case 0x50:
                        c->ai.direction = *(char*)((s32)&c->cmdcurr->i + 3);
                        break;
                    case 0x51:
                        if (0 < c->cmdcurr->i) {
                            c->ai.direction = (char)(qrand() / (0xffff / c->cmdcurr->i + 1));
                        } else {
                            c->ai.direction = 0;
                        }
                        break;
                    case 0x5b: // < 0x5d
                        c->obj.invisible = 1;
                        break;
                    case 0x5c:
                        c->obj.invisible = 0;
                        break;
                    case 0x5d:
                        // piVar32 = (s32 *)c->cmdcurr->i;
                        if (c->cmdcurr->i != NULL) {
                            // iVar16 = (s32)c->cmdcurr->f;
                            // local_68 = (long long)iVar16;
                            c->cmdcurr->i = c->cmdcurr->f;
                        }
                        break;
                    case 0x5e:
                        c->ai.i0 = c->cmdcurr->i;
                        break;
                    case 0x5f:
                        c->ai.i1 = c->cmdcurr->i;
                        // goto LAB_80007394;
                        break;
                    case 0x60:
                        // pcVar9 = c->cmdcurr;
                        //uVar34 = c->obj.flags | c->cmdcurr->i;
                        c->obj.flags = c->obj.flags | c->cmdcurr->i;
                        if ((c->cmdcurr->i & 0x10000U) != 0) {
                            GetTopBot(c);
                        }
                        break;
                    case 0x61:
                        // pcVar9 = c->cmdcurr;
                        //uVar34 = c->obj.flags | c->cmdcurr->i;
                        c->obj.flags = c->obj.flags | c->cmdcurr->i;
                        uVar34 = c->obj.flags ^ c->cmdcurr->i;
                        c->obj.flags = uVar34;
                        if ((c->cmdcurr->i & 0x10000U) != 0) {
                            GetTopBot(c);
                        }
                        break;
                    case 0x86:
                        local_88 = c->cmdcurr->i;
                        //(*(code *)c->cmdcurr->i)(c,pos); //FIX THIS
                        break;
                    case 0x31:
                        c->ai.look_creature = c->cmdcurr->i;
                        break;
                    case 0x16:
                        c->ai.count = c->cmdcurr->i;
                        break;
                    case 0x17:
                        //uVar34 = c->cmdcurr->i >> 0x1f;
                        iVar16 = (c->cmdcurr->i >= 0) ? c->cmdcurr->i : -c->cmdcurr->i;
                        if (iVar16 > 1) {
                            iVar18 = qrand();
                            c->ai.count = (u8)(iVar18 / (0xffff / (iVar16 + 1) + 1));
                        } else {
                            c->ai.count = 0;
                        }
                        break;
                    case 0x18: // < 0x1a
                        iVar16 = (u32)c->ai.count + c->cmdcurr->i;
                        if (iVar16 < 0) {
                            iVar16 = 0;
                        }
                        c->ai.count = (u8)iVar16;
                        break;
                    case 0x19:
                        c->ai.count = c->ai.count + 1;
                        break;
                    case 0x1a: // >= 0x1a
                        uVar5 = c->ai.count;
                        if (uVar5 != 0) {
                            c->ai.count = uVar5 + 0xff;
                        }
                        break;
                    case 0x1b:
                        c->ai.hits = c->cmdcurr->i;
                        break;
                    case 0x1c: //< 0x1e
                        uVar34 = c->cmdcurr->i;
                        uVar19 = (s32)uVar34 >> 0x1f;
                        iVar16 = (uVar19 ^ uVar34) - uVar19;
                        if (iVar16 > 1) {
                            iVar18 = qrand();
                            c->ai.hits = (u8)(iVar18 / (0xffff / (iVar16 + 1) + 1));
                        } else {
                            c->ai.hits = 0;
                        }
                        break;
                    case 0x1d:
                        iVar16 = (u32)c->ai.hits + c->cmdcurr->i;
                        if (iVar16 < 0) {
                            iVar16 = 0;
                        }
                        c->ai.hits = (u8)iVar16;
                        break;
                    case 0x1e: //>= 0x1e
                        c->ai.hits = c->ai.hits + 1;
                        break;
                    case 0x1f:
                        uVar5 = c->ai.hits;
                        if (uVar5 != 0) {
                            c->ai.hits = uVar5 + 0xff;
                        }
                        break;
                    case 0x45:
                        NewRumble(&player->rumble, c->cmdcurr->i);
                        break;
                    case 0x46:
                        NewBuzz(&player->rumble, c->cmdcurr->i);
                        break;
                    case 0x55:
                        c->ai.movflags = c->cmdcurr->i;
                        if ((c->cmdcurr->i & 0x800U) != 0) {
                            c->ai.height = c->cmdcurr->f;
                        }
                        break;
                    case 0x56:
                        // pcVar9 = c->cmdcurr;
                        // uVar12 = *(u16 *)((s32)&c->cmdcurr->i + 2);
                        c->ai.rotflags = c->cmdcurr->i;
                        if ((c->ai.rotflags & 1) != 0) {
                            // dVar40 = NuFabs(c->cmdcurr->f);
                            c->ai.spins = NuFabs(c->cmdcurr->f);
                        }
                        break;
                    case 0x33:
                        c->ai.rotadjust = c->cmdcurr->i;
                        break;
                    case 0x57:
                        c->ai.terflags = c->cmdcurr->i;
                        break;
                    case 0x34: // >= 0x34
                        c->obj.shadow = 2000000.0f;
                        break;
                    case 0x35:
                        if ((c->obj.flags & 0x10000) != 0) {
                            local_c8.x = c->obj.pos.x;
                            local_c8.y = c->obj.pos.y - 1.0f;
                            local_c8.z = c->obj.pos.z;
                            //pnVar26 = &local_c8;
                            c->obj.shadow = NewShadowMaskPlat(&local_c8, 0.0f, -1);
                        } else {
                            //pnVar26 = pnVar15;
                            c->obj.shadow = NewShadowMaskPlat(pnVar15, 0.0f, -1);
                        }
                        // fVar10 = NewShadowMaskPlat(pnVar26,0.0f,-1);
                        //c->obj.shadow = NewShadowMaskPlat(pnVar26, 0.0f, -1);
                        GetSurfaceInfo(&c->obj, 1, c->obj.shadow);
                        ObjectRotation(&c->obj, 0, 0);
                        // uVar12 = temp_zrot;
                        c->obj.surface_xrot = temp_xrot;
                        c->obj.surface_zrot = temp_zrot;
                        break;
                    case 0x36: // < 0x38
                        if ((c->obj.flags & 0x10000) != 0) {
                            c->obj.xrot = c->obj.roof_xrot;
                            c->obj.zrot = c->obj.roof_zrot;
                        } else {
                            c->obj.xrot = c->obj.surface_xrot;
                            c->obj.zrot = c->obj.surface_zrot;
                        }
                        // c->obj.xrot = uVar12;
                        // c->obj.zrot = uVar28;
                        break;
                    case 0x89:
                        sVar11 = c->obj.character;
                        ChangeCharacter(c, (s32)(player->obj).character);
                        ChangeCharacter(player, sVar11);
                        bVar2 = 0; // goto LAB_80005a24;
                        break;
                    case 0x87:
                        ChangeCharacter(c, c->cmdcurr->i);
                        break;
                    case 0x88:
                        ChangeCharacter(player, c->cmdcurr->i);
                        break;
                    case 0x47:
                        ResetAnimPacket(&c->obj.anim, c->cmdcurr->i);
                        break;
                    case 0x48:
                        c->obj.anim.newaction = c->cmdcurr->i;
                        break;
                    case 0x59:
                        fVar10 = c->cmdcurr->f;
                        c->obj.anim.anim_time = fVar10;
                        if (fVar10 < 1.0f) {
                            c->obj.anim.anim_time = 1.0f;
                        }
                        break;
                    case 0x5a:
                        if ((c->obj.model)->anmdata[c->obj.anim.newaction] != NULL) {
                            // uVar34 = qrand();
                            // local_68 = qrand();
                            c->obj.anim.anim_time = (float)(qrand()) * 0.000015259022f
                                    * ((c->obj.model)->anmdata[c->obj.anim.newaction]->time - 1.0f) + 1.0f;
                        }
                        break;
                    case 0x49:
                        c->ai.force_action = c->cmdcurr->i;
                        break;
                    case 0x4a:
                        c->ai.idle_update = c->cmdcurr->i;
                        break;
                    case 0x3c: ////>= 0x3c
                        c->ai.duration = c->cmdcurr->f;
                        break;
                    case 0x3d:
                        NuVecSub(&local_c8, &c->ai.newpos, &c->ai.oldpos);
                        fVar10 = NuVecMag(&local_c8);
                        c->ai.duration = fVar10 / c->cmdcurr->f;
                        // c->ai.duration = fVar10;
                        if (fVar10 < 0.01666667f) {
                            c->ai.duration = 0.01666667f;
                        }
                        break;
                    case 0x3e: // < 0x40
                        // pCVar21 = c->obj.model;
                        //sVar11 = c->obj.anim.newaction;
                        // pnVar22 = c->obj.model->anmdata[sVar11];
                        if (c->obj.model->anmdata[c->obj.anim.newaction] != NULL) {
                            c->ai.duration = (c->obj.model->anmdata[c->obj.anim.newaction]->time - 1.0f)
                                / (c->obj.model->animlist[c->obj.anim.newaction]->speed * 30.0f);
                        } else {
                            c->ai.duration = 1.0f;
                        }
                        //c->ai.duration = fVar10;
                        //fVar10 = c->cmdcurr->f;
                        if (1.0f < c->cmdcurr->f) {
                            //fVar24 = c->ai.duration;
                            c->ai.duration = c->ai.duration * c->cmdcurr->f;
                            //goto LAB_80006b98;
                        }
                        break;
                    case 0x40: // >= 0x40
                        //sVar11 = c->obj.anim.newaction;
                        // pCVar21 = c->obj.model;
                        // pnVar22 = c->obj.model->anmdata[sVar11];
                        if (c->obj.model->anmdata[c->obj.anim.newaction] == NULL) { //goto LAB_80006c8c;
                            c->ai.duration = 1.0f;
                            break;
                        }
                        fVar24 = c->cmdcurr->f;
                        fVar10 = c->obj.model->anmdata[c->obj.anim.newaction]->time;
                        fVar30 = c->obj.anim.anim_time;
                        if (((1.0f < fVar24) && (fVar24 < fVar10)) && (fVar30 > fVar24)) {
                            fVar10 = fVar24;
                        }
                        fVar24 = c->obj.model->animlist[c->obj.anim.newaction]->speed * 30.0f;
                        fVar10 = fVar10 - fVar30;
                    //LAB_80006c80:
                        c->ai.duration = fVar10 / fVar24;
                        break;
                    case 0x3f:
                        // pCVar21 = c->obj.model;
                        //sVar11 = c->obj.anim.newaction;
                        if (c->obj.model->anmdata[c->obj.anim.newaction] == NULL) {
            //LAB_80006c8c:
                            c->ai.duration = 1.0f;
                        } else {
                            c->ai.duration = (c->cmdcurr->f - 1.0f) / (c->obj.model->animlist[c->obj.anim.newaction]->speed * 30.0f);
                        }
                        break;
                    case 0x41:
                        c->ai.duration = 0.0f;
                        break;
                    case 0x42:
                        //uVar34 = qrand();
                        //fVar10 = c->cmdcurr->f;
                        //fVar24 = (float)(uVar34)*0.000015259022f;
                    //LAB_80006b98:
                        c->ai.duration = ((float)(qrand()) * 0.000015259022f) * c->cmdcurr->f;
                        break;
                    case 0x43:
                        c->ai.duration = c->ai.duration + c->cmdcurr->f;
                        break;
                    case 0x44:
                        if (c->ai.rotspeed != 0) {
                            iVar16 = NuAtan2D(c->ai.newpos.x - c->obj.pos.x, c->ai.newpos.z - c->obj.pos.z);
                            uVar34 = RotDiff(c->ai.old_yrot, (u16)iVar16);
                            if (((c->ai.rotflags & 0x200) != 0) && ((s32)uVar34 < 0)) {
                                uVar34 = uVar34 + 0x10000;
                            } else {
                                if (((c->ai.rotflags & 0x400) != 0) && (0 < (s32)uVar34)) {
                                    uVar34 = uVar34 - 0x10000;
                                }
                            }
                            // local_68 = CONCAT44(0x43300000,((s32)uVar34 >> 0x1f ^ uVar34) -
                            //                             ((s32)uVar34 >> 0x1f) ^ 0x80000000);
                            //fVar10 = (float)((s32)uVar34 >> 0x1f ^ uVar34) - ((s32)uVar34 >> 0x1f);
                            // local_68 = CONCAT44(0x43300000,
                            //          (u32)c->ai.rotspeed * 0x3c ^ 0x80000000);
                            //fVar24 = (float)((u32)c->ai.rotspeed * 0x3c);
                            //goto LAB_80006c80;
                            c->ai.duration = ((float)((uVar34 >= 0) ? uVar34 : -uVar34) / (c->ai.rotspeed * 0x3c));
                            break;
                        }
                        //goto LAB_80006c8c;
                        c->ai.duration = 1.0f;
                        break;
                    case 0x37:
                        c->obj.attack = c->cmdcurr->i;
                        break;
                    case 0x38: // >= 0x38
                        //uVar12 = c->cmdcurr->i;
                        c->obj.vulnerable = c->cmdcurr->i;
                        if ((((c->obj.flags & 4) != 0) && (unaff_r17 >= 0)) && ((unaff_r17 < 0x3e && (c->obj.character != 0x16)))) {
                            c->obj.vulnerable = c->obj.vulnerable | 0x180;
                        }
                        break;
                    case 0x39:
                        c->ai.kill = c->cmdcurr->i;
                        break;
                    case 0x3a: //< 0x3c
                        c->ai.locator_kill = c->cmdcurr->i;
                        break;
                    case 0x3b:
                        // pcVar9 = c->cmdcurr;
                        c->ai.attack_locator_bits = c->cmdcurr->i;
                        c->ai.attack_locator_radius = c->cmdcurr->f;
                        break;
                    case 0x6a:
                        // iVar16 = c->cmdcurr->i;
                        if (c->cmdcurr->i != -1) {
                            local_c8 = pos[c->cmdcurr->i];
                        } else {
                            local_c8 = c->obj.pos;
                        }
                        AddKaboom(0x10, &local_c8, c->cmdcurr->f);
                        if (c->obj.character == 0x80) {
                            AddDeb3(&local_c8, 9, 1, NULL);
                            AddDeb3(&local_c8, 9, 1, NULL);
                            AddDeb3(&local_c8, 9, 1, NULL);
                            AddGameDebris(0xd, &local_c8);
                            AddGameDebris(0xe, &local_c8);
                        }
                        break;
                    case 0x6c:
                        JudderGameCamera(GameCam, 0.3f, NULL);
                         //goto LAB_80007394;
                        break;
                    case 0x71:
                        // iVar16 = KillPlayer(&plr->obj,GetDieAnim(&plr->obj,c->cmdcurr->i));
                        if (KillPlayer(&plr->obj, GetDieAnim(&plr->obj, c->cmdcurr->i)) == 0) {
                            KillGameObject(&c->obj, 4);
                        }
                        break;
                    case 0x72:
                        (plr->obj).mom.x = NuTrigTable[c->obj.hdg] * c->cmdcurr->f;
                        (plr->obj).mom.z = NuTrigTable[(c->obj.hdg + 0x4000) & 0x2ffff] * c->cmdcurr->f;
                        break;
                    case 0x6d:
                        AddGameDebris(c->cmdcurr->i, &c->obj.pos);
                        break;
                    case 0x6e:
                        AddGameDebris(c->cmdcurr->i, &local_98);
                        break;
                    case 0x83:
                        // iVar16 = c->cmdcurr->cmd;
                        // iVar18 = 0;
                        // if ((s32*)c->cmdcurr->i != NULL) {
                        //     // local_68 = (long long)(s32)pcVar9->f;
                        //     bVar1 = *(s32*)c->cmdcurr->i == (s32)c->cmdcurr->f;
                        //     if (bVar1) {
                        //         iVar18 = 1;
                        //     }
                        // }
                        // // pcVar9 = c->cmdcurr;
                        // c->cmdcurr = c->cmdcurr + 1;
                        // if (c->cmdcurr[1].cmd == 0x8d) {
                        //     if (iVar18 != 0)
                        //         goto LAB_800071dc;
                        // } else if ((c->cmdcurr[1].cmd == 0x8e) && (iVar18 == 0)) {
                        //     // iVar16 = FindAILabel(c->cmdtable,c->cmdcurr[1].i);
                        //     c->cmdcurr = c->cmdtable + FindAILabel(c->cmdtable, c->cmdcurr[1].i);
                        // }
                        // break;
                    case 0x84:
                        iVar16 = c->cmdcurr->cmd;
                        iVar18 = 0;
                        // iVar16 = c->cmdcurr->i;
                        switch(c->cmdcurr->i) {
                             case 0:
                             case -1:
                             if ((c->cmdcurr->i == 0) && (fVar10 = NuVecDist(&(plr->obj).pos,pnVar15,NULL),
                                fVar10 < c->cmdcurr->f)) iVar18 = 1; //goto LAB_800071a0;
                             break;
                             case 1:
                             fVar10 = NuVecDist(&(plr->obj).pos,pnVar15,NULL);
                             if (c->cmdcurr->f < fVar10) iVar18 = 1; //goto LAB_800071a0;
                             break;
                             case 2:
                                 fVar10 = NuVecXZDist(&(plr->obj).pos,pnVar15,NULL);
                                 if (fVar10 < c->cmdcurr->f) iVar18 = 1; //goto LAB_800071a0;
                             break;
                             case 3:
                               fVar10 = NuVecXZDist(&(plr->obj).pos,pnVar15,NULL);
                               if (fVar10 > c->cmdcurr->f) iVar18 = 1; //goto LAB_800071a0;
                             break;
                             case 4:
                                 iVar18 =
                         PlayerLateralInRange(c->ai.lateral,&(plr->obj).pos,&pos[c->ai.i0],&pos[c->ai.i1],c->cmdcurr->f);
                             break;
                             case 5:
                                 iVar18 =
                         PlayerLateralOutOfRange(c->ai.lateral,&(plr->obj).pos,&pos[c->ai.i0],&pos[c->ai.i1],c->cmdcurr->f);
                             break;
                             case 6:
                                 if ((plr->obj).dead != 0) iVar18 = 1; //goto LAB_800071a0;
                             break;
                            case 7:
                                 if (plr_crates.count >= DESTRUCTIBLECRATECOUNT) iVar18 = 1; //goto LAB_800071a0;
                             break;
                             case 8:
                               iVar16 = (s32)c->cmdcurr->i;
                               //local_68 = (long long)iVar16;
                               //bVar1 = c->ai.i0 == c->cmdcurr->i;
                                 //goto LAB_8000719c;
                    //LAB_8000719c:
                                if (c->ai.i0 == c->cmdcurr->i) {
                    //LAB_800071a0:
                                  iVar18 = 1;
                                }
                             break;
                             case 9:
                               //local_68 = (long long)(s32)pcVar9->f;
                               //bVar1 = (s32)c->ai.i1 == (s32)c->cmdcurr->f;
                                 //goto LAB_8000719c;
                                if (c->ai.i1 == (s32)c->cmdcurr->f) {
                                  iVar18 = 1;
                                }
                             break;
                             case 10:
                                 //bVar1 = (s32)c->ai.i0 == uVar33 - 1;
                                 //goto LAB_8000719c;
                                if (c->ai.i0 == uVar33 - 1) {
                                  iVar18 = 1;
                                }
                             break;
                             case 0xb:
                                 //bVar1 = (s32)c->ai.i1 == uVar33 - 1;
                                 //goto LAB_8000719c;
                                if (c->ai.i1 == uVar33 - 1) {
                                  iVar18 = 1;
                                }
                             break;
                             case 0xd:
                                 //bVar1 = c->ai.i0 == 0;
                                 //goto LAB_8000719c;
                                if (c->ai.i0 == 0) {
                                  iVar18 = 1;
                                }
                             break;
                             case 0xe:
                                if ((c->cmdcurr->i == 0xe) && ((c->obj.touch & local_88) != 0)) {
                                    iVar18 = 1; 
                                    break; //goto LAB_800071a0;
                                } 
                                goto LAB_800071a4; 
                             break;
                             case 0xc:
                                 //bVar1 = c->ai.i0 == 0;
                                 //goto LAB_8000719c;
                                if (c->ai.i0 == 0) {
                                  iVar18 = 1;
                                }
                             break;
                         }
                        // if (c->cmdcurr->i == 7) {
                        //     if (DESTRUCTIBLECRATECOUNT <= plr_crates.count)
                        //         goto LAB_800071a0;
                        // } else {
                        //     if (7 < c->cmdcurr->i) {
                        //         if (c->cmdcurr->i == 0xb) {
                        //             bVar1 = (s32)c->ai.i1 == uVar33 - 1;
                        //         } else if (c->cmdcurr->i < 0xc) {
                        //             if (c->cmdcurr->i == 9) {
                        //                 // local_68 = (long long)(s32)pcVar9->f;
                        //                 bVar1 = (s32)c->ai.i1 == (s32)c->cmdcurr->f;
                        //             } else if (c->cmdcurr->i < 10) {
                        //                 iVar16 = (s32)c->cmdcurr->i;
                        //                 // local_68 = (long long)iVar16;
                        //                 bVar1 = c->ai.i0 == c->cmdcurr->i;
                        //             } else {
                        //                 bVar1 = (s32)c->ai.i0 == uVar33 - 1;
                        //             }
                        //         } else if (c->cmdcurr->i == 0xd) {
                        //             bVar1 = c->ai.i0 == 0;
                        //         } else {
                        //             if (0xc < c->cmdcurr->i) {
                        //                 if ((c->cmdcurr->i == 0xe) && ((c->obj.touch & local_88) != 0))
                        //                     goto LAB_800071a0;
                        //                 goto LAB_800071a4;
                        //             }
                        //             bVar1 = c->ai.i0 == 0;
                        //         }
                        //         goto LAB_8000719c;
                        //     }
                        //     if (c->cmdcurr->i == 3) {
                        //         fVar10 = NuVecXZDist(&(plr->obj).pos, pnVar15, NULL);
                        //         if (c->cmdcurr->f < fVar10)
                        //             goto LAB_800071a0;
                        //     } else if (c->cmdcurr->i < 4) {
                        //         if (c->cmdcurr->i == 1) {
                        //             fVar10 = NuVecDist(&(plr->obj).pos, pnVar15, NULL);
                        //             if (c->cmdcurr->f < fVar10)
                        //                 goto LAB_800071a0;
                        //         } else if (c->cmdcurr->i < 2) {
                        //             if ((c->cmdcurr->i == 0)
                        //                 && (fVar10 = NuVecDist(&(plr->obj).pos, pnVar15, NULL), fVar10 < c->cmdcurr->f))
                        //                 goto LAB_800071a0;
                        //         } else {
                        //             fVar10 = NuVecXZDist(&(plr->obj).pos, pnVar15, NULL);
                        //             if (fVar10 < c->cmdcurr->f)
                        //                 goto LAB_800071a0;
                        //         }
                        //     } else if (c->cmdcurr->i == 5) {
                        //         iVar18 = PlayerLateralOutOfRange(
                        //             c->ai.lateral, &(plr->obj).pos, &pos[c->ai.i0], &pos[c->ai.i1], c->cmdcurr->f
                        //         );
                        //     } else if (c->cmdcurr->i < 6) {
                        //         iVar18 = PlayerLateralInRange(
                        //             c->ai.lateral, &(plr->obj).pos, &pos[c->ai.i0], &pos[c->ai.i1], c->cmdcurr->f
                        //         );
                        //     } else if ((plr->obj).dead != 0)
                        //         goto LAB_800071a0;
                        // }
                    LAB_800071a4:
                        // pcVar9 = c->cmdcurr;
                        c->cmdcurr = c->cmdcurr + 1;
                        if (c->cmdcurr[1].cmd == 0x8d) {
                            if (iVar18 != 0)
                                goto LAB_800071dc;
                        } else if ((c->cmdcurr[1].cmd == 0x8e) && (iVar18 == 0)) {
                        LAB_800071dc:
                            // iVar16 = FindAILabel(c->cmdtable,c->cmdcurr[1].i);
                            c->cmdcurr = c->cmdtable + FindAILabel(c->cmdtable, c->cmdcurr[1].i);
                        }
                        break;
                    case 0x62:
                        // local_84 = c->cmdcurr->i;
                        unaff_r14 = -1;
                        break;
                    case 0x63:
                        unaff_r14 = c->cmdcurr->i;
                        break;
                    case 0x6b:
                        uVar19 = 0;
                        pnVar26 = (struct nuvec_s*)&c->mtxLOCATOR[0][0]._30;
                        local_c8 = v000;
                        for(uVar34 = 0; (uVar34 + 1) < 0x10; uVar34++) {
                            if ((c->cmdcurr->i >> uVar34 & 1U) != 0) {
                                NuVecAdd(&local_c8, &local_c8, pnVar26);
                                uVar19++;
                            }
                            pnVar26++;
                        }
                        if (uVar19 != 0) {
                            if (1 < (s32)uVar19) {
                                // local_68 = CONCAT44(0x43300000,uVar19 ^ 0x80000000);
                                NuVecScale(&local_c8, &local_c8, 1.0f / (float)(uVar19));
                            }
                            if (unaff_r14 != -1) {
                                pnVar26 = pos + unaff_r14;
                            } else {
                                pnVar26 = NULL;
                            }
                            AddProjectile(
                                &local_c8, pnVar26, &c->momLOCATOR[0][uVar36], c->cmdcurr->i, c->obj.hdg, &c->obj
                            );
                        }
                        break;
                    case 0x65:
                        gamesfx_effect_volume = c->cmdcurr->i;
                        break;
                    case 0x66:
                        gamesfx_pitch = c->cmdcurr->i;
                        break;
                    case 0x67:
                        GameSfx(c->cmdcurr->i, NULL);
                        break;
                    case 0x68:
                        GameSfx(c->cmdcurr->i, pnVar15);
                        break;
                    case 0x69:
                        gamesfx_effect_volume = (s32)c->cmdcurr->f;
                        // local_68 = (long long)gamesfx_effect_volume;
                        GameSfx(c->cmdcurr->i, pnVar15);
                        break;
                    case 0x6f:
                        ResetHGobjAnim(&c->ai.obj);
                        break;
                    case 0x70:
                        StartHGobjAnim(&c->ai.obj);
                        break;
                    case 0x73:
                        c->ai.scale = c->cmdcurr->f;
                        break;
    
                    
                    // case 0x58: //> 0x57
                    //     bVar2 = 0; // goto LAB_80005a24;
                    //     break;
                    // case 0x64:
                    //     bVar2 = 0; // goto LAB_80005a24;
                    //     break;                
                    // case 0x7b:
                    //     *(s32*)((s32)c->ai.event_function + iVar16 + -100) = *(s32*)((s32)&c->cmdcurr->i + 3);
                    //     break;
                    // case 0x7e:
                    //     iVar16 = c->cmdcurr->cmd;
                    //     iVar18 = 0;
                    //     bVar1 = (s32)(player->obj).character == c->cmdcurr->i;
                    //     if (bVar1) {
                    //         iVar18 = 1;
                    //     }
                    //     // pcVar9 = c->cmdcurr;
                    //     c->cmdcurr = c->cmdcurr + 1;
                    //     if (c->cmdcurr[1].cmd == 0x8d) {
                    //         if (iVar18 != 0)
                    //             goto LAB_800071dc;
                    //     } else if ((c->cmdcurr[1].cmd == 0x8e) && (iVar18 == 0)) {
                    //         // iVar16 = FindAILabel(c->cmdtable,c->cmdcurr[1].i);
                    //         c->cmdcurr = c->cmdtable + FindAILabel(c->cmdtable, c->cmdcurr[1].i);
                    //     }
                    //     break;
                    // case 0x7f:
                    //     iVar16 = c->cmdcurr->cmd;
                    //     iVar18 = 0;
                    //     bVar1 = (s32)c->obj.anim.newaction == c->cmdcurr->i;
                    //     if (bVar1) {
                    //         iVar18 = 1;
                    //     }
                    //     // pcVar9 = c->cmdcurr;
                    //     c->cmdcurr = c->cmdcurr + 1;
                    //     if (c->cmdcurr[1].cmd == 0x8d) {
                    //         if (iVar18 != 0)
                    //             goto LAB_800071dc;
                    //     } else if ((c->cmdcurr[1].cmd == 0x8e) && (iVar18 == 0)) {
                    //         // iVar16 = FindAILabel(c->cmdtable,c->cmdcurr[1].i);
                    //         c->cmdcurr = c->cmdtable + FindAILabel(c->cmdtable, c->cmdcurr[1].i);
                    //     }
                    //     break;
                    // case 0x80:
                    //     iVar16 = c->cmdcurr->cmd;
                    //     iVar18 = 0;
                    //     bVar1 = (s32)c->ai.i0 == c->cmdcurr->i;
                    //     if (bVar1) {
                    //         iVar18 = 1;
                    //     }
                    //     // pcVar9 = c->cmdcurr;
                    //     c->cmdcurr = c->cmdcurr + 1;
                    //     if (c->cmdcurr[1].cmd == 0x8d) {
                    //         if (iVar18 != 0)
                    //             goto LAB_800071dc;
                    //     } else if ((c->cmdcurr[1].cmd == 0x8e) && (iVar18 == 0)) {
                    //         // iVar16 = FindAILabel(c->cmdtable,c->cmdcurr[1].i);
                    //         c->cmdcurr = c->cmdtable + FindAILabel(c->cmdtable, c->cmdcurr[1].i);
                    //     }
                    //     break;
                    // case 0x81:
                    //     iVar16 = c->cmdcurr->cmd;
                    //     iVar18 = 0;
                    //     bVar1 = (u32)c->ai.count == c->cmdcurr->i;
                    //     if (bVar1) {
                    //         iVar18 = 1;
                    //     }
                    //     // pcVar9 = c->cmdcurr;
                    //     c->cmdcurr = c->cmdcurr + 1;
                    //     if (c->cmdcurr[1].cmd == 0x8d) {
                    //         if (iVar18 != 0)
                    //             goto LAB_800071dc;
                    //     } else if ((c->cmdcurr[1].cmd == 0x8e) && (iVar18 == 0)) {
                    //         // iVar16 = FindAILabel(c->cmdtable,c->cmdcurr[1].i);
                    //         c->cmdcurr = c->cmdtable + FindAILabel(c->cmdtable, c->cmdcurr[1].i);
                    //     }
                    //     break;
                    // case 0x82:
                    //     iVar16 = c->cmdcurr->cmd;
                    //     iVar18 = 0;
                    //     bVar1 = (u32)c->ai.hits == c->cmdcurr->i;
                    // LAB_8000719c:
                    //     if (bVar1) {
                    //     LAB_800071a0:
                    //         iVar18 = 1;
                    //     }
                    //     // pcVar9 = c->cmdcurr;
                    //     c->cmdcurr = c->cmdcurr + 1;
                    //     if (c->cmdcurr[1].cmd == 0x8d) {
                    //         if (iVar18 != 0)
                    //             goto LAB_800071dc;
                    //     } else if ((c->cmdcurr[1].cmd == 0x8e) && (iVar18 == 0)) {
                    //         // iVar16 = FindAILabel(c->cmdtable,c->cmdcurr[1].i);
                    //         c->cmdcurr = c->cmdtable + FindAILabel(c->cmdtable, c->cmdcurr[1].i);
                    //     }
                    //     break;
                    
                    // case 0x85:
                    //     iVar16 = c->cmdcurr->cmd;
                    //     iVar18 = 0;
                    //     // goto LAB_800071a4;
                    //     // pcVar9 = c->cmdcurr;
                    //     c->cmdcurr = c->cmdcurr + 1;
                    //     if (c->cmdcurr[1].cmd == 0x8d) {
                    //         if (iVar18 != 0)
                    //             goto LAB_800071dc;
                    //     } else if ((c->cmdcurr[1].cmd == 0x8e) && (iVar18 == 0)) {
                    //         // iVar16 = FindAILabel(c->cmdtable,c->cmdcurr[1].i);
                    //         c->cmdcurr = c->cmdtable + FindAILabel(c->cmdtable, c->cmdcurr[1].i);
                    //     }
                    //     //(*(code *)c->cmdcurr->i)(c,pos); //FIX THIS
                    //     break;
                    
                    
                    
                    
                    // case 0x8a:
                    //     bVar2 = 0; // goto LAB_80005a24;
                    // case 0x8b:
                    //     // goto LAB_80007394;
                    //     break;
                }
                /*if (iVar16 == 0x41) {
                  c->ai.duration = 0.0f;
                }*/
                LAB_80007394:
                if (bVar2) {
                    c->ai.oldpos = c->obj.pos;
                    c->ai.time = c->obj.mom.x = c->obj.mom.y = c->obj.mom.z = 0.0f;
                    c->cmdcurr++;
                    
                    c->ai.oldpos = c->obj.pos;
                    c->ai.old_yrot = c->obj.hdg;
                    c->ai.oldangle = c->ai.angle;
                    continue; //goto LAB_80005684;
                }
                
                for (i = 0; c->cmdcurr->cmd != 0x8c; i++) {
                    c->cmdcurr = &c->cmdtable[FindAILabel(c->cmdtable, c->cmdcurr->i)];
                }
                if (i != 0) continue;
                //     goto LAB_80005684;
                bVar2 = 0;
                // fVar24 = c->ai.time + 0.01666667f;
                iVar16 = -2;
                // fVar10 = c->ai.duration;
                c->ai.time = c->ai.time + 0.01666667f;
                if ((0.0f < c->ai.duration) && (c->ai.time >= c->ai.duration)) {
                    c->ai.time = c->ai.duration;
                    iVar16 = -1;
                    // psVar25 = c->ai.event_branch;
                    for (j = 0; j < 2; j++) {
                        if ((c->ai.event[j] != -1) && (c->ai.event_occured[j] != 0)) {
                            iVar16 = c->ai.event_branch[j];
                        }
                        c->ai.event[j] = -1;
                    }
                    bVar2 = 1;
                }
                
                fVar10 = c->ai.duration;
                dVar40 = 0.0f;
                if (0.0f < fVar10) {
                    dVar40 = (c->ai.time / fVar10);
                }
                uVar12 = c->ai.movflags;
                c->ai.wobble_ang = c->ai.wobble_ang + c->ai.wobble_speed;
                if ((c->ai.movflags & 0x40) != 0) {
                    iVar18 = (s32)c->ai.i0;
                    uVar34 = iVar18 + 1;
                    if (uVar34 == uVar33) {
                        uVar34 = 0;
                    }
                    NuVecSub(&local_c8, pos + uVar34, pos + iVar18);
                    // fVar10 = c->obj.pos.x;
                    // fVar24 = c->obj.pos.z;
                    fVar30 = -local_c8.x;
                    local_b8.x = local_c8.x;
                    local_c8.x = local_c8.z + c->obj.pos.x;
                    if (((player->obj).pos.x - c->obj.pos.x) * ((fVar30 + c->obj.pos.z) - c->obj.pos.z)
                            + ((player->obj).pos.z - c->obj.pos.z) * (c->obj.pos.x - local_c8.x) >= 0.0f)
                    {
                        if (uVar34 == uVar33) {
                            uVar34 = 0;
                        }
                        bVar1 = 0;
                    } else {
                        bVar1 = 1;
                        uVar34 = (u32)c->ai.i0;
                        uVar34 = (s32)c->ai.i0 + 1;
                    }
                    fVar23 = pos[uVar34].x - c->obj.pos.x;
                    fVar10 = pos[uVar34].z - c->obj.pos.z;
                    dVar41 = (fVar23 * fVar23 + fVar10 * fVar10);
                    local_c8.z = fVar30 + c->obj.pos.z;
                    uVar19 = NuAtan2D(fVar23, fVar10);
                    fVar10 = c->ai.speed * 0.01666667f;
                    if (dVar41 <= (fVar10 * fVar10)) {
                        // fVar24 = pos[uVar34].z;
                        // fVar10 = pos[uVar34].y;
                        c->obj.pos = pos[uVar34];
                        if (bVar1 == 0) {
                            c->ai.i0 = (char)uVar34;
                        } else {
                            // bVar14 = c->ai.i0 - 1;
                            c->ai.i0 = c->ai.i0 - 1;
                            if ((c->ai.i0 & 0x80) != 0) {
                                c->ai.i0 = c->ai.i0 + cVar3;
                            }
                        }
                    } else {
                        // fVar24 = c->obj.pos.z;
                        c->obj.pos.x = NuTrigTable[uVar19 & 0xffff] * fVar10 + c->obj.pos.x;
                        c->obj.pos.z = NuTrigTable[((uVar19 & 0xffff) + 0x4000) & 0x2ffff] * fVar10 + c->obj.pos.z;
                    }
                } else {
                    if ((c->ai.movflags & 2) != 0) {
                        if (c->ai.direction == 0) {
                            c->ai.angle = c->ai.angle + c->ai.rotspeed;
                        } else {
                            c->ai.angle = c->ai.angle - c->ai.rotspeed;
                        }
                        // c->ai.angle = uVar12;
                        uVar34 = (u32)c->ai.angle;
                        // fVar10 = c->ai.radius;
                        // fVar24 = c->ai.newpos.y;
                        // fVar30 = c->ai.newpos.z;
                        c->obj.pos.x = NuTrigTable[c->ai.angle] * c->ai.radius + c->ai.newpos.x;
                        c->obj.pos.y = c->ai.newpos.y;
                        c->obj.pos.z = NuTrigTable[(c->ai.angle + 0x4000) & 0x2ffff] * c->ai.radius + c->ai.newpos.z;
                    } else {
                        if ((c->ai.movflags & 1) != 0) {
                            if ((c->ai.movflags & 0x30) != 0) {
                                if ((c->ai.movflags & 0x10) != 0) {
                                    uVar34 = (dVar40 * 16384.0f);
                                    fVar10 = 1.0f - NuTrigTable[uVar34 & 0xffff];
                                } else {
                                    uVar34 = (dVar40 * 16384.0f + 16384.0f);
                                    fVar10 = NuTrigTable[uVar34 & 0xffff];
                                }
                                // local_68 = (long long)(s32)uVar34;
                                // fVar24 = c->ai.newpos.x;
                                // fVar30 = c->ai.newpos.y;
                                // fVar23 = c->ai.newpos.z;
                                // fVar29 = c->ai.oldpos.x;
                                // fVar6 = c->ai.oldpos.y;
                                c->obj.pos.x = (c->ai.oldpos.x - c->ai.newpos.x) * fVar10 + c->ai.newpos.x;
                                c->obj.pos.y = (c->ai.oldpos.y - c->ai.newpos.y) * fVar10 + c->ai.newpos.y;
                                c->obj.pos.z = (c->ai.oldpos.z - c->ai.newpos.z) * fVar10 + c->ai.newpos.z;
                            } else {
                                // dVar41 = c->ai.oldpos.x;
                                // dVar39 = c->ai.oldpos.y;
                                // dVar42 = c->ai.oldpos.z;
                                // fVar24 = c->ai.newpos.x;
                                // fVar30 = c->ai.newpos.z;
                                // c->obj.pos.y = fVar10;
                                c->obj.pos.x = (c->ai.newpos.x - c->ai.oldpos.x) * dVar40 + c->ai.oldpos.x;
                                c->obj.pos.y = (c->ai.newpos.y - c->ai.oldpos.y) * dVar40 + c->ai.oldpos.y;
                                c->obj.pos.z = (c->ai.newpos.z - c->ai.oldpos.z) * dVar40 + c->ai.oldpos.z;
                                if ((c->ai.movflags & 0x800) != 0) {
                                    // local_68 = (long long)(s32)(dVar40 * 32768.0f);
                                    c->obj.pos.y = NuTrigTable[(s32)(dVar40 * 32768.0f) & 0xffff] * c->ai.height + fVar10;
                                }
                            }
                        } else if ((c->ai.movflags & 4) != 0) {
                            // cVar3 = c->ai.i1;
                            // cVar13 = c->ai.i0;
                            fVar10 = pos[c->ai.i1].x - pos[c->ai.i0].x;
                            fVar24 = pos[c->ai.i1].z - pos[c->ai.i0].z;
                            // fVar10 = NuFsqrt(fVar10 * fVar10 + fVar24 * fVar24);
                            dVar39 = ((c->ai.speed * 0.01666667f) / NuFsqrt(fVar10 * fVar10 + fVar24 * fVar24));
                            dVar41 = RatioAlongLine(&(plr->obj).pos, &pos[c->ai.i0], &pos[c->ai.i1]);
                            // dVar41 = fVar10;
                            dVar42 = NuFabs(c->ai.lateral - dVar41);
                            if (dVar42 > dVar39) {
                                //dVar42 = c->ai.lateral;
                                if (c->ai.lateral > dVar41) {
                                    //dVar42 = dVar42 - dVar39;
                                    //sVar11 = 0x39;
                                //LAB_8000791c:
                                    c->obj.anim.newaction = 0x39;
                                    c->ai.lateral = c->ai.lateral - dVar39;
                                } else {
                                    if (c->ai.lateral < dVar41) {
                                        //dVar42 = dVar42 + dVar39;
                                        //sVar11 = 0x32;
                                        //goto LAB_8000791c;
                                        c->obj.anim.newaction = 0x32;
                                        c->ai.lateral = c->ai.lateral + dVar39;
                                    }
                                }
                                // cVar3 = c->ai.i0;
                                // cVar13 = c->ai.i1;
                                fVar10 = c->ai.lateral;
                                c->obj.pos.x = (pos[c->ai.i1].x - pos[c->ai.i0].x) * fVar10 + pos[c->ai.i0].x;
                                c->obj.pos.y = (pos[c->ai.i1].y - pos[c->ai.i0].y) * fVar10 + pos[c->ai.i0].y;
                                c->obj.pos.z = (pos[c->ai.i1].z - pos[c->ai.i0].z) * fVar10 + pos[c->ai.i0].z;
                            }
                        } else {
                            if ((c->ai.movflags & 8) != 0) {
                                fVar10 = pos[c->ai.i0].y;
                                fVar24 = pos[c->ai.i1].y;
                                fVar30 = fVar10;
                                if (fVar24 < fVar10) {
                                    fVar30 = fVar24;
                                    fVar24 = fVar10;
                                }
                                fVar10 = ((plr->obj).bot + (plr->obj).top) * (plr->obj).SCALE * 0.5f + (plr->obj).pos.y;
                                if ((fVar30 < fVar10)) {
                                    fVar30 = fVar10;
                                } else if (fVar24 > fVar10) {
                                      fVar30 = fVar24;
                                }
                                dVar42 = (fVar30 - fVar10);
                                if (NuFabs(dVar42) > 0.2f) {
                                    if (dVar42 < 0.0f) {
                                        c->obj.anim.newaction = 0x16;
                                    } else {
                                        c->obj.anim.newaction = 0x58;
                                    }
                                } else {
                                    c->obj.anim.newaction = 0x22;
                                }
                                //c->obj.anim.newaction = sVar11;
                                iVar18 = (s32)c->obj.anim.newaction;
                                if ((iVar18 != 0x22) && ((c->obj.model)->anmdata[iVar18] == NULL)) {
                                    c->obj.anim.newaction = 0x22;
                                }
                                c->obj.pos.y = dVar42 * 0.02f + c->obj.pos.y;
                            } else {
                                if ((c->ai.movflags & 0x400) != 0) {
                                    old_hdg = c->obj.hdg;
                                    c->ai.angle = c->ai.angle + c->ai.rotspeed;
                                    NuVecSub(&local_c8, &pos[c->ai.i1], &pos[c->ai.i0]);
                                    iVar18 = NuAtan2D(local_c8.x, local_c8.z);
                                    // uVar12 = c->ai.angle;
                                    c->obj.hdg = (u16)iVar18;
                                    NuVecScale(&local_c8, &local_c8, NuTrigTable[c->ai.angle] * 0.5f + 0.5f);
                                    NuVecAdd(pnVar15, &pos[c->ai.i0], &local_c8);
                                    uVar12 = c->ai.rotflags;
                                    // uVar28 = c->obj.hdg + c->ai.angle;
                                    c->obj.hdg = c->obj.hdg + c->ai.angle;
                                    if ((c->ai.rotflags & 0x400) != 0) {
                                        c->obj.hdg = c->obj.hdg + 0x8000;
                                    }
                                    iVar18 = RotDiff(old_hdg, c->obj.hdg);
                                    c->obj.dyrot = (short)iVar18;
                                }
                            }
                        }
                    }
                }
                if ((c->ai.movflags & 0x100) != 0) {
                    // fVar24 = c->obj.pos.y;
                    // fVar10 = c->obj.pos.z;
                    c->ai.wobblepos = c->obj.pos;
                }
                if ((c->ai.movflags & 0x200) != 0) {
                    c->obj.pos.y = NuTrigTable[c->ai.wobble_ang] * c->ai.wobble + c->ai.wobblepos.y;
                }
                // fVar30 = c->obj.pos.z - c->obj.oldpos.z;
                // fVar10 = c->obj.oldpos.y;
                // fVar24 = c->obj.pos.y;
                // fVar23 = c->obj.pos.x - c->obj.oldpos.x;
                c->obj.mom.x = c->obj.pos.x - c->obj.oldpos.x;
                c->obj.mom.y = c->obj.pos.y - c->obj.oldpos.y;
                c->obj.mom.z = c->obj.pos.z - c->obj.oldpos.z;
                c->obj.xz_distance = NuFsqrt(c->obj.mom.x * c->obj.mom.x + c->obj.mom.z * c->obj.mom.z);
                // fVar10 = c->obj.mom.y;
                // fVar24 = c->obj.mom.x;
                // fVar30 = c->obj.mom.z;
                // c->obj.xz_distance = fVar23;
                fVar10 = NuFsqrt(c->obj.mom.x * c->obj.mom.x + c->obj.mom.y * c->obj.mom.y + c->obj.mom.z * c->obj.mom.z);
                // uVar12 = c->ai.movflags;
                c->obj.xyz_distance = fVar10;
                if ((c->ai.angle & 0x400) == 0) {
                    old_hdg = c->obj.hdg;
                }
                if ((c->ai.angle & 2) != 0) {
                    if (c->ai.direction == 0) {
                        c->obj.hdg = c->ai.angle + 0x4000;
                    } else {
                        c->obj.hdg = c->ai.angle + 0xc000;
                    }
                    goto LAB_800080cc;
                }
                uVar12 = c->ai.rotflags;
                if ((c->ai.rotflags & 1) != 0) {
                    dVar41 = (c->ai.spins * 65536.0f);
                    if ((c->ai.rotflags & 0x80) != 0) {
                        dVar42 = dVar41 * (1.0f - NuTrigTable[(s32)(dVar40 * 16384.0f) & 0xffff]);
                        if ((c->ai.rotflags & 0x400) != 0) {
                            uVar12 = c->ai.old_yrot;
                            iVar18 = (s32)(-dVar41 + dVar42);
                            // local_68 = (long long)iVar18;
                            uVar28 = (u16)iVar18;
                        } else {
                            uVar12 = c->ai.old_yrot;
                            iVar18 = (s32)(dVar41 - dVar42);
                            // local_68 = (long long)iVar18;
                            uVar28 = (u16)iVar18;
                        }
                    } else {
                        if ((c->ai.rotflags & 0x100) != 0) {
                            // local_68 = (long long)(s32)(dVar41 * dVar40);
                            sVar11 = (dVar41 * dVar40);
                            if ((c->ai.rotflags & 0x400) == 0) {
                                c->obj.hdg = c->ai.old_yrot + sVar11;
                            } else {
                                c->obj.hdg = c->ai.old_yrot - sVar11;
                            }
                            //goto LAB_800080c8;
                        }
                        if ((c->ai.rotflags & 0x400) == 0) {
                            uVar12 = c->ai.old_yrot;
                            iVar18 = (s32)dVar41 - (dVar41 * NuTrigTable[(s32)(dVar40 * 16384.0f + 16384.0f) & 0xffff]);
                            // local_68 = (long long)iVar18;
                            uVar28 = (u16)iVar18;
                        } else {
                            uVar12 = c->ai.old_yrot;
                            iVar18 = (s32)(-dVar41 + (dVar41 * NuTrigTable[(s32)(dVar40 * 16384.0f + 16384.0f) & 0xffff]));
                            // local_68 = (long long)iVar18;
                            uVar28 = (u16)iVar18;
                        }
                    }
                LAB_800080c4:
                    uVar12 = uVar12 + uVar28;
                //LAB_800080c8:
                    c->obj.hdg = uVar12;
                } else {
                    if ((c->ai.rotflags & 2) != 0) {
                        iVar18 = NuAtan2D(c->ai.newpos.x - c->ai.oldpos.x, c->ai.newpos.z - c->ai.oldpos.z);
                        uVar34 = RotDiff(c->ai.old_yrot, (short)iVar18 + c->ai.rotadjust);
                        uVar12 = c->ai.rotflags;
                        if (((uVar12 & 0x200) != 0) && ((s32)uVar34 < 0)) {
                            uVar34 = uVar34 + 0x10000;
                        } else {
                            uVar12 = c->ai.rotflags;
                            if (((uVar12 & 0x400) != 0) && (0 < (s32)uVar34)) {
                                uVar34 = uVar34 - 0x10000;
                            }
                        }
                        // local_68 = CONCAT44(0x43300000,uVar34 ^ 0x80000000);
                        dVar41 = (float)uVar34;
                        if ((uVar12 & 0x80) != 0) {
                            dVar42 = dVar41 * (1.0f - NuTrigTable[(s32)(dVar40 * 16384.0f) & 0xffff]);
                            if ((uVar12 & 0x400) != 0) {
                                uVar12 = c->ai.old_yrot;
                                iVar18 = (s32)(-dVar41 + dVar42);
                                // local_68 = (long long)iVar18;
                                uVar28 = (u16)iVar18;
                            } else {
                                uVar12 = c->ai.old_yrot;
                                iVar18 = (s32)(dVar41 - dVar42);
                                // local_68 = (long long)iVar18;
                                uVar28 = (u16)iVar18;
                            }
                        } else {
                            if ((uVar12 & 0x100) != 0) {
                                uVar12 = c->ai.old_yrot;
                                // local_68 = (long long)(s32)(dVar41 * dVar40);
                                uVar28 = (dVar41 * dVar40);
                            } else if ((uVar12 & 0x400) == 0) {
                                uVar12 = c->ai.old_yrot;
                                iVar18 =
                                    (s32)(dVar41 - (dVar41 * NuTrigTable[(s32)(dVar40 * 16384.0f + 16384.0f) & 0xffff]));
                                // local_68 = (long long)iVar18;
                                uVar28 = iVar18;
                            } else {
                                uVar12 = c->ai.old_yrot;
                                iVar18 =
                                    (s32)(-dVar41 + (dVar41 * NuTrigTable[(s32)(dVar40 * 16384.0f + 16384.0f) & 0xffff]));
                                // local_68 = (long long)iVar18;
                                uVar28 = iVar18;
                            }
                        }
                        goto LAB_800080c4;
                    }
                    if ((uVar12 & 0x180c) == 0) {
                        if ((uVar12 & 0x20) == 0) {
                            if ((uVar12 & 0x40) != 0) {
                                uVar12 = c->obj.hdg;
                                uVar28 = c->ai.rotspeed;
                                goto LAB_800080c4;
                            }
                            goto LAB_800080cc;
                        }
                        if (c->obj.xz_distance <= 0.0f)
                            goto LAB_800080cc;
                        iVar18 = NuAtan2D(c->obj.mom.x, c->obj.mom.z);
                        uVar28 = (short)iVar18 + c->ai.rotadjust;
                        uVar34 = RotDiff(c->obj.hdg, uVar28);
                        uVar12 = c->ai.rotspeed;
                        if ((uVar34 >= 0 ? uVar34 : -uVar34) <= uVar12) {
                            c->obj.hdg = uVar28;
                            goto LAB_800080cc;
                        }
                        if ((s32)uVar34 < 1) {
                            if (-1 < (s32)uVar34)
                                goto LAB_800080cc;
                            c->obj.hdg = c->obj.hdg - uVar12;
                        } else {
                            c->obj.hdg = c->obj.hdg + uVar12;
                        }
                        //goto LAB_800080c8;
                    }
                    if ((uVar12 & 0x800) != 0) {
                        fVar10 = FindNearestCreature(pnVar15, (s32)c->ai.look_creature, &local_c8);
                        if (1000000.0f > fVar10) // goto LAB_800080cc;
                            pnVar26 = &local_c8;
                    } else {
                        if ((uVar12 & 0x1000) != 0) {
                            pnVar26 = &ai_lookpos;
                        } else {
                            if ((uVar12 & 8) != 0) {
                                pnVar26 = &GameCam[0].pos;
                            } else {
                                pnVar26 = &(plr->obj).pos;
                            }
                        }
                    }
                    iVar18 = NuAtan2D(pnVar26->x - c->obj.pos.x, pnVar26->z - c->obj.pos.z);
                    uVar12 = (short)iVar18 + c->ai.rotadjust;
                    if ((c->ai.rotflags & 0x10) != 0) {
                        // uVar12 = SeekRot(c->obj.hdg,uVar12,3);
                        c->obj.hdg = SeekRot(c->obj.hdg, uVar12, 3);
                    } else {
                        // uVar12 = TurnRot(c->obj.hdg,uVar12,(u32)c->ai.rotspeed);
                        c->obj.hdg = TurnRot(c->obj.hdg, uVar12, (u32)c->ai.rotspeed);
                    }
                }
            LAB_800080cc:
                if ((c->ai.movflags & 0x400) != 0) {
                    // iVar18 = RotDiff(old_hdg,c->obj.hdg);
                    c->obj.dyrot = (short)RotDiff(old_hdg, c->obj.hdg);
                }
                uVar12 = c->ai.terflags;
                c->obj.ground = 0;
                if ((uVar12 & 1) != 0) {
                    if ((uVar12 & 2) == 0) {
                        if ((uVar12 & 4) == 0) {
                            if ((uVar12 & 8) == 0) {
                                if ((uVar12 & 0x10) == 0) {
                                    if ((uVar12 & 0x60) == 0) {
                                        // fVar10 = NewShadowMaskPlat(pnVar15,0.0f,-1);
                                        c->obj.shadow = NewShadowMaskPlat(pnVar15, 0.0f, -1);
                                        GetSurfaceInfo(&c->obj, 1, c->obj.shadow);
                                    } else {
                                        // fVar10 = NewShadowMaskPlat(pnVar15,0.0f,-1);
                                        c->obj.shadow = NewShadowMaskPlat(pnVar15, 0.0f, -1);
                                        GetSurfaceInfo(&c->obj, 1, c->obj.shadow);
                                        if (c->obj.shadow != 2000000.0f) {
                                            if ((c->ai.terflags & 0x40) != 0) {
                                                dVar40 = 1.0f - dVar40;
                                            }
                                            // fVar10 = c->ai.height;
                                            c->obj.ground = 0;
                                            c->obj.pos.y =
                                                c->ai.height * dVar40 + (c->obj.shadow - c->obj.bot * c->obj.SCALE);
                                        }
                                    }
                                } else {
                                    // fVar10 = NewShadowMaskPlat(pnVar15,0.0f,-1);
                                    c->obj.shadow = NewShadowMaskPlat(pnVar15, 0.0f, -1);
                                    GetSurfaceInfo(&c->obj, 1, c->obj.shadow);
                                    fVar10 = c->obj.shadow;
                                    if (fVar10 != 2000000.0f) {
                                        fVar24 = c->ai.height;
                                        c->obj.ground = (s8)uVar12 & 8;
                                        c->obj.pos.y = (fVar10 - c->obj.bot * c->obj.SCALE) + fVar24;
                                    }
                                }
                            } else {
                                local_c8.x = c->obj.pos.x;
                                local_c8.z = c->obj.pos.z;
                                local_c8.y = c->obj.pos.y - 1.0f;
                                // fVar10 = NewShadowMaskPlat(&local_c8,0.0f,-1);
                                c->obj.shadow = NewShadowMaskPlat(&local_c8, 0.0f, -1);
                                GetSurfaceInfo(&c->obj, 1, c->obj.shadow);
                                if (c->obj.roof_type != -1) {
                                    c->obj.ground = (s8)uVar12 & 4;
                                    c->obj.pos.y = c->obj.roof_y - c->obj.top * c->obj.SCALE;
                                }
                            }
                        } else {
                            // fVar10 = NewShadowMaskPlat(pnVar15,0.0f,-1);
                            c->obj.shadow = NewShadowMaskPlat(pnVar15, 0.0f, -1);
                            GetSurfaceInfo(&c->obj, 1, c->obj.shadow);
                            fVar10 = c->obj.shadow;
                            if ((fVar10 != 2000000.0f)
                                && (fVar10 = fVar10 - c->obj.bot * c->obj.SCALE, c->obj.pos.y <= fVar10))
                            {
                                c->obj.pos.y = fVar10;
                                c->obj.ground = 3;
                            }
                        }
                    } else {
                        // fVar10 = NewShadowMaskPlat(pnVar15,0.0f,-1);
                        c->obj.shadow = NewShadowMaskPlat(pnVar15, 0.0f, -1);
                        GetSurfaceInfo(&c->obj, 1, c->obj.shadow);
                        // fVar10 = c->obj.shadow;
                        if (c->obj.shadow != 2000000.0f) {
                            c->obj.ground = 3;
                            c->obj.pos.y = c->obj.shadow - c->obj.bot * c->obj.SCALE;
                        }
                    }
                    ObjectRotation(&c->obj, 2, 0);
                }
                c->obj.mom.y = c->obj.pos.y - c->obj.oldpos.y;
                if (!bVar2) {
                    iVar18 = -1;
                    dVar40 = 0.5f;
                    iVar37 = 0;
                    pcVar31 = c->ai.event;
                    // pfVar35 = c->ai.event_distance;
                LAB_800083c8:
                    cVar3 = pcVar31[iVar37];
                    if (pcVar31[iVar37] == -1)
                        goto LAB_800089c0;
                    if (pcVar31[iVar37] == '\t') {
                        bVar2 = (plr->obj).dead == 0;
                    LAB_80008964:
                        if (!bVar2)
                            goto LAB_80008968;
                    } else if (pcVar31[iVar37] < '\n') {
                        if (pcVar31[iVar37] == 4) {
                            if (((plr->obj).dead == 0)
                                && (cVar3 = c->ai.event_index[iVar37],
                                    fVar24 = (plr->obj).pos.x - pos[c->ai.event_index[iVar37]].x,
                                    fVar10 = (plr->obj).pos.z - pos[c->ai.event_index[iVar37]].z,
                                    fVar24 * fVar24 + fVar10 * fVar10
                                        < c->ai.event_distance[iVar37] * c->ai.event_distance[iVar37]))
                                goto LAB_80008968;
                        } else if (cVar3 < 5) {
                            if (cVar3 == 1) {
                                if ((plr->obj).dead == 0) {
                                    local_b8.y = c->obj.shadow;
                                    local_b8.x = c->obj.pos.x;
                                    if (local_b8.y == 2000000.0f) {
                                        local_b8.y = c->obj.pos.y;
                                    }
                                    local_b8.z = c->obj.pos.z;
                                    local_a8.x = (plr->obj).pos.x;
                                    local_a8.y = (plr->obj).objbot;
                                    local_a8.z = (plr->obj).pos.z;
                                    NuVecSub(&local_c8, &local_a8, &local_b8);
                                    if (local_c8.x * local_c8.x + local_c8.y * local_c8.y + local_c8.z * local_c8.z
                                        < c->ai.event_distance[iVar37] * c->ai.event_distance[iVar37])
                                        goto LAB_80008968;
                                }
                            } else if (cVar3 < 2) {
                                if ((cVar3 == 0) && ((plr->obj).dead == 0)) {
                                    local_b8.x = c->obj.pos.x;
                                    local_b8.z = c->obj.pos.z;
                                    local_b8.y = (c->obj.objtop + c->obj.objbot) * dVar40;
                                    local_a8.x = (plr->obj).pos.x;
                                    local_a8.y = ((plr->obj).objtop + (plr->obj).objbot) * dVar40;
                                    local_a8.z = (plr->obj).pos.z;
                                    NuVecSub(&local_c8, &local_a8, &local_b8);
                                    if (local_c8.x * local_c8.x + local_c8.y * local_c8.y + local_c8.z * local_c8.z
                                        < c->ai.event_distance[iVar37] * c->ai.event_distance[iVar37])
                                        goto LAB_80008968;
                                }
                            } else if (cVar3 == 2) {
                                if ((plr->obj).dead == 0) {
                                    iVar20 = PlayerLateralInRange(
                                        c->ai.lateral, &(plr->obj).pos, &pos[c->ai.i0], &pos[c->ai.i1],
                                        c->ai.event_distance[iVar37]
                                    );
                                    goto LAB_80008960;
                                }
                            } else if ((cVar3 == 3) && ((plr->obj).dead == 0)) {
                                iVar20 = PlayerLateralOutOfRange(
                                    c->ai.lateral, &(plr->obj).pos, &pos[c->ai.i0], &pos[c->ai.i1],
                                    c->ai.event_distance[iVar37]
                                );
                            LAB_80008960:
                                bVar2 = iVar20 == 0;
                                goto LAB_80008964;
                            }
                        } else if (cVar3 == 6) {
                            if (((plr->obj).dead == 0)
                                && (cVar3 = c->ai.event_index[iVar37],
                                    fVar24 = (plr->obj).pos.x - pos[c->ai.event_index[iVar37]].x,
                                    fVar10 = (plr->obj).pos.z - pos[c->ai.event_index[iVar37]].z,
                                    c->ai.event_distance[iVar37] * c->ai.event_distance[iVar37]
                                        < fVar24 * fVar24 + fVar10 * fVar10))
                                goto LAB_80008968;
                        } else if (cVar3 < 6) {
                            if ((plr->obj).dead == 0) {
                                // cVar3 = c->ai.event_index[iVar37];
                                local_b8.x = (plr->obj).pos.x;
                                local_b8.y = ((plr->obj).objtop + (plr->obj).objbot) * dVar40;
                                local_b8.z = (plr->obj).pos.z;
                                local_a8.x = pos[c->ai.event_index[iVar37]].x;
                                local_a8.y = pos[c->ai.event_index[iVar37]].y;
                                local_a8.z = pos[c->ai.event_index[iVar37]].z;
                                NuVecSub(&local_c8, &local_a8, &local_b8);
                                if (local_c8.x * local_c8.x + local_c8.y * local_c8.y + local_c8.z * local_c8.z
                                    < c->ai.event_distance[iVar37] * c->ai.event_distance[iVar37])
                                    goto LAB_80008968;
                            }
                        } else if (cVar3 == '\a') {
                            if ((plr->obj).dead == 0) {
                                // cVar3 = c->ai.event_index[iVar37];
                                local_b8.x = (plr->obj).pos.x;
                                local_b8.y = ((plr->obj).objtop + (plr->obj).objbot) * dVar40;
                                local_b8.z = (plr->obj).pos.z;
                                local_a8.x = pos[c->ai.event_index[iVar37]].x;
                                local_a8.y = pos[c->ai.event_index[iVar37]].y;
                                local_a8.z = pos[c->ai.event_index[iVar37]].z;
                                NuVecSub(&local_c8, &local_a8, &local_b8);
                                if (c->ai.event_distance[iVar37] * c->ai.event_distance[iVar37]
                                    < local_c8.x * local_c8.x + local_c8.y * local_c8.y + local_c8.z * local_c8.z)
                                    goto LAB_80008968;
                            }
                        } else if (((cVar3 == '\b') && ((plr->obj).dead == 0)) && ((struct obj_s*)c->obj.contact == &plr->obj))
                        {
                            uVar5 = c->obj.contact_type;
                            goto joined_r0x8000893c;
                        }
                    } else if (cVar3 == '\x0e') {
                        if (((plr->obj).dead == 0) && (DESTRUCTIBLECRATECOUNT <= plr_crates.count))
                            goto LAB_80008968;
                    } else if (cVar3 < '\x0f') {
                        if (cVar3 == '\v') {
                            if (((plr->obj).dead == 0)
                                && (fVar10 = NuVecDist(&(plr->obj).pos, pnVar15, NULL),
                                    c->ai.event_distance[iVar37] < fVar10))
                                goto LAB_80008968;
                        } else if (cVar3 < '\v') {
                            if (((plr->obj).dead == 0)
                                && (fVar10 = NuVecDist(&(plr->obj).pos, pnVar15, NULL),
                                    fVar10 < c->ai.event_distance[iVar37]))
                                goto LAB_80008968;
                        } else if (cVar3 == '\f') {
                            if (((plr->obj).dead == 0)
                                && (fVar10 = NuVecXZDist(&(plr->obj).pos, pnVar15, NULL),
                                    fVar10 < c->ai.event_distance[iVar37]))
                                goto LAB_80008968;
                        }
              else if ((cVar3 == '\r') && (((plr->obj).dead == 0 &&
                       (fVar10 = NuVecXZDist(&(plr->obj).pos,pnVar15,NULL), c->ai.event_distance[iVar37] < fVar10))))
                        {
                        LAB_80008968:
                            iVar18 = iVar37;
                        }
                    } else if (cVar3 == '\x12') {
                        if ((plr->obj).dead == 0) {
                            bVar2 = c->obj.kill_contact == 0;
                            goto LAB_80008964;
                        }
                    } else if (cVar3 < '\x13') {
                        if (((cVar3 == '\x10') && ((plr->obj).dead == 0)) && ((struct obj_s*)c->obj.contact == &plr->obj))
                            goto LAB_80008968;
                    } else if (cVar3 == '\x13') {
                        uVar5 = c->hit_type;
                    joined_r0x8000893c:
                        if (uVar5 == 2)
                            goto LAB_80008968;
                    } else if (cVar3 == '\x14') {
                        // iVar20 = (*(code *)c->ai.event_function[iVar37])(c,pos); //FIX
                        goto LAB_80008960;
                    }
                    if (iVar18 == -1)
                        goto LAB_800089c0;
                    if ((c->ai.event_flags[iVar37] & 1) == 0) {
                        iVar16 = (s32)c->ai.event_branch[iVar37];
                    } else {
                        c->ai.event_occured[iVar37] = 1;
                    }
                    pcVar27 = c->ai.event + 1;
                    do {
                        *pcVar27 = -1;
                        pcVar27 = pcVar27 + -1;
                    } while ((s32)pcVar31 <= (s32)pcVar27);
                }
            LAB_800089d0:
                if (iVar16 == -2) {
                    if (c->cmdcurr->cmd - 0x8fU < 2) {
                        if (c->obj.dead != 0)
                            goto LAB_80008d0c;
                        iVar16 = c->cmdcurr->i;
                        if ((iVar16 == 0) && (iVar16 = 1, (c->obj.touch & 6) != 0)) {
                            iVar16 = 4;
                        }
                        if (iVar16 == 1) {
                            iVar18 = NuAtan2D(c->obj.pos.x - (plr->obj).pos.x, c->obj.pos.z - (plr->obj).pos.z);
                            FlyGameObject(&c->obj, (u16)iVar18);
                        }
                        KillGameObject(&c->obj, iVar16);
                    }
                    if (c->obj.dead != 0)
                        goto LAB_80008d0c;
                    if ((((plr->obj).dead != 0) || (c->ai.attack_locator_bits == 0)) || (c->obj.draw_frame == 0))
                        goto LAB_80008d00;
                    // dVar40 = 0.10000001f;
                    uVar33 = 0;
                    iVar16 = 0;
                    iVar37 = 0;
                    iVar18 = 0;
                    goto LAB_80008b50;
                }
                // fVar30 = c->obj.pos.x;
                c->ai.time = 0.0f;
                // fVar10 = c->obj.pos.z;
                // fVar24 = c->obj.pos.y;
                c->ai.oldpos.x = c->obj.pos.x;
                c->ai.oldpos.z = c->obj.pos.z;
                c->ai.oldpos.y = c->obj.pos.y;
                c->ai.old_yrot = c->obj.hdg;
                if (iVar16 == -1) {
                    c->cmdcurr++;
                    goto LAB_80005684;
                }
                c->cmdcurr = &c->cmdtable[iVar16];
            } while (1);
        break;
    }
    
    goto LAB_80008d00;
LAB_800089c0:
    iVar37 = iVar37 + 1;
    // pfVar35 = pfVar35 + 1;
    if (1 < iVar37)
        goto LAB_800089d0;
    goto LAB_800083c8;
LAB_80008b50:
    do {
        if (((c->ai.attack_locator_bits >> (uVar33 & 0x3f) & 1U) != 0) && (c->obj.model->pLOCATOR[iVar18] != 0)) {
            local_a8.y = *(float*)((s32)&c->mtxLOCATOR[0][0]._31 + iVar37);
            fVar10 = c->ai.attack_locator_radius * c->obj.SCALE;
            if (((plr->obj).objbot <= local_a8.y + fVar10) && (local_a8.y - fVar10 <= (plr->obj).objtop)) {
                local_a8.x = *(float*)((s32)&c->mtxLOCATOR[0][0]._30 + iVar37);
                local_a8.z = *(float*)((s32)&c->mtxLOCATOR[0][0]._32 + iVar37);
                fVar23 = local_a8.z - (plr->obj).pos.z;
                fVar30 = local_a8.x - (plr->obj).pos.x;
                fVar24 = (plr->obj).RADIUS + fVar10;
                if (fVar30 * fVar30 + fVar23 * fVar23 <= fVar24 * fVar24) {
                    local_b8.y = local_a8.y - *(float*)((s32)&c->momLOCATOR[0][0].y + iVar16);
                    if (c->obj.attack != 0) {
                        if ((plr->spin == 0)
                            || ((s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES
                                <= (s32)plr->spin_frame)
                            || ((local_b8.y + fVar10) - YTOL <= (plr->obj).oldobjbot))
                        {
                            // bVar14 = (plr->obj).boing;
                            uVar33 = NuAtan2D(fVar30, fVar23);
                            (plr->obj).mom.x = -NuTrigTable[uVar33 & 0xffff] * 0.10000001f;
                            (plr->obj).mom.z = -NuTrigTable[((uVar33 & 0xffff) + 0x4000) & 0x2ffff] * 0.10000001f;
                        // } else if ((local_b8.y + fVar10) - YTOL <= (plr->obj).oldobjbot) {
                            iVar16 = (s32)c->ai.locator_kill;
                            if (iVar16 == -1) {
                                iVar16 = GetDieAnim(&plr->obj, -1);
                            }
                            iVar16 = KillPlayer(&plr->obj, iVar16);
                            if (iVar16 == 0) {
                                KillGameObject(&c->obj, 4);
                            }
                        // } else if ((plr->obj).oldobjtop <= (local_b8.y - fVar10) + YTOL) {
                        //     (plr->obj).mom.y = 0.0f;
                        // } else {
                            // (plr->obj).mom.y = 0.0f;
                            // (plr->obj).boing = (plr->obj).boing | 1;
                            // NewRumble(&plr->rumble, 0x7f);
                            // NewBuzz(&plr->rumble, 0xc);
                        }
                    }
                    break;
                }
            }
        }
        uVar33 = uVar33 + 1;
        iVar16 = iVar16 + 0xc;
        iVar37 = iVar37 + 0x40;
        iVar18 = iVar18 + 4;
    } while ((s32)uVar33 < 0x10);
LAB_80008d00:
    if (c->obj.dead == 0) {
        // sVar7 = c->ai.force_action;
        if (c->ai.force_action != -1) {
            c->obj.anim.newaction = c->ai.force_action;
        }
    LAB_80008d54:
        fVar10 = 0.5f;
    } else {
    LAB_80008d0c:
        // sVar7 = c->obj.die_action;
        if (c->obj.die_action != -1) {
            c->obj.anim.newaction = c->obj.die_action;
            goto LAB_80008d54;
        }
        fVar10 = 0.0f;
    }
    // dVar40 = fVar10;
    if (c->ai.idle_update != 0) {
        UpdateCharacterIdle(c, (s32)c->obj.character);
    }
    if (c->obj.dead == 0) {
        // pCVar21 = c->obj.model;
        UpdateAnimPacket(c->obj.model, &c->obj.anim, fVar10, c->obj.xz_distance);
    } else {
        // pCVar21 = &CModel[c->obj.die_model[0]];
        UpdateAnimPacket(&CModel[c->obj.die_model[0]], &c->obj.anim, fVar10, c->obj.xz_distance);
    }
    // UpdateAnimPacket(pCVar21,&c->obj.anim,fVar10,c->obj.xz_distance);
    // sVar7 = c->obj.character;
    c->obj.frame++;
    switch (c->obj.character) {
        case 0xb1:
            gamesfx_pitch = 200;
            gamesfx_effect_volume = 0x3fff;
            GameSfxLoop(0x61, &c->obj.pos);
        break;
        case 0x9A:
            if (unaff_r17 == 0x35 && 0.00555 < c->obj.xyz_distance) {
                switch (Level) {
                    case 6:
                    iVar16 = 0xb6;
                    break;
                    case 0x22:
                    iVar16 = 0xb5;
                    break;
                    case 2:
                    iVar16 = 0xb7;
                    break;
                    default:
                    iVar16 = -1;
                    break;
                }
                if (iVar16 != -1) {
                    GameSfxLoop(iVar16, &c->obj.pos);
                }
            }
        break;
        case 0x1C:
            switch (Level) {
                case 6:
                iVar16 = 0xb5;
                break;
                case 0x22:
                iVar16 = 0xb4;
                break;
                case 2:
                iVar16 = 0xb6;
                break;
                default:
                iVar16 = -1;
                break;
            }
            if (iVar16 != -1) {
                GameSfxLoop(iVar16, &c->obj.pos);
            }
        break;
        case 0x1E:
        case 0x1B:
        case 0x22:
            switch (Level) {
                case 6:
                iVar16 = 0xb4;
                break;
                case 0x22:
                iVar16 = 0xb3;
                break;
                case 2:
                iVar16 = 0xb5;
                break;
                default:
                iVar16 = -1;
                break;
            }
            if (iVar16 != -1) {
                GameSfxLoop(iVar16, &c->obj.pos);
            }
        break;
    }    
}