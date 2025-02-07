//NGC MATCH
void ResetPlayerEvents(void) {
  struct plrevent_s *event;
  s32 i;
  s32 j;
  
  if (world_scene[0] != NULL) {
    event = PlrEvent;
    for(i = 0; i < 8; i++,event++) {
      if (event->spl != NULL) {
        if (AheadOfCheckpoint((s32)event->iRAIL,(s32)event->iALONG,event->fALONG) != 0) {
          for(j = 0; j < 0x10; j++) {
            ResetHGobjAnim(&event->obj[j]);
            event->played = 0;
          }
        }
        else {
          if (Level == 0x14) {
            for(j = 0; j < 0x10; j++) {
              FinishHGobjAnim(&event->obj[j]);
              event->played = 1;
            }
          }
        }
      }
    }
  }
  return;
}

//NGC MATCH
void InitPlayerEvents(void) {
  struct plrevent_s *event;
  static char name_153[256];
  static char c_154;
  static char objname_155[32];
  s32 i;
  s32 j;
  s32 i_txt;
  s32 length;
  s32 count;
  struct nuvec_s *p0;
  struct nuvec_s* p1;
  
  if (world_scene[0] == NULL) {
      return;
  }
    event = PlrEvent;
    for(i = 0; i < 8; i++, event++) {
      event->spl = NULL;
      event->played = 0;
      sprintf(tbuf,"player_trigger_%.2i_",i);
      i_txt = strlen(tbuf);
      event->spl = NuSplineFindPartial(world_scene[0],tbuf,name_153);
      if (event->spl != NULL) {
        if (event->spl->len != 2) {
          event->spl = NULL;
        }
        if (event->spl != NULL) {
          p0 = (struct nuvec_s *)event->spl->pts;
          p1 = (struct nuvec_s *)(event->spl->pts + event->spl->ptsize);
          event->pos.x = ((p0->x + p1->x) * 0.5f);
          event->pos.y = ((p0->y + p1->y) * 0.5f);
          event->pos.z = ((p0->z + p1->z) * 0.5f);
          GetALONG(&event->pos,NULL,-1,-1,1);
          event->iRAIL = (char)temp_iRAIL;
          event->iALONG = temp_iALONG;
          event->fALONG = temp_fALONG;
          for(length = 0; length < 0x10; length++) {
            event->obj[length].special = NULL;
          }
          length = strlen((char *)name_153);
          for(j = 0, count = 0; i_txt < length; i_txt++) {
              c_154 = name_153[i_txt];
              if (((c_154 == 0x78) || (c_154 == 0x58)) || (i_txt == length - 1)) {
                if (i_txt == length - 1) {
                  objname_155[j] = c_154;
                  j++;
                }
                objname_155[j] = 0;
                if (NuSpecialFind(world_scene[0],&event->obj[count],objname_155) != 0) {
                  count++;
                  if (count == 0x10) break;
                }
                j = 0;
              }
              else {
                objname_155[j] = c_154;
                j++;
              }
          }
        }
      }
    }
  return;
}

//NGC MATCH
void CheckPlayerEvents(struct obj_s *obj) {
  struct plrevent_s *event;
  struct nuvec_s* p0;
  struct nuvec_s* p1;
  s32 i;
  s32 j;
  s32 k;
  
  if (world_scene[0] != NULL) {
    event = PlrEvent;
    for(i = 0; i < 8; i++, event++) {
      if (event->spl != NULL) {
        p0 = (struct nuvec_s *)event->spl->pts;
        p1 = (struct nuvec_s *) (event->spl->pts + event->spl->ptsize);
        if ((event->played == 0) &&
           (LineCrossed((obj->oldpos).x,(obj->oldpos).z,(obj->pos).x,(obj->pos).z,p0->x,p0->z,p1->x,p1->z) == 2)) {
          event->played = 1;
          k = 0;
          for(j = 0; j < 0x10; j++) {
            if (StartHGobjAnim(&event->obj[j]) != 0) {
              k++;
            }
          }
          if (k != 0) {
            JudderGameCamera(GameCam,0.5f,NULL);
            GameSfx(0x3b,NULL);
          }
        }
      }
    }
  }
  return;
}

