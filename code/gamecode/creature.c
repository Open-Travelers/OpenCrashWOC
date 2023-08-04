void ResetPlayer(int set)

{
  bool bVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  mask *pmVar5;
  int iVar6;
  nuvec_s *ppos;
  obj_s *obj;
  double dVar7;
  ushort angle;
  creature_s *c;
  creature_s *plr;
  short veh;
  
  if (PLAYERCOUNT == 0) goto LAB_8001763c;
  if (set != 0) {
    if ((Level != 0x11) || (cp_iALONG < 0x6b)) {
      edobjResetAnimsToZero();
    }
    c = player;
    PlayerStartPos(player,&(player->obj).startpos);
    bVar1 = false;
    if ((bonus_restart == 0) ||
       ((((LDATA->vBONUS).x == 0.0 && ((LDATA->vBONUS).y == 0.0)) && ((LDATA->vBONUS).z == 0.0))))  {
      if (cp_goto == -1) {
        if (cp_iRAIL == -1) {
          fVar3 = (c->obj).startpos.z;
          fVar2 = (c->obj).startpos.y;
          (c->obj).pos.x = (c->obj).startpos.x;
          bVar1 = true;
          goto LAB_80017328;
        }
        ppos = &cpPOS;
        fVar2 = cpPOS.x;
      }
      else {
        ppos = &cpGOTO;
        fVar2 = cpGOTO.x;
      }
      fVar4 = ppos->z;
      fVar3 = ppos->y;
      (c->obj).pos.x = fVar2;
      (c->obj).pos.y = fVar3;
      (c->obj).pos.z = fVar4;
    }
    else {
      fVar3 = (LDATA->vBONUS).z;
      fVar2 = (LDATA->vBONUS).y;
      (c->obj).pos.x = (LDATA->vBONUS).x;
LAB_80017328:
      (c->obj).pos.y = fVar2;
      (c->obj).pos.z = fVar3;
    }
    ppos = &(c->obj).pos;
    if (bonus_restart != 0) {
      bonus_restart = 0;
    }
    if (((Level == 2) && (cp_iRAIL == 0)) && (cp_iALONG == 0x23)) {
      iVar6 = 2;
    }
    else if (((LBIT._0_4_ & 4 | LBIT._4_4_ & 0x40) != 0) || ((Level == 2 && (bVar1)))) {
      iVar6 = 1;
    }
    else {
      iVar6 = 0;
    }
    obj = &c->obj;
    ResetPlayerMoves(c);
    GetTopBot(c);
    NewTopBot(obj);
    OldTopBot(obj);
    fVar2 = NewShadowPlat(ppos,0.0);
    dVar7 = (double)fVar2;
    GetSurfaceInfo(obj,1,fVar2);
    if (cp_goto == -1) {
      if (iVar6 == 2) {
        fVar2 = (c->obj).SCALE;
        fVar3 = CData[32].min.y;
LAB_80017438:
        (c->obj).pos.y = (c->obj).pos.y - fVar3 * fVar2;
      }
      else if (iVar6 == 0) {
        if ((Level != 0x1d) && (dVar7 != 2000000.0)) {
          (c->obj).pos.y = (float)(dVar7 - (double)((c->obj).bot * (c->obj).SCALE));
        }
      }
      else if (cp_iRAIL != -1) {
        fVar3 = (c->obj).bot;
        fVar2 = (c->obj).SCALE;
        goto LAB_80017438;
      }
    }
    else {
      cp_goto = -1;
    }
    fVar3 = (c->obj).pos.y;
    fVar2 = (c->obj).pos.z;
    (c->obj).oldpos.x = (c->obj).pos.x;
    (c->obj).oldpos.y = fVar3;
    (c->obj).oldpos.z = fVar2;
    fVar3 = v000.z;
    fVar2 = v000.y;
    (c->obj).mom.x = v000.x;
    (c->obj).mom.z = fVar3;
    (c->obj).mom.y = fVar2;
    ComplexRailPosition(ppos,(int)(c->obj).RPos.iRAIL,(int)(c->obj).RPos.iALONG,&(c->obj).RPos,1) ;
    if ((VEHICLECONTROL == 2) || ((VEHICLECONTROL == 1 && ((c->obj).vehicle == 0x20)))) {
      (c->obj).thdg = 0;
      (c->obj).hdg = 0;
    }
    else if (Level == 0x19) {
      (c->obj).hdg = 0x4000;
    }
    else if (best_cRPos != (RPos_s *)0x0) {
      angle = best_cRPos->angle;
      (c->obj).hdg = angle;
      (c->obj).thdg = angle;
    }
    veh = (c->obj).vehicle;
    if (veh == 0x53) {
      ResetAtlas(c);
    }
    else if (veh < 0x54) {
      if ((veh == 0x36) && (Level == 0x18)) {
        SetWeatherStartPos(c);
      }
    }
    else if (veh == 99) {
      ResetJeep(c);
    }
    plr_rebound = 0;
    ATLASCAMHEIGHT = 2.5;
    ResetLights(&c->lights);
    pmVar5 = (c->obj).mask;
    if ((pmVar5 != (mask *)0x0) && (pmVar5->active != 0)) {
      if (2 < pmVar5->active) {
        pmVar5->active = 2;
      }
      ResetLights(&((c->obj).mask)->lights);
    }
  }
  c = player;
  plr_crates.draw = 0;
  plr_crates.frame = '\0';
  plr_crates.count = 0;
  (player->obj).dead = '\0';
  plr = player;
  (c->obj).scale = 1.0;
  (plr->obj).finished = '\0';
  pmVar5 = (player->obj).mask;
  if (((pmVar5 != (mask *)0x0) && (pmVar5->active == 0)) && (4 < LivesLost)) {
    NewMask(pmVar5,&(player->obj).pos);
  }
LAB_8001763c:
  if ((Demo == 0) && (GameMode != 1)) {
    force_panel_items_update = 0x3c;
    force_panel_wumpa_update = 0x3c;
    force_panel_crate_update = 0x3c;
    force_panel_lives_update = 0x3c;
  }
  boss_dead = 0;
  point_time = 0.0;
  check_duration = 0.0;
  check_time = 0.0;
  point_duration = 0;
  return;
}


void ResetPlayerMoves(creature_s *c)

{
  float fVar1;
  
  c->crawl = '\0';
  c->crawl_lock = '\x01';
  c->jump = '\0';
  c->slam = '\0';
  c->spin = '\0';
  c->tiptoe = '\0';
  c->sprint = '\0';
  c->somersault = '\0';
  c->land = '\0';
  c->idle_mode = '\0';
  c->idle_sigh = '\0';
  fVar1 = IDLEWAIT * 30.0;
  c->crouch_pos = 0;
  c->slam_wait = 0;
  c->spin_wait = 0;
  c->slide = 0;
  c->target = '\0';
  c->fire = '\0';
  c->tap = '\0';
  c->freeze = '\0';
  (c->obj).transporting = '\0';
  c->idle_action = 0x22;
  c->idle_wait = fVar1;
  c->idle_time = 0.0;
  (c->obj).idle_gametime = 0.0;
  ResetAnimPacket(&(c->obj).anim,0x22);
  (c->obj).ground = '\x03';
  (c->obj).frame = 0;
  (c->obj).submerged = '\0';
  (c->obj).scale = 1.0;
  (c->obj).RADIUS = (c->obj).radius;
  (c->obj).attack = 0;
  (c->obj).dyrot = 0;
  (c->obj).boing = '\0';
  (c->obj).dangle = '\0';
  (c->obj).old_ground = '\x03';
  (c->obj).SCALE = 1.0;
  return;
}

void RemoveCreature(creature_s *c)

{
  RemoveGameObject(&c->obj);
  c->used = '\0';
  return;
}


void CloseCreatures(void)

{
  NUHGOBJ_s *hgobj;
  int i;
  creature_s *c;
  int j;
  
  i = 0;
  c = Character;
  j = 9;
  do {
    if ((&Character[0].used)[i] != '\0') {
      RemoveCreature(c);
    }
    c = c + 1;
    i = i + 0xce4;
    j = j + -1;
  } while (j != 0);
  j = 0;
  i = 0x31;
  do {
    hgobj = *(NUHGOBJ_s **)((int)CModel[0].anmdata + j + -4);
    if (hgobj != (NUHGOBJ_s *)0x0) {
      NuHGobjDestroy(hgobj);
      *(undefined4 *)((int)CModel[0].anmdata + j + -4) = 0;
    }
    j = j + 0x7ac;
    i = i + -1;
  } while (i != 0);
  return;
}


void ManageCreatures(void)

{
  bool bVar1;
  float fVar2;
  short sVar3;
  char cVar4;
  ai_s *paVar5;
  creature_s *pcVar6;
  uint uVar7;
  nuvec_s *a;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  creature_s *c;
  int iVar12;
  double dVar13;
  double in_f29;
  double dVar14;
  double dVar15;
  float fVar16;
  
  pcVar6 = player;
  if ((Level == 0x25) || (fVar16 = AIVISRANGE, (LDATA->flags & 0x202) != 0)) {
    fVar16 = (float)((double)CONCAT44(0x43300000,(uint)LDATA->farclip) - 4503599627370496.0);
  }
  if ((float)((double)CONCAT44(0x43300000,LDATA->farclip ^ 0x80000000) - 4503601774854144.0) <
      fVar16) {
    fVar16 = (float)((double)CONCAT44(0x43300000,(uint)LDATA->farclip) - 4503599627370496.0);
  }
  dVar14 = (double)(fVar16 * fVar16);
  if (((player != (creature_s *)0x0) && (player->used != '\0')) && ((player->obj).dead != '\0')) {
    fVar16 = (player->obj).die_duration;
    fVar2 = (player->obj).die_time + 0.01666667;
    (player->obj).die_time = fVar2;
    iVar8 = new_mode;
    if (((fVar16 <= fVar2) && ((pcVar6->obj).die_time = fVar16, iVar8 == -1)) && (new_level == -1) )
    {
      if (Demo == 0) {
        if (Bonus == 2) {
          if (((plr_bonus_wumpas.count == 0) && (bonus_wumpa_delay == '\0')) &&
             ((bonus_wumpa_wait <= 0.0 &&
              ((((save_bonus_crates_destroyed * 6 + 6 <= bonus_finish_frame &&
                 (bonus_crates_wait <= 0.0)) && (bonus_lives == 0)) &&
               ((bonus_life_delay == '\0' && (bonus_lives_wait <= 0.0)))))))) {
            NuSoundStopStream(0);
            NuSoundStopStream(1);
            bonus_restart = 1;
            new_mode = GameMode;
          }
        }
        else if (TimeTrial == 0) {
          if (plr_lives.count == 0) {
            new_level = 0x26;
          }
          else {
            plr_lives.count = plr_lives.count + -1;
            if (Adventure != 0) {
              LivesLost = LivesLost + 1;
              Game.lives = (uchar)plr_lives.count;
            }
            new_mode = GameMode;
            player->jump_hold = '\0';
          }
          plr_died = 1;
        }
        else {
          new_mode = GameMode;
        }
      }
      else {
        new_level = 0x23;
      }
    }
  }
  dVar13 = 0.01666666753590107;
  dVar15 = 0.0;
  iVar8 = -1;
  iVar9 = 1;
  iVar10 = 0xce4;
  pcVar6 = Character;
  do {
    c = pcVar6 + 1;
    if (c->used == '\0') {
      if (iVar8 == -1) {
        iVar8 = iVar9;
      }
    }
    else if (pcVar6[1].on == '\0') {
      if ((pcVar6[1].off_wait != '\0') &&
         (cVar4 = pcVar6[1].off_wait + -1, pcVar6[1].off_wait = cVar4, cVar4 == '\0')) {
        RemoveCreature(c);
      }
    }
    else {
      cVar4 = pcVar6[1].obj.dead;
      if ((int)cVar4 == 0) {
        fVar16 = NuVecDistSqr(&AITab[pcVar6[1].i_aitab].origin,&(player->obj).pos,(nuvec_s *)0x0);
        if ((dVar14 < (double)fVar16) ||
           ((level_part_2 != 0 && (AITab[pcVar6[1].i_aitab].ai_type != 'O')))) {
          sVar3 = pcVar6[1].obj.character;
          pcVar6[1].on = cVar4;
          pcVar6[1].off_wait = '\x02';
          if (sVar3 == 0x76) {
            clock_ok = (int)cVar4;
          }
        }
      }
      else {
        fVar16 = (float)((double)*(float *)((int)&Character[0].ai + iVar10 + -0x60) + dVar13);
        *(float *)((int)&Character[0].ai + iVar10 + -0x60) = fVar16;
        fVar2 = pcVar6[1].obj.die_duration;
        if (fVar2 <= fVar16) {
          *(float *)((int)&Character[0].ai + iVar10 + -0x60) = fVar2;
          pcVar6[1].on = '\0';
          pcVar6[1].off_wait = '\x02';
          iVar11 = (int)pcVar6[1].i_aitab;
          if ((iVar11 != -1) && ((double)AITab[iVar11].delay <= dVar15)) {
            AITab[iVar11].status = '\0';
          }
        }
      }
    }
    iVar9 = iVar9 + 1;
    iVar10 = iVar10 + 0xce4;
    pcVar6 = c;
  } while (iVar9 < 9);
  if ((iVar8 == -1) && (iVar8 = c_slot + 1, c_slot + 1 == 9)) {
    c_slot = 1;
    iVar8 = c_slot;
  }
  c_slot = iVar8;
  iVar8 = -1;
  a = &(player->obj).pos;
  if ((Character[c_slot].used != '\0') && (iVar8 = (int)Character[c_slot].i_aitab, iVar8 != -1)) {
    fVar16 = NuVecDistSqr(a,AITab[iVar8].pos,(nuvec_s *)0x0);
    in_f29 = (double)fVar16;
  }
  iVar9 = iVar8;
  if (0 < LEVELAICOUNT) {
    dVar13 = 0.0;
    paVar5 = AITab;
    iVar10 = 0;
    do {
      if (((dVar13 < (double)paVar5->delay) && (dVar13 < (double)paVar5->time)) &&
         (fVar16 = (float)((double)paVar5->time - 0.01666666753590107), paVar5->time = fVar16,
         (double)fVar16 < dVar13)) {
        paVar5->time = (float)dVar13;
      }
      iVar11 = iVar10 + 1;
      iVar12 = 8;
      pcVar6 = Character;
      do {
        pcVar6 = pcVar6 + 1;
        if (((pcVar6->used != '\0') && (pcVar6->i_aitab == iVar10)) &&
           ((paVar5->delay <= 0.0 || (paVar5->time != 0.0)))) goto LAB_80017f48;
        iVar12 = iVar12 + -1;
      } while (iVar12 != 0);
      if (((level_part_2 == 0) || (paVar5->ai_type == 'O')) &&
         ((paVar5->status != '\0' &&
          (uVar7 = (uint)(byte)paVar5->ai_type, CRemap[AIType[uVar7].character] != -1)))) {
        if (uVar7 < 0x54) {
          if (uVar7 < 0x52) {
            if (uVar7 < 0x50) {
              if (uVar7 < 0x4e) {
                if (uVar7 < 0x4c) goto LAB_80017f20;
                if ((Game.level[Level].flags & 8) == 0) {
                  bVar1 = (plr_items & 1) == 0;
LAB_80017ed4:
                  if ((bVar1) && ((LDATA->flags & 0x200) == 0)) goto LAB_80017f20;
                }
              }
              else if (((Demo == 0) && ((Hub == 5 || ((Game.level[Level].flags & 8) != 0)))) &&
                      (uVar7 = clock_ok, TimeTrial == 0)) goto joined_r0x80017f1c;
            }
            else if ((Game.level[Level].flags & 0x10) == 0) {
              bVar1 = (plr_items & 2) == 0;
              goto LAB_80017ec8;
            }
          }
          else if (((((Game.level[Level].flags & 0x20) == 0) && ((plr_items & 4) == 0)) &&
                   (TimeTrial == 0)) && (uVar7 = bonusgem_ok, (LDATA->flags & 0x200) == 0)) {
joined_r0x80017f1c:
            if (uVar7 != 0) goto LAB_80017f20;
          }
        }
        else if (uVar7 == 0x57) {
          if ((Game.level[Level].flags & 0x200) == 0) {
            bVar1 = (plr_items & 0x40) == 0;
            goto LAB_80017ec8;
          }
        }
        else if (uVar7 < 0x58) {
          if (uVar7 == 0x55) {
            if ((Game.level[Level].flags & 0x80) == 0) {
              bVar1 = (plr_items & 0x20) == 0;
              goto LAB_80017ec8;
            }
          }
          else if (uVar7 < 0x56) {
            if ((Game.level[Level].flags & 0x40) == 0) {
              bVar1 = (plr_items & 8) == 0;
LAB_80017ec8:
              if (bVar1) {
                bVar1 = TimeTrial == 0;
                goto LAB_80017ed4;
              }
            }
          }
          else if ((Game.level[Level].flags & 0x100) == 0) {
            bVar1 = (plr_items & 0x10) == 0;
            goto LAB_80017ec8;
          }
        }
        else if (uVar7 == 0x59) {
LAB_80017efc:
          if (boss_dead == 1) {
            uVar7 = LBIT._4_4_ & 0x3e00000;
            goto joined_r0x80017f1c;
          }
        }
        else if (uVar7 < 0x59) {
          if ((Game.level[Level].flags & 0x400) == 0) {
            bVar1 = (plr_items & 0x80) == 0;
            goto LAB_80017ec8;
          }
        }
        else {
          if (uVar7 == 0x5a) {
            if ((Game.powerbits & 0x20) != 0) goto LAB_80017f48;
          }
          else if (uVar7 < 0x5f) goto LAB_80017efc;
LAB_80017f20:
          fVar16 = NuVecDistSqr(a,paVar5->pos,(nuvec_s *)0x0);
          if ((iVar9 == -1) || ((double)fVar16 < in_f29)) {
            iVar9 = iVar10;
            in_f29 = (double)fVar16;
          }
        }
      }
LAB_80017f48:
      paVar5 = paVar5 + 1;
      iVar10 = iVar11;
    } while (iVar11 < LEVELAICOUNT);
  }
  iVar10 = c_slot;
  if (((iVar9 != -1) && (iVar9 != iVar8)) &&
     ((dVar13 = 0.0, AITab[iVar9].delay <= 0.0 || (AITab[iVar9].time <= 0.0)))) {
    if (Character[c_slot].used == '\0') {
      fVar16 = NuVecDistSqr(&AITab[iVar9].origin,&(player->obj).pos,(nuvec_s *)0x0);
      if ((double)fVar16 < dVar14) {
        AddCreature((int)AIType[(byte)AITab[iVar9].ai_type].character,c_slot,iVar9);
        if (dVar13 < (double)AITab[iVar9].delay) {
          AITab[iVar9].time = AITab[iVar9].delay;
        }
      }
    }
    else {
      bVar1 = Character[c_slot].obj.character != 0x76;
      if ((bVar1) || (Level != 0x22)) {
        Character[c_slot].off_wait = '\x02';
        Character[iVar10].on = '\0';
        if (!bVar1) {
          clock_ok = 0;
        }
      }
    }
  }
  return;
}


//PS2
float ModelAnimDuration(u32 character,u32 action,float start,float end)
{
    f32 t;
    struct CharacterModel *model;
    s32 index;

    if ((character > 0xBE) || (action > 0x75)){
        return 1.0f;
    }
    
    index = CRemap[character];
    if(index == -1)
    { 
        return 1.0f;
    }
    
    model = &CModel[index];
    if(model->anmdata[action] == NULL)
    { 
        return 1.0f;
    }
    
    t = model->anmdata[action]->time;
    if ((start >= 1.0f) && (start < t))
    {
        if ((end >= 1.0f) && (end < t) && (start < end))
        {
            t = end - start;
        }
        else
        {
            t -= start;
        }
    }
    else if ((end >= 1.0f) && (end < t))
    {
        if (start < end)
        {
            t = end - 1.0f;
        }
    }
    return t * (1.0f / model->animlist[action]->speed) * 0.033333335f;
}


int LoadCharacterModel(int character,int level,int *cmodel_index,int clist_entry,int *remap)

{
  int iVar1;
  uchar poi_id;
  int index;
  int iVar2;
  NUHGOBJ_s *HGobj;
  NUPOINTOFINTEREST_s *poi;
  nuAnimData_s *animdat;
  Font3DObjTab *Fnt3dTab;
  space *GCTab;
  CharacterModel *C_Model;
  NUPOINTOFINTEREST_s **pLoc;
  AnimList *CHanim;
  AnimList *pAVar3;
  int cnt;
  int iVar4;
  char path [64];
  short act;
  
  index = *cmodel_index;
  iVar2 = index * 0x7ac;
  C_Model = CModel + index;
  if (level == 0x28) {
    GCTab = (space *)SpaceGameCutTab[gamecut * 2];
  }
  else {
    GCTab = (space *)0x0;
  }
  if ((character == 0xff) || (0x30 < *remap)) {
    return 0;
  }
  memset(C_Model,0,0x7ac);
  path._0_4_ = 0x63686172;
  path._4_2_ = s\_repeated;
  path[6] = '\0';
  strcat(path,CData[character].path);
  strcat(path,"\\");
  if (C_Model->hobj == (NUHGOBJ_s *)0x0) {
    strcpy(tbuf,path);
    strcat(tbuf,CData[character].file);
    strcat(tbuf,".hgo");
    HGobj = NuHGobjRead(&superbuffer_ptr,tbuf);
    C_Model->hobj = HGobj;
    if ((uint)character < 2) {
      if (HGobj == (NUHGOBJ_s *)0x0) goto LAB_800185e0;
      crash_loaded = 1;
    }
    if (HGobj == (NUHGOBJ_s *)0x0) goto LAB_800185e0;
  }
  cnt = 0;
  pLoc = CModel[index].pLOCATOR;
  do {
    poi_id = (uchar)cnt;
    cnt = cnt + 1;
    poi = NuHGobjGetPOI(C_Model->hobj,poi_id);
    *pLoc = poi;
    pLoc = pLoc + 1;
  } while (cnt < 0x10);
  if ((character == 0x54) || (character == 0x9f)) {
    path._0_4_ = 0x63686172;
    path._4_2_ = 0x735c;
    path[6] = '\0';
    strcat(path,CData[0].path);
    strcat(path,"\\");
  }
  CHanim = CData[character].anim;
  if (level == 0x28) {
    CHanim = GCTab[clist_entry].animlist;
  }
  else if (character == 0) {
    if (((LBIT._0_4_ & 0x10 | LBIT._4_4_ & 0x1002000) == 0) || (level == 0x1e)) {
      if ((LBIT._4_4_ & 0x40000) == 0) {
        if ((LBIT._0_4_ & 1 | LBIT._4_4_ & 0x210801) == 0) {
          if (level == 0x1d) {
            CHanim = CrashAnim_GYRO;
          }
          else if (level == 0x1c) {
            CHanim = CrashAnim_MINETUB;
          }
          else if (level == 0x2b) {
            CHanim = CrashAnim_CREDITS;
          }
        }
        else {
          CHanim = CrashAnim_ATLASPHERE;
        }
      }
      else {
        CHanim = CrashAnim_MOSQUITO;
      }
    }
    else {
      CHanim = CrashAnim_GLIDER;
    }
  }
  else if ((character == 1) && ((LBIT._4_4_ & 0x4000000) != 0)) {
    CHanim = CocoAnim_DROPSHIP;
  }
  if (((CHanim != (AnimList *)0x0) && (CHanim->file != (char *)0x0)) &&
     (act = CHanim->action, -1 < act)) {
    while (act < 0x76) {
      if ((*(uint *)&CHanim->levbits & LBIT._0_4_ |
          *(uint *)((int)&CHanim->levbits + 4) & LBIT._4_4_) != 0) {
        if (((CHanim->flags & 2) != 0) &&
           (iVar4 = iVar2 + -0x7fdac120, *(int *)(iVar4 + act * 4) == 0)) {
          strcpy(tbuf,path);
          strcat(tbuf,CHanim->file);
          strcat(tbuf,".ani");
          animdat = InstAnimDataLoad(tbuf);
          *(nuAnimData_s **)(iVar4 + CHanim->action * 4) = animdat;
          iVar1 = CHanim->action * 4;
          if (*(int *)(iVar4 + iVar1) != 0) {
            *(AnimList **)(iVar2 + -0x7fdabf48 + iVar1) = CHanim;
          }
        }
        if (((CHanim->flags & 4) != 0) &&
           (iVar4 = iVar2 + -0x7fdabd70, *(int *)(iVar4 + CHanim->action * 4) == 0)) {
          strcpy(tbuf,path);
          strcat(tbuf,CHanim->file);
          strcat(tbuf,".bsa");
          animdat = InstAnimDataLoad(tbuf);
          *(nuAnimData_s **)(iVar4 + CHanim->action * 4) = animdat;
          iVar1 = CHanim->action * 4;
          if (*(int *)(iVar4 + iVar1) != 0) {
            *(AnimList **)(iVar2 + -0x7fdabb98 + iVar1) = CHanim;
          }
        }
      }
      pAVar3 = CHanim + 1;
      if (((pAVar3 == (AnimList *)0x0) || (pAVar3->file == (char *)0x0)) ||
         (act = CHanim[1].action, CHanim = pAVar3, act < 0)) break;
    }
  }
  CModel[index].character = (short)character;
  CRemap[character] = *(char *)((int)cmodel_index + 3);
  *cmodel_index = *cmodel_index + 1;
  *remap = *remap + 1;
LAB_800185e0:
  Fnt3dTab = Font3DObjTab;
  iVar2 = 0;
  index = 0;
  do {
    if (((Fnt3dTab->flags & 1) != 0) && (*(short *)((int)&Font3DObjTab[0].i + index) == character) )
    {
      Fnt3dTab = (Font3DObjTab *)Font3DMtlTab;
      CLetter[character] = (char)iVar2 + 'a';
      index = 0x138;
      iVar2 = 0x1a;
    }
    iVar2 = iVar2 + 1;
    Fnt3dTab = Fnt3dTab + 1;
    index = index + 0xc;
  } while (iVar2 < 0x1a);
  return 1;
}


