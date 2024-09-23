#include "gamecode/creature.h"

s32 gamecut; //cut.c

#define ABS(x) (x >= 0 ? x : -x)

/*
	//TODO
	ManageCreatures 88%
	LoadCharacterModel 95%
	LoadCharacterModels 92%
	MovePlayer 29%**
	DrawCharacterModel 96%
	UpdateAnimPacket 99%*
	DrawCreatures 89%
	
*/

/*
//NGC MATCH
void ResetPlayer(s32 set) {
    struct creature_s* c;
    s32 start;
    s32 water;
    float y;

    if (PLAYERCOUNT != 0) {
        if (set != 0) {
            if ((Level != 0x11) || (cp_iALONG < 0x6b)) {
                edobjResetAnimsToZero();
            }
            c = player;
            PlayerStartPos(player, &(player->obj).startpos);
            start = 0;
            if ((bonus_restart != 0)
                && ((((LDATA->vBONUS).x != 0.0f || ((LDATA->vBONUS).y != 0.0f)) || ((LDATA->vBONUS).z != 0.0f))))
            {
                (c->obj).pos = (LDATA->vBONUS);
            } else {
                if (cp_goto != -1) {
                    (c->obj).pos = cpGOTO;
                } else {
                    if (cp_iRAIL != -1) {
                        (c->obj).pos = cpPOS;
                    } else {
                        start = 1;
                        (c->obj).pos = (c->obj).startpos;
                    }
                }
            }
            if (bonus_restart != 0) {
                bonus_restart = 0;
            }
            if (((Level == 2) && (cp_iRAIL == 0)) && (cp_iALONG == 0x23)) {
                water = 2;
            } else if ((LBIT & 0x0000000400000040) || ((Level == 2 && start))) {
                water = 1;
            } else {
                water = 0;
            }
            ResetPlayerMoves(c);
            GetTopBot(c);
            NewTopBot(&c->obj);
            OldTopBot(&c->obj);
            y = NewShadowPlat(&(c->obj).pos, 0.0f);
            GetSurfaceInfo(&c->obj, 1, y);
            if (cp_goto != -1) {
                cp_goto = -1;
            } else {
                if (water == 2) {
                    (c->obj).pos.y -= CData[32].min.y * (c->obj).SCALE;
                } else if (water != 0) {
                    if (cp_iRAIL != -1) {
                        (c->obj).pos.y -= (c->obj).bot * (c->obj).SCALE;
                    }
                } else if ((Level != 0x1d) && (y != 2000000.0f)) {
                    (c->obj).pos.y = (y - ((c->obj).bot * (c->obj).SCALE));
                }
            }
            (c->obj).oldpos = (c->obj).pos;
            (c->obj).mom = v000;
            ComplexRailPosition(&(c->obj).pos, (s32)(c->obj).RPos.iRAIL, (s32)(c->obj).RPos.iALONG, &(c->obj).RPos, 1);
            if ((VEHICLECONTROL == 2) || ((VEHICLECONTROL == 1 && ((c->obj).vehicle == 0x20)))) {
                (c->obj).thdg = (c->obj).hdg = 0;
            } else if (Level == 0x19) {
                (c->obj).hdg = 0x4000;
            } else if (best_cRPos != NULL) {
                (c->obj).hdg = (c->obj).thdg = best_cRPos->angle;
            }
            
            switch (c->obj.vehicle) {
                case 0x53:
                    ResetAtlas(c);
                    break;
                case 0x36:
                    if (Level == 0x18) {
                        SetWeatherStartPos(c);
                    }
                    break;
                case 99:
                    ResetJeep(c);
                    break;
            }
            
            plr_rebound = 0;
            ATLASCAMHEIGHT = 2.5f;
            ResetLights(&c->lights);
            
            if (((c->obj).mask != NULL) && ((c->obj).mask->active != 0)) {
                if ((c->obj).mask->active > 2) {
                    (c->obj).mask->active = 2;
                }
                ResetLights(&((c->obj).mask)->lights);
            }
            
        }
        plr_crates.count = plr_crates.draw = 0;
        plr_crates.frame = 0;
        (player->obj).scale = 1.0f;
        (player->obj).dead = 0;
        (player->obj).finished = 0;
        if ((((player->obj).mask != NULL) && ((player->obj).mask->active == 0)) && (LivesLost > 4)) {
            NewMask((player->obj).mask, &(player->obj).pos);
        }
    }
    if ((Demo == 0) && (GameMode != 1)) {
        force_panel_wumpa_update = 60;
        force_panel_crate_update = 60;
        force_panel_lives_update = 60;
        force_panel_items_update = 60;
    }
    boss_dead = 0;
    check_duration = 0.0f;
    check_time = 0.0f;
    point_duration = 0.0f;
    point_time = 0.0f;
}

//PS2 MATCH
void ResetPlayerMoves(struct creature_s *c) {
    c->jump = 0;
    c->slam = 0;
    c->spin = 0;
    c->crawl = 0;
    c->tiptoe = 0;
    c->sprint = 0;
    c->somersault = 0;
    c->land = 0;
    c->idle_mode = 0;
    c->idle_sigh = 0;
    c->crawl_lock = 1;
    c->crouch_pos = 0;
    c->slam_wait = 0;
    c->spin_wait = 0;
    c->slide = 0;
    c->idle_action = 0x22;
    c->idle_wait = IDLEWAIT * 30.0f;
    c->obj.idle_gametime = 0.0f;
    c->idle_time = 0.0f;
    c->target = 0;
    c->fire = 0;
    c->tap = 0;
    c->freeze = 0;
    c->obj.transporting = 0;
    
    ResetAnimPacket(&c->obj.anim, 0x22);
    
    c->obj.frame = 0;
    c->obj.attack = 0;
    c->obj.dyrot = 0;
    c->obj.boing = 0;
    c->obj.dangle = 0;
    c->obj.old_ground = 3;
    c->obj.submerged = 0;
    c->obj.SCALE = 1.0f;
    c->obj.scale = 1.0f;
    c->obj.RADIUS = c->obj.radius;
    c->obj.ground = 3;
    return;
}
*/
//NGC MATCH
void RemoveCreature(struct creature_s *c) {
  RemoveGameObject(&c->obj);
  c->used = 0;
  return;
}

/*
//NGC MATCH
void CloseCreatures(void) {
  s32 i;
  
  for(i = 0; i < 9; i++) {
        if (Character[i].used != 0) {
          RemoveCreature(&Character[i]);
        }
  }
  for(i = 0; i < 0x31; i++) {
        if (CModel[i].hobj != NULL) {
          NuHGobjDestroy(CModel[i].hobj);
          CModel[i].hobj = 0;
        }
  }
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
*/

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

volatile int crash_loaded; //main.c

//95% NGC
s32 LoadCharacterModel(s32 character, s32 level, s32* cmodel_index, s32 clist_entry, s32* remap) {
    s32 k;
    s32 iVar2;
    struct CharacterModel* model;
    struct animlist* anim;
    s32 cnt;
    char path[64];
    struct cdata_s* cdata;
    struct {
        s32 character;
        struct animlist animlist[5];
    } * space;

    model = &CModel[*cmodel_index];
    if (level == 0x28) {
        space = (struct space_s*)SpaceGameCutTab[0][gamecut * 2];
    } else {
        space = NULL;
    }
    if ((character != 0xff) && (0x31 > *remap)) {

        memset(model, 0, sizeof(struct CharacterModel));
        strcpy(path, "chars\\");
        cdata = &CData[character];
        strcat(path, cdata->path);
        strcat(path, "\\");
        if (model->hobj == NULL) {
            strcpy(tbuf, path);
            strcat(tbuf, cdata->file);
            strcat(tbuf, ".hgo");
            model->hobj = NuHGobjRead(&superbuffer_ptr, tbuf);
            if (character == 0 || character == 1) {
                if (model->hobj == NULL)
                    goto LAB_800185e0;
                crash_loaded = 1;
            }
            if (model->hobj == NULL)
                goto LAB_800185e0;
        }
        for (cnt = 0; cnt < 16; cnt++) {
            model->pLOCATOR[cnt] = NuHGobjGetPOI(model->hobj, (u8)cnt);
        }
        if ((character == 0x54) || (character == 0x9f)) {
            strcpy(path, "chars\\");
            strcat(path, CData[0].path);
            strcat(path, "\\");
        }
        anim = cdata->anim;
        if (level == 0x28) {
            anim = space[clist_entry].animlist;
        } else if (character == 0) {
            //(0x01002000)
            // 0x00001000
            if ((LBIT & 0x0000001001002000) && (level != 0x1e)) {
                anim = CrashAnim_GLIDER;
            } else {
                if ((LBIT & 0x40000) != 0) {
                    anim = CrashAnim_MOSQUITO;
                } else if ((LBIT & 0x0000000100210801) != 0) {
                    anim = CrashAnim_ATLASPHERE;
                } else if (level == 0x1d) {
                    anim = CrashAnim_GYRO;
                } else if (level == 0x1c) {
                    anim = CrashAnim_MINETUB;
                } else if (level == 0x2b) {
                    anim = CrashAnim_CREDITS;
                }
            }
        } else if ((character == 1) && (LBIT & 0x4000000)) {
            anim = CocoAnim_DROPSHIP;
        }
        while ((anim != NULL) && (anim->file != NULL) && (anim->action >= 0) && anim->action < 0x76) {
            if (anim->levbits & LBIT) {
                if (((anim->flags & 2) != 0) && (model->anmdata[anim->action]) == 0) {
                    strcpy(tbuf, path);
                    strcat(tbuf, anim->file);
                    strcat(tbuf, ".ani");
                    model->anmdata[anim->action] = InstAnimDataLoad(tbuf);
                    if (model->anmdata[anim->action] != 0) {
                        model->animlist[anim->action] = anim;
                    }
                }
                if (((anim->flags & 4) != 0) && ((model->fanmdata[anim->action]) == 0)) {
                    strcpy(tbuf, path);
                    strcat(tbuf, anim->file);
                    strcat(tbuf, ".bsa");
                    model->fanmdata[anim->action] = InstAnimDataLoad(tbuf);
                    if (model->fanmdata[anim->action] != 0) {
                        model->fanimlist[anim->action] = anim;
                    }
                }
            }
            anim = anim + 1;
        }
        model->character = character;
        CRemap[character] = cmodel_index[0];
        *cmodel_index = *cmodel_index + 1;
        *remap = *remap + 1;
    LAB_800185e0:	//Problem with this for loop
        for (k = 0; k < 26; k++) {
            if (((Font3DObjTab[k].flags & 1)) && (Font3DObjTab[k].i == character)) {
                CLetter[character] = (char)k + 'a';
                k = 26;
            }
        }
        return 1;
    }
    return 0;
}

//NGC MATCH
void PurgeCharacterModels(void) {
  s32 i;

  for (i = 0; i < 0xbf; i++) {
    CRemap[i] = -1;
    CLetter[i] = '?';
  }
    i = 48;
  while (i >= 0) {
    CModel[i].hobj = NULL;
    i--;
  }
  return;
}

//92%
void LoadCharacterModels(void) {
    s32 i;
    s32 j;
    s32 character;
    s32 cmodel_index;
    // struct nudathdr_s* dfanim;
    struct space_s *space;
    char charsdat_filename[128];
    s32 clist_entry;
    s32 remap;


    if(CModel[0].hobj != NULL)
    {
        clist_entry = 1;
    } else {
        clist_entry = 0;
    }

    if (Level == 0x28) {
        space = (struct space_s *)SpaceGameCutTab[0][gamecut * 2];
    }
    else {
        space = NULL;
    }
   if (LDATA->clist != NULL) {
        i = clist_entry;
        remap = i;
        cmodel_index = i;
        while( 1 ) {
            force_decal = 0;
            if (space != NULL) {
                // fakery
                // j = i << 5;
                // j &= 0xFFFFFFF0;
                // character = ((s32*)space)[j];

                character = space[i].character;
            }
            else {
                character = LDATA->clist[i];
            }

            if ((character == 0x1b) || (character == 0x86)) {
                force_decal = 1;
            }
            if (LoadCharacterModel(character, Level, &cmodel_index, i, &remap) == 0)
            {
                break;
            }
            i++;
        }
    }

    CrashMoveInfo.JUMPLANDFRAMES   = ModelAnimDuration(0,   0x30, 0.0f, 0.0f) * 60.0f;
    CrashMoveInfo.SLAMWAITFRAMES   = ModelAnimDuration(0,   0x1f, 0.0f, 0.0f) * 60.0f;
    CrashMoveInfo.SOMERSAULTFRAMES = ModelAnimDuration(0,   0x44, 0.0f, 0.0f) * 60.0f;
    CrashMoveInfo.SLIDEFRAMES      = ModelAnimDuration(0,   0x43, 0.0f, 0.0f) * 60.0f;
    CrashMoveInfo.CROUCHINGFRAMES  = ModelAnimDuration(0,   3,    0.0f, 0.0f) * 60.0f;
    CocoMoveInfo.JUMPLANDFRAMES    = ModelAnimDuration(1,   0x30, 0.0f, 0.0f) * 60.0f;
    CocoMoveInfo.SLAMWAITFRAMES    = ModelAnimDuration(1,   0x1f, 0.0f, 0.0f) * 60.0f;
    CocoMoveInfo.SOMERSAULTFRAMES  = ModelAnimDuration(1,   0x44, 0.0f, 0.0f) * 60.0f;
    CocoMoveInfo.SPINFRAMES        = ModelAnimDuration(1,   0x46, 0.0f, 0.0f) * 60.0f;
    CocoMoveInfo.SLIDEFRAMES       = ModelAnimDuration(1,   0x43, 0.0f, 0.0f) * 60.0f;
    MineCartMoveInfo.JUMPFRAMES0   = ModelAnimDuration(0x89,  99, 0.0f, 0.0f) * 60.0f;
    return;
}

//NGC MATCH
void ChangeCharacter(struct creature_s *c,s32 character) {

  struct cdata_s* cdata;
  
  if (((u32)character < 0xbf) && (CRemap[character] != -1)) {
    (c->obj).character = character;
    cdata = &CData[character];
    if ((s16)character == 0) {
      c->OnFootMoveInfo = &CrashMoveInfo;
    }
    else if ((s16)character == 1) {
      c->OnFootMoveInfo = &CocoMoveInfo;
    }
    else {
      c->OnFootMoveInfo = &DefaultMoveInfo;
    }
    (c->obj).model = &CModel[CRemap[character]];
    (c->obj).radius = (c->obj).RADIUS = cdata->radius;
    (c->obj).min = cdata->min;
    (c->obj).max = cdata->max;
    (c->obj).bot = (c->obj).min.y;
    (c->obj).top = (c->obj).max.y;
    if (c == player) {
      ResetPlayerMoves(c);
    }
    else {
      ResetAnimPacket(&(c->obj).anim,0x22);
    }
  }
  return;
}

//NGC MATCH
void PlayerStartPos(struct creature_s *c,struct nuvec_s *pos) {

  if (Level == 0x25) {
        if (gamecut_hack == 1) {
          gamecut_hack = 0;
          last_hub = 0;
          last_level = 0x15;
        }
        if (GameMode == 1) {
          *pos = cutpos_CRASH;
          (c->obj).hdg = cutang_CRASH;
          tumble_action = -1;
          tumble_duration = 0.0f;
          tumble_time = 0.0f;
          last_level = -1;
          last_hub = -1;
          return;
        }
        if (((last_hub != -1) && (last_level != -1)) && (LData[last_level].hub != -1)) {
          HubStart(&c->obj,last_hub,last_level,pos);
          return;
        }
  }
  if (pos_START != NULL) {
      *pos = *pos_START;
      if (Level == 0x25) {
        (c->obj).hdg = 0x8000;
        tumble_action = -1;
        tumble_duration = 0.0f;
        tumble_time = 0.0f;
        last_level = -1;
        last_hub = -1;
      } 
  } else {
     *pos = v000;
  }
  return;
}