//97.37% NGC
void AnimateCRASH(struct creature_s* plr) {
    struct MoveInfo* minfo;
    struct nuvec_s pos;

    minfo = &CrashMoveInfo;
    if ((u32)plr->obj.dead >= 2) {
        plr->obj.anim.newaction = plr->obj.die_action;
    } else if (GameMode == 1) {
        plr->obj.anim.newaction = 0x22;
    } else if (((Level == 0x25) && (tumble_time < tumble_duration)) && (last_hub != -1)) {
        plr->obj.anim.newaction = tumble_action;
    } else if (plr_tumblehack != 0) {
        plr->obj.anim.newaction = 0x22;
    } else if (plr_rebound != 0) {
        plr->obj.anim.newaction = 0x19;
    } else if (plr->freeze != 0) {
        plr->obj.anim.newaction = 0x1e;
    } else if (plr->target != 0) {
        if (plr->fire != 0) {
            plr->obj.anim.newaction = 0x4f;
        } else if (plr->obj.dyrot != 0) {
            plr->obj.anim.newaction = 0x50;
        } else {
            plr->obj.anim.newaction = 0x4e;
        }
    } else if (plr->obj.dangle != 0) {
        if (plr->obj.dangle == 2) {
            plr->obj.anim.newaction = 0x36;
        } else {
            plr->obj.anim.newaction = (plr->obj.pad_speed == 0.0f) ? 0x20 : 0x4d;
        }
    } else if (plr->slam_wait != 0) {
        plr->obj.anim.newaction = 0x1f;
    } else if (plr->slam == 2) {
        plr->obj.anim.newaction = 0x4b;
    } else if (plr->slam == 1) {
        plr->obj.anim.newaction = 0x41;
    } else if (plr->slide != 0) {
        plr->obj.anim.newaction = 0x43;
    }
    else if ((plr->jump != 0) && ((plr->obj.ground == 0 || (plr->jump_frame < plr->jump_frames))))
    {
        if (plr->somersault != 0) {
            plr->obj.anim.newaction = 0x44;
        } else if (plr->jump_type == 4) {
            plr->obj.anim.newaction = 0x19;
        } else if (plr->jump_type == 0) {
            plr->obj.anim.newaction = (plr->jump_subtype == 1) ? 0x2d : 0x2e;
        } else {
            plr->obj.anim.newaction = (plr->jump != 5) ? 0x2c : 0x44;
        }
    } else if ((plr->land != 0) && (plr->obj.pad_speed == 0.0f)) {
        plr->obj.anim.newaction = 0x30;
    } else if (((plr->crouch_pos > 0) && (plr->crouch_pos < plr->OnFootMoveInfo->CROUCHINGFRAMES)) && (plr->obj.pad_speed == 0.0f))
    {
        plr->obj.anim.newaction = (plr->crawl != 0) ? 3 : 5;
    } else {
        if ((plr->crawl != 0) && ((plr->jump == 0) || (plr->obj.ground != 0))) {
            if (plr->obj.pad_speed == 0.0f) {
                plr->obj.anim.newaction = 4;
            } else {
                plr->obj.anim.newaction = 2;
            }
        } else {
            if (plr->obj.wade != 0 && plr->obj.ground != 0 && plr->obj.pad_speed != 0.0f) {
                plr->obj.anim.newaction = 0x72;
            } else if ((plr->obj.ground != 0 || (plr->allow_jump) != 0) && 
                    (TerSurface[plr->obj.surface_type].flags & 0x20) && 
                    ((plr->obj.pad_speed != 0.0f || (plr->obj.xz_distance >= minfo->IDLESPEED)))) {
                plr->obj.anim.newaction = (plr->obj.pad_speed == 0.0f) ? 0x40 : 0x3f;
            } else if (((plr->obj.ground == 0) && (plr->allow_jump == 0)) && (plr->obj.mom.y < -FALLONCRATEBREAKSPEED)) {
                plr->obj.anim.newaction = 0x19;
            } else {
                if ((plr->obj.pad_speed == 0.0f)
                    && ((
                        plr->pad_type != 1
                        || !(((plr->obj.mom.x * plr->obj.mom.x + plr->obj.mom.z * plr->obj.mom.z))
                            >= (plr->OnFootMoveInfo->IDLESPEED * plr->OnFootMoveInfo->IDLESPEED))
                    )))
                {
                    if (((plr->obj.ground == 1) || ((plr->obj.last_ground == 1 && (plr->allow_jump != 0))))
                        && ((plr->obj.pos.y + plr->obj.bot * plr->obj.SCALE) - plr->obj.shadow > 0.25f))
                    {
                        plr->obj.anim.newaction = 0x51;
                    } else {
                        plr->obj.anim.newaction = 0x22;
                    }
                } else {
                    if (plr->tiptoe != 0) {
                        plr->obj.anim.newaction = 0x53;
                    } else if (plr->sprint != 0) {
                        plr->obj.anim.newaction = 0x48;
                    } else if (plr->obj.pad_speed == minfo->WALKSPEED) {
                        plr->obj.anim.newaction = 0x74;
                    } else {
                        plr->obj.anim.newaction = 0x3a;
                    }
                }
            } 
        }
    }

    UpdateChaseRunAnim(plr);
    if (((plr->obj.anim.newaction == 2) || (plr->obj.anim.newaction == 0x43)) || (plr->obj.anim.newaction == 4)) {
        plr->crouch_pos = minfo->CROUCHINGFRAMES;
    } else if ((plr->obj.pad_speed != 0.0f) || ((plr->obj.anim.newaction != 3 && (plr->obj.anim.newaction != 5))))
    {
        plr->crouch_pos = 0;
    }
    
    UpdateCharacterIdle(plr, 0);
    if (((plr->obj.dead == 0) && (plr->spin != 0))
        && ((s32)plr->spin_frame < (s32)plr->spin_frames - (s32)plr->OnFootMoveInfo->SPINRESETFRAMES)) {
        pos.x = plr->obj.pos.x;
        pos.y = (plr->obj.top - plr->obj.bot * plr->obj.SCALE) * ((float)qrand() * 0.000015259022f - 0.5f) +
            (plr->obj.bot + plr->obj.top * plr->obj.SCALE * 0.5f) + plr->obj.pos.y;
        pos.z = plr->obj.pos.z;
        
        AddGameDebrisRot(0xb, &pos, 1, 0x4000, qrand());
        AddGameDebrisRot(0xc, &plr->obj.pos, 1, 0x2000, qrand());
    }
}

//NGC MATCH
void AnimateCOCO(struct creature_s *plr) {
  struct MoveInfo* minfo = &CocoMoveInfo;
  
  if ((u32)plr->obj.dead > 1) {
    plr->obj.anim.newaction =  plr->obj.die_action;
  }
  else {
    if (((Level == 0x25) && (tumble_time < tumble_duration)) && (last_hub != -1)) {
            plr->obj.anim.newaction =  tumble_action;
    }
    else if ((plr->spin != 0) && (plr->spin_frame < plr->spin_frames - plr->OnFootMoveInfo->SPINRESETFRAMES)) {
        plr->obj.anim.newaction =  0x46;
      }
      else {
        if (plr->slam_wait != 0) {
          plr->obj.anim.newaction =  0x1f;
        }
        else {
          if (plr->slam == 1) {
            plr->obj.anim.newaction =  0x41;
          }
          else if (plr->slide != 0) {
            plr->obj.anim.newaction =  0x43;
          }
          else if ((plr->jump != 0) && ((plr->obj.ground == 0 || (plr->jump_frame < plr->jump_frames)))) {
                if (plr->somersault != 0) {
                  plr->obj.anim.newaction =  0x44;
                }
                else if (plr->jump_type == 4) {
                    plr->obj.anim.newaction =  0x19;
                }
                else {
                    if (plr->jump_type == 2) {
                      plr->obj.anim.newaction =  0x2d;
                    } else {
                      plr->obj.anim.newaction =  0x2c;
                    }
                } 
          }
            else if ((plr->land != 0) && (plr->obj.pad_speed == 0.0f)) {
                if (plr->jump_type == 2) {
                  plr->obj.anim.newaction =  0x31;
                } else {
                    plr->obj.anim.newaction =  0x30;
                }   
              } else if ((((plr->obj.ground != 0) || (plr->allow_jump != 0)) &&
                    ((TerSurface[plr->obj.surface_type].flags & 0x20) != 0)) &&
                   ((plr->obj.pad_speed != 0.0f || (plr->obj.xz_distance >= minfo->IDLESPEED)))) {
                  plr->obj.anim.newaction =  0x40;
              }
              else {
                  if ((plr->obj.ground == 0) && ((plr->allow_jump == 0 && (plr->obj.mom.y < -FALLONCRATEBREAKSPEED)))) {
                    plr->obj.anim.newaction =  0x19;
                  }
                  else {
                    if ((plr->obj.pad_speed == 0.0f) && (plr->pad_type != 1
                     || !((POW2(plr->obj.mom.x) + POW2(plr->obj.mom.z)) >= POW2(plr->OnFootMoveInfo->IDLESPEED)))) {
                      plr->obj.anim.newaction =  0x22;
                    }
                    else {
                      if (plr->sprint != 0) {
                        plr->obj.anim.newaction =  0x48;
                      }
                      else {
                            if (plr->obj.pad_speed == minfo->WALKSPEED) {
                              plr->obj.anim.newaction =  0x74;
                            } else {
                              plr->obj.anim.newaction =  0x3a;
                            }
                      }
                    }
                  }
              }
        }
      }
  }
  UpdateCharacterIdle(plr,1);
  return;
}