void PurgeCharacterModels(void)

{
  CharacterModel *Cmodel;
  int i;
  int iVar1;
  
  iVar1 = 0xbf;
  i = 0;
  do {
    CRemap[i] = -1;
    CLetter[i] = 0x3f;
    i = i + 1;
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  Cmodel = CModel + 0x30;
  do {
    Cmodel->hobj = (NUHGOBJ_s *)0x0;
    Cmodel = Cmodel + -1;
  } while (-0x7fdac125 < (int)Cmodel);
  return;
}


void LoadCharacterModels(void)

{
  uint character;
  int iVar1;
  space *space;
  int clist_entry;
  double dVar2;
  float fVar3;
  int cmodel_index;
  int remap;
  char local_28 [12];
  
  clist_entry = (int)(CModel[0].hobj != (NUHGOBJ_s *)0x0);
  if (Level == 0x28) {
    space = (space *)SpaceGameCutTab[gamecut * 2];
  }
  else {
    space = (space *)0x0;
  }
  if (LDATA->cList != (uchar *)0x0) {
    cmodel_index = clist_entry;
    remap = clist_entry;
    while( true ) {
      force_decal = 0;
      if (space == (space *)0x0) {
        character = (uint)LDATA->cList[clist_entry];
      }
      else {
        character = space[clist_entry].character;
      }
      if ((character == 0x1b) || (character == 0x86)) {
        force_decal = 1;
      }
      iVar1 = LoadCharacterModel(character,Level,&cmodel_index,clist_entry,&remap);
      if (iVar1 == 0) break;
      clist_entry = clist_entry + 1;
    }
  }
  fVar3 = ModelAnimDuration(0,0x30,0.0,0.0);
  dVar2 = 60.0;
  local_28._0_8_ = (longlong)(int)(fVar3 * 60.0);
  CrashMoveInfo.JUMPLANDFRAMES = (short)(int)(fVar3 * 60.0);
  fVar3 = ModelAnimDuration(0,0x1f,0.0,0.0);
  local_28._0_8_ = (longlong)(int)((double)fVar3 * dVar2);
  CrashMoveInfo.SLAMWAITFRAMES = (short)(int)((double)fVar3 * dVar2);
  fVar3 = ModelAnimDuration(0,0x44,0.0,0.0);
  local_28._0_8_ = (longlong)(int)((double)fVar3 * dVar2);
  CrashMoveInfo.SOMERSAULTFRAMES = (short)(int)((double)fVar3 * dVar2);
  fVar3 = ModelAnimDuration(0,0x43,0.0,0.0);
  local_28._0_8_ = (longlong)(int)((double)fVar3 * dVar2);
  CrashMoveInfo.SLIDEFRAMES = (short)(int)((double)fVar3 * dVar2);
  fVar3 = ModelAnimDuration(0,3,0.0,0.0);
  local_28._0_8_ = (longlong)(int)((double)fVar3 * dVar2);
  CrashMoveInfo.CROUCHINGFRAMES = (short)(int)((double)fVar3 * dVar2);
  fVar3 = ModelAnimDuration(1,0x30,0.0,0.0);
  local_28._0_8_ = (longlong)(int)((double)fVar3 * dVar2);
  CocoMoveInfo.JUMPLANDFRAMES = (short)(int)((double)fVar3 * dVar2);
  fVar3 = ModelAnimDuration(1,0x1f,0.0,0.0);
  local_28._0_8_ = (longlong)(int)((double)fVar3 * dVar2);
  CocoMoveInfo.SLAMWAITFRAMES = (short)(int)((double)fVar3 * dVar2);
  fVar3 = ModelAnimDuration(1,0x44,0.0,0.0);
  local_28._0_8_ = (longlong)(int)((double)fVar3 * dVar2);
  CocoMoveInfo.SOMERSAULTFRAMES = (short)(int)((double)fVar3 * dVar2);
  fVar3 = ModelAnimDuration(1,0x46,0.0,0.0);
  local_28._0_8_ = (longlong)(int)((double)fVar3 * dVar2);
  CocoMoveInfo.SPINFRAMES = (short)(int)((double)fVar3 * dVar2);
  fVar3 = ModelAnimDuration(1,0x43,0.0,0.0);
  local_28._0_8_ = (longlong)(int)((double)fVar3 * dVar2);
  CocoMoveInfo.SLIDEFRAMES = (short)(int)((double)fVar3 * dVar2);
  fVar3 = ModelAnimDuration(0x89,99,0.0,0.0);
  MineCartMoveInfo.JUMPFRAMES0 = (short)(int)((double)fVar3 * dVar2);
  return;
}


void ChangeCharacter(creature_s *c,int character)

{
  short cht;
  Moveinfo_s *moveinfo;
  float y;
  bool check;
  float radius;
  
  if (((uint)character < 0xbf) && (CRemap[character] != -1)) {
    cht = (short)character;
    (c->obj).character = cht;
    if (cht == 0) {
      moveinfo = &CrashMoveInfo;
    }
    else if (cht == 1) {
      moveinfo = &CocoMoveInfo;
    }
    else {
      moveinfo = (Moveinfo_s *)&DefaultMoveInfo;
    }
    c->OnFootMoveInfo = moveinfo;
    check = c == player;
    (c->obj).model = CModel + CRemap[character];
    radius = CData[character].radius;
    (c->obj).radius = radius;
    (c->obj).RADIUS = radius;
    radius = CData[character].min.z;
    y = CData[character].min.y;
    (c->obj).min.x = CData[character].min.x;
    (c->obj).min.z = radius;
    (c->obj).min.y = y;
    radius = CData[character].max.z;
    y = CData[character].max.y;
    (c->obj).max.x = CData[character].max.x;
    (c->obj).max.z = radius;
    (c->obj).max.y = y;
    radius = (c->obj).max.y;
    (c->obj).bot = (c->obj).min.y;
    (c->obj).top = radius;
    if (check) {
      ResetPlayerMoves(c);
    }
    else {
      ResetAnimPacket(&(c->obj).anim,0x22);
    }
  }
  return;
}

void PlayerStartPos(creature_s *c,nuvec_s *pos)

{
  float Cpos_x;
  float Cpos_y;
  ushort hdg;
  int lev;
  float v000_x;
  float v000_y;
  
  lev = Level;
  hdg = cutang_CRASH;
  v000_y = v000.y;
  v000_x = v000.x;
  Cpos_y = cutpos_CRASH.y;
  Cpos_x = cutpos_CRASH.x;
  if (Level == 0x25) {
    if (gamecut_hack == 1) {
      last_hub = 0;
      last_level = 0x15;
      gamecut_hack = 0;
    }
    if (GameMode == 1) {
      pos->z = cutpos_CRASH.z;
      pos->x = Cpos_x;
      pos->y = Cpos_y;
      (c->obj).hdg = hdg;
      last_hub = -1;
      last_level = -1;
      tumble_time = 0.0;
      tumble_action = -1;
      tumble_duration = 0.0;
      return;
    }
    if (((last_hub != -1) && (last_level != -1)) && (LData[last_level].hub != -1)) {
      HubStart(&c->obj,last_hub,last_level,pos);
      return;
    }
  }
  if (pos_START == (nuvec_s *)0x0) {
    pos->z = v000.z;
    pos->x = v000_x;
    pos->y = v000_y;
    return;
  }
  Cpos_y = pos_START->x;
  Cpos_x = pos_START->y;
  pos->z = pos_START->z;
  pos->x = Cpos_y;
  pos->y = Cpos_x;
  if (lev == 0x25) {
    (c->obj).hdg = 0x8000;
    tumble_duration = 0.0;
    tumble_action = -1;
    tumble_time = 0.0;
    last_level = -1;
    last_hub = -1;
    return;
  }
  return;
}


int AddCreature(int character,int index,int i_aitab)

{
  char cVar1;
  float fVar2;
  uint flags;
  nuAnimData_s *pnVar3;
  float fVar4;
  float fVar5;
  Moveinfo_s *moveinfo;
  float fVar6;
  CharacterModel *model;
  nuAnimData_s **anmdat;
  short action;
  creatcmd_s *CMD;
  creature_s *creature;
  nuvec_s pos_c;
  int adv;
  uchar livesD;
  uchar wumpasD;
  
  if (((((uint)index < 9) && ((uint)character < 0xbf)) &&
      ((i_aitab == -1 || (character == AIType[(byte)AITab[i_aitab].ai_type].character)))) &&
     (CRemap[character] != -1)) {
    creature = Character + index;
    if (Character[index].used != '\0') {
      return 0;
    }
    memset(creature,0,0xce4);
    adv = AddGameObject(&Character[index].obj,creature);
    if (adv != 0) {
      Character[index].obj.pLOCATOR = (numtx_s *)(index * 0xce4 + -0x7fdb32b4);
      Character[index].used = '\x01';
      Character[index].on = '\x01';
      Character[index].off_wait = '\x02';
      if (i_aitab == -1) {
        PlayerStartPos(creature,&pos_c);
        CMD = (creatcmd_s *)0x0;
      }
      else {
        CMD = AIType[(byte)AITab[i_aitab].ai_type].cmd;
        adv = CMD->i;
        pos_c.x = AITab[i_aitab].pos[adv].x;
        pos_c.z = AITab[i_aitab].pos[adv].z;
        pos_c.y = AITab[i_aitab].pos[adv].y;
      }
      action = (short)character;
      Character[index].i_aitab = (char)i_aitab;
      Character[index].cmdtable = CMD;
      Character[index].cmdcurr = CMD;
      Character[index].obj.character = action;
      if (action == 0) {
        moveinfo = &CrashMoveInfo;
      }
      else if (action == 1) {
        moveinfo = &CocoMoveInfo;
      }
      else {
        moveinfo = (Moveinfo_s *)&DefaultMoveInfo;
      }
      Character[index].OnFootMoveInfo = moveinfo;
      fVar2 = CData[character].radius;
      cVar1 = CRemap[character];
      fVar6 = CData[character].min.x;
      fVar5 = CData[character].min.z;
      fVar4 = CData[character].min.y;
      Character[index].obj.radius = fVar2;
      Character[index].obj.min.x = fVar6;
      fVar6 = CData[character].max.x;
      Character[index].obj.RADIUS = fVar2;
      Character[index].obj.model = CModel + cVar1;
      Character[index].obj.min.z = fVar5;
      Character[index].obj.min.y = fVar4;
      fVar2 = CData[character].max.z;
      fVar4 = CData[character].max.y;
      Character[index].obj.max.x = fVar6;
      Character[index].obj.max.z = fVar2;
      Character[index].obj.max.y = fVar4;
      fVar2 = Character[index].obj.max.y;
      Character[index].obj.bot = Character[index].obj.min.y;
      Character[index].obj.top = fVar2;
      Character[index].obj.startpos.x = pos_c.x;
      Character[index].obj.startpos.z = pos_c.z;
      Character[index].obj.startpos.y = pos_c.y;
      Character[index].obj.oldpos.x = Character[index].obj.startpos.x;
      Character[index].obj.oldpos.z = pos_c.z;
      Character[index].obj.oldpos.y = pos_c.y;
      Character[index].obj.pos.x = Character[index].obj.oldpos.x;
      Character[index].obj.pos.y = pos_c.y;
      adv = Adventure;
      Character[index].obj.pos.z = pos_c.z;
      Character[index].obj.flags = 2;
      wumpasD = Game.wumpas;
      livesD = Game.lives;
      if ((adv == 0) || (creature != player)) {
        flags = Character[index].obj.flags;
        Character[index].obj.vehicle = -1;
        Character[index].obj.flags = flags | 4;
        Character[index].pad_type = '\0';
        Character[index].obj.mask = (mask *)0x0;
      }
      else {
        Character[index].obj.vehicle = LDATA->vehicle;
        Character[index].obj.flags = 3;
        plr_lives.draw = (short)livesD;
        plr_wumpas.draw = (short)wumpasD;
        plr_lives.count = plr_lives.draw;
        plr_wumpas.count = plr_wumpas.draw;
        Character[index].obj.mask = &Mask;
        Character[index].pad_type = '\x01';
      }
      fVar5 = Character[index].obj.pos.x;
      Character[index].obj.die_duration = 0.5;
      fVar4 = Character[index].obj.pos.y;
      fVar2 = Character[index].obj.pos.z;
      Character[index].ai.oldpos.x = fVar5;
      Character[index].ai.oldpos.z = fVar2;
      Character[index].ai.oldpos.y = fVar4;
      Character[index].obj.anim.action = 0x22;
      model = Character[index].obj.model;
      if ((model != (CharacterModel *)0x0) &&
         (anmdat = model->anmdata, model->anmdata[0x22] == (nuAnimData_s *)0x0)) {
        adv = 0;
        pnVar3 = *anmdat;
        while (pnVar3 == (nuAnimData_s *)0x0) {
          adv = adv + 1;
          if (0x75 < adv) goto LAB_80018fa8;
          anmdat = anmdat + 1;
          pnVar3 = *anmdat;
        }
        if (adv < 0x76) {
          Character[index].obj.anim.action = (short)adv;
        }
      }
LAB_80018fa8:
      action = Character[index].obj.anim.action;
      Character[index].obj.anim.oldaction = action;
      Character[index].obj.anim.newaction = action;
      ResetLights(&Character[index].lights);
      return 1;
    }
  }
  return 0;
}


void TerrainFailsafe(obj_s *obj)

{
  bool check;
  float Y;
  
  Y = SAFEY;
  if (obj->shadow != 2000000.0) {
    return;
  }
  check = SAFEY <= obj->bot * obj->SCALE + (obj->pos).y;
  obj->shadow = SAFEY;
  if (check) {
    return;
  }
  (obj->pos).y = Y - (obj->min).y * obj->SCALE;
  return;
}


int NewCharacterIdle(creature_s *c,CharacterModel *model)

{
  short sVar1;
  short sVar2;
  float fVar3;
  bool bVar4;
  int qrnd;
  int iVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  undefined4 local_208;
  undefined4 local_30;
  uint uStack_2c;
  
  if (GameMode == 1) {
LAB_80019374:
    iVar5 = 0;
  }
  else {
    if (((c->idle_mode == '\0') && (c->idle_sigh == '\0')) &&
       (c->idle_sigh = '\x01', model->anmdata[0x3d] != (nuAnimData_s *)0x0)) {
      c->idle_repeat = '\x01';
      c->idle_action = 0x3d;
      iVar6 = 0x22;
    }
    else {
      iVar5 = 0;
      iVar7 = 0;
      qrnd = 0;
      iVar6 = 0;
      do {
        if ((*(int *)((int)model->anmdata + qrnd) != 0) &&
           ((*(ushort *)(*(int *)((int)model->animlist + qrnd) + 0xc) & 8) != 0)) {
          bVar4 = true;
          if (((c == player) && ((c->obj).character == 0)) &&
             ((((GameMode == 1 && (iVar7 - 0x25U < 3)) ||
               ((iVar7 == 0x27 &&
                (uVar8 = RotDiff(GameCam.hdg_to_player,(c->obj).hdg),
                (int)(((int)uVar8 >> 0x1f ^ uVar8) - ((int)uVar8 >> 0x1f)) < 0x6000)))) ||
              ((iVar7 == 0x29 &&
               (((((GemPath == 1 || (GemPath == 3)) || (Death == 1)) ||
                 ((Death == 3 || (Bonus == 1)))) || (Bonus == 3)))))))) {
            bVar4 = false;
          }
          if (bVar4) {
            iVar5 = iVar5 + 1;
            *(int *)((int)&local_208 + iVar6) = iVar7;
            iVar6 = iVar6 + 4;
          }
        }
        iVar7 = iVar7 + 1;
        qrnd = qrnd + 4;
      } while (iVar7 < 0x76);
      if (iVar5 < 1) goto LAB_80019374;
      do {
        iVar6 = -1;
        if (iVar5 < 2) {
          qrnd = 0;
        }
        else {
          qrnd = qrand();
          qrnd = qrnd / (0xffff / iVar5 + 1);
        }
        sVar1 = (c->obj).character;
        sVar2 = *(short *)((int)&local_208 + qrnd * 4 + 2);
        c->idle_repeat = '\x01';
        c->idle_action = sVar2;
        if (sVar1 == 0) {
          if (sVar2 == 0x28) {
            qrnd = qrand();
            if (qrnd < 0) {
              qrnd = qrnd + 0x1fff;
            }
            iVar6 = 0x10;
            c->idle_repeat = (char)(qrnd >> 0xd) + '\b';
          }
          else if (0x28 < sVar2) {
            if (sVar2 == 0x29) {
              qrnd = qrand();
              if (qrnd < 0) {
                qrnd = qrnd + 0x3fff;
              }
              c->idle_repeat = (char)(qrnd >> 0xe) + '\x02';
            }
            else if (sVar2 == 0x3d) {
              iVar6 = 0x22;
            }
          }
        }
      } while ((iVar5 >= 2) && (c->idle_action == c->old_idle_action));
    }
    c->idle_mode = '\x01';
    c->old_idle_action = c->idle_action;
    if (('\x01' < c->idle_repeat) && ((model->animlist[c->idle_action]->flags & 1) == 0)) {
      c->idle_repeat = '\x01';
    }
    uStack_2c = (int)c->idle_repeat ^ 0x80000000;
    c->idle_time = 0.0;
    fVar3 = (model->anmdata[c->idle_action]->time - 1.0) *
            (float)((double)CONCAT44(0x43300000,uStack_2c) - 4503601774854144.0);
    c->idle_wait = fVar3;
    uVar8 = (uint)model->animlist[c->idle_action]->blend_out_frames;
    if (uVar8 != 0) {
      uStack_2c = uVar8 ^ 0x80000000;
      fVar3 = fVar3 - (float)((double)CONCAT44(0x43300000,uStack_2c) - 4503601774854144.0) * 0.5;
      c->idle_wait = fVar3;
      if (fVar3 < 1.0) {
        c->idle_wait = 1.0;
      }
    }
    local_30 = 0x43300000;
    if (iVar6 != -1) {
      GameSfx(iVar6,&(c->obj).pos);
    }
    iVar5 = 1;
  }
  return iVar5;
}


void UpdateCharacterIdle(creature_s *c,int character)

{
  int iVar1;
  int i;
  short action;
  float wait;
  
  i = (int)CRemap[character];
  if (i != -1) {
    if (((c->obj).anim.newaction != 0x22) ||
       ((c->spin != '\0' &&
        ((int)c->spin_frame < (int)c->spin_frames - (int)c->OnFootMoveInfo->SPINRESETFRAMES))))
    goto LAB_80019500;
    action = c->idle_action;
    (c->obj).idle_gametime = (c->obj).idle_gametime + 0.01666667;
    iVar1 = action * 4;
    wait = 0.5;
    if (*(int *)(i * 0x7ac + -0x7fdac120 + iVar1) != 0) {
      wait = *(float *)(*(int *)(i * 0x7ac + -0x7fdabf48 + iVar1) + 8) * 0.5;
    }
    wait = c->idle_time + wait;
    c->idle_time = wait;
    if (c->idle_mode == '\0') {
      if (wait <= c->idle_wait) goto LAB_800194f4;
    }
    else {
      if ((c->idle_mode != '\x01') || (wait <= c->idle_wait)) goto LAB_800194f4;
      if (((LBIT._4_4_ & 0x200000a1) == 0) || ((c->obj).character != 0)) {
        c->idle_mode = '\0';
        wait = IDLEWAIT * 30.0;
        c->idle_action = 0x22;
        c->idle_time = 0.0;
        c->idle_wait = wait;
        goto LAB_800194f4;
      }
    }
    i = NewCharacterIdle(c,CModel + i);
    if (i != 0) {
LAB_800194f4:
      (c->obj).anim.newaction = c->idle_action;
      return;
    }
  }
LAB_80019500:
  c->idle_sigh = '\0';
  c->idle_mode = '\0';
  wait = IDLEWAIT * 30.0;
  c->idle_action = 0x22;
  c->idle_wait = wait;
  c->old_idle_action = -1;
  c->idle_time = 0.0;
  (c->obj).idle_gametime = 0.0;
  return;
}


void UpdateRumble(rumble_s *rumble)

{
  if (rumble->buzz != '\0') {
    rumble->buzz = rumble->buzz + 0xff;
  }
  if (rumble->frame == '\0') {
    return;
  }
  rumble->frame = rumble->frame + 0xff;
  return;
}


void NewRumble(rumble_s *rumble,int power)

{
  uchar frame;
  
  if ((rumble->frame != 0) &&
     (power <= (int)(((uint)rumble->power * (uint)rumble->frame) / (uint)rumble->frames))) {
    return;
  }
  rumble->power = (uchar)power;
  frame = (uchar)((uint)(power * 0x3c) >> 8);
  rumble->frame = frame;
  rumble->frames = frame;
  return;
}


void NewBuzz(rumble_s *rumble,int frames)

{
  if (frames <= (int)(uint)rumble->buzz) {
    return;
  }
  rumble->buzz = (uchar)frames;
  return;
}


void ProcessCreatures(void)

{
  float fVar1;
  float fVar2;
  creature_s *c;
  double dVar3;
  double dVar4;
  nuvec_s pos;
  int i;
  float radius;
  
  if (FRAME == 0) {
    tbslotBegin(app_tbset,5);
  }
  i = 8;
  GetTopBot(Character);
  NewTopBot(&Character[0].obj);
  dVar3 = 0.5;
  dVar4 = 1.0;
  c = Character;
  do {
    if (c[1].on != '\0') {
      radius = c[1].obj.SCALE;
      c[1].obj.pos_adjusted = '\0';
      c[1].obj.got_shadow = '\0';
      c[1].obj.old_SCALE = radius;
      MoveCreature(c + 1);
      if ((USELIGHTS != 0) && (LIGHTCREATURES != 0)) {
        pos.x = c[1].obj.pos.x;
        pos.y = (float)((double)((c[1].obj.bot + c[1].obj.top) * c[1].obj.SCALE) * dVar3 +
                       (double)c[1].obj.pos.y);
        pos.z = c[1].obj.pos.z;
        GetLights(&pos,&c[1].lights,1);
      }
      if (c[1].obj.dead == '\x01') {
        radius = (float)(dVar4 - (double)(c[1].obj.die_time / c[1].obj.die_duration)) *
                 c[1].ai.scale;
      }
      else {
        radius = c[1].ai.scale;
      }
      c[1].obj.scale = radius;
      radius = c[1].obj.scale;
      fVar1 = c[1].obj.radius;
      fVar2 = CData[c[1].obj.character].scale;
      c[1].hit_type = '\0';
      radius = radius * fVar2;
      c[1].obj.SCALE = radius;
      c[1].obj.RADIUS = fVar1 * radius;
    }
    i = i + -1;
    c = c + 1;
  } while (i != 0);
  if ((FRAME == 0) && (tbslotEnd(app_tbset,5), FRAME == 0)) {
    tbslotBegin(app_tbset,6);
  }
  if ((PLAYERCOUNT != 0) && (Character[0].on != '\0')) {
    Character[0].obj.got_shadow = '\0';
    Character[0].obj.pos_adjusted = '\0';
    Character[0].obj.old_SCALE = Character[0].obj.SCALE;
    MovePlayer(Character,Pad[0]);
    if ((USELIGHTS != 0) && (LIGHTCREATURES != 0)) {
      pos.x = Character[0].obj.pos.x;
      pos.y = (Character[0].obj.bot + Character[0].obj.top) * Character[0].obj.SCALE * 0.5 +
              Character[0].obj.pos.y;
      pos.z = Character[0].obj.pos.z;
      GetLights(&pos,&Character[0].lights,1);
    }
    i = cRPosCOUNT;
    if ((((level_part_2 == 0) &&
         (ComplexRailPosition(&Character[0].obj.pos,(int)Character[0].obj.RPos.iRAIL,
                              (int)Character[0].obj.RPos.iALONG,&Character[0].obj.RPos,1),
         cRPosCOUNT != i)) && (GameCam.mode != -1)) && (Character[0].obj.transporting == '\0')) {
      BlendGameCamera(&GameCam,0.5);
    }
    if (((Character[0].obj.mask != (mask *)0x0) && ((Character[0].obj.mask)->active != 0)) &&
       ((LDATA->flags & 0xe00) == 0)) {
      UpdateMask(Character[0].obj.mask,&Character[0].obj);
    }
    if (Level == 0x25) {
      HubSelect(Character);
      HubLevelSelect(&Character[0].obj,Hub);
      HubMoveVR();
    }
    else if (Level < 0x23) {
      CheckPlayerEvents(&Character[0].obj);
      CheckFinish(&Character[0].obj);
      CheckGates(&Character[0].obj);
      CheckRings(&Character[0].obj,&i_ring);
    }
    if (Character[0].obj.dead == '\x01') {
      Character[0].obj.scale = 1.0 - Character[0].obj.die_time / Character[0].obj.die_duration;
    }
    else {
      Character[0].obj.scale = 1.0;
    }
    Character[0].obj.SCALE = Character[0].obj.scale * CData[Character[0].obj.character].scale;
    radius = CData[115].radius;
    if (((VEHICLECONTROL != 2) && (radius = Character[0].obj.radius, VEHICLECONTROL == 1)) &&
       (Character[0].obj.vehicle != -1)) {
      radius = CData[Character[0].obj.vehicle].radius;
    }
    Character[0].obj.RADIUS = radius * Character[0].obj.SCALE;
  }
  if (FRAME == 0) {
    tbslotEnd(app_tbset,6);
  }
  return;
}


void EvalModelAnim(CharacterModel *model,anim_s *anim,numtx_s *m,numtx_s *tmtx,float ***dwa,
                  numtx_s *mLOCATOR)

{
  float vtxtime;
  NUPOINTOFINTEREST_s *poi;
  int nlayers;
  float **ppfVar1;
  short *layer;
  nuAnimData_s *vtxanim;
  NUPOINTOFINTEREST_s **pLoc;
  short layertab [2];
  
  layer = layertab;
  layertab = (short  [2])0x1;
  nlayers = 1;
  if (model->character == 0) {
    nlayers = 2;
  }
  if (anim->blend == '\0') {
LAB_8001d488:
    if (((ushort)anim->action < 0x76) &&
       (vtxanim = model->fanmdata[anim->action], vtxanim != (nuAnimData_s *)0x0)) {
      vtxtime = anim->anim_time;
      goto LAB_8001d4b4;
    }
LAB_8001d4c0:
    *dwa = (float **)0x0;
  }
  else {
    if ((((0x75 < (ushort)anim->blend_src_action) ||
         (model->fanmdata[anim->blend_src_action] == (nuAnimData_s *)0x0)) ||
        (0x75 < (ushort)anim->blend_dst_action)) ||
       (model->fanmdata[anim->blend_dst_action] == (nuAnimData_s *)0x0)) {
      if (anim->blend == '\0') goto LAB_8001d488;
      goto LAB_8001d4c0;
    }
    nlayers = 1;
    vtxtime = anim->anim_time;
    layer = (short *)0x0;
    vtxanim = model->fanmdata[anim->action];
LAB_8001d4b4:
    ppfVar1 = NuHGobjEvalDwa(nlayers,layer,vtxanim,vtxtime);
    *dwa = ppfVar1;
  }
  if (anim->blend == '\0') {
LAB_8001d5a0:
    if (((ushort)anim->action < 0x76) && (model->anmdata[anim->action] != (nuAnimData_s *)0x0)) {
      NuHGobjEvalAnim(model->hobj,model->anmdata[anim->action],anim->anim_time,0,
                      (NUJOINTANIM_s *)0x0,tmtx);
      temp_time = anim->anim_time;
      temp_action = (int)anim->action;
      goto LAB_8001d604;
    }
  }
  else {
    if ((ushort)anim->blend_src_action < 0x76) {
      if (((model->anmdata[anim->blend_src_action] != (nuAnimData_s *)0x0) &&
          ((ushort)anim->blend_dst_action < 0x76)) &&
         (model->anmdata[anim->blend_dst_action] != (nuAnimData_s *)0x0)) {
        NuHGobjEvalAnimBlend
                  (model->hobj,model->anmdata[anim->blend_src_action],anim->blend_src_time,
                   model->anmdata[anim->blend_dst_action],anim->blend_dst_time,
                   (float)((double)CONCAT44(0x43300000,(int)anim->blend_frame ^ 0x80000000) -
                          4503601774854144.0) /
                   (float)((double)CONCAT44(0x43300000,(int)anim->blend_frames ^ 0x80000000) -
                          4503601774854144.0),0,(NUJOINTANIM_s *)0x0,tmtx);
        temp_time = anim->blend_dst_time;
        temp_action = (int)anim->blend_dst_action;
        goto LAB_8001d604;
      }
    }
    if (anim->blend == '\0') goto LAB_8001d5a0;
  }
  NuHGobjEval(model->hobj,0,(NUJOINTANIM_s *)0x0,tmtx);
  temp_action = -1;
LAB_8001d604:
  if (mLOCATOR != (numtx_s *)0x0) {
    pLoc = model->pLOCATOR;
    nlayers = 0;
    do {
      poi = *pLoc;
      pLoc = pLoc + 1;
      if (poi != (NUPOINTOFINTEREST_s *)0x0) {
        NuHGobjPOIMtx(model->hobj,(uchar)nlayers,m,tmtx,mLOCATOR);
      }
      nlayers = nlayers + 1;
      mLOCATOR = mLOCATOR + 1;
    } while (nlayers < 0x10);
  }
  return;
}


void StoreLocatorMatrices(CharacterModel *model,numtx_s *mC,numtx_s *tmtx,numtx_s *mtx,nuvec_s *m om)

{
  float fVar1;
  float fVar2;
  float fVar3;
  int size;
  numtx_s *pnVar4;
  int cnt;
  numtx_s *ptr;
  numtx_s *pnVar5;
  int poiId;
  numtx_s m;
  numtx_s *ptr2;
  
  if (mtx != (numtx_s *)0x0) {
    poiId = 0;
    do {
      cnt = poiId + 1;
      if (model->pLOCATOR[poiId] != (NUPOINTOFINTEREST_s *)0x0) {
        ptr = mtx + poiId;
        fVar1 = ptr->_30;
        fVar2 = ptr->_31;
        fVar3 = ptr->_32;
        NuHGobjPOIMtx(model->hobj,(uchar)poiId,mC,tmtx,&m);
        size = 0x30;
        ptr2 = &m;
        do {
          pnVar5 = ptr;
          pnVar4 = ptr2;
          size = size + -0x18;
          pnVar5->_00 = pnVar4->_00;
          pnVar5->_01 = pnVar4->_01;
          pnVar5->_02 = pnVar4->_02;
          pnVar5->_03 = pnVar4->_03;
          pnVar5->_10 = pnVar4->_10;
          pnVar5->_11 = pnVar4->_11;
          ptr2 = (numtx_s *)&pnVar4->_12;
          ptr = (numtx_s *)&pnVar5->_12;
        } while (size != 0);
        pnVar5->_12 = pnVar4->_12;
        pnVar5->_13 = pnVar4->_13;
        pnVar5->_20 = pnVar4->_20;
        pnVar5->_21 = pnVar4->_21;
        if (mom != (nuvec_s *)0x0) {
          mom[poiId].x = mtx[poiId]._30 - fVar1;
          mom[poiId].y = mtx[poiId]._31 - fVar2;
          mom[poiId].z = mtx[poiId]._32 - fVar3;
        }
      }
      poiId = cnt;
    } while (cnt < 0x10);
  }
  return;
}



int DrawCharacterModel(CharacterModel *model,anim_s *anim,numtx_s *mC,numtx_s *mS,int render,
                      numtx_s *mR,numtx_s *loc_mtx,nuvec_s *loc_mom,obj_s *obj)

{
  short sVar1;
  double dVar2;
  float **dwa;
  char cVar3;
  int iVar4;
  int pafVar5;
  int iVar5;
  NUJOINTANIM_s *janim;
  CharacterModel *model2;
  int action;
  double time;
  NUJOINTANIM_s joint [4];
  short *local_70;
  short *local_58;
  uint uStack_54;
  int numjoints;
  
  iVar5 = 0;
  local_70 = (short *)0x1;
  if (jeep_draw == 0) {
    joint[0].rx = (float)((double)CONCAT44(0x43300000,
                                           -(uint)(ushort)(player->obj).target_xrot & 0xffff) -
                         4503599627370496.0) * 9.58738e-05;
    joint[0].tx = 0.0;
    joint[0].sx = 1.0;
    uStack_54 = -(uint)(ushort)(player->obj).target_yrot & 0xffff;
    joint[0].joint_id = (uchar)jointnum;
    joint[0].flags = '\x01';
    local_58 = (short *)0x43300000;
    joint[0].rz = 0.0;
    joint[0].tz = 0.0;
    joint[0].ty = 0.0;
    joint[0].sz = 1.0;
    joint[0].ry = (float)((double)CONCAT44(0x43300000,uStack_54) - 4503599627370496.0) * 9.58738e- 05
    ;
    joint[0].sy = 1.0;
  }
  else {
    numjoints = 4;
    cVar3 = '\0';
    pafVar5 = (int)&joint[0].joint_id;
    iVar4 = 0;
    uStack_54 = ((GameTimer.frame % 0x3c) * 0x10000) / 0x3c;
    local_58 = (short *)0x43300000;
    dVar2 = (double)CONCAT44(0x43300000,uStack_54);
    do {
      *(float *)(joint[0].pad + iVar4 + -0x32) = (float)(dVar2 - 4503599627370496.0) * 9.58738e-05 ;
      *(undefined4 *)(joint[0].pad + iVar4 + -0x2e) = 0;
      *(undefined4 *)(joint[0].pad + iVar4 + -0x2a) = 0;
      *(undefined4 *)(joint[0].pad + iVar4 + -0x1e) = 0;
      *(undefined4 *)(joint[0].pad + iVar4 + -0x22) = 0;
      *(undefined4 *)(joint[0].pad + iVar4 + -0x26) = 0;
      *(undefined4 *)(joint[0].pad + iVar4 + -0x12) = 0x3f800000;
      *(undefined4 *)(joint[0].pad + iVar4 + -0x16) = 0x3f800000;
      *(undefined4 *)(joint[0].pad + iVar4 + -0x1a) = 0x3f800000;
      *(char *)pafVar5 = cVar3;
      iVar4 = iVar4 + 0x34;
      pafVar5 = pafVar5 + 0x34;
      cVar3 = cVar3 + '\x01';
      numjoints = numjoints + -1;
    } while (numjoints != 0);
  }
  if (mC == (numtx_s *)0x0) {
    plr_render = 0;
    jeep_draw = 0;
    return 0;
  }
  iVar4 = 1;
  if (model->character == 0) {
    iVar4 = 2;
  }
  if (anim == (anim_s *)0x0) {
LAB_8001dac4:
    dwa = (float **)0x0;
  }
  else if (anim->blend == '\0') {
LAB_8001da88:
    if ((0x75 < (ushort)anim->action) || (model->fanmdata[anim->action] == (nuAnimData_s *)0x0))
    goto LAB_8001dac4;
    dwa = NuHGobjEvalDwa(1,(void *)0x0,model->fanmdata[anim->action],anim->anim_time);
  }
  else {
    if ((((0x75 < (ushort)anim->blend_src_action) ||
         (model->fanmdata[anim->blend_src_action] == (nuAnimData_s *)0x0)) ||
        (0x75 < (ushort)anim->blend_dst_action)) ||
       (model->fanmdata[anim->blend_dst_action] == (nuAnimData_s *)0x0)) {
      if (anim->blend != '\0') goto LAB_8001dac4;
      goto LAB_8001da88;
    }
    uStack_54 = (int)anim->blend_frames ^ 0x80000000;
    local_58 = (short *)0x43300000;
    dwa = NuHGobjEvalDwaBlend(iVar4,(short *)&local_70,model->fanmdata[anim->blend_src_action],
                              anim->blend_src_time,model->fanmdata[anim->blend_dst_action],
                              anim->blend_dst_time,
                              (float)((double)CONCAT44(0x43300000,
                                                       (int)anim->blend_frame ^ 0x80000000) -
                                     4503601774854144.0) /
                              (float)((double)CONCAT44(0x43300000,uStack_54) - 4503601774854144.0) );
  }
  model2 = model;
  if (model->character == 0x54) {
    cVar3 = CRemap[0];
    if ((LBIT._0_4_ & 4 | LBIT._4_4_ & 0x40) != 0) {
      cVar3 = CRemap[115];
    }
LAB_8001db18:
    if (cVar3 != -1) {
      model2 = CModel + cVar3;
    }
  }
  else {
    cVar3 = CRemap[8];
    if (model->character == 0x9f) goto LAB_8001db18;
  }
  janim = (NUJOINTANIM_s *)0x0;
  numjoints = 0;
  if ((((jeep_draw == 0) && (plr_render != 0)) &&
      ((player->target != '\0' && ((VEHICLECONTROL != 1 || ((player->obj).vehicle == -1)))))) &&
     ((sVar1 = model2->character, sVar1 == 0 || ((sVar1 == 0x54 || (sVar1 == 0x8c)))))) {
    janim = joint;
    numjoints = 1;
  }
  if (ChrisJointOveride != 0) {
    janim = ChrisJointList;
    numjoints = ChrisNumJoints;
  }
  if (anim != (anim_s *)0x0) {
    if (anim->blend != '\0') {
      if (((((ushort)anim->blend_src_action < 0x76) &&
           (model2->anmdata[anim->blend_src_action] != (nuAnimData_s *)0x0)) &&
          ((ushort)anim->blend_dst_action < 0x76)) &&
         (model2->anmdata[anim->blend_dst_action] != (nuAnimData_s *)0x0)) {
        uStack_54 = (int)anim->blend_frames ^ 0x80000000;
        local_58 = (short *)0x43300000;
        NuHGobjEvalAnimBlend
                  (model2->hobj,model2->anmdata[anim->blend_src_action],anim->blend_src_time,
                   model2->anmdata[anim->blend_dst_action],anim->blend_dst_time,
                   (float)((double)CONCAT44(0x43300000,(int)anim->blend_frame ^ 0x80000000) -
                          4503601774854144.0) /
                   (float)((double)CONCAT44(0x43300000,uStack_54) - 4503601774854144.0),numjoints,
                   janim,tmtx);
        time = (double)anim->blend_dst_time;
        action = (int)anim->blend_dst_action;
        goto LAB_8001dcf8;
      }
      if (anim->blend != '\0') goto LAB_8001dcdc;
    }
    if (((ushort)anim->action < 0x76) && (model2->anmdata[anim->action] != (nuAnimData_s *)0x0)) {
      NuHGobjEvalAnim(model2->hobj,model2->anmdata[anim->action],anim->anim_time,numjoints,janim,
                      tmtx);
      time = (double)anim->anim_time;
      action = (int)anim->action;
      goto LAB_8001dcf8;
    }
  }
LAB_8001dcdc:
  action = -1;
  NuHGobjEval(model2->hobj,numjoints,janim,tmtx);
LAB_8001dcf8:
  if (glass_draw == 0) {
    StoreLocatorMatrices(model2,mC,tmtx,loc_mtx,loc_mom);
  }
  if (((action != -1) && (loc_mtx != (numtx_s *)0x0)) && ((Paused == 0 && (glass_draw == 0)))) {
    AddAnimDebris(model,loc_mtx,action,(float)time,obj);
  }
  if (render != 0) {
    if (((plr_render != 0) && (model->character == 0)) && ((player->obj).dead == '\x12')) {
      iVar4 = 1;
    }
    iVar5 = NuHGobjRndrMtxDwa(model->hobj,mC,iVar4,(short *)&local_70,tmtx,dwa);
    if (((iVar5 != 0) && (obj != (obj_s *)0x0)) && (obj->character == 0xb1)) {
      DrawProbeFX(obj);
    }
    if (mR != (numtx_s *)0x0) {
      NuHGobjRndrMtxDwa(model->hobj,mR,iVar4,(short *)&local_70,tmtx,dwa);
    }
    if (mS != (numtx_s *)0x0) {
      NuMtlSetStencilRender(NUSTENCIL_REPLACE_NODRAW);
      NuHGobjRndrMtx(model->hobj,mS,iVar4,(short *)&local_70,tmtx);
      NuMtlSetStencilRender(NUSTENCIL_NOSTENCIL);
    }
  }
  jeep_draw = 0;
  plr_render = 0;
  return iVar5;
}



void UpdateAnimPacket(CharacterModel *mod,anim_s *anim,float dt,float xz_distance)

{
  byte bVar1;
  float fVar2;
  float fVar3;
  char cVar4;
  byte bVar5;
  short sVar8;
  int iVar6;
  uint uVar7;
  int iVar9;
  
  if (mod == (CharacterModel *)0x0) {
    return;
  }
  if (anim == (anim_s *)0x0) {
    return;
  }
  cVar4 = anim->blend;
  if (cVar4 == '\0') {
    sVar8 = anim->newaction;
    iVar9 = (int)sVar8;
    iVar6 = (int)anim->oldaction;
    if (iVar9 == iVar6) {
      anim->action = sVar8;
    }
    else {
      if ((((iVar6 != -1) && (iVar9 != -1)) && (mod->anmdata[iVar6] != (nuAnimData_s *)0x0)) &&
         (((mod->anmdata[iVar9] != (nuAnimData_s *)0x0 &&
           (1 < mod->animlist[iVar6]->blend_out_frames)) &&
          (1 < mod->animlist[iVar9]->blend_in_frames)))) {
        anim->blend_dst_action = sVar8;
        anim->blend = '\x01';
        anim->blend_src_action = anim->oldaction;
        anim->blend_src_time = anim->anim_time;
        if ((((mod->animlist[iVar6]->flags & 1) == 0) || ((mod->animlist[iVar9]->flags & 1) == 0))
           || ((mod->animlist[iVar6]->speed != mod->animlist[iVar9]->speed ||
               (mod->anmdata[iVar6]->time != mod->anmdata[iVar9]->time)))) {
          anim->blend_dst_time = 1.0;
        }
        else {
          anim->blend_dst_time = anim->anim_time;
        }
        if (((mod->character == 0) && (PLAYERCOUNT != 0)) && (player->used != '\0')) {
          if (anim->blend_dst_action == 3) {
            anim->blend_dst_time =
                 ((float)((double)CONCAT44(0x43300000,(int)player->crouch_pos ^ 0x80000000) -
                         4503601774854144.0) * (mod->anmdata[3]->time - 1.0)) /
                 (float)((double)CONCAT44(0x43300000,
                                          (int)player->OnFootMoveInfo->CROUCHINGFRAMES ^ 0x8000000 0)
                        - 4503601774854144.0);
          }
          else if (anim->blend_dst_action == 5) {
            uVar7 = (uint)player->OnFootMoveInfo->CROUCHINGFRAMES;
            anim->blend_dst_time =
                 ((float)((double)CONCAT44(0x43300000,uVar7 - (int)player->crouch_pos ^ 0x80000000 )
                         - 4503601774854144.0) * (mod->anmdata[5]->time - 1.0)) /
                 (float)((double)CONCAT44(0x43300000,uVar7 ^ 0x80000000) - 4503601774854144.0);
          }
        }
        anim->blend_frame = 0;
        bVar5 = mod->animlist[anim->newaction]->blend_in_frames;
        anim->blend_frames = (ushort)bVar5;
        bVar1 = mod->animlist[anim->oldaction]->blend_out_frames;
        if (bVar1 < bVar5) {
          anim->blend_frames = (ushort)bVar1;
        }
        goto LAB_8001fcbc;
      }
      cVar4 = '\0';
      anim->action = sVar8;
      anim->anim_time = 1.0;
    }
    anim->blend = cVar4;
  }
  else {
    sVar8 = anim->blend_frame + 1;
    anim->blend_frame = sVar8;
    if (sVar8 == anim->blend_frames) {
      anim->blend = '\0';
      anim->action = anim->blend_dst_action;
      anim->anim_time = anim->blend_dst_time;
    }
  }
LAB_8001fcbc:
  anim->flags = '\0';
  if (anim->blend == '\0') {
    sVar8 = anim->action;
    if (mod->anmdata[sVar8] == (nuAnimData_s *)0x0) {
      return;
    }
    fVar3 = dt * mod->animlist[sVar8]->speed;
    if ((mod->animlist[sVar8]->flags & 0x10) != 0) {
      fVar3 = fVar3 * xz_distance * 10.0;
    }
    fVar3 = anim->anim_time + fVar3;
    anim->anim_time = fVar3;
    fVar2 = mod->anmdata[sVar8]->time;
    if (fVar3 <= fVar2) {
      return;
    }
    if ((mod->animlist[sVar8]->flags & 1) != 0) {
      anim->flags = '\x02';
      anim->anim_time = fVar3 - (fVar2 - 1.0);
      return;
    }
    bVar5 = 1;
    anim->anim_time = fVar2;
  }
  else {
    sVar8 = anim->blend_src_action;
    if (mod->anmdata[sVar8] == (nuAnimData_s *)0x0) {
      return;
    }
    if (mod->anmdata[anim->blend_dst_action] == (nuAnimData_s *)0x0) {
      return;
    }
    fVar3 = dt * mod->animlist[sVar8]->speed;
    if ((mod->animlist[sVar8]->flags & 0x10) != 0) {
      fVar3 = fVar3 * xz_distance * 10.0;
    }
    fVar3 = anim->blend_src_time + fVar3;
    anim->blend_src_time = fVar3;
    fVar2 = mod->anmdata[sVar8]->time;
    if (fVar2 < fVar3) {
      if ((mod->animlist[sVar8]->flags & 1) == 0) {
        anim->blend_src_time = fVar2;
      }
      else {
        anim->blend_src_time = fVar3 - (fVar2 - 1.0);
      }
    }
    sVar8 = anim->blend_dst_action;
    fVar3 = dt * mod->animlist[sVar8]->speed;
    if ((mod->animlist[sVar8]->flags & 0x10) != 0) {
      fVar3 = fVar3 * xz_distance * 10.0;
    }
    fVar3 = anim->blend_dst_time + fVar3;
    anim->blend_dst_time = fVar3;
    fVar2 = mod->anmdata[sVar8]->time;
    if (fVar3 <= fVar2) {
      return;
    }
    if ((mod->animlist[sVar8]->flags & 1) != 0) {
      anim->flags = anim->flags | 2;
      anim->blend_dst_time = fVar3 - (fVar2 - 1.0);
      return;
    }
    anim->blend_dst_time = fVar2;
    bVar5 = anim->flags | 1;
  }
  anim->flags = bVar5;
  return;
}


void DrawCreatures(creature_s *c,int count,int render,int shadow)

{
  bool bVar1;
  bool bVar2;
  bool bVar3;
  float fVar4;
  char cVar5;
  ushort uVar6;
  short sVar7;
  float fVar8;
  bool bVar9;
  bool bVar10;
  bool bVar11;
  int iVar12;
  float *pfVar13;
  float *pfVar14;
  nuAnimData_s *pnVar15;
  numtx_s *pnVar16;
  numtx_s *pnVar17;
  uint uVar18;
  numtx_s *pnVar19;
  numtx_s *pnVar20;
  int iVar21;
  uint uVar22;
  uint uVar23;
  nuvec_s *loc_mom;
  short sVar24;
  int iVar25;
  int iVar26;
  byte bVar27;
  byte bVar28;
  double dVar29;
  double dVar30;
  double dVar31;
  double dVar32;
  double dVar33;
  double dVar34;
  nuvec_s local_1e8;
  numtx_s local_1d8;
  numtx_s local_198;
  numtx_s nStack_158;
  numtx_s local_118;
  CharacterModel *Cmod [2];
  numtx_s local_d0;
  
  if ((((((DRAWCREATURESHADOWS == 0) || (Level == 0x1d)) || (Level == 0x24)) ||
       ((Level == 0x1e && (level_part_2 != 0)))) || (Level == 0x1a)) ||
     ((((LDATA->flags & 0x1000) != 0 || (VEHICLECONTROL == 2)) ||
      ((local_d0._02 = (float)shadow, VEHICLECONTROL == 1 && ((player->obj).vehicle == 0x20))))))  {
    local_d0._02 = 0.0;
  }
  if (((LDATA->flags & 0x202) != 0) || (fVar4 = AIVISRANGE, Level == 0x1c)) {
    fVar4 = (float)((double)CONCAT44(0x43300000,LDATA->farclip ^ 0x80000000) - 4503601774854144.0 );
  }
  if ((float)((double)CONCAT44(0x43300000,LDATA->farclip ^ 0x80000000) - 4503601774854144.0) < fVa r4
     ) {
    fVar4 = (float)((double)CONCAT44(0x43300000,(uint)LDATA->farclip) - 4503599627370496.0);
  }
  iVar25 = 0;
  dVar33 = (double)(fVar4 * fVar4);
  bVar1 = render == 0;
  if (0 < count) {
    dVar31 = 1.0;
    dVar32 = 2000000.0;
    dVar29 = 0.05000000074505806;
    dVar30 = 0.0;
    local_d0._00 = (float)count;
    local_d0._01 = (float)render;
    do {
      uVar23 = (c->obj).flags & 1;
      if (uVar23 == 0) {
LAB_8001e064:
        sVar24 = -1;
LAB_8001e068:
        cVar5 = c->used;
        uVar22 = (c->obj).draw_frame;
        (c->obj).draw_frame = 0;
        if ((((cVar5 != '\0') && (c->on != '\0')) && ((c->obj).model != (CharacterModel *)0x0)) &&
           (((cVar5 = (c->obj).dead, cVar5 != '\x16' && (cVar5 != '\x04')) && (cVar5 != '\a')))) {
          if (Level == 0x17) {
            if (glass_phase == 0) {
              if ((c->obj).character != 0x7f) goto LAB_8001e0f4;
            }
            else if ((c->obj).character == 0x7f) goto LAB_8001e0f4;
          }
          else {
LAB_8001e0f4:
            if (((((c->obj).invisible == '\0') || ((c->obj).character == 0x77)) &&
                ((bVar27 = (c->obj).invincible, bVar27 == 0 || (1 < (bVar27 & 3))))) &&
               ((fVar4 = *(float *)(pCam + 0xac) - (c->obj).pos.z,
                fVar8 = *(float *)(pCam + 0xa4) - (c->obj).pos.x,
                dVar34 = (double)(fVar8 * fVar8 + fVar4 * fVar4), (LDATA->flags & 0x200) != 0 ||
                (dVar34 <= dVar33)))) {
              bVar9 = false;
              bVar10 = false;
              bVar11 = false;
              bVar2 = sVar24 == 99;
              if (((bVar2) || (((sVar24 == 0x36 || (sVar24 == 0x81)) || (sVar24 == 0x53)))) ||
                 (sVar24 == 0x8b)) {
                if (bVar1) goto LAB_8001f958;
                SetCreatureLights(c);
                if (bVar2) {
                  pfVar13 = (float *)DrawPlayerJeep(c);
                  (c->obj).draw_frame = uVar22 + 1;
                  if (pfVar13 == (float *)0x0) goto LAB_8001f958;
                  iVar12 = 0x30;
                  pnVar20 = &local_1d8;
                  do {
                    pnVar16 = pnVar20;
                    pfVar14 = pfVar13;
                    iVar12 = iVar12 + -0x18;
                    pnVar16->_00 = *pfVar14;
                    pnVar16->_01 = pfVar14[1];
                    pnVar16->_02 = pfVar14[2];
                    pnVar16->_03 = pfVar14[3];
                    pnVar16->_10 = pfVar14[4];
                    pnVar16->_11 = pfVar14[5];
                    pfVar13 = pfVar14 + 6;
                    pnVar20 = (numtx_s *)&pnVar16->_12;
                  } while (iVar12 != 0);
                  bVar9 = true;
                  pnVar16->_12 = pfVar14[6];
                  pnVar16->_13 = pfVar14[7];
                  pnVar16->_20 = pfVar14[8];
                  pnVar16->_21 = pfVar14[9];
                }
                else {
                  if (sVar24 < 100) {
                    if (sVar24 != 0x36) {
                      if (sVar24 == 0x53) {
                        ForceShader = 0x80;
                        DrawAtlas(c);
                        bVar9 = true;
                        ForceShader = 0xffffffff;
                        (c->obj).draw_frame = uVar22 + 1;
                        NuMtxSetRotationY(&local_1d8,(uint)(c->obj).hdg);
                        NuMtxTranslate(&local_1d8,&(c->obj).pos);
                        local_1d8._31 = local_1d8._31 + ATLASPLAYERLIFT;
                      }
                      goto LAB_8001e374;
                    }
                  }
                  else if (sVar24 == 0x81) {
                    ForceShader = 0x80;
                  }
                  else if (sVar24 != 0x8b) goto LAB_8001e374;
                  DrawGlider(c);
                  ForceShader = 0xffffffff;
                  (c->obj).draw_frame = uVar22 + 1;
                  iVar12 = 0x30;
                  pnVar20 = &mTEMP;
                  pnVar16 = &local_1d8;
                  do {
                    pnVar19 = pnVar16;
                    pnVar17 = pnVar20;
                    iVar12 = iVar12 + -0x18;
                    pnVar19->_00 = pnVar17->_00;
                    pnVar19->_01 = pnVar17->_01;
                    pnVar19->_02 = pnVar17->_02;
                    pnVar19->_03 = pnVar17->_03;
                    pnVar19->_10 = pnVar17->_10;
                    pnVar19->_11 = pnVar17->_11;
                    pnVar20 = (numtx_s *)&pnVar17->_12;
                    pnVar16 = (numtx_s *)&pnVar19->_12;
                  } while (iVar12 != 0);
                  bVar9 = true;
                  pnVar19->_12 = pnVar17->_12;
                  pnVar19->_13 = pnVar17->_13;
                  pnVar19->_20 = pnVar17->_20;
                  pnVar19->_21 = pnVar17->_21;
                }
              }
LAB_8001e374:
              bVar3 = uVar23 == 0;
              bVar27 = bVar3 << 1;
              if (bVar3) {
LAB_8001ecd4:
                sVar7 = (c->obj).character;
                if ((sVar7 == 0x77) && ((c->obj).invisible != '\0')) {
                  if (!bVar1) {
                    Draw3DCrateCount(&(c->obj).pos,(c->obj).hdg);
                    goto LAB_8001ed04;
                  }
                  goto LAB_8001f958;
                }
                uVar23 = (c->obj).hdg - 0x8000 & 0xffff;
                if (!bVar3) {
                  cVar5 = (c->obj).dead;
                  if ((cVar5 == '\x03') || (cVar5 == '\b')) {
                    uVar23 = uVar23 - 0x8000;
LAB_8001edf8:
                    uVar23 = uVar23 & 0xffff;
                  }
                  else if (c->freeze == '\0') {
                    if ((((c->spin != '\0') &&
                         ((int)c->spin_frame <
                          (int)c->spin_frames - (int)c->OnFootMoveInfo->SPINRESETFRAMES)) &&
                        (sVar24 == 0x3b)) &&
                       (((c->obj).anim.newaction == 0x69 &&
                        (((c->obj).model)->anmdata[0x69] != (nuAnimData_s *)0x0)))) {
                      uVar23 = uVar23 - ((int)c->spin_frame << 0x10) / (int)GyroMoveInfo.SPINFRAME S;
                      goto LAB_8001edf8;
                    }
                  }
                  else {
                    uVar23 = (uint)GameCam.hdg_to_player;
                  }
                }
                bVar28 = !bVar9 << 1;
                if (!bVar9) {
                  if (((bVar3) && ((c->obj).vehicle == 0xa1)) ||
                     ((VEHICLECONTROL == 2 ||
                      (((sVar24 == 0x20 || (sVar24 == 0x89)) || (sVar24 == 0xa1)))))) {
                    local_1e8.x = (c->obj).SCALE;
                    local_1e8.y = local_1e8.x;
                    local_1e8.z = local_1e8.x;
                    NuMtxSetScale(&local_198,&local_1e8);
                    NuMtxRotateZ(&local_198,(uint)(c->obj).zrot);
                    if (sVar24 == 0x20) {
                      iVar12 = RotDiff(0,(c->obj).xrot);
                      if (iVar12 < 0) {
                        iVar12 = iVar12 + 3;
                      }
                      uVar18 = iVar12 >> 2;
                    }
                    else {
                      uVar18 = (uint)(c->obj).xrot;
                    }
                    NuMtxRotateX(&local_198,uVar18);
                    NuMtxRotateY(&local_198,uVar23);
                    NuMtxTranslate(&local_198,&(c->obj).pos);
                    pnVar16 = &c->m;
                    pnVar20 = &local_198;
                    iVar12 = 0x30;
                    do {
                      iVar12 = iVar12 + -0x18;
                      pnVar16->_00 = pnVar20->_00;
                      pnVar16->_01 = pnVar20->_01;
                      pnVar16->_02 = pnVar20->_02;
                      pnVar16->_03 = pnVar20->_03;
                      pnVar16->_10 = pnVar20->_10;
                      pfVar13 = &pnVar20->_11;
                      pnVar20 = (numtx_s *)&pnVar20->_12;
                      pnVar16->_11 = *pfVar13;
                      pnVar16 = (numtx_s *)&pnVar16->_12;
                    } while (iVar12 != 0);
                  }
                  else {
                    if ((Level == 0x17) && (sVar7 == 0x7f)) {
                      uVar18 = ((GameTimer.frame % 0x24) * 0x10000) / 0x24 & 0xffff;
                      local_1e8.z = (float)((double)NuTrigTable[uVar18] * dVar29 + dVar31);
                      local_1e8.x = local_1e8.z * (c->obj).SCALE;
                      if (local_1e8.x < 0.0) {
                        local_1e8.x = 0.0;
                      }
                      local_1e8.y = (float)((double)*(float *)((int)NuTrigTable +
                                                              ((uVar18 + 0x4000) * 4 & 0x3fffc)) *
                                            dVar29 + dVar31) * (c->obj).SCALE;
                      if (local_1e8.y < 0.0) {
                        local_1e8.y = 0.0;
                      }
                      local_1e8.z = local_1e8.z * (c->obj).SCALE;
                      if (local_1e8.z < 0.0) {
                        local_1e8.z = 0.0;
                      }
                    }
                    else {
                      local_1e8.x = (c->obj).SCALE;
                      local_1e8.y = local_1e8.x;
                      local_1e8.z = local_1e8.x;
                    }
                    if (((c->obj).flags & 0x10000) != 0) {
                      local_1e8.y = -local_1e8.y;
                    }
                    NuMtxSetScale(&local_198,&local_1e8);
                    NuMtxRotateY(&local_198,uVar23);
                    NuMtxRotateZ(&local_198,(uint)(c->obj).zrot);
                    NuMtxRotateX(&local_198,(uint)(c->obj).xrot);
                    NuMtxTranslate(&local_198,&(c->obj).pos);
                    pnVar16 = &c->m;
                    iVar12 = 0x30;
                    pnVar20 = &local_198;
                    do {
                      iVar12 = iVar12 + -0x18;
                      pnVar16->_00 = pnVar20->_00;
                      pnVar16->_01 = pnVar20->_01;
                      pnVar16->_02 = pnVar20->_02;
                      pnVar16->_03 = pnVar20->_03;
                      pnVar16->_10 = pnVar20->_10;
                      pfVar13 = &pnVar20->_11;
                      pnVar20 = (numtx_s *)&pnVar20->_12;
                      pnVar16->_11 = *pfVar13;
                      pnVar16 = (numtx_s *)&pnVar16->_12;
                    } while (iVar12 != 0);
                  }
                  pnVar16->_00 = pnVar20->_00;
                  *(float *)((int)pnVar16 + 4) = *(float *)((int)pnVar20 + 4);
                  *(float *)((int)pnVar16 + 8) = *(float *)((int)pnVar20 + 8);
                  *(float *)((int)pnVar16 + 0xc) = *(float *)((int)pnVar20 + 0xc);
                }
                else {
                  iVar12 = 0x30;
                  pnVar20 = &local_1d8;
                  pnVar16 = &local_198;
                  do {
                    pnVar19 = pnVar16;
                    pnVar17 = pnVar20;
                    iVar12 = iVar12 + -0x18;
                    pnVar19->_00 = pnVar17->_00;
                    pnVar19->_01 = pnVar17->_01;
                    pnVar19->_02 = pnVar17->_02;
                    pnVar19->_03 = pnVar17->_03;
                    pnVar19->_10 = pnVar17->_10;
                    pnVar19->_11 = pnVar17->_11;
                    pnVar20 = (numtx_s *)&pnVar17->_12;
                    pnVar16 = (numtx_s *)&pnVar19->_12;
                  } while (iVar12 != 0);
                  iVar12 = 0x30;
                  pnVar19->_12 = pnVar17->_12;
                  pnVar19->_13 = pnVar17->_13;
                  pnVar19->_20 = pnVar17->_20;
                  pnVar19->_21 = pnVar17->_21;
                  pnVar20 = &c->m;
                  pnVar16 = &local_198;
                  do {
                    pnVar19 = pnVar16;
                    pnVar17 = pnVar20;
                    iVar12 = iVar12 + -0x18;
                    pnVar17->_00 = pnVar19->_00;
                    pnVar17->_01 = pnVar19->_01;
                    pnVar17->_02 = pnVar19->_02;
                    pnVar17->_03 = pnVar19->_03;
                    pnVar17->_10 = pnVar19->_10;
                    pnVar17->_11 = pnVar19->_11;
                    pnVar20 = (numtx_s *)&pnVar17->_12;
                    pnVar16 = (numtx_s *)&pnVar19->_12;
                  } while (iVar12 != 0);
                  pnVar17->_12 = pnVar19->_12;
                  pnVar17->_13 = pnVar19->_13;
                  pnVar17->_20 = pnVar19->_20;
                  pnVar17->_21 = pnVar19->_21;
                }
                if (!bVar1) {
                  if ((((double)(c->obj).reflect_y != dVar32) &&
                      ((c != player || ((c->obj).dead != '\x02')))) && (glass_draw == 0)) {
                    iVar12 = 0x30;
                    pnVar20 = &local_118;
                    pnVar16 = &local_198;
                    do {
                      pnVar19 = pnVar16;
                      pnVar17 = pnVar20;
                      iVar12 = iVar12 + -0x18;
                      pnVar17->_00 = pnVar19->_00;
                      pnVar17->_01 = pnVar19->_01;
                      pnVar17->_02 = pnVar19->_02;
                      pnVar17->_03 = pnVar19->_03;
                      pnVar17->_10 = pnVar19->_10;
                      pnVar17->_11 = pnVar19->_11;
                      pnVar20 = (numtx_s *)&pnVar17->_12;
                      pnVar16 = (numtx_s *)&pnVar19->_12;
                    } while (iVar12 != 0);
                    pnVar17->_12 = pnVar19->_12;
                    pnVar17->_13 = pnVar19->_13;
                    pnVar17->_20 = pnVar19->_20;
                    pnVar17->_21 = pnVar19->_21;
                    local_118._01 = -local_118._01;
                    local_118._11 = -local_118._11;
                    local_118._21 = -local_118._21;
                    fVar4 = HUBREFLECTY;
                    if (Level != 0x25) {
                      fVar4 = (c->obj).reflect_y;
                    }
                    bVar11 = true;
                    local_118._31 = fVar4 - (local_118._31 - fVar4);
                  }
                  if ((((((!bVar1) && (local_d0._02 != 0.0)) &&
                        (fVar4 = (c->obj).shadow, (double)fVar4 != dVar32)) &&
                       ((dVar34 < dVar33 && ((TerSurface[(c->obj).surface_type].flags & 1) == 0))) )
                      && ((c->freeze == '\0' &&
                          ((glass_draw == 0 && (((c->obj).flags & 0x4000) == 0)))))) &&
                     ((((bool)(bVar28 >> 1 & 1) &&
                       ((((c->obj).dead != '\b' &&
                         ((((bool)(bVar27 >> 1 & 1) || (SKELETALCRASH == 0)) && (sVar24 != 0xa1))))
                        && ((sVar24 != 0x89 && (!bVar2)))))) &&
                      ((!(bool)(bVar27 >> 1 & 1) || ((c->obj).vehicle != 0xa1)))))) {
                    ScaleFlatShadow(&local_1e8,(c->obj).pos.y,fVar4,(c->obj).SCALE);
                    NuMtxSetScale(&nStack_158,&local_1e8);
                    NuMtxRotateY(&nStack_158,uVar23);
                    NuMtxRotateZ(&nStack_158,(uint)(c->obj).surface_zrot);
                    NuMtxRotateX(&nStack_158,(uint)(c->obj).surface_xrot);
                    nStack_158._30 = (c->obj).pos.x;
                    dVar34 = 0.02500000037252903;
                    nStack_158._31 = (c->obj).shadow + 0.025;
                    nStack_158._32 = (c->obj).pos.z;
                    if (sVar24 == 0x99) {
                      if (Level == 3) {
                        dVar34 = dVar29;
                      }
                      nStack_158._31 = (float)((double)nStack_158._31 + dVar34);
                    }
                    temp_surface_xrot = (c->obj).surface_xrot;
                    bVar10 = true;
                    temp_surface_yrot = (c->obj).hdg;
                    temp_surface_zrot = (c->obj).surface_zrot;
                  }
                }
                Cmod[0] = (c->obj).model;
                if ((((c->obj).dead == '\0') || (iVar12 = (int)(c->obj).die_model[0], iVar12 == -1 ))
                   || (iVar12 == CRemap[(c->obj).character])) {
                  if (!(bool)(bVar27 >> 1 & 1)) {
                    if (VEHICLECONTROL == 2) {
                      iVar12 = (int)CRemap[115];
                      goto LAB_8001f480;
                    }
                    goto LAB_8001f488;
                  }
                }
                else {
LAB_8001f480:
                  Cmod[0] = CModel + iVar12;
LAB_8001f488:
                  if (((!(bool)(bVar27 >> 1 & 1)) &&
                      (((Cmod[0]->character == 0 || (Cmod[0]->character == 0x73)) &&
                       (CRemap[84] != -1)))) &&
                     ((SKELETALCRASH != 0 ||
                      (((c->obj).dead == '\x11' && (GameTimer.frame % 0xc < 6)))))) {
                    Cmod[0] = CModel + CRemap[84];
                  }
                }
                Cmod[1] = (CharacterModel *)0x0;
                if ((((c->obj).dead == '\0') || (iVar12 = (int)(c->obj).die_model[1], iVar12 == -1 ))
                   || ((iVar12 == CRemap[(c->obj).character] || (iVar12 == (c->obj).die_model[0])) ))
                {
                  sVar7 = (c->obj).character;
                  iVar12 = -1;
                  if (sVar7 == 0x11) {
                    iVar12 = 0x12;
                  }
                  else if ((sVar7 == 0x24) && ((c->obj).anim.newaction == 0)) {
                    iVar12 = 0x87;
                  }
                  else if (sVar7 == 0x6d) {
                    iVar12 = 0x3d;
                  }
                  else {
                    cVar5 = (c->obj).dead;
                    if (((cVar5 == '\0') && (sVar7 == 0)) &&
                       ((ushort)(c->obj).anim.newaction - 0x25 < 2)) {
                      iVar12 = 0x45;
                    }
                    else if ((c->freeze == '\0') || (cVar5 != '\0')) {
                      if ((((bool)(bVar27 >> 1 & 1)) || ((cVar5 != '\0' || (c->target == '\0')))) | |
                         (sVar7 != 0)) {
                        if (sVar24 != -1) goto LAB_8001f634;
LAB_8001f6cc:
                        if (((bool)(bVar27 >> 1 & 1)) && ((c->obj).vehicle == 0xa1)) {
LAB_8001f6dc:
                          iVar12 = 0xa1;
                        }
                      }
                      else if (sVar24 == -1) {
                        iVar12 = 0x8c;
                      }
                      else {
LAB_8001f634:
                        if (vtog_time != vtog_duration) goto LAB_8001f6cc;
                        if (sVar24 == 0x6b) {
                          iVar12 = 0x6b;
                        }
                        else if (sVar24 == 0xa0) {
                          iVar12 = 0xa0;
                        }
                        else if (sVar24 == 0x44) {
                          iVar12 = 0x44;
                        }
                        else if (sVar24 == 0xb2) {
                          iVar12 = 0xb2;
                        }
                        else if (sVar24 == 0x3b) {
                          iVar12 = 0x3b;
                        }
                        else if (sVar24 == 0x20) {
                          iVar12 = 0x20;
                        }
                        else if (sVar24 == 0x89) {
                          iVar12 = 0x89;
                        }
                        else {
                          if (sVar24 == 0xa1) goto LAB_8001f6dc;
                          if (sVar24 == 0x99) {
                            iVar12 = 0x99;
                          }
                        }
                      }
                    }
                    else {
                      iVar12 = 0x4f;
                    }
                  }
                  if ((iVar12 != -1) && (iVar12 = (int)CRemap[iVar12], iVar12 != -1))
                  goto LAB_8001f704;
                }
                else {
LAB_8001f704:
                  Cmod[1] = CModel + iVar12;
                }
                if (!bVar1) {
                  SetCreatureLights(c);
                }
                dVar34 = 1.298701286315918;
                iVar26 = 0;
                loc_mom = (nuvec_s *)c->momLOCATOR;
                pnVar20 = (numtx_s *)c->mtxLOCATOR;
                iVar12 = 0;
                do {
                  if ((*(int *)((int)Cmod + iVar12) != 0) &&
                     (((c->obj).anim.blend == '\0' ||
                      (((((sVar24 = *(short *)(*(int *)((int)Cmod + iVar12) + 0x768), sVar24 != 0x 45
                          && (sVar24 != 0x8c)) && (sVar24 != 0xa0)) && (sVar24 != 0x6b)) ||
                       ((iVar21 = *(int *)((int)Cmod + iVar12) + 4,
                        *(int *)(iVar21 + (c->obj).anim.blend_src_action * 4) != 0 &&
                        (*(int *)(iVar21 + (c->obj).anim.blend_dst_action * 4) != 0)))))))) {
                    bVar28 = !bVar11 << 1;
                    if ((Level == 0x1c) && ((iVar26 == 1 && (Cmod[0]->character == 0x7f)))) {
                      local_1e8.z = (float)dVar34;
                      local_1e8.y = (float)dVar34;
                      local_1e8.x = (float)dVar34;
                      NuMtxPreScale(&local_198,&local_1e8);
                      if (bVar10) {
                        NuMtxPreScale(&nStack_158,&local_1e8);
                      }
                      if (bVar11) {
                        NuMtxPreScale(&local_118,&local_1e8);
                      }
                    }
                    if (!(bool)(bVar27 >> 1 & 1)) {
                      plr_render = 1;
                    }
                    if ((!bVar1) && (*(ushort *)(*(int *)((int)Cmod + iVar12) + 0x768) - 0xaf < 2) )
                    {
                      SetLevelLights();
                    }
                    if ((*(CharacterModel **)((int)Cmod + iVar12))->character == 0x99) {
                      jeep_draw = 1;
                    }
                    if ((bVar10) &&
                       (((((bool)(bVar27 >> 1 & 1) || (iVar26 != 0)) ||
                         (Cmod[1] == (CharacterModel *)0x0)) ||
                        (((sVar24 = Cmod[1]->character, sVar24 != 0x44 && (sVar24 != 0xb2)) &&
                         ((sVar24 != 0x99 && (sVar24 != 99)))))))) {
                      pnVar16 = &nStack_158;
                    }
                    else {
                      pnVar16 = (numtx_s *)0x0;
                    }
                    pnVar17 = (numtx_s *)0x0;
                    if (!(bool)(bVar28 >> 1 & 1)) {
                      pnVar17 = &local_118;
                    }
                    DrawCharacterModel(*(CharacterModel **)((int)Cmod + iVar12),&(c->obj).anim,
                                       &local_198,pnVar16,(int)local_d0._01,pnVar17,pnVar20,loc_mo m,
                                       &c->obj);
                  }
                  iVar26 = iVar26 + 1;
                  loc_mom = loc_mom + 0x10;
                  pnVar20 = pnVar20 + 0x10;
                  iVar12 = iVar12 + 4;
                } while (iVar26 < 2);
                if ((((!bVar1) && ((c->obj).character == 0x76)) &&
                    (((LDATA->flags & 0x200) != 0 || (Level == 0x1d)))) &&
                   (ObjTab[66].obj.special != (nuspecial_s *)0x0)) {
                  NuRndrGScnObj((ObjTab[66].obj.scene)->gobjs
                                [(ObjTab[66].obj.special)->instance->objid],&local_198);
                }
              }
              else {
                if (((VEHICLECONTROL == 2) && ((c->obj).dead == '\0')) &&
                   ((c->spin != '\0' &&
                    (((int)c->spin_frame <
                      (int)c->spin_frames - (int)c->OnFootMoveInfo->SPINRESETFRAMES && (!bVar9)))) ))
                {
                  if (CRemap[116] != -1) {
                    local_1e8.x = (c->obj).SCALE;
                    uVar23 = (c->obj).hdg - 0x8000 & 0xffff;
                    local_1e8.y = local_1e8.x;
                    local_1e8.z = local_1e8.x;
                    NuMtxSetScale(&local_198,&local_1e8);
                    NuMtxRotateZ(&local_198,c->spin_frame * 0x1555);
                    NuMtxRotateX(&local_198,(uint)(c->obj).xrot);
                    NuMtxRotateY(&local_198,uVar23);
                    NuMtxTranslate(&local_198,&(c->obj).pos);
                    iVar12 = (int)CRemap[116];
                    Cmod[0] = CModel + iVar12;
                    pnVar15 = CModel[iVar12].anmdata[0x46];
                    if (pnVar15 == (nuAnimData_s *)0x0) {
                      NuHGobjEval(CModel[iVar12].hobj,0,(NUJOINTANIM_s *)0x0,tmtx);
                    }
                    else {
                      NuHGobjEvalAnim(CModel[iVar12].hobj,pnVar15,
                                      (float)((double)(float)((double)pnVar15->time - dVar31) *
                                              (double)((float)((double)CONCAT44(0x43300000,
                                                                                (int)c->spin_frame  ^
                                                                                0x80000000) -
                                                              4503601774854144.0) /
                                                      (float)((double)CONCAT44(0x43300000,
                                                                               (int)c->
                                                  OnFootMoveInfo->SPINFRAMES +
                                                  c->OnFootMoveInfo->SUPERSPINFRAMES * 3 ^
                                                  0x80000000) - 4503601774854144.0)) + dVar31),0,
                                      (NUJOINTANIM_s *)0x0,tmtx);
                    }
                    if (glass_draw == 0) {
                      StoreLocatorMatrices
                                (Cmod[0],&local_198,tmtx,(numtx_s *)c->mtxLOCATOR,
                                 (nuvec_s *)c->momLOCATOR);
                    }
                    if (bVar1) goto LAB_8001f958;
                    bVar2 = local_d0._02 != 0.0;
                    SetCreatureLights(c);
                    NuHGobjRndrMtx(Cmod[0]->hobj,&local_198,1,(short *)0x0,tmtx);
                    if ((((double)(c->obj).reflect_y != dVar32) && ((double)glass_mix == dVar30))  &&
                       (glass_draw == 0)) {
                      iVar12 = 0x30;
                      pnVar20 = &local_198;
                      pnVar16 = &local_118;
                      do {
                        pnVar19 = pnVar16;
                        pnVar17 = pnVar20;
                        iVar12 = iVar12 + -0x18;
                        pnVar19->_00 = pnVar17->_00;
                        pnVar19->_01 = pnVar17->_01;
                        pnVar19->_02 = pnVar17->_02;
                        pnVar19->_03 = pnVar17->_03;
                        pnVar19->_10 = pnVar17->_10;
                        pnVar19->_11 = pnVar17->_11;
                        pnVar20 = (numtx_s *)&pnVar17->_12;
                        pnVar16 = (numtx_s *)&pnVar19->_12;
                      } while (iVar12 != 0);
                      pnVar19->_12 = pnVar17->_12;
                      pnVar19->_13 = pnVar17->_13;
                      pnVar19->_20 = pnVar17->_20;
                      pnVar19->_21 = pnVar17->_21;
                      local_118._01 = -local_118._01;
                      local_118._11 = -local_118._11;
                      local_118._21 = -local_118._21;
                      fVar4 = (c->obj).reflect_y;
                      local_118._31 = fVar4 - (local_118._31 - fVar4);
                      NuHGobjRndrMtx(Cmod[0]->hobj,&local_118,1,(short *)0x0,tmtx);
                    }
                    if (((bVar2) && (fVar4 = (c->obj).shadow, (double)fVar4 != dVar32)) &&
                       ((((dVar34 < dVar33 &&
                          (((TerSurface[(c->obj).surface_type].flags & 1) == 0 &&
                           (SKELETALCRASH == 0)))) && (c->freeze == '\0')) && (glass_draw == 0))))  {
                      ScaleFlatShadow(&local_1e8,(c->obj).pos.y,fVar4,(c->obj).SCALE);
                      NuMtxSetScale(&nStack_158,&local_1e8);
                      NuMtxRotateY(&nStack_158,uVar23);
                      NuMtxRotateZ(&nStack_158,(uint)(c->obj).surface_zrot);
                      NuMtxRotateX(&nStack_158,(uint)(c->obj).surface_xrot);
                      nStack_158._30 = (c->obj).pos.x;
                      nStack_158._31 = (c->obj).shadow + 0.025;
                      nStack_158._32 = (c->obj).pos.z;
                    }
                  }
LAB_8001ed04:
                  (c->obj).draw_frame = uVar22 + 1;
                  goto LAB_8001f958;
                }
                if (((((bVar3) || ((c->obj).character != 0)) || ((c->obj).dead != '\0')) ||
                    ((c->spin == '\0' ||
                     ((int)c->spin_frames - (int)c->OnFootMoveInfo->SPINRESETFRAMES <=
                      (int)c->spin_frame)))) || ((bVar9 || ((sVar24 != -1 || (c->freeze != '\0')))) )
                   ) goto LAB_8001ecd4;
                if (!bVar1) {
                  SetCreatureLights(c);
                }
                if (CRemap[8] != -1) {
                  local_1e8.x = (c->obj).SCALE;
                  uVar23 = c->spin_frame * -0x1555 & 0xffff;
                  local_1e8.y = local_1e8.x;
                  local_1e8.z = local_1e8.x;
                  NuMtxSetScale(&local_198,&local_1e8);
                  NuMtxRotateY(&local_198,uVar23);
                  NuMtxRotateZ(&local_198,(uint)(c->obj).zrot);
                  NuMtxRotateX(&local_198,(uint)(c->obj).xrot);
                  NuMtxTranslate(&local_198,&(c->obj).pos);
                  if ((SKELETALCRASH == 0) || (iVar12 = (int)CRemap[159], iVar12 == -1)) {
                    iVar12 = (int)CRemap[8];
                  }
                  Cmod[0] = CModel + iVar12;
                  uVar18 = 0x46;
                  if ((c->obj).dangle != '\0') {
                    uVar18 = 0x47;
                  }
                  if ((uVar18 < 0x76) &&
                     (pnVar15 = *(nuAnimData_s **)(iVar12 * 0x7ac + -0x7fdac120 + uVar18 * 4),
                     pnVar15 != (nuAnimData_s *)0x0)) {
                    NuHGobjEvalAnim(Cmod[0]->hobj,pnVar15,
                                    (float)((double)(float)((double)pnVar15->time - dVar31) *
                                            (double)((float)((double)CONCAT44(0x43300000,
                                                                              (int)c->spin_frame ^
                                                                              0x80000000) -
                                                            4503601774854144.0) /
                                                    (float)((double)CONCAT44(0x43300000,
                                                                             (int)c->OnFootMoveInfo
                                                                                  ->SPINFRAMES +
                                                                             c->OnFootMoveInfo->
                                                                             SUPERSPINFRAMES * 3 ^
                                                                             0x80000000) -
                                                           4503601774854144.0)) + dVar31),0,
                                    (NUJOINTANIM_s *)0x0,tmtx);
                  }
                  else {
                    NuHGobjEval(Cmod[0]->hobj,0,(NUJOINTANIM_s *)0x0,tmtx);
                  }
                  if (glass_draw == 0) {
                    StoreLocatorMatrices
                              (Cmod[0],&local_198,tmtx,(numtx_s *)c->mtxLOCATOR,
                               (nuvec_s *)c->momLOCATOR);
                  }
                  if (bVar1) goto LAB_8001f958;
                  bVar2 = local_d0._02 != 0.0;
                  NuHGobjRndrMtx(Cmod[0]->hobj,&local_198,1,(short *)0x0,tmtx);
                  if ((((double)(c->obj).reflect_y != dVar32) && ((double)glass_mix == dVar30)) &&
                     (glass_draw == 0)) {
                    iVar12 = 0x30;
                    pnVar20 = &local_198;
                    pnVar16 = &local_118;
                    do {
                      pnVar19 = pnVar16;
                      pnVar17 = pnVar20;
                      iVar12 = iVar12 + -0x18;
                      pnVar19->_00 = pnVar17->_00;
                      pnVar19->_01 = pnVar17->_01;
                      pnVar19->_02 = pnVar17->_02;
                      pnVar19->_03 = pnVar17->_03;
                      pnVar19->_10 = pnVar17->_10;
                      pnVar19->_11 = pnVar17->_11;
                      pnVar20 = (numtx_s *)&pnVar17->_12;
                      pnVar16 = (numtx_s *)&pnVar19->_12;
                    } while (iVar12 != 0);
                    pnVar19->_12 = pnVar17->_12;
                    pnVar19->_13 = pnVar17->_13;
                    pnVar19->_20 = pnVar17->_20;
                    pnVar19->_21 = pnVar17->_21;
                    local_118._01 = -local_118._01;
                    local_118._11 = -local_118._11;
                    local_118._21 = -local_118._21;
                    fVar4 = HUBREFLECTY;
                    if (Level != 0x25) {
                      fVar4 = (c->obj).reflect_y;
                    }
                    local_118._31 = fVar4 - (local_118._31 - fVar4);
                    NuHGobjRndrMtx(Cmod[0]->hobj,&local_118,1,(short *)0x0,tmtx);
                  }
                  if (((((bVar2) && (fVar4 = (c->obj).shadow, (double)fVar4 != dVar32)) &&
                       ((dVar34 < dVar33 &&
                        (((TerSurface[(c->obj).surface_type].flags & 1) == 0 && (SKELETALCRASH ==  0)
                         ))))) && (c->freeze == '\0')) && (glass_draw == 0)) {
                    ScaleFlatShadow(&local_1e8,(c->obj).pos.y,fVar4,(c->obj).SCALE);
                    NuMtxSetScale(&nStack_158,&local_1e8);
                    NuMtxRotateY(&nStack_158,uVar23);
                    NuMtxRotateZ(&nStack_158,(uint)(c->obj).surface_zrot);
                    NuMtxRotateX(&nStack_158,(uint)(c->obj).surface_xrot);
                    nStack_158._30 = (c->obj).pos.x;
                    nStack_158._31 = (c->obj).shadow + 0.025;
                    nStack_158._32 = (c->obj).pos.z;
                    NuMtlSetStencilRender(NUSTENCIL_REPLACE_NODRAW);
                    NuHGobjRndrMtx(Cmod[0]->hobj,&nStack_158,1,(short *)0x0,tmtx);
                    NuMtlSetStencilRender(NUSTENCIL_NOSTENCIL);
                  }
                }
                if (bVar1) goto LAB_8001f958;
                if (((c->obj).dangle != '\0') && (CRemap[9] != -1)) {
                  uVar6 = (c->obj).hdg;
                  local_1e8.x = (c->obj).SCALE;
                  local_1e8.y = local_1e8.x;
                  local_1e8.z = local_1e8.x;
                  NuMtxSetScale(&local_198,&local_1e8);
                  NuMtxRotateY(&local_198,uVar6 - 0x8000 & 0xffff);
                  NuMtxRotateZ(&local_198,(uint)(c->obj).zrot);
                  NuMtxRotateX(&local_198,(uint)(c->obj).xrot);
                  NuMtxTranslate(&local_198,&(c->obj).pos);
                  Cmod[1] = CModel + CRemap[9];
                  NuHGobjRndr(CModel[CRemap[9]].hobj,&local_198,1,(short *)0x0);
                  if (((double)(c->obj).reflect_y != dVar32) &&
                     (((double)glass_mix == dVar30 && (glass_draw == 0)))) {
                    iVar12 = 0x30;
                    pnVar20 = &local_198;
                    pnVar16 = &local_118;
                    do {
                      pnVar19 = pnVar16;
                      pnVar17 = pnVar20;
                      iVar12 = iVar12 + -0x18;
                      pnVar19->_00 = pnVar17->_00;
                      pnVar19->_01 = pnVar17->_01;
                      pnVar19->_02 = pnVar17->_02;
                      pnVar19->_03 = pnVar17->_03;
                      pnVar19->_10 = pnVar17->_10;
                      pnVar19->_11 = pnVar17->_11;
                      pnVar20 = (numtx_s *)&pnVar17->_12;
                      pnVar16 = (numtx_s *)&pnVar19->_12;
                    } while (iVar12 != 0);
                    pnVar19->_12 = pnVar17->_12;
                    pnVar19->_13 = pnVar17->_13;
                    pnVar19->_20 = pnVar17->_20;
                    pnVar19->_21 = pnVar17->_21;
                    local_118._01 = -local_118._01;
                    local_118._11 = -local_118._11;
                    local_118._21 = -local_118._21;
                    fVar4 = (c->obj).reflect_y;
                    local_118._31 = fVar4 - (local_118._31 - fVar4);
                    NuHGobjRndr(Cmod[1]->hobj,&local_118,1,(short *)0x0);
                  }
                }
              }
              (c->obj).draw_frame = uVar22 + 1;
            }
          }
        }
      }
      else if (((in_finish_range != 0x3c) && ((c->obj).finished == '\0')) &&
              ((Level != 0x25 || (warp_level == -1)))) {
        if ((VEHICLECONTROL != 1) || (sVar24 = (c->obj).vehicle, sVar24 == -1)) goto LAB_8001e064;
        goto LAB_8001e068;
      }
LAB_8001f958:
      iVar25 = iVar25 + 1;
      c->anim_processed = '\x01';
      c = c + 1;
    } while (iVar25 < (int)local_d0._00);
  }
  glass_phase = 0;
  glass_draw = 0;
  if (!bVar1) {
    SetLevelLights();
  }
  return;
}



void MovePlayer(creature_s *plr,nupad_s *pad)

{
  bool bVar1;
  float fVar2;
  short sVar3;
  uint uVar4;
  RPos_s *pRVar5;
  float fVar6;
  char cVar8;
  mask *pmVar7;
  uchar uVar9;
  rumble_s *prVar10;
  ushort uVar12;
  int Awrd;
  ushort uVar13;
  int iVar11;
  CharacterModel *mod;
  char cVar14;
  float fVar15;
  nuvec_s *pos;
  short sVar16;
  uint uVar17;
  int *piVar18;
  int unaff_r19;
  Moveinfo_s *moveinfo;
  short sVar19;
  int iVar20;
  obj_s *obj;
  bool bVar21;
  byte in_cr0;
  byte in_cr1;
  byte in_cr2;
  byte bVar22;
  byte in_cr3;
  byte bVar23;
  byte unaff_cr4;
  byte in_cr5;
  byte in_cr6;
  byte in_cr7;
  double dVar24;
  double dVar25;
  double dVar26;
  double dVar27;
  double dVar28;
  float fVar29;
  int local_108;
  int local_104;
  int local_100;
  int local_fc;
  short local_f8;
  int iStack_f6;
  nuvec_s local_f0;
  nuvec_s local_e0;
  nuvec_s local_d0;
  float local_c0;
  float local_b8;
  nuvec_s local_b0;
  nuvec_s local_a0;
  ushort local_90 [20];
  double local_68;
  uint local_60;
  
  local_60 = (uint)(in_cr0 & 0xf) << 0x1c | (uint)(in_cr1 & 0xf) << 0x18 |
             (uint)(in_cr2 & 0xf) << 0x14 | (uint)(in_cr3 & 0xf) << 0x10 |
             (uint)(unaff_cr4 & 0xf) << 0xc | (uint)(in_cr5 & 0xf) << 8 | (uint)(in_cr6 & 0xf) <<  4
             | (uint)(in_cr7 & 0xf);
  piVar18 = &local_108;
  local_108 = 0x80007;
  local_104 = 0x100020;
  local_100 = 0x400080;
  local_fc = 0x1000200;
  local_f8 = 0x400;
  if (pad == (nupad_s *)0x0) {
    if (Demo == 0) {
      return;
    }
    pad = (nupad_s *)demopad;
  }
  prVar10 = &plr->rumble;
  UpdateRumble(prVar10);
  if (pad != (nupad_s *)0x0) {
    if ((Game.vibration == '\0') || (Demo != 0)) {
      NuPs2PadSetMotors(pad,0,0);
    }
    else {
      uVar17 = (uint)(plr->rumble).frame;
      if (uVar17 == 0) {
        uVar17 = 0;
      }
      else {
        uVar17 = ((plr->rumble).power * uVar17) / (uint)(plr->rumble).frames;
      }
      NuPs2PadSetMotors(pad,(uint)((plr->rumble).buzz != '\0'),uVar17);
    }
  }
  if ((((Level == 0x16) && (boss_dead == 1)) && ((plr->obj).pos.z < 49.0)) &&
     (fVar6 = (plr->obj).oldpos.z, 49.0 <= fVar6)) {
    (plr->obj).pos.x = (plr->obj).oldpos.x;
    (plr->obj).pos.z = fVar6;
    (plr->obj).mom.z = 0.05;
  }
  Awrd = VEHICLECONTROL;
  if ((plr->obj).vehicle == -1) {
    VEHICLECONTROL = 0;
  }
  else if ((((LBIT._0_4_ & 0x11 | LBIT._4_4_ & 0x5252801) == 0) && (Level != 3)) &&
          ((Level != 0x1d && (Level != 0x1c)))) {
    if (((LBIT._0_4_ & 4 | LBIT._4_4_ & 0x40) != 0) && (VEHICLECONTROL == 0)) {
      VEHICLECONTROL = 1;
    }
  }
  else {
    VEHICLECONTROL = 1;
  }
  if ((VEHICLECONTROL == 1) && (vtog_time == vtog_duration)) {
    (plr->obj).vehicle_frame = (plr->obj).vehicle_frame + 1;
  }
  else {
    (plr->obj).vehicle_frame = 0;
    plr_vehicle_time = 0.0;
    plr_vehicle_speedmul = 1.0;
  }
  ToggleVehicle(plr);
  sVar19 = -1;
  if ((VEHICLECONTROL == 1) && (sVar16 = (plr->obj).vehicle, sVar16 != -1)) {
    sVar19 = sVar16;
  }
  bVar23 = (sVar19 == 0x6b) << 1;
  if (sVar19 == 0x6b) {
    moveinfo = &ScooterMoveInfo;
  }
  else if (sVar19 == 0xa0) {
    moveinfo = &SnowBoardMoveInfo;
  }
  else if (sVar19 == 0x44) {
    moveinfo = &MechMoveInfo;
  }
  else if (sVar19 == 0xb2) {
    moveinfo = &FireEngineMoveInfo;
  }
  else if (sVar19 == 0x3b) {
    moveinfo = &GyroMoveInfo;
  }
  else if (sVar19 == 0x20) {
    moveinfo = &SubmarineMoveInfo;
  }
  else if (sVar19 == 0x89) {
    moveinfo = &MineCartMoveInfo;
  }
  else if (sVar19 == 0xa1) {
    moveinfo = &MineTubMoveInfo;
  }
  else if (sVar19 == 0x99) {
    moveinfo = &OffRoaderMoveInfo;
  }
  else if (VEHICLECONTROL == 2) {
    moveinfo = &SwimmingMoveInfo;
  }
  else {
    moveinfo = plr->OnFootMoveInfo;
  }
  if ((VEHICLECONTROL == 1) && (Awrd != 1)) {
    plr_vehicle_time = 0.0;
    if (sVar19 == 0x89) {
      VEHICLETIME = 60.0;
    }
    else if (sVar19 == 99) {
      ResetJeep(plr);
    }
    ResetTubs();
  }
  uVar9 = (plr->obj).invincible;
  if (uVar9 != '\0') {
    (plr->obj).invincible = uVar9 + 0xff;
  }
  if (plr->freeze != '\0') {
    plr->freeze = plr->freeze + 0xff;
  }
  pos = &(plr->obj).pos;
  fVar15 = (plr->obj).pos.y;
  fVar6 = (plr->obj).pos.z;
  (plr->obj).oldpos.x = (plr->obj).pos.x;
  (plr->obj).oldpos.z = fVar6;
  obj = &plr->obj;
  (plr->obj).oldpos.y = fVar15;
  cVar14 = (plr->obj).dangle;
  (plr->obj).old_ground = (plr->obj).ground;
  GetTopBot(plr);
  OldTopBot(obj);
  if (GameMode != 1) {
    if (vtog_time < vtog_duration) {
      vtog_time = vtog_time + 0.01666667;
      if ((vtog_duration * 0.333 <= vtog_time) && (vtog_time < vtog_duration * 0.333)) {
        gamesfx_effect_volume = 0x2fff;
        GameSfx(0x2f,pos);
      }
      if ((vtog_duration <= vtog_time) && (vtog_time = vtog_duration, VEHICLECONTROL == 1)) {
        Awrd = 0x48;
        if (sVar19 == 0xa0) {
          Awrd = 0x54;
        }
        if (Awrd != -1) {
          GameSfx(Awrd,pos);
        }
        NewRumble(prVar10,0xbf);
        NewBuzz(prVar10,0xc);
      }
      uVar12 = vtog_angle;
      if (vtog_blend != 0) {
        dVar27 = (double)(vtog_time / vtog_duration);
        (plr->obj).pos.x =
             (float)((double)(float)((double)vtog_newpos.x - (double)vtog_oldpos.x) * dVar27 +
                    (double)vtog_oldpos.x);
        (plr->obj).pos.y =
             (float)((double)(float)((double)vtog_newpos.y - (double)vtog_oldpos.y) * dVar27 +
                    (double)vtog_oldpos.y);
        (plr->obj).pos.z =
             (float)((double)(float)((double)vtog_newpos.z - (double)vtog_oldpos.z) * dVar27 +
                    (double)vtog_oldpos.z);
        fVar15 = v000.z;
        fVar6 = v000.y;
        (plr->obj).mom.x = v000.x;
        (plr->obj).mom.z = fVar15;
        (plr->obj).mom.y = fVar6;
        (plr->obj).thdg = uVar12;
        uVar12 = SeekRot((plr->obj).hdg,uVar12,3);
        (plr->obj).hdg = uVar12;
        fVar6 = NewShadowMaskPlat(pos,0.0,-1);
        (plr->obj).shadow = fVar6;
        if (fVar6 != 2000000.0) {
          GetSurfaceInfo(obj,1,fVar6);
        }
        if ((LBIT._0_4_ & 4 | LBIT._4_4_ & 0x40) == 0) {
          local_68 = (double)(longlong)(int)(dVar27 * 32768.0);
          (plr->obj).pos.y =
               (plr->obj).pos.y +
               NuTrigTable[(int)(dVar27 * 32768.0) & 0xffff] +
               NuTrigTable[(int)(dVar27 * 32768.0) & 0xffff];
        }
        ObjectRotation(obj,2,1);
        if ((Level != 0x16) || (vtog_time != vtog_duration)) goto LAB_8001cbd4;
      }
    }
    if (Level == 0x25) {
      if (tumble_time < tumble_duration) {
        tumble_time = tumble_time + 0.01666667;
        if ((tumble_action == 0x56) && (new_lev_flags != 0)) {
          fVar6 = (plr->obj).anim.anim_time;
          if ((fVar6 - ((plr->obj).model)->animlist[0x56]->speed * 0.5 < tumble_item_starttime + 1 .0
              ) && (tumble_item_starttime + 1.0 <= fVar6)) {
            Awrd = 0;
            do {
              uVar12 = new_lev_flags & *(ushort *)piVar18;
              if ((uVar12 != 0) && ((temp_lev_flags & uVar12) == 0)) {
                temp_lev_flags = uVar12 | temp_lev_flags;
                Awrd = AddAward(Hub,last_level,uVar12);
                if (Awrd == 0) {
                  new_lev_flags = uVar12 ^ (uVar12 | new_lev_flags);
                  Game.level[last_level].flags = uVar12 | Game.level[last_level].flags;
                }
                piVar18 = &iStack_f6;
                Awrd = 9;
              }
              Awrd = Awrd + 1;
              piVar18 = (int *)((int)piVar18 + 2);
            } while (Awrd < 9);
          }
        }
        if (tumble_duration <= tumble_time) {
          if ((new_lev_flags | temp_lev_flags) == temp_lev_flags) {
            tumble_time = tumble_duration;
            plr->jump = '\x01';
            if (tumble_action == 0x56) {
              plr->jump_type = '\0';
              Awrd = qrand();
              if (Awrd < 0) {
                Awrd = Awrd + 0x7fff;
              }
              plr->jump_subtype = (char)(Awrd >> 0xf);
            }
            else {
              plr->jump_type = '\x01';
            }
            if (plr->jump_type == '\0') {
              sVar16 = moveinfo->STARJUMPFRAMES;
            }
            else {
              sVar16 = moveinfo->JUMPFRAMES0;
            }
            plr->jump_frames = sVar16;
            (plr->obj).anim.anim_time = 1.0;
            (plr->obj).ground = '\0';
            plr->jump_hack = '\x01';
            plr->jump_frame = 0;
            plr->somersault = '\0';
            plr->land = '\0';
            AddGameDebris(0x10,pos);
            AddGameDebris(0x11,pos);
            GameSfx(0x2f,pos);
          }
          else {
            tumble_time = tumble_cycleduration;
            (plr->obj).anim.anim_time = tumble_item_starttime;
          }
        }
      }
      if (Level != 0x25) goto LAB_80019f04;
      if ((tumble_duration <= tumble_time) || (last_hub == -1)) {
        if (warp_level != -1) goto LAB_8001cbd4;
        goto LAB_80019f04;
      }
      if (tumble_time < tumble_moveduration) {
        fVar6 = tumble_time / tumble_moveduration;
        (plr->obj).pos.x = (tumble_newpos.x - tumble_oldpos.x) * fVar6 + tumble_oldpos.x;
        (plr->obj).pos.y = (tumble_newpos.y - tumble_oldpos.y) * fVar6 + tumble_oldpos.y;
        (plr->obj).pos.z = (tumble_newpos.z - tumble_oldpos.z) * fVar6 + tumble_oldpos.z;
        fVar6 = NewShadowMaskPlat(pos,0.0,-1);
        (plr->obj).shadow = fVar6;
        if (fVar6 != 2000000.0) {
          GetSurfaceInfo(obj,1,fVar6);
        }
      }
      else {
        (plr->obj).pos.x = tumble_newpos.x;
        (plr->obj).pos.y = tumble_newpos.y;
        (plr->obj).pos.z = tumble_newpos.z;
      }
      uVar12 = tumble_hdg;
      bVar1 = sVar19 == 0x20;
      (plr->obj).ground = '\x03';
      (plr->obj).thdg = uVar12;
      (plr->obj).hdg = uVar12;
      (plr->obj).old_ground = '\x03';
LAB_8001c028:
      fVar6 = NewShadowMaskPlatRot(pos,0.0,-1);
      if (fVar6 == 2000000.0) {
        cVar8 = (plr->obj).transporting;
        plr_terrain_ok = (int)cVar8;
        if (plr_terrain_ok != 0) goto LAB_8001c0a8;
        if (((NOTERRAINSTOP == 0) || (Level == 8)) || (Level == 6)) goto LAB_8001c0b0;
        fVar6 = (plr->obj).oldpos.z;
        (plr->obj).pos.x = (plr->obj).oldpos.x;
        (plr->obj).pos.z = fVar6;
        (plr->obj).mom.z = 0.0;
        (plr->obj).dangle = cVar8;
        (plr->obj).mom.x = 0.0;
      }
      else {
LAB_8001c0a8:
        plr_terrain_ok = 1;
LAB_8001c0b0:
        unaff_r19 = (int)(plr->obj).layer_type;
        if (((plr->obj).got_shadow == '\0') ||
           ((((plr->obj).transporting == '\0' && (fVar6 != 2000000.0)) &&
            ((plr->obj).shadow < fVar6)))) {
          (plr->obj).got_shadow = '\0';
          (plr->obj).shadow = fVar6;
          GetSurfaceInfo(obj,2,fVar6);
        }
        else {
          GetSurfaceInfo(obj,0,fVar6);
        }
      }
      (plr->obj).wade = '\0';
      if ((plr->obj).layer_type == -1) {
        (plr->obj).submerged = '\0';
      }
      else {
        if ((plr->obj).layer_shadow <= (plr->obj).top * (plr->obj).SCALE + (plr->obj).pos.y) {
          (plr->obj).submerged = '\0';
        }
        else {
          bVar22 = (plr->obj).submerged;
          if (bVar22 < 0x3c) {
            (plr->obj).submerged = bVar22 + 1;
          }
          else if (((Level != 0x25) && (!bVar1)) && (VEHICLECONTROL != 2)) {
            KillPlayer(obj,10);
          }
        }
        if (((plr->obj).bot + (plr->obj).top) * (plr->obj).SCALE * 0.5 + (plr->obj).pos.y <
            (plr->obj).layer_shadow) {
          (plr->obj).wade = '\x01';
        }
        cVar8 = (plr->obj).layer_type;
        Awrd = 1;
        local_e0.x = (plr->obj).pos.x;
        iVar20 = -1;
        local_e0.y = (plr->obj).layer_shadow;
        local_e0.z = (plr->obj).pos.z;
        if (cVar8 == '\x02') {
LAB_8001c324:
          iVar20 = 1;
        }
        else if (cVar8 < '\x03') {
          if (cVar8 == '\x01') {
            uVar17 = qrand();
            dVar26 = 4503601774854144.0;
            local_68 = (double)CONCAT44(0x43300000,uVar17 ^ 0x80000000);
            dVar27 = 1.525878928987368e-06;
            local_a0.x = (float)(local_68 - 4503601774854144.0) * 1.525879e-06 + local_e0.x;
            uVar17 = qrand();
            local_68 = (double)CONCAT44(0x43300000,uVar17 ^ 0x80000000);
            local_a0.y = (float)((double)(float)(local_68 - dVar26) * dVar27 + (double)local_e0.y) ;
            uVar17 = qrand();
            local_68 = (double)CONCAT44(0x43300000,uVar17 ^ 0x80000000);
            (plr->obj).ddr = '@';
            (plr->obj).ddg = 'x';
            local_a0.z = (float)((double)(float)(local_68 - dVar26) * dVar27 + (double)local_e0.z) ;
            (plr->obj).ddb = -0x80;
            (plr->obj).ddwater = 'x';
            if ((plr->obj).idle_gametime == 0.0) {
              iVar11 = qrand();
              bVar21 = 0x7fff < iVar11;
            }
            else {
              iVar11 = qrand();
              bVar21 = 0xfff < iVar11;
            }
            if (!bVar21) {
              NuRndrAddWaterRipple(&local_a0,0.2,0.4,0x20,0x60706050);
            }
          }
        }
        else {
          if (cVar8 == '\x03') goto LAB_8001c324;
          if ((cVar8 == '\x04') && ((plr->obj).idle_gametime == 0.0)) {
            iVar20 = 1;
            Awrd = 2;
            local_e0.y = local_e0.y + 0.03;
          }
        }
        if (((Paused == 0) && (-1 < iVar20)) && (jonframe1 == (jonframe1 / Awrd) * Awrd)) {
          AddVariableShotDebrisEffect(GDeb[iVar20].i,&local_e0,1,0,0);
        }
      }
      cVar8 = (plr->obj).surface_type;
      if ('\0' < cVar8) {
        local_e0.x = (plr->obj).pos.x;
        Awrd = 1;
        local_e0.y = (plr->obj).pos.y;
        iVar20 = -1;
        local_e0.z = (plr->obj).pos.z;
        if (cVar8 == '\b') {
          if (((plr->obj).idle_gametime == 0.0) && (Level != 3)) {
            iVar20 = 4;
            Awrd = 4;
LAB_8001c484:
            local_e0.y = local_e0.y + 0.03;
          }
        }
        else if (cVar8 < '\t') {
          if (cVar8 == '\x01') {
LAB_8001c40c:
            if ((plr->obj).idle_gametime == 0.0) {
              iVar20 = 2;
              Awrd = 2;
            }
          }
          else if ((cVar8 == '\x02') && ((plr->obj).idle_gametime == 0.0)) {
            iVar20 = 1;
            Awrd = 2;
            goto LAB_8001c484;
          }
        }
        else if ((cVar8 < '\x0e') && ('\v' < cVar8)) goto LAB_8001c40c;
        if ((((Paused == 0) && (-1 < iVar20)) && ((plr->obj).ground != '\0')) &&
           (jonframe1 == (jonframe1 / Awrd) * Awrd)) {
          AddVariableShotDebrisEffect(GDeb[iVar20].i,&local_e0,1,0,0);
        }
      }
      Awrd = (int)(plr->obj).roof_type;
      if (((Awrd == -1) || ((TerSurface[Awrd].flags & 0x10) == 0)) || (VEHICLECONTROL == 1)) {
        (plr->obj).dangle = '\0';
        if (cVar14 != '\0') {
          sVar16 = plr->jump_frames;
          plr->jump = '\x06';
          (plr->obj).mom.y = 0.0;
          plr->jump_frame = sVar16;
          plr->jump_type = '\x04';
          GameSfx(0x1c,pos);
        }
      }
      else {
        fVar6 = (plr->obj).SCALE;
        fVar15 = (plr->obj).roof_y - moveinfo->DANGLEGAP;
        if ((fVar15 <= (plr->obj).top * fVar6 + (plr->obj).pos.y) || (cVar14 != '\0')) {
          fVar29 = (plr->obj).max.y;
          (plr->obj).dangle = '\x01';
          (plr->obj).pos.y = fVar15 - fVar29 * fVar6;
          if (cVar14 == '\0') {
            GameSfx(0x1b,pos);
          }
        }
      }
      cVar14 = (plr->obj).layer_type;
      if ((cVar14 == '\x01') && (unaff_r19 == -1)) {
        GameSfx(0x47,pos);
      }
      else if ((cVar14 == -1) && (unaff_r19 == 1)) {
        GameSfx(0x47,pos);
      }
      TerrainFailsafe(obj);
      if ((plr->obj).got_shadow == '\0') {
        bVar21 = ((plr->obj).bot * (plr->obj).SCALE + (plr->obj).pos.y) - (plr->obj).shadow < 0.02 5;
      }
      else {
        bVar21 = (plr->obj).bot * (plr->obj).SCALE + (plr->obj).pos.y <= (plr->obj).shadow;
      }
      cVar14 = *(char *)((int)&(plr->obj).gndflags + 1);
      (plr->obj).ground = '\0';
      if (cVar14 != '\0') {
        (plr->obj).ground = '\x01';
      }
      if (bVar21) {
        (plr->obj).ground = (plr->obj).ground | 2;
      }
      cVar14 = (plr->obj).ground;
      if (cVar14 != '\0') {
        (plr->obj).last_ground = cVar14;
      }
      if ((((plr->obj).ground & 2U) != 0) && ((plr->obj).dead == '\0')) {
        cVar14 = (plr->obj).surface_type;
        if ((((TerSurface[cVar14].flags & 1) == 0) ||
            (((uVar9 = (plr->obj).invincible, uVar9 != '\0' && ((plr->obj).layer_type == -1)) ||
             (sVar19 == 0x20)))) || (VEHICLECONTROL == 2)) {
          if ((TerSurface[cVar14].flags & 2) != 0) {
            (plr->obj).boing = (plr->obj).boing | 2;
            GameSfx(2,pos);
            NewRumble(&player->rumble,0x7f);
            NewBuzz(&player->rumble,0xc);
          }
        }
        else {
          if ((VEHICLECONTROL == 1) && ((plr->obj).vehicle != -1)) {
            Awrd = 0xb;
            cVar8 = (plr->obj).layer_type;
          }
          else {
            cVar8 = (plr->obj).layer_type;
            if ((plr->obj).character == 0) {
              if (((cVar8 == -1) || (CRemap[79] == -1)) || (Level != 7)) {
                if ((cVar14 != '\a') || (CRemap[151] == -1)) goto LAB_8001c7b0;
                Awrd = 0xd;
              }
              else {
                Awrd = 8;
              }
            }
            else {
LAB_8001c7b0:
              Awrd = 5;
              if ((cVar8 != -1) && ((plr->obj).submerged != '\0')) {
                Awrd = 10;
              }
            }
          }
          pmVar7 = (plr->obj).mask;
          bVar21 = false;
          if (((pmVar7 != (mask *)0x0) && (pmVar7->active != 0)) && ((LDATA->flags & 0xe00) == 0))  {
            bVar21 = 2 < pmVar7->active;
          }
          if (((cVar8 == -1) || ((plr->obj).submerged != '\0')) || ((Level != 2 || (Bonus != 0))))  {
            (plr->obj).invincible = '\0';
            KillGameObject(obj,Awrd);
            pmVar7 = (plr->obj).mask;
            if (((pmVar7 != (mask *)0x0) && (pmVar7->active != 0)) &&
               (uVar12 = LDATA->flags, (uVar12 & 0xe00) == 0)) {
              AddMaskFeathers(pmVar7);
              ((plr->obj).mask)->active = uVar12 & 0xe00;
            }
          }
          else if (((pmVar7 == (mask *)0x0) || (pmVar7->active == 0)) ||
                  ((LDATA->flags & 0xe00) != 0)) {
            KillGameObject(obj,Awrd);
          }
          else if ((pmVar7->active < 3) && (uVar9 == '\0')) {
            LoseMask(obj);
          }
          if ((((plr->obj).dead != '\0') && (bVar21)) && (Awrd = NuSoundKeyStatus(4), Awrd == 1))  {
            NuSoundStopStream(4);
          }
        }
      }
      ObjectRotation(obj,2,(uint)(sVar19 == -1));
      if ((((plr->obj).transporting == '\0') && (sVar19 != 0x89)) && (sVar19 != 0xa1)) {
        WumpaCollisions(obj);
      }
      plr_allow_jump = (int)plr->allow_jump;
      if ((plr->obj).ground == '\0') {
        if (plr->allow_jump != '\0') {
          uVar9 = plr->allow_jump + 0xff;
          goto LAB_8001c9d0;
        }
      }
      else {
        uVar9 = '\f';
LAB_8001c9d0:
        plr->allow_jump = uVar9;
      }
      bVar22 = (plr->obj).boing;
      if (bVar22 != 0) {
        plr->land = '\0';
        plr->ok_slam = '\x01';
        cVar14 = '\x01';
        plr->jump = '\x01';
        plr->somersault = '\0';
        if ((bVar22 & 2) != 0) {
          cVar14 = '\x03';
        }
        plr->jump_type = cVar14;
        sVar16 = moveinfo->JUMPFRAMES1;
        plr->jump_frame = 0;
        plr->jump_frames = sVar16 + 1;
        fVar6 = moveinfo->JUMPHEIGHT;
        local_68 = (double)CONCAT44(0x43300000,(int)moveinfo->JUMPFRAMES2 ^ 0x80000000);
        if (cVar14 == '\x03') {
          fVar6 = fVar6 * 1.5;
        }
        (plr->obj).mom.y = fVar6 / (float)(local_68 - 4503601774854144.0);
        plr->slam = '\x03';
        plr->slam_wait = 0;
        (plr->obj).anim.anim_time = 1.0;
      }
      if (plr->fire != '\0') {
        plr->fire = plr->fire + 0xff;
      }
      if (plr->tap != '\0') {
        plr->tap = plr->tap + 0xff;
      }
    }
    else {
LAB_80019f04:
      if ((((plr->obj).dead != '\0') && (Level != 0x1d)) ||
         (((((Cursor.menu != -1 &&
             (((Cursor.menu != '\"' && (Cursor.menu != '$')) && (Cursor.menu != '\x12')))) &&
            ((Cursor.menu != '\x10' || (Level != 0x1d)))) || (Cursor.wait != '\0')) ||
          ((((cVar8 = (plr->obj).finished, cVar8 != '\0' && (sVar19 != 99)) &&
            ((sVar19 != 0xa1 && (((sVar19 != 0x36 && (sVar19 != 0x8b)) && (sVar19 != 0x81)))))) &&
           (sVar19 != 0x3b)))))) {
        cVar14 = (plr->obj).dead;
        if (cVar14 == '\n') {
          fVar6 = (plr->obj).pos.y;
          (plr->obj).pos.y = ((plr->obj).layer_shadow - fVar6) * 0.04 + fVar6;
        }
        else if ((sVar19 != 1) &&
                (((cVar14 == '\x06' || (cVar14 == '\x10')) ||
                 (((cVar14 == '\x12' || ((cVar14 == '\x13' || (cVar14 == '\r')))) ||
                  (((bool)(bVar23 >> 1 & 1) || ((sVar19 == 0xa0 || (sVar19 == 0x99)))))))))) {
          fVar6 = NewShadowMaskPlat(pos,0.0,-1);
          (plr->obj).shadow = fVar6;
          dVar27 = 2000000.0;
          if (fVar6 != 2000000.0) {
            local_b0.x = (plr->obj).pos.x;
            local_b0.z = (plr->obj).pos.z;
            local_b0.y = ((plr->obj).bot + (plr->obj).top) * (plr->obj).SCALE * 0.5 +
                         (plr->obj).pos.y;
            fVar6 = CrateTopBelow(&local_b0);
            if (((double)fVar6 != dVar27) && ((double)(plr->obj).shadow < (double)fVar6)) {
              (plr->obj).shadow = fVar6;
            }
            if ((plr->obj).dead == '\x13') {
              (plr->obj).pos.y = (plr->obj).shadow - (plr->obj).bot * (plr->obj).SCALE;
            }
            else {
              fVar15 = (plr->obj).mom.y + GRAVITY;
              fVar29 = (plr->obj).pos.y + fVar15;
              fVar6 = (plr->obj).shadow;
              fVar2 = (plr->obj).bot * (plr->obj).SCALE;
              (plr->obj).mom.y = fVar15;
              (plr->obj).pos.y = fVar29;
              if (fVar29 + fVar2 < fVar6) {
                (plr->obj).pos.y = fVar6 - fVar2;
              }
            }
          }
        }
        goto LAB_8001cbd4;
      }
      if ((Level != 0x1d) ||
         (((plr->obj).dead == '\0' && ((cVar8 == '\0' || (in_finish_range != 0x3c)))))) {
        dVar27 = 0.0;
        local_f0.y = 0.0;
        if ((GameTimer.frame < 0x3c) ||
           (((((plr->obj).finished != '\0' || (plr->freeze != '\0')) ||
             ((Level == 0x16 && (FireBossHoldPlayer != 0)))) ||
            (((Cursor.menu == '\"' || (Cursor.menu == '$')) || (0 < fadeval)))))) {
          dVar27 = 0.0;
          (plr->obj).pad_angle = 0;
          plr->pad_type = '\x01';
          local_f0.x = 0.0;
          local_f0.z = 0.0;
          (plr->obj).pad_speed = 0.0;
          (plr->obj).pad_dx = 0.0;
          (plr->obj).pad_dz = 0.0;
        }
        else {
          fVar6 = pad->r_dy;
          if (((uint)fVar6 & 0xf000) == 0) {
            local_68 = (double)CONCAT44(0x43300000,(uint)pad->buttons_hi);
            dVar26 = local_68 - 4503599627370496.0;
            local_68 = (double)CONCAT44(0x43300000,(uint)pad->buttons_lo);
            fVar6 = (float)dVar26 - 127.5;
            dVar28 = (double)fVar6;
            dVar26 = -(double)((float)(local_68 - 4503599627370496.0) - 127.5);
            if ((float)(dVar28 * dVar28 + (double)(float)(dVar26 * dVar26)) < 1806.25) {
              dVar24 = (double)fVar6;
              NuFabs(fVar6);
              dVar25 = 42.5;
              if (dVar24 < 42.5) {
                dVar28 = dVar27;
              }
              dVar24 = (double)(float)dVar26;
              NuFabs((float)dVar26);
              if (dVar24 < dVar25) {
                dVar26 = dVar27;
              }
            }
            if ((dVar28 != 0.0) || (dVar26 != 0.0)) {
              cVar8 = '\x02';
              goto LAB_8001a324;
            }
          }
          else {
            if (((uint)fVar6 & 0x8000) == 0) {
              if (((uint)fVar6 & 0x2000) != 0) {
                dVar27 = 127.5;
              }
            }
            else {
              dVar27 = -127.5;
            }
            if (((uint)fVar6 & 0x4000) == 0) {
              if (((uint)fVar6 & 0x1000) == 0) {
                dVar26 = 0.0;
              }
              else {
                dVar26 = 127.5;
              }
            }
            else {
              dVar26 = -127.5;
            }
            dVar28 = dVar27;
            if ((dVar27 != 0.0) || (dVar26 != 0.0)) {
              cVar8 = '\x01';
LAB_8001a324:
              plr->pad_type = cVar8;
            }
          }
          local_f0.z = (float)(dVar26 * 0.007843137718737125);
          local_f0.x = (float)(dVar28 * 0.007843137718737125);
          NuVecMag(&local_f0);
          Awrd = NuAtan2D(local_f0.x,local_f0.z);
          local_e0.y = 0.0;
          local_e0.x = 0.0;
          dVar27 = (double)local_f0.x;
          NuFabs(local_f0.x);
          dVar26 = (double)local_f0.z;
          NuFabs(local_f0.z);
          if (dVar27 <= dVar26) {
            dVar27 = (double)local_f0.z;
            NuFabs(local_f0.z);
          }
          else {
            dVar27 = (double)local_f0.x;
            NuFabs(local_f0.x);
          }
          local_e0.z = (float)dVar27;
          NuVecRotateY(&local_f0,&local_e0,Awrd);
          fVar6 = NuVecMag(&local_f0);
          if (0.2 <= fVar6) {
            if (0.6 <= fVar6) {
              dVar27 = (double)moveinfo->RUNSPEED;
            }
            else {
              dVar27 = (double)moveinfo->WALKSPEED;
            }
          }
          else {
            dVar27 = 0.0;
          }
          (plr->obj).pad_speed = (float)dVar27;
          (plr->obj).pad_dx = local_f0.x;
          (plr->obj).pad_dz = local_f0.z;
          Awrd = NuAtan2D(local_f0.x,local_f0.z);
          (plr->obj).pad_angle = (ushort)Awrd;
        }
        if (((((((VEHICLECONTROL == 1) && (sVar16 = (plr->obj).vehicle, sVar16 != 0x3b)) &&
               (sVar16 != 0x20)) && ((sVar16 != 0x6b && (sVar16 != 0xa0)))) && (sVar16 != 0x44)) & &
            ((sVar16 != 0xb2 && (sVar16 != 0x89)))) && ((sVar16 != 0xa1 && (sVar16 != 0x99)))) {
          (plr->obj).boing = '\0';
          MoveVehicle(plr,pad);
          fVar6 = (plr->obj).pos.x - (plr->obj).oldpos.x;
          dVar28 = (double)(fVar6 * fVar6);
          fVar6 = (plr->obj).pos.z - (plr->obj).oldpos.z;
          dVar26 = (double)(fVar6 * fVar6);
          dVar27 = (double)((plr->obj).pos.y - (plr->obj).oldpos.y);
          fVar6 = NuFsqrt((float)(dVar28 + dVar26));
          (plr->obj).xz_distance = fVar6;
          fVar6 = NuFsqrt((float)((double)(float)(dVar27 * dVar27 + dVar28) + dVar26));
          (plr->obj).xyz_distance = fVar6;
          goto LAB_8001cbd4;
        }
        if (((((bool)(bVar23 >> 1 & 1)) || (sVar19 == 0xa0)) || (sVar19 == 0x99)) ||
           ((sVar19 == 0xa1 || (bVar21 = sVar19 != 0xb2, !bVar21)))) {
          bVar21 = false;
          if ((Level == 3) && (0 < SmokeyCountDownValue)) {
            bVar21 = true;
          }
          bVar1 = false;
          if ((plr->obj).ground != '\0') {
            bVar1 = (plr->obj).surface_type == '\n';
          }
          if (Level == 3) {
            fVar6 = SMOKEYSPEED;
            if (plr->fire != '\0') {
              fVar6 = SMOKEYBOOSTSPEED;
            }
            dVar27 = (double)(fVar6 * 0.01666667 * offroader_speedtime);
            if (bVar1) {
              dVar27 = (double)(float)(dVar27 * 0.25);
            }
          }
          else if (Level == 0x16) {
            plr->sprint = '\0';
            if (FireBossHoldPlayer == 0) {
              if (((uint)pad->r_dy & 0x80) != 0) {
                dVar27 = (double)moveinfo->SPRINTSPEED;
                cVar8 = '\x01';
                goto LAB_8001a604;
              }
              dVar27 = (double)moveinfo->RUNSPEED;
            }
            else {
              dVar27 = 0.0;
            }
          }
          else {
            if ((((uint)pad->r_dy & 0x88) == 0) &&
               ((sVar19 != 0x99 || (((uint)pad->r_dy & 0x40) == 0)))) {
              dVar27 = (double)moveinfo->RUNSPEED;
              cVar8 = '\0';
            }
            else {
              dVar27 = (double)moveinfo->SPRINTSPEED;
              cVar8 = '\x01';
            }
LAB_8001a604:
            plr->sprint = cVar8;
          }
          pRVar5 = best_cRPos;
          if (best_cRPos == (RPos_s *)0x0) {
            local_90[0] = (plr->obj).thdg;
          }
          else {
            local_90[0] = best_cRPos->angle;
          }
          (plr->obj).thdg = local_90[0];
          Awrd = 3;
          if ((pRVar5 != (RPos_s *)0x0) && (((pRVar5->mode & 3) != 0 || (Level == 9)))) {
            if (Level == 3) {
              uVar17 = 0x2aab;
              Awrd = 5;
            }
            else if (sVar19 == 0xa0) {
              uVar17 = 0x2000;
            }
            else if (((bool)(bVar23 >> 1 & 1)) || (sVar19 == 0xb2)) {
              uVar17 = 0x1555;
            }
            else {
              uVar17 = 0x1000;
            }
            local_68 = (double)CONCAT44(0x43300000,uVar17 ^ 0x80000000);
            iVar20 = (int)((float)(local_68 - 4503601774854144.0) * (plr->obj).pad_dx);
            local_68 = (double)(longlong)iVar20;
            if (!bVar21) {
              sVar16 = (short)iVar20;
              if (((best_cRPos->mode & 1) == 0) || (Level == 9)) {
                uVar12 = (plr->obj).thdg - sVar16;
              }
              else {
                uVar12 = (plr->obj).thdg + sVar16;
              }
              (plr->obj).thdg = uVar12;
            }
          }
          bVar21 = !bVar21;
          uVar12 = (plr->obj).hdg;
          if ((bVar21) && (Awrd != 0)) {
            uVar13 = SeekRot(uVar12,(plr->obj).thdg,Awrd);
            (plr->obj).hdg = uVar13;
          }
          Awrd = RotDiff(uVar12,(plr->obj).hdg);
          (plr->obj).dyrot = (short)Awrd;
          if (bVar21) {
            uVar17 = (uint)(plr->obj).hdg;
            local_c0 = (float)((double)NuTrigTable[uVar17] * dVar27);
            local_b8 = (float)((double)*(float *)((int)NuTrigTable +
                                                 ((uVar17 + 0x4000) * 4 & 0x3fffc)) * dVar27);
          }
          (plr->obj).dangle = '\0';
          if (bVar21) {
            if (Level == 3) {
              fVar6 = (OFFROADERSEEK - 0.25) * ((plr->obj).xz_distance / 0.05) + 0.25;
              if (fVar6 < OFFROADERSEEK) {
                fVar6 = OFFROADERSEEK;
              }
            }
            else {
              fVar6 = 0.25;
            }
            fVar15 = (plr->obj).mom.x;
            fVar29 = (plr->obj).mom.z;
            (plr->obj).mom.z = (local_b8 - fVar29) * fVar6 + fVar29;
            (plr->obj).mom.x = (local_c0 - fVar15) * fVar6 + fVar15;
          }
          else {
            (plr->obj).mom.x = 0.0;
            (plr->obj).mom.z = 0.0;
          }
          fVar6 = (plr->obj).mom.y;
          if (-TERMINALVELOCITY <= fVar6) {
            if (TERMINALVELOCITY < fVar6) {
              (plr->obj).mom.y = TERMINALVELOCITY;
            }
          }
          else {
            (plr->obj).mom.y = -TERMINALVELOCITY;
          }
          if (best_cRPos == (RPos_s *)0x0) {
            bVar1 = sVar19 == 0x20;
            (plr->obj).direction = '\0';
          }
          else {
            uVar17 = RotDiff(best_cRPos->angle,(plr->obj).hdg);
            Awrd = ((int)uVar17 >> 0x1f ^ uVar17) - ((int)uVar17 >> 0x1f);
            if (Awrd < 0x2aab) {
              cVar8 = '\0';
            }
            else {
              if (Awrd < 0x5555) {
                bVar1 = sVar19 == 0x20;
                (plr->obj).direction = '\x02';
                goto LAB_8001bb80;
              }
              cVar8 = '\x01';
            }
            bVar1 = sVar19 == 0x20;
            (plr->obj).direction = cVar8;
          }
        }
        else {
          bVar22 = (sVar19 == 0x3b) << 1;
          if ((sVar19 == 0x3b) && (best_cRPos != (RPos_s *)0x0)) {
            if ((Level == 0x1d) && (GameTimer.frame < 0xb4)) {
              local_b0.x = v000.x;
              local_b0.z = v000.z;
              local_b0.y = v000.y;
              dVar27 = 0.3330000042915344;
              (plr->obj).direction = '\0';
            }
            else {
              if (((plr->obj).pad_speed <= 0.0) || (plr->tap != '\0')) {
                fVar6 = 0.333;
                local_b0.x = v000.x;
                local_b0.z = v000.z;
                local_b0.y = v000.y;
              }
              else {
                local_b0.x = (plr->obj).pad_dx * moveinfo->WALKSPEED;
                local_b0.z = 0.0;
                local_b0.y = -(plr->obj).pad_dz * moveinfo->WALKSPEED;
                NuVecRotateY(&local_b0,&local_b0,(uint)best_railangle);
                fVar6 = 1.0;
              }
              dVar27 = (double)fVar6;
              if ((pad->old_paddata & 0x60) == 0) {
                if ((((uint)pad->r_dy & 0x60) != 0) && (Awrd = qrand(), Awrd < 0x4000)) {
                  prVar10 = &player->rumble;
                  Awrd = qrand();
                  if (Awrd < 0) {
                    Awrd = Awrd + 0x1ff;
                  }
                  NewRumble(prVar10,Awrd >> 9);
                }
              }
              else {
                NewRumble(&player->rumble,0x9f);
              }
              uVar17 = (uint)pad->r_dy & 0x60;
              if (plr->tap == '\0') {
                if (uVar17 == 0x40) {
                  if ((plr->obj).direction == '\0') {
                    local_b0.x = NuTrigTable[best_railangle] * moveinfo->RUNSPEED + local_b0.x;
                    dVar27 = 1.0;
                    local_b0.z = *(float *)((int)NuTrigTable +
                                           ((best_railangle + 0x4000) * 4 & 0x3fffc)) *
                                 moveinfo->RUNSPEED + local_b0.z;
                  }
                  else {
                    (plr->obj).direction = '\0';
                    plr->tap = '\x1e';
                  }
                }
                else if (uVar17 == 0x20) {
                  if ((plr->obj).direction == '\x01') {
                    local_b0.x = local_b0.x - NuTrigTable[best_railangle] * moveinfo->RUNSPEED;
                    dVar27 = 1.0;
                    local_b0.z = local_b0.z -
                                 *(float *)((int)NuTrigTable +
                                           ((best_railangle + 0x4000) * 4 & 0x3fffc)) *
                                 moveinfo->RUNSPEED;
                  }
                  else {
                    (plr->obj).direction = '\x01';
                    plr->tap = '\x1e';
                  }
                }
              }
            }
            uVar12 = best_railangle;
            fVar6 = (float)(dVar27 * 0.01666666753590107);
            fVar15 = (plr->obj).mom.x;
            fVar29 = (plr->obj).mom.y;
            fVar2 = (plr->obj).mom.z;
            cVar8 = (plr->obj).direction;
            (plr->obj).thdg = best_railangle;
            (plr->obj).mom.z = (local_b0.z - fVar2) * fVar6 + fVar2;
            (plr->obj).mom.x = (local_b0.x - fVar15) * fVar6 + fVar15;
            (plr->obj).mom.y = (local_b0.y - fVar29) * fVar6 + fVar29;
            if (cVar8 == '\x01') {
              (plr->obj).thdg = uVar12 + 0x8000;
            }
            Awrd = RotDiff((plr->obj).hdg,(plr->obj).thdg);
            if ((plr->tap != '\0') && (Awrd < 0)) {
              Awrd = Awrd + 0x10000;
            }
            bVar1 = false;
            (plr->obj).hdg = (plr->obj).hdg + (short)(Awrd >> 4);
          }
          else {
            if (plr->target == '\0') {
              NuVecRotateY(&local_f0,&local_f0,GameCam.yrot);
            }
            local_90[0] = (plr->obj).hdg;
            plr->fire_lock = '\0';
            Awrd = plr_target_found;
            plr_target_found = 0;
            iVar20 = (int)plr->target;
            if ((iVar20 == 0) && (plr_target_found = iVar20, plr->target == '\0')) {
              if (sVar19 == 0x20) {
                if ((0.0 < (plr->obj).pad_speed) && (local_90[0] == (plr->obj).thdg)) {
                  uVar17 = (uint)(16384.0 - NuTrigTable[(plr->obj).pad_angle] * 16384.0);
                  local_68 = (double)(longlong)(int)uVar17;
                  uVar17 = uVar17 & 0xffff;
                  uVar12 = (ushort)uVar17;
                  if (local_90[0] == 0) {
                    local_90[0] = uVar12;
                    if (0x4aab < uVar17) {
                      uVar13 = 0x4000;
LAB_8001b07c:
                      (plr->obj).thdg = uVar13;
                      local_90[0] = uVar12;
                    }
                  }
                  else {
                    uVar13 = 0x8000;
                    if (local_90[0] == 0x8000) {
                      local_90[0] = uVar12;
                      if (uVar17 < 0x3555) {
                        uVar13 = 0x4000;
                        goto LAB_8001b07c;
                      }
                    }
                    else if (uVar17 < 0xaab) {
                      (plr->obj).thdg = 0;
                      local_90[0] = uVar12;
                    }
                    else {
                      local_90[0] = uVar12;
                      if (0x7555 < uVar17) goto LAB_8001b07c;
                    }
                  }
                }
                uVar12 = (plr->obj).hdg;
                uVar13 = (plr->obj).thdg;
                uVar17 = (uint)uVar12;
                uVar4 = (uint)uVar13;
                if (uVar17 < uVar4) {
                  if ((int)(uVar4 - uVar17) < 0x2d8) {
LAB_8001b0c0:
                    (plr->obj).hdg = uVar13;
                  }
                  else {
                    (plr->obj).hdg = uVar12 + 0x2d8;
                  }
                }
                else if (uVar4 < uVar17) {
                  if ((int)(uVar17 - uVar4) < 0x2d8) goto LAB_8001b0c0;
                  (plr->obj).hdg = uVar12 - 0x2d8;
                }
              }
              else if (VEHICLECONTROL == 2) {
                if (0.0 < (plr->obj).pad_speed) {
                  uVar17 = (uint)(plr->obj).pad_angle;
                  if (uVar17 - 0x1555 < 0x5557) {
                    (plr->obj).thdg = 0;
                  }
                  else if ((uVar17 + 0x6aab & 0xffff) < 0x5557) {
                    (plr->obj).thdg = 0x8000;
                  }
                }
                Awrd = RotDiff((plr->obj).hdg,(plr->obj).thdg);
                if (0 < Awrd) {
                  Awrd = Awrd + -0x10000;
                }
                if (Awrd < -0x2d8) {
                  (plr->obj).hdg = (plr->obj).hdg - 0x2d8;
                }
                else {
                  (plr->obj).hdg = (plr->obj).thdg;
                }
              }
              else if (plr_rebound == 0) {
                if (plr->slide == 0) {
                  if (((ExtraMoves != 0) || ((Game.powerbits & 0x20) != 0)) &&
                     ((plr->slam != '\0' && ((plr->slam < '\x03' && ((plr->obj).ground == '\0')))) ))
                  {
                    (plr->obj).hdg = local_90[0] - 0xa3d;
                    goto LAB_8001b3f4;
                  }
                  if (Cursor.menu == '$') {
                    Awrd = NuAtan2D(GameCam.pos.x - (plr->obj).pos.x,
                                    GameCam.pos.z - (plr->obj).pos.z);
                    iVar20 = 3;
                    uVar13 = (plr->obj).hdg;
                    uVar12 = (ushort)Awrd;
                    (plr->obj).thdg = uVar12;
                    goto LAB_8001b3ec;
                  }
                  if (((((plr->slam_wait != 0) || ((plr->obj).pad_speed <= 0.0)) ||
                       (cVar8 = (plr->obj).dangle, cVar8 == '\x02')) ||
                      (((cVar8 != '\0' && (plr->spin != '\0')) &&
                       ((int)plr->spin_frame <
                        (int)plr->spin_frames - (int)plr->OnFootMoveInfo->SPINRESETFRAMES)))) ||
                     (((plr->jump != '\0' && (plr->jump_hold != '\0')) ||
                      ((bVar1 = sVar19 == 0x44, bVar1 && (*(short *)&plr->fire != 0))))))
                  goto LAB_8001b3f4;
                  dVar26 = (double)local_f0.x;
                  NuFabs(local_f0.x);
                  dVar28 = 0.0;
                  if (0.0 < dVar26) {
LAB_8001b2ec:
                    Awrd = NuAtan2D(local_f0.x,local_f0.z);
                    (plr->obj).thdg = (ushort)Awrd;
                  }
                  else {
                    dVar26 = (double)local_f0.z;
                    NuFabs(local_f0.z);
                    if (dVar28 < dVar26) goto LAB_8001b2ec;
                  }
                  if (((((plr->slam == '\0') || ('\x02' < plr->slam)) || ((plr->obj).ground != '\0 ')
                       ) && (((ExtraMoves == 0 && ((Game.powerbits & 4) == 0)) ||
                             ((plr->spin == '\0' ||
                              ((int)plr->spin_frames - (int)plr->OnFootMoveInfo->SPINRESETFRAMES < =
                               (int)plr->spin_frame)))))) && (plr->target == '\0')) {
                    if ((plr->crawl != '\0') ||
                       (((bVar1 || (!bVar21)) &&
                        ((plr->jump != '\0' &&
                         ((plr->jump_type == '\x02' && (plr->jump_hold == '\0')))))))) {
                      iVar20 = 4;
                    }
                    else if (((plr->tiptoe != '\0') || (bVar1)) || (!bVar21)) {
                      iVar20 = 3;
                    }
                    else {
                      iVar20 = 2;
                    }
                  }
                  else {
                    iVar20 = 5;
                  }
                  uVar13 = (plr->obj).hdg;
                  uVar12 = (plr->obj).thdg;
                }
                else {
                  uVar12 = (plr->obj).thdg;
                  iVar20 = 2;
                  uVar13 = local_90[0];
                }
LAB_8001b3ec:
                uVar12 = SeekRot(uVar13,uVar12,iVar20);
                (plr->obj).hdg = uVar12;
              }
              else {
                (plr->obj).hdg = (plr->obj).thdg;
                (plr->obj).pad_speed = 0.0;
              }
            }
            else {
              if (plr->fire == '\0') {
                if (sVar19 == 0x44) {
                  if ((plr->obj).pad_speed <= 0.0) {
                    sVar16 = (plr->obj).target_xrot;
                    iVar11 = (int)(plr->obj).target_yrot;
                    iVar20 = (int)sVar16;
                  }
                  else {
                    iVar20 = (int)(-(plr->obj).pad_dz * 5461.0);
                    sVar16 = (plr->obj).target_xrot;
                    iVar11 = (int)((plr->obj).pad_dx * 10923.0);
                    local_68 = (double)(longlong)iVar11;
                  }
                  sVar3 = (plr->obj).target_yrot;
                  plr_target_firepos.x = plr->mtxLOCATOR[8][0]._30;
                  (plr->obj).target_xrot = sVar16 + (short)(iVar20 - sVar16 >> 5);
                  (plr->obj).target_yrot = sVar3 + (short)(iVar11 - sVar3 >> 5);
                  plr_target_firepos.z = plr->mtxLOCATOR[8][0]._32;
                  plr_target_firepos.y = plr->mtxLOCATOR[8][0]._31;
                  plr_target_pos[0].x = plr_target_firepos.x;
                  plr_target_pos[0].y = plr_target_firepos.y;
                  plr_target_pos[0].z = plr_target_firepos.z;
                  NuVecRotateX(&plr_target_dir,&v001,-(uint)(ushort)(plr->obj).target_xrot & 0xfff f)
                  ;
                  NuVecRotateY(&plr_target_dir,&plr_target_dir,
                               (uint)(plr->obj).hdg + (uint)(ushort)(plr->obj).target_yrot);
                  fVar6 = MECHTARGETHACK;
                }
                else {
                  iVar20 = (int)(plr->obj).target_yrot +
                           (int)((plr->obj).pad_dx * 16384.0 * 0.01666667);
                  if (iVar20 < -0x3555) {
                    iVar11 = -0x3555 - iVar20;
                    iVar20 = -0x3555;
LAB_8001ad90:
                    (plr->obj).hdg = local_90[0] - (short)(iVar11 / 2);
                  }
                  else if (0x3555 < iVar20) {
                    iVar11 = 0x3555 - iVar20;
                    iVar20 = 0x3555;
                    goto LAB_8001ad90;
                  }
                  (plr->obj).target_yrot = (short)iVar20;
                  iVar20 = (int)((plr->obj).pad_dz * 16384.0 * 0.01666667);
                  local_68 = (double)(longlong)iVar20;
                  iVar20 = (plr->obj).target_xrot - iVar20;
                  if (iVar20 < -0x1555) {
                    iVar20 = -0x1555;
                  }
                  else if (0x2aab < iVar20) {
                    iVar20 = 0x2aab;
                  }
                  (plr->obj).target_xrot = (short)iVar20;
                  plr_target_firepos.x = plr->mtxLOCATOR[8][0]._30;
                  plr_target_firepos.z = plr->mtxLOCATOR[8][0]._32;
                  plr_target_firepos.y = plr->mtxLOCATOR[8][0]._31;
                  plr_target_pos[0].x = plr_target_firepos.x;
                  plr_target_pos[0].y = plr_target_firepos.y;
                  plr_target_pos[0].z = plr_target_firepos.z;
                  NuVecRotateX(&plr_target_dir,&v001,-(uint)(ushort)(plr->obj).target_xrot & 0xfff f)
                  ;
                  NuVecRotateY(&plr_target_dir,&plr_target_dir,
                               (uint)(plr->obj).hdg + (uint)(ushort)(plr->obj).target_yrot);
                  fVar6 = BAZOOKATARGETHACK;
                }
                plr_target_pos[0].x = plr_target_pos[0].x - plr_target_dir.x * fVar6;
                plr_target_pos[0].y = plr_target_pos[0].y - plr_target_dir.y * fVar6;
                plr_target_pos[0].z = plr_target_pos[0].z - plr_target_dir.z * fVar6;
                plr_target_sightpos._0_4_ = plr->mtxLOCATOR[8][1]._30;
                plr_target_sightpos._8_4_ = plr->mtxLOCATOR[8][1]._32;
                plr_target_sightpos._4_4_ = plr->mtxLOCATOR[8][1]._31;
              }
              GameRayCast(plr_target_pos,&plr_target_dir,10.0,plr_target_pos + 1);
              NuMtxSetRotationX(&plr_target_mtx,-(plr->obj).target_xrot);
              NuMtxRotateY(&plr_target_mtx,
                           (uint)(plr->obj).hdg + (uint)(ushort)(plr->obj).target_yrot);
              NuVecSub(&local_e0,plr_target_pos + 1,&plr_target_firepos);
              NuVecNorm(&local_e0,&local_e0);
              fVar6 = NuVecDot(&local_e0,&plr_target_dir);
              if (fVar6 <= 0.0) {
                plr->fire_lock = '\x01';
              }
              if (((plr->fire_lock == '\0') && (Awrd == 0)) && (plr_target_found != 0)) {
                GameSfx(5,pos);
              }
              GameSfxLoop(3,(nuvec_s *)0x0);
              plr_target_frame = plr_target_frame + 1;
            }
LAB_8001b3f4:
            bVar1 = sVar19 == 0x20;
            Awrd = RotDiff(local_90[0],(plr->obj).hdg);
            (plr->obj).dyrot = (short)Awrd;
            if (!bVar1) {
              if (((VEHICLECONTROL == 2) && (plr->spin != '\0')) &&
                 ((int)plr->spin_frame <
                  (int)plr->spin_frames - (int)plr->OnFootMoveInfo->SPINRESETFRAMES)) {
LAB_8001b5ec:
                dVar27 = (double)moveinfo->SPRINTSPEED;
              }
              else if (plr_rebound == 0) {
                if (((plr->slam_wait == 0) && (plr->target == '\0')) &&
                   (((plr->jump == '\0' || (plr->jump_hold == '\0')) &&
                    ((sVar19 != 0x44 || (*(short *)&plr->fire == 0)))))) {
                  if ((plr->slam == '\0') || (('\x02' < plr->slam || ((plr->obj).ground != '\0'))) )
                  {
                    cVar8 = (plr->obj).dangle;
                    if (cVar8 == '\0') {
                      if (plr->slide == 0) {
                        if (plr->crawl == '\0') {
                          if (plr->tiptoe == '\0') {
                            if ((plr->obj).wade == '\0') {
                              if (plr->sprint != '\0') {
                                if (0.0 < (plr->obj).pad_speed) goto LAB_8001b5ec;
                                goto LAB_8001b5f4;
                              }
                            }
                            else if ((plr->obj).pad_speed <= 0.0) {
LAB_8001b5f4:
                              dVar27 = 0.0;
                            }
                            else {
                              dVar27 = (double)moveinfo->WADESPEED;
                            }
                          }
                          else {
                            if ((plr->obj).pad_speed <= 0.0) goto LAB_8001b5f4;
                            dVar27 = (double)moveinfo->TIPTOESPEED;
                          }
                        }
                        else {
                          if ((plr->obj).pad_speed <= 0.0) goto LAB_8001b5f4;
                          dVar27 = (double)moveinfo->CRAWLSPEED;
                        }
                      }
                      else {
                        if ((plr->obj).character == 1) goto LAB_8001b548;
                        dVar27 = (double)moveinfo->SLIDESPEED;
                      }
                    }
                    else {
                      if (((plr->obj).pad_speed == 0.0) ||
                         (((plr->spin != '\0' &&
                           ((int)plr->spin_frame <
                            (int)plr->spin_frames - (int)plr->OnFootMoveInfo->SPINRESETFRAMES)) ||
                          (cVar8 == '\x02')))) goto LAB_8001b548;
                      dVar27 = (double)moveinfo->DANGLESPEED;
                    }
                  }
                  else {
                    dVar27 = (double)(float)(dVar27 * 0.1000000014901161);
                  }
                }
                else {
LAB_8001b548:
                  dVar27 = 0.0;
                }
              }
              else {
                dVar27 = (double)moveinfo->WALKSPEED;
              }
            }
            in_speed = (float)dVar27;
            in_s_friction = 0.005;
            in_f_friction = 0.005;
            if (bVar1) {
              in_s_friction = 0.001;
              NuVecRotateX(&local_e0,&v001,-(uint)(plr->obj).xrot);
              dVar27 = 0.0;
              if ((plr->obj).pad_speed <= 0.0) {
                local_e0.z = 0.0;
                local_e0.y = 0.0;
              }
              else {
                fVar6 = (plr->obj).pad_dx;
                dVar26 = (double)fVar6;
                NuFabs(fVar6);
                if (0.3333333432674408 <= dVar26) {
                  local_e0.z = local_e0.z * (plr->obj).pad_speed;
                }
                else {
                  local_e0.z = (float)dVar27;
                }
                if (0x8000 < (plr->obj).pad_angle) {
                  local_e0.z = -local_e0.z;
                }
                local_e0.y = local_e0.y * (plr->obj).pad_speed;
              }
              fVar6 = (plr->obj).mom.z;
              if (fVar6 <= local_e0.z) {
                if ((fVar6 < local_e0.z) &&
                   (fVar6 = fVar6 + in_s_friction, (plr->obj).mom.z = fVar6, local_e0.z < fVar6))
                goto LAB_8001b6fc;
              }
              else {
                fVar6 = fVar6 - in_s_friction;
                (plr->obj).mom.z = fVar6;
                if (fVar6 < local_e0.z) {
LAB_8001b6fc:
                  (plr->obj).mom.z = local_e0.z;
                }
              }
              fVar6 = (plr->obj).mom.y;
              if (fVar6 <= local_e0.y) {
                if ((fVar6 < local_e0.y) &&
                   (fVar6 = fVar6 + in_s_friction, (plr->obj).mom.y = fVar6, local_e0.y < fVar6))
                goto LAB_8001b740;
              }
              else {
                fVar6 = fVar6 - in_s_friction;
                (plr->obj).mom.y = fVar6;
                if (fVar6 < local_e0.y) {
LAB_8001b740:
                  (plr->obj).mom.y = local_e0.y;
                }
              }
              if (pos_START == (nuvec_s *)0x0) goto LAB_8001b980;
LAB_8001b970:
              fVar6 = pos_START->x - (plr->obj).pos.x;
            }
            else {
              if (VEHICLECONTROL != 2) {
                if (plr->slide == 0) {
                  if (plr->freeze == '\0') {
                    if ((plr->obj).ground != '\0') {
                      fVar6 = TerSurface[(plr->obj).surface_type].friction;
                      fVar15 = TerSurface[(plr->obj).surface_type].friction;
                      goto LAB_8001baf8;
                    }
                    if (((ExtraMoves != 0) || ((Game.powerbits & 4) != 0)) &&
                       ((plr->spin != '\0' &&
                        ((int)plr->spin_frame <
                         (int)plr->spin_frames - (int)plr->OnFootMoveInfo->SPINRESETFRAMES)))) {
                      fVar6 = 0.3333333;
                      goto LAB_8001ba80;
                    }
                    if (plr->jump != '\0') {
                      if (plr->jump_type == '\0') {
                        fVar6 = 0.5;
                        fVar15 = fVar6;
                      }
                      else {
                        if (((sVar19 != 0x44) && (bVar21)) ||
                           ((plr->jump_type != '\x02' || (plr->jump_hold != '\0'))))
                        goto LAB_8001bafc;
                        fVar6 = 0.25;
                        fVar15 = fVar6;
                      }
LAB_8001baf8:
                      in_s_friction = fVar15 * 0.005;
                      in_f_friction = fVar6 * 0.005;
                    }
                  }
                  else {
                    if ((plr->obj).ground == '\0') {
                      fVar6 = 0.5;
                    }
                    else {
                      fVar6 = 0.15;
                    }
                    in_s_friction = fVar6 * 0.005;
                    in_f_friction = fVar6 * 0.005;
                  }
                }
                else {
                  fVar6 = 3.0;
LAB_8001ba80:
                  in_s_friction = fVar6 * 0.005;
                  in_f_friction = in_s_friction;
                }
LAB_8001bafc:
                if (((bool)((bVar22 & 2) >> 1)) && (best_cRPos != (RPos_s *)0x0)) {
                  in_s_friction = in_s_friction * 0.1;
                  in_f_friction = in_f_friction * 0.1;
                  Awrd = NuAtan2D(local_f0.x,local_f0.z);
                  local_90[0] = (ushort)Awrd;
                  MoveLoopXZ(obj,local_90);
                }
                else {
                  MoveLoopXZ(obj,&(plr->obj).thdg);
                }
                fVar6 = (plr->obj).mom.y;
                if (-TERMINALVELOCITY <= fVar6) {
                  if (TERMINALVELOCITY < fVar6) {
                    (plr->obj).mom.y = TERMINALVELOCITY;
                  }
                }
                else {
                  (plr->obj).mom.y = -TERMINALVELOCITY;
                }
                goto LAB_8001bb80;
              }
              if (((plr->spin == '\0') || (plr->spin_frames <= plr->spin_frame)) &&
                 (plr->tap == '\0')) {
                in_s_friction = 0.0005;
              }
              NuVecRotateX(&local_e0,&v001,-(uint)(plr->obj).xrot);
              if ((plr->obj).thdg == 0x8000) {
                local_e0.z = -local_e0.z;
              }
              bVar21 = plr->spin != '\0';
              if (((bVar21) &&
                  ((int)plr->spin_frame <
                   (int)plr->spin_frames - (int)plr->OnFootMoveInfo->SPINRESETFRAMES)) ||
                 (5 < plr->tap)) {
                fVar15 = moveinfo->SPRINTSPEED;
                fVar6 = moveinfo->SPRINTSPEED;
LAB_8001b86c:
                local_e0.z = local_e0.z * fVar15;
                local_e0.y = local_e0.y * fVar6;
              }
              else {
                if (((bVar21) &&
                    ((int)plr->spin_frames - (int)plr->OnFootMoveInfo->SPINRESETFRAMES <=
                     (int)plr->spin_frame)) || ((byte)(plr->tap - 1) < 5)) {
                  fVar15 = moveinfo->RUNSPEED;
                  fVar6 = moveinfo->RUNSPEED;
                  goto LAB_8001b86c;
                }
                dVar27 = 0.0;
                if ((plr->obj).pad_speed <= 0.0) {
                  local_e0.z = 0.0;
                  local_e0.y = -0.004166667;
                }
                else {
                  fVar6 = (plr->obj).pad_dx;
                  dVar26 = (double)fVar6;
                  NuFabs(fVar6);
                  if (0.3333333432674408 <= dVar26) {
                    local_e0.z = local_e0.z * (plr->obj).pad_speed;
                  }
                  else {
                    local_e0.z = (float)dVar27;
                  }
                  local_e0.y = local_e0.y * (plr->obj).pad_speed;
                }
              }
              fVar6 = (plr->obj).mom.z;
              if (fVar6 <= local_e0.z) {
                if ((fVar6 < local_e0.z) &&
                   (fVar6 = fVar6 + in_s_friction, (plr->obj).mom.z = fVar6, local_e0.z < fVar6))
                goto LAB_8001b91c;
              }
              else {
                fVar6 = fVar6 - in_s_friction;
                (plr->obj).mom.z = fVar6;
                if (fVar6 < local_e0.z) {
LAB_8001b91c:
                  (plr->obj).mom.z = local_e0.z;
                }
              }
              fVar6 = (plr->obj).mom.y;
              if (fVar6 <= local_e0.y) {
                if ((fVar6 < local_e0.y) &&
                   (fVar6 = fVar6 + in_s_friction, (plr->obj).mom.y = fVar6, local_e0.y < fVar6))
                goto LAB_8001b960;
              }
              else {
                fVar6 = fVar6 - in_s_friction;
                (plr->obj).mom.y = fVar6;
                if (fVar6 < local_e0.y) {
LAB_8001b960:
                  (plr->obj).mom.y = local_e0.y;
                }
              }
              if (pos_START != (nuvec_s *)0x0) goto LAB_8001b970;
LAB_8001b980:
              fVar6 = 0.0;
            }
            (plr->obj).mom.x = fVar6;
          }
        }
LAB_8001bb80:
        if (((LIFTPLAYER != 0) && (((uint)pad->r_dy & 0x10) != 0)) &&
           ((plr->obj).transporting == '\0')) {
          fVar6 = (plr->obj).pos.y;
          (plr->obj).gndflags.all = 0;
          (plr->obj).pos.y = fVar6 + 0.1;
          (plr->obj).mom.y = 0.0;
          (plr->obj).ground = '\0';
        }
        if (FRAME == 0) {
          tbslotBegin(app_tbset,7);
        }
        if ((plr->obj).transporting == '\0') {
          local_d0.x = (plr->obj).pos.x;
          local_d0.y = (plr->obj).bot * (plr->obj).SCALE + (plr->obj).pos.y;
          local_d0.z = (plr->obj).pos.z;
          if ((((plr->obj).ground == '\x01') && ((plr->obj).mom.x == 0.0)) &&
             (((plr->obj).mom.z == 0.0 && (0.1 < local_d0.y - (plr->obj).shadow)))) {
            (plr->obj).gndflags.all = 0;
          }
          fVar6 = 0.0;
          if ((plr->obj).pad_speed <= 0.0) {
            fVar6 = 0.003;
          }
          fVar15 = (plr->obj).RADIUS;
          NewTerrainScaleY(&local_d0,&(plr->obj).mom,&(plr->obj).gndflags.chrs,
                           (uint)&plr[0x9eb25].lights.dir2.Direction.y / 0xce4,fVar6,fVar15,
                           (((plr->obj).top - (plr->obj).bot) * (plr->obj).SCALE) /
                           (fVar15 + fVar15));
          fVar6 = (plr->obj).SCALE;
          fVar15 = (plr->obj).bot;
          (plr->obj).pos.x = local_d0.x;
          (plr->obj).pos.y = local_d0.y - fVar15 * fVar6;
          (plr->obj).pos.z = local_d0.z;
        }
        else {
          NuVecAdd(pos,pos,&(plr->obj).mom);
        }
        if (FRAME == 0) {
          tbslotEnd(app_tbset,7);
        }
        Awrd = PlatformCrush();
        if (Awrd != 0) {
          Awrd = Awrd + -1;
          if (Awrd < 9) {
            if ((Awrd < 7) && (Awrd != 5)) {
              if (Awrd == 6) {
                Awrd = 0x13;
              }
              else {
LAB_8001bd90:
                Awrd = GetDieAnim(obj,-1);
              }
            }
            else {
              Awrd = 0x12;
            }
          }
          else if (Awrd == 9) {
            Awrd = 9;
          }
          else {
            if (Awrd != 10) goto LAB_8001bd90;
            Awrd = 0x11;
          }
          KillPlayer(obj,Awrd);
        }
        pmVar7 = (plr->obj).mask;
        if (((pmVar7 != (mask *)0x0) && (2 < pmVar7->active)) &&
           (pmVar7->active = pmVar7->active + -1, ((plr->obj).mask)->active == 2)) {
          GameMusic((int)LDATA->music[0],0);
        }
        (plr->obj).transporting = '\0';
        BonusTransporter(plr);
        DeathTransporter(plr);
        GemPathTransporter(plr);
        if ((plr->obj).dead == '\0') {
          if ((((plr->obj).transporting == '\0') && (GetTopBot(plr), sVar19 != 0x89)) &&
             (sVar19 != 0xa1)) {
            PlayerCreatureCollisions(obj);
            if ((plr->obj).dangle != '\0') {
              plr->slam = '\x03';
            }
            HitItems(obj);
          }
          if ((plr->obj).dead == '\0') {
            if ((plr->obj).transporting == '\0') {
              GetTopBot(plr);
              NewTopBot(obj);
              if ((Level == 1) &&
                 ((Awrd = PlayerObjectAnimCollision(obj,ObjTab[58].obj.special,0.4), Awrd != 0 ||
                  (Awrd = PlayerObjectAnimCollision(obj,ObjTab[59].obj.special,0.4), Awrd != 0))))  {
                KillPlayer(obj,3);
              }
            }
            if (((((plr->obj).dead == '\0') && ((plr->obj).transporting == '\0')) &&
                (GetTopBot(plr), sVar19 != 0x89)) && (sVar19 != 0xa1)) {
              CrateCollisions(obj);
            }
          }
        }
        bVar22 = (plr->obj).boing;
        if (bVar22 == 0) {
          if ((temp_crate_y_ceiling_adjust != 0) && (plr->jump != '\0')) {
            plr->jump = '\x06';
            plr->jump_frame = plr->jump_frames;
            plr->jump_type = '\x04';
          }
        }
        else {
          plr->land = '\0';
          plr->ok_slam = '\x01';
          cVar8 = '\x01';
          plr->jump = '\x01';
          plr->somersault = '\0';
          if ((bVar22 & 2) != 0) {
            cVar8 = '\x03';
          }
          plr->jump_type = cVar8;
          sVar16 = moveinfo->JUMPFRAMES1;
          plr->jump_frame = 0;
          plr->jump_frames = sVar16 + 1;
          fVar6 = moveinfo->JUMPHEIGHT;
          local_68 = (double)CONCAT44(0x43300000,(int)moveinfo->JUMPFRAMES2 ^ 0x80000000);
          if (cVar8 == '\x03') {
            fVar6 = fVar6 * 1.5;
          }
          (plr->obj).mom.y = fVar6 / (float)(local_68 - 4503601774854144.0);
          (plr->obj).anim.anim_time = 1.0;
          (plr->obj).ground = '\0';
        }
        if (((temp_crate_y_floor_adjust != 0) || (temp_crate_y_ceiling_adjust != 0)) ||
           (temp_crate_xz_adjust != 0)) {
          (plr->obj).pos_adjusted = '\x01';
        }
        goto LAB_8001c028;
      }
    }
    if (sVar19 == 0x20) {
      MoveSUBMARINE(plr,pad);
    }
    else if ((bool)(bVar23 >> 1 & 1)) {
      MoveSCOOTER(plr,pad);
    }
    else if (sVar19 == 0xa0) {
      MoveSNOWBOARD(plr,pad);
    }
    else if (sVar19 == 0x44) {
      MoveMECH(plr,pad);
    }
    else if (sVar19 == 0xb2) {
      MoveFIREENGINE(plr,pad);
    }
    else if (sVar19 == 0x3b) {
      MoveGYRO(plr,pad);
    }
    else if (sVar19 == 0x89) {
      MoveMINECART(plr,pad);
    }
    else if (sVar19 == 0xa1) {
      MoveMINETUB(plr,pad);
    }
    else if (sVar19 == 0x99) {
      MoveOFFROADER(plr,pad);
    }
    else if ((plr->obj).character == 1) {
      MoveCOCO(plr,pad);
    }
    else if (VEHICLECONTROL == 2) {
      MoveSWIMMING(plr,pad);
    }
    else {
      MoveCRASH(plr,pad);
    }
    (plr->obj).boing = '\0';
  }
LAB_8001cbd4:
  if (((plr->obj).dead != '\0') && (InvincibilityCHEAT != 0)) {
    (plr->obj).dead = '\0';
    plr_lives.count = (short)Game.lives;
  }
  if (Adventure != 0) {
    Game.wumpas = (uchar)plr_wumpas.count;
    Game.lives = (uchar)plr_lives.count;
    pmVar7 = (plr->obj).mask;
    if ((pmVar7 != (mask *)0x0) && (Game.mask = '\x02', pmVar7->active < 3)) {
      Game.mask = *(uchar *)((int)&pmVar7->active + 1);
    }
  }
  BonusTiming(plr);
  fVar6 = (plr->obj).pos.x - (plr->obj).oldpos.x;
  dVar28 = (double)(fVar6 * fVar6);
  fVar6 = (plr->obj).pos.z - (plr->obj).oldpos.z;
  dVar26 = (double)(fVar6 * fVar6);
  dVar27 = (double)((plr->obj).pos.y - (plr->obj).oldpos.y);
  fVar6 = NuFsqrt((float)(dVar28 + dVar26));
  (plr->obj).xz_distance = fVar6;
  fVar29 = NuFsqrt((float)((double)(float)(dVar27 * dVar27 + dVar28) + dVar26));
  fVar15 = vtog_duration;
  fVar6 = vtog_time;
  sVar19 = (plr->obj).anim.action;
  (plr->obj).xyz_distance = fVar29;
  (plr->obj).anim.oldaction = sVar19;
  if ((fVar6 < fVar15) && (vtog_blend != 0)) {
    AnimateDIVE(plr,fVar6 / fVar15);
    goto LAB_8001ce44;
  }
  if (VEHICLECONTROL == 1) {
    sVar19 = (plr->obj).vehicle;
    if (sVar19 == 0x53) {
      AnimateATLASPHERE(plr);
      goto LAB_8001ce44;
    }
    if (sVar19 == 0x20) {
      AnimateSUBMARINE(plr);
      goto LAB_8001ce44;
    }
    if (sVar19 == 0x6b) {
      AnimateSCOOTER(plr);
      goto LAB_8001ce44;
    }
    if (sVar19 == 0xa0) {
      AnimateSNOWBOARD(plr);
      goto LAB_8001ce44;
    }
    if (sVar19 == 0x36) {
      AnimateGLIDER(plr);
      goto LAB_8001ce44;
    }
    if (sVar19 == 0x81) {
      AnimateDROPSHIP(plr);
      goto LAB_8001ce44;
    }
    if (sVar19 == 0x3b) {
      AnimateGYRO(plr,pad);
      goto LAB_8001ce44;
    }
    if (sVar19 == 0x44) {
      AnimateMECH(plr);
      goto LAB_8001ce44;
    }
    if (sVar19 == 0xb2) {
      AnimateFIREENGINE(plr);
      goto LAB_8001ce44;
    }
    if (sVar19 == 99) {
      AnimateJEEP(plr);
      goto LAB_8001ce44;
    }
    if (sVar19 == 0x89) {
      AnimateMINECART(plr);
      goto LAB_8001ce44;
    }
    if (sVar19 == 0xa1) {
      AnimateMINETUB(plr);
      goto LAB_8001ce44;
    }
    if (sVar19 == 0x8b) {
      AnimateMOSQUITO(plr);
      goto LAB_8001ce44;
    }
    if (sVar19 == 0x99) {
      AnimateOFFROADER(plr);
      goto LAB_8001ce44;
    }
  }
  if (VEHICLECONTROL == 2) {
    AnimateSWIMMING(plr);
  }
  else if ((plr->obj).character == 1) {
    AnimateCOCO(plr);
  }
  else {
    AnimateCRASH(plr);
  }
LAB_8001ce44:
  cVar14 = (plr->obj).dead;
  if (cVar14 == '\0') {
    if ((VEHICLECONTROL == 2) && (CRemap[115] != -1)) {
      mod = CModel + CRemap[115];
    }
    else {
      mod = (plr->obj).model;
    }
  }
  else {
    mod = CModel + (plr->obj).die_model[0];
  }
  if ((cVar14 != '\x16') && (cVar14 != '\x04')) {
    UpdateAnimPacket(mod,&(plr->obj).anim,0.5,(plr->obj).xz_distance);
  }
  (plr->obj).frame = (plr->obj).frame + 1;
  if ((Cursor.menu == -1) && (GameMode != 1)) {
    if (Level == 0x25) {
      Awrd = InLoadSaveZone(plr);
      if (Awrd != 0) {
        if ((pad == (nupad_s *)0x0) || ((pad->old_paddata & 0x40) == 0)) {
          loadsave_frame = loadsave_frame + 1;
        }
        else {
          loadsave_frame = 0x3c;
        }
        Awrd = loadsave_frame;
        if (0x3c < loadsave_frame) {
          loadsave_frame = 0x3d;
          Awrd = loadsave_frame;
        }
      }
      loadsave_frame = Awrd;
      if (loadsave_frame == 0x3c) {
        NewMenu(&Cursor,0x15,4,-1);
        loadsave_frame = 0x3d;
        GameSfx(0x36,(nuvec_s *)0x0);
      }
    }
  }
  else {
    loadsave_frame = 0;
  }
  return;
}