#include "gamecode/creature.h"

s32 gamecut; //cut.c

#define ABS(x) (x >= 0 ? x : -x)

/*
	//TODO
	ManageCreatures 93%
	LoadCharacterModel 95%
	LoadCharacterModels MATCH (Check)
	MovePlayer 83%*
	DrawCharacterModel 96%
	UpdateAnimPacket 99%*
	DrawCreatures 90%
	
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
        space = (struct space_s*)SpaceGameCutTab[gamecut][0];
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

//NGC MATCH
void LoadCharacterModels(void) {
    s32 i;
    s32 j;
    s32 character;
    s32 cmodel_index;
    struct space_s *space;
    char charsdat_filename[128];
    s32 clist_entry;
    s32 remap;

    if(CModel[0].hobj != NULL) {
        clist_entry = 1;
    } else {
        clist_entry = 0;
    }
    
    if (Level == 0x28) {
        space = (struct space_s *)SpaceGameCutTab[gamecut][0];
    }
    else {
        space = NULL;
    }
   if (LDATA->clist != NULL) {
        i = clist_entry;
        remap = i;
        cmodel_index = i;
loop_1:
            force_decal = 0;
            if (space != NULL) {
                // fakery ??
                 j = i << 5;
                character = ((s32*)space)[j]; //space[j].character
            }
            else {
                character = LDATA->clist[i];
            }
            
            if ((character == 0x1b) || (character == 0x86)) {
                force_decal = 1;
            }
            if (LoadCharacterModel(character, Level, &cmodel_index, i, &remap) != 0) 
            {
                i++;
                goto loop_1;
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

//90.69% NGC
void DrawCreatures(struct creature_s *c, s32 count, s32 render, s32 shadow) {
    struct nuvec_s s; // 0x10(r1)
    s32 i; // 0x13C(r1)
    s32 j; // r30
    s32 vflag; // r30
    s32 shflag; // r18
    s32 reflect; // r17
    s32 action; // r0
    s32 frames; // r0
    s32 old_frame; // r21
    s32 PLAYER; // 
    s32 VEHICLE; // r28
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
    struct CharacterModel* model[2]; // 0x130(r1)
    struct numtx_s* m; // r3

  s32 bVar9;
  s32 bVar10;
  s32 bVar11;
  s32 iVar12;
  u32 uVar18;
  
    if (
        (DRAWCREATURESHADOWS == 0)
        || (Level == 0x1d)
        || (Level == 0x24)
        || ((Level == 0x1e) && (level_part_2 != 0))
        || (Level == 0x1a)
        || ((LDATA->flags & 0x1000) != 0)
        || (VEHICLECONTROL == 2)
        || ((VEHICLECONTROL == 1) && ((player->obj).vehicle == 0x20))
    ) {
        shadow = 0;
    }
    
    if (
        ((LDATA->flags & 0x202) != 0)
        || (Level == 0x1c)
    ) {
        r2 = (s32)LDATA->farclip;
    } else {
        r2 = AIVISRANGE;
    }
    
    if (LDATA->farclip < r2) {
        r2 = LDATA->farclip;
    }
    
    r2 = (r2 * r2);
    
    for (i = 0; i < count; i++, c++) {
        vflag = c->obj.flags & 1;

        // if (vflag != 0){
        //     if (
        //         ((in_finish_range == 0x3C)
        //         || (c->obj.finished != 0x00)
        //         || ((Level == 0x25) && (warp_level != -1)))
        //     )
        //     {
        //         continue;
        //     }
        //     if (VEHICLECONTROL == 1) {
        //         VEHICLE = (s16)c->obj.vehicle;
        //         if (c->obj.vehicle == -1) {
        //             VEHICLE = -1;
        //         }
        //     }
        // } else {
        //     VEHICLE = -1;
        // }
        
        
        if (vflag == 0) {
            // VEHICLE = -1;
            goto here;
        }
        
        if (
            ((in_finish_range != 0x3C)
            && (c->obj.finished == 0x00)
            && ((Level != 0x25) || (warp_level == -1)))
        )
        {
            if ((VEHICLECONTROL == 1) && (c->obj.vehicle != -1)) {
                VEHICLE = c->obj.vehicle;
            } else {
                here:
                VEHICLE = -1;
            }
            
            old_frame = c->obj.draw_frame;
            c->obj.draw_frame = 0;
            if (
                (c->used != 0x00) 
                && (c->on != 0x00) 
                && (c->obj.model != NULL) 
                && (c->obj.dead != 0x16)
                && (c->obj.dead != 0x04)
                && (c->obj.dead != 0x07)
                ) 
            {
                if (
                    (Level != 0x17) 
                    || ((glass_phase == 0) && (c->obj.character != 0x7F))
                    || ((glass_phase != 0) && (c->obj.character == 0x7F))
                    ) 
                {
                    
                    if (
                           ((c->obj.invisible == 0) || (c->obj.character == 0x77)) 
                        && ((c->obj.invincible == 0) || (1 < (c->obj.invincible & 3)))
                        // && ((LDATA->flags & 0x200) == 0)
                        // && ((LDATA->flags & 0x200) != 0 || (dx <= r2))
                    ) {
                        // fVar4 = (pCam->pos.z - c->obj.pos.z);
                        // fVar8 = (pCam->pos.x - c->obj.pos.x); 
                        dx = (((pCam->pos.x - c->obj.pos.x) * (pCam->pos.x - c->obj.pos.x)) + ((pCam->pos.z - c->obj.pos.z) * (pCam->pos.z - c->obj.pos.z)));
                        if (((LDATA->flags & 0x200) != 0) || !(dx > r2)) {
                            bVar9 = 0;
                            bVar10 = 0;
                            bVar11 = 0;
                            
                            // switch(VEHICLE) {
                            //     case 0x36:
                            //     case 0x81:
                            //     case 0x53:
                            //     case 0x8B:
                            //     case 0x63:
                            if (
                                (VEHICLE == 0x63)
                                || (VEHICLE == 0x36)
                                || (VEHICLE == 0x81)
                                || (VEHICLE == 0x53)
                                || (VEHICLE == 0x8B)
                            )
                            {
                                if (render == 0) {
                                    // c->anim_processed = 1;
                                    continue;
                                    // goto LAB_8001f958;
                                }
                            
                                SetCreatureLights(c);
                                switch(VEHICLE) {
                                    case 0x81:
                                        ForceShader = 0x80;
                                    case 0x8B:
                                    case 0x36:
                                        DrawGlider(c);
                                        ForceShader = -1;
                                        c->obj.draw_frame = old_frame + 1;
                                        mV = mTEMP;
                                        bVar9 = 1;
                                        break;
                                    case 0x53:
                                        ForceShader = 0x80;
                                        DrawAtlas(c);
                                        bVar9 = 1;
                                        ForceShader = -1;
                                        c->obj.draw_frame = old_frame + 1;
                                        NuMtxSetRotationY(&mV, (uint)c->obj.hdg);
                                        NuMtxTranslate(&mV, &c->obj.pos);
                                        mV._31 = mV._31 + ATLASPLAYERLIFT;
                                        break;
                                    case 0x63:
                                        m = DrawPlayerJeep(c);
                                        c->obj.draw_frame = old_frame + 1;
                                        if (m == NULL) {
                                            continue;
                                        } 
                                        mV = *m;
                                        bVar9 = 1;
                                        break;
                                }
                            }
                            LAB_8001e374:
                            //bVar3 = vflag == 0;
                            //bVar27 = bVar3 << 1;
                            if (vflag != 0) {
                                if (
                                    (VEHICLECONTROL == 2)
                                    && (c->obj.dead == 0x00)
                                    && (c->spin != 0x00)
                                    && (c->spin_frame < (c->spin_frames - c->OnFootMoveInfo->SPINRESETFRAMES)) 
                                    && (bVar9 == 0)
                                )
                                {
                                    if (CRemap[116] != -1) {
                                        s.x = s.y = s.z = c->obj.SCALE;
                                        yrot = c->obj.hdg - 0x8000;
                                        NuMtxSetScale(&mC, &s);
                                        NuMtxRotateZ(&mC, c->spin_frame * 0x1555);
                                        NuMtxRotateX(&mC, c->obj.xrot);
                                        NuMtxRotateY(&mC, yrot);
                                        NuMtxTranslate(&mC, &c->obj.pos);
                                        iVar12 = (s32)CRemap[116];
                                        model[0] = CModel + iVar12;
                                        if (model[0]->anmdata[0x46] != NULL) {
                                            NuHGobjEvalAnim(model[0]->hobj, 
                                                model[0]->anmdata[0x46],
                                                (model[0]->anmdata[0x46]->time - 1.0f) * 
                                                ((float)c->spin_frame / (float)(c->OnFootMoveInfo->SPINFRAMES + c->OnFootMoveInfo->SUPERSPINFRAMES * 3))
                                                + 1.0f,
                                            // (float)(
                                            // (float)(model[0].anmdata[0x46]->time - 1.0f) *
                                            // ((float)(CONCAT44(0x43300000, (s32)c->spin_frame ^ 0x80000000) - 4503601774854144.0f) /
                                            // (float)(CONCAT44(0x43300000, (s32)c->OnFootMoveInfo->SPINFRAMES + c->OnFootMoveInfo->SUPERSPINFRAMES * 3 ^ 0x80000000) - 4503601774854144.0f)) 
                                            //  + 1.0f),
                                                0,
                                                NULL,
                                                tmtx
                                            );
                                        }
                                        else {
                                            NuHGobjEval(model[0]->hobj, 0, NULL, tmtx);
                                        }
                                        
                                        if (glass_draw == 0) {
                                            StoreLocatorMatrices(model[0], &mC, tmtx, &c->mtxLOCATOR[0][0], &c->momLOCATOR[0][0]);
                                        }
                                        
                                        if (render == 0) {
                                            continue;
                                            // goto LAB_8001f958;
                                        }
                                        //bVar2 = local_d0._02 != 0.0f;
                                        SetCreatureLights(c);
                                        NuHGobjRndrMtx(model[0]->hobj, &mC, 1, NULL, tmtx);
                                        if (
                                            (c->obj.reflect_y != 2000000.0f)
                                            && (glass_mix == 0.0f)
                                            && (glass_draw == 0)
                                        ) {
                                            mR = mC;
                                            mR._01 = -mR._01;
                                            mR._11 = -mR._11;
                                            mR._21 = -mR._21;
                                            // fVar4 = c->obj.reflect_y;
                                            mR._31 -= (mR._31 - c->obj.reflect_y);
                                            NuHGobjRndrMtx(model[0]->hobj, &mR, 1, NULL, tmtx);
                                        }
                                        if (
                                            (shadow != 0)
                                            && (c->obj.shadow != 2000000.0f)
                                            && (dx < r2)
                                            && ((TerSurface[c->obj.surface_type].flags & 1) == 0)
                                            && (SKELETALCRASH == 0)
                                            && (c->freeze == 0)
                                            && (glass_draw == 0)
                                        )
                                        {
                                            ScaleFlatShadow(&s, c->obj.pos.y, c->obj.shadow, c->obj.SCALE);
                                            NuMtxSetScale(&mS, &s);
                                            NuMtxRotateY(&mS, vflag);
                                            NuMtxRotateZ(&mS, (uint)c->obj.surface_zrot);
                                            NuMtxRotateX(&mS, (uint)c->obj.surface_xrot);
                                            mS._30 = c->obj.pos.x;
                                            mS._31 = c->obj.shadow + 0.025f;
                                            mS._32 = c->obj.pos.z;
                                        }
                                    }
                                    LAB_8001ed04:
                                    // c->obj.draw_frame = uVar22 + 1;
                                    // goto LAB_8001f958;
                                }
                                else if (
                                    (vflag != 0) 
                                    && (c->obj.character == 0)
                                    && (c->obj.dead == 0)
                                    && (c->spin != 0)
                                    && (c->spin_frame < (c->spin_frames - c->OnFootMoveInfo->SPINRESETFRAMES))
                                    && (bVar9 == 0)
                                    && (VEHICLE == -1) 
                                    && (c->freeze == 0)
                                ) {
                                    if (render != 0) {
                                        SetCreatureLights(c);
                                    }
                                    
                                    if (CRemap[8] != -1) {
                                        yrot = -(c->spin_frame * 0x1555);
                                        s.x = s.y = s.z = c->obj.SCALE;
                                        NuMtxSetScale(&mC, &s);
                                        NuMtxRotateY(&mC, yrot);
                                        NuMtxRotateZ(&mC, c->obj.zrot);
                                        NuMtxRotateX(&mC, c->obj.xrot);
                                        NuMtxTranslate(&mC, &c->obj.pos);
                                        
                                        if ((SKELETALCRASH != 0) && (CRemap[159] != -1)) {
                                            model[0] = CModel + CRemap[159];
                                        } else {
                                            model[0] = CModel + CRemap[8];
                                        }
                                        
                                        if (c->obj.dangle != 0) {
                                            uVar18 = 0x47;
                                        } else {
                                            uVar18 = 0x46;
                                        }
                                        
                                        if ((uVar18 < 0x76) && ((model[0]->anmdata[uVar18]) != NULL)) {
                                            NuHGobjEvalAnim(
                                                model[0]->hobj,
                                                model[0]->anmdata[uVar18],
                                                (float)(model[0]->anmdata[uVar18]->time - 1.0f) * ((float)c->spin_frame / (float)(c->OnFootMoveInfo->SPINFRAMES + c->OnFootMoveInfo->SUPERSPINFRAMES * 3)) + 1.0f,
                                                0,
                                                NULL,
                                                tmtx
                                            );
                                        }
                                        else {
                                            NuHGobjEval(model[0]->hobj, 0, NULL, tmtx);
                                        }
                                        
                                        if (glass_draw == 0) {
                                            StoreLocatorMatrices(model[0], &mC, tmtx, &c->mtxLOCATOR[0][0], &c->momLOCATOR[0][0]);
                                        }
                                        
                                        if (render == 0) {
                                            continue;
                                        }
                                        //bVar2 = local_d0._02 != 0.0f;
                                        NuHGobjRndrMtx(model[0]->hobj, &mC, 1, NULL, tmtx);
                                        if (
                                            (c->obj.reflect_y != 2000000.0f)
                                            && (glass_mix == 0.0f)
                                            && (glass_draw == 0)
                                        ) {
                                            mR = mC;
                                            mR._01 = -mR._01;
                                            mR._11 = -mR._11;
                                            mR._21 = -mR._21;
                                            if (Level != 0x25) {
                                                y = c->obj.reflect_y;
                                            } else {
                                                y = HUBREFLECTY;
                                            }
                                            mR._31 = y - (mR._31 - y);
                                            NuHGobjRndrMtx(model[0]->hobj, &mR, 1, NULL, tmtx);
                                        }
                                        if (
                                            (shadow != 0) 
                                            && (c->obj.shadow != 2000000.0f)
                                            && (dx < r2)
                                            && ((TerSurface[c->obj.surface_type].flags & 1) == 0) 
                                            && (SKELETALCRASH == 0) 
                                            && (c->freeze == 0)
                                            && (glass_draw == 0)
                                        ) {
                                            ScaleFlatShadow(&s, c->obj.pos.y, c->obj.shadow, c->obj.SCALE);
                                            NuMtxSetScale(&mS, &s);
                                            NuMtxRotateY(&mS, yrot);
                                            NuMtxRotateZ(&mS, c->obj.surface_zrot);
                                            NuMtxRotateX(&mS, c->obj.surface_xrot);
                                            mS._30 = c->obj.pos.x;
                                            mS._31 = c->obj.shadow + 0.025f;
                                            mS._32 = c->obj.pos.z;
                                            NuMtlSetStencilRender(NUSTENCIL_REPLACE_NODRAW);
                                            NuHGobjRndrMtx(model[0]->hobj, &mS, 1, NULL, tmtx);
                                            NuMtlSetStencilRender(NUSTENCIL_NOSTENCIL);
                                        }
                                    }
                                    if (render == 0) {
                                        continue;
                                    }
                                    if (
                                        (c->obj.dangle != 0) 
                                        && (CRemap[9] != -1)
                                    ) {
                                        s.x = s.y = s.z = c->obj.SCALE;
                                        yrot = c->obj.hdg - 0x8000;
                                        NuMtxSetScale(&mC, &s);
                                        NuMtxRotateY(&mC, yrot);
                                        NuMtxRotateZ(&mC, c->obj.zrot);
                                        NuMtxRotateX(&mC, c->obj.xrot);
                                        NuMtxTranslate(&mC, &c->obj.pos);
                                        model[1] = CModel + CRemap[9];
                                        NuHGobjRndr(model[1]->hobj, &mC, 1, NULL);
                                        
                                        if (
                                            (c->obj.reflect_y != 2000000.0f)
                                            && (glass_mix == 0.0f)
                                            && (glass_draw == 0)
                                        ) {
                                            mR = mC;
                                            mR._01 = -mR._01;
                                            mR._11 = -mR._11;
                                            mR._21 = -mR._21;
                                            mR._31 = c->obj.reflect_y - (mR._31 - c->obj.reflect_y);
                                            NuHGobjRndr(model[1]->hobj, &mR, 1, NULL);
                                        }
                                    }
                                    continue;
                                }
                            }
                            
                            LAB_8001ecd4:
                            // sVar7 = c->obj.character;
                            if ((c->obj.character == 0x77U) && (c->obj.invisible != 0)) {
                                if (render != 0) {
                                    Draw3DCrateCount(&c->obj.pos, c->obj.hdg);
                                    c->obj.draw_frame = old_frame + 1;
                                }
                                continue;
                            } 
                            
                            if (
                                (render == 0) 
                                || (c->obj.character == 0x75)
                                || (c->obj.character == 0x77)
                                || (c->obj.character == 0x78)
                                || (c->obj.character == 0x79)
                                || (c->obj.character == 0x7A)
                                || (c->obj.character == 0x7B)
                                || (c->obj.character == 0x7C)
                            )
                            {
                                yrot = c->obj.hdg - 0x8000;
                                if (vflag != 0) {
                                    if ((c->obj.dead == 0x03) || (c->obj.dead == 0x08)) {
                                        yrot -= 0x8000;
                                    }
                                    else if (c->freeze != 0x00) {
                                        yrot = GameCam->hdg_to_player;
                                    }
                                    else {
                                        if (
                                            (c->spin != 0x00)
                                            && (c->spin_frame < (c->spin_frames - c->OnFootMoveInfo->SPINRESETFRAMES))
                                            && (VEHICLE == 0x3b)
                                            && (c->obj.anim.newaction == 0x69)
                                            && ((c->obj.model)->anmdata[0x69] != NULL)
                                        ) {
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
                                    if (
                                        (vflag == 0)
                                        && (
                                            (c->obj.vehicle == 0xa1)
                                            || (VEHICLECONTROL == 2)
                                            || (VEHICLE == 0x20)
                                            || (VEHICLE == 0x89) 
                                            || (VEHICLE == 0xa1)
                                        )
                                    ) {
                                        s.x = s.y = s.z = c->obj.SCALE;
                                        NuMtxSetScale(&mC, &s);
                                        NuMtxRotateZ(&mC, c->obj.zrot);
                                        
                                        if (VEHICLE == 0x20) {
                                            uVar18 = RotDiff(0, c->obj.xrot) / 4;
                                        }
                                        else {
                                            uVar18 = c->obj.xrot;
                                        }
                                        
                                        NuMtxRotateX(&mC, uVar18);
                                        NuMtxRotateY(&mC, yrot);
                                        NuMtxTranslate(&mC, &c->obj.pos);
                                        c->m = mC;
                                    }
                                    else {
                                        if (
                                            (Level == 0x17)
                                            && (c->obj.character == 0x7f)
                                        ) {
                                            a = ((GameTimer.frame % 0x24) * 0x10000) / 0x24;
                                            
                                            s.x = (NuTrigTable[a] * 0.05f + 1.0f);
                                            s.y = (NuTrigTable[a + 0x4000] * 0.05f + 1.0f);
                                            s.z = (NuTrigTable[a] * 0.05f + 1.0f);
                                            
                                            s.x *= c->obj.SCALE;
                                            if (s.x < 0.0f) {
                                                s.x = 0.0f;
                                            }
                                            
                                            s.y *= c->obj.SCALE;
                                            if (s.y < 0.0f) {
                                                s.y = 0.0f;
                                            }
                                            
                                            s.z *= c->obj.SCALE;
                                            if (s.z < 0.0f) {
                                                s.z = 0.0f;
                                            }
                                        }
                                        else {
                                            s.x = s.y = s.z = c->obj.SCALE;
                                        }
                                        
                                        if ((c->obj.flags & 0x10000) != 0) {
                                            s.y = -s.y;
                                        }
                                        
                                        NuMtxSetScale(&mC, &s);
                                        NuMtxRotateY(&mC, yrot);
                                        NuMtxRotateZ(&mC, c->obj.zrot);
                                        NuMtxRotateX(&mC, c->obj.xrot);
                                        NuMtxTranslate(&mC, &c->obj.pos);
                                        c->m = mC;
                                    }
                                    
                                }
                                
                                if (render != 0) {
                                    if (
                                        (c->obj.reflect_y != 2000000.0f)
                                        && ((c != player) || (c->obj.dead != 0x02))
                                        && (glass_draw == 0)
                                    ) {
                                        mR = mC;
                                        mR._01 = -mR._01;
                                        mR._11 = -mR._11;
                                        mR._21 = -mR._21;
                                        
                                        if (Level != 0x25) {
                                            y = c->obj.reflect_y;
                                        } else {
                                            y = HUBREFLECTY;
                                        }
                                        
                                        bVar11 = 1;
                                        mR._31 = y - (mR._31 - y);
                                    }
                                    
                                    if (
                                        (render != 0)
                                        && (shadow != 0)
                                        && (c->obj.shadow != 2000000.0f)
                                        && (dx < r2)
                                        && ((TerSurface[c->obj.surface_type].flags & 1) == 0)
                                        && (c->freeze == 0x00)
                                        && (glass_draw == 0)
                                        && ((c->obj.flags & 0x4000) == 0)
                                        && (!bVar9)
                                        && (c->obj.dead != 0x08)
                                        && ((vflag == 0) || (SKELETALCRASH == 0))
                                        && (VEHICLE != 0xA1)
                                        && (VEHICLE != 0x89)
                                        && (VEHICLE != 99)
                                        && ((vflag == 0) || (c->obj.vehicle != 0xa1))
                                        ) 
                                    {
                                        ScaleFlatShadow(&s, c->obj.pos.y, c->obj.shadow, c->obj.SCALE);
                                        NuMtxSetScale(&mS, &s);
                                        NuMtxRotateY(&mS, yrot);
                                        NuMtxRotateZ(&mS, c->obj.surface_zrot);
                                        NuMtxRotateX(&mS, c->obj.surface_xrot);
                                        
                                        mS._30 = c->obj.pos.x;
                                        if (c->obj.dead == 1) {
                                            mS._31 = c->obj.pos.y + (c->obj.shadow - c->obj.oldpos.y);
                                        }
                                        mS._31 = c->obj.shadow + 0.025f;
                                        mS._32 = c->obj.pos.z;
                                        
                                        if (VEHICLE == 0x99) {
                                            if (Level == 3) {
                                                mS._31 = mS._31 + 0.05f;
                                            } else {
                                                mS._31 = mS._31 + 0.025f;
                                            }
                                        }
                                        
                                        bVar10 = 1;
                                        temp_surface_xrot = c->obj.surface_xrot;
                                        temp_surface_yrot = c->obj.hdg;
                                        temp_surface_zrot = c->obj.surface_zrot;
                                    }
                                }
                                
                                model[0] = c->obj.model;
                                if (
                                    (c->obj.dead != 0x00) 
                                    && (c->obj.die_model[0] != -1)
                                    && (c->obj.die_model[0] != CRemap[c->obj.character])
                                ) 
                                {
                                    model[0] = CModel + c->obj.die_model[0];
                                } else if (!(vflag == 0) && (VEHICLECONTROL == 2)) {
                                    model[0] = CModel + CRemap[115];
                                }
                                
                                if (
                                    !(vflag == 0)
                                    && (
                                        (model[0]->character == 0)
                                        || (model[0]->character == 0x73)
                                    )
                                    && (CRemap[84] != -1)
                                    && (
                                        (SKELETALCRASH != 0)
                                        || (
                                            (c->obj.dead == 0x11) 
                                            && (GameTimer.frame % 0xc < 6)
                                        )
                                    )
                                )
                                {
                                    model[0] = CModel + CRemap[84];
                                }
                                
                                model[1] = NULL;
                                
                                if (
                                    (c->obj.dead != 0x00) 
                                    && (c->obj.die_model[1] != -1)
                                    && (c->obj.die_model[1] != CRemap[c->obj.character])
                                    && (c->obj.die_model[1] == c->obj.die_model[0])
                                )
                                {
                                //     iVar12 = c->obj.die_model[1];
                                }
                                else {
                                    iVar12 = -1;
                                    if (c->obj.character == 0x11) {
                                        iVar12 = 0x12;
                                    }
                                    else if ((c->obj.character == 0x24) && (c->obj.anim.newaction == 0)) {
                                        iVar12 = 0x87;
                                    }
                                    else if (c->obj.character == 0x6d) {
                                        iVar12 = 0x3d;
                                    }
                                    else if (
                                        (c->obj.dead == 0x00) 
                                        && (c->obj.character == 0) 
                                        && (
                                            (c->obj.anim.newaction == 0x25)
                                            || (c->obj.anim.newaction == 0x26)
                                        )
                                    ) {
                                        iVar12 = 0x45;
                                    }
                                    else if (
                                        (c->freeze != 0x00) 
                                        && (c->obj.dead == 0x00)
                                    )
                                    {
                                        iVar12 = 0x4f;
                                    }
                                    else if (
                                        (vflag != 0)
                                        && (c->obj.dead == 0x00)
                                        && (c->target != 0x00)
                                        && (c->obj.character == 0)
                                        && (VEHICLE == -1)
                                    ) {
                                        iVar12 = 0x8c;
                                    }
                                    else if ((VEHICLE != -1) && (vtog_time == vtog_duration)) {
                                        if (VEHICLE == 0x6b) {
                                            iVar12 = 0x6b;
                                        }
                                        else if (VEHICLE == 0xa0) {
                                            iVar12 = 0xa0;
                                        }
                                        else if (VEHICLE == 0x44) {
                                            iVar12 = 0x44;
                                        }
                                        else if (VEHICLE == 0xb2) {
                                            iVar12 = 0xb2;
                                        }
                                        else if (VEHICLE == 0x3b) {
                                            iVar12 = 0x3b;
                                        }
                                        else if (VEHICLE == 0x20) {
                                            iVar12 = 0x20;
                                        }
                                        else if (VEHICLE == 0x89) {
                                            iVar12 = 0x89;
                                        }
                                        else if (VEHICLE == 0xA1) {
                                            iVar12 = 0xA1;
                                        }
                                        else if (VEHICLE == 0x99) {
                                            iVar12 = 0x99;
                                        }
                                    } 
                                    else {
                                        if ((vflag == 0) && (c->obj.vehicle == 0xA1)) {
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
                        
                                dx = 1.2987013f;
                                // iVar26 = 0;
                                // iVar12 = 0;
                                for(j = 0; j < 2; j++, c++) {
                                    if (model[j] == 0) {
                                        continue;
                                    }
                                    
                                    if (
                                        (c->obj.anim.blend == 0x00)
                                        || (
                                            (model[j]->character != 0x45)
                                            && (model[j]->character != 0x8c)
                                            && (model[j]->character != 0xa0)
                                            && (model[j]->character != 0x6b)
                                        )
                                        ||
                                    ((model[j]->anmdata[c->obj.anim.blend_src_action] != 0) &&
                                    (model[j]->anmdata[c->obj.anim.blend_dst_action] != 0))
                                    ) 
                                    {
                                        // bVar28 = bVar11 << 1;
                                        if ((Level == 0x1c) && (j == 1) && (model[0]->character == 0x7f)) {
                                            s.x = s.y = s.z = dx;
                                            NuMtxPreScale(&mC, &s);
                                            if (bVar10) {
                                                NuMtxPreScale(&mS, &s);
                                            }
                                            if (bVar11) {
                                                NuMtxPreScale(&mR, &s);
                                            }
                                        }
                                        if (vflag != 0) {
                                            plr_render = 1;
                                        }
                                        if ((render != 0) && (model[j]->character == 0xAF || model[j]->character == 0xB0))
                                        {
                                            SetLevelLights();
                                        }
                                        
                                        if (model[j]->character == 0x99) {
                                            jeep_draw = 1;
                                        }
                                        
                                        DrawCharacterModel(
                                            model[j],
                                            &c->obj.anim,
                                            &mC,
                                            ((bVar10 != 0)  && (vflag == 0)  || (j == 0) 
                                            && (model[1] != NULL)
                                            && (model[1]->character != 0x44) 
                                            && (model[1]->character != 0xb2) 
                                            && (model[1]->character != 0x99)    
                                            && (model[1]->character != 99)) ? &mS : NULL,
                                            render,
                                            (bVar11 != 0) ? &mR : NULL,
                                            &c->mtxLOCATOR[0][j],
                                            &c->momLOCATOR[0][j],
                                            &c->obj
                                        );
                                    }
                                    // iVar26 = iVar26 + 1;
                                    // c++;
                                    // iVar12 = iVar12 + 4;
                                }
                                
                                // do {
                                    
                                // } while (iVar26 < 2);
                                if (
                                    (render != 0)
                                    && (c->obj.character == 0x76)
                                    && (
                                        ((LDATA->flags & 0x200) != 0)
                                        || (Level == 0x1d)
                                    )
                                    && (ObjTab[66].obj.special != NULL)
                                ) {
                                    NuRndrGScnObj((ObjTab[66].obj.scene)->gobjs[(ObjTab[66].obj.special)->instance->objid], &mC);
                                }
                            }
                            c->obj.draw_frame = old_frame + 1;
                        }
                    }
                }
            }
        }
        LAB_8001f958:
        //i = i + 1;
        c->anim_processed = 1;
        // c = c + 1;
    } //while (i < (s32)local_d0._00);
  //}
    glass_phase = 0;
    glass_draw = 0;
    if (render != 0) {
        SetLevelLights();
    }
}

//83% NGC
void MovePlayer(struct creature_s *plr,struct nupad_s *pad) {
  struct MoveInfo *minfo;
  u16 gotlist[9];
  u16 sVartmp;
  u16 uVar12;
  u16 uVar13;
  short sVar16;
  s32 bVar1;
  s32 uVar4;
  //char cVar8;
  s32 iVar4;
  s32 iVar11;
  s32 cVar14;
  s32 uVar17;
  //s32 *piVar18;
  s32 unaff_r19;
  s32 sVar19;
  s32 iVar20;
  s32 bVar21;
  struct nuvec_s local_f0;
  struct nuvec_s local_e0;
  struct nuvec_s local_d0;
  struct nuvec_s local_c0;
  struct nuvec_s local_b0;
  struct nuvec_s local_a0;
  float fVar2;
  float fVar15;
  float fVar6;
  float dVar24;
  float dVar25;
  float dVar26;
  float dVar27;
  float dVar28;
  float fVar29;
  struct CharacterModel *model;

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
      //uVar17 = plr->rumble.frame;
      if (plr->rumble.frame == 0) {
        uVar17 = 0;
      }
      else {
        uVar17 = (plr->rumble.power * plr->rumble.frame) / plr->rumble.frames;
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
  if ((((Level == 0x16) && (boss_dead == 1)) && (plr->obj.pos.z < 49.0f)) &&
     (plr->obj.oldpos.z >= 49.0f)) {
    plr->obj.pos.x = plr->obj.oldpos.x;
    plr->obj.pos.z = plr->obj.oldpos.z;
    plr->obj.mom.z = 0.05f;
  }
  iVar4 = VEHICLECONTROL;
  if (plr->obj.vehicle != -1) {
    if ((((LBIT & 0x00000001105252801) == 0) && (Level != 3)) &&
              ((Level != 0x1d || (Level == 0x1c)))) {
        iVar4 = 1;
    }
    else if (((LBIT & 0x0000000400000040) != 0) && (iVar4 == 0)) {
          VEHICLECONTROL = 1;
    }
  }
  else {
    VEHICLECONTROL = 0;
  }
  if ((VEHICLECONTROL == 1) && (vtog_time == vtog_duration)) {
    plr->obj.vehicle_frame = plr->obj.vehicle_frame + 1;
  }
  else {
    plr->obj.vehicle_frame = 0;
    plr_vehicle_time = 0.0f;
    plr_vehicle_speedmul = 1.0f;
  }
  ToggleVehicle(plr);
  sVar19 = -1;
  if ((VEHICLECONTROL == 1) && (plr->obj.vehicle != -1)) {
    sVar19 = plr->obj.vehicle;
  }
  if (sVar19 == 0x6b) {
    minfo = &ScooterMoveInfo;
  }
  else if (sVar19 == 0xa0) {
    minfo = &SnowBoardMoveInfo;
  }
  else if (sVar19 == 0x44) {
    minfo = &MechMoveInfo;
  }
  else if (sVar19 == 0xb2) {
    minfo = &FireEngineMoveInfo;
  }
  else if (sVar19 == 0x3b) {
    minfo = &GyroMoveInfo;
  }
  else if (sVar19 == 0x20) {
    minfo = &SubmarineMoveInfo;
  }
  else if (sVar19 == 0x89) {
    minfo = &MineCartMoveInfo;
  }
  else if (sVar19 == 0xa1) {
    minfo = &MineTubMoveInfo;
  }
  else if (sVar19 == 0x99) {
    minfo = &OffRoaderMoveInfo;
  }
  else if (VEHICLECONTROL == 2) {
    minfo = &SwimmingMoveInfo;
  }
  else {
    minfo = plr->OnFootMoveInfo;
  }
  if ((VEHICLECONTROL == 1) && (iVar4 != 1)) {
    plr_vehicle_time = 0.0f;
    if (sVar19 == 0x89) {
      VEHICLETIME = 60.0f;
    }
    else if (sVar19 == 99) {
      ResetJeep(plr);
    }
    ResetTubs();
  }
  if (plr->obj.invincible != 0) {
    plr->obj.invincible += 0xff;
  }
  if (plr->freeze != 0) {
    plr->freeze += 0xff;
  }
  //fVar15 = plr->obj.pos.y;
  //fVar6 = plr->obj.pos.z;
  plr->obj.oldpos = plr->obj.pos;
  plr->obj.old_ground = plr->obj.ground;
  cVar14 = plr->obj.dangle;
  GetTopBot(plr);
  OldTopBot(&plr->obj);
  if (GameMode != 1) {
    if (vtog_time < vtog_duration) {
      vtog_time += 0.016666668f;
      if ((vtog_time >= vtog_duration * 0.333f) && (vtog_time < vtog_duration * 0.333f)) {
        gamesfx_effect_volume = 0x2fff;
        GameSfx(0x2f,&plr->obj.pos);
      }
      if ((vtog_time >= vtog_duration) && (vtog_time = vtog_duration, VEHICLECONTROL == 1)) {
        iVar4 = 0x48;
        if (sVar19 == 0xa0) {
          iVar4 = 0x54;
        }
        if (iVar4 != -1) {
          GameSfx(iVar4,&plr->obj.pos);
        }
        NewRumble(&plr->rumble,0xbf);
        NewBuzz(&plr->rumble,0xc);
      }
      uVar12 = vtog_angle;
      if (vtog_blend != 0) {
        dVar27 = (vtog_time / vtog_duration);
        plr->obj.pos.x = (vtog_newpos.x - vtog_oldpos.x) * dVar27 + vtog_oldpos.x;
        plr->obj.pos.y = (vtog_newpos.y - vtog_oldpos.y) * dVar27 + vtog_oldpos.y;
        plr->obj.pos.z = (vtog_newpos.z - vtog_oldpos.z) * dVar27 + vtog_oldpos.z;
        //fVar15 = v000.z;
        //fVar6 = v000.y;
        plr->obj.mom = v000;
        plr->obj.thdg = vtog_angle;
        //uVar12 = SeekRot(plr->obj.hdg,uVar12,3);
        plr->obj.hdg = SeekRot(plr->obj.hdg,uVar12,3);
        plr->obj.shadow = NewShadowMaskPlat(&plr->obj.pos,0.0f,-1);
        if (plr->obj.shadow != 2000000.0f) {
          GetSurfaceInfo(&plr->obj,1,plr->obj.shadow);
        }
        if ((LBIT & 0x0000000400000040) == 0) {
          plr->obj.pos.y += NuTrigTable[(s32)(dVar27 * 32768.0f) & 0xffff] +
               NuTrigTable[(s32)(dVar27 * 32768.0f) & 0xffff];
        }
        ObjectRotation(&plr->obj,2,1);
        if ((Level != 0x16) || (vtog_time != vtog_duration)) goto LAB_8001cbd4;
      }
    }
    if (Level == 0x25) {
      if (tumble_time < tumble_duration) {
        tumble_time = tumble_time + 0.016666668f;
        if ((tumble_action == 0x56) && (new_lev_flags != 0)) {
          if ((plr->obj.anim.anim_time - (plr->obj.model)->animlist[0x56]->speed * 0.5f < tumble_item_starttime + 1.0f
              ) && (plr->obj.anim.anim_time >= tumble_item_starttime + 1.0f )) {
            for(iVar4 = 0; iVar4 < 9; iVar4++) {
              uVar12 = new_lev_flags & gotlist[iVar4];
              if ((uVar12 != 0) && ((temp_lev_flags & uVar12) == 0)) {
                temp_lev_flags = uVar12 | temp_lev_flags;
                if (AddAward(Hub,last_level,uVar12) == 0) {
                  new_lev_flags = uVar12 ^ (uVar12 | new_lev_flags);
                  Game.level[last_level].flags = uVar12 | Game.level[last_level].flags;
                }
                //piVar18 = &iStack_f6;
                iVar4 = 9;
              }
              //piVar18 = (s32 *)((s32)piVar18 + 2);
            }
          }
        }
        if (tumble_time >= tumble_duration) {
          if ((new_lev_flags | temp_lev_flags) != temp_lev_flags) {
            tumble_time = tumble_cycleduration;
            plr->obj.anim.anim_time = tumble_item_starttime;
          }
          else {
            tumble_time = tumble_duration;
            plr->jump = 1;
            if (tumble_action == 0x56) {
              plr->jump_type = 0;
              iVar4 = qrand();
              if (iVar4 < 0) {
                iVar4 += 0x7fff;
              }
              plr->jump_subtype = (char)(iVar4 >> 0xf);
            }
            else {
              plr->jump_type = 1;
            }
            if (plr->jump_type == 0) {
              plr->jump_frames = minfo->STARJUMPFRAMES;
            }
            else {
              plr->jump_frames = minfo->JUMPFRAMES0;
            }
            plr->jump_frame = 0;
            plr->somersault = 0;
            plr->obj.anim.anim_time = 1.0f;
            plr->jump_hack = 1;
            plr->land = 0;
            plr->obj.ground = 0;
            AddGameDebris(0x10,&plr->obj.pos);
            AddGameDebris(0x11,&plr->obj.pos);
            GameSfx(0x2f,&plr->obj.pos);              
          }
        }
      }
    if (Level == 0x25) {
        if ((tumble_duration < tumble_time) && (last_hub != -1)) {
          if (tumble_time >= tumble_moveduration) {
            plr->obj.pos.x = tumble_newpos.x;
            plr->obj.pos.y = tumble_newpos.y;
            plr->obj.pos.z = tumble_newpos.z;
          }
          else {
            fVar6 = tumble_time / tumble_moveduration;
            plr->obj.pos.x = (tumble_newpos.x - tumble_oldpos.x) * fVar6 + tumble_oldpos.x;
            plr->obj.pos.y = (tumble_newpos.y - tumble_oldpos.y) * fVar6 + tumble_oldpos.y;
            plr->obj.pos.z = (tumble_newpos.z - tumble_oldpos.z) * fVar6 + tumble_oldpos.z;
            plr->obj.shadow = NewShadowMaskPlat(&plr->obj.pos,0.0f,-1);
            if (plr->obj.shadow != 2000000.0f) {
              GetSurfaceInfo(&plr->obj,1,plr->obj.shadow);
            }
          }
          //uVar12 = tumble_hdg;
          plr->obj.ground = 3;
          plr->obj.thdg = tumble_hdg;
          plr->obj.hdg = tumble_hdg;
          plr->obj.old_ground = 3;
          bVar1 = (sVar19 == 0x20) ? 0 : 1;
        }
        else if (Level == 0x25) {
            if ((warp_level != -1)) goto LAB_8001cbd4;
        }
    }
//LAB_80019f04:
      if (((((plr->obj.dead != 0) && (Level != 0x1d)) || ((Cursor.menu != -1 &&
            ((((Cursor.menu != 0x22 && (Cursor.menu != 0x24)) && (Cursor.menu != 0x12)) &&
             ((Cursor.menu != 0x10 || (Level != 0x1d)))))))) || (Cursor.wait != 0)) ||
         ((((plr->obj.finished != 0 && (sVar19 != 99)) &&
           ((sVar19 != 0xa1 && (((sVar19 != 0x36 && (sVar19 != 0x8b)) && (sVar19 != 0x81)))))) &&
          (sVar19 != 0x3b)))) {
        if (plr->obj.dead == 10) {
          plr->obj.pos.y += (plr->obj.layer_shadow - plr->obj.pos.y) * 0.04f;
        }
        else if ((sVar19 != 1) && (((plr->obj.dead == 6 || (plr->obj.dead == 0x10)) ||
                 ((((plr->obj.dead == 0x12 || ((plr->obj.dead == 0x13 || (plr->obj.dead == '\r')))) ||
                   (sVar19 == 0x6b)) || ((sVar19 == 0xa0 || (sVar19 == 0x99)))))))) {
          plr->obj.shadow = NewShadowMaskPlat(&plr->obj.pos,0.0f,-1);
          //dVar27 = 2000000.0f;
          if (plr->obj.shadow != 2000000.0f) {
            local_b0.x = plr->obj.pos.x;
            local_b0.y = (plr->obj.bot + plr->obj.top) * plr->obj.SCALE * 0.5f +
                         plr->obj.pos.y;
            local_b0.z = plr->obj.pos.z;
            fVar6 = CrateTopBelow(&local_b0);
            if ((fVar6 != 2000000.0f) && (plr->obj.shadow > fVar6)) {
              plr->obj.shadow = fVar6;
            }
            if (plr->obj.dead != 0x13) {
              fVar15 = plr->obj.mom.y + GRAVITY;
              fVar29 = plr->obj.pos.y + fVar15;
              fVar2 = plr->obj.SCALE * plr->obj.bot;
              plr->obj.mom.y = plr->obj.mom.y + GRAVITY;
              plr->obj.pos.y = plr->obj.pos.y + fVar15;
              if (fVar29 + fVar2 < plr->obj.shadow) {
                plr->obj.pos.y = plr->obj.shadow - fVar2;
              }
            }
            else {
              plr->obj.pos.y = plr->obj.shadow - plr->obj.bot * plr->obj.SCALE;
            }
          }
        }
        goto LAB_8001cbd4;
      }
      if ((Level != 0x1d) ||
         ((plr->obj.dead == 0 && ((plr->obj.finished == 0 || (in_finish_range != 0x3c)))))) {
        dVar27 = 0.0f;
        local_f0.y = 0.0f;
        if ((GameTimer.frame < 0x3c) || ((((plr->obj.finished != 0 || (plr->freeze != 0)) ||
             ((Level == 0x16 && (FireBossHoldPlayer != 0)))) ||
            (((Cursor.menu == 0x22 || (Cursor.menu == 0x24)) || (fadeval > 0)))))) {
          dVar27 = 0.0f;
          plr->obj.pad_angle = 0;
          plr->pad_type = 1;
          local_f0.x = 0.0f;
          local_f0.z = 0.0f;
          plr->obj.pad_speed = 0.0f;
          plr->obj.pad_dx = 0.0f;
          plr->obj.pad_dz = 0.0f;
        }
        else {
          if ((pad->paddata & 0xf000) != 0) {
            if ((pad->paddata & 0x8000) != 0) {
              dVar28 = -127.5f;
            }
            else {
              if ((pad->paddata & 0x2000) != 0) {
                dVar28 = 127.5f;
              } 
            }
            if ((pad->paddata & 0x4000) != 0) {
              dVar26 = -127.5f;
            }
            else {
              if ((pad->paddata & 0x1000) != 0) {
                dVar26 = 127.5f;
              }
              else {
                dVar26 = 0.0f;
              }
            }
            //dVar28 = dVar27;
            if ((dVar28 != 0.0f) || (dVar26 != 0.0f)) {
              //cVar8 = 1;
              plr->pad_type = 1;
            }
          }
          else {
            dVar28 = ((float)pad->l_alg_x - 127.5f);
            dVar26 = -((float)pad->l_alg_y - 127.5f);
            if ((dVar28 * dVar28 + (dVar26 * dVar26)) < 1806.25f) {
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
              //cVar8 = 2;
              plr->pad_type = 2;
            }
          }
          local_f0.z = (dVar26 * 0.007f);
          local_f0.x = (dVar28 * 0.007f);
          NuVecMag(&local_f0);
          iVar4 = NuAtan2D(local_f0.x,local_f0.z);
          local_e0.y = 0.0f;
          local_e0.x = 0.0f;
          //dVar27 = NuFabs(local_f0.x);
          //dVar26 = NuFabs(local_f0.z);
          if (NuFabs(local_f0.x) > NuFabs(local_f0.z)) {
            local_e0.z = NuFabs(local_f0.x);
          }
          else {
            local_e0.z = NuFabs(local_f0.z);
          }
          //local_e0.z = dVar27;
          NuVecRotateY(&local_f0,&local_e0,iVar4);
          fVar6 = NuVecMag(&local_f0);
          if (fVar6 < 0.2f) {
            plr->obj.pad_speed = 0.0f;
          } else if (fVar6 < 0.6f) {
              plr->obj.pad_speed = minfo->RUNSPEED;
          } else {
              plr->obj.pad_speed = minfo->WALKSPEED;
          }
          //plr->obj.pad_speed = dVar27;
          plr->obj.pad_dx = local_f0.x;
          plr->obj.pad_dz = local_f0.z;
          //iVar4 = NuAtan2D(local_f0.x,local_f0.z);
          plr->obj.pad_angle = (u16)NuAtan2D(local_f0.x,local_f0.z);
        }
        if (((((((VEHICLECONTROL == 1) && (plr->obj.vehicle != 0x3b)) &&
               (plr->obj.vehicle != 0x20)) && ((plr->obj.vehicle != 0x6b && (plr->obj.vehicle != 0xa0)))) && (plr->obj.vehicle != 0x44)) &&
            ((plr->obj.vehicle != 0xb2 && (plr->obj.vehicle != 0x89)))) && ((plr->obj.vehicle != 0xa1 && (plr->obj.vehicle != 0x99)))) {
          plr->obj.boing = 0;
          MoveVehicle(plr,pad);
          fVar6 = plr->obj.pos.x - plr->obj.oldpos.x;
          dVar28 = (fVar6 * fVar6);
          fVar6 = plr->obj.pos.z - plr->obj.oldpos.z;
          dVar26 = (fVar6 * fVar6);
          dVar27 = (plr->obj.pos.y - plr->obj.oldpos.y);
          plr->obj.xz_distance = NuFsqrt((dVar28 + dVar26));
          plr->obj.xyz_distance = NuFsqrt((dVar27 * dVar27 + dVar28) + dVar26);
          //plr->obj.xyz_distance = fVar6;
          goto LAB_8001cbd4;
        }
        if ((((sVar19 == 0x6b) || (sVar19 == 0xa0)) || (sVar19 == 0x99)) ||
           ((sVar19 == 0xa1 || (sVar19 == 0xb2)))) {
          bVar21 = 0;
          if ((Level == 3) && (0 < SmokeyCountDownValue)) {
            bVar21 = 1;
          }
          bVar1 = 0;
          if (plr->obj.ground != 0) {
            bVar1 = plr->obj.surface_type == 10;
          }
          if (Level == 3) {
            dVar27 = (((plr->fire != 0) ? SMOKEYBOOSTSPEED : SMOKEYSPEED) * 0.016666668f * offroader_speedtime);
            if (bVar1) {
              dVar27 *= 0.25;
            }
          }
          else if (Level == 0x16) {
            plr->sprint = 0;
            if (FireBossHoldPlayer != 0) {
              dVar27 = 0.0f;
            }
            else if ((pad->paddata & 0x80) != 0) {
                dVar27 = minfo->SPRINTSPEED;
                //cVar8 = 1;
                plr->sprint = 1;
            } else {
                dVar27 = minfo->RUNSPEED;
            }
          }
          else {
            if (((pad->paddata & 0x88) != 0) || ((sVar19 == 0x99 && ((pad->paddata & 0x40) != 0))) )
            {
              dVar27 = minfo->SPRINTSPEED;
              plr->sprint = 1;
            } else {
              dVar27 = minfo->RUNSPEED;
              plr->sprint = 0;
            }
          }
          if (best_cRPos != NULL) {
            plr->obj.thdg = best_cRPos->angle;
          }
          //plr->obj.thdg = sVartmp;
          iVar4 = 3;
          if ((best_cRPos != NULL) && (((best_cRPos->mode & 3) != 0 || (Level == 9)))) {
            if (Level == 3) {
              uVar17 = 0x2aab;
              iVar4 = 5;
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
            sVar16 = (s32)((float)(uVar17) * plr->obj.pad_dx);
            if (bVar21 == 0) {
              if (((best_cRPos->mode & 1) == 0) || (Level != 9)) {
                plr->obj.thdg += sVar16;
              }
              else {
                plr->obj.thdg -= sVar16;
              }
            }
          }
          uVar12 = plr->obj.hdg;
          if ((bVar21 == 0) && (iVar4 != 0)) {
            //uVar13 = SeekRot(plr->obj.hdg,plr->obj.thdg,iVar4);
            plr->obj.hdg = SeekRot(plr->obj.hdg,plr->obj.thdg,iVar4);
          } else {
              //iVar4 = RotDiff(plr->obj.hdg,plr->obj.hdg);
              plr->obj.dyrot = RotDiff(plr->obj.hdg,plr->obj.hdg);
          }
          if (bVar21) {
            uVar17 = plr->obj.hdg;
            local_c0.x = NuTrigTable[uVar17] * dVar27;
            local_c0.z = NuTrigTable[(uVar17 + 0x4000) & 0x2ffff] * dVar27;
          }
          plr->obj.dangle = 0;
          if (bVar21 == 0) {
            if (Level == 3) {
              fVar6 = (OFFROADERSEEK - 0.25f) * (plr->obj.xz_distance / 0.05f) + 0.25f;
              if (fVar6 < OFFROADERSEEK) {
                fVar6 = OFFROADERSEEK;
              }
            }
            else {
              fVar6 = 0.25f;
            }
            //fVar15 = plr->obj.mom.x;
            //fVar29 = plr->obj.mom.z;
            plr->obj.mom.x += (local_c0.x - plr->obj.mom.x) * fVar6;
            plr->obj.mom.z += (local_c0.z - plr->obj.mom.z) * fVar6;
          }
          else {
            plr->obj.mom.x = plr->obj.mom.z = 0.0f;
          }
          //fVar6 = plr->obj.mom.y;
          if (plr->obj.mom.y < -TERMINALVELOCITY) {
            plr->obj.mom.y = -TERMINALVELOCITY;
          } else if (plr->obj.mom.y > TERMINALVELOCITY) {
              plr->obj.mom.y = TERMINALVELOCITY;
          }
          if (best_cRPos != NULL) {
            uVar17 = RotDiff(best_cRPos->angle,plr->obj.hdg);
            iVar4 =  (uVar17 >= 0 ? uVar17 : -uVar17);
            if (iVar4 < 0x2aab) {
              plr->obj.direction = 0;
            }
            else {
              if (iVar4 < 0x5555) {
                bVar1 = sVar19 == 0x20 ? 0 : 1;
                plr->obj.direction = 2;
                goto LAB_8001bb80;
              }
              plr->obj.direction = 1;
            }
            bVar1 = sVar19 == 0x20 ? 0 : 1;
            //plr->obj.direction = cVar8;
          }
          else {
            bVar1 = sVar19 == 0x20 ? 0 : 1;
            plr->obj.direction = 0;
          }
        }
        else {
          //bVar22 = (sVar19 == 0x3b);
          if ((sVar19 == 0x3b) && (best_cRPos != NULL)) {
            if ((Level == 0x1d) && (GameTimer.frame < 0xb4)) {
              local_b0 = v000;
              dVar27 = 0.333f;
              plr->obj.direction = 0;
            }
            else {
              if ((plr->obj.pad_speed > 0.0f) && (plr->tap == 0)) {
                local_b0.x = plr->obj.pad_dx * minfo->WALKSPEED;
                local_b0.z = 0.0f;
                local_b0.y = -plr->obj.pad_dz * minfo->WALKSPEED;
                NuVecRotateY(&local_b0,&local_b0,best_railangle);
                dVar27 = 1.0f;
              }
              else {
                dVar27 = 0.333f;
                local_b0 = v000;
              }
              //dVar27 = fVar6;
              if ((pad->paddata_db & 0x60) != 0) {
                NewRumble(&player->rumble,0x9f);
              }
              else {
                if (((pad->paddata & 0x60) != 0) && (qrand() < 0x4000)) {
                  iVar4 = qrand();
                  if (iVar4 < 0) {
                    iVar4 += 0x1ff;
                  }
                  NewRumble(&plr->rumble,iVar4 >> 9);
                } 
              }
              uVar17 = pad->paddata & 0x60;
              if (plr->tap == 0) {
                if (uVar17 == 0x40) {
                  if (plr->obj.direction != 0) {
                    plr->obj.direction = 0;
                    plr->tap = 0x1e;
                  }
                  else {
                    local_b0.x = NuTrigTable[best_railangle] * minfo->RUNSPEED + local_b0.x;
                    dVar27 = 1.0f;
                    local_b0.z = NuTrigTable[(best_railangle + 0x4000) & 0x2ffff] * minfo->RUNSPEED + local_b0.z;
                      
                  }
                }
                else if (uVar17 == 0x20) {
                  if (plr->obj.direction != 1) {
                    plr->obj.direction = 1;
                    plr->tap = 0x1e;
                  }
                  else {
                    local_b0.x = local_b0.x - NuTrigTable[best_railangle] * minfo->RUNSPEED;
                    dVar27 = 1.0f;
                    local_b0.z -= NuTrigTable[(best_railangle + 0x4000) & 0x2ffff] * minfo->RUNSPEED;
                  }
                }
              }
            }
            //uVar12 = best_railangle;
            fVar6 = dVar27 * 0.016666668f;
            fVar15 = plr->obj.mom.x;
            fVar29 = plr->obj.mom.y;
            fVar2 = plr->obj.mom.z;
            plr->obj.thdg = best_railangle;
            plr->obj.mom.z = (local_b0.z - fVar2) * fVar6 + fVar2;
            plr->obj.mom.x = (local_b0.x - fVar15) * fVar6 + fVar15;
            plr->obj.mom.y = (local_b0.y - fVar29) * fVar6 + fVar29;
            if (plr->obj.direction == 1) {
              plr->obj.thdg += 0x8000;
            }
            iVar4 = RotDiff(plr->obj.hdg,plr->obj.thdg);
            if ((plr->tap != 0) && (iVar4 < 0)) {
              iVar4 += 0x10000;
            }
            bVar1 = 0;
            plr->obj.hdg = plr->obj.hdg + (short)(iVar4 >> 4);
          }
          else {
            if (plr->target == 0) {
              NuVecRotateY(&local_f0,&local_f0,GameCam[0].yrot);
            }
            sVartmp = plr->obj.hdg;
            plr->fire_lock = 0;
            iVar4 = plr_target_found;
            plr_target_found = 0;
            iVar20 = (s32)plr->target;
            if (plr->target == 0) {
                plr_target_found = plr->target;
            }
            if (plr->target != 0) {
              if (plr->fire == 0) {
                if (sVar19 == 0x44) {
                  if (plr->obj.pad_speed > 0.0f) {
                    iVar20 = (s32)(-plr->obj.pad_dz * 5461.0f);
                    iVar11 = (s32)(plr->obj.pad_dx * 10923.0f);
                  }
                  else {
                    iVar11 = (s32)plr->obj.target_yrot;
                    iVar20 = (s32)plr->obj.target_xrot;
                  }
                  plr->obj.target_xrot += (short)(iVar20 - plr->obj.target_xrot >> 5);
                  plr->obj.target_yrot = plr->obj.target_yrot + (short)(iVar11 - plr->obj.target_yrot >> 5);
                  plr_target_firepos = plr_target_pos[0] = *(struct nuvec_s*)&plr->mtxLOCATOR[8][0]._30;
                  NuVecRotateX(&plr_target_dir,&v001,-plr->obj.target_xrot & 0xffff);
                  NuVecRotateY(&plr_target_dir,&plr_target_dir,
                               plr->obj.hdg + plr->obj.target_yrot);
                  fVar6 = MECHTARGETHACK;
                }
                else {
                  iVar20 = (s32)plr->obj.target_yrot +
                           (s32)(plr->obj.pad_dx * 16384.0f * 0.016666668f);
                  if (iVar20 < -0x3555) {
                    iVar11 = -0x3555 - iVar20;
                    iVar20 = -0x3555;
//LAB_8001ad90:
                    plr->obj.hdg -= (short)(iVar11 / 2);
                  }
                  else if (0x3555 < iVar20) {
                    iVar11 = 0x3555 - iVar20;
                    iVar20 = 0x3555;
                    plr->obj.hdg -= (short)(iVar11 / 2);
                  }
                  plr->obj.target_yrot = (short)iVar20;
                  iVar20 = (s32)plr->obj.target_xrot -
                           (s32)(plr->obj.pad_dz * 16384.0f * 0.016666668f);
                  if (iVar20 < -0x1555) {
                    iVar20 = -0x1555;
                  }
                  else if (iVar20 > 0x2aab) {
                    iVar20 = 0x2aab;
                  }
                  plr->obj.target_xrot = (short)iVar20;
                  plr_target_firepos = plr_target_pos[0] = *(struct nuvec_s*)&plr->mtxLOCATOR[8][0]._30;
                  NuVecRotateX(&plr_target_dir,&v001,-plr->obj.target_xrot & 0xffff);
                  NuVecRotateY(&plr_target_dir,&plr_target_dir,
                               plr->obj.hdg + plr->obj.target_yrot);
                  fVar6 = BAZOOKATARGETHACK;
                }
                plr_target_pos[0].x -= plr_target_dir.x * fVar6;
                plr_target_pos[0].y -= plr_target_dir.y * fVar6;
                plr_target_pos[0].z -= plr_target_dir.z * fVar6;
                plr_target_sightpos = *(struct nuvec_s*)&plr->mtxLOCATOR[8][1]._30;
              }
              GameRayCast(plr_target_pos,&plr_target_dir,10.0f,plr_target_pos + 1);
              NuMtxSetRotationX(&plr_target_mtx,-plr->obj.target_xrot);
              NuMtxRotateY(&plr_target_mtx,plr->obj.hdg + plr->obj.target_yrot);
              NuVecSub(&local_e0,plr_target_pos + 1,&plr_target_firepos);
              NuVecNorm(&local_e0,&local_e0);
              if (NuVecDot(&local_e0,&plr_target_dir) <= 0.0f) {
                plr->fire_lock = 1;
              }
              if (((plr->fire_lock == 0) && (iVar4 == 0)) && (plr_target_found != 0)) {
                GameSfx(5,&plr->obj.pos);
              }
              GameSfxLoop(3,NULL);
              plr_target_frame++;
            }
            else {
              if (sVar19 == 0x20) {
                if ((plr->obj.pad_speed > 0.0f) && (plr->obj.hdg == plr->obj.thdg)) {
                  uVar17 = (s32)(16384.0f - NuTrigTable[plr->obj.pad_angle] * 16384.0f);
                  uVar4 = uVar17;
                  uVar12 = (u16)uVar4;
                  if (plr->obj.hdg == 0) {
                    plr->obj.hdg = uVar12;
                    if (uVar4 > 0x4aab) {
                      //uVar13 = 0x4000;
//LAB_8001b07c:
                      plr->obj.thdg = 0x4000;
                      plr->obj.hdg = uVar12;
                    }
                  }
                  else {
                    uVar13 = 0x8000;
                    if (plr->obj.hdg == 0x8000) {
                      plr->obj.hdg = uVar12;
                      if (uVar4 < 0x3555) {
                        plr->obj.thdg = 0x4000;
                      }
                    }
                    else if (uVar4 < 0xaab) {
                      plr->obj.thdg = 0;
                      plr->obj.hdg = uVar12;
                    }
                    else {
                      plr->obj.hdg = uVar12;
                      if (0x7555 < uVar4) plr->obj.thdg = 0x4000;
                    }
                  }
                }
                uVar12 = plr->obj.hdg;
                uVar13 = plr->obj.thdg;
                uVar17 = uVar12;
                uVar4 = uVar13;
                if (uVar17 < uVar4) {
                  if ((s32)(uVar4 - uVar17) < 0x2d8) {
                    plr->obj.hdg = uVar13;
                  }
                  else {
                    plr->obj.hdg += 0x2d8;
                  }
                }
                else if (uVar4 < uVar17) {
                  if ((s32)(uVar17 - uVar4) < 0x2d8) {
                    plr->obj.hdg = uVar13;
                  } else {
                    plr->obj.hdg -= 0x2d8;
                  }
                }
              }
              else if (VEHICLECONTROL == 2) {
                if (plr->obj.pad_speed > 0.0f) {
                  uVar17 = plr->obj.pad_angle;
                  if (uVar17 - 0x1555 < 0x5557) {
                    plr->obj.thdg = 0;
                  }
                  else if ((uVar17 + 0x6aab & 0xffff) < 0x5557) {
                    plr->obj.thdg = 0x8000;
                  }
                }
                iVar4 = RotDiff(plr->obj.hdg,plr->obj.thdg);
                if (iVar4 > 0) {
                  iVar4 -= 0x10000;
                }
                if (iVar4 < -0x2d8) {
                  plr->obj.hdg -= 0x2d8;
                }
                else {
                  plr->obj.hdg = plr->obj.thdg;
                }
              }
              else if (plr_rebound != 0) {
                plr->obj.hdg = plr->obj.thdg;
                plr->obj.pad_speed = 0.0f;
              }
              else {
                if (plr->slide != 0) {
                  uVar12 = plr->obj.thdg;
                  iVar20 = 2;
                  uVar13 = sVartmp;
                }
                else {
                  if (((ExtraMoves != 0) || ((Game.powerbits & 0x20) != 0)) &&
                     ((plr->slam != 0 && ((plr->slam < 3 && (plr->obj.ground == 0)))) ))
                  {
                    plr->obj.hdg = sVartmp - 0xa3d;
                    goto LAB_8001b3f4;
                  }
                  if (Cursor.menu == 0x24) {
                    iVar20 = 3;
                    uVar13 = plr->obj.hdg;
                    plr->obj.thdg = NuAtan2D(GameCam[0].pos.x - plr->obj.pos.x,
                                    GameCam[0].pos.z - plr->obj.pos.z);
                    //uVar12 = (u16)plr->obj.thdg;
                  }
                  else {
                    if (((((plr->slam_wait == 0) && (plr->obj.pad_speed > 0.0f)) &&
                         (plr->obj.dangle != 2)) &&
                        (((plr->obj.dangle != 0 && (plr->spin == 0)) ||
                         ((s32)plr->spin_frame <
                          (s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES)))) &&
                       (((plr->jump != 0 && (plr->jump_hold == 0)) &&
                        ((sVar19 != 0x44 || (plr->tap != 0)))))) goto LAB_8001b3f4;
                    dVar26 = NuFabs(local_f0.x);
                    dVar28 = 0.0f;
                    if ((dVar26 > 0.0f) || (NuFabs(local_f0.z) > dVar28)) {
                      plr->obj.thdg = NuAtan2D(local_f0.x,local_f0.z);
                    }
                    if (((((plr->slam == 0) || (plr->slam > 2)) || (plr->obj.ground != 0)) &&
                        (((ExtraMoves == 0 && ((Game.powerbits & 4) == 0)) ||
                         ((plr->spin == 0 || (s32)plr->spin_frame >= 
                        (s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES
                           ))))) && (plr->target != 0)) {
                      iVar20 = 5;
                    }
                    else {
                      if ((plr->crawl != 0) || (((bVar1 || (!bVar21)) &&
                          ((plr->jump != 0 &&
                           ((plr->jump_type == 2 && (plr->jump_hold == 0)))))))) {
                        iVar20 = 4;
                      }
                      else if (((plr->tiptoe != 0) || (bVar1 == 0)) || (!bVar21)) {
                        iVar20 = 3;
                      }
                      else {
                        iVar20 = 2;
                      }
                    }
                    uVar13 = plr->obj.hdg;
                    uVar12 = plr->obj.thdg;
                  } 
                }
                uVar12 = SeekRot(uVar13,uVar12,iVar20);
                plr->obj.hdg = uVar12;
              }
            }
LAB_8001b3f4:
            //bVar1 = sVar19 == 0x20;
            //iVar4 = RotDiff(iVar4,plr->obj.hdg);
            plr->obj.dyrot = (short)RotDiff(iVar4,plr->obj.hdg);
            if (sVar19 != 0x20) {
              if (((VEHICLECONTROL == 2) && (plr->spin != 0)) &&
                 !((s32)plr->spin_frame <
                  (s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES)) {
                  if (plr_rebound != 0) {
                    dVar27 = minfo->WALKSPEED;
                  }
                  else {
                    if (((plr->slam_wait == 0) && (plr->target == 0)) &&
                       (((plr->jump == 0 || (plr->jump_hold == 0)) &&
                        ((sVar19 != 0x44 ||
                         (plr->tap == 0)))))) {
                      if ((plr->slam == 0) || ((plr->slam > 2 || (plr->obj.ground == 0)))) {
                        dVar27 *= 0.1f;
                      }
                      else {
                        if (plr->obj.dangle != 0) {
                          if ((plr->obj.pad_speed == 0.0f) ||
                             (((plr->spin != 0 &&
                               ((s32)plr->spin_frame <
                                (s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES)) ||
                              (plr->obj.dangle == 2)))) goto LAB_8001b548;
                          dVar27 = minfo->DANGLESPEED;
                        }
                        else {
                          if (plr->slide != 0) {
                            if (plr->obj.character == 1) goto LAB_8001b548;
                            dVar27 = minfo->SLIDESPEED;
                          }
                          else {
                            if (plr->crawl != 0) {
                              if (plr->obj.pad_speed < 0.0f) goto LAB_8001b5f4;
                              dVar27 = minfo->CRAWLSPEED;
                            }
                            else {
                              if (plr->tiptoe != 0) {
                                if (plr->obj.pad_speed < 0.0f) goto LAB_8001b5f4;
                                dVar27 = minfo->TIPTOESPEED;
                              }
                              else {
                                if (plr->obj.wade == 0) {
                                  if (plr->sprint != 0) {
                                    if (plr->obj.pad_speed > 0.0f) goto LAB_8001b5ec;
                                    goto LAB_8001b5f4;
                                  }
                                }
                                else if (plr->obj.pad_speed < 0.0f) {
LAB_8001b5f4:
                                  dVar27 = 0.0f;
                                }
                                else {
                                  dVar27 = minfo->WADESPEED;
                                }
                              }
                            }
                          } 
                        } 
                      }
                    }
                    else {
LAB_8001b548:
                      dVar27 = 0.0f;
                    } 
                  }
              }
              else {
LAB_8001b5ec:
                dVar27 = minfo->SPRINTSPEED;
              }
            }
            in_speed = dVar27;
            in_s_friction = 0.005f;
            in_f_friction = 0.005f;
            if (bVar1 == 0) {
              in_s_friction = 0.001f;
              NuVecRotateX(&local_e0,&v001,-plr->obj.xrot);
              dVar27 = 0.0f;
              if (plr->obj.pad_speed > 0.0f) {
                dVar26 = NuFabs(plr->obj.pad_dx);
                if (dVar26 < 0.333f) {
                  local_e0.z = dVar27;
                }
                else {
                  local_e0.z *= plr->obj.pad_speed;
                }
                if (plr->obj.pad_angle > 0x8000) {
                  local_e0.z = -local_e0.z;
                }
                local_e0.y = local_e0.y * plr->obj.pad_speed;
              }
              else {
                local_e0.y = 0.0f;
                local_e0.z = 0.0f;
              }
              if (plr->obj.mom.z > local_e0.z) {
                plr->obj.mom.z -= in_s_friction;
                if (plr->obj.mom.z < local_e0.z) {
                  plr->obj.mom.z = local_e0.z;
                }
              }
              else if (plr->obj.mom.z < local_e0.z) {
                    plr->obj.mom.z += in_s_friction;
                    if (plr->obj.mom.z > local_e0.z) {
                      plr->obj.mom.z = local_e0.z;
                    }
              }
              fVar6 = plr->obj.mom.y;
              if (plr->obj.mom.y > local_e0.y) {
                plr->obj.mom.y -= in_s_friction;
                if (plr->obj.mom.y < local_e0.y) {
                  plr->obj.mom.y = local_e0.y;
                }
              }
              else if (plr->obj.mom.y < local_e0.y) {
                    plr->obj.mom.y += in_s_friction;
                    if (plr->obj.mom.y > local_e0.y) {
                      plr->obj.mom.y = local_e0.y;
                    }
              }
              if (pos_START != NULL) goto LAB_8001b970;
LAB_8001b980:
              fVar6 = 0.0f;
            }
            else {
              if (VEHICLECONTROL == 2) {
                  if (((plr->spin == 0) || (plr->spin_frame <= plr->spin_frames)) &&
                     (plr->tap == 0)) {
                    in_s_friction *= 0.0005f;
                  }
                  NuVecRotateX(&local_e0,&v001,-plr->obj.xrot);
                  if (plr->obj.thdg == 0x8000) {
                    local_e0.z = -local_e0.z;
                  }
                  //bVar21 = plr->spin != 0;
                  if (((plr->spin != 0) &&
                      ((s32)plr->spin_frame <
                       (s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES)) ||
                     (plr->tap > 5)) {
                    fVar15 = minfo->SPRINTSPEED;
                    fVar6 = minfo->SPRINTSPEED;
                    local_e0.z = local_e0.z * fVar15;
                    local_e0.y = local_e0.y * fVar6;
                  }
                  else {
                    if (((plr->spin != 0) && ((s32)plr->spin_frame >
                         (s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES)) 
                        || ((char)(plr->tap - 1) < 5)) {
                        fVar15 = minfo->RUNSPEED;
                        fVar6 = minfo->RUNSPEED;
                        local_e0.z *= fVar15;
                        local_e0.y *= fVar6;
                    }
                    dVar27 = 0.0f;
                    if (plr->obj.pad_speed > 0.0f) {
                      dVar26 = NuFabs(plr->obj.pad_dx);
                      if (dVar26 < 0.333f) {
                        local_e0.z = dVar27;
                      }
                      else {
                        local_e0.z *= plr->obj.pad_speed;
                      }
                      local_e0.y *= plr->obj.pad_speed;
                    }
                    else {
                      local_e0.y = -0.0041666669f;  
                      local_e0.z = 0.0f;
                    }
                  }
              } else {
                if (plr->slide != 0) {
                  fVar6 = 3.0f;
LAB_8001ba80:
                  in_s_friction = fVar6 * 0.005f;
                  in_f_friction = in_s_friction;
                }
                else {
                  if (plr->freeze != 0) {
                    if (plr->obj.ground == 0) {
                      fVar6 = 0.5f;
                    }
                    else {
                      fVar6 = 0.15f;
                    }
                    in_s_friction = fVar6 * 0.005f;
                    in_f_friction = fVar6 * 0.005f;
                  }
                  else {
              if (plr->obj.mom.z <= local_e0.z) {
                if ((plr->obj.mom.z < local_e0.z) &&
                   (plr->obj.mom.z += in_s_friction, local_e0.z < plr->obj.mom.z))
                goto LAB_8001b91c;
              }
              else {
                plr->obj.mom.z -= in_s_friction;
                if (plr->obj.mom.z < local_e0.z) {
LAB_8001b91c:
                  plr->obj.mom.z = local_e0.z;
                }
              }
              if (plr->obj.mom.y <= local_e0.y) {
                if ((plr->obj.mom.y < local_e0.y) &&
                   (plr->obj.mom.y += in_s_friction, local_e0.y < plr->obj.mom.y))
                goto LAB_8001b960;
              }
              else {
                plr->obj.mom.y -= in_s_friction;
                if (plr->obj.mom.y < local_e0.y) {
LAB_8001b960:
                  plr->obj.mom.y = local_e0.y;
                }
              }
              if (pos_START == NULL) goto LAB_8001b980;
LAB_8001b970:
              fVar6 = pos_START->x - plr->obj.pos.x;            
              }
            }
            plr->obj.mom.x = fVar6;
          }
                    if (plr->obj.ground != 0) {
                      fVar6 = TerSurface[plr->obj.surface_type].friction;
                      fVar15 = TerSurface[plr->obj.surface_type].friction;
                      in_s_friction = fVar15 * 0.005f;
                      in_f_friction = fVar6 * 0.005f;
                    }
                    if (((ExtraMoves != 0) || ((Game.powerbits & 4) != 0)) &&
                       ((plr->spin != 0 &&
                        ((s32)plr->spin_frame <
                         (s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES)))) {
                      fVar6 = 0.3333333f;
                      in_s_friction = fVar15 * 0.005f;
                      in_f_friction = fVar6 * 0.005f;
                    }
                    else if (plr->jump != 0) {
                      if (plr->jump_type == 0) {
                        fVar6 = 0.5f;
                        fVar15 = fVar6;
                      }
                      else {
                        if (((sVar19 != 0x44) && (bVar21)) ||
                           ((plr->jump_type != 2 || (plr->jump_hold != 0))))
                        goto LAB_8001bafc;
                        fVar6 = 0.25f;
                        fVar15 = fVar6;
                      }
LAB_8001baf8:
                      in_s_friction = fVar15 * 0.005f;
                      in_f_friction = fVar6 * 0.005f;
                    }
                  }
                }
LAB_8001bafc:
                if ((sVar19 == 0x3b) && (best_cRPos != NULL)) { //plr->obj.invincible
                  in_s_friction = in_s_friction * 0.1f;
                  in_f_friction = in_f_friction * 0.1f;
                  sVartmp = NuAtan2D(local_f0.x,local_f0.z);
                  //sVartmp = (u16)iVar4;
                  MoveLoopXZ(&plr->obj,gotlist);
                } else {
                  MoveLoopXZ(&plr->obj,&plr->obj.thdg);
                }
                if (plr->obj.mom.y < -TERMINALVELOCITY) {
                  plr->obj.mom.y = -TERMINALVELOCITY;
                }
                else if (plr->obj.mom.y > TERMINALVELOCITY) {
                    plr->obj.mom.y = TERMINALVELOCITY;
                }
                goto LAB_8001bb80;
        }
LAB_8001bb80:
        if (((LIFTPLAYER != 0) && ((pad->paddata & 0x10) != 0)) && (plr->obj.transporting == 0)
           ) {
          //fVar6 = plr->obj.pos.y;
          plr->obj.gndflags.all = 0;
          plr->obj.pos.y += 0.1f;
          plr->obj.mom.y = 0.0f;
          plr->obj.ground = 0;
        }
        if (FRAME == 0) {
          tbslotBegin(app_tbset,7);
        }
        if (plr->obj.transporting == 0) {
          local_d0.x = plr->obj.pos.x;
          local_d0.y = plr->obj.bot * plr->obj.SCALE + plr->obj.pos.y;
          local_d0.z = plr->obj.pos.z;
          if (((plr->obj.ground == 1) && (plr->obj.mom.x == 0.0f)) &&
             ((plr->obj.mom.z == 0.0f && (0.1f < local_d0.y - plr->obj.shadow)))) {
            plr->obj.gndflags.all = 0;
          }
          fVar6 = 0.0f;
          if (plr->obj.pad_speed <= 0.0f) {
            fVar6 = 0.003f;
          }
          fVar15 = plr->obj.RADIUS;
          NewTerrainScaleY(&local_d0,&plr->obj.mom,&plr->obj.gndflags.chrs,
                           (u32)&plr[0x9eb25].lights.dir2.Direction.y / 0xce4,fVar6,fVar15,
                           ((plr->obj.top - plr->obj.bot) * plr->obj.SCALE) / (fVar15 + fVar15));
          fVar6 = plr->obj.SCALE;
          fVar15 = plr->obj.bot;
          plr->obj.pos.x = local_d0.x;
          plr->obj.pos.y = local_d0.y - plr->obj.bot * plr->obj.SCALE;
          plr->obj.pos.z = local_d0.z;
        }
        else {
          NuVecAdd(&plr->obj.pos,&plr->obj.pos,&plr->obj.mom);
        }
        if (FRAME == 0) {
          tbslotEnd(app_tbset,7);
        }
        iVar4 = PlatformCrush();
        switch(iVar4) {
            case 1:
            case 2:
            case 3:
            case 4:
                iVar4--;
            break;
            case 5:
                iVar4 = 0x12;
            break;
            case 6:
                iVar4 = 0x13;
            break;
            case 9:
                iVar4 = 9;
            break;
            case 10:
                iVar4 = 0x11;
            break;
            default:
                iVar4 = GetDieAnim(&plr->obj,-1);
            break;
        }
        KillPlayer(&plr->obj,iVar4);
        if (((plr->obj.mask != NULL) && (2 < plr->obj.mask->active)) &&
           (plr->obj.mask->active = plr->obj.mask->active + -1, (plr->obj.mask)->active == 2)) {
          GameMusic((s32)LDATA->music[0],0);
        }
        plr->obj.transporting = 0;
        BonusTransporter(plr);
        DeathTransporter(plr);
        GemPathTransporter(plr);
        if (plr->obj.dead == 0) {
          if (((plr->obj.transporting == 0) && (GetTopBot(plr), sVar19 != 0x89)) &&
             (sVar19 != 0xa1)) {
            PlayerCreatureCollisions(&plr->obj);
            if (plr->obj.dangle != 0) {
              plr->slam = 3;
            }
            HitItems(&plr->obj);
          }
          if (plr->obj.dead == 0) {
            if (plr->obj.transporting == 0) {
              GetTopBot(plr);
              NewTopBot(&plr->obj);
              if ((Level == 1) &&
                 ((PlayerObjectAnimCollision(&plr->obj,ObjTab[58].obj.special,0.4f) != 0 ||
                  (PlayerObjectAnimCollision(&plr->obj,ObjTab[59].obj.special,0.4f) != 0))))  {
                KillPlayer(&plr->obj,3);
              }
            }
            if ((((plr->obj.dead == 0) && (plr->obj.transporting == 0)) &&
                (GetTopBot(plr), sVar19 != 0x89)) && (sVar19 != 0xa1)) {
              CrateCollisions(&plr->obj);
            }
          }
        }
        if (plr->obj.boing != 0) {
          plr->jump = 1;
          plr->ok_slam = 1;
          plr->somersault = 0;
          plr->land = 0;
          plr->jump_type = (plr->obj.boing & 2) ? 3 : 1;
          plr->jump_frames = minfo->JUMPFRAMES1 + 1;
          plr->jump_frame = 0;
          fVar6 = minfo->JUMPHEIGHT;
          if (plr->jump_type == 3) {
            fVar6 *= 1.5f;
          }
          plr->obj.mom.y = fVar6 / ((float)minfo->JUMPFRAMES2);
          plr->obj.anim.anim_time = 1.0f;
          plr->obj.ground = 0;
        }
        else {
          if ((temp_crate_y_ceiling_adjust != 0) && (plr->jump != 0)) {
            plr->jump = 6;
            plr->jump_frame = plr->jump_frames;
            plr->jump_type = 4;
          } 
        }
        if (((temp_crate_y_floor_adjust != 0) || (temp_crate_y_ceiling_adjust != 0)) ||
           (temp_crate_xz_adjust != 0)) {
          plr->obj.pos_adjusted = 1;
        }
        goto LAB_8001c028;
      }
    }
    else {
LAB_8001c028:
      fVar6 = NewShadowMaskPlatRot(&plr->obj.pos,0.0f,-1);
      if (fVar6 == 2000000.0f) {
        plr_terrain_ok = (s32)plr->obj.transporting;
        if (plr_terrain_ok != 0) goto LAB_8001c0a8;
        if (((NOTERRAINSTOP == 0) || (Level == 8)) || (Level == 6)) goto LAB_8001c0b0;
        //fVar6 = plr->obj.oldpos.z;
        plr->obj.pos.x = plr->obj.oldpos.x;
        plr->obj.pos.z = plr->obj.oldpos.z;
        plr->obj.mom.z = 0.0f;
        plr->obj.dangle = plr->obj.transporting;
        plr->obj.mom.x = 0.0f;
      }
      else {
LAB_8001c0a8:
        plr_terrain_ok = 1;
LAB_8001c0b0:
        unaff_r19 = (s32)plr->obj.layer_type;
        if ((plr->obj.got_shadow == 0) ||
           (((plr->obj.transporting == 0 && (fVar6 != 2000000.0f)) && (fVar6 > plr->obj.shadow)))) {
          plr->obj.got_shadow = 0;
          plr->obj.shadow = fVar6;
          GetSurfaceInfo(&plr->obj,2,fVar6);
        }
        else {
          GetSurfaceInfo(&plr->obj,0,fVar6);
        }
      }
      plr->obj.wade = 0;
      if (plr->obj.layer_type != -1) {
        if (plr->obj.top * plr->obj.SCALE + plr->obj.pos.y < plr->obj.layer_shadow) {
          if (plr->obj.submerged < 0x3c) {
            plr->obj.submerged++;
          }
          else if (((Level != 0x25) && (bVar1 != 0)) && (VEHICLECONTROL != 2)) {
            KillPlayer(&plr->obj,10);
          }
        } else {
          plr->obj.submerged = 0;
        }
        if ((plr->obj.bot + plr->obj.top) * plr->obj.SCALE * 0.5f + plr->obj.pos.y <
            plr->obj.layer_shadow) {
          plr->obj.wade = 1;
        }
        iVar4 = 1;
        local_e0.x = plr->obj.pos.x;
        iVar20 = -1;
        local_e0.y = plr->obj.layer_shadow;
        local_e0.z = plr->obj.pos.z;
        switch(plr->obj.layer_type) {
            case 1:
                //dVar27 = 0.0000015258789f;
                local_a0.x = (float)qrand() * 0.0000015258789f + local_e0.x;
                local_a0.y = ((float)qrand() * 0.0000015258789f + local_e0.y);
                uVar17 = qrand();
                plr->obj.ddr = 0x40;
                plr->obj.ddwater = 0x78;
                local_a0.z = ((float)uVar17 * 0.0000015258789f + local_e0.z);
                plr->obj.ddg = 0x78;
                plr->obj.ddb = -0x80;
                if ((plr->obj.idle_gametime != 0.0f) ? qrand() > 0xfff : qrand() > 0x7fff) {
                  NuRndrAddWaterRipple(&local_a0,0.2f,0.4f,0x20,0x60706050);
                }
            break;
            case 2:
LAB_8001c324:
                iVar20 = 1;
            break;
            case 3:
                goto LAB_8001c324;
            break;
            case 4:
                if (plr->obj.idle_gametime == 0.0f) {
                    iVar20 = 1;
                    iVar4 = 2;
                    local_e0.y = local_e0.y + 0.03f;
                }
            break;
        }
        if (((Paused == 0) && (iVar20 > 0)) && (jonframe1 % iVar4) == 0) {
          AddVariableShotDebrisEffect(GDeb[iVar20].i,&local_e0,1,0,0);
        }
      }
      else {
        plr->obj.submerged = 0;
      }
      if (plr->obj.surface_type > 0) {
        local_e0.x = plr->obj.pos.x;
        iVar4 = 1;
        local_e0.y = plr->obj.pos.y;
        iVar20 = -1;
        local_e0.z = plr->obj.pos.z;
        switch(plr->obj.surface_type) {
            case 8:
              if ((plr->obj.idle_gametime == 0.0f) && (Level != 3)) {
                iVar20 = 4;
                iVar4 = 4;
                local_e0.y += 0.03f;
              }
            break;
            case 1:
            case 12:
            case 13:
                if (plr->obj.idle_gametime == 0.0f) {
                  iVar20 = 2;
                  iVar4 = 2;
                }
            break;
            case 2:
              if (plr->obj.idle_gametime == 0.0f) {
                iVar20 = 1;
                iVar4 = 2;
                local_e0.y += 0.03f;
              }
            break;
            default:
            break;
        }
        if ((((Paused == 0) && (iVar20 > 0)) && (plr->obj.ground != 0)) &&
                (jonframe1 % iVar4) == 0) {
            AddVariableShotDebrisEffect(GDeb[iVar20].i,&local_e0,1,0,0);
        }
      }
      iVar4 = (s32)plr->obj.roof_type;
      if (((iVar4 != -1) && ((TerSurface[iVar4].flags & 0x10) != 0)) && (VEHICLECONTROL != 1)) {
        fVar6 = plr->obj.SCALE;
        fVar15 = plr->obj.roof_y - minfo->DANGLEGAP;
        if ((fVar15 >= plr->obj.top * fVar6 + plr->obj.pos.y) || (cVar14 != 0)) {
          fVar29 = plr->obj.max.y;
          plr->obj.dangle = 1;
          plr->obj.pos.y = fVar15 - fVar29 * fVar6;
          if (cVar14 == 0) {
            GameSfx(0x1b,&plr->obj.pos);
          }
        }
      }
      else {
        plr->obj.dangle = 0;
        if (cVar14 != 0) {
          plr->jump = 6;
          plr->obj.mom.y = 0.0f;
          plr->jump_frame = plr->jump_frames;
          plr->jump_type = 4;
          GameSfx(0x1c,&plr->obj.pos);
        }
      }
      if ((plr->obj.layer_type == 1) && (unaff_r19 == -1)) {
        GameSfx(0x47,&plr->obj.pos);
      }
      else if ((plr->obj.layer_type == -1) && (unaff_r19 == 1)) {
        GameSfx(0x47,&plr->obj.pos);
      }
      TerrainFailsafe(&plr->obj);
      if (plr->obj.got_shadow != 0) {
        bVar21 = plr->obj.bot * plr->obj.SCALE + plr->obj.pos.y <= plr->obj.shadow;
      }
      else {
        bVar21 = (plr->obj.bot * plr->obj.SCALE + plr->obj.pos.y) - plr->obj.shadow < 0.025f;
      }
      plr->obj.ground = 0;
      if (plr->obj.gndflags.chrs != 0) {
        plr->obj.ground = 1;
      }
      if (bVar21) {
        plr->obj.ground |= 2;
      }
      if (plr->obj.ground != 0) {
        plr->obj.last_ground = plr->obj.ground;
      }
      if (((plr->obj.ground & 2U) != 0) && (plr->obj.dead == 0)) {
        if ((((TerSurface[plr->obj.surface_type].flags & 1) == 0) ||
            (((plr->obj.invincible != 0 && (plr->obj.layer_type == -1)) ||
             (sVar19 == 0x20)))) || (VEHICLECONTROL != 2)) {
          if ((VEHICLECONTROL == 1) && (plr->obj.vehicle != -1)) {
            iVar4 = 0xb;
          }
          else {
            if (plr->obj.character == 0) {
              if (((plr->obj.layer_type != -1) && (CRemap[79] != -1)) && (Level == 7)) {
                iVar4 = 8;
              }
              else {
                if ((plr->obj.character == 0) && ((plr->obj.surface_type == 7)) && (CRemap[151] == -1)) goto LAB_8001c7b0;
                iVar4 = 0xd;
              }
            }
            else {
LAB_8001c7b0:
              iVar4 = 5;
              if ((plr->obj.layer_type != -1) && (plr->obj.submerged != 0)) {
                iVar4 = 10;
              }
            }
          }
          bVar21 = 0;
          if (((plr->obj.mask != NULL) && (plr->obj.mask->active != 0)) && ((LDATA->flags & 0xe00) == 0))  {
            bVar21 = plr->obj.mask->active > 2;
          }
          if ((((plr->obj.layer_type != -1) && (plr->obj.submerged == 0)) && (Level == 2)) && (Bonus == 0))  {
              if (((plr->obj.mask != NULL) && (plr->obj.mask->active == 0)) ||
                      ((LDATA->flags & 0xe00) == 0)) {
                if ((plr->obj.mask->active < 3) && (plr->obj.invincible == 0)) {
                    LoseMask(&plr->obj);
                }
              } else {
                    KillGameObject(&plr->obj,iVar4);
              }
          } else {
            plr->obj.invincible = 0;
            KillGameObject(&plr->obj,iVar4);
            if (((plr->obj.mask != NULL) && (plr->obj.mask->active != 0)) &&
               (uVar12 = LDATA->flags, (uVar12 & 0xe00) == 0)) {
              AddMaskFeathers(plr->obj.mask);
              (plr->obj.mask)->active = uVar12 & 0xe00;
            }
          }
          if (((plr->obj.dead != 0) && (bVar21)) && (NuSoundKeyStatus(4) == 1)) {
            NuSoundStopStream(4);
          }
        } else if ((TerSurface[plr->obj.surface_type].flags & 2) != 0) {
            plr->obj.boing = plr->obj.boing | 2;
            GameSfx(2,&plr->obj.pos);
            NewRumble(&player->rumble,0x7f);
            NewBuzz(&player->rumble,0xc);
        }
      }
      ObjectRotation(&plr->obj,2,(sVar19 == -1) ? 0 : 1);
      if (((plr->obj.transporting == 0) && (sVar19 != 0x89)) && (sVar19 != 0xa1)) {
        WumpaCollisions(&plr->obj);
      }
      plr_allow_jump = (s32)plr->allow_jump;
      if (plr->obj.ground != 0) {
        plr->allow_jump = 0xc;
      } else if (plr->allow_jump != 0) {
          plr->allow_jump--;
      }
      if (plr->obj.boing != 0) {
        plr->ok_slam = 1;
        plr->jump = 1;
        plr->somersault = 0;
        plr->land = 0;
        if ((plr->obj.boing & 2) != 0) {
          plr->jump_type = 3;
        } else {
          plr->jump_type = 1;
        }
        plr->jump_frame = 0;
        plr->jump_frames = minfo->JUMPFRAMES1 + 1;
        fVar6 = minfo->JUMPHEIGHT;
        if (plr->jump_type == 3) {
          fVar6 = fVar6 * 1.5f;
        }
        plr->obj.mom.y = fVar6 / (float)((s32)minfo->JUMPFRAMES2);
        plr->slam = 3;
        plr->slam_wait = 0;
        plr->obj.anim.anim_time = 1.0f;
      }
      if (plr->fire != 0) {
        plr->fire--;
      }
      if (plr->tap != 0) {
        plr->tap--;
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
    else if (plr->obj.character == 1) {
      MoveCOCO(plr,pad);
    }
    else if (VEHICLECONTROL == 2) {
      MoveSWIMMING(plr,pad);
    }
    else {
      MoveCRASH(plr,pad);
    }
    plr->obj.boing = 0;
  }
LAB_8001cbd4:
  if ((plr->obj.dead != 0) && (InvincibilityCHEAT != 0)) {
    plr->obj.dead = 0;
    plr_lives.count = (short)Game.lives;
  }
  if (Adventure != 0) {
    Game.wumpas = (u8)plr_wumpas.count;
    Game.lives = (u8)plr_lives.count;
    if ((plr->obj.mask != NULL) && (Game.mask = 2, plr->obj.mask->active < 3)) {
      Game.mask = *(u8 *)((s32)&plr->obj.mask->active + 1);
    }
  }
  BonusTiming(plr);
  fVar6 = plr->obj.pos.x - plr->obj.oldpos.x;
  dVar28 = (fVar6 * fVar6);
  fVar6 = plr->obj.pos.z - plr->obj.oldpos.z;
  dVar26 = (fVar6 * fVar6);
  dVar27 = (plr->obj.pos.y - plr->obj.oldpos.y);
  plr->obj.xz_distance = NuFsqrt(dVar28 + dVar26);
  plr->obj.xyz_distance = NuFsqrt((dVar27 * dVar27 + dVar28) + dVar26);
  plr->obj.anim.oldaction = plr->obj.anim.action;
  if ((vtog_time < vtog_duration) && (vtog_blend != 0)) {
    AnimateDIVE(plr,vtog_time / vtog_duration);
    goto LAB_8001ce44;
  }
  if (VEHICLECONTROL == 1) {
    if (plr->obj.vehicle == 0x53) {
      AnimateATLASPHERE(plr);
      goto LAB_8001ce44;
    }
    if (plr->obj.vehicle == 0x20) {
      AnimateSUBMARINE(plr);
      goto LAB_8001ce44;
    }
    if (plr->obj.vehicle == 0x6b) {
      AnimateSCOOTER(plr);
      goto LAB_8001ce44;
    }
    if (plr->obj.vehicle == 0xa0) {
      AnimateSNOWBOARD(plr);
      goto LAB_8001ce44;
    }
    if (plr->obj.vehicle == 0x36) {
      AnimateGLIDER(plr);
      goto LAB_8001ce44;
    }
    if (plr->obj.vehicle == 0x81) {
      AnimateDROPSHIP(plr);
      goto LAB_8001ce44;
    }
    if (plr->obj.vehicle == 0x3b) {
      AnimateGYRO(plr,pad);
      goto LAB_8001ce44;
    }
    if (plr->obj.vehicle == 0x44) {
      AnimateMECH(plr);
      goto LAB_8001ce44;
    }
    if (plr->obj.vehicle == 0xb2) {
      AnimateFIREENGINE(plr);
      goto LAB_8001ce44;
    }
    if (plr->obj.vehicle == 99) {
      AnimateJEEP(plr);
      goto LAB_8001ce44;
    }
    if (VEHICLECONTROL == 1) {
      if (plr->obj.vehicle == 0x89) {
        AnimateMINECART(plr);
        goto LAB_8001ce44;
      }
      if (plr->obj.vehicle == 0xa1) {
        AnimateMINETUB(plr);
        goto LAB_8001ce44;
      }
      if (plr->obj.vehicle == 0x8b) {
        AnimateMOSQUITO(plr);
        goto LAB_8001ce44;
      }
      if (plr->obj.vehicle == 0x99) {
        AnimateOFFROADER(plr);
        goto LAB_8001ce44;
      }
    }
  }
  if (VEHICLECONTROL == 2) {
    AnimateSWIMMING(plr);
  }
  else if (plr->obj.character == 1) {
    AnimateCOCO(plr);
  }
  else {
    AnimateCRASH(plr);
  }
LAB_8001ce44:
  if (plr->obj.dead != 0) {
    model = &CModel[plr->obj.die_model[0]];
  }
  else {
    if ((VEHICLECONTROL == 2) && (CRemap[115] != -1)) {
      model = &CModel[CRemap[115]];
    }
    else {
      model = plr->obj.model;
    }
      
  }
  if ((plr->obj.dead != 0x16) && (plr->obj.dead != 4)) {
    UpdateAnimPacket(model,&plr->obj.anim,0.5f,plr->obj.xz_distance);
  }
  plr->obj.frame++;
  if ((Cursor.menu != -1) || (GameMode == 1)) {
    loadsave_frame = 0;
  } else if (Level == 0x25) {
      if (InLoadSaveZone(plr) != 0) {
        if ((pad != NULL) && ((pad->paddata_db & 0x40) != 0)) {
          loadsave_frame = 0x3c;
        } else {
          loadsave_frame++;
        }
        if (loadsave_frame > 0x3c) {
          loadsave_frame = 0x3d;
        }
      } else {
        loadsave_frame = 0;
      }
      if (loadsave_frame == 0x3c) {
        NewMenu(&Cursor,0x15,4,-1);
        loadsave_frame = 0x3d;
        GameSfx(0x36,NULL);
      }
  }
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