//NGC MATCH
void MoveSWIMMING(struct creature_s *plr,struct nupad_s *pad) {
  struct MoveInfo* minfo = &SwimmingMoveInfo;
  s32 SPIN;

  SPIN = (pad->paddata & 0xa8);
  GameSfxLoop(0x27,&plr->obj.pos);
  if ((plr->tap == 0) && ((pad->paddata_db & 0x40) != 0)) {
    plr->tap = 0xc;
    gamesfx_pitch = 0x96;
    GameSfx(0x28,&plr->obj.pos);
  }
  if (plr->spin_wait != 0) {
    plr->spin_wait--;
  }
  if ((plr->spin != 0) && (plr->spin_frame < plr->spin_frames)) {
    plr->spin_frame++;
  }
  if (SPIN != 0) {
    if (plr->spin == 0) {
      plr->spin_frame = 0;
      plr->spin = 1;
      plr->spin_frames = minfo->SPINFRAMES + minfo->SPINRESETFRAMES;
      plr->spin_wait = minfo->SUPERSPINWAITFRAMES;
      GameSfx(0x28,&plr->obj.pos);
    }
    else if (((ExtraMoves != 0) || ((Game.powerbits & 4) != 0)) && ((char)plr->spin < 7)) {
      if ((plr->spin_wait != 0) && ((plr->spin & 1) == 0)) {
        plr->spin++;
        plr->spin_frames = plr->spin_frames + minfo->SUPERSPINFRAMES;
        plr->spin_wait = minfo->SUPERSPINWAITFRAMES;
        GameSfx(0x28,&plr->obj.pos);
      }
    }
  }
  else {
    if (plr->spin_frame == plr->spin_frames) {
      plr->spin = 0;
    }
    else if (((ExtraMoves != 0) || ((Game.powerbits & 4) != 0)) && ((plr->spin & 1U) != 0)) {
      plr->spin++;
    }
  }
  plr->obj.attack = 0;
  if ((plr->spin != 0) &&
     (plr->spin_frame < plr->spin_frames - plr->OnFootMoveInfo->SPINRESETFRAMES)) {
    plr->obj.attack = 2;
  }
  if ((plr->obj.mask != NULL) && (2 < plr->obj.mask->active)) {
    plr->obj.attack = plr->obj.attack | 0x80;
  }
  return;
}

//NGC MATCH
void AnimateSWIMMING(struct creature_s *plr) {
  struct MoveInfo* minfo = &SwimmingMoveInfo;
  
  if ((u32)plr->obj.dead > 1) {
    plr->obj.anim.newaction = plr->obj.die_action;
  }
  else {
    if ((plr->spin != 0) && (plr->spin_frame < plr->spin_frames - plr->OnFootMoveInfo->SPINRESETFRAMES)) {
      plr->obj.anim.newaction = 0x46;
    }
    else {
      if ((plr->obj.pad_speed > 0.0f) || ((plr->tap != 0 ||(NuFabs(plr->obj.mom.z) > minfo->WALKSPEED)))) {
        plr->obj.anim.newaction = 0x4c;
      }
      else {
        plr->obj.anim.newaction = 0x22;
      }
    }
  }
  UpdateCharacterIdle(plr,0x73);
  return;
}

//NGC MATCH
void AnimateSCOOTER(struct creature_s *plr) {
  if ((u32)plr->obj.dead > 1) {
    plr->obj.anim.newaction = plr->obj.die_action;
  }
  else {
    if ((plr->jump != 0) && ((plr->obj.ground == 0 || (plr->jump_frame < plr->jump_frames)))) {
      if (plr->obj.anim.anim_time >= SCOOTERJUMPPAUSEFRAME) {
        plr->obj.anim.anim_time = SCOOTERJUMPPAUSEFRAME;
      }
      plr->obj.anim.newaction = 99;
    }
    else {
      if (plr->land != 0) {
        plr->obj.anim.newaction = 99;
      }
      else {
        if (plr->sprint != 0) {
          plr->obj.anim.newaction = 0x6a;
        } else {
          plr->obj.anim.newaction = 0x68;
        }
      }
    }
  }
  UpdateCharacterIdle(plr,1);
  return;
}

//NGC MATCH
void AnimateOFFROADER(struct creature_s *plr) {
  s32 i;
  
  if (best_cRPos != NULL) {
    i = RotDiff(best_cRPos->angle,plr->obj.hdg);
  }
  else {
    i = 0;
  }
  if ((u32)plr->obj.dead > 1) {
    plr->obj.anim.newaction = plr->obj.die_action;
  }
  else {
    if (Level == 3) {
      if ((((Pad[0] == NULL) || ((Pad[0]->paddata & 0x60) == 0)) ||
          (SmokeyCountDownValue > 0)) && (plr->obj.xz_distance < 0.01666667f)) {
        plr->obj.anim.newaction = 0x62;
        goto LAB_80056fb0;
      }
    }
    if (i > 0x800) {
      plr->obj.anim.newaction = 0x67;
    }
    else {
      if (i < -0x800) {
        plr->obj.anim.newaction = 0x65;
      } else {
      plr->obj.anim.newaction = 0x68;
      }
    } 
  }
LAB_80056fb0:
  UpdateCharacterIdle(plr,0);
  return;
}

