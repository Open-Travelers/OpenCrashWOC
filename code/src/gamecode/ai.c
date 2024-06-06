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


void ResetAI(void)		//WIP

{
  float x;
  float z;
  nuvec_s *origin;
  int count;
  int i;
  ai_s *Tab;
  obj_s *obj;
  nuvec_s *pos;
  creature_s *chr;
  double dVar1;
  bool check;
  char t_iRail;
  float y;
  
  check = false;
  if ((Rail[7].type == '\x03') &&
     (count = AheadOfCheckpoint((int)gempath_RPos.iRAIL,(int)gempath_RPos.iALONG,gempath_RPos.fALO NG
                               ), count != 0)) {
    check = true;
  }
  count = 0;
  if (0 < LEVELAICOUNT) {
    dVar1 = 0.0;
    Tab = AITab;
    do {
      GetALONG(Tab->pos,(RPos_s *)0x0,-1,-1,1);
      t_iRail = (char)temp_iRAIL;
      Tab->iRAIL = (char)temp_iRAIL;
      y = temp_fALONG;
      Tab->iALONG = temp_iALONG._2_2_;
      Tab->fALONG = y;
      if ((t_iRail == -1) || (Rail[t_iRail].type != '\x03')) {
        if ((bonus_restart == 0) &&
           (i = AheadOfCheckpoint((int)Tab->iRAIL,(int)Tab->iALONG,Tab->fALONG), i != 0))
        goto LAB_80003688;
      }
      else if (check) {
LAB_80003688:
        Tab->status = '\x01';
      }
      Tab->time = (float)dVar1;
      origin = &Tab->origin;
      x = Tab->pos[0].x;
      Tab->delay = AIType[(byte)Tab->ai_type].delay;
      y = Tab->pos[0].y;
      z = Tab->pos[0].z;
      (Tab->origin).x = x;
      (Tab->origin).y = y;
      (Tab->origin).z = z;
      if (1 < AIType[(byte)Tab->ai_type].points) {
        i = 1;
        if (1 < AIType[(byte)Tab->ai_type].points) {
          pos = Tab->pos;
          do {
            pos = pos + 1;
            i = i + 1;
            NuVecAdd(origin,origin,pos);
          } while (i < AIType[(byte)Tab->ai_type].points);
        }
        NuVecScale(origin,origin,
                   1.0 / (float)((double)CONCAT44(0x43300000,
                                                  (int)AIType[(byte)Tab->ai_type].points ^
                                                  0x80000000) - 4503601774854144.0));
      }
      count = count + 1;
      Tab = Tab + 1;
    } while (count < LEVELAICOUNT);
  }
  chr = Character + 1;
  obj = &Character[1].obj;
  do {
    if (chr->used != '\0') {
      RemoveGameObject(obj);
      chr->used = '\0';
      chr->on = '\0';
    }
    chr = chr + 1;
    obj = obj + 1;
  } while ((int)chr < -0x7fdace07);
  return;
}


void ResetDRAINDAMAGE(void)

{
  int qr;
  
  DrainDamage_Intro = 1;
  drain_wawa_ok = 0;
  qr = qrand();
  drain_ang[0] = (ushort)qr;
  qr = qrand();
  drain_ang[1] = (ushort)qr;
  qr = qrand();
  drain_ang[2] = (ushort)qr;
  return;
}


void UpdateDRAINDAMAGE(void)