//NGC MATCH
s32 AddCreature(s32 character,s32 index,s32 i_aitab) {
  struct nuanimdata_s **anmdat;
  s32 i;
  struct creature_s *c;
  struct creatcmd_s *commands;
  struct nuvec_s startpos;
  struct cdata_s * cdata;
  
  if (((((u32)index < 9) && ((u32)character < 0xbf)) &&
      ((i_aitab == -1 || (character == AIType[AITab[i_aitab].ai_type].character)))) && (CRemap[character] != -1)) {
   
    c = &Character[index];
    if (c->used != 0) {
      return 0;
    }
    memset(c,0,0xce4);
    if (AddGameObject(&c->obj,c) == 0) {
        return 0;
    }
      else {
      c->obj.pLOCATOR = &c->mtxLOCATOR[0][0];
      c->used = 1;
      c->on = 1;
      c->off_wait = 2;
      cdata = &CData[character];
      if (i_aitab == -1) {
        commands = NULL;
        PlayerStartPos(c,&startpos);
      }
      else {
        commands = AIType[AITab[i_aitab].ai_type].cmd;
        startpos = AITab[i_aitab].pos[commands->i];
      }
      c->i_aitab = (char)i_aitab;
      c->cmdcurr = commands;
      c->cmdtable = commands;
      c->obj.character = character;
      if (c->obj.character == 0) {
        c->OnFootMoveInfo = &CrashMoveInfo;
      }
      else if (c->obj.character == 1) {
        c->OnFootMoveInfo = &CocoMoveInfo;
      }
      else {
        c->OnFootMoveInfo = &DefaultMoveInfo;
      }
      c->obj.RADIUS = cdata->radius;
      c->obj.radius = cdata->radius;
      c->obj.model = &CModel[CRemap[character]];
      c->obj.min = cdata->min;
      c->obj.max = cdata->max;
      c->obj.bot = c->obj.min.y;
      c->obj.top = c->obj.max.y;
      c->obj.startpos = startpos;
      c->obj.oldpos = c->obj.startpos;
      c->obj.pos = c->obj.oldpos;
      c->obj.flags = 2;
      if ((Adventure != 0) && (c == player)) {
        c->obj.vehicle = LDATA->vehicle;
        c->obj.flags = 3;
        plr_lives.draw = (short)Game.lives;
        plr_wumpas.draw = (short)Game.wumpas;
        plr_lives.count = plr_lives.draw;
        plr_wumpas.count = plr_wumpas.draw;
        c->obj.mask = Mask;
        c->pad_type = 1;
      }
      else {
        c->obj.vehicle = -1;
        c->obj.flags = c->obj.flags | 4;
        c->pad_type = 0;
        c->obj.mask = NULL;
      }
      c->obj.die_duration = 0.5f;
      c->ai.oldpos = c->obj.pos;
      c->obj.anim.action = 0x22;
      if (c->obj.model != NULL) {
          anmdat = c->obj.model->anmdata;
          if (anmdat[0x22] == NULL ) {
            for (i = 0; c->obj.model->anmdata[i] == NULL; ) {
              i++;
              if (0x75 < i) goto LAB_80018fa8;
            }
            if (i < 0x76) {
              c->obj.anim.action = (short)i;
            }
          }
      }
LAB_80018fa8:
      c->obj.anim.newaction = c->obj.anim.action;
      c->obj.anim.oldaction = c->obj.anim.action;
      ResetLights(&c->lights);
      }
  }
    else{
        return 0;
    }
  return 1;
}

//NGC MATCH
void TerrainFailsafe(struct obj_s *obj) {
  if (obj->shadow != 2000000.0f) {
    return;
  }
  obj->shadow = SAFEY;
  if (obj->bot * obj->SCALE + (obj->pos).y < SAFEY) {
   (obj->pos).y = SAFEY - (obj->min).y * obj->SCALE;
  }
  return;
}

//NGC MATCH
s32 NewCharacterIdle(struct creature_s* c, struct CharacterModel* model) {
    s32 i;
    s32 ok;
    s32 count;
    s32 list[118];
    s32 sfx;

    if (GameMode != 1) {
        if ((c->idle_mode == 0) && (c->idle_sigh == 0)) {
            c->idle_sigh = 1;
            if (model->anmdata[0x3d] != NULL) {
                c->idle_repeat = 1;
                c->idle_action = 0x3d;
                sfx = 0x22;
                goto New;
            }
        }
        count = 0;
        for (i = 0; i < 0x76; i++) {
            if ((model->anmdata[i] != 0) && ((model->animlist[i]->flags & 8) != 0)) {
                ok = 1;
                if ((c == player && c->obj.character == 0)) {
                    if (GameMode == 1 && (i == 0x25 || i == 0x26 || i == 0x27)) {
                        ok = 0;
                    }
                    else if (i == 0x27 && (abs(RotDiff(GameCam->hdg_to_player, c->obj.hdg)) < 0x6000)) {
                        ok = 0;
                    }
                    else if (i == 0x29 && (GemPath == 1 || GemPath == 3 || Death == 1 || Death == 3 || Bonus == 1 || Bonus == 3)) {
                        ok = 0;
                    }
                }
                
                if (ok) {
                    list[count++] = i;
                }
            }
        }
        
        if (count < 1)
            return 0;

    Retry:
        sfx = -1;
        i = (count <= 1) ? 0 : qrand() / (0xffff / count + 1);
        c->idle_repeat = 1;
        c->idle_action = list[i];
        if (c->obj.character == 0) {
            switch (c->idle_action) {
                case 0x29:
                    c->idle_repeat = (char)(qrand() / 0x4000) + 2;
                    break;
                case 0x28:
                    c->idle_repeat = (char)(qrand() / 0x2000) + 8;
                    sfx = 0x10;
                    break;
                case 0x3d:
                    sfx = 0x22;
                    break;
                case 0x27:
                    break;
            }
        }
        if ((count >= 2) && (c->idle_action == c->old_idle_action)) {
            goto Retry;
        }

    New:
        c->idle_mode = 1;
        c->old_idle_action = c->idle_action;
        if ((1 < c->idle_repeat) && ((model->animlist[c->idle_action]->flags & 1) == 0)) {
            c->idle_repeat = 1;
        }
        c->idle_time = 0.0f;
        c->idle_wait = (model->anmdata[c->idle_action]->time - 1.0f) * c->idle_repeat; 
        i = model->animlist[c->idle_action]->blend_out_frames;
        if (i != 0) {
            c->idle_wait -= i * 0.5f;
            if (c->idle_wait < 1.0f) {
                c->idle_wait = 1.0f;
            }
        }
        if (sfx != -1) {
            GameSfx(sfx, &c->obj.pos);
        }
        return 1;
    }
    return 0;
}

//NGC MATCH
void UpdateCharacterIdle(struct creature_s* c, s32 character) {

    struct CharacterModel* model;
    float t;
    s32 i;

    i = CRemap[character];
    if (i != -1) {
        model = &CModel[i];
        if (((c->obj).anim.newaction == 0x22)
            && ((c->spin == 0 || ((s32)c->spin_frame >= (s32)c->spin_frames - (s32)c->OnFootMoveInfo->SPINRESETFRAMES))))
        {
            (c->obj).idle_gametime = (c->obj).idle_gametime + 0.01666667f;
            t = 0.5f;
            if (model->anmdata[c->idle_action] != 0) {
                t *= model->animlist[c->idle_action]->speed;
            }
            c->idle_time += t;
            switch (c->idle_mode) {
                case 0:
                    if ((c->idle_time > c->idle_wait)) {
                        i = NewCharacterIdle(c, model);
                        if (i == 0) {
                            goto StartIdle;
                        }
                    }
                    break;
                case 1:
                    if ((c->idle_time > c->idle_wait)) {
                        if (((LBIT & 0x200000A1) != 0) && ((c->obj).character == 0)) {
                            i = NewCharacterIdle(c, model);
                            if (i == 0) {
                                goto StartIdle;
                            }
                        } else {
                            c->idle_mode = 0;
                            c->idle_action = 0x22;
                            c->idle_time = 0.0f;
                            c->idle_wait = IDLEWAIT * 30.0f;
                            break;
                        }
                    }
                    break;
            }
            (c->obj).anim.newaction = c->idle_action;
            return;
        }
    }
StartIdle:
    c->idle_mode = 0;
    c->idle_sigh = 0;
    c->idle_action = 0x22;
    c->old_idle_action = -1;
    (c->obj).idle_gametime = 0.0f;
    c->idle_time = 0.0f;
    c->idle_wait = IDLEWAIT * 30.0f;
    return;
}

//NGC MATCH
void UpdateRumble(struct rumble_s *rumble) {
  if (rumble->buzz != '\0') {
    rumble->buzz = rumble->buzz - 1;
  }
  if (rumble->frame == '\0') {
    return;
  }
  rumble->frame = rumble->frame - 1;
  return;
}

//NGC MATCH
void NewRumble(struct rumble_s *rumble,s32 power) {
  if ((rumble->frame != 0) && (power <= (rumble->power * rumble->frame) / rumble->frames)) {
    return;
  }
  rumble->power = (u8)power;
  rumble->frames = (power * 0x3c) >> 8;
  rumble->frame = (power * 0x3c) >> 8;
  return;
}

//NGC MATCH
void NewBuzz(struct rumble_s *rumble,s32 frames) {
  if (frames <= (s32)rumble->buzz) {
    return;
  }
  rumble->buzz = (u8)frames;
  return;
}

//NGC MATCH
void ProcessCreatures(void) {
  struct creature_s *c;
  s32 i;
  struct nuvec_s pos;
  
  if (FRAME == 0) {
    tbslotBegin(app_tbset,5);
  }
  c = &Character[0];
  GetTopBot(c);
  NewTopBot(&c->obj);
  c++;
  for (i = 0; i < 8; i++, c++) {
    if (c->on != '\0') {
      c->obj.pos_adjusted = '\0';
      c->obj.got_shadow = '\0';
      c->obj.old_SCALE = c->obj.SCALE;
      MoveCreature(c);
      if ((USELIGHTS != 0) && (LIGHTCREATURES != 0)) {
        pos.x = c->obj.pos.x;
        pos.y = (((c->obj.bot + c->obj.top) * c->obj.SCALE) * 0.5f + c->obj.pos.y);
        pos.z = c->obj.pos.z;
        GetLights(&pos,&c->lights,1);
      }
      if (c->obj.dead == '\x01') {
        c->obj.scale = (1.0f - (c->obj.die_time / c->obj.die_duration)) * c->ai.scale;
      }
      else {
        c->obj.scale = c->ai.scale;
      }
      c->obj.SCALE = c->obj.scale * CData[c->obj.character].scale;
      c->hit_type = '\0';
      c->obj.RADIUS = c->obj.radius * c->obj.SCALE;
    }
  }
  if ((FRAME == 0) && (tbslotEnd(app_tbset,5), FRAME == 0)) {
    tbslotBegin(app_tbset,6);
  }
  c = &Character[0];
  if ((PLAYERCOUNT != 0) && (c->on != '\0')) {
    c->obj.pos_adjusted = '\0';
    c->obj.got_shadow = '\0';
    c->obj.old_SCALE = c->obj.SCALE;
    MovePlayer(c,Pad[0]);
    if ((USELIGHTS != 0) && (LIGHTCREATURES != 0)) {
      pos.x = c->obj.pos.x;
      pos.y = (c->obj.bot + c->obj.top) * c->obj.SCALE * 0.5f +
              c->obj.pos.y;
      pos.z = c->obj.pos.z;
      GetLights(&pos,&c->lights,1);
    }
    i = cRPosCOUNT;
    if ((((level_part_2 == 0) &&
         (ComplexRailPosition(&c->obj.pos,(int)c->obj.RPos.iRAIL,
                              (int)c->obj.RPos.iALONG,&c->obj.RPos,1),
         cRPosCOUNT != i)) && (GameCam.mode != -1)) && (c->obj.transporting == '\0')) {
      BlendGameCamera(&GameCam,0.5f);
    }
    if (((c->obj.mask != NULL) && ((c->obj.mask)->active != 0)) &&
       ((LDATA->flags & 0xe00) == 0)) {
      UpdateMask(c->obj.mask,&c->obj);
    }
    if (Level == 0x25) {
      HubSelect(c);
      HubLevelSelect(&c->obj,Hub);
      HubMoveVR();
    }
    else if (Level < 0x23) {
      CheckPlayerEvents(&c->obj);
      CheckFinish(&c->obj);
      CheckGates(&c->obj);
      CheckRings(&c->obj,&i_ring);
    }
    if (c->obj.dead == '\x01') {
      c->obj.scale = 1.0f - c->obj.die_time / c->obj.die_duration;
    }
    else {
      c->obj.scale = 1.0f;
    }
    c->obj.SCALE = c->obj.scale * CData[c->obj.character].scale;
        
    if (((VEHICLECONTROL != 1) && ( VEHICLECONTROL == 2))) {
         c->obj.RADIUS = CData[115].radius;
        
    }
    else if (VEHICLECONTROL == 1){
            if  (c->obj.vehicle != -1){
             c->obj.RADIUS = CData[c->obj.vehicle].radius;
            }
          else{
               c->obj.RADIUS = c->obj.radius ;
          }
    } else {
        c->obj.RADIUS = c->obj.radius ;
    }
        c->obj.RADIUS *= c->obj.SCALE;
    
  }
  if (FRAME == 0) {
    tbslotEnd(app_tbset,6);
  }
  return;
}

//NGC MATCH
void EvalModelAnim(struct CharacterModel *model,struct anim_s *anim,struct numtx_s *m,struct numtx_s *tmtx, float ***dwa, struct numtx_s *mLOCATOR) {
    short layertab [2] = {0, 1};
    short *layer;
    s32 nlayers;
    s32 i;

    layer = layertab;
    if (model->character == 0) {
        nlayers = 2;
    } else {
        nlayers = 1;
    }

    if (anim->blend != 0 &&
        (anim->blend_src_action <= 0x75U && model->fanmdata[anim->blend_src_action]) &&
            (anim->blend_dst_action <= 0x75U && model->fanmdata[anim->blend_dst_action])) {
        *dwa = NuHGobjEvalDwa(1, 0, model->fanmdata[anim->action],anim->anim_time);
    }
    else if (anim->blend == 0 && (anim->action <= 0x75U && model->fanmdata[anim->action])) {
        *dwa = NuHGobjEvalDwa(nlayers, layer, model->fanmdata[anim->action],anim->anim_time);
    } else {
        *dwa = NULL;
    }

    if (anim->blend != 0 &&
            (anim->blend_src_action <= 0x75U && model->anmdata[anim->blend_src_action]) &&
            (anim->blend_dst_action <= 0x75U && model->anmdata[anim->blend_dst_action])) {

        NuHGobjEvalAnimBlend(model->hobj, model->anmdata[anim->blend_src_action],
                anim->blend_src_time, model->anmdata[anim->blend_dst_action],
                anim->blend_dst_time, (float)anim->blend_frame / anim->blend_frames,
                0, NULL, tmtx);
        temp_action = anim->blend_dst_action; // needs to get merged
        temp_time = anim->blend_dst_time;

    } else if (anim->blend == 0 &&
            (anim->action <= 0x75U && model->anmdata[anim->action])) {

        NuHGobjEvalAnim(model->hobj, model->anmdata[anim->action], anim->anim_time,
                0, NULL, tmtx);
        temp_action = anim->action; // needs to get merged
        temp_time = anim->anim_time;

    } else {
        NuHGobjEval(model->hobj, 0, 0, tmtx);
        temp_action = -1;
    }

    if (mLOCATOR == NULL) return;
    for(i = 0; i < 0x10; i++) {
        if (model->pLOCATOR[i] != NULL) {
            NuHGobjPOIMtx(model->hobj,(u8)i,m,tmtx,&mLOCATOR[i]);
        }
    }
}


//NGC MATCH
void StoreLocatorMatrices(struct CharacterModel *model,struct numtx_s *mC,struct numtx_s *tmtx,struct numtx_s *mtx,struct nuvec_s *mom) {
  struct nuvec_s oldpos;
  s32 i;
  struct numtx_s m;

  if (mtx != NULL) {
    for(i = 0; i < 0x10; i++) {
      if (model->pLOCATOR[i] != NULL) {
        oldpos.x = mtx[i]._30;
        oldpos.y = mtx[i]._31;
        oldpos.z = mtx[i]._32;
        NuHGobjPOIMtx(model->hobj,i,mC,tmtx,&m);
          mtx[i] = m;
        if (mom != NULL) {
          mom[i].x = mtx[i]._30 - oldpos.x;
          mom[i].y = mtx[i]._31 - oldpos.y;
          mom[i].z = mtx[i]._32 - oldpos.z;
        }
      }
    }
  }
  return;
}