//NGC MATCH
void AnimateFIREENGINE(struct creature_s *plr) {
  if ((u32)plr->obj.dead > 1) {
    plr->obj.anim.newaction = plr->obj.die_action;
  }
  else {
    if ((Level == 0x16) && (FireBossHoldPlayer != 0)) {
      plr->obj.anim.newaction = 0x62;
    }
    else if ((plr->jump != 0) && ((plr->obj.ground == 0 || (plr->jump_frame < plr->jump_frames)))) {
        if (plr->jump_type == 2) {
          if (plr->obj.anim.anim_time >= FIREENGINESTANDINGJUMPPAUSEFRAME) {
            plr->obj.anim.anim_time = FIREENGINESTANDINGJUMPPAUSEFRAME;
          }
          plr->obj.anim.newaction = 100;
        } else {
          if (plr->obj.anim.anim_time >= FIREENGINERUNNINGJUMPPAUSEFRAME) {
            plr->obj.anim.anim_time = FIREENGINERUNNINGJUMPPAUSEFRAME;
          }
          plr->obj.anim.newaction = 99;
        }
    }
    else if (plr->land != 0) {
        if (plr->jump_type == 2) {
          plr->obj.anim.newaction = 100;
        } else {
          plr->obj.anim.newaction = 99;
        }
      }
      else {
        plr->obj.anim.newaction = 0x68;
      }
  }
  UpdateCharacterIdle(plr,0);
  return;
}

//NGC MATCH
void DrawCarpet() {
  struct numtx_s m;
  float **dwa;
  struct nuvec_s v;
  
  if (carpet_model[0] != NULL) {
    SetNearestLights(&carpet_lights);
    v.x = v.y = v.z = CData[carpet_character[0]].scale * 0.5f;
    NuMtxSetScale(&m,&v);
    NuMtxRotateX(&m,carpet_xrot);
    NuMtxRotateY(&m,carpet_yrot + 0x8000);
    NuMtxTranslate(&m,&carpet_pos);
    EvalModelAnim(carpet_model[0],&carpet_anim,&m,tmtx,&dwa,NULL);
    NuHGobjRndrMtxDwa(carpet_model[0]->hobj,&m,1,NULL,tmtx,dwa);
    SetLevelLights();
  }
  return;
}

//NGC MATCH
void AnimateSUBMARINE(struct creature_s *plr) {
  plr->obj.anim.newaction = 0x62;
  UpdateCharacterIdle(plr,0);
  AddGameDebrisRot(0x56,plr->momLOCATOR[2],1,0,0);
  AddGameDebrisRot(0x56,(struct nuvec_s *)&plr[1].ai.radius,1,0,0);
  return;
}

//NGC MATCH
void AnimateMINECART(struct creature_s *plr) {
  if ((u32)plr->obj.dead > 1) {
    plr->obj.anim.newaction = plr->obj.die_action;
  }
  else {
    if (plr->jump != 0) {
      if (plr->jump_frame >= plr->jump_frames) {
       plr->obj.anim.newaction = 0x68;
      } else {
       plr->obj.anim.newaction = 99;
      }
    } else {
        plr->obj.anim.newaction = 0x68;
    }
  }
  UpdateCharacterIdle(plr,0);
  return;
}

//NGC MATCH
void AnimateMINETUB(struct creature_s* plr) {
    s32 l;
    s32 r;
    s32 off1;
    s32 off2;
    struct nuvec_s vec;

    if (plr->obj.dead != 0 && plr->obj.dead != 1) {
        plr->obj.anim.newaction = plr->obj.die_action;
    } else if (plr->obj.direction == 3) {
        plr->obj.anim.newaction = 0x65;
    } else if (plr->obj.direction == 4) {
        plr->obj.anim.newaction = 0x67;
    } else {
        plr->obj.anim.newaction = 0x68;
    }

    UpdateCharacterIdle(plr, 0);
    if (plr->obj.anim.newaction != plr->obj.die_action) {
        off1 = 0x5c;
        off2 = 0x5c;
        l = 0x4000;
        r = -0x4000;
        if (plr_tub_tilt < 0) {
            l = -0x4000;
            off2 = 0x5d;
        } else if (plr_tub_tilt > 0) {
            off1 = 0x5c;
            off2 = 0x5d;
            r = 0x4000;
        } else if (PlrTub.spark != 0) {
            off1 = 0x5d;
            off2 = 0x5d;
        }

        if ((Level != 0x1c) || (race_finished == 0)) {
            vec.x = NuTrigTable[(s32)plr->obj.hdg + l & 0xffff] * 0.45f + plr->obj.pos.x;
            vec.y = plr->obj.pos.y;
            vec.z = NuTrigTable[(((s32)plr->obj.hdg + l & 0xffff) + 0x4000) & 0x2ffff] * 0.45f + plr->obj.pos.z;
            AddVariableShotDebrisEffect(GDeb[off1].i, &vec, 1, 0, plr->obj.hdg);

            vec.x = NuTrigTable[(s32)plr->obj.hdg + r & 0xffff] * 0.44999999f + plr->obj.pos.x;
            vec.y = plr->obj.pos.y;
            vec.z =
                NuTrigTable[(((s32)plr->obj.hdg + r & 0xffff) + 0x4000) & 0x2ffff] * 0.44999999f + plr->obj.pos.z;
            AddVariableShotDebrisEffect(GDeb[off2].i, &vec, 1, 0, plr->obj.hdg);
        }
    }
    
    if (Level == 0x1c) {
        if (opptub_wait != 0) {
            opptub_wait--;
            if (opptub_wait == 0) {
                if (opptub_action == 0x62) {
                    opptub_action = (qrand() < 0x8000) ? 0x5f : 0x60;
                    opptub_wait = ModelAnimDuration(OppTub.c->obj.character, opptub_action, 0.0f, 0.0f) * 60.0f;
                } else {
                    opptub_action = 0x62;
                    opptub_wait = (qrand() * 0x3c / 0x10000) + 0x3c;
                }
            }
        }
        OppTub.c->obj.anim.oldaction = OppTub.c->obj.anim.action;
        OppTub.c->obj.anim.newaction = opptub_action;
        UpdateAnimPacket(OppTub.c->obj.model, &OppTub.c->obj.anim, 0.5f, OppTub.c->obj.xz_distance);
    }
    return;
}