{
  int i;
  int xrot;
  int yrot;
  creature_s *crunch;
  double Y_vec;
  float fVar1;
  float x;
  float y;
  float z;
  nuvec_s v;
  
  FindNearestCreature(&(player->obj).pos,0x7f,(nuvec_s *)0x0);
  if (temp_creature_i == -1) {
    crunch = (creature_s *)0x0;
  }
  else {
    crunch = Character + temp_creature_i;
  }
  drain_wawa_model = (CharacterModel *)0x0;
  i = (int)CRemap[87];
  if (i != -1) {
    drain_wawa_model = CModel + i;
    if ((CModel[i].anmdata[0xc] == (nuAnimData_s *)0x0) ||
       (CModel[i].anmdata[0xd] == (nuAnimData_s *)0x0)) {
      drain_wawa_model = (CharacterModel *)0x0;
    }
    else {
      drain_ang[0] = drain_ang[0] + 0x6bd;
      drain_ang[1] = drain_ang[1] + 0x492;
      drain_ang[2] = drain_ang[2] + 0x8e8;
      if (crunch == (creature_s *)0x0) {
        if (drain_wawa_ok == 0) {
          drain_wawa_ok = (int)crunch;
          return;
        }
      }
      else {
        x = (crunch->obj).pos.x - NuTrigTable[(crunch->obj).hdg] * 3.0;
        y = (crunch->obj).pos.y + DRAINWAWADY;
        z = (crunch->obj).pos.z -
            *(float *)((int)NuTrigTable + (((crunch->obj).hdg + 0x4000) * 4 & 0x3fffc)) * 3.0;
      }
      NuVecSub(&v,&GameCam.pos,&drain_wawa_pos);
      Y_vec = (double)v.y;
      fVar1 = NuFsqrt(v.x * v.x + v.z * v.z);
      xrot = NuAtan2D((float)Y_vec,fVar1);
      yrot = NuAtan2D(v.x,v.z);
      if (drain_wawa_ok == 0) {
        drain_wawa_pos.x = x;
        drain_wawa_ok = 1;
        drain_wawa_pos.y = y;
        drain_wawa_pos.z = z;
        drain_wawa_xrot = (ushort)xrot;
        drain_wawa_yrot = (ushort)yrot;
        ResetAnimPacket(&drain_wawa_anim,0xc);
        GameSfx(0x1e,&drain_wawa_pos);
      }
      else {
        drain_wawa_xrot = SeekRot(drain_wawa_xrot,(ushort)xrot,3);
        drain_wawa_yrot = SeekRot(drain_wawa_yrot,(ushort)yrot,3);
      }
      drain_wawa_anim.oldaction = drain_wawa_anim.action;
      UpdateAnimPacket(CModel + i,&drain_wawa_anim,0.5,0.0);
      if ((drain_wawa_anim.flags & 1) != 0) {
        if (drain_wawa_anim.action == 0xc) {
          if ((crunch == (creature_s *)0x0) ||
             (fVar1 = NuVecDistSqr(&drain_wawa_pos,&(crunch->obj).pos,(nuvec_s *)0x0), 25.0 < fVar 1)
             ) {
            drain_wawa_anim.newaction = 0xd;
            GameSfx(0x21,&drain_wawa_pos);
          }
        }
        else {
          drain_wawa_ok = 0;
        }
      }
    }
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

void ResetCRUNCHTIME(void)

{
  uint *attack;
  int i;
  nuvec_s v;
  
  i = 9;
  fire_attack_on = -1;
  attack = weather_attack_on + 8;
  fire_attack_wait = 0;
  jcrunch = 0;
  crunch_vulnerable = 0;
  crunchtime_attack_phase = 0;
  earth_attack_on = 0;
  earth_attack_wait = 0;
  water_attack_on = 0;
  water_attack_wait = 0;
  water_last_chute = -1;
  do {
    *attack = 0;
    attack = attack + -1;
    i = i + -1;
  } while (i != 0);
  CrunchTime_Intro = 1;
  weather_attack_wait = 0;
  InitVehMasks();
  InitVehMask(0,3);
  v.y = 5.0;
  v.x = 0.0;
  v.z = 0.0;
  SetNewMaskStuff(0,&crunchtime_arena_midpos,&v,0.0,-540.0,1.0,0,0,1.0,0.0);
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


int GetTotalSpaceBossObjectives(void)

{
  return 4;
}


int GetCurrentSpaceBossObjectives(void)

{
  return 4 - crunchtime_attack_phase;
}


float RatioDifferenceAlongLine(float r0,float r1,nuvec_s *p0,nuvec_s *p1)

{
  double diff;
  float dx;
  float dz;
  
  diff = (double)(r1 - r0);
  dz = (float)((double)(p1->z - p0->z) * diff);
  dx = (float)((double)(p1->x - p0->x) * diff);
  dz = NuFsqrt(dx * dx + dz * dz);
  if (diff < 0.0) {
    dz = -dz;
  }
  return dz;
}


int FindAILabel(creatcmd_s *cmd,int i)

{
  int j;
  
  j = 0;
  while( true ) {
    if (cmd->cmd == 0x90) {
      return 0;
    }
    if ((cmd->cmd == 0x8b) && (cmd->i == i)) break;
    cmd = cmd + 1;
    j = j + 1;
  }
  return j;
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


int PlayerLateralInRange(float lateral,nuvec_s *pos,nuvec_s *p0,nuvec_s *p1,float distance)

{
  double dVar1;
  double dVar2;
  float fVar3;
  
  dVar2 = (double)distance;
  dVar1 = (double)lateral;
  fVar3 = RatioAlongLine(pos,p0,p1);
  fVar3 = RatioDifferenceAlongLine((float)dVar1,fVar3,p0,p1);
  dVar1 = (double)fVar3;
  NuFabs(fVar3);
  return (uint)(dVar1 < dVar2);
}


int PlayerLateralOutOfRange(float lateral,nuvec_s *pos,nuvec_s *p0,nuvec_s *p1,float distance)

{
  double dVar1;
  double dVar2;
  float fVar3;
  
  dVar2 = (double)distance;
  dVar1 = (double)lateral;
  fVar3 = RatioAlongLine(pos,p0,p1);
  fVar3 = RatioDifferenceAlongLine((float)dVar1,fVar3,p0,p1);
  dVar1 = (double)fVar3;
  NuFabs(fVar3);
  return (uint)(dVar2 < dVar1);
}


float FindNearestCreature(nuvec_s *pos,int character,nuvec_s *dst)		//CHECK

{

  float z;
  float y;
  int unaff_r13;
  int j;
  int i;
  creature_s *c;
  nuvec_s *p;
  double dVar1;
  double dVar2;
  
  dVar2 = 1000000.0;
  c = Character;
  j = -1;
  i = 0;
  do {
    if ((c->on != '\0') && (c->used != '\0')) {
      p = &(c->obj).pos;
      if (((c->obj).character == character) && (pos != p)) {
        z = NuVecDistSqr(pos,p,(nuvec_s *)0x0);
        dVar1 = (double)z;
        if ((dVar1 < dVar2) && (j = i, dVar2 = dVar1, dst != (nuvec_s *)0x0)) {
          z = (c->obj).pos.z;
          y = (c->obj).pos.y;
          dst->x = (c->obj).pos.x;
          dst->y = y;
          dst->z = z;
      }
    }
    i = i + 1;
    c = c + 1;
  } while (i < 9);
  temp_creature_i = j;
  return (float)dVar2;
}




void MoveCreature(creature_s *c)		//TODO

{
  bool bVar1;
  bool bVar2;
  char cVar3;
  char cVar4;
  uchar uVar5;
  float fVar6;
  short sVar7;
  creature_s *pcVar8;
  creatcmd_s *pcVar9;
  char cVar13;
  byte bVar14;
  float fVar10;
  short sVar11;
  ushort uVar12;
  nuvec_s *pnVar15;
  int iVar16;
  creature_s *pcVar17;
  int iVar18;
  uint uVar19;
  int iVar20;
  CharacterModel *pCVar21;
  nuAnimData_s *pnVar22;
  float fVar23;
  ushort uVar28;
  float fVar24;
  short *psVar25;
  nuvec_s *pnVar26;
  char *pcVar27;
  float fVar29;
  float fVar30;
  char *pcVar31;
  int *piVar32;
  int unaff_r14;
  ushort unaff_r15;
  uint unaff_r17;
  uint uVar33;
  nuvec_s *pos;
  uint uVar34;
  float *pfVar35;
  uint uVar36;
  int iVar37;
  obj_s *poVar38;
  double dVar39;
  double dVar40;
  double dVar41;
  double dVar42;
  nuvec_s local_c8;
  nuvec_s local_b8;
  nuvec_s local_a8;
  nuvec_s local_98;
  uint local_88;
  int local_84;
  double local_68;
  
  pcVar8 = player;
  GetTopBot(player);
  NewTopBot(&pcVar8->obj);
  cVar3 = (c->obj).dead;
  if (cVar3 == '\0') {
    pnVar15 = &(c->obj).pos;
    fVar10 = (c->obj).top;
    fVar24 = (c->obj).bot;
    fVar29 = (c->obj).pos.z;
    poVar38 = &c->obj;
    fVar23 = (c->obj).pos.y;
    (c->obj).oldpos.x = (c->obj).pos.x;
    fVar30 = (c->obj).SCALE;
    local_98.y = (c->obj).pos.y;
    local_98.x = (c->obj).pos.x;
    local_98.z = (c->obj).pos.z;
    (c->obj).oldpos.z = fVar29;
    (c->obj).oldpos.y = fVar23;
    local_98.y = (fVar24 + fVar10) * fVar30 * 0.5 + local_98.y;
    (c->obj).anim.oldaction = (c->obj).anim.action;
    OldTopBot();
    unaff_r17 = (uint)(byte)AITab[c->i_aitab].ai_type;
    pos = AITab[c->i_aitab].pos;
    sVar7 = AIType[unaff_r17].points;
    uVar33 = (uint)sVar7;
    if (c->cmdcurr != (creatcmd_s *)0x0) goto LAB_80005684;
    pcVar9 = c->cmdtable;
    do {
      c->cmdcurr = pcVar9;
LAB_80005684:
      pcVar17 = player;
      pcVar9 = c->cmdcurr;
      bVar2 = true;
      iVar16 = pcVar9->cmd;
      cVar3 = (char)sVar7;
      if (iVar16 == 0x41) {
        (c->ai).duration = 0.0;
      }
      else if (iVar16 < 0x42) {
        if (iVar16 == 0x1f) {
          uVar5 = (c->ai).hits;
          if (uVar5 != '\0') {
            (c->ai).hits = uVar5 + 0xff;
          }
        }
        else if (iVar16 < 0x20) {
          if (iVar16 == 0xf) {
            iVar16 = pcVar9->i >> 8;
            uVar34 = pcVar9->i & 0xff;
            (c->ai).newpos.x = (pos[uVar34].x - pos[iVar16].x) * pcVar9->f + pos[iVar16].x;
            (c->ai).newpos.y = (pos[uVar34].y - pos[iVar16].y) * pcVar9->f + pos[iVar16].y;
            (c->ai).newpos.z = (pos[uVar34].z - pos[iVar16].z) * pcVar9->f + pos[iVar16].z;
          }
          else if (iVar16 < 0x10) {
            if (iVar16 == 7) {
              iVar16 = pcVar9->i >> 8;
              uVar34 = pcVar9->i & 0xff;
              (c->obj).pos.x = (pos[uVar34].x - pos[iVar16].x) * pcVar9->f + pos[iVar16].x;
              (c->obj).pos.y = (pos[uVar34].y - pos[iVar16].y) * pcVar9->f + pos[iVar16].y;
              (c->obj).pos.z = (pos[uVar34].z - pos[iVar16].z) * pcVar9->f + pos[iVar16].z;
            }
            else if (iVar16 < 8) {
              if (iVar16 == 3) {
                if (((c->obj).flags & 0x10000) == 0) {
                  fVar10 = (c->obj).shadow;
                  if (fVar10 != 2000000.0) {
                    fVar24 = (c->obj).bot;
                    goto LAB_80005c98;
                  }
                }
                else {
                  fVar10 = (c->obj).roof_y;
                  if (fVar10 != 2000000.0) {
                    fVar24 = (c->obj).top;
LAB_80005c98:
                    (c->obj).pos.y = fVar10 - fVar24 * (c->obj).SCALE;
                  }
                }
              }
              else if (iVar16 < 4) {
                if (iVar16 == 1) {
                  cVar13 = *(char *)((int)&c->cmdcurr->i + 3);
LAB_80005d30:
                  (c->ai).i0 = cVar13;
                  fVar24 = pos[cVar13].z;
                  fVar10 = pos[cVar13].y;
                  (c->obj).pos.x = pos[cVar13].x;
                  (c->obj).pos.y = fVar10;
                  (c->obj).pos.z = fVar24;
                }
                else if (iVar16 < 2) {
                  if (iVar16 != 0) goto LAB_80005a24;
                  memset(&c->ai,0,0x98);
                  pcVar31 = (c->ai).event + 1;
                  (c->ai).scale = 1.0;
                  do {
                    *pcVar31 = -1;
                    pcVar31 = pcVar31 + -1;
                  } while ((int)(c->ai).event <= (int)pcVar31);
                  if ((c->obj).character == 0x3f) {
                    FindGongBongerAnim(pos,&(c->ai).obj);
                  }
                  (c->ai).look_creature = -1;
                  (c->ai).locator_kill = -1;
                  (c->ai).kill = -1;
                  (c->ai).force_action = -1;
                }
                else {
                  iVar16 = c->cmdcurr->i;
                  fVar24 = pos[iVar16].z;
                  fVar10 = pos[iVar16].y;
                  (player->obj).pos.x = pos[iVar16].x;
                  (pcVar17->obj).pos.z = fVar24;
                  (pcVar17->obj).pos.y = fVar10;
                  (pcVar17->obj).oldpos.x = (pcVar17->obj).pos.x;
                  (pcVar17->obj).oldpos.z = fVar24;
                  (pcVar17->obj).oldpos.y = fVar10;
                  fVar24 = v000.z;
                  fVar10 = v000.y;
                  (pcVar17->obj).mom.x = v000.x;
                  (pcVar17->obj).mom.z = fVar24;
                  (pcVar17->obj).mom.y = fVar10;
                  (c->obj).touch = '\0';
                  (c->obj).contact = (void *)0x0;
                  (c->obj).kill_contact = '\0';
                }
              }
              else if (iVar16 == 5) {
                cVar13 = (c->ai).i0;
                cVar4 = (c->ai).i1;
                fVar10 = c->cmdcurr->f;
                (c->ai).lateral = fVar10;
                (c->obj).pos.x = (pos[cVar4].x - pos[cVar13].x) * fVar10 + pos[cVar13].x;
                (c->obj).pos.y = (pos[cVar4].y - pos[cVar13].y) * fVar10 + pos[cVar13].y;
                (c->obj).pos.z = (pos[cVar4].z - pos[cVar13].z) * fVar10 + pos[cVar13].z;
              }
              else if (iVar16 < 6) {
                (c->obj).pos.y = (c->obj).pos.y + c->cmdcurr->f;
              }
              else {
                fVar10 = RatioAlongLine(&(pcVar8->obj).pos,pos + (c->ai).i0,pos + (c->ai).i1);
                cVar13 = (c->ai).i0;
                cVar4 = (c->ai).i1;
                (c->ai).lateral = fVar10;
                (c->obj).pos.x = (pos[cVar4].x - pos[cVar13].x) * fVar10 + pos[cVar13].x;
                (c->obj).pos.y = (pos[cVar4].y - pos[cVar13].y) * fVar10 + pos[cVar13].y;
                (c->obj).pos.z = (pos[cVar4].z - pos[cVar13].z) * fVar10 + pos[cVar13].z;
              }
            }
            else if (iVar16 == 0xb) {
              fVar10 = (c->obj).shadow;
              if (fVar10 != 2000000.0) {
                (c->ai).newpos.y = fVar10;
              }
            }
            else if (iVar16 < 0xc) {
              if (iVar16 != 9) {
                if (9 < iVar16) {
                  pnVar26 = &(c->ai).newpos;
                  fVar24 = (c->obj).pos.y;
                  fVar10 = (c->obj).pos.z;
                  (c->ai).newpos.x = (c->obj).pos.x;
                  goto LAB_800062d0;
                }
                cVar13 = cVar3 + -1;
                goto LAB_80005d30;
              }
              pcVar9 = c->cmdcurr;
              (c->ai).newpos.x = pos[pcVar9->i].x;
              (c->ai).newpos.y = pos[pcVar9->i].y;
              (c->ai).newpos.z = pos[pcVar9->i].z;
            }
            else if (iVar16 == 0xd) {
              cVar13 = (c->ai).i0;
              cVar4 = (c->ai).i1;
              fVar10 = c->cmdcurr->f;
              (c->ai).lateral = fVar10;
              (c->ai).newpos.x = (pos[cVar4].x - pos[cVar13].x) * fVar10 + pos[cVar13].x;
              (c->ai).newpos.y = (pos[cVar4].y - pos[cVar13].y) * fVar10 + pos[cVar13].y;
              (c->ai).newpos.z = (pos[cVar4].z - pos[cVar13].z) * fVar10 + pos[cVar13].z;
            }
            else if (iVar16 < 0xe) {
              (c->ai).newpos.y = (c->ai).newpos.y + c->cmdcurr->f;
            }
            else {
              fVar10 = RatioAlongLine(&(pcVar8->obj).pos,pos + (c->ai).i0,pos + (c->ai).i1);
              cVar13 = (c->ai).i0;
              cVar4 = (c->ai).i1;
              (c->ai).lateral = fVar10;
              (c->ai).newpos.x = (pos[cVar4].x - pos[cVar13].x) * fVar10 + pos[cVar13].x;
              (c->ai).newpos.y = (pos[cVar4].y - pos[cVar13].y) * fVar10 + pos[cVar13].y;
              (c->ai).newpos.z = (pos[cVar4].z - pos[cVar13].z) * fVar10 + pos[cVar13].z;
            }
          }
          else if (iVar16 == 0x17) {
            uVar34 = pcVar9->i >> 0x1f;
            iVar16 = (uVar34 ^ pcVar9->i) - uVar34;
            if (iVar16 < 1) {
              (c->ai).count = '\0';
            }
            else {
              iVar18 = qrand();
              (c->ai).count = (uchar)(iVar18 / (0xffff / (iVar16 + 1) + 1));
            }
          }
          else if (iVar16 < 0x18) {
            if (iVar16 < 0x15) {
              if (iVar16 < 0x13) {
                if (iVar16 == 0x11) {
                  bVar14 = (c->ai).i0 - 1;
                  (c->ai).i0 = bVar14;
                  if ((bVar14 & 0x80) != 0) {
                    (c->ai).i0 = bVar14 + cVar3;
                  }
                  iVar16 = (int)(c->ai).i0;
                }
                else if (iVar16 < 0x12) {
                  cVar13 = (c->ai).i0 + '\x01';
                  (c->ai).i0 = cVar13;
                  if ((int)cVar13 == uVar33) {
                    (c->ai).i0 = '\0';
                  }
                  iVar16 = (int)(c->ai).i0;
                }
                else {
                  iVar16 = (int)(char)(cVar3 + -1);
                  (c->ai).i0 = cVar3 + -1;
                }
                fVar10 = pos[iVar16].z;
                fVar24 = pos[iVar16].y;
                (c->ai).newpos.x = pos[iVar16].x;
                (c->ai).newpos.z = fVar10;
                (c->ai).newpos.y = fVar24;
              }
              else {
                local_c8.y = 0.0;
                local_c8.x = 0.0;
                local_c8.z = c->cmdcurr->f;
                if (c->cmdcurr->cmd == 0x13) {
                  uVar34 = qrand();
                  local_68 = (double)CONCAT44(0x43300000,uVar34 ^ 0x80000000);
                  local_c8.z = local_c8.z * (float)(local_68 - 4503601774854144.0) * 1.525902e-05;
                }
                iVar16 = qrand();
                pnVar26 = &(c->ai).newpos;
                NuVecRotateX(&local_c8,&local_c8,iVar16);
                iVar16 = qrand();
                NuVecRotateY(pnVar26,&local_c8,iVar16);
                NuVecAdd(pnVar26,pnVar26,&(c->ai).wobblepos);
              }
            }
            else if (iVar16 == 0x15) {
              iVar16 = qrand();
              iVar16 = iVar16 - (iVar16 / (int)uVar33) * uVar33;
              (c->ai).newpos.x = pos[iVar16].x;
              (c->ai).newpos.y = pos[iVar16].y;
              (c->ai).newpos.z = pos[iVar16].z;
            }
            else if (iVar16 == 0x16) {
              (c->ai).count = *(uchar *)((int)&c->cmdcurr->i + 3);
            }
            else {
LAB_80005a24:
              bVar2 = false;
            }
          }
          else if (iVar16 == 0x1b) {
            (c->ai).hits = *(uchar *)((int)&pcVar9->i + 3);
          }
          else if (iVar16 < 0x1c) {
            if (iVar16 == 0x19) {
              (c->ai).count = (c->ai).count + '\x01';
            }
            else if (iVar16 < 0x1a) {
              iVar16 = (uint)(c->ai).count + c->cmdcurr->i;
              if (iVar16 < 0) {
                iVar16 = 0;
              }
              (c->ai).count = (uchar)iVar16;
            }
            else {
              uVar5 = (c->ai).count;
              if (uVar5 != '\0') {
                (c->ai).count = uVar5 + 0xff;
              }
            }
          }
          else if (iVar16 == 0x1d) {
            iVar16 = (uint)(c->ai).hits + c->cmdcurr->i;
            if (iVar16 < 0) {
              iVar16 = 0;
            }
            (c->ai).hits = (uchar)iVar16;
          }
          else if (iVar16 < 0x1e) {
            uVar34 = c->cmdcurr->i;
            uVar19 = (int)uVar34 >> 0x1f;
            iVar16 = (uVar19 ^ uVar34) - uVar19;
            if (iVar16 < 1) {
              (c->ai).hits = '\0';
            }
            else {
              iVar18 = qrand();
              (c->ai).hits = (uchar)(iVar18 / (0xffff / (iVar16 + 1) + 1));
            }
          }
          else {
            (c->ai).hits = (c->ai).hits + '\x01';
          }
        }
        else if (iVar16 == 0x31) {
          (c->ai).look_creature = *(short *)((int)&pcVar9->i + 2);
        }
        else if (iVar16 < 0x32) {
          if (iVar16 == 0x29) {
            iVar16 = qrand();
            (c->obj).hdg = (ushort)iVar16;
          }
          else if (iVar16 < 0x2a) {
            if (iVar16 < 0x26) {
              if (iVar16 < 0x23) {
                if (iVar16 == 0x21) {
                  (c->obj).hdg = *(ushort *)((int)&c->cmdcurr->i + 2);
                  goto LAB_80007394;
                }
                if (iVar16 != 0x22) goto LAB_80005a24;
                iVar16 = c->cmdcurr->i;
                fVar10 = pos[iVar16].x - (c->obj).pos.x;
                fVar24 = pos[iVar16].z - (c->obj).pos.z;
              }
              else {
LAB_800060f0:
                iVar16 = c->cmdcurr->cmd;
                if (iVar16 == 0x28) {
                  pnVar26 = &ai_lookpos;
                }
                else if (iVar16 == 0x25) {
                  pnVar26 = &(c->ai).newpos;
                }
                else if (iVar16 == 0x24) {
                  pnVar26 = &GameCam.pos;
                }
                else {
                  pnVar26 = &(pcVar8->obj).pos;
                }
                fVar10 = pnVar26->x - (c->obj).pos.x;
                fVar24 = pnVar26->z - (c->obj).pos.z;
              }
            }
            else {
              if (iVar16 == 0x27) {
                fVar10 = FindNearestCreature(pnVar15,(int)(c->ai).look_creature,&local_c8);
                if (1000000.0 <= fVar10) goto LAB_80007394;
              }
              else {
                if (0x27 < iVar16) goto LAB_800060f0;
                local_c8.x = v000.x;
                local_c8.z = v000.z;
                local_c8.y = v000.y;
                uVar34 = uVar33;
                pnVar26 = pos;
                if (0 < (int)uVar33) {
                  do {
                    NuVecAdd(&local_c8,&local_c8,pnVar26);
                    uVar34 = uVar34 - 1;
                    pnVar26 = pnVar26 + 1;
                  } while (uVar34 != 0);
                }
                local_68 = (double)CONCAT44(0x43300000,uVar33 ^ 0x80000000);
                NuVecScale(&local_c8,&local_c8,1.0 / (float)(local_68 - 4503601774854144.0));
              }
              fVar10 = local_c8.x - (c->obj).pos.x;
              fVar24 = local_c8.z - (c->obj).pos.z;
            }
            iVar16 = NuAtan2D(fVar10,fVar24);
            (c->obj).hdg = (ushort)iVar16;
          }
          else if (iVar16 == 0x2d) {
            (c->ai).rotspeed = *(ushort *)((int)&pcVar9->i + 2);
          }
          else if (iVar16 < 0x2e) {
            if (iVar16 == 0x2b) {
              GetALONG(pnVar15,&(c->obj).RPos,(int)(c->obj).RPos.iRAIL,(int)(c->obj).RPos.iALONG,
                       c->cmdcurr->i);
            }
            else {
              if (iVar16 < 0x2c) {
                pcVar9 = c->cmdcurr;
                uVar12 = (c->obj).hdg;
              }
              else {
                if ((c->obj).RPos.iRAIL == -1) goto LAB_80007394;
                pcVar9 = c->cmdcurr;
                uVar12 = (c->obj).RPos.angle;
              }
              (c->obj).hdg = uVar12 + *(short *)((int)&pcVar9->i + 2);
            }
          }
          else if (iVar16 == 0x2f) {
            pnVar26 = &(c->ai).wobblepos;
            fVar24 = (c->obj).pos.y;
            fVar10 = (c->obj).pos.z;
            (c->ai).wobblepos.x = (c->obj).pos.x;
LAB_800062d0:
            pnVar26->z = fVar10;
            pnVar26->y = fVar24;
          }
          else if (iVar16 < 0x30) {
            iVar16 = qrand();
            iVar16 = c->cmdcurr->i * iVar16;
            if (iVar16 < 0) {
              iVar16 = iVar16 + 0xffff;
            }
            (c->ai).rotspeed = (ushort)((uint)iVar16 >> 0x10);
          }
          else {
            iVar16 = qrand();
            (c->ai).wobble_ang = (ushort)iVar16;
            pcVar9 = c->cmdcurr;
            (c->ai).wobble_speed = *(ushort *)((int)&pcVar9->i + 2);
            (c->ai).wobble = pcVar9->f;
          }
        }
        else if (iVar16 == 0x39) {
          (c->ai).kill = *(char *)((int)&pcVar9->i + 3);
        }
        else if (iVar16 < 0x3a) {
          if (iVar16 == 0x35) {
            pnVar26 = pnVar15;
            if (((c->obj).flags & 0x10000) != 0) {
              local_c8.x = (c->obj).pos.x;
              local_c8.z = (c->obj).pos.z;
              local_c8.y = (c->obj).pos.y - 1.0;
              pnVar26 = &local_c8;
            }
            fVar10 = NewShadowMaskPlat(pnVar26,0.0,-1);
            (c->obj).shadow = fVar10;
            GetSurfaceInfo(poVar38,1,(c->obj).shadow);
            ObjectRotation(poVar38,0,0);
            uVar12 = temp_zrot;
            (c->obj).surface_xrot = temp_xrot;
            (c->obj).surface_zrot = uVar12;
          }
          else if (iVar16 < 0x36) {
            if (iVar16 == 0x33) {
              (c->ai).rotadjust = *(ushort *)((int)&c->cmdcurr->i + 2);
            }
            else if (iVar16 < 0x34) {
              iVar16 = qrand();
              if (iVar16 < (int)(uint)*(ushort *)((int)&c->cmdcurr->i + 2)) {
                (c->ai).rotspeed = -(c->ai).rotspeed;
              }
            }
            else {
              (c->obj).shadow = 2000000.0;
            }
          }
          else if (iVar16 == 0x37) {
            (c->obj).attack = *(ushort *)((int)&c->cmdcurr->i + 2);
          }
          else if (iVar16 < 0x38) {
            if (((c->obj).flags & 0x10000) == 0) {
              uVar12 = (c->obj).surface_xrot;
              uVar28 = (c->obj).surface_zrot;
            }
            else {
              uVar12 = (c->obj).roof_xrot;
              uVar28 = (c->obj).roof_zrot;
            }
            (c->obj).xrot = uVar12;
            (c->obj).zrot = uVar28;
          }
          else {
            uVar12 = *(ushort *)((int)&c->cmdcurr->i + 2);
            (c->obj).vulnerable = uVar12;
            if (((((c->obj).flags & 4) != 0) && (unaff_r17 < 0x3e)) && ((c->obj).character != 0x16 ))
            {
              (c->obj).vulnerable = uVar12 | 0x180;
            }
          }
        }
        else if (iVar16 == 0x3d) {
          NuVecSub(&local_c8,&(c->ai).newpos,&(c->ai).oldpos);
          fVar10 = NuVecMag(&local_c8);
          fVar10 = fVar10 / c->cmdcurr->f;
          (c->ai).duration = fVar10;
          if (fVar10 < 0.01666667) {
            (c->ai).duration = 0.01666667;
          }
        }
        else if (iVar16 < 0x3e) {
          if (iVar16 == 0x3b) {
            pcVar9 = c->cmdcurr;
            (c->ai).attack_locator_bits = *(ushort *)((int)&pcVar9->i + 2);
            (c->ai).attack_locator_radius = pcVar9->f;
          }
          else if (iVar16 < 0x3c) {
            (c->ai).locator_kill = *(char *)((int)&c->cmdcurr->i + 3);
          }
          else {
            (c->ai).duration = c->cmdcurr->f;
          }
        }
        else if (iVar16 == 0x3f) {
          pCVar21 = (c->obj).model;
          sVar11 = (c->obj).anim.newaction;
          if (pCVar21->anmdata[sVar11] == (nuAnimData_s *)0x0) {
LAB_80006c8c:
            (c->ai).duration = 1.0;
          }
          else {
            (c->ai).duration = (c->cmdcurr->f - 1.0) / (pCVar21->animlist[sVar11]->speed * 30.0);
          }
        }
        else if (iVar16 < 0x40) {
          pCVar21 = (c->obj).model;
          sVar11 = (c->obj).anim.newaction;
          pnVar22 = pCVar21->anmdata[sVar11];
          if (pnVar22 == (nuAnimData_s *)0x0) {
            fVar10 = 1.0;
          }
          else {
            fVar10 = (pnVar22->time - 1.0) / (pCVar21->animlist[sVar11]->speed * 30.0);
          }
          (c->ai).duration = fVar10;
          fVar10 = c->cmdcurr->f;
          if (1.0 < fVar10) {
            fVar24 = (c->ai).duration;
            goto LAB_80006b98;
          }
        }
        else {
          sVar11 = (c->obj).anim.newaction;
          pCVar21 = (c->obj).model;
          pnVar22 = pCVar21->anmdata[sVar11];
          if (pnVar22 == (nuAnimData_s *)0x0) goto LAB_80006c8c;
          fVar24 = c->cmdcurr->f;
          fVar10 = pnVar22->time;
          fVar30 = (c->obj).anim.anim_time;
          if (((1.0 < fVar24) && (fVar24 < fVar10)) && (fVar30 < fVar24)) {
            fVar10 = fVar24;
          }
          fVar10 = fVar10 - fVar30;
          fVar24 = pCVar21->animlist[sVar11]->speed * 30.0;
LAB_80006c80:
          (c->ai).duration = fVar10 / fVar24;
        }
      }
      else if (iVar16 == 0x62) {
        local_84 = pcVar9->i;
        unaff_r14 = -1;
      }
      else if (iVar16 < 99) {
        if (iVar16 == 0x51) {
          if (pcVar9->i < 1) {
            (c->ai).direction = '\0';
          }
          else {
            iVar16 = qrand();
            (c->ai).direction = (char)(iVar16 / (0xffff / c->cmdcurr->i + 1));
          }
        }
        else if (iVar16 < 0x52) {
          if (iVar16 == 0x49) {
            (c->ai).force_action = *(short *)((int)&pcVar9->i + 2);
          }
          else if (iVar16 < 0x4a) {
            if (iVar16 == 0x45) {
              NewRumble(&player->rumble,pcVar9->i);
            }
            else if (iVar16 < 0x46) {
              if (iVar16 == 0x43) {
                (c->ai).duration = (c->ai).duration + c->cmdcurr->f;
              }
              else {
                if (0x43 < iVar16) {
                  if ((c->ai).rotspeed != 0) {
                    iVar16 = NuAtan2D((c->ai).newpos.x - (c->obj).pos.x,
                                      (c->ai).newpos.z - (c->obj).pos.z);
                    uVar34 = RotDiff((c->ai).old_yrot,(ushort)iVar16);
                    if ((((c->ai).rotflags & 0x200) == 0) || (-1 < (int)uVar34)) {
                      if ((((c->ai).rotflags & 0x400) != 0) && (0 < (int)uVar34)) {
                        uVar34 = uVar34 - 0x10000;
                      }
                    }
                    else {
                      uVar34 = uVar34 + 0x10000;
                    }
                    local_68 = (double)CONCAT44(0x43300000,
                                                ((int)uVar34 >> 0x1f ^ uVar34) -
                                                ((int)uVar34 >> 0x1f) ^ 0x80000000);
                    fVar10 = (float)(local_68 - 4503601774854144.0);
                    local_68 = (double)CONCAT44(0x43300000,
                                                (uint)(c->ai).rotspeed * 0x3c ^ 0x80000000);
                    fVar24 = (float)(local_68 - 4503601774854144.0);
                    goto LAB_80006c80;
                  }
                  goto LAB_80006c8c;
                }
                uVar34 = qrand();
                local_68 = (double)CONCAT44(0x43300000,uVar34 ^ 0x80000000);
                fVar10 = c->cmdcurr->f;
                fVar24 = (float)(local_68 - 4503601774854144.0) * 1.525902e-05;
LAB_80006b98:
                (c->ai).duration = fVar24 * fVar10;
              }
            }
            else if (iVar16 == 0x47) {
              ResetAnimPacket(&(c->obj).anim,c->cmdcurr->i);
            }
            else if (iVar16 < 0x48) {
              NewBuzz(&player->rumble,c->cmdcurr->i);
            }
            else {
              (c->obj).anim.newaction = *(short *)((int)&c->cmdcurr->i + 2);
            }
          }
          else if (iVar16 == 0x4d) {
            (c->ai).speed = pcVar9->f;
          }
          else if (iVar16 < 0x4e) {
            if (iVar16 == 0x4b) {
              uVar12 = *(ushort *)((int)&c->cmdcurr->i + 2);
              (c->ai).angle = uVar12;
              (c->ai).oldangle = uVar12;
            }
            else if (iVar16 < 0x4c) {
              (c->ai).idle_update = *(char *)((int)&c->cmdcurr->i + 3);
            }
            else {
              iVar16 = qrand();
              (c->ai).angle = (ushort)iVar16;
              (c->ai).oldangle = (ushort)iVar16;
            }
          }
          else if (iVar16 == 0x4f) {
            (c->ai).speed = (c->ai).speed + c->cmdcurr->f;
          }
          else if (iVar16 < 0x50) {
            uVar34 = qrand();
            local_68 = (double)CONCAT44(0x43300000,uVar34 ^ 0x80000000);
            (c->ai).speed = (float)(local_68 - 4503601774854144.0) * 1.525902e-05 * c->cmdcurr->f;
          }
          else {
            (c->ai).direction = *(char *)((int)&c->cmdcurr->i + 3);
          }
        }
        else if (iVar16 == 0x5a) {
          if (((c->obj).model)->anmdata[(c->obj).anim.newaction] != (nuAnimData_s *)0x0) {
            uVar34 = qrand();
            local_68 = (double)CONCAT44(0x43300000,uVar34 ^ 0x80000000);
            (c->obj).anim.anim_time =
                 (float)(local_68 - 4503601774854144.0) * 1.525902e-05 *
                 (((c->obj).model)->anmdata[(c->obj).anim.newaction]->time - 1.0) + 1.0;
          }
        }
        else if (iVar16 < 0x5b) {
          if (iVar16 == 0x55) {
            (c->ai).movflags = *(ushort *)((int)&pcVar9->i + 2);
            if ((pcVar9->i & 0x800U) != 0) {
              (c->ai).height = pcVar9->f;
            }
          }
          else if (iVar16 < 0x56) {
            if (iVar16 == 0x53) {
              (c->ai).radius = c->cmdcurr->f;
            }
            else if (iVar16 < 0x54) {
              (c->ai).distance = c->cmdcurr->f;
            }
            else {
              (c->ai).height = c->cmdcurr->f;
            }
          }
          else if (iVar16 == 0x57) {
            (c->ai).terflags = *(ushort *)((int)&c->cmdcurr->i + 2);
          }
          else if (iVar16 < 0x57) {
            pcVar9 = c->cmdcurr;
            uVar12 = *(ushort *)((int)&pcVar9->i + 2);
            (c->ai).rotflags = uVar12;
            if ((uVar12 & 1) != 0) {
              fVar10 = pcVar9->f;
              dVar40 = (double)fVar10;
              NuFabs(fVar10);
              (c->ai).spins = (float)dVar40;
            }
          }
          else {
            if (iVar16 != 0x59) goto LAB_80005a24;
            fVar10 = c->cmdcurr->f;
            (c->obj).anim.anim_time = fVar10;
            if (fVar10 < 1.0) {
              (c->obj).anim.anim_time = 1.0;
            }
          }
        }
        else if (iVar16 == 0x5e) {
          (c->ai).i0 = *(char *)((int)&pcVar9->i + 3);
        }
        else if (iVar16 < 0x5f) {
          if (iVar16 == 0x5c) {
            (c->obj).invisible = '\0';
          }
          else if (iVar16 < 0x5d) {
            (c->obj).invisible = '\x01';
          }
          else {
            piVar32 = (int *)c->cmdcurr->i;
            if (piVar32 != (int *)0x0) {
              iVar16 = (int)c->cmdcurr->f;
              local_68 = (double)(longlong)iVar16;
              *piVar32 = iVar16;
            }
          }
        }
        else {
          if (iVar16 == 0x60) {
            pcVar9 = c->cmdcurr;
            uVar34 = (c->obj).flags | pcVar9->i;
          }
          else {
            if (iVar16 < 0x61) {
              (c->ai).i1 = *(char *)((int)&c->cmdcurr->i + 3);
              goto LAB_80007394;
            }
            pcVar9 = c->cmdcurr;
            uVar34 = (c->obj).flags | pcVar9->i;
            (c->obj).flags = uVar34;
            uVar34 = uVar34 ^ pcVar9->i;
          }
          (c->obj).flags = uVar34;
          if ((pcVar9->i & 0x10000U) != 0) {
            GetTopBot(c);
          }
        }
      }
      else if (iVar16 == 0x73) {
        (c->ai).scale = pcVar9->f;
      }
      else if (iVar16 < 0x74) {
        if (iVar16 == 0x6b) {
          uVar19 = 0;
          pnVar26 = (nuvec_s *)&c->mtxLOCATOR[0]._30;
          local_c8.x = v000.x;
          local_c8.z = v000.z;
          local_c8.y = v000.y;
          uVar34 = 0;
          do {
            uVar36 = uVar34;
            if ((c->cmdcurr->i >> (uVar36 & 0x3f) & 1U) != 0) {
              uVar19 = uVar19 + 1;
              NuVecAdd(&local_c8,&local_c8,pnVar26);
            }
            pnVar26 = (nuvec_s *)&pnVar26[5].y;
            uVar34 = uVar36 + 1;
          } while ((int)(uVar36 + 1) < 0x10);
          if (uVar19 != 0) {
            if (1 < (int)uVar19) {
              local_68 = (double)CONCAT44(0x43300000,uVar19 ^ 0x80000000);
              NuVecScale(&local_c8,&local_c8,1.0 / (float)(local_68 - 4503601774854144.0));
            }
            if (unaff_r14 == -1) {
              pnVar26 = (nuvec_s *)0x0;
            }
            else {
              pnVar26 = pos + unaff_r14;
            }
            AddProjectile(&local_c8,pnVar26,c->momLOCATOR + uVar36 + 1,local_84,(c->obj).hdg,poVar 38
                         );
          }
        }
        else if (iVar16 < 0x6c) {
          if (iVar16 == 0x67) {
            GameSfx(pcVar9->i,(nuvec_s *)0x0);
          }
          else if (iVar16 < 0x68) {
            if (iVar16 == 0x65) {
              gamesfx_effect_volume = c->cmdcurr->i;
            }
            else if (iVar16 < 0x66) {
              if (iVar16 != 99) goto LAB_80005a24;
              unaff_r14 = c->cmdcurr->i;
            }
            else {
              gamesfx_pitch = c->cmdcurr->i;
            }
          }
          else if (iVar16 == 0x69) {
            gamesfx_effect_volume = (int)c->cmdcurr->f;
            local_68 = (double)(longlong)gamesfx_effect_volume;
            GameSfx(c->cmdcurr->i,pnVar15);
          }
          else if (iVar16 < 0x6a) {
            GameSfx(c->cmdcurr->i,pnVar15);
          }
          else {
            iVar16 = c->cmdcurr->i;
            if (iVar16 == -1) {
              local_c8.x = (c->obj).pos.x;
              local_c8.y = (c->obj).pos.y;
              local_c8.z = (c->obj).pos.z;
            }
            else {
              local_c8.x = pos[iVar16].x;
              local_c8.z = pos[iVar16].z;
              local_c8.y = pos[iVar16].y;
            }
            AddKaboom(0x10,&local_c8,c->cmdcurr->f);
            if ((c->obj).character == 0x80) {
              AddDeb3(&local_c8,9,1,(nuangvec_s *)0x0);
              AddDeb3(&local_c8,9,1,(nuangvec_s *)0x0);
              AddDeb3(&local_c8,9,1,(nuangvec_s *)0x0);
              AddGameDebris(0xd,&local_c8);
              AddGameDebris(0xe,&local_c8);
            }
          }
        }
        else if (iVar16 == 0x6f) {
          ResetHGobjAnim(&(c->ai).obj);
        }
        else if (iVar16 < 0x70) {
          if (iVar16 == 0x6d) {
            pcVar9 = c->cmdcurr;
            pnVar26 = pnVar15;
          }
          else {
            if (iVar16 < 0x6e) {
              JudderGameCamera(&GameCam,0.3,(nuvec_s *)0x0);
              goto LAB_80007394;
            }
            pcVar9 = c->cmdcurr;
            pnVar26 = &local_98;
          }
          AddGameDebris(pcVar9->i,pnVar26);
        }
        else if (iVar16 == 0x71) {
          iVar16 = GetDieAnim(&pcVar8->obj,c->cmdcurr->i);
          iVar16 = KillPlayer(&pcVar8->obj,iVar16);
          if (iVar16 == 0) {
            KillGameObject(poVar38,4);
          }
        }
        else if (iVar16 < 0x72) {
          StartHGobjAnim(&(c->ai).obj);
        }
        else {
          (pcVar8->obj).mom.x = NuTrigTable[(c->obj).hdg] * c->cmdcurr->f;
          (pcVar8->obj).mom.z =
               *(float *)((int)NuTrigTable + (((c->obj).hdg + 0x4000) * 4 & 0x3fffc)) *
               c->cmdcurr->f;
        }
      }
      else if (iVar16 < 0x7e) {
        if ((iVar16 < 0x7c) && (iVar16 < 0x7a)) {
          if (iVar16 < 0x78) {
            if (((c->obj).dead == '\0') && ((pcVar8->obj).dead == '\0')) {
              iVar16 = c->cmdcurr->cmd;
              uVar34 = iVar16 - 0x76;
              if (uVar34 < 2) {
                *(undefined *)((int)(c->ai).event_function + iVar16 + -0x62) = 0x14;
                (&(c->obj).anim.anim_time)[iVar16] = (float)c->cmdcurr->i;
              }
              else {
                cVar13 = *(char *)((int)&c->cmdcurr->i + 3);
                uVar34 = iVar16 - 0x74;
                *(char *)((int)(c->ai).event_function + iVar16 + -0x60) = cVar13;
                if ((-1 < cVar13) && ((cVar13 < '\b' || ((cVar13 < '\x0e' && ('\t' < cVar13)))))) {
                  *(float *)(&(c->obj).anim.newaction + iVar16 * 2) = c->cmdcurr->f;
                }
              }
              (c->ai).event_branch[uVar34] = -1;
              (c->ai).event_flags[uVar34] = '\0';
              (c->ai).event_occured[uVar34] = '\0';
            }
            else {
              pcVar31 = (c->ai).event + 1;
              do {
                *pcVar31 = -1;
                pcVar31 = pcVar31 + -1;
              } while ((int)(c->ai).event <= (int)pcVar31);
            }
          }
          else {
            iVar18 = c->cmdcurr->cmd;
            iVar16 = FindAILabel(c->cmdtable,c->cmdcurr->i);
            *(short *)((int)&c->ai + iVar18 * 2 + -0x7c) = (short)iVar16;
          }
        }
        else {
          *(undefined *)((int)(c->ai).event_function + iVar16 + -100) =
               *(undefined *)((int)&pcVar9->i + 3);
        }
      }
      else if (iVar16 == 0x86) {
        local_88 = (uint)*(ushort *)((int)&pcVar9->i + 2);
      }
      else if (iVar16 < 0x87) {
        if (iVar16 < 0x85) {
          iVar16 = pcVar9->cmd;
          iVar18 = 0;
          if (iVar16 == 0x7e) {
            iVar16 = pcVar9->i;
            if (iVar16 == 7) {
              if (DESTRUCTIBLECRATECOUNT <= plr_crates.count) goto LAB_800071a0;
            }
            else {
              if (7 < iVar16) {
                if (iVar16 == 0xb) {
                  bVar1 = (int)(c->ai).i1 == uVar33 - 1;
                }
                else if (iVar16 < 0xc) {
                  if (iVar16 == 9) {
                    local_68 = (double)(longlong)(int)pcVar9->f;
                    bVar1 = (int)(c->ai).i1 == (int)pcVar9->f;
                  }
                  else if (iVar16 < 10) {
                    iVar16 = (int)c->cmdcurr->f;
                    local_68 = (double)(longlong)iVar16;
                    bVar1 = (c->ai).i0 == iVar16;
                  }
                  else {
                    bVar1 = (int)(c->ai).i0 == uVar33 - 1;
                  }
                }
                else if (iVar16 == 0xd) {
                  bVar1 = (c->ai).i0 == '\0';
                }
                else {
                  if (0xc < iVar16) {
                    if ((iVar16 == 0xe) && (((c->obj).touch & local_88) != 0)) goto LAB_800071a0;
                    goto LAB_800071a4;
                  }
                  bVar1 = (c->ai).i0 == '\0';
                }
                goto LAB_8000719c;
              }
              if (iVar16 == 3) {
                fVar10 = NuVecXZDist(&(pcVar8->obj).pos,pnVar15,(nuvec_s *)0x0);
                if (c->cmdcurr->f < fVar10) goto LAB_800071a0;
              }
              else if (iVar16 < 4) {
                if (iVar16 == 1) {
                  fVar10 = NuVecDist(&(pcVar8->obj).pos,pnVar15,(nuvec_s *)0x0);
                  if (c->cmdcurr->f < fVar10) goto LAB_800071a0;
                }
                else if (iVar16 < 2) {
                  if ((iVar16 == 0) &&
                     (fVar10 = NuVecDist(&(pcVar8->obj).pos,pnVar15,(nuvec_s *)0x0),
                     fVar10 < c->cmdcurr->f)) goto LAB_800071a0;
                }
                else {
                  fVar10 = NuVecXZDist(&(pcVar8->obj).pos,pnVar15,(nuvec_s *)0x0);
                  if (fVar10 < c->cmdcurr->f) goto LAB_800071a0;
                }
              }
              else if (iVar16 == 5) {
                iVar18 = PlayerLateralOutOfRange
                                   ((c->ai).lateral,&(pcVar8->obj).pos,pos + (c->ai).i0,
                                    pos + (c->ai).i1,pcVar9->f);
              }
              else if (iVar16 < 6) {
                iVar18 = PlayerLateralInRange
                                   ((c->ai).lateral,&(pcVar8->obj).pos,pos + (c->ai).i0,
                                    pos + (c->ai).i1,c->cmdcurr->f);
              }
              else if ((pcVar8->obj).dead != '\0') goto LAB_800071a0;
            }
          }
          else {
            if (iVar16 == 0x7f) {
              bVar1 = (int)(c->obj).anim.newaction == pcVar9->i;
            }
            else if (iVar16 == 0x80) {
              bVar1 = (int)(c->ai).i0 == pcVar9->i;
            }
            else if (iVar16 == 0x81) {
              bVar1 = (uint)(c->ai).count == pcVar9->i;
            }
            else {
              if (iVar16 != 0x82) {
                if (iVar16 == 0x83) {
                  if ((int *)pcVar9->i != (int *)0x0) {
                    local_68 = (double)(longlong)(int)pcVar9->f;
                    bVar1 = *(int *)pcVar9->i == (int)pcVar9->f;
                    goto LAB_8000719c;
                  }
                }
                else if (iVar16 == 0x84) {
                  bVar1 = (int)(player->obj).character == pcVar9->i;
                  goto LAB_8000719c;
                }
                goto LAB_800071a4;
              }
              bVar1 = (uint)(c->ai).hits == pcVar9->i;
            }
LAB_8000719c:
            if (bVar1) {
LAB_800071a0:
              iVar18 = 1;
            }
          }
LAB_800071a4:
          pcVar9 = c->cmdcurr;
          c->cmdcurr = pcVar9 + 1;
          if (pcVar9[1].cmd == 0x8d) {
            if (iVar18 != 0) goto LAB_800071dc;
          }
          else if ((pcVar9[1].cmd == 0x8e) && (iVar18 == 0)) {
LAB_800071dc:
            iVar16 = FindAILabel(c->cmdtable,pcVar9[1].i);
            c->cmdcurr = c->cmdtable + iVar16;
          }
        }
        else {
          (*(code *)c->cmdcurr->i)(c,pos);
        }
      }
      else {
        if (iVar16 == 0x88) {
          pcVar9 = c->cmdcurr;
        }
        else {
          if (0x87 < iVar16) {
            if (iVar16 == 0x89) {
              sVar11 = (c->obj).character;
              ChangeCharacter(c,(int)(player->obj).character);
              ChangeCharacter(player,(int)sVar11);
            }
            else if (iVar16 != 0x8b) goto LAB_80005a24;
            goto LAB_80007394;
          }
          pcVar9 = c->cmdcurr;
          pcVar17 = c;
        }
        ChangeCharacter(pcVar17,pcVar9->i);
      }
LAB_80007394:
      if (bVar2) {
        fVar24 = (c->obj).pos.y;
        fVar10 = (c->obj).pos.z;
        (c->ai).oldpos.x = (c->obj).pos.x;
        (c->ai).oldpos.y = fVar24;
        (c->ai).oldpos.z = fVar10;
        pcVar9 = c->cmdcurr;
        (c->ai).time = 0.0;
        (c->obj).mom.z = 0.0;
        c->cmdcurr = pcVar9 + 1;
        (c->obj).mom.y = 0.0;
        (c->obj).mom.x = 0.0;
        fVar10 = (c->obj).pos.z;
        fVar24 = (c->obj).pos.y;
        (c->ai).oldpos.x = (c->obj).pos.x;
        (c->ai).oldpos.z = fVar10;
        (c->ai).oldpos.y = fVar24;
        (c->ai).old_yrot = (c->obj).hdg;
        (c->ai).oldangle = (c->ai).angle;
        goto LAB_80005684;
      }
      iVar16 = 0;
      while (c->cmdcurr->cmd == 0x8c) {
        iVar16 = iVar16 + 1;
        iVar18 = FindAILabel(c->cmdtable,c->cmdcurr->i);
        c->cmdcurr = c->cmdtable + iVar18;
      }
      if (iVar16 != 0) goto LAB_80005684;
      bVar2 = false;
      fVar24 = (c->ai).time + 0.01666667;
      iVar18 = -2;
      fVar10 = (c->ai).duration;
      (c->ai).time = fVar24;
      if ((0.0 < fVar10) && (fVar10 <= fVar24)) {
        (c->ai).time = fVar10;
        iVar18 = -1;
        psVar25 = (c->ai).event_branch;
        do {
          if (((c->ai).event[iVar16] != -1) && ((c->ai).event_occured[iVar16] != '\0')) {
            iVar18 = (int)*psVar25;
          }
          (c->ai).event[iVar16] = -1;
          psVar25 = psVar25 + 1;
          iVar16 = iVar16 + 1;
        } while (iVar16 < 2);
        bVar2 = true;
      }
      fVar10 = (c->ai).duration;
      dVar40 = 0.0;
      if (0.0 < fVar10) {
        dVar40 = (double)((c->ai).time / fVar10);
      }
      uVar12 = (c->ai).movflags;
      (c->ai).wobble_ang = (c->ai).wobble_ang + (c->ai).wobble_speed;
      if ((uVar12 & 0x40) == 0) {
        if ((uVar12 & 2) == 0) {
          if ((uVar12 & 1) == 0) {
            if ((uVar12 & 4) == 0) {
              if ((uVar12 & 8) == 0) {
                if ((uVar12 & 0x400) != 0) {
                  unaff_r15 = (c->obj).hdg;
                  (c->ai).angle = (c->ai).angle + (c->ai).rotspeed;
                  NuVecSub(&local_c8,pos + (c->ai).i1,pos + (c->ai).i0);
                  iVar16 = NuAtan2D(local_c8.x,local_c8.z);
                  uVar12 = (c->ai).angle;
                  (c->obj).hdg = (ushort)iVar16;
                  NuVecScale(&local_c8,&local_c8,NuTrigTable[uVar12] * 0.5 + 0.5);
                  NuVecAdd(pnVar15,pos + (c->ai).i0,&local_c8);
                  uVar12 = (c->ai).rotflags;
                  uVar28 = (c->obj).hdg + (c->ai).angle;
                  (c->obj).hdg = uVar28;
                  if ((uVar12 & 0x400) != 0) {
                    (c->obj).hdg = uVar28 + 0x8000;
                  }
                  iVar16 = RotDiff(unaff_r15,(c->obj).hdg);
                  (c->obj).dyrot = (short)iVar16;
                }
              }
              else {
                fVar10 = pos[(c->ai).i0].y;
                fVar24 = pos[(c->ai).i1].y;
                fVar30 = fVar10;
                if (fVar24 < fVar10) {
                  fVar30 = fVar24;
                  fVar24 = fVar10;
                }
                fVar10 = ((pcVar8->obj).bot + (pcVar8->obj).top) * (pcVar8->obj).SCALE * 0.5 +
                         (pcVar8->obj).pos.y;
                if ((fVar30 <= fVar10) && (fVar30 = fVar10, fVar24 < fVar10)) {
                  fVar30 = fVar24;
                }
                fVar30 = fVar30 - (((c->obj).bot + (c->obj).top) * (c->obj).SCALE * 0.5 +
                                  (c->obj).pos.y);
                dVar42 = (double)fVar30;
                dVar41 = (double)fVar30;
                NuFabs(fVar30);
                if (dVar41 <= 0.2000000029802322) {
                  sVar11 = 0x22;
                }
                else {
                  sVar11 = 0x16;
                  if (0.0 <= dVar42) {
                    sVar11 = 0x58;
                  }
                }
                (c->obj).anim.newaction = sVar11;
                iVar16 = (int)(c->obj).anim.newaction;
                if ((iVar16 != 0x22) && (((c->obj).model)->anmdata[iVar16] == (nuAnimData_s *)0x0) )
                {
                  (c->obj).anim.newaction = 0x22;
                }
                (c->obj).pos.y = (float)(dVar42 * 0.01999999955296516 + (double)(c->obj).pos.y);
              }
            }
            else {
              cVar3 = (c->ai).i1;
              cVar13 = (c->ai).i0;
              fVar10 = pos[cVar3].x - pos[cVar13].x;
              fVar24 = pos[cVar3].z - pos[cVar13].z;
              fVar10 = NuFsqrt(fVar10 * fVar10 + fVar24 * fVar24);
              dVar39 = (double)(((c->ai).speed * 0.01666667) / fVar10);
              fVar10 = RatioAlongLine(&(pcVar8->obj).pos,pos + (c->ai).i0,pos + (c->ai).i1);
              dVar41 = (double)fVar10;
              fVar10 = (float)((double)(c->ai).lateral - dVar41);
              dVar42 = (double)fVar10;
              NuFabs(fVar10);
              if (dVar39 < dVar42) {
                dVar42 = (double)(c->ai).lateral;
                if (dVar42 <= dVar41) {
                  if (dVar42 < dVar41) {
                    dVar42 = dVar42 + dVar39;
                    sVar11 = 0x32;
                    goto LAB_8000791c;
                  }
                }
                else {
                  dVar42 = dVar42 - dVar39;
                  sVar11 = 0x39;
LAB_8000791c:
                  (c->obj).anim.newaction = sVar11;
                  (c->ai).lateral = (float)dVar42;
                }
                cVar3 = (c->ai).i0;
                cVar13 = (c->ai).i1;
                fVar10 = (c->ai).lateral;
                (c->obj).pos.x = (pos[cVar13].x - pos[cVar3].x) * fVar10 + pos[cVar3].x;
                (c->obj).pos.y = (pos[cVar13].y - pos[cVar3].y) * fVar10 + pos[cVar3].y;
                (c->obj).pos.z = (pos[cVar13].z - pos[cVar3].z) * fVar10 + pos[cVar3].z;
              }
            }
          }
          else if ((uVar12 & 0x30) == 0) {
            dVar41 = (double)(c->ai).oldpos.x;
            dVar39 = (double)(c->ai).oldpos.y;
            dVar42 = (double)(c->ai).oldpos.z;
            fVar24 = (c->ai).newpos.x;
            fVar30 = (c->ai).newpos.z;
            fVar10 = (float)((double)(float)((double)(c->ai).newpos.y - dVar39) * dVar40 + dVar39) ;
            (c->obj).pos.y = fVar10;
            (c->obj).pos.x = (float)((double)(float)((double)fVar24 - dVar41) * dVar40 + dVar41);
            (c->obj).pos.z = (float)((double)(float)((double)fVar30 - dVar42) * dVar40 + dVar42);
            if ((uVar12 & 0x800) != 0) {
              local_68 = (double)(longlong)(int)(dVar40 * 32768.0);
              (c->obj).pos.y =
                   NuTrigTable[(int)(dVar40 * 32768.0) & 0xffff] * (c->ai).height + fVar10;
            }
          }
          else {
            if ((uVar12 & 0x10) == 0) {
              uVar34 = (uint)(dVar40 * 16384.0 + 16384.0);
              fVar10 = NuTrigTable[uVar34 & 0xffff];
            }
            else {
              uVar34 = (uint)(dVar40 * 16384.0);
              fVar10 = 1.0 - NuTrigTable[uVar34 & 0xffff];
            }
            local_68 = (double)(longlong)(int)uVar34;
            fVar24 = (c->ai).newpos.x;
            fVar30 = (c->ai).newpos.y;
            fVar23 = (c->ai).newpos.z;
            fVar29 = (c->ai).oldpos.x;
            fVar6 = (c->ai).oldpos.y;
            (c->obj).pos.z = ((c->ai).oldpos.z - fVar23) * fVar10 + fVar23;
            (c->obj).pos.x = (fVar29 - fVar24) * fVar10 + fVar24;
            (c->obj).pos.y = (fVar6 - fVar30) * fVar10 + fVar30;
          }
        }
        else {
          if ((c->ai).direction == '\0') {
            uVar12 = (c->ai).angle + (c->ai).rotspeed;
          }
          else {
            uVar12 = (c->ai).angle - (c->ai).rotspeed;
          }
          (c->ai).angle = uVar12;
          uVar34 = (uint)(c->ai).angle;
          fVar10 = (c->ai).radius;
          fVar24 = (c->ai).newpos.y;
          fVar30 = (c->ai).newpos.z;
          (c->obj).pos.x = NuTrigTable[uVar34] * fVar10 + (c->ai).newpos.x;
          (c->obj).pos.y = fVar24;
          (c->obj).pos.z =
               *(float *)((int)NuTrigTable + ((uVar34 + 0x4000) * 4 & 0x3fffc)) * fVar10 + fVar30;
        }
      }
      else {
        iVar16 = (int)(c->ai).i0;
        uVar34 = iVar16 + 1;
        if (uVar34 == uVar33) {
          uVar34 = 0;
        }
        NuVecSub(&local_c8,pos + uVar34,pos + iVar16);
        fVar10 = (c->obj).pos.x;
        fVar24 = (c->obj).pos.z;
        fVar30 = -local_c8.x;
        local_b8.x = local_c8.x;
        local_c8.x = local_c8.z + fVar10;
        if (((player->obj).pos.x - fVar10) * ((fVar30 + fVar24) - fVar24) +
            ((player->obj).pos.z - fVar24) * (fVar10 - local_c8.x) < 0.0) {
          bVar1 = true;
          uVar34 = (uint)(c->ai).i0;
        }
        else {
          uVar34 = (int)(c->ai).i0 + 1;
          if (uVar34 == uVar33) {
            uVar34 = 0;
          }
          bVar1 = false;
        }
        fVar23 = pos[uVar34].x - (c->obj).pos.x;
        fVar10 = pos[uVar34].z - (c->obj).pos.z;
        dVar41 = (double)(fVar23 * fVar23 + fVar10 * fVar10);
        local_c8.z = fVar30 + fVar24;
        uVar19 = NuAtan2D(fVar23,fVar10);
        fVar10 = (c->ai).speed * 0.01666667;
        if ((double)(fVar10 * fVar10) < dVar41) {
          fVar24 = (c->obj).pos.z;
          (c->obj).pos.x = NuTrigTable[uVar19 & 0xffff] * fVar10 + (c->obj).pos.x;
          (c->obj).pos.z =
               *(float *)((int)NuTrigTable + (((uVar19 & 0xffff) + 0x4000) * 4 & 0x3fffc)) * fVar1 0
               + fVar24;
        }
        else {
          fVar24 = pos[uVar34].z;
          fVar10 = pos[uVar34].y;
          (c->obj).pos.x = pos[uVar34].x;
          (c->obj).pos.y = fVar10;
          (c->obj).pos.z = fVar24;
          if (bVar1) {
            bVar14 = (c->ai).i0 - 1;
            (c->ai).i0 = bVar14;
            if ((bVar14 & 0x80) != 0) {
              (c->ai).i0 = bVar14 + cVar3;
            }
          }
          else {
            (c->ai).i0 = (char)uVar34;
          }
        }
      }
      if (((c->ai).movflags & 0x100) != 0) {
        fVar24 = (c->obj).pos.y;
        fVar10 = (c->obj).pos.z;
        (c->ai).wobblepos.x = (c->obj).pos.x;
        (c->ai).wobblepos.z = fVar10;
        (c->ai).wobblepos.y = fVar24;
      }
      if (((c->ai).movflags & 0x200) != 0) {
        (c->obj).pos.y = NuTrigTable[(c->ai).wobble_ang] * (c->ai).wobble + (c->ai).wobblepos.y;
      }
      fVar30 = (c->obj).pos.z - (c->obj).oldpos.z;
      fVar10 = (c->obj).oldpos.y;
      fVar24 = (c->obj).pos.y;
      fVar23 = (c->obj).pos.x - (c->obj).oldpos.x;
      (c->obj).mom.x = fVar23;
      (c->obj).mom.z = fVar30;
      (c->obj).mom.y = fVar24 - fVar10;
      fVar23 = NuFsqrt(fVar23 * fVar23 + fVar30 * fVar30);
      fVar10 = (c->obj).mom.y;
      fVar24 = (c->obj).mom.x;
      fVar30 = (c->obj).mom.z;
      (c->obj).xz_distance = fVar23;
      fVar10 = NuFsqrt(fVar30 * fVar30 + fVar24 * fVar24 + fVar10 * fVar10);
      uVar12 = (c->ai).movflags;
      (c->obj).xyz_distance = fVar10;
      if ((uVar12 & 0x400) == 0) {
        unaff_r15 = (c->obj).hdg;
      }
      if ((uVar12 & 2) != 0) {
        if ((c->ai).direction == '\0') {
          (c->obj).hdg = (c->ai).angle + 0x4000;
        }
        else {
          (c->obj).hdg = (c->ai).angle + 0xc000;
        }
        goto LAB_800080cc;
      }
      uVar12 = (c->ai).rotflags;
      if ((uVar12 & 1) == 0) {
        if ((uVar12 & 2) != 0) {
          iVar16 = NuAtan2D((c->ai).newpos.x - (c->ai).oldpos.x,(c->ai).newpos.z - (c->ai).oldpos.z )
          ;
          uVar34 = RotDiff((c->ai).old_yrot,(short)iVar16 + (c->ai).rotadjust);
          uVar12 = (c->ai).rotflags;
          if (((uVar12 & 0x200) == 0) || (-1 < (int)uVar34)) {
            uVar12 = (c->ai).rotflags;
            if (((uVar12 & 0x400) != 0) && (0 < (int)uVar34)) {
              uVar34 = uVar34 - 0x10000;
            }
          }
          else {
            uVar34 = uVar34 + 0x10000;
          }
          local_68 = (double)CONCAT44(0x43300000,uVar34 ^ 0x80000000);
          dVar41 = (double)(float)(local_68 - 4503601774854144.0);
          if ((uVar12 & 0x80) == 0) {
            if ((uVar12 & 0x100) == 0) {
              uVar12 = (c->ai).old_yrot;
              local_68 = (double)(longlong)(int)(dVar41 * dVar40);
              uVar28 = (ushort)(int)(dVar41 * dVar40);
            }
            else if ((uVar12 & 0x400) == 0) {
              uVar12 = (c->ai).old_yrot;
              iVar16 = (int)(dVar41 - (double)(float)(dVar41 * (double)NuTrigTable
                                                                       [(int)(dVar40 * 16384.0 +
                                                                             16384.0) & 0xffff]));
              local_68 = (double)(longlong)iVar16;
              uVar28 = (ushort)iVar16;
            }
            else {
              uVar12 = (c->ai).old_yrot;
              iVar16 = (int)(-dVar41 +
                            (double)(float)(dVar41 * (double)NuTrigTable
                                                             [(int)(dVar40 * 16384.0 + 16384.0) &
                                                              0xffff]));
              local_68 = (double)(longlong)iVar16;
              uVar28 = (ushort)iVar16;
            }
          }
          else {
            dVar42 = (double)(float)(dVar41 * (double)(1.0 - NuTrigTable
                                                             [(int)(dVar40 * 16384.0) & 0xffff]));
            if ((uVar12 & 0x400) == 0) {
              uVar12 = (c->ai).old_yrot;
              iVar16 = (int)(dVar41 - dVar42);
              local_68 = (double)(longlong)iVar16;
              uVar28 = (ushort)iVar16;
            }
            else {
              uVar12 = (c->ai).old_yrot;
              iVar16 = (int)(-dVar41 + dVar42);
              local_68 = (double)(longlong)iVar16;
              uVar28 = (ushort)iVar16;
            }
          }
          goto LAB_800080c4;
        }
        if ((uVar12 & 0x180c) == 0) {
          if ((uVar12 & 0x20) == 0) {
            if ((uVar12 & 0x40) != 0) {
              uVar12 = (c->obj).hdg;
              uVar28 = (c->ai).rotspeed;
              goto LAB_800080c4;
            }
            goto LAB_800080cc;
          }
          if ((c->obj).xz_distance <= 0.0) goto LAB_800080cc;
          iVar16 = NuAtan2D((c->obj).mom.x,(c->obj).mom.z);
          uVar28 = (short)iVar16 + (c->ai).rotadjust;
          uVar34 = RotDiff((c->obj).hdg,uVar28);
          uVar12 = (c->ai).rotspeed;
          if ((int)(((int)uVar34 >> 0x1f ^ uVar34) - ((int)uVar34 >> 0x1f)) <= (int)(uint)uVar12)  {
            (c->obj).hdg = uVar28;
            goto LAB_800080cc;
          }
          if ((int)uVar34 < 1) {
            if (-1 < (int)uVar34) goto LAB_800080cc;
            uVar12 = (c->obj).hdg - uVar12;
          }
          else {
            uVar12 = (c->obj).hdg + uVar12;
          }
          goto LAB_800080c8;
        }
        if ((uVar12 & 0x800) == 0) {
          if ((uVar12 & 0x1000) == 0) {
            if ((uVar12 & 8) == 0) {
              pnVar26 = &(pcVar8->obj).pos;
            }
            else {
              pnVar26 = &GameCam.pos;
            }
          }
          else {
            pnVar26 = &ai_lookpos;
          }
        }
        else {
          fVar10 = FindNearestCreature(pnVar15,(int)(c->ai).look_creature,&local_c8);
          if (1000000.0 <= fVar10) goto LAB_800080cc;
          pnVar26 = &local_c8;
        }
        iVar16 = NuAtan2D(pnVar26->x - (c->obj).pos.x,pnVar26->z - (c->obj).pos.z);
        uVar12 = (short)iVar16 + (c->ai).rotadjust;
        if (((c->ai).rotflags & 0x10) == 0) {
          uVar12 = TurnRot((c->obj).hdg,uVar12,(uint)(c->ai).rotspeed);
          (c->obj).hdg = uVar12;
        }
        else {
          uVar12 = SeekRot((c->obj).hdg,uVar12,3);
          (c->obj).hdg = uVar12;
        }
      }
      else {
        dVar41 = (double)((c->ai).spins * 65536.0);
        if ((uVar12 & 0x80) == 0) {
          if ((uVar12 & 0x100) == 0) {
            local_68 = (double)(longlong)(int)(dVar41 * dVar40);
            sVar11 = (short)(int)(dVar41 * dVar40);
            if ((uVar12 & 0x400) == 0) {
              uVar12 = (c->ai).old_yrot + sVar11;
            }
            else {
              uVar12 = (c->ai).old_yrot - sVar11;
            }
            goto LAB_800080c8;
          }
          if ((uVar12 & 0x400) == 0) {
            uVar12 = (c->ai).old_yrot;
            iVar16 = (int)(dVar41 - (double)(float)(dVar41 * (double)NuTrigTable
                                                                     [(int)(dVar40 * 16384.0 +
                                                                           16384.0) & 0xffff]));
            local_68 = (double)(longlong)iVar16;
            uVar28 = (ushort)iVar16;
          }
          else {
            uVar12 = (c->ai).old_yrot;
            iVar16 = (int)(-dVar41 +
                          (double)(float)(dVar41 * (double)NuTrigTable
                                                           [(int)(dVar40 * 16384.0 + 16384.0) &
                                                            0xffff]));
            local_68 = (double)(longlong)iVar16;
            uVar28 = (ushort)iVar16;
          }
        }
        else {
          dVar42 = (double)(float)(dVar41 * (double)(1.0 - NuTrigTable
                                                           [(int)(dVar40 * 16384.0) & 0xffff]));
          if ((uVar12 & 0x400) == 0) {
            uVar12 = (c->ai).old_yrot;
            iVar16 = (int)(dVar41 - dVar42);
            local_68 = (double)(longlong)iVar16;
            uVar28 = (ushort)iVar16;
          }
          else {
            uVar12 = (c->ai).old_yrot;
            iVar16 = (int)(-dVar41 + dVar42);
            local_68 = (double)(longlong)iVar16;
            uVar28 = (ushort)iVar16;
          }
        }
LAB_800080c4:
        uVar12 = uVar12 + uVar28;
LAB_800080c8:
        (c->obj).hdg = uVar12;
      }
LAB_800080cc:
      if (((c->ai).movflags & 0x400) == 0) {
        iVar16 = RotDiff(unaff_r15,(c->obj).hdg);
        (c->obj).dyrot = (short)iVar16;
      }
      uVar12 = (c->ai).terflags;
      (c->obj).ground = '\0';
      if ((uVar12 & 1) != 0) {
        if ((uVar12 & 2) == 0) {
          if ((uVar12 & 4) == 0) {
            if ((uVar12 & 8) == 0) {
              if ((uVar12 & 0x10) == 0) {
                if ((uVar12 & 0x60) == 0) {
                  fVar10 = NewShadowMaskPlat(pnVar15,0.0,-1);
                  (c->obj).shadow = fVar10;
                  GetSurfaceInfo(poVar38,1,fVar10);
                }
                else {
                  fVar10 = NewShadowMaskPlat(pnVar15,0.0,-1);
                  (c->obj).shadow = fVar10;
                  GetSurfaceInfo(poVar38,1,fVar10);
                  if ((c->obj).shadow != 2000000.0) {
                    if (((c->ai).terflags & 0x40) != 0) {
                      dVar40 = (double)(float)(1.0 - dVar40);
                    }
                    fVar10 = (c->ai).height;
                    (c->obj).ground = '\0';
                    (c->obj).pos.y =
                         (float)((double)fVar10 * dVar40 +
                                (double)((c->obj).shadow - (c->obj).bot * (c->obj).SCALE));
                  }
                }
              }
              else {
                fVar10 = NewShadowMaskPlat(pnVar15,0.0,-1);
                (c->obj).shadow = fVar10;
                GetSurfaceInfo(poVar38,1,fVar10);
                fVar10 = (c->obj).shadow;
                if (fVar10 != 2000000.0) {
                  fVar24 = (c->ai).height;
                  (c->obj).ground = (byte)uVar12 & 8;
                  (c->obj).pos.y = (fVar10 - (c->obj).bot * (c->obj).SCALE) + fVar24;
                }
              }
            }
            else {
              local_c8.x = (c->obj).pos.x;
              local_c8.z = (c->obj).pos.z;
              local_c8.y = (c->obj).pos.y - 1.0;
              fVar10 = NewShadowMaskPlat(&local_c8,0.0,-1);
              (c->obj).shadow = fVar10;
              GetSurfaceInfo(poVar38,1,fVar10);
              if ((c->obj).roof_type != -1) {
                (c->obj).ground = (byte)uVar12 & 4;
                (c->obj).pos.y = (c->obj).roof_y - (c->obj).top * (c->obj).SCALE;
              }
            }
          }
          else {
            fVar10 = NewShadowMaskPlat(pnVar15,0.0,-1);
            (c->obj).shadow = fVar10;
            GetSurfaceInfo(poVar38,1,fVar10);
            fVar10 = (c->obj).shadow;
            if ((fVar10 != 2000000.0) &&
               (fVar10 = fVar10 - (c->obj).bot * (c->obj).SCALE, (c->obj).pos.y <= fVar10)) {
              (c->obj).pos.y = fVar10;
              (c->obj).ground = '\x03';
            }
          }
        }
        else {
          fVar10 = NewShadowMaskPlat(pnVar15,0.0,-1);
          (c->obj).shadow = fVar10;
          GetSurfaceInfo(poVar38,1,fVar10);
          fVar10 = (c->obj).shadow;
          if (fVar10 != 2000000.0) {
            (c->obj).ground = '\x03';
            (c->obj).pos.y = fVar10 - (c->obj).bot * (c->obj).SCALE;
          }
        }
        ObjectRotation(poVar38,2,0);
      }
      (c->obj).mom.y = (c->obj).pos.y - (c->obj).oldpos.y;
      if (!bVar2) {
        iVar16 = -1;
        dVar40 = 0.5;
        iVar37 = 0;
        pcVar31 = (c->ai).event;
        pfVar35 = (c->ai).event_distance;
LAB_800083c8:
        cVar3 = pcVar31[iVar37];
        if (cVar3 == -1) goto LAB_800089c0;
        if (cVar3 == '\t') {
          bVar2 = (pcVar8->obj).dead == '\0';
LAB_80008964:
          if (!bVar2) goto LAB_80008968;
        }
        else if (cVar3 < '\n') {
          if (cVar3 == '\x04') {
            if (((pcVar8->obj).dead == '\0') &&
               (cVar3 = (c->ai).event_index[iVar37], fVar24 = (pcVar8->obj).pos.x - pos[cVar3].x,
               fVar10 = (pcVar8->obj).pos.z - pos[cVar3].z,
               fVar24 * fVar24 + fVar10 * fVar10 < *pfVar35 * *pfVar35)) goto LAB_80008968;
          }
          else if (cVar3 < '\x05') {
            if (cVar3 == '\x01') {
              if ((pcVar8->obj).dead == '\0') {
                local_b8.y = (c->obj).shadow;
                local_b8.x = (c->obj).pos.x;
                if (local_b8.y == 2000000.0) {
                  local_b8.y = (c->obj).pos.y;
                }
                local_b8.z = (c->obj).pos.z;
                local_a8.x = (pcVar8->obj).pos.x;
                local_a8.y = (pcVar8->obj).objbot;
                local_a8.z = (pcVar8->obj).pos.z;
                NuVecSub(&local_c8,&local_a8,&local_b8);
                if (local_c8.z * local_c8.z + local_c8.x * local_c8.x + local_c8.y * local_c8.y <
                    *pfVar35 * *pfVar35) goto LAB_80008968;
              }
            }
            else if (cVar3 < '\x02') {
              if ((cVar3 == '\0') && ((pcVar8->obj).dead == '\0')) {
                local_b8.x = (c->obj).pos.x;
                local_b8.z = (c->obj).pos.z;
                local_b8.y = (float)((double)((c->obj).objtop + (c->obj).objbot) * dVar40);
                local_a8.x = (pcVar8->obj).pos.x;
                local_a8.y = (float)((double)((pcVar8->obj).objtop + (pcVar8->obj).objbot) * dVar4 0)
                ;
                local_a8.z = (pcVar8->obj).pos.z;
                NuVecSub(&local_c8,&local_a8,&local_b8);
                if (local_c8.z * local_c8.z + local_c8.x * local_c8.x + local_c8.y * local_c8.y <
                    *pfVar35 * *pfVar35) goto LAB_80008968;
              }
            }
            else if (cVar3 == '\x02') {
              if ((pcVar8->obj).dead == '\0') {
                iVar20 = PlayerLateralInRange
                                   ((c->ai).lateral,&(pcVar8->obj).pos,pos + (c->ai).i0,
                                    pos + (c->ai).i1,*pfVar35);
                goto LAB_80008960;
              }
            }
            else if ((cVar3 == '\x03') && ((pcVar8->obj).dead == '\0')) {
              iVar20 = PlayerLateralOutOfRange
                                 ((c->ai).lateral,&(pcVar8->obj).pos,pos + (c->ai).i0,
                                  pos + (c->ai).i1,*pfVar35);
LAB_80008960:
              bVar2 = iVar20 == 0;
              goto LAB_80008964;
            }
          }
          else if (cVar3 == '\x06') {
            if (((pcVar8->obj).dead == '\0') &&
               (cVar3 = (c->ai).event_index[iVar37], fVar24 = (pcVar8->obj).pos.x - pos[cVar3].x,
               fVar10 = (pcVar8->obj).pos.z - pos[cVar3].z,
               *pfVar35 * *pfVar35 < fVar24 * fVar24 + fVar10 * fVar10)) goto LAB_80008968;
          }
          else if (cVar3 < '\x06') {
            if ((pcVar8->obj).dead == '\0') {
              local_b8.x = (pcVar8->obj).pos.x;
              cVar3 = (c->ai).event_index[iVar37];
              local_a8.x = pos[cVar3].x;
              local_b8.y = (float)((double)((pcVar8->obj).objtop + (pcVar8->obj).objbot) * dVar40) ;
              local_b8.z = (pcVar8->obj).pos.z;
              local_a8.z = pos[cVar3].z;
              local_a8.y = pos[cVar3].y;
              NuVecSub(&local_c8,&local_a8,&local_b8);
              if (local_c8.z * local_c8.z + local_c8.x * local_c8.x + local_c8.y * local_c8.y <
                  *pfVar35 * *pfVar35) goto LAB_80008968;
            }
          }
          else if (cVar3 == '\a') {
            if ((pcVar8->obj).dead == '\0') {
              local_b8.x = (pcVar8->obj).pos.x;
              cVar3 = (c->ai).event_index[iVar37];
              local_a8.x = pos[cVar3].x;
              local_b8.y = (float)((double)((pcVar8->obj).objtop + (pcVar8->obj).objbot) * dVar40) ;
              local_b8.z = (pcVar8->obj).pos.z;
              local_a8.z = pos[cVar3].z;
              local_a8.y = pos[cVar3].y;
              NuVecSub(&local_c8,&local_a8,&local_b8);
              if (*pfVar35 * *pfVar35 <
                  local_c8.z * local_c8.z + local_c8.x * local_c8.x + local_c8.y * local_c8.y)
              goto LAB_80008968;
            }
          }
          else if (((cVar3 == '\b') && ((pcVar8->obj).dead == '\0')) &&
                  ((obj_s *)(c->obj).contact == &pcVar8->obj)) {
            uVar5 = (c->obj).contact_type;
            goto joined_r0x8000893c;
          }
        }
        else if (cVar3 == '\x0e') {
          if (((pcVar8->obj).dead == '\0') && (DESTRUCTIBLECRATECOUNT <= plr_crates.count))
          goto LAB_80008968;
        }
        else if (cVar3 < '\x0f') {
          if (cVar3 == '\v') {
            if (((pcVar8->obj).dead == '\0') &&
               (fVar10 = NuVecDist(&(pcVar8->obj).pos,pnVar15,(nuvec_s *)0x0), *pfVar35 < fVar10))
            goto LAB_80008968;
          }
          else if (cVar3 < '\v') {
            if (((pcVar8->obj).dead == '\0') &&
               (fVar10 = NuVecDist(&(pcVar8->obj).pos,pnVar15,(nuvec_s *)0x0), fVar10 < *pfVar35))
            goto LAB_80008968;
          }
          else if (cVar3 == '\f') {
            if (((pcVar8->obj).dead == '\0') &&
               (fVar10 = NuVecXZDist(&(pcVar8->obj).pos,pnVar15,(nuvec_s *)0x0), fVar10 < *pfVar35 ))
            goto LAB_80008968;
          }
          else if ((cVar3 == '\r') &&
                  (((pcVar8->obj).dead == '\0' &&
                   (fVar10 = NuVecXZDist(&(pcVar8->obj).pos,pnVar15,(nuvec_s *)0x0),
                   *pfVar35 < fVar10)))) {
LAB_80008968:
            iVar16 = iVar37;
          }
        }
        else if (cVar3 == '\x12') {
          if ((pcVar8->obj).dead == '\0') {
            bVar2 = (c->obj).kill_contact == '\0';
            goto LAB_80008964;
          }
        }
        else if (cVar3 < '\x13') {
          if (((cVar3 == '\x10') && ((pcVar8->obj).dead == '\0')) &&
             ((obj_s *)(c->obj).contact == &pcVar8->obj)) goto LAB_80008968;
        }
        else if (cVar3 == '\x13') {
          uVar5 = c->hit_type;
joined_r0x8000893c:
          if (uVar5 == '\x02') goto LAB_80008968;
        }
        else if (cVar3 == '\x14') {
          iVar20 = (*(code *)(c->ai).event_function[iVar37])(c,pos);
          goto LAB_80008960;
        }
        if (iVar16 == -1) goto LAB_800089c0;
        if (((c->ai).event_flags[iVar37] & 1) == 0) {
          iVar18 = (int)(c->ai).event_branch[iVar37];
        }
        else {
          (c->ai).event_occured[iVar37] = '\x01';
        }
        pcVar27 = (c->ai).event + 1;
        do {
          *pcVar27 = -1;
          pcVar27 = pcVar27 + -1;
        } while ((int)pcVar31 <= (int)pcVar27);
      }
LAB_800089d0:
      if (iVar18 == -2) {
        if (c->cmdcurr->cmd - 0x8fU < 2) {
          if ((c->obj).dead != '\0') goto LAB_80008d0c;
          iVar16 = c->cmdcurr->i;
          if ((iVar16 == 0) && (iVar16 = 1, ((c->obj).touch & 6) != 0)) {
            iVar16 = 4;
          }
          if (iVar16 == 1) {
            uVar33 = NuAtan2D((c->obj).pos.x - (pcVar8->obj).pos.x,
                              (c->obj).pos.z - (pcVar8->obj).pos.z);
            FlyGameObject(poVar38,uVar33 & 0xffff);
          }
          KillGameObject(poVar38,iVar16);
        }
        if ((c->obj).dead != '\0') goto LAB_80008d0c;
        if ((((pcVar8->obj).dead != '\0') || ((c->ai).attack_locator_bits == 0)) ||
           ((c->obj).draw_frame == 0)) goto LAB_80008d00;
        dVar40 = 0.1000000089406967;
        uVar33 = 0;
        iVar16 = 0;
        iVar37 = 0;
        iVar18 = 0;
        goto LAB_80008b50;
      }
      fVar30 = (c->obj).pos.x;
      (c->ai).time = 0.0;
      fVar10 = (c->obj).pos.z;
      fVar24 = (c->obj).pos.y;
      (c->ai).oldpos.x = fVar30;
      (c->ai).oldpos.z = fVar10;
      (c->ai).oldpos.y = fVar24;
      (c->ai).old_yrot = (c->obj).hdg;
      if (iVar18 == -1) {
        c->cmdcurr = c->cmdcurr + 1;
        goto LAB_80005684;
      }
      pcVar9 = c->cmdtable + iVar18;
    } while( true );
  }
  if (cVar3 == '\x01') {
    pnVar15 = &(c->obj).pos;
    poVar38 = &c->obj;
    NuVecAdd(pnVar15,pnVar15,&(c->obj).mom);
    iVar16 = HitCreatures(poVar38,1,4);
    if (((iVar16 == 0) && (iVar16 = HitCrates(poVar38,1), iVar16 == 0)) &&
       (iVar16 = HitItems(poVar38), iVar16 == 0)) {
      HitWumpa(poVar38,0);
    }
    else {
      (c->obj).die_time = (c->obj).die_duration;
    }
  }
  goto LAB_80008d00;
LAB_800089c0:
  iVar37 = iVar37 + 1;
  pfVar35 = pfVar35 + 1;
  if (1 < iVar37) goto LAB_800089d0;
  goto LAB_800083c8;
LAB_80008b50:
  do {
    if ((((int)(uint)(c->ai).attack_locator_bits >> (uVar33 & 0x3f) & 1U) != 0) &&
       (*(int *)((int)((c->obj).model)->pLOCATOR + iVar18) != 0)) {
      local_a8.y = *(float *)((int)&c->mtxLOCATOR[0]._31 + iVar37);
      fVar10 = (c->ai).attack_locator_radius * (c->obj).SCALE;
      if (((pcVar8->obj).objbot <= local_a8.y + fVar10) &&
         (local_a8.y - fVar10 <= (pcVar8->obj).objtop)) {
        local_a8.x = *(float *)((int)&c->mtxLOCATOR[0]._30 + iVar37);
        local_a8.z = *(float *)((int)&c->mtxLOCATOR[0]._32 + iVar37);
        fVar23 = local_a8.z - (pcVar8->obj).pos.z;
        fVar30 = local_a8.x - (pcVar8->obj).pos.x;
        fVar24 = (pcVar8->obj).RADIUS + fVar10;
        if (fVar30 * fVar30 + fVar23 * fVar23 <= fVar24 * fVar24) {
          local_b8.y = local_a8.y - *(float *)((int)&c->momLOCATOR[0].y + iVar16);
          if ((c->obj).attack != 0) {
            if ((pcVar8->spin == '\0') ||
               ((int)pcVar8->spin_frames - (int)pcVar8->OnFootMoveInfo->SPINRESETFRAMES <=
                (int)pcVar8->spin_frame)) {
              iVar16 = (int)(c->ai).locator_kill;
              if (iVar16 == -1) {
                iVar16 = GetDieAnim(&pcVar8->obj,-1);
              }
              iVar16 = KillPlayer(&pcVar8->obj,iVar16);
              if (iVar16 == 0) {
                KillGameObject(poVar38,4);
              }
            }
            else if ((local_b8.y + fVar10) - YTOL <= (pcVar8->obj).oldobjbot) {
              bVar14 = (pcVar8->obj).boing;
              (pcVar8->obj).mom.y = 0.0;
              (pcVar8->obj).boing = bVar14 | 1;
              NewRumble(&pcVar8->rumble,0x7f);
              NewBuzz(&pcVar8->rumble,0xc);
            }
            else if ((pcVar8->obj).oldobjtop <= (local_b8.y - fVar10) + YTOL) {
              (pcVar8->obj).mom.y = 0.0;
            }
            else {
              uVar33 = NuAtan2D(fVar30,fVar23);
              (pcVar8->obj).mom.x = (float)(-(double)NuTrigTable[uVar33 & 0xffff] * dVar40);
              (pcVar8->obj).mom.z =
                   (float)(-(double)*(float *)((int)NuTrigTable +
                                              (((uVar33 & 0xffff) + 0x4000) * 4 & 0x3fffc)) * dVar4 0
                          );
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
  } while ((int)uVar33 < 0x10);
LAB_80008d00:
  if ((c->obj).dead == '\0') {
    sVar7 = (c->ai).force_action;
    if (sVar7 != -1) {
      (c->obj).anim.newaction = sVar7;
    }
LAB_80008d54:
    fVar10 = 0.5;
  }
  else {
LAB_80008d0c:
    sVar7 = (c->obj).die_action;
    if (sVar7 != -1) {
      (c->obj).anim.newaction = sVar7;
      goto LAB_80008d54;
    }
    fVar10 = 0.0;
  }
  dVar40 = (double)fVar10;
  if ((c->ai).idle_update != '\0') {
    UpdateCharacterIdle(c,(int)(c->obj).character);
  }
  if ((c->obj).dead == '\0') {
    pCVar21 = (c->obj).model;
  }
  else {
    pCVar21 = CModel + (c->obj).die_model[0];
  }
  UpdateAnimPacket(pCVar21,&(c->obj).anim,(float)dVar40,(c->obj).xz_distance);
  sVar7 = (c->obj).character;
  (c->obj).frame = (c->obj).frame + 1;
  if (sVar7 != 0x1e) {
    if (sVar7 < 0x1f) {
      if (sVar7 != 0x1b) {
        if (sVar7 != 0x1c) {
          return;
        }
        if (Level == 6) {
LAB_80008e90:
          iVar16 = 0xb5;
          goto LAB_80008eac;
        }
        if (Level < 7) {
          if (Level == 2) goto LAB_80008ea0;
        }
        else if (Level == 0x22) {
          iVar16 = 0xb4;
          goto LAB_80008eac;
        }
LAB_80008ea8:
        iVar16 = -1;
LAB_80008eac:
        if (iVar16 == -1) {
          return;
        }
        GameSfxLoop(iVar16,&(c->obj).pos);
        return;
      }
    }
    else {
      if (sVar7 == 0x9a) {
        if (unaff_r17 != 0x35) {
          return;
        }
        if ((c->obj).xyz_distance <= 0.00555) {
          return;
        }
        if (Level == 6) {
LAB_80008ea0:
          iVar16 = 0xb6;
          goto LAB_80008eac;
        }
        if (Level < 7) {
          if (Level == 2) {
            iVar16 = 0xb7;
            goto LAB_80008eac;
          }
        }
        else if (Level == 0x22) goto LAB_80008e90;
        goto LAB_80008ea8;
      }
      if (0x9a < sVar7) {
        if (sVar7 != 0xb1) {
          return;
        }
        gamesfx_pitch = 200;
        gamesfx_effect_volume = 0x3fff;
        GameSfxLoop(0x61,&(c->obj).pos);
        return;
      }
      if (sVar7 != 0x22) {
        return;
      }
    }
  }
  if (Level == 6) {
    iVar16 = 0xb4;
  }
  else {
    if (Level < 7) {
      if (Level == 2) {
        iVar16 = 0xb5;
        goto LAB_80008f08;
      }
    }
    else if (Level == 0x22) {
      iVar16 = 0xb3;
      goto LAB_80008f08;
    }
    iVar16 = -1;
  }
LAB_80008f08:
  if (iVar16 != -1) {
    GameSfxLoop(iVar16,&(c->obj).pos);
  }
  return;
}