/*

//96% NGC
s32 DrawCharacterModel(struct CharacterModel* model,struct anim_s* anim,struct numtx_s* mC,struct numtx_s* mS,
    s32 render,struct numtx_s* mR,struct numtx_s* loc_mtx,struct nuvec_s* loc_mom,struct obj_s* obj) {
    short sVar1;
    float dVar2;
    float** dwa;
    s32 cVar3;
    s32 iVar4;
    s32 iVar5;
    s32 pafVar5;
    s32 Drawn = 0;
    struct NUJOINTANIM_s* pJ;
    struct CharacterModel* model2;
    s32 action;
    float time;
    struct NUJOINTANIM_s joint[4];
    short layertab[2] = { 0, 1 };
    short* local_58;
    s32 numjoints;

    if (jeep_draw != 0) {
        dVar2 = ((GameTimer.frame % 0x3c) * 0x10000) / 0x3c;
        for (iVar5 = 0; iVar5 < 4; iVar5++) {
            joint[iVar5].rx = dVar2 * 9.58738e-05f;
            joint[iVar5].ry = 0;
            joint[iVar5].rz = 0;
            joint[iVar5].tx = 0;
            joint[iVar5].ty = 0;
            joint[iVar5].tz = 0;
            joint[iVar5].sx = 1;
            joint[iVar5].sy = 1;
            joint[iVar5].sz = 1;
            joint[iVar5].joint_id = iVar5;
        }
    } else {
        joint->rx = (f32)((u16)-(player->obj).target_xrot) * 9.58738e-05f;
        joint->ry = (f32)((u16)-(player->obj).target_yrot) * 9.58738e-05f;
        joint->rz = 0.0f;
        joint->tx = 0.0f;
        joint->ty = 0.0f;
        joint->tz = 0.0f;
        joint->sx = 1.0f;
        joint->sy = 1.0f;
        joint->sz = 1.0f;
        joint->joint_id = (u8)jointnum;
        joint->flags = '\x01';
    }
    if (mC == NULL) {
        goto Exit;
    }
    iVar4 = 1;
    if (model->character == 0) {
        iVar4 = 2;
    }
    
    if (anim != NULL) {
        if ((anim->blend != 0)
            && (((0x75 >= (u16)anim->blend_src_action) && (model->fanmdata[anim->blend_src_action] != NULL))
                && ((0x75 >= (u16)anim->blend_dst_action) && (model->fanmdata[anim->blend_dst_action] != NULL))))
        {
            dwa = NuHGobjEvalDwaBlend(
                iVar4, (short*)&layertab[0], model->fanmdata[anim->blend_src_action], anim->blend_src_time,
                model->fanmdata[anim->blend_dst_action], anim->blend_dst_time,
                (float)anim->blend_frame / (float)anim->blend_frames
            );
        } else if (
            (anim->blend == 0) &&
            (((0x75 >= (u16)anim->action) && (model->fanmdata[anim->action] != NULL)))) {
            dwa = NuHGobjEvalDwa(1, NULL, model->fanmdata[anim->action], anim->anim_time);
        } else {
            dwa = NULL;
        }
    } else {
        dwa = NULL;
    }
    
    model2 = model;
    cVar3 = model->character;
    if (cVar3 == 0x54) {
        if ((LBIT & 0x0000000400000040)) {
            cVar3 = (s32)CRemap[115];
        } else {
            cVar3 = (s32)CRemap[0];
        }
    }
    else {
        if (cVar3 == 0x9f) {
            cVar3 = (s32)CRemap[8];
        }
        else {
            // TODO: Fix this
            goto after;
        }
    }
    if (cVar3 != -1) {
        model2 = &CModel[cVar3];
    }
    after:
    pJ = NULL;
    numjoints = 0;
    if ((((jeep_draw == 0) && (plr_render != 0))
         && ((player->target != '\0' && ((VEHICLECONTROL != 1 || ((player->obj).vehicle == -1))))))
        && ((sVar1 = model2->character, sVar1 == 0 || ((sVar1 == 0x54 || (sVar1 == 0x8c))))))
    {
        pJ = joint;
        numjoints = 1;
    }
    if (ChrisJointOveride != 0) {
        pJ = ChrisJointList;
        numjoints = ChrisNumJoints;
    }
    if (anim != NULL) {
        if (anim->blend != '\0') {
            if (((((u16)anim->blend_src_action < 0x76) && (model2->anmdata[anim->blend_src_action] != NULL))
                 && ((u16)anim->blend_dst_action < 0x76))
                && (model2->anmdata[anim->blend_dst_action] != NULL))
            {
                NuHGobjEvalAnimBlend(
                    model2->hobj, model2->anmdata[anim->blend_src_action], (f32)anim->blend_src_time,
                    model2->anmdata[anim->blend_dst_action], (f32)anim->blend_dst_time,
                    (f32)anim->blend_frame / (f32)anim->blend_frames, numjoints, pJ, tmtx);
                action = (s32)anim->blend_dst_action;
                time = anim->blend_dst_time;
            } else if (anim->blend != '\0'){
                 goto LAB_8001dcdc;      
            }

        }
        else if (((u16)anim->action < 0x76) && (model2->anmdata[anim->action] != NULL)) {
            NuHGobjEvalAnim(model2->hobj, model2->anmdata[anim->action], anim->anim_time, numjoints, pJ, tmtx);
            action = (s32)anim->action;
            time = anim->anim_time;
            goto LAB_8001dcf8;
        }
    } else {
        LAB_8001dcdc:
        NuHGobjEval(model2->hobj, numjoints, pJ, tmtx);
        action = -1;
    }
LAB_8001dcf8:
    if (glass_draw == 0) {
        StoreLocatorMatrices(model2, mC, tmtx, loc_mtx, loc_mom);
    }
    if (((action != -1) && (loc_mtx != NULL)) && ((Paused == 0 && (glass_draw == 0)))) {
        AddAnimDebris(model, loc_mtx, action, (float)time, obj);
    }
    if (render != 0) {
        if (((plr_render != 0) && (model->character == 0)) && ((player->obj).dead == '\x12')) {
            iVar4 = 1;
        }
        Drawn = NuHGobjRndrMtxDwa(model->hobj, mC, iVar4, (short*)&layertab, tmtx, dwa);
        if (((Drawn != 0) && (obj != NULL)) && (obj->character == 0xb1)) {
            DrawProbeFX(obj);
        }
        if (mR != NULL) {
            NuHGobjRndrMtxDwa(model->hobj, mR, iVar4, (short*)&layertab, tmtx, dwa);
        }
        if (mS != NULL) {
            NuMtlSetStencilRender(NUSTENCIL_REPLACE_NODRAW);
            NuHGobjRndrMtx(model->hobj, mS, iVar4, (short*)&layertab, tmtx);
            NuMtlSetStencilRender(NUSTENCIL_NOSTENCIL);
        }
    }
Exit:
    plr_render = 0;
    jeep_draw = 0;
    return Drawn;
}

//99% NGC //regswap
void UpdateAnimPacket(struct CharacterModel *mod,struct anim_s *anim,float dt,float xz_distance) {
    float t;
  
    if (mod == NULL) {
        return;
    }
    if (anim == NULL) {
        return;
    }
    
    if (anim->blend != 0) {
        anim->blend_frame++;
        if (anim->blend_frame == anim->blend_frames) {
            anim->blend = 0;
            anim->action = anim->blend_dst_action;
            anim->anim_time = anim->blend_dst_time;
        }
    }
    else if (anim->newaction != anim->oldaction) {
        if ((((anim->oldaction != -1) && (anim->newaction != -1)) && (mod->anmdata[anim->oldaction] != NULL)) && (((mod->anmdata[anim->newaction] != NULL &&
        (1 < mod->animlist[anim->oldaction]->blend_out_frames)) && (1 < mod->animlist[anim->newaction]->blend_in_frames)))) {
            anim->blend = 1;
            anim->blend_src_action = anim->oldaction;
            anim->blend_dst_action = anim->newaction;
            anim->blend_src_time = anim->anim_time;
            if ((((mod->animlist[anim->oldaction]->flags & 1) != 0) && ((mod->animlist[anim->newaction]->flags & 1) != 0))
            && ((mod->animlist[anim->oldaction]->speed == mod->animlist[anim->newaction]->speed && (mod->anmdata[anim->oldaction]->time == mod->anmdata[anim->newaction]->time)))) {
                anim->blend_dst_time = anim->anim_time;
            }
            else {
                anim->blend_dst_time = 1.0f;
            }
            
            if (((mod->character == 0) && (PLAYERCOUNT != 0)) && (player->used != 0)) {
                if (anim->blend_dst_action == 3) {
                    anim->blend_dst_time = ((float)(player->crouch_pos) * (mod->anmdata[2]->time - 1.0f)) / (float)(player->OnFootMoveInfo->CROUCHINGFRAMES);
                }
                else if (anim->blend_dst_action == 5) {
                    anim->blend_dst_time = ((float)(player->OnFootMoveInfo->CROUCHINGFRAMES - player->crouch_pos) * (mod->anmdata[4]->time - 1.0f)) / (float)player->OnFootMoveInfo->CROUCHINGFRAMES;
                }
            }
            anim->blend_frame = 0;
            anim->blend_frames = (u16)mod->animlist[anim->newaction]->blend_in_frames;
            if (mod->animlist[anim->oldaction]->blend_out_frames < anim->blend_frames) {
                anim->blend_frames = (u16)mod->animlist[anim->oldaction]->blend_out_frames;
            }
        } else {
            anim->action = anim->newaction;
            anim->anim_time = 1.0f;   
            anim->blend = 0;
        }
    } else {
        anim->action = anim->newaction;
        anim->blend = 0;
    }
    
    anim->flags = 0;
    if (anim->blend != 0) {
        if (mod->anmdata[anim->blend_src_action] == NULL) {
            return;
        }
        if (mod->anmdata[anim->blend_dst_action] == NULL) {
            return;
        }
        t = dt * mod->animlist[anim->blend_src_action]->speed;
        if ((mod->animlist[anim->blend_src_action]->flags & 0x10) != 0) {
            t *= xz_distance * 10.0f;
        }
        
        anim->blend_src_time += t;
        if (anim->blend_src_time > mod->anmdata[anim->blend_src_action]->time) {
            if ((mod->animlist[anim->blend_src_action]->flags & 1) != 0) {
                anim->blend_src_time -= (mod->anmdata[anim->blend_src_action]->time - 1.0f);
            }
            else {
                anim->blend_src_time = mod->anmdata[anim->blend_src_action]->time;
            }
        }
        t = dt * mod->animlist[anim->blend_dst_action]->speed;
        if ((mod->animlist[anim->blend_dst_action]->flags & 0x10) != 0) {
            t *= xz_distance * 10.0f;
        }
        
        anim->blend_dst_time += t;
        
        if (anim->blend_dst_time > mod->anmdata[anim->blend_dst_action]->time) {
            if ((mod->animlist[anim->blend_dst_action]->flags & 1) != 0) {
                anim->blend_dst_time -= (mod->anmdata[anim->blend_dst_action]->time - 1.0f);
                anim->flags = anim->flags | 2;
            } else {
                anim->blend_dst_time = mod->anmdata[anim->blend_dst_action]->time;
                anim->flags = anim->flags | 1;
            }
        }
    }
    else {
        if (mod->anmdata[anim->action] == NULL) {
            return;
        }
        t = dt * mod->animlist[anim->action]->speed;
        if ((mod->animlist[anim->action]->flags & 0x10) != 0) {
            t *= xz_distance * 10.0f;
        }
        
        anim->anim_time += t;
        if (anim->anim_time > mod->anmdata[anim->action]->time) {
            if ((mod->animlist[anim->action]->flags & 1) != 0) {
                anim->anim_time -= (mod->anmdata[anim->action]->time - 1.0f);
                anim->flags = 2;
            } else {
                anim->anim_time = mod->anmdata[anim->action]->time;
                anim->flags = 1;
            }
        }
        
    }
    return;
}

//89.18% NGC
void DrawCreatures(struct creature_s *c, s32 count, s32 render, s32 shadow) {
    struct nuvec_s s; // 0x10(r1)
    int i; // 0x13C(r1)
    int j; // r30
    int vflag; // r30
    int shflag; // r18
    int reflect; // r17
    int action; // r0
    int frames; // r0
    int old_frame; // r21
    int PLAYER; // 
    int VEHICLE; // r28
    float dx; // f0
    float dz; // f13
    float dist; // f31
    float r2; // f30
    float t; // f1
    float y; // f13
    unsigned short yrot; // r27
    unsigned short a; // 
    struct numtx_s mV; // 0x20(r1)
    struct numtx_s mC; // 0x60(r1)
    struct numtx_s mS; // 0xA0(r1)
    struct numtx_s mR; // 0xE0(r1)
    // Size: 0x7AC, DWARF: 0x14D9C0
    struct CharacterModel* model[2]; // 0x130(r1)
    struct numtx_s* m; // r3
    
  float fVar4;
  short sVar7;
  s32 bVar9;
  s32 bVar10;
  s32 bVar11;
  s32 iVar12;
  struct numtx_s *pnVar16;
  struct numtx_s *pnVar17;
  uint uVar18;
  uint uVar23;
  short sVar24;
  s32 iVar25;
  char bVar28;
  float dVar33;
  float dVar34;
  
    if ((DRAWCREATURESHADOWS == 0) || (Level == 0x1d)
        || (Level == 0x24)
        || ((Level == 0x1e) && (level_part_2 != 0))
        || (Level == 0x1a)
        || ((LDATA->flags & 0x1000) != 0)
        || (VEHICLECONTROL == 2)
        || ((VEHICLECONTROL == 1) && ((player->obj).vehicle == 0x20))) {
        shadow = 0;
    }
    
    if (
        ((LDATA->flags & 0x202) != 0)
        || (Level == 0x1c)
    ) {
        dVar33 = (s32)LDATA->farclip;
    } else {
        dVar33 = AIVISRANGE;
    }
    
    if ((LDATA->farclip) < dVar33) {
        dVar33 = LDATA->farclip;
    }
    
    dVar33 = (dVar33 * dVar33);
    
    for (iVar25 = 0; iVar25 < count; iVar25++, c++) {
        uVar23 = (c->obj).flags & 1;
        if (uVar23 == 0){
            // sVar24 = -1;
            goto here;
        }
        
        if (
            ((in_finish_range != 0x3C)
            && ((c->obj).finished == 0x00)
            && ((Level != 0x25) || (warp_level == -1)))
        )
        {
            if ((VEHICLECONTROL == 1) && ((c->obj).vehicle != -1)) {
                sVar24 = (c->obj).vehicle;
            } else {
                here:
                sVar24 = -1;
            }
            
            old_frame = (c->obj).draw_frame;
            (c->obj).draw_frame = 0;
            if ((c->used != 0x00) 
                && (c->on != 0x00) 
                && ((c->obj).model != NULL) 
                && ((c->obj).dead != 0x16)
                && ((c->obj).dead != 0x04)
                && ((c->obj).dead != 0x07)) {
                if ((Level != 0x17) 
                    || ((glass_phase == 0) && ((c->obj).character != 0x7F))
                    || ((glass_phase != 0) && ((c->obj).character == 0x7F))) {
                    
                    if ((((c->obj).invisible == 0) || ((c->obj).character == 0x77)) 
                        && (((c->obj).invincible == 0) || (1 < ((c->obj).invincible & 3)))
                        // && ((LDATA->flags & 0x200) == 0)
                        // && ((LDATA->flags & 0x200) != 0 || (dVar34 <= dVar33))
                    ) {
                        dVar34 = ((((pCam->pos).x - (c->obj).pos.x) * ((pCam->pos).x - (c->obj).pos.x)) + (((pCam->pos).z - (c->obj).pos.z) * ((pCam->pos).z - (c->obj).pos.z)));
                        if (((LDATA->flags & 0x200) != 0) || (dVar34 < dVar33)) {
                            bVar9 = 0;
                            bVar10 = 0;
                            bVar11 = 0;
                            if ((sVar24 == 0x63)
                                || (sVar24 == 0x36)
                                || (sVar24 == 0x81)
                                || (sVar24 == 0x53)
                                || (sVar24 == 0x8B)) {
                                if (render == 0) {
                                    // c->anim_processed = 1;
                                    continue;
                                }
                            
                                SetCreatureLights(c);
                                switch(sVar24) {
                                    case 0x81:
                                        ForceShader = 0x80;
                                    case 0x8B:
                                    case 0x36:
                                        DrawGlider(c);
                                        ForceShader = -1;
                                        (c->obj).draw_frame = old_frame + 1;
                                        mV = mTEMP;
                                        bVar9 = 1;
                                        break;
                                    case 0x53:
                                        ForceShader = 0x80;
                                        DrawAtlas(c);
                                        bVar9 = 1;
                                        ForceShader = -1;
                                        (c->obj).draw_frame = old_frame + 1;
                                        NuMtxSetRotationY(&mV, (uint)(c->obj).hdg);
                                        NuMtxTranslate(&mV, &(c->obj).pos);
                                        mV._31 = mV._31 + ATLASPLAYERLIFT;
                                        break;
                                    case 0x63:
                                        m = DrawPlayerJeep(c);
                                        (c->obj).draw_frame = old_frame + 1;
                                        if (m == NULL) {
                                            continue;
                                        } 
                                        mV = *m;
                                        bVar9 = 1;
                                        break;
                                }
                            }
                            LAB_8001e374:
                            vflag = uVar23 == 0;
                            if (uVar23 != 0) {
                                if ((VEHICLECONTROL == 2) && ((c->obj).dead == 0x00) && (c->spin != 0x00)
                                    && (c->spin_frame < (c->spin_frames - c->OnFootMoveInfo->SPINRESETFRAMES)) 
                                    && (bVar9 == 0)
                                )
                                {
                                    if (CRemap[116] != -1) {
                                        s.x = s.y = s.z = (c->obj).SCALE;
                                        yrot = (c->obj).hdg - 0x8000;
                                        NuMtxSetScale(&mC, &s);
                                        NuMtxRotateZ(&mC, c->spin_frame * 0x1555);
                                        NuMtxRotateX(&mC, (c->obj).xrot);
                                        NuMtxRotateY(&mC, yrot);
                                        NuMtxTranslate(&mC, &(c->obj).pos);
                                        iVar12 = (s32)CRemap[116];
                                        model[0] = CModel + iVar12;
                                        if (CModel[iVar12].anmdata[0x46] != NULL) {
                                            NuHGobjEvalAnim(CModel[iVar12].hobj, 
                                                CModel[iVar12].anmdata[0x46],
                                                (CModel[iVar12].anmdata[0x46]->time - 1.0f) * 
                                                ((float)c->spin_frame / (float)(c->OnFootMoveInfo->SPINFRAMES + c->OnFootMoveInfo->SUPERSPINFRAMES * 3))
                                                + 1.0f, 0,NULL,tmtx);
                                        }
                                        else {
                                            NuHGobjEval(CModel[iVar12].hobj, 0, NULL, tmtx);
                                        }
                                        
                                        if (glass_draw == 0) {
                                            StoreLocatorMatrices(model[0], &mC, tmtx, c->mtxLOCATOR, c->momLOCATOR);
                                        }
                                        
                                        if (render == 0) {
                                            continue;
                                            // goto LAB_8001f958;
                                        }
                                        SetCreatureLights(c);
                                        NuHGobjRndrMtx(model[0]->hobj, &mC, 1, NULL, tmtx);
                                        if (
                                            ((c->obj).reflect_y != 2000000.0f)
                                            && (glass_mix == 0.0f)
                                            && (glass_draw == 0)
                                        ) {
                                            mR = mC;
                                            mR._01 = -mR._01;
                                            mR._11 = -mR._11;
                                            mR._21 = -mR._21;
                                            //do {} while (0);
                                            mR._31 -= (mR._31 - (c->obj).reflect_y);
                                            NuHGobjRndrMtx(model[0]->hobj, &mR, 1, NULL, tmtx);
                                        }
                                        if (
                                            (shadow != 0)
                                            && ((c->obj).shadow != 2000000.0f)
                                            && (dVar34 < dVar33)
                                            && ((TerSurface[(c->obj).surface_type].flags & 1) == 0)
                                            && (SKELETALCRASH == 0)
                                            && (c->freeze == 0)
                                            && (glass_draw == 0)
                                        )
                                        {
                                            ScaleFlatShadow(&s, (c->obj).pos.y, (c->obj).shadow, (c->obj).SCALE);
                                            NuMtxSetScale(&mS, &s);
                                            NuMtxRotateY(&mS, uVar23);
                                            NuMtxRotateZ(&mS, (uint)(c->obj).surface_zrot);
                                            NuMtxRotateX(&mS, (uint)(c->obj).surface_xrot);
                                            mS._30 = (c->obj).pos.x;
                                            mS._31 = (c->obj).shadow + 0.025f;
                                            mS._32 = (c->obj).pos.z;
                                        }
                                    }
                                    LAB_8001ed04:
                                }
                                else if (
                                    (uVar23 != 0)  && ((c->obj).character == 0) && ((c->obj).dead == 0)
                                    && (c->spin != 0)
                                    && (c->spin_frame < (c->spin_frames - c->OnFootMoveInfo->SPINRESETFRAMES))
                                    && (bVar9 == 0) && (sVar24 == -1) && (c->freeze == 0)
                                ) {
                                    if (render != 0) {
                                        SetCreatureLights(c);
                                    }
                                    
                                    if (CRemap[8] != -1) {
                                        yrot = -(c->spin_frame * 0x1555);
                                        s.x = s.y = s.z = (c->obj).SCALE;
                                        
                                        NuMtxSetScale(&mC, &s);
                                        NuMtxRotateY(&mC, yrot);
                                        NuMtxRotateZ(&mC, (c->obj).zrot);
                                        NuMtxRotateX(&mC, (c->obj).xrot);
                                        NuMtxTranslate(&mC, &(c->obj).pos);
                                        
                                        if ((SKELETALCRASH != 0) && (CRemap[159] != -1)) {
                                            model[0] = CModel + CRemap[159];
                                        } else {
                                            model[0] = CModel + CRemap[8];
                                        }
                                        
                                        if ((c->obj).dangle != 0) {
                                            uVar18 = 0x47;
                                        } else {
                                            uVar18 = 0x46;
                                        }
                                        
                                        if ((uVar18 < 0x76) && ((model[0]->anmdata[uVar18]) != NULL)) {
                                            NuHGobjEvalAnim(
                                                model[0]->hobj,
                                                model[0]->anmdata[uVar18],
                                                (float)(model[0]->anmdata[uVar18]->time - 1.0f) * ((float)c->spin_frame / (float)(c->OnFootMoveInfo->SPINFRAMES + c->OnFootMoveInfo->SUPERSPINFRAMES * 3)) + 1.0f, 0,
                                                NULL,tmtx);
                                        }
                                        else {
                                            NuHGobjEval(model[0]->hobj, 0, NULL, tmtx);
                                        }
                                        
                                        if (glass_draw == 0) {
                                            StoreLocatorMatrices(model[0], &mC, tmtx, c->mtxLOCATOR, c->momLOCATOR);
                                        }
                                        
                                        if (render == 0) {
                                            continue;
                                        }
                                        NuHGobjRndrMtx(model[0]->hobj, &mC, 1, NULL, tmtx);
                                        if (
                                            ((c->obj).reflect_y != 2000000.0f)
                                            && (glass_mix == 0.0f)
                                            && (glass_draw == 0)
                                        ) {
                                            mR = mC;
                                            mR._01 = -mR._01;
                                            mR._11 = -mR._11;
                                            mR._21 = -mR._21;
                                            if (Level != 0x25) {
                                                fVar4 = (c->obj).reflect_y;
                                            } else {
                                                fVar4 = HUBREFLECTY;
                                            }
                                            mR._31 = fVar4 - (mR._31 - fVar4);
                                            NuHGobjRndrMtx(model[0]->hobj, &mR, 1, NULL, tmtx);
                                        }
                                        if ((shadow != 0) 
                                            && ((c->obj).shadow != 2000000.0f) && (dVar34 < dVar33)
                                            && ((TerSurface[(c->obj).surface_type].flags & 1) == 0) 
                                            && (SKELETALCRASH == 0)  && (c->freeze == 0) && (glass_draw == 0)) {
                                            ScaleFlatShadow(&s, (c->obj).pos.y, (c->obj).shadow, (c->obj).SCALE);
                                            NuMtxSetScale(&mS, &s);
                                            NuMtxRotateY(&mS, yrot);
                                            NuMtxRotateZ(&mS, (c->obj).surface_zrot);
                                            NuMtxRotateX(&mS, (c->obj).surface_xrot);
                                            mS._30 = (c->obj).pos.x;
                                            mS._31 = (c->obj).shadow + 0.025f;
                                            mS._32 = (c->obj).pos.z;
                                            NuMtlSetStencilRender(NUSTENCIL_REPLACE_NODRAW);
                                            NuHGobjRndrMtx(model[0]->hobj, &mS, 1, NULL, tmtx);
                                            NuMtlSetStencilRender(NUSTENCIL_NOSTENCIL);
                                        }
                                    }
                                    if (render == 0) {
                                        continue;
                                    }
                                    if (
                                        ((c->obj).dangle != 0) 
                                        && (CRemap[9] != -1)
                                    ) {
                                        s.x = s.y = s.z = (c->obj).SCALE;
                                        yrot = (c->obj).hdg - 0x8000;
                                        NuMtxSetScale(&mC, &s);
                                        NuMtxRotateY(&mC, yrot);
                                        NuMtxRotateZ(&mC, (c->obj).zrot);
                                        NuMtxRotateX(&mC, (c->obj).xrot);
                                        NuMtxTranslate(&mC, &(c->obj).pos);
                                        model[1] = CModel + CRemap[9];
                                        NuHGobjRndr(CModel[CRemap[9]].hobj, &mC, 1, NULL);
                                        
                                        if (
                                            ((c->obj).reflect_y != 2000000.0f)
                                            && (glass_mix == 0.0f)
                                            && (glass_draw == 0)
                                        ) {
                                            mR = mC;
                                            mR._01 = -mR._01;
                                            mR._11 = -mR._11;
                                            mR._21 = -mR._21;
                                            fVar4 = (c->obj).reflect_y;
                                            mR._31 = fVar4 - (mR._31 - fVar4);
                                            NuHGobjRndr(model[1]->hobj, &mR, 1, NULL);
                                        }
                                    }
                                    continue;
                                }
                            }
                            
                            LAB_8001ecd4:
                            if (((c->obj).character == 0x77U) && ((c->obj).invisible != 0)) {
                                if (render != 0) {
                                    Draw3DCrateCount(&(c->obj).pos, (c->obj).hdg);
                                    (c->obj).draw_frame = old_frame + 1;
                                }
                                continue;
                            } 
                            
                            if (
                                (render == 0) 
                                || ((c->obj).character == 0x75)
                                || ((c->obj).character == 0x77)
                                || ((c->obj).character == 0x78)
                                || ((c->obj).character == 0x79)
                                || ((c->obj).character == 0x7A)
                                || ((c->obj).character == 0x7B)
                                || ((c->obj).character == 0x7C)
                            )
                            {
                                yrot = (c->obj).hdg - 0x8000;
                                if (uVar23 != 0) {
                                    if (((c->obj).dead == 0x03) || ((c->obj).dead == 0x08)) {
                                        yrot -= 0x8000;
                                    }
                                    else if (c->freeze != 0x00) {
                                        yrot = GameCam->hdg_to_player;
                                    }
                                    else {
                                        if ((c->spin != 0x00)
                                            && (c->spin_frame < (c->spin_frames - c->OnFootMoveInfo->SPINRESETFRAMES))
                                            && (sVar24 == 0x3b)
                                            && ((c->obj).anim.newaction == 0x69)
                                            && (((c->obj).model)->anmdata[0x69] != NULL)) {
                                            yrot -= (c->spin_frame << 0x10) / GyroMoveInfo.SPINFRAMES;
                                        }
                                    }
                                }
                                // bVar28 = !bVar9 << 1;
                                if (bVar9 != 0) {
                                    mC = mV;
                                    c->m = mC;
                                }
                                else {
                                    if ((uVar23 == 0)
                                        && (((c->obj).vehicle == 0xa1)
                                            || (VEHICLECONTROL == 2)
                                            || (sVar24 == 0x20)
                                            || (sVar24 == 0x89) 
                                            || (sVar24 == 0xa1))) {
                                        s.x = s.y = s.z = (c->obj).SCALE;
                                        NuMtxSetScale(&mC, &s);
                                        NuMtxRotateZ(&mC, (c->obj).zrot);
                                        
                                        if (sVar24 == 0x20) {
                                            uVar18 = RotDiff(0, (c->obj).xrot) / 4;
                                        }
                                        else {
                                            uVar18 = (c->obj).xrot;
                                        }
                                        
                                        NuMtxRotateX(&mC, uVar18);
                                        NuMtxRotateY(&mC, yrot);
                                        NuMtxTranslate(&mC, &(c->obj).pos);
                                        c->m = mC;
                                    }
                                    else {
                                        if (
                                            (Level == 0x17)
                                            && ((c->obj).character == 0x7f)
                                        ) {
                                            a = ((GameTimer.frame % 0x24) * 0x10000) / 0x24;
                                            
                                            s.x = (NuTrigTable[a] * 0.05f + 1.0f);
                                            s.y = (NuTrigTable[a + 0x4000] * 0.05f + 1.0f);
                                            s.z = (NuTrigTable[a] * 0.05f + 1.0f);
                                            
                                            s.x *= (c->obj).SCALE;
                                            if (s.x < 0.0f) {
                                                s.x = 0.0f;
                                            }
                                            
                                            s.y *= (c->obj).SCALE;
                                            if (s.y < 0.0f) {
                                                s.y = 0.0f;
                                            }
                                            
                                            s.z *= (c->obj).SCALE;
                                            if (s.z < 0.0f) {
                                                s.z = 0.0f;
                                            }
                                        }
                                        else {
                                            s.x = s.y = s.z = (c->obj).SCALE;
                                        }
                                        
                                        if (((c->obj).flags & 0x10000) != 0) {
                                            s.y = -s.y;
                                        }
                                        
                                        NuMtxSetScale(&mC, &s);
                                        NuMtxRotateY(&mC, yrot);
                                        NuMtxRotateZ(&mC, (c->obj).zrot);
                                        NuMtxRotateX(&mC, (c->obj).xrot);
                                        NuMtxTranslate(&mC, &(c->obj).pos);
                                        c->m = mC;
                                    }
                                    
                                }
                                
                                if (render != 0) {
                                    if (
                                        ((c->obj).reflect_y != 2000000.0f)
                                        && ((c != player) || ((c->obj).dead != 0x02))
                                        && (glass_draw == 0)
                                    ) {
                                        mR = mC;
                                        mR._01 = -mR._01;
                                        mR._11 = -mR._11;
                                        mR._21 = -mR._21;
                                        
                                        if (Level != 0x25) {
                                            fVar4 = (c->obj).reflect_y;
                                        } else {
                                            fVar4 = HUBREFLECTY;
                                        }
                                        
                                        bVar11 = 1;
                                        mR._31 = fVar4 - (mR._31 - fVar4);
                                    }
                                    
                                    if ((render != 0)
                                        && (shadow != 0)
                                        && ((c->obj).shadow != 2000000.0f)
                                        && (dVar34 < dVar33)
                                        && ((TerSurface[(c->obj).surface_type].flags & 1) == 0)
                                        && (c->freeze == 0x00)
                                        && (glass_draw == 0)
                                        && (((c->obj).flags & 0x4000) == 0)
                                        && (!bVar28)
                                        && ((c->obj).dead != 0x08)
                                        && ((uVar23 == 0) || (SKELETALCRASH == 0))
                                        && (sVar24 != 0xA1)
                                        && (sVar24 != 0x89)
                                        && (sVar24 != 99)
                                        && ((uVar23 == 0) || ((c->obj).vehicle != 0xa1))) {
                                        ScaleFlatShadow(&s, (c->obj).pos.y, (c->obj).shadow, (c->obj).SCALE);
                                        NuMtxSetScale(&mS, &s);
                                        NuMtxRotateY(&mS, yrot);
                                        NuMtxRotateZ(&mS, (c->obj).surface_zrot);
                                        NuMtxRotateX(&mS, (c->obj).surface_xrot);
                                        
                                        mS._30 = (c->obj).pos.x;
                                        if ((c->obj).dead == 1) {
                                            mS._31 = (c->obj).pos.y + ((c->obj).shadow - (c->obj).oldpos.y);
                                        }
                                        mS._31 = (c->obj).shadow + 0.025f;
                                        mS._32 = (c->obj).pos.z;
                                        
                                        if (sVar24 == 0x99) {
                                            if (Level == 3) {
                                                mS._31 = mS._31 + 0.05f;
                                            } else {
                                                mS._31 = mS._31 + 0.025f;
                                            }
                                        }
                                        
                                        bVar10 = 1;
                                        temp_surface_xrot = (c->obj).surface_xrot;
                                        temp_surface_yrot = (c->obj).hdg;
                                        temp_surface_zrot = (c->obj).surface_zrot;
                                    }
                                }
                                
                                model[0] = (c->obj).model;
                                if (((c->obj).dead != 0x00)&& ((c->obj).die_model[0] != -1)
                                    && ((c->obj).die_model[0] != CRemap[(c->obj).character])){
                                    model[0] = CModel + (c->obj).die_model[0];
                                } else if (!(uVar23 == 0) && (VEHICLECONTROL == 2)) {
                                    model[0] = CModel + CRemap[115];
                                }
                                
                                if (
                                    !(uVar23 == 0)
                                    && ((model[0]->character == 0)|| (model[0]->character == 0x73))
                                    && (CRemap[84] != -1) && ((SKELETALCRASH != 0) || (
                                            ((c->obj).dead == 0x11) && (GameTimer.frame % 0xc < 6)))){
                                    model[0] = CModel + CRemap[84];
                                }
                                
                                model[1] = NULL;
                                
                                if (((c->obj).dead != 0x00) 
                                    && ((c->obj).die_model[1] != -1)
                                    && ((c->obj).die_model[1] != CRemap[(c->obj).character])
                                    && ((c->obj).die_model[1] == (c->obj).die_model[0])){
                                //     iVar12 = (c->obj).die_model[1];
                                }
                                else {
                                    iVar12 = -1;
                                    if ((c->obj).character == 0x11) {
                                        iVar12 = 0x12;
                                    }
                                    else if (((c->obj).character == 0x24) && ((c->obj).anim.newaction == 0)) {
                                        iVar12 = 0x87;
                                    }
                                    else if ((c->obj).character == 0x6d) {
                                        iVar12 = 0x3d;
                                    }
                                    else if (((c->obj).dead == 0x00) 
                                        && ((c->obj).character == 0) 
                                        && (
                                            ((c->obj).anim.newaction == 0x25)
                                            || ((c->obj).anim.newaction == 0x26)
                                        )) {
                                        iVar12 = 0x45;
                                    }
                                    else if ((c->freeze != 0x00) 
                                        && ((c->obj).dead == 0x00)
                                    )
                                    {
                                        iVar12 = 0x4f;
                                    }
                                    else if ((uVar23 != 0)
                                        && ((c->obj).dead == 0x00)
                                        && (c->target != 0x00)
                                        && ((c->obj).character == 0)
                                        && (sVar24 == -1)
                                    ) {
                                        iVar12 = 0x8c;
                                    }
                                    else if ((sVar24 != -1) && (vtog_time == vtog_duration)) {
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
                                        else if (sVar24 == 0xA1) {
                                            iVar12 = 0xA1;
                                        }
                                        else if (sVar24 == 0x99) {
                                            iVar12 = 0x99;
                                        }
                                    } 
                                    else {
                                        if ((uVar23 == 0) && ((c->obj).vehicle == 0xA1)) {
                                            iVar12 = 0xA1;
                                        }
                                    }
                                    
                                    if ((iVar12 != -1) && (CRemap[iVar12] != -1)){
                                        iVar12 = CRemap[iVar12];
                                    }
                                
                                }
                                model[1] = CModel + iVar12;
                                
                                if (render != 0) {
                                    SetCreatureLights(c);
                                }
                        
                                //dVar34 = 1.2987013f;
                                for(j = 0; j < 2; j++, c++) {
                                    struct numtx_s* mom_loc = c->momLOCATOR;
                                    struct numtx_s* mtx_loc = c->mtxLOCATOR;
                                    if (model[j] == 0) {
                                        continue;
                                    }
                                    
                                    if (((c->obj).anim.blend == 0x00) || (
                                            (model[j]->character != 0x45)
                                            && (model[j]->character != 0x8c)
                                            && (model[j]->character != 0xa0)
                                            && (model[j]->character != 0x6b))
                                        || ((model[j]->anmdata[(c->obj).anim.blend_src_action] != 0) &&
                                    (model[j]->anmdata[(c->obj).anim.blend_dst_action] != 0))) {
                                        // bVar28 = bVar11 << 1;
                                        if ((Level == 0x1c) && (j == 1) && (model[0]->character == 0x7f)) {
                                            s.z = 1.2987013f;
                                            s.y = 1.2987013f;
                                            s.x = 1.2987013f;
                                            NuMtxPreScale(&mC, &s);
                                            if (bVar10) {
                                                NuMtxPreScale(&mS, &s);
                                            }
                                            if (bVar11) {
                                                NuMtxPreScale(&mR, &s);
                                            }
                                        }
                                        if (uVar23 != 0) {
                                            plr_render = 1;
                                        }
                                        if ((render != 0) && (model[j]->character == 0xAF || model[j]->character == 0xB0))
                                        {
                                            SetLevelLights();
                                        }
                                        
                                        if (model[j]->character == 0x99) {
                                            jeep_draw = 1;
                                        }
                                        
                                        if ((bVar10)
                                            && (uVar23 == 0) 
                                            || (j == 0) 
                                            && (model[1] != NULL)
                                            && (model[1]->character != 0x44) 
                                            && (model[1]->character != 0xb2) 
                                            && (model[1]->character != 0x99)    
                                            && (model[1]->character != 99)
                                        )
                                        {
                                            pnVar16 = &mS;
                                        }
                                        else {
                                            pnVar16 = NULL;
                                        }
                                        
                                        if ((bVar28 >> 1 & 1)) {
                                            pnVar17 = &mR;
                                        }
                                        else {
                                            pnVar17 = NULL;
                                        }
                                        
                                        DrawCharacterModel(model[j],&(c->obj).anim,&mC,pnVar16,render,
                                            pnVar17,mtx_loc,mom_loc,&c->obj);
                                    }
                                }
                                if ((render != 0) && ((c->obj).character == 0x76) && (
                                        ((LDATA->flags & 0x200) != 0)|| (Level == 0x1d))
                                    && (ObjTab[66].obj.special != NULL)) {
                                    NuRndrGScnObj((ObjTab[66].obj.scene)->gobjs[(ObjTab[66].obj.special)->instance->objid], &mC);
                                }
                            }
                            (c->obj).draw_frame = old_frame + 1;
                        }
                    }
                }
            }
        }
        LAB_8001f958:
        c->anim_processed = '\x01';
    }
    glass_phase = 0;
    glass_draw = 0;
    if (render != 0) {
        SetLevelLights();
    }
    return;
}

//29% NGC
void MovePlayer(struct creature_s *plr,struct nupad_s *pad) {
  struct MoveInfo *moveinfo;
  u16 gotlist[9];
  u16 sVartmp;
  s32 bVar1;
  float fVar2;
  short sVar3;
  s32 uVar4;
  float fVar6;
  char cVar8;
  u8 uVar9;
  u16 uVar12;
  s32 Awrd;
  u16 uVar13;
  s32 iVar11;
  struct CharacterModel *mod;
  char cVar14;
  float fVar15;
  struct nuvec_s *pos;
  short sVar16;
  s32 uVar17;
  //s32 *piVar18;
  s32 unaff_r19;
  short sVar19;
  s32 iVar20;
  s32 bVar21;
  char bVar22;
  //char bVar23;
  float dVar24;
  float dVar25;
  float dVar26;
  float dVar27;
  float dVar28;
  float fVar29;
  s32 local_108;
  s32 local_104;
  s32 local_100;
  s32 local_fc;
  short local_f8;
  //s32 iStack_f6;
  struct nuvec_s local_f0;
  struct nuvec_s local_e0;
  struct nuvec_s local_d0;
  struct nuvec_s local_c0;
  struct nuvec_s local_b0;
  struct nuvec_s local_a0;
  //double local_68;
  
  /*local_108 = 0x80007; //??
  local_104 = 0x100020;
  local_100 = 0x400080;
  local_fc = 0x1000200;
  local_f8 = 0x400;
///
  local_108[0] = 8;
  local_108[1] = 7;
  local_108[2] = 0x10;
  local_108[3] = 0x20;
  local_108[4] = 0x40;
  local_108[5] = 0x80;
  local_108[6] = 0x100;
  local_108[7] = 0x200;
  local_108[8] = 0x400;
  */
  memcpy(gotlist, lbl_801083ec, sizeof(u16) * 9);
  //piVar18 = &lbl_801083ec;
  if (pad == NULL) {
    if (Demo == 0) {
      return;
    }
    pad = &demopad;
  }
  if (pad != NULL) {
    UpdateRumble(&plr->rumble);
  }
  if (pad != NULL) {
    if ((Game.vibration != 0) && (Demo == 0)) {
      //uVar17 = (u32)(plr->rumble).frame;
      if ((plr->rumble).frame == 0) {
        uVar17 = 0;
      }
      else {
        uVar17 = ((plr->rumble).power * (plr->rumble).frame) / (u32)(plr->rumble).frames;
      }
        if (plr->rumble.buzz != 0) {
          NuPs2PadSetMotors(pad,uVar17,uVar17);
        } else {
            NuPs2PadSetMotors(pad,0,uVar17);
        }
    }
    else {
      NuPs2PadSetMotors(pad,0,0);
    }
  }
  if ((((Level == 0x16) && (boss_dead == 1)) && ((plr->obj).pos.z < 49.0f)) &&
     (fVar6 = (plr->obj).oldpos.z, 49.0f <= fVar6)) {
    (plr->obj).pos.x = (plr->obj).oldpos.x;
    (plr->obj).pos.z = fVar6;
    (plr->obj).mom.z = 0.05;
  }
  Awrd = VEHICLECONTROL;
  if ((plr->obj).vehicle != -1) {
    if ((((LBIT & 0x11 | LBIT & 0x5252801) == 0) && (Level != 3)) &&
              ((Level != 0x1d && (Level != 0x1c)))) {
        if (((LBIT & 4 | LBIT & 0x40) != 0) && (VEHICLECONTROL == 0)) {
          VEHICLECONTROL = 1;
        }
      }
      else {
        VEHICLECONTROL = 0;
      }
  }
  else {
    VEHICLECONTROL = 0;
  }
  if ((VEHICLECONTROL == 1) && (vtog_time == vtog_duration)) {
    (plr->obj).vehicle_frame = (plr->obj).vehicle_frame + 1;
  }
  else {
    (plr->obj).vehicle_frame = 0;
    plr_vehicle_time = 0.0f;
    plr_vehicle_speedmul = 1.0f;
  }
  ToggleVehicle(plr);
  sVar19 = -1;
  if ((VEHICLECONTROL == 1) && (sVar16 = (plr->obj).vehicle, sVar16 != -1)) {
    sVar19 = sVar16;
  }
  //bVar23 = (sVar19 == 0x6b) << 1;
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
    plr_vehicle_time = 0.0f;
    if (sVar19 == 0x89) {
      VEHICLETIME = 60.0f;
    }
    else if (sVar19 == 99) {
      ResetJeep(plr);
    }
    ResetTubs();
  }
  uVar9 = (plr->obj).invincible;
  if (uVar9 != 0) {
    (plr->obj).invincible = uVar9 + 0xff;
  }
  if (plr->freeze != 0) {
    plr->freeze = plr->freeze + 0xff;
  }
  pos = &(plr->obj).pos;
  //fVar15 = (plr->obj).pos.y;
  //fVar6 = (plr->obj).pos.z;
  (plr->obj).oldpos = (plr->obj).pos;
  (plr->obj).old_ground = (plr->obj).ground;
  cVar14 = (plr->obj).dangle;
  GetTopBot(plr);
  OldTopBot(&plr->obj);
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
        NewRumble(&plr->rumble,0xbf);
        NewBuzz(&plr->rumble,0xc);
      }
      uVar12 = vtog_angle;
      if (vtog_blend != 0) {
        dVar27 = (vtog_time / vtog_duration);
        (plr->obj).pos.x = (vtog_newpos.x - vtog_oldpos.x) * dVar27 + vtog_oldpos.x;
        (plr->obj).pos.y = (vtog_newpos.y - vtog_oldpos.y) * dVar27 + vtog_oldpos.y;
        (plr->obj).pos.z = (vtog_newpos.z - vtog_oldpos.z) * dVar27 + vtog_oldpos.z;
        //fVar15 = v000.z;
        //fVar6 = v000.y;
        (plr->obj).mom = v000;
        (plr->obj).thdg = vtog_angle;
        //uVar12 = SeekRot((plr->obj).hdg,uVar12,3);
        (plr->obj).hdg = SeekRot((plr->obj).hdg,uVar12,3);
        fVar6 = NewShadowMaskPlat(pos,0.0f,-1);
        (plr->obj).shadow = fVar6;
        if (fVar6 != 2000000.0f) {
          GetSurfaceInfo(&plr->obj,1,fVar6);
        }
        if ((LBIT & 4 | LBIT & 0x40) == 0) {
          (plr->obj).pos.y = (plr->obj).pos.y +
               NuTrigTable[(s32)(dVar27 * 32768.0f) & 0xffff] +
               NuTrigTable[(s32)(dVar27 * 32768.0f) & 0xffff];
        }
        ObjectRotation(&plr->obj,2,1);
        if ((Level != 0x16) || (vtog_time != vtog_duration)) goto LAB_8001cbd4;
      }
    }
    if (Level == 0x25) {
      if (tumble_time < tumble_duration) {
        tumble_time = tumble_time + 0.01666667;
        if ((tumble_action == 0x56) && (new_lev_flags != 0)) {
          fVar6 = (plr->obj).anim.anim_time;
          if ((fVar6 - ((plr->obj).model)->animlist[0x56]->speed * 0.5f < tumble_item_starttime + 1.0f
              ) && (tumble_item_starttime + 1.0f <= fVar6)) {
            Awrd = 0;
            do {
              uVar12 = new_lev_flags & gotlist[Awrd];
              if ((uVar12 != 0) && ((temp_lev_flags & uVar12) == 0)) {
                temp_lev_flags = uVar12 | temp_lev_flags;
                Awrd = AddAward(Hub,last_level,uVar12);
                if (Awrd == 0) {
                  new_lev_flags = uVar12 ^ (uVar12 | new_lev_flags);
                  Game.level[last_level].flags = uVar12 | Game.level[last_level].flags;
                }
                //piVar18 = &iStack_f6;
                Awrd = 9;
              }
              Awrd = Awrd + 1;
              //piVar18 = (s32 *)((s32)piVar18 + 2);
            } while (Awrd < 9);
          }
        }
        if (tumble_duration <= tumble_time) {
          if ((new_lev_flags | temp_lev_flags) == temp_lev_flags) {
            tumble_time = tumble_duration;
            plr->jump = 1;
            if (tumble_action == 0x56) {
              plr->jump_type = 0;
              Awrd = qrand();
              if (Awrd < 0) {
                Awrd = Awrd + 0x7fff;
              }
              plr->jump_subtype = (char)(Awrd >> 0xf);
            }
            else {
              plr->jump_type = 1;
            }
            if (plr->jump_type == 0) {
              sVar16 = moveinfo->STARJUMPFRAMES;
            }
            else {
              sVar16 = moveinfo->JUMPFRAMES0;
            }
            plr->jump_frames = sVar16;
            (plr->obj).anim.anim_time = 1.0f;
            (plr->obj).ground = 0;
            plr->jump_hack = 1;
            plr->jump_frame = 0;
            plr->somersault = 0;
            plr->land = 0;
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
      if ((tumble_duration < tumble_time) && (last_hub != -1)) {
        if ((1) && (warp_level != -1)) goto LAB_8001cbd4;
        goto LAB_80019f04;
      }
      if (tumble_time >= tumble_moveduration) {
        fVar6 = tumble_time / tumble_moveduration;
        (plr->obj).pos.x = (tumble_newpos.x - tumble_oldpos.x) * fVar6 + tumble_oldpos.x;
        (plr->obj).pos.y = (tumble_newpos.y - tumble_oldpos.y) * fVar6 + tumble_oldpos.y;
        (plr->obj).pos.z = (tumble_newpos.z - tumble_oldpos.z) * fVar6 + tumble_oldpos.z;
        fVar6 = NewShadowMaskPlat(pos,0.0f,-1);
        (plr->obj).shadow = fVar6;
        if (fVar6 != 2000000.0f) {
          GetSurfaceInfo(&plr->obj,1,fVar6);
        }
      }
      else {
        (plr->obj).pos.x = tumble_newpos.x;
        (plr->obj).pos.y = tumble_newpos.y;
        (plr->obj).pos.z = tumble_newpos.z;
      }
      uVar12 = tumble_hdg;
      bVar1 = sVar19 == 0x20;
      (plr->obj).ground = 3;
      (plr->obj).thdg = uVar12;
      (plr->obj).hdg = uVar12;
      (plr->obj).old_ground = 3;
LAB_8001c028:
      fVar6 = NewShadowMaskPlatRot(pos,0.0f,-1);
      if (fVar6 == 2000000.0f) {
        cVar8 = (plr->obj).transporting;
        plr_terrain_ok = (s32)cVar8;
        if (plr_terrain_ok != 0) goto LAB_8001c0a8;
        if (((NOTERRAINSTOP == 0) || (Level == 8)) || (Level == 6)) goto LAB_8001c0b0;
        //fVar6 = (plr->obj).oldpos.z;
        (plr->obj).pos.x = (plr->obj).oldpos.x;
        (plr->obj).pos.z = (plr->obj).oldpos.z;
        (plr->obj).mom.z = 0.0f;
        (plr->obj).dangle = cVar8;
        (plr->obj).mom.x = 0.0f;
      }
      else {
LAB_8001c0a8:
        plr_terrain_ok = 1;
LAB_8001c0b0:
        unaff_r19 = (s32)(plr->obj).layer_type;
        if (((plr->obj).got_shadow == 0) ||
           ((((plr->obj).transporting == 0 && (fVar6 != 2000000.0f)) &&
            ((plr->obj).shadow < fVar6)))) {
          (plr->obj).got_shadow = 0;
          (plr->obj).shadow = fVar6;
          GetSurfaceInfo(&plr->obj,2,fVar6);
        }
        else {
          GetSurfaceInfo(&plr->obj,0,fVar6);
        }
      }
      (plr->obj).wade = 0;
      if ((plr->obj).layer_type == -1) {
        (plr->obj).submerged = 0;
      }
      else {
        if ((plr->obj).layer_shadow <= (plr->obj).top * (plr->obj).SCALE + (plr->obj).pos.y) {
          (plr->obj).submerged = 0;
        }
        else {
          bVar22 = (plr->obj).submerged;
          if (bVar22 < 0x3c) {
            (plr->obj).submerged = bVar22 + 1;
          }
          else if (((Level != 0x25) && (!bVar1)) && (VEHICLECONTROL != 2)) {
            KillPlayer(&plr->obj,10);
          }
        }
        if (((plr->obj).bot + (plr->obj).top) * (plr->obj).SCALE * 0.5f + (plr->obj).pos.y <
            (plr->obj).layer_shadow) {
          (plr->obj).wade = 1;
        }
        cVar8 = (plr->obj).layer_type;
        Awrd = 1;
        local_e0.x = (plr->obj).pos.x;
        iVar20 = -1;
        local_e0.y = (plr->obj).layer_shadow;
        local_e0.z = (plr->obj).pos.z;
//SWITCH??
        if (cVar8 == 2) {
LAB_8001c324:
          iVar20 = 1;
        }
        else if (cVar8 < 3) {
          if (cVar8 == 1) {
            uVar17 = qrand();
            //dVar26 = 4503601774854144.0f;
            //local_68 = CONCAT44(0x43300000,uVar17 ^ 0x80000000);
            dVar27 = 1.525878928987368e-06;
            local_a0.x = (float)uVar17 * 1.525879e-06 + local_e0.x;
            uVar17 = qrand();
            //local_68 = CONCAT44(0x43300000,uVar17 ^ 0x80000000);
            local_a0.y = (float)((float)uVar17 * dVar27 + local_e0.y);
            uVar17 = qrand();
            //local_68 = CONCAT44(0x43300000,uVar17 ^ 0x80000000);
            (plr->obj).ddr = '@';
            (plr->obj).ddg = 'x';
            local_a0.z = ((float)uVar17 * dVar27 + local_e0.z);
            (plr->obj).ddb = -0x80;
            (plr->obj).ddwater = 'x';
            if ((plr->obj).idle_gametime == 0.0f) {
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
          if (cVar8 == 3) goto LAB_8001c324;
          if ((cVar8 == 4) && ((plr->obj).idle_gametime == 0.0f)) {
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
      if (0 < cVar8) {
        local_e0.x = (plr->obj).pos.x;
        Awrd = 1;
        local_e0.y = (plr->obj).pos.y;
        iVar20 = -1;
        local_e0.z = (plr->obj).pos.z;
        if (cVar8 == '\b') {
          if (((plr->obj).idle_gametime == 0.0f) && (Level != 3)) {
            iVar20 = 4;
            Awrd = 4;
LAB_8001c484:
            local_e0.y = local_e0.y + 0.03;
          }
        }
        else if (cVar8 < '\t') {
          if (cVar8 == 1) {
LAB_8001c40c:
            if ((plr->obj).idle_gametime == 0.0f) {
              iVar20 = 2;
              Awrd = 2;
            }
          }
          else if ((cVar8 == 2) && ((plr->obj).idle_gametime == 0.0f)) {
            iVar20 = 1;
            Awrd = 2;
            goto LAB_8001c484;
          }
        }
        else if ((cVar8 < '\x0e') && ('\v' < cVar8)) goto LAB_8001c40c;
        if ((((Paused == 0) && (-1 < iVar20)) && ((plr->obj).ground != 0)) &&
           (jonframe1 == (jonframe1 / Awrd) * Awrd)) {
          AddVariableShotDebrisEffect(GDeb[iVar20].i,&local_e0,1,0,0);
        }
      }
      Awrd = (s32)(plr->obj).roof_type;
      if (((Awrd == -1) || ((TerSurface[Awrd].flags & 0x10) == 0)) || (VEHICLECONTROL == 1)) {
        (plr->obj).dangle = 0;
        if (cVar14 != 0) {
          sVar16 = plr->jump_frames;
          plr->jump = 6;
          (plr->obj).mom.y = 0.0f;
          plr->jump_frame = sVar16;
          plr->jump_type = 4;
          GameSfx(0x1c,pos);
        }
      }
      else {
        fVar6 = (plr->obj).SCALE;
        fVar15 = (plr->obj).roof_y - moveinfo->DANGLEGAP;
        if ((fVar15 <= (plr->obj).top * fVar6 + (plr->obj).pos.y) || (cVar14 != 0)) {
          fVar29 = (plr->obj).max.y;
          (plr->obj).dangle = 1;
          (plr->obj).pos.y = fVar15 - fVar29 * fVar6;
          if (cVar14 == 0) {
            GameSfx(0x1b,pos);
          }
        }
      }
      cVar14 = (plr->obj).layer_type;
      if ((cVar14 == 1) && (unaff_r19 == -1)) {
        GameSfx(0x47,pos);
      }
      else if ((cVar14 == -1) && (unaff_r19 == 1)) {
        GameSfx(0x47,pos);
      }
      TerrainFailsafe(&plr->obj);
      if ((plr->obj).got_shadow == 0) {
        bVar21 = ((plr->obj).bot * (plr->obj).SCALE + (plr->obj).pos.y) - (plr->obj).shadow < 0.025f;
      }
      else {
        bVar21 = (plr->obj).bot * (plr->obj).SCALE + (plr->obj).pos.y <= (plr->obj).shadow;
      }
      cVar14 = *(char *)((s32)&(plr->obj).gndflags + 1);
      (plr->obj).ground = 0;
      if (cVar14 != 0) {
        (plr->obj).ground = 1;
      }
      if (bVar21) {
        (plr->obj).ground = (plr->obj).ground | 2;
      }
      cVar14 = (plr->obj).ground;
      if (cVar14 != 0) {
        (plr->obj).last_ground = cVar14;
      }
      if ((((plr->obj).ground & 2U) != 0) && ((plr->obj).dead == 0)) {
        cVar14 = (plr->obj).surface_type;
        if ((((TerSurface[cVar14].flags & 1) == 0) ||
            (((uVar9 = (plr->obj).invincible, uVar9 != 0 && ((plr->obj).layer_type == -1)) ||
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
                if (((0) || (cVar14 != '\a')) || (CRemap[151] == -1)) goto LAB_8001c7b0;
                Awrd = 0xd;
              }
              else {
                Awrd = 8;
              }
            }
            else {
LAB_8001c7b0:
              Awrd = 5;
              if ((cVar8 != -1) && ((plr->obj).submerged != 0)) {
                Awrd = 10;
              }
            }
          }
          bVar21 = 0;
          if ((((plr->obj).mask != NULL) && ((plr->obj).mask->active != 0)) && ((LDATA->flags & 0xe00) == 0))  {
            bVar21 = 2 < (plr->obj).mask->active;
          }
          if ((((cVar8 == -1) || ((plr->obj).submerged != 0)) || (Level != 2)) || (Bonus != 0))  {
            (plr->obj).invincible = 0;
            KillGameObject(&plr->obj,Awrd);
            if ((((plr->obj).mask != NULL) && ((plr->obj).mask->active != 0)) &&
               (uVar12 = LDATA->flags, (uVar12 & 0xe00) == 0)) {
              AddMaskFeathers((plr->obj).mask);
              ((plr->obj).mask)->active = uVar12 & 0xe00;
            }
          }
          else if ((((plr->obj).mask == NULL) || ((plr->obj).mask->active == 0)) ||
                  ((LDATA->flags & 0xe00) != 0)) {
            KillGameObject(&plr->obj,Awrd);
          }
          else if (((plr->obj).mask->active < 3) && (uVar9 == 0)) {
            LoseMask(&plr->obj);
          }
          if ((((plr->obj).dead != 0) && (bVar21)) && (Awrd = NuSoundKeyStatus(4), Awrd == 1)) {
            NuSoundStopStream(4);
          }
        }
      }
      ObjectRotation(&plr->obj,2,(u32)(sVar19 == -1));
      if ((((plr->obj).transporting == 0) && (sVar19 != 0x89)) && (sVar19 != 0xa1)) {
        WumpaCollisions(&plr->obj);
      }
      plr_allow_jump = (s32)plr->allow_jump;
      if ((plr->obj).ground == 0) {
        if (plr->allow_jump != 0) {
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
        plr->land = 0;
        plr->ok_slam = 1;
        cVar14 = 1;
        plr->jump = 1;
        plr->somersault = 0;
        if ((bVar22 & 2) != 0) {
          cVar14 = 3;
        }
        plr->jump_type = cVar14;
        sVar16 = moveinfo->JUMPFRAMES1;
        plr->jump_frame = 0;
        plr->jump_frames = sVar16 + 1;
        fVar6 = moveinfo->JUMPHEIGHT;
        //local_68 = CONCAT44(0x43300000,(s32)moveinfo->JUMPFRAMES2 ^ 0x80000000);
        if (cVar14 == 3) {
          fVar6 = fVar6 * 1.5f;
        }
        (plr->obj).mom.y = fVar6 / (float)((s32)moveinfo->JUMPFRAMES2);
        plr->slam = 3;
        plr->slam_wait = 0;
        (plr->obj).anim.anim_time = 1.0f;
      }
      if (plr->fire != 0) {
        plr->fire = plr->fire + 0xff;
      }
      if (plr->tap != 0) {
        plr->tap = plr->tap + 0xff;
      }
    }
    else {
LAB_80019f04:
      if ((((((plr->obj).dead != 0) && (Level != 0x1d)) ||
           ((Cursor.menu != -1 &&
            ((((Cursor.menu != '\"' && (Cursor.menu != '$')) && (Cursor.menu != 0x12)) &&
             ((Cursor.menu != 0x10 || (Level != 0x1d)))))))) || (Cursor.wait != 0)) ||
         ((((cVar8 = (plr->obj).finished, cVar8 != 0 && (sVar19 != 99)) &&
           ((sVar19 != 0xa1 && (((sVar19 != 0x36 && (sVar19 != 0x8b)) && (sVar19 != 0x81)))))) &&
          (sVar19 != 0x3b)))) {
        cVar14 = (plr->obj).dead;
        if (cVar14 == '\n') {
          fVar6 = (plr->obj).pos.y;
          (plr->obj).pos.y = ((plr->obj).layer_shadow - fVar6) * 0.04 + fVar6;
        }
        else if ((sVar19 != 1) &&
                (((cVar14 == 6 || (cVar14 == 0x10)) ||
                 ((((cVar14 == 0x12 || ((cVar14 == 0x13 || (cVar14 == '\r')))) ||
                   (sVar19 == 0x6b)) || ((sVar19 == 0xa0 || (sVar19 == 0x99)))))))) {
          fVar6 = NewShadowMaskPlat(pos,0.0f,-1);
          (plr->obj).shadow = fVar6;
          dVar27 = 2000000.0f;
          if (fVar6 != 2000000.0f) {
            local_b0.x = (plr->obj).pos.x;
            local_b0.z = (plr->obj).pos.z;
            local_b0.y = ((plr->obj).bot + (plr->obj).top) * (plr->obj).SCALE * 0.5f +
                         (plr->obj).pos.y;
            fVar6 = CrateTopBelow(&local_b0);
            if ((fVar6 != dVar27) && ((plr->obj).shadow < fVar6)) {
              (plr->obj).shadow = fVar6;
            }
            if ((plr->obj).dead == 0x13) {
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
         (((plr->obj).dead == 0 && ((cVar8 == 0 || (in_finish_range != 0x3c)))))) {
        dVar27 = 0.0f;
        local_f0.y = 0.0f;
        if ((GameTimer.frame < 0x3c) ||
           (((((plr->obj).finished != 0 || (plr->freeze != 0)) ||
             ((Level == 0x16 && (FireBossHoldPlayer != 0)))) ||
            (((Cursor.menu == '\"' || (Cursor.menu == '$')) || (0 < fadeval)))))) {
          dVar27 = 0.0f;
          (plr->obj).pad_angle = 0;
          plr->pad_type = 1;
          local_f0.x = 0.0f;
          local_f0.z = 0.0f;
          (plr->obj).pad_speed = 0.0f;
          (plr->obj).pad_dx = 0.0f;
          (plr->obj).pad_dz = 0.0f;
        }
        else {
          uVar17 = pad->paddata;
          if ((uVar17 & 0xf000) == 0) {
            //local_68 = CONCAT44(0x43300000,(u32)pad->l_alg_x);
            dVar26 = pad->l_alg_x;
            //local_68 = CONCAT44(0x43300000,(u32)pad->l_alg_y);
            dVar28 = ((float)dVar26 - 127.5f);
            dVar26 = -((float)pad->l_alg_y - 127.5f);
            if ((float)(dVar28 * dVar28 + (float)(dVar26 * dVar26)) < 1806.25) {
              dVar24 = NuFabs(dVar28);
              dVar25 = 42.5f;
              if (dVar24 < 42.5f) {
                dVar28 = dVar27;
              }
              dVar24 = NuFabs(dVar26);
              if (dVar24 < dVar25) {
                dVar26 = dVar27;
              }
            }
            if ((dVar28 != 0.0f) || (dVar26 != 0.0f)) {
              cVar8 = 2;
              goto LAB_8001a324;
            }
          }
          else {
            if ((uVar17 & 0x8000) == 0) {
              if ((uVar17 & 0x2000) != 0) {
                dVar27 = 127.5f;
              }
            }
            else {
              dVar27 = -127.5f;
            }
            if ((uVar17 & 0x4000) == 0) {
              if ((uVar17 & 0x1000) == 0) {
                dVar26 = 0.0f;
              }
              else {
                dVar26 = 127.5f;
              }
            }
            else {
              dVar26 = -127.5f;
            }
            dVar28 = dVar27;
            if ((dVar27 != 0.0f) || (dVar26 != 0.0f)) {
              cVar8 = 1;
LAB_8001a324:
              plr->pad_type = cVar8;
            }
          }
          local_f0.z = (float)(dVar26 * 0.007843137718737125);
          local_f0.x = (float)(dVar28 * 0.007843137718737125);
          NuVecMag(&local_f0);
          Awrd = NuAtan2D(local_f0.x,local_f0.z);
          local_e0.y = 0.0f;
          local_e0.x = 0.0f;
          dVar27 = NuFabs(local_f0.x);
          dVar26 = NuFabs(local_f0.z);
          if (dVar27 <= dVar26) {
            dVar27 = NuFabs(local_f0.z);
          }
          else {
            dVar27 = NuFabs(local_f0.x);
          }
          local_e0.z = (float)dVar27;
          NuVecRotateY(&local_f0,&local_e0,Awrd);
          fVar6 = NuVecMag(&local_f0);
          if (0.2 <= fVar6) {
            if (0.6 <= fVar6) {
              dVar27 = moveinfo->RUNSPEED;
            }
            else {
              dVar27 = moveinfo->WALKSPEED;
            }
          }
          else {
            dVar27 = 0.0f;
          }
          (plr->obj).pad_speed = (float)dVar27;
          (plr->obj).pad_dx = local_f0.x;
          (plr->obj).pad_dz = local_f0.z;
          Awrd = NuAtan2D(local_f0.x,local_f0.z);
          (plr->obj).pad_angle = (u16)Awrd;
        }
        if (((((((VEHICLECONTROL == 1) && (sVar16 = (plr->obj).vehicle, sVar16 != 0x3b)) &&
               (sVar16 != 0x20)) && ((sVar16 != 0x6b && (sVar16 != 0xa0)))) && (sVar16 != 0x44)) &&
            ((sVar16 != 0xb2 && (sVar16 != 0x89)))) && ((sVar16 != 0xa1 && (sVar16 != 0x99)))) {
          (plr->obj).boing = 0;
          MoveVehicle(plr,pad);
          fVar6 = (plr->obj).pos.x - (plr->obj).oldpos.x;
          dVar28 = (fVar6 * fVar6);
          fVar6 = (plr->obj).pos.z - (plr->obj).oldpos.z;
          dVar26 = (fVar6 * fVar6);
          dVar27 = ((plr->obj).pos.y - (plr->obj).oldpos.y);
          fVar6 = NuFsqrt((float)(dVar28 + dVar26));
          (plr->obj).xz_distance = fVar6;
          fVar6 = NuFsqrt((float)((float)(dVar27 * dVar27 + dVar28) + dVar26));
          (plr->obj).xyz_distance = fVar6;
          goto LAB_8001cbd4;
        }
        if ((((sVar19 == 0x6b) || (sVar19 == 0xa0)) || (sVar19 == 0x99)) ||
           ((sVar19 == 0xa1 || (bVar21 = sVar19 != 0xb2, !bVar21)))) {
          bVar21 = 0;
          if ((Level == 3) && (0 < SmokeyCountDownValue)) {
            bVar21 = 1;
          }
          bVar1 = 0;
          if ((plr->obj).ground != 0) {
            bVar1 = (plr->obj).surface_type == '\n';
          }
          if (Level == 3) {
            fVar6 = SMOKEYSPEED;
            if (plr->fire != 0) {
              fVar6 = SMOKEYBOOSTSPEED;
            }
            dVar27 = (fVar6 * 0.01666667 * offroader_speedtime);
            if (bVar1) {
              dVar27 = (float)(dVar27 * 0.25);
            }
          }
          else if (Level == 0x16) {
            plr->sprint = 0;
            if (FireBossHoldPlayer == 0) {
              if ((pad->paddata & 0x80) != 0) {
                dVar27 = moveinfo->SPRINTSPEED;
                cVar8 = 1;
                goto LAB_8001a604;
              }
              dVar27 = moveinfo->RUNSPEED;
            }
            else {
              dVar27 = 0.0f;
            }
          }
          else {
            if (((pad->paddata & 0x88) == 0) && ((sVar19 != 0x99 || ((pad->paddata & 0x40) == 0))) )
            {
              dVar27 = moveinfo->RUNSPEED;
              cVar8 = 0;
            }
            else {
              dVar27 = moveinfo->SPRINTSPEED;
              cVar8 = 1;
            }
LAB_8001a604:
            plr->sprint = cVar8;
          }
          if (best_cRPos == NULL) {
            sVartmp = (plr->obj).thdg;
          }
          else {
            sVartmp = best_cRPos->angle;
          }
          (plr->obj).thdg = sVartmp;
          Awrd = 3;
          if ((best_cRPos != NULL) && (((best_cRPos->mode & 3) != 0 || (Level == 9)))) {
            if (Level == 3) {
              uVar17 = 0x2aab;
              Awrd = 5;
            }
            else if (sVar19 == 0xa0) {
              uVar17 = 0x2000;
            }
            else if ((sVar19 == 0x6b) || (sVar19 == 0xb2)) {
              uVar17 = 0x1555;
            }
            else {
              uVar17 = 0x1000;
            }
            //local_68 = CONCAT44(0x43300000,uVar17 ^ 0x80000000);
            if (!bVar21) {
              sVar16 = (short)(s32)((float)(uVar17) * (plr->obj).pad_dx);
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
            uVar17 = (u32)(plr->obj).hdg;
            local_c0.x = (float)(NuTrigTable[uVar17] * dVar27);
            local_c0.z = NuTrigTable[(uVar17 + 0x4000) & 0x2ffff] * dVar27;
          }
          (plr->obj).dangle = 0;
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
            (plr->obj).mom.z = (local_c0.z - fVar29) * fVar6 + fVar29;
            (plr->obj).mom.x = (local_c0.x - fVar15) * fVar6 + fVar15;
          }
          else {
            (plr->obj).mom.x = 0.0f;
            (plr->obj).mom.z = 0.0f;
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
          if (best_cRPos == NULL) {
            bVar1 = sVar19 == 0x20;
            (plr->obj).direction = 0;
          }
          else {
            uVar17 = RotDiff(best_cRPos->angle,(plr->obj).hdg);
            Awrd =  (uVar17 >= 0 ? uVar17 : -uVar17);
            //Awrd = ((s32)uVar17 >> 0x1f ^ uVar17) - ((s32)uVar17 >> 0x1f);
            if (Awrd < 0x2aab) {
              cVar8 = 0;
            }
            else {
              if (Awrd < 0x5555) {
                bVar1 = sVar19 == 0x20;
                (plr->obj).direction = 2;
                goto LAB_8001bb80;
              }
              cVar8 = 1;
            }
            bVar1 = sVar19 == 0x20;
            (plr->obj).direction = cVar8;
          }
        }
        else {
          bVar22 = (sVar19 == 0x3b) << 1;
          if ((sVar19 == 0x3b) && (best_cRPos != NULL)) {
            if ((Level == 0x1d) && (GameTimer.frame < 0xb4)) {
              local_b0.x = v000.x;
              local_b0.z = v000.z;
              local_b0.y = v000.y;
              dVar27 = 0.3330000042915344;
              (plr->obj).direction = 0;
            }
            else {
              if (((plr->obj).pad_speed <= 0.0f) || (plr->tap != 0)) {
                fVar6 = 0.333;
                local_b0 = v000;
              }
              else {
                local_b0.x = (plr->obj).pad_dx * moveinfo->WALKSPEED;
                local_b0.z = 0.0f;
                local_b0.y = -(plr->obj).pad_dz * moveinfo->WALKSPEED;
                NuVecRotateY(&local_b0,&local_b0,(u32)best_railangle);
                fVar6 = 1.0f;
              }
              dVar27 = fVar6;
              if ((pad->paddata_db & 0x60) == 0) {
                if (((pad->paddata & 0x60) != 0) && (Awrd = qrand(), Awrd < 0x4000)) {
                  Awrd = qrand();
                  if (Awrd < 0) {
                    Awrd = Awrd + 0x1ff;
                  }
                  NewRumble(&plr->rumble,Awrd >> 9);
                }
              }
              else {
                NewRumble(&player->rumble,0x9f);
              }
              uVar17 = pad->paddata & 0x60;
              if (plr->tap == 0) {
                if (uVar17 == 0x40) {
                  if ((plr->obj).direction == 0) {
                    local_b0.x = NuTrigTable[best_railangle] * moveinfo->RUNSPEED + local_b0.x;
                    dVar27 = 1.0f;
                    local_b0.z = NuTrigTable[(best_railangle + 0x4000) & 0x2ffff] * moveinfo->RUNSPEED + local_b0.z;
                  }
                  else {
                    (plr->obj).direction = 0;
                    plr->tap = '\x1e';
                  }
                }
                else if (uVar17 == 0x20) {
                  if ((plr->obj).direction == 1) {
                    local_b0.x = local_b0.x - NuTrigTable[best_railangle] * moveinfo->RUNSPEED;
                    dVar27 = 1.0f;
                    local_b0.z -= NuTrigTable[(best_railangle + 0x4000) & 0x2ffff] * moveinfo->RUNSPEED;
                  }
                  else {
                    (plr->obj).direction = 1;
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
            if (cVar8 == 1) {
              (plr->obj).thdg = uVar12 + 0x8000;
            }
            Awrd = RotDiff((plr->obj).hdg,(plr->obj).thdg);
            if ((plr->tap != 0) && (Awrd < 0)) {
              Awrd = Awrd + 0x10000;
            }
            bVar1 = 0;
            (plr->obj).hdg = (plr->obj).hdg + (short)(Awrd >> 4);
          }
          else {
            if (plr->target == 0) {
              NuVecRotateY(&local_f0,&local_f0,GameCam[0].yrot);
            }
            sVartmp = (plr->obj).hdg;
            plr->fire_lock = 0;
            Awrd = plr_target_found;
            plr_target_found = 0;
            iVar20 = (s32)plr->target;
            if ((iVar20 == 0) && (plr_target_found = iVar20, plr->target == 0)) {
              if (sVar19 == 0x20) {
                if ((0.0f < (plr->obj).pad_speed) && (sVartmp == (plr->obj).thdg)) {
                  uVar17 = (u32)(16384.0f - NuTrigTable[(plr->obj).pad_angle] * 16384.0f);
                  uVar4 = uVar17;
                  uVar12 = (u16)uVar4;
                  if (sVartmp == 0) {
                    sVartmp = uVar12;
                    if (0x4aab < uVar4) {
                      uVar13 = 0x4000;
LAB_8001b07c:
                      (plr->obj).thdg = uVar13;
                      sVartmp = uVar12;
                    }
                  }
                  else {
                    uVar13 = 0x8000;
                    if (sVartmp == 0x8000) {
                      sVartmp = uVar12;
                      if (uVar4 < 0x3555) {
                        uVar13 = 0x4000;
                        goto LAB_8001b07c;
                      }
                    }
                    else if (uVar4 < 0xaab) {
                      (plr->obj).thdg = 0;
                      sVartmp = uVar12;
                    }
                    else {
                      sVartmp = uVar12;
                      if (0x7555 < uVar4) goto LAB_8001b07c;
                    }
                  }
                }
                uVar12 = (plr->obj).hdg;
                uVar13 = (plr->obj).thdg;
                uVar17 = (u32)uVar12;
                uVar4 = (u32)uVar13;
                if (uVar17 < uVar4) {
                  if ((s32)(uVar4 - uVar17) < 0x2d8) {
LAB_8001b0c0:
                    (plr->obj).hdg = uVar13;
                  }
                  else {
                    (plr->obj).hdg = uVar12 + 0x2d8;
                  }
                }
                else if (uVar4 < uVar17) {
                  if ((s32)(uVar17 - uVar4) < 0x2d8) goto LAB_8001b0c0;
                  (plr->obj).hdg = uVar12 - 0x2d8;
                }
              }
              else if (VEHICLECONTROL == 2) {
                if (0.0f < (plr->obj).pad_speed) {
                  uVar17 = (u32)(plr->obj).pad_angle;
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
                     ((plr->slam != 0 && ((plr->slam < 3 && ((plr->obj).ground == 0)))) ))
                  {
                    (plr->obj).hdg = sVartmp - 0xa3d;
                    goto LAB_8001b3f4;
                  }
                  if (Cursor.menu == '$') {
                    Awrd = NuAtan2D(GameCam[0].pos.x - (plr->obj).pos.x,
                                    GameCam[0].pos.z - (plr->obj).pos.z);
                    iVar20 = 3;
                    uVar13 = (plr->obj).hdg;
                    uVar12 = (u16)Awrd;
                    (plr->obj).thdg = uVar12;
                  }
                  else {
                    if (((((plr->slam_wait != 0) || ((plr->obj).pad_speed <= 0.0f)) ||
                         (cVar8 = (plr->obj).dangle, cVar8 == 2)) ||
                        (((cVar8 != 0 && (plr->spin != 0)) &&
                         ((s32)plr->spin_frame <
                          (s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES)))) ||
                       (((plr->jump != 0 && (plr->jump_hold != 0)) ||
                        ((bVar1 = sVar19 == 0x44, bVar1 &&
                         (sVar16 = plr->fire, sVar16 = plr->tap, sVar16 != 0))))))
                    goto LAB_8001b3f4;
                    dVar26 = NuFabs(local_f0.x);
                    dVar28 = 0.0f;
                    if ((0.0f < dVar26) ||
                       (dVar26 = NuFabs(local_f0.z), dVar28 < dVar26)) {
                      Awrd = NuAtan2D(local_f0.x,local_f0.z);
                      (plr->obj).thdg = (u16)Awrd;
                    }
                    if (((((plr->slam == 0) || (2 < plr->slam)) ||
                         ((plr->obj).ground != 0)) &&
                        (((ExtraMoves == 0 && ((Game.powerbits & 4) == 0)) ||
                         ((plr->spin == 0 ||
                          ((s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES <=
                           (s32)plr->spin_frame)))))) && (plr->target == 0)) {
                      if ((plr->crawl != 0) ||
                         (((bVar1 || (!bVar21)) &&
                          ((plr->jump != 0 &&
                           ((plr->jump_type == 2 && (plr->jump_hold == 0)))))))) {
                        iVar20 = 4;
                      }
                      else if (((plr->tiptoe != 0) || (bVar1)) || (!bVar21)) {
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
                }
                else {
                  uVar12 = (plr->obj).thdg;
                  iVar20 = 2;
                  uVar13 = sVartmp;
                }
                uVar12 = SeekRot(uVar13,uVar12,iVar20);
                (plr->obj).hdg = uVar12;
              }
              else {
                (plr->obj).hdg = (plr->obj).thdg;
                (plr->obj).pad_speed = 0.0f;
              }
            }
            else {
              if (plr->fire == 0) {
                if (sVar19 == 0x44) {
                  if ((plr->obj).pad_speed <= 0.0f) {
                    sVar16 = (plr->obj).target_xrot;
                    iVar11 = (s32)(plr->obj).target_yrot;
                    iVar20 = (s32)sVar16;
                  }
                  else {
                    iVar20 = (s32)(-(plr->obj).pad_dz * 5461.0f);
                    sVar16 = (plr->obj).target_xrot;
                    iVar11 = (s32)((plr->obj).pad_dx * 10923.0f);
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
                  NuVecRotateX(&plr_target_dir,&v001,-(u32)(u16)(plr->obj).target_xrot & 0xffff);
                  NuVecRotateY(&plr_target_dir,&plr_target_dir,
                               (u32)(plr->obj).hdg + (u32)(u16)(plr->obj).target_yrot);
                  fVar6 = MECHTARGETHACK;
                }
                else {
                  iVar20 = (s32)(plr->obj).target_yrot +
                           (s32)((plr->obj).pad_dx * 16384.0f * 0.01666667);
                  if (iVar20 < -0x3555) {
                    iVar11 = -0x3555 - iVar20;
                    iVar20 = -0x3555;
LAB_8001ad90:
                    (plr->obj).hdg = sVartmp - (short)(iVar11 / 2);
                  }
                  else if (0x3555 < iVar20) {
                    iVar11 = 0x3555 - iVar20;
                    iVar20 = 0x3555;
                    goto LAB_8001ad90;
                  }
                  (plr->obj).target_yrot = (short)iVar20;
                  iVar20 = (s32)(plr->obj).target_xrot -
                           (s32)((plr->obj).pad_dz * 16384.0f * 0.01666667);
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
                  NuVecRotateX(&plr_target_dir,&v001,-(u32)(u16)(plr->obj).target_xrot & 0xffff);
                  NuVecRotateY(&plr_target_dir,&plr_target_dir,
                               (u32)(plr->obj).hdg + (u32)(u16)(plr->obj).target_yrot);
                  fVar6 = BAZOOKATARGETHACK;
                }
                plr_target_pos[0].x = plr_target_pos[0].x - plr_target_dir.x * fVar6;
                plr_target_pos[0].y = plr_target_pos[0].y - plr_target_dir.y * fVar6;
                plr_target_pos[0].z = plr_target_pos[0].z - plr_target_dir.z * fVar6;
                plr_target_sightpos.x = plr->mtxLOCATOR[8][1]._30;
                plr_target_sightpos.z = plr->mtxLOCATOR[8][1]._32;
                plr_target_sightpos.y = plr->mtxLOCATOR[8][1]._31;
              }
              GameRayCast(plr_target_pos,&plr_target_dir,10.0f,plr_target_pos + 1);
              NuMtxSetRotationX(&plr_target_mtx,-(plr->obj).target_xrot);
              NuMtxRotateY(&plr_target_mtx,
                           (u32)(plr->obj).hdg + (u32)(u16)(plr->obj).target_yrot);
              NuVecSub(&local_e0,plr_target_pos + 1,&plr_target_firepos);
              NuVecNorm(&local_e0,&local_e0);
              fVar6 = NuVecDot(&local_e0,&plr_target_dir);
              if (fVar6 <= 0.0f) {
                plr->fire_lock = 1;
              }
              if (((plr->fire_lock == 0) && (Awrd == 0)) && (plr_target_found != 0)) {
                GameSfx(5,pos);
              }
              GameSfxLoop(3,NULL);
              plr_target_frame = plr_target_frame + 1;
            }
LAB_8001b3f4:
            //bVar1 = sVar19 == 0x20;
            Awrd = RotDiff(sVartmp,(plr->obj).hdg);
            (plr->obj).dyrot = (short)Awrd;
            if (sVar19 != 0x20) {
              if (((VEHICLECONTROL == 2) && (plr->spin != 0)) &&
                 ((s32)plr->spin_frame <
                  (s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES)) {
LAB_8001b5ec:
                dVar27 = moveinfo->SPRINTSPEED;
              }
              else if (plr_rebound == 0) {
                if (((plr->slam_wait == 0) && (plr->target == 0)) &&
                   (((plr->jump == 0 || (plr->jump_hold == 0)) &&
                    ((sVar19 != 0x44 ||
                     (sVar3 = plr->fire, sVar3 = plr->tap, sVar3 == 0)))))) {
                  if ((plr->slam == 0) || ((2 < plr->slam || ((plr->obj).ground != 0))) )
                  {
                    cVar8 = (plr->obj).dangle;
                    if (cVar8 == 0) {
                      if (plr->slide == 0) {
                        if (plr->crawl == 0) {
                          if (plr->tiptoe == 0) {
                            if ((plr->obj).wade == 0) {
                              if (plr->sprint != 0) {
                                if (0.0f < (plr->obj).pad_speed) goto LAB_8001b5ec;
                                goto LAB_8001b5f4;
                              }
                            }
                            else if ((plr->obj).pad_speed <= 0.0f) {
LAB_8001b5f4:
                              dVar27 = 0.0f;
                            }
                            else {
                              dVar27 = moveinfo->WADESPEED;
                            }
                          }
                          else {
                            if ((plr->obj).pad_speed <= 0.0f) goto LAB_8001b5f4;
                            dVar27 = moveinfo->TIPTOESPEED;
                          }
                        }
                        else {
                          if ((plr->obj).pad_speed <= 0.0f) goto LAB_8001b5f4;
                          dVar27 = moveinfo->CRAWLSPEED;
                        }
                      }
                      else {
                        if ((plr->obj).character == 1) goto LAB_8001b548;
                        dVar27 = moveinfo->SLIDESPEED;
                      }
                    }
                    else {
                      if (((plr->obj).pad_speed == 0.0f) ||
                         (((plr->spin != 0 &&
                           ((s32)plr->spin_frame <
                            (s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES)) ||
                          (cVar8 == 2)))) goto LAB_8001b548;
                      dVar27 = moveinfo->DANGLESPEED;
                    }
                  }
                  else {
                    dVar27 = (float)(dVar27 * 0.1000000014901161);
                  }
                }
                else {
LAB_8001b548:
                  dVar27 = 0.0f;
                }
              }
              else {
                dVar27 = moveinfo->WALKSPEED;
              }
            }
            in_speed = (float)dVar27;
            in_s_friction = 0.005f;
            in_f_friction = 0.005f;
            if (bVar1) {
              in_s_friction = 0.001;
              NuVecRotateX(&local_e0,&v001,-(u32)(plr->obj).xrot);
              dVar27 = 0.0f;
              if ((plr->obj).pad_speed <= 0.0f) {
                local_e0.z = 0.0f;
                local_e0.y = 0.0f;
              }
              else {
                dVar26 = NuFabs((plr->obj).pad_dx);
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
              if (pos_START != NULL) goto LAB_8001b970;
LAB_8001b980:
              fVar6 = 0.0f;
            }
            else {
              if (VEHICLECONTROL != 2) {
                if (plr->slide == 0) {
                  if (plr->freeze == 0) {
                    if ((plr->obj).ground != 0) {
                      fVar6 = TerSurface[(plr->obj).surface_type].friction;
                      fVar15 = TerSurface[(plr->obj).surface_type].friction;
                      goto LAB_8001baf8;
                    }
                    if (((ExtraMoves != 0) || ((Game.powerbits & 4) != 0)) &&
                       ((plr->spin != 0 &&
                        ((s32)plr->spin_frame <
                         (s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES)))) {
                      fVar6 = 0.3333333;
                      goto LAB_8001ba80;
                    }
                    if (plr->jump != 0) {
                      if (plr->jump_type == 0) {
                        fVar6 = 0.5f;
                        fVar15 = fVar6;
                      }
                      else {
                        if (((sVar19 != 0x44) && (bVar21)) ||
                           ((plr->jump_type != 2 || (plr->jump_hold != 0))))
                        goto LAB_8001bafc;
                        fVar6 = 0.25;
                        fVar15 = fVar6;
                      }
LAB_8001baf8:
                      in_s_friction = fVar15 * 0.005f;
                      in_f_friction = fVar6 * 0.005f;
                    }
                  }
                  else {
                    if ((plr->obj).ground == 0) {
                      fVar6 = 0.5f;
                    }
                    else {
                      fVar6 = 0.15;
                    }
                    in_s_friction = fVar6 * 0.005f;
                    in_f_friction = fVar6 * 0.005f;
                  }
                }
                else {
                  fVar6 = 3.0f;
LAB_8001ba80:
                  in_s_friction = fVar6 * 0.005f;
                  in_f_friction = in_s_friction;
                }
LAB_8001bafc:
                if (((s32)((bVar22 & 2) >> 1)) && (best_cRPos != NULL)) {
                  in_s_friction = in_s_friction * 0.1f;
                  in_f_friction = in_f_friction * 0.1f;
                  Awrd = NuAtan2D(local_f0.x,local_f0.z);
                  sVartmp = (u16)Awrd;
                  MoveLoopXZ(&plr->obj,gotlist);
                }
                else {
                  MoveLoopXZ(&plr->obj,&(plr->obj).thdg);
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
              if (((plr->spin == 0) || (plr->spin_frames <= plr->spin_frame)) &&
                 (plr->tap == 0)) {
                in_s_friction = 0.0005;
              }
              NuVecRotateX(&local_e0,&v001,-(u32)(plr->obj).xrot);
              if ((plr->obj).thdg == 0x8000) {
                local_e0.z = -local_e0.z;
              }
              bVar21 = plr->spin != 0;
              if (((bVar21) &&
                  ((s32)plr->spin_frame <
                   (s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES)) ||
                 (5 < plr->tap)) {
                fVar15 = moveinfo->SPRINTSPEED;
                fVar6 = moveinfo->SPRINTSPEED;
LAB_8001b86c:
                local_e0.z = local_e0.z * fVar15;
                local_e0.y = local_e0.y * fVar6;
              }
              else {
                if (((bVar21) &&
                    ((s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES <=
                     (s32)plr->spin_frame)) || ((char)(plr->tap - 1) < 5)) {
                  fVar15 = moveinfo->RUNSPEED;
                  fVar6 = moveinfo->RUNSPEED;
                  goto LAB_8001b86c;
                }
                dVar27 = 0.0f;
                if ((plr->obj).pad_speed <= 0.0f) {
                  local_e0.z = 0.0f;
                  local_e0.y = -0.004166667;
                }
                else {
                  dVar26 = NuFabs((plr->obj).pad_dx);
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
              if (pos_START == NULL) goto LAB_8001b980;
LAB_8001b970:
              fVar6 = pos_START->x - (plr->obj).pos.x;
            }
            (plr->obj).mom.x = fVar6;
          }
        }
LAB_8001bb80:
        if (((LIFTPLAYER != 0) && ((pad->paddata & 0x10) != 0)) && ((plr->obj).transporting == 0)
           ) {
          fVar6 = (plr->obj).pos.y;
          (plr->obj).gndflags.all = 0;
          (plr->obj).pos.y = fVar6 + 0.1f;
          (plr->obj).mom.y = 0.0f;
          (plr->obj).ground = 0;
        }
        if (FRAME == 0) {
          tbslotBegin(app_tbset,7);
        }
        if ((plr->obj).transporting == 0) {
          local_d0.x = (plr->obj).pos.x;
          local_d0.y = (plr->obj).bot * (plr->obj).SCALE + (plr->obj).pos.y;
          local_d0.z = (plr->obj).pos.z;
          if ((((plr->obj).ground == 1) && ((plr->obj).mom.x == 0.0f)) &&
             (((plr->obj).mom.z == 0.0f && (0.1f < local_d0.y - (plr->obj).shadow)))) {
            (plr->obj).gndflags.all = 0;
          }
          fVar6 = 0.0f;
          if ((plr->obj).pad_speed <= 0.0f) {
            fVar6 = 0.003;
          }
          fVar15 = (plr->obj).RADIUS;
          NewTerrainScaleY(&local_d0,&(plr->obj).mom,&(plr->obj).gndflags.chrs,
                           (u32)&plr[0x9eb25].lights.dir2.Direction.y / 0xce4,fVar6,fVar15,
                           (((plr->obj).top - (plr->obj).bot) * (plr->obj).SCALE) / (fVar15 + fVar15));
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
                Awrd = GetDieAnim(&plr->obj,-1);
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
          KillPlayer(&plr->obj,Awrd);
        }
        if ((((plr->obj).mask != NULL) && (2 < (plr->obj).mask->active)) &&
           ((plr->obj).mask->active = (plr->obj).mask->active + -1, ((plr->obj).mask)->active == 2)) {
          GameMusic((s32)LDATA->music[0],0);
        }
        (plr->obj).transporting = 0;
        BonusTransporter(plr);
        DeathTransporter(plr);
        GemPathTransporter(plr);
        if ((plr->obj).dead == 0) {
          if ((((plr->obj).transporting == 0) && (GetTopBot(plr), sVar19 != 0x89)) &&
             (sVar19 != 0xa1)) {
            PlayerCreatureCollisions(&plr->obj);
            if ((plr->obj).dangle != 0) {
              plr->slam = 3;
            }
            HitItems(&plr->obj);
          }
          if ((plr->obj).dead == 0) {
            if ((plr->obj).transporting == 0) {
              GetTopBot(plr);
              NewTopBot(&plr->obj);
              if ((Level == 1) &&
                 ((Awrd = PlayerObjectAnimCollision(&plr->obj,ObjTab[58].obj.special,0.4), Awrd != 0 ||
                  (Awrd = PlayerObjectAnimCollision(&plr->obj,ObjTab[59].obj.special,0.4), Awrd != 0))))  {
                KillPlayer(&plr->obj,3);
              }
            }
            if (((((plr->obj).dead == 0) && ((plr->obj).transporting == 0)) &&
                (GetTopBot(plr), sVar19 != 0x89)) && (sVar19 != 0xa1)) {
              CrateCollisions(&plr->obj);
            }
          }
        }
        bVar22 = (plr->obj).boing;
        if (bVar22 == 0) {
          if ((temp_crate_y_ceiling_adjust != 0) && (plr->jump != 0)) {
            plr->jump = 6;
            plr->jump_frame = plr->jump_frames;
            plr->jump_type = 4;
          }
        }
        else {
          plr->land = 0;
          plr->ok_slam = 1;
          cVar8 = 1;
          plr->jump = 1;
          plr->somersault = 0;
          if ((bVar22 & 2) != 0) {
            cVar8 = 3;
          }
          plr->jump_type = cVar8;
          sVar16 = moveinfo->JUMPFRAMES1;
          plr->jump_frame = 0;
          plr->jump_frames = sVar16 + 1;
          fVar6 = moveinfo->JUMPHEIGHT;
          //local_68 = CONCAT44(0x43300000,(s32)moveinfo->JUMPFRAMES2 ^ 0x80000000);
          if (cVar8 == 3) {
            fVar6 = fVar6 * 1.5f;
          }
          (plr->obj).mom.y = fVar6 / (float)((s32)moveinfo->JUMPFRAMES2);
          (plr->obj).anim.anim_time = 1.0f;
          (plr->obj).ground = 0;
        }
        if (((temp_crate_y_floor_adjust != 0) || (temp_crate_y_ceiling_adjust != 0)) ||
           (temp_crate_xz_adjust != 0)) {
          (plr->obj).pos_adjusted = 1;
        }
        goto LAB_8001c028;
      }
    }
    if (sVar19 == 0x20) {
      MoveSUBMARINE(plr,pad);
    }
    else if (sVar19 == 0x6b) {
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
    (plr->obj).boing = 0;
  }
LAB_8001cbd4:
  if (((plr->obj).dead != 0) && (InvincibilityCHEAT != 0)) {
    (plr->obj).dead = 0;
    plr_lives.count = (short)Game.lives;
  }
  if (Adventure != 0) {
    Game.wumpas = (u8)plr_wumpas.count;
    Game.lives = (u8)plr_lives.count;
    if (((plr->obj).mask != NULL) && (Game.mask = 2, (plr->obj).mask->active < 3)) {
      Game.mask = *(u8 *)((s32)&(plr->obj).mask->active + 1);
    }
  }
  BonusTiming(plr);
  fVar6 = (plr->obj).pos.x - (plr->obj).oldpos.x;
  dVar28 = (fVar6 * fVar6);
  fVar6 = (plr->obj).pos.z - (plr->obj).oldpos.z;
  dVar26 = (fVar6 * fVar6);
  dVar27 = ((plr->obj).pos.y - (plr->obj).oldpos.y);
  fVar6 = NuFsqrt((float)(dVar28 + dVar26));
  (plr->obj).xz_distance = fVar6;
  fVar29 = NuFsqrt((float)((float)(dVar27 * dVar27 + dVar28) + dVar26));
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
    if (1) {
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
  if (cVar14 == 0) {
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
  if ((cVar14 != 0x16) && (cVar14 != 4)) {
    UpdateAnimPacket(mod,&(plr->obj).anim,0.5f,(plr->obj).xz_distance);
  }
  (plr->obj).frame = (plr->obj).frame + 1;
  if ((Cursor.menu == -1) && (GameMode != 1)) {
    if (Level == 0x25) {
      Awrd = InLoadSaveZone(plr);
      if (Awrd == 0) {
        loadsave_frame = 0;
      }
      else {
        if ((pad == NULL) || ((pad->paddata_db & 0x40) == 0)) {
          loadsave_frame = loadsave_frame + 1;
        }
        else {
          loadsave_frame = 0x3c;
        }
        if (0x3c < loadsave_frame) {
          loadsave_frame = 0x3d;
        }
      }
      if (loadsave_frame == 0x3c) {
        NewMenu(&Cursor,0x15,4,-1);
        loadsave_frame = 0x3d;
        GameSfx(0x36,NULL);
      }
    }
  }
  else {
    loadsave_frame = 0;
  }
  return;
}

//NGC MATCH
void ResetAnimPacket(struct anim_s *anim,s32 action) {
  if (anim == NULL) {
    return;
  }
  anim->newaction = (short)action;
  anim->oldaction = (short)action;
  anim->action = (short)action;
  anim->anim_time = 1.0f;
  anim->blend = '\0';
  anim->flags = '\0';
  return;
}