//NGC MATCH
void AnimateGYRO(struct creature_s* plr, struct nupad_s* pad) {
    s32 i;

    i = pad->paddata & 0x60;
    if ((plr->spin != 0) && (plr->spin_frame < plr->spin_frames - plr->OnFootMoveInfo->SPINRESETFRAMES)) {
        plr->obj.anim.newaction = 0x69;
    } else if (plr->tap != 0) {
        plr->obj.anim.newaction = 0x67;
    } else if ((i == 0x40) || (i == 0x20)) {
        plr->obj.anim.newaction = 0x70;
    } else if (plr->obj.pad_speed > 0.0f) {
        if ((plr->obj.pad_angle < 0x2000) || (plr->obj.pad_angle > 0xE000)) {
            plr->obj.anim.newaction = 0x5a;
        } else if ((plr->obj.pad_angle > 0x6000) && (plr->obj.pad_angle < 0xA000)) {
            plr->obj.anim.newaction = 0x70;
        } else if ((plr->obj.pad_angle & 0x8000) == 0) {
            plr->obj.anim.newaction = (plr->obj.direction == 0) ? 0x67 : 0x65;
        } else {
            plr->obj.anim.newaction = (plr->obj.direction == 0) ? 0x65 : 0x67;
        }
    } else {
        plr->obj.anim.newaction = 0x62;
    }

    UpdateCharacterIdle(plr, 0);
    return;
}

//NGC DONE (96%) loop
void CheckGates(struct obj_s* obj) {
    struct nugspline_s* spl;
    struct nuvec_s* p0;
    struct nuvec_s* p1;
    s32 i;
    s32 j;
    s32 sfx;
    s32 iVar4;

    if ((TimeTrial == 0) && (SplTab[28].spl != NULL)) {
        spl = SplTab[28].spl;
        for (i = 0, j = 1; i < GATECOUNT; i++, j += 2) {
            if ((gate_bits & (1 << i)) == 0) {
                p0 = (struct nuvec_s*)(spl->pts + ((j * 1) * spl->ptsize));
                p1 = (struct nuvec_s*)(spl->pts + ((i * 2) * spl->ptsize));
                
                iVar4 = LineCrossed(obj->oldpos.x, obj->oldpos.z, obj->pos.x, obj->pos.z, p1->x, p1->z, p0->x, p0->z);
                
                if (iVar4 != 0) {
                    NewBuzz(&player->rumble, 6);
                }
                if (iVar4 == 2) {
                    sfx = 0x36;
                    gate_bits |= 1 << i;
                    plr_gates++;
                    if (plr_gates == GATECOUNT) {
                        if ((Level == 5) && ((Game.level[0].flags & 0x20) == 0)) {
                            PickupBonusGem(4);
                        }
                    }
                } else if (iVar4 == 1) {
                    sfx = 0x3c;
                } else {
                    sfx = -1;
                }
                
                if (sfx != -1) {
                    GameSfx(sfx, NULL);
                }
            }
        }
    }
    return;
}

//NGC MATCH
void ResetGates(void) {
  struct nugspline_s *spl;
  struct nuvec_s* p0;
  struct nuvec_s* p1;
  struct nuvec_s pos;
  
  GATECOUNT = 0;
  spl = SplTab[28].spl;
  if (spl != NULL) {
    GATECOUNT = (int)(spl)->len / 2;
    if (0x40 < GATECOUNT) {
      GATECOUNT = 0x40;
    }
    p0 = (struct nuvec_s *) spl->pts;
    p1 = (struct nuvec_s *) (spl->pts + spl->ptsize);
    pos.x = (p0->x + p1->x) * 0.5f;
    pos.y = (p0->y + p1->y) * 0.5f;
    pos.z = (p0->z + p1->z) * 0.5f;
    GetALONG(&pos,NULL,-1,-1,1);
    if (AheadOfCheckpoint(temp_iRAIL,temp_iALONG,temp_fALONG) != 0) {
      plr_gates = 0;
      gate_bits = 0;
      if (Level == 5) {
        bonusgem_ok = 0;
      }
    }
  }
  return;
}

//NGC MATCH
void ResetTub(struct opptub_s *tub,struct creature_s *c) {
  tub->c = c;
  tub->track = -1;
  tub->count = -1;
  tub->laps = -1;
  tub->wait = 0;
  tub->finished = 0;
  tub->stall = 0;
  tub->boost = 0;
  return;
}

//NGC MATCH
void UpdateArrow(struct nuvec_s *src,struct nuvec_s *dst) {
  s32 temp;
  u16 xrot;
  u16 yrot;
  struct nuvec_s v;
  
  NuVecSub(&v,src,dst);
  temp = NuAtan2D(v.y,NuFsqrt(v.x * v.x + v.z * v.z));
  xrot = -(short)temp;
  yrot = (u16)NuAtan2D(v.x,v.z);
  if (GameTimer.frame != 0) {
    arrow_xrot = SeekRot(arrow_xrot,-(short)temp,2);
    arrow_yrot = SeekRot(arrow_yrot,(u16)yrot,2);
    return;
  }
  arrow_xrot = xrot;
  arrow_yrot = yrot;
  return;
}

//NGC MATCH
void DrawPanel3DArrow(void) {
  if (ObjTab[108].obj.special != NULL) {
    DrawPanel3DObject(0x6c,0.0f,0.5f,1.0f,0.125f,0.125f,0.125f,arrow_xrot,RotDiff(GameCam[0].yrot,arrow_yrot),0,ObjTab[108].obj.scene,
                      ObjTab[108].obj.special,0);
  }
  return;
}

//NGC MATCH
void ResetRings(void) {
  struct nuvec_s *pos;
  struct nuvec_s v;
  s32 i;
  s32 temp;
  
  RINGCOUNT = 0;
  if (((SplTab[64].spl != NULL) && (ObjTab[103].obj.special != NULL)) && (ObjTab[104].obj.special != NULL)) {
    RINGCOUNT = (s32)(SplTab[64].spl)->len / 2;
    if (0x30 < RINGCOUNT) {
      RINGCOUNT = 0x30;
    }
    pos = (struct nuvec_s *)(SplTab[64].spl)->pts;
    GetALONG(pos,NULL,-1,-1,1);
    if (AheadOfCheckpoint(temp_iRAIL,temp_iALONG,temp_fALONG) != 0) {
      i_ring = 0;
    }
    for (i = 0; i < RINGCOUNT; i++, pos+=2) {
        NuVecSub(&v,(pos + 1),pos);
        ring_yrot[i] = (short) NuAtan2D(v.x,v.z);
        temp = NuAtan2D(v.y,NuFsqrt(v.x * v.x + v.z * v.z));
        ring_xrot[i] = -(short)temp;
        if (i == i_ring) {
          ring_pos = *pos;
        }
    }
  }
  return;
}

//NGC MATCH
void CheckRings(struct obj_s* obj, s32* index) {
    struct nugspline_s* spl;
    struct nugspline_s* spl1;
    struct nuvec_s* pos;
    struct nuvec_s oldpos;
    struct nuvec_s newpos;
    struct nuvec_s opos;
    struct nuvec_s npos;
    struct nuvec_s n;
    struct nuvec_s* p0;
    struct nuvec_s* p1;
    struct nuvec_s lpos;
    struct nuvec_s rpos;
    struct nuvec_s v;
    float d;
    float r;
    s32 PLAYER;
    s32 i;
    
    if ((RINGCOUNT != 0) && (index[0] != RINGCOUNT)) {
        oldpos.x = obj->oldpos.x;
        oldpos.y = (((obj->bot * obj->old_SCALE) + obj->oldpos.y) + ((obj->top * obj->old_SCALE) + obj->oldpos.y)) * 0.5f;
        oldpos.z = obj->oldpos.z;
        
        newpos.x = obj->pos.x;
        newpos.y = (((obj->bot + obj->top) * obj->SCALE) * 0.5f) + obj->pos.y;
        newpos.z = obj->pos.z;
        
        PLAYER = obj == &player->obj;
        
        pos = (struct nuvec_s*)(SplTab[64].spl)->pts;
        
        for (i = 0; i < RINGCOUNT; i++, pos += 2) {
            d = NuVecDist(pos + 1, pos, NULL);
            if (NuVecDist(&oldpos, pos, NULL) <= d) {
                if (NuVecDist(&newpos, pos, NULL) <= d) {
                    NuVecSub(&n, pos + 1, pos);
                    NuVecNorm(&n, &n);
                    NuVecSub(&opos, &oldpos, pos);
                    NuVecNorm(&opos, &opos);
                    NuVecSub(&npos, &newpos, pos);
                    NuVecNorm(&npos, &npos);
                    if (((NuVecDot(&n, &opos) < 0.0f) && (0.0f <= NuVecDot(&n, &npos)))
                        || ((0.0f <= NuVecDot(&n, &opos) && (NuVecDot(&n, &npos) < 0.0f))))
                    {
                        NewBuzz(&player->rumble, 6);
                        if (i == index[0]) {
                            GameSfx(0x1e, &obj->pos);
                            index[0]++;
                            if (PLAYER == 0) {
                                return;
                            }
                            if (index[0] < RINGCOUNT) {
                                ring_pos = *(pos + 3);
                            }
                        } else {
                            GameSfx(0x4f, &obj->pos);
                        }
                        break;
                    }
                }
            }
        }
        
        if (PLAYER != 0) {
            if (best_cRPos != NULL) {
                spl = Rail[best_cRPos->iRAIL].pLEFT;
                p0 = (struct nuvec_s*)(spl->pts + (best_cRPos->iALONG * spl->ptsize));
                p1 = (struct nuvec_s*)(spl->pts + (best_cRPos->i1 * spl->ptsize));
                
                lpos.x = (p0->x + p1->x) * 0.5f;
                lpos.y = (p0->y + p1->y) * 0.5f;
                lpos.z = (p0->z + p1->z) * 0.5f;

                spl1 = Rail[best_cRPos->iRAIL].pRIGHT;
                p0 = (struct nuvec_s*)(spl1->pts + (best_cRPos->iALONG * spl1->ptsize));
                p1 = (struct nuvec_s*)(spl1->pts + (best_cRPos->i1 * spl1->ptsize));
                
                rpos.x = (p0->x + p1->x) * 0.5f;
                rpos.y = (p0->y + p1->y) * 0.5f;
                rpos.z = (p0->z + p1->z) * 0.5f;
                
                v.x = (lpos.x + rpos.x) * 0.5f;
                v.y = (lpos.y + rpos.y) * 0.5f;
                v.z = (lpos.z + rpos.z) * 0.5f;
                
                d = NuVecDist(&newpos, &v, NULL);
                if (2.5f < d) {
                    obj->mom.x = ((v.x - newpos.x) * RINGPULL) + obj->mom.x;
                    obj->mom.y = ((v.y - newpos.y) * RINGPULL) + obj->mom.y;
                    obj->mom.z = ((v.z - newpos.z) * RINGPULL) + obj->mom.z;
                }
            }
            
            if (PLAYER != 0) {
                UpdateArrow(&newpos, &ring_pos);
            }
        }
    }
    return;
}

//NGC MATCH
void DrawRings(void) {
    struct nugspline_s* spl;
    struct nuvec_s* pos;
    s32 j;
    s32 i;

    if (RINGCOUNT != 0) {
        spl = SplTab[64].spl;
        for (i = 0; i < RINGCOUNT; i++) {
            pos = (struct nuvec_s*)(spl->pts + ((i * 2) * spl->ptsize));

            if ((i == (RINGCOUNT - 1)) && (i_ring == RINGCOUNT)) {
                j = 0x69;
            } else if (i == i_ring) {
                if ((i == 0) || (i == RINGCOUNT - 1)) {
                    j = 0x69;
                } else {
                    j = 0x68;
                }
            } else {
                j = 0x67;
            }

            Draw3DObject(j, pos, ring_xrot[i], ring_yrot[i], 0, 1.0f, 1.0f, 1.0f, ObjTab[j].obj.scene, ObjTab[j].obj.special, 0);
        }
    }
    return;
}

//NGC MATCH
void UpdateChaseRunAnim(struct creature_s *plr) {
  s32 turn;
  s32 iVar4;
  s32 rot;
  
  if (((plr->obj).anim.newaction == 0x3a) &&
     ((((Chase[0].status == 2 || (Chase[1].status == 2)) || (Chase[2].status == 2))
      && (((best_cRPos != NULL) && (rot = RotDiff((plr->obj).hdg,best_cRPos->angle), rot = rot >= 0 ? rot : -rot,
      (rot < 0x2000))))))) {
        if ((plr_tocam_wait != 0) && (plr_tocam_wait = plr_tocam_wait + -1, plr_tocam_wait == 0)) {
          plr_tocam_wait = (qrand() * 0x3c >> 0x10) + 0x3c;
          if (plr_tocam_turn == -1) {
            iVar4 = qrand();
              if (iVar4 < 0) {
                iVar4 += 0x7fff;
              }
              plr_tocam_turn = iVar4 >> 0xf;
          }
          else {
            if (plr_tocam_turn == 1) {
            iVar4 = -qrand();
              if (iVar4 < 0) {
                iVar4 += 0x7fff;
              }
            plr_tocam_turn = iVar4 >> 0xf;
            } else {
              if (qrand() < 0x8000) {
                  turn = -1;
              } else {
                turn = 1;
              }
                plr_tocam_turn = turn;
            }
          }
        }
  }
  else {
    plr_tocam_turn = 0;
    plr_tocam_wait = (qrand() * 0x3c >> 0x10) + 0x3c;
  }
  if ((plr->obj).anim.newaction == 0x3a) {
    if (plr_tocam_turn < 0) {
      (plr->obj).anim.newaction = 0x32;
    }
    else if (plr_tocam_turn > 0) {
      (plr->obj).anim.newaction = 0x39;
    }
    else {
      (plr->obj).anim.newaction = 0x3a;
    }
  }
  return;
}

//NGC MATCH
void AnimateGLIDER(struct creature_s *plr) {
  if (FlyingLevelVictoryDance != 0) {
    if (plr->obj.anim.newaction != 0x75) {
      plr->obj.anim.oldaction = plr->obj.anim.action;
    }
    plr->obj.anim.newaction = 0x75;
  }
  else {
    if (((plr->obj.pad_speed > 0.0f) && (plr->obj.pad_angle > 0x9555)) && (plr->obj.pad_angle < 0xeaab)) {
      plr->obj.anim.newaction = 0x65;
    }
    else {
      if ((plr->obj.pad_speed > 0.0f) && (plr->obj.pad_angle > 0x1555)) {
          if (plr->obj.pad_angle > 0x6aaa) {
            plr->obj.anim.newaction = 0x62;
          } else {
           plr->obj.anim.newaction = 0x67;
          }
      } else {
        plr->obj.anim.newaction = 0x62;
      }
    }
  }
  UpdateCharacterIdle(plr,0);
  return;
}

//NGC MATCH
void AnimateDROPSHIP(struct creature_s *plr) {
  if (FlyingLevelVictoryDance != 0) {
    if (plr->obj.anim.newaction != 0x75) {
      plr->obj.anim.oldaction = plr->obj.anim.action;
    }
    plr->obj.anim.newaction = 0x75;
  }
  else {
    plr->obj.anim.newaction = 0x62;
  }
  UpdateCharacterIdle(plr,1);
  return;
}

//NGC MATCH
void AnimateATLASPHERE(struct creature_s *plr) {
  if ((u32)plr->obj.dead > 1) {
    plr->obj.anim.newaction = plr->obj.die_action;
  }
  else {
    if ((plr->obj.pad_speed == 0.0f) && (plr->obj.xz_distance < 0.004166667f)) {
      plr->obj.anim.newaction = 0x22;
    }
    else {
      if (plr->obj.xz_distance < CrashMoveInfo.WALKSPEED) {
        plr->obj.anim.newaction = 0x71;
      } else {
       plr->obj.anim.newaction = 0x68;
      }
    }
  }
  UpdateCharacterIdle(plr,0);
  return;
}

//NGC MATCH
void AnimateJEEP(struct creature_s *plr) {
  if ((u32)plr->obj.dead < 2) {
    plr->obj.anim.newaction = 0x68;
  } else {
    plr->obj.anim.newaction = plr->obj.die_action;
  }
  UpdateCharacterIdle(plr,0);
  return;
}

//NGC MATCH
void AnimateMOSQUITO(struct creature_s *plr) {
  if (FlyingLevelVictoryDance != 0) {
    if (plr->obj.anim.newaction != 0x75) {
      plr->obj.anim.oldaction = plr->obj.anim.action;
    }
    plr->obj.anim.newaction = 0x75;
  }
  else {
    plr->obj.anim.newaction = 0x62;
  }
  UpdateCharacterIdle(plr,0);
  return;
}

//NGC MATCH
void AnimateMECH(struct creature_s *plr) {
  struct MoveInfo *minfo;

  minfo = &MechMoveInfo;
  if ((u32)plr->obj.dead > 1) {
    plr->obj.anim.newaction = plr->obj.die_action;
  }
  else {
    if (plr->fire != 0) {
      plr->obj.anim.newaction = plr->fire_action;
    }
    else {
      if (plr->target != 0) {
        if (plr->fire_action == 0x5d) {
          plr->obj.anim.newaction = 0x6c;
        }
        else if (plr->fire_action == 0x5f) {
          plr->obj.anim.newaction = 0x6e;
        }
        else {
          if (plr->fire_action == 0x60) {
            plr->obj.anim.newaction = 0x6f;
          } else {
            plr->obj.anim.newaction = 0x6b;
          } 
        }
      }
      else {
        if ((plr->jump != 0) && ((plr->obj.ground == 0 || (plr->jump_frame < plr->jump_frames)))) {
            if (plr->jump_type == 2) {
              if (plr->obj.anim.anim_time >= MECHSTANDINGJUMPPAUSEFRAME) {
                plr->obj.anim.anim_time = MECHSTANDINGJUMPPAUSEFRAME;
              }
              plr->obj.anim.newaction = 100;
            }
            else {
              if (plr->obj.anim.anim_time >= MECHRUNNINGJUMPPAUSEFRAME) {
                plr->obj.anim.anim_time = MECHRUNNINGJUMPPAUSEFRAME;
              }
              plr->obj.anim.newaction = 99;
            }
        } else if (plr->land != 0) {
            if (plr->jump_type == 2) {
              plr->obj.anim.newaction = 100;
            } else {
              plr->obj.anim.newaction = 99;
            }
          }
          else {
            if ((plr->obj.pad_speed == 0.0f) && ((plr->pad_type != 1 || !(plr->obj.mom.x * plr->obj.mom.x + plr->obj.mom.z * plr->obj.mom.z >= plr->OnFootMoveInfo->IDLESPEED * plr->OnFootMoveInfo->IDLESPEED)))) {
              plr->obj.anim.newaction = 0x62;
            }
            else {
              if (plr->obj.pad_speed == minfo->WALKSPEED) {
                plr->obj.anim.newaction = 0x71;
              } else {
                plr->obj.anim.newaction = 0x68;
              }
            }
          }
      }
    }
  }
  UpdateCharacterIdle(plr,0);
  return;
}

//NGC MATCH
void AnimateSNOWBOARD(struct creature_s *plr) {
  s32 i;
  
  if (best_cRPos != NULL) {
    i = RotDiff(best_cRPos->angle,plr->obj.hdg);
  }
  else {
    i = 0;
  }
  if ((u32)plr->obj.dead > 1) {
    plr->obj.anim.newaction = plr->obj.die_action;
  }
  else {
    if ((plr->jump != 0) && ((plr->obj.ground == 0 || (plr->jump_frame < plr->jump_frames)))) {
      if (plr->obj.anim.anim_time >= SNOWBOARDJUMPPAUSEFRAME) {
        plr->obj.anim.anim_time = SNOWBOARDJUMPPAUSEFRAME;
      }
      plr->obj.anim.newaction = 99;
    }
    else {
      if (plr->land != 0) {
        plr->obj.anim.newaction = 99;
      }
      else {
        if (plr->sprint != 0) {
          plr->obj.anim.newaction = 0x6a;
        }
        else {
          if (i > 0x1000) {
            plr->obj.anim.newaction = 0x65;
          }
          else if (i < -0x1000) {
              plr->obj.anim.newaction = 0x67;
            } else {
              plr->obj.anim.newaction = 0x68;
            }
        }
      }
    }
  }
  UpdateCharacterIdle(plr,1);
  if (((plr->obj.anim.newaction == 0x68) || (plr->obj.anim.newaction == 0x6a)) || (plr->land != 0)) {
    AddGameDebrisRot(0x53,(struct nuvec_s *)&plr->mtxLOCATOR[8][0]._30,1,0,0);
    AddGameDebrisRot(0x53,(struct nuvec_s *)&plr->mtxLOCATOR[8][1]._30,1,0,0);
    AddGameDebrisRot(0x54,(struct nuvec_s *)&plr->mtxLOCATOR[8][2]._30,2,0,0);
  }
  else {
    if (plr->obj.anim.newaction == 0x65) {
        AddGameDebrisRot(0x55,(struct nuvec_s *)&plr->mtxLOCATOR[9][2]._30,3,0,0);
        return;
    } else if (plr->obj.anim.newaction == 0x67) {
        AddGameDebrisRot(0x55,(struct nuvec_s *)&plr->mtxLOCATOR[8][3]._30,3,0,0);
        return;
      }
  }
  return;
}

//NGC MATCH
void AnimateDIVE(struct creature_s *plr,float ratio) {
  if (ratio < 0.333f) {
    plr->obj.anim.newaction = 0x2c;
    return;
  }
  plr->obj.anim.newaction = 0x44;
  return;
}

//NGC MATCH
void ResetTubs(void) {
  struct cdata_s* cdata;
  struct nuvec_s *p0;
  struct nuvec_s *p1;
  struct creature_s* c;
  s32 i;
  
  ResetTub(&PlrTub,player);
  c = &OppTubCreature;
  ResetTub(&OppTub,c);
  memset(c,0,sizeof(struct creature_s));
  ResetAnimPacket(&c->obj.anim,0x62);
  c->obj.character = 0x7f;
  c->obj.vehicle = (CRemap[161] != -1) ? 0xa1 : -1;
  c->obj.reflect_y = 2000000.0f;
  if (CRemap[127] != -1) {
    c->obj.model = &CModel[CRemap[127]];
  }
  else {
    c->obj.model = NULL;
  }
  cdata = &CData[c->obj.character];
  
  c->obj.RADIUS = cdata->radius;
  c->obj.radius = cdata->radius;
  c->obj.min = cdata->min;
  c->obj.max = cdata->max;
  c->obj.SCALE = 0.77f;
  c->obj.scale = 0.77f;
  c->obj.bot = c->obj.min.y;
  c->obj.top = c->obj.max.y;
  c->used = 1;
  c->on = 1;
  ResetLights(&c->lights);
  OppTub.c = c;
  if ((SplTab[30].spl != NULL) && (SplTab[46].spl != NULL)) {
    p0 = (struct nuvec_s *)(SplTab[30].spl)->pts;
    p1 = (struct nuvec_s *)(SplTab[46].spl)->pts;
    c->obj.pos.x = (p0->x + p1->x) * 0.5f;
    c->obj.pos.y = (p0->y + p1->y) * 0.5f;
    c->obj.pos.z = (p0->z + p1->z) * 0.5f;
  }
  opptub_action = 0x62;
  i = qrand() * 0x3c;
  opptub_wait = (i / 0x10000) + 0x3c;
  return;
}