//NGC MATCH
void ResetCrateType2(CrateCube *crt) {
  crt->type2 = crt->type1;
  if (crt->model == NULL) {
    return;
  }
  crt->model->type[1] = crt->type1;
  return;
}

//NGC MATCH
struct crate_s * NextCrate(struct crate_s *a) {
  return (struct crate_s *)NuLstGetNext(crates,(struct nulnkhdr_s *)a);
}

//NGC MATCH
void InitCrates(void) {
  s32 iVar2;
  s32 iVar4;
  
  crates = NuLstCreate(0x100,0x28);
  CRATECOUNT = 0;
  CRATEGROUPCOUNT = 0;
  if (crate_scene != NULL) {
    for(iVar2 = 0; iVar2 < 28; iVar2++) {
      NuSpecialFind(crate_scene,(struct nuhspecial_s* )&crate_list[iVar2].obj,crate_list[iVar2].name);
    }
  }
  else {
    for(iVar2 = 0; iVar2 < 0x1d; iVar2++) {
      crate_list[iVar2].obj.special = 0;
    }
  }
  MarkerCrate.type[0] = 0x1a;
  marker_crate = &MarkerCrate;
  LockCrate.type[0] = 0x1b;
  lock_crate = &LockCrate;
  HighlightCrate.type[0] = 0x1c;
  highlight_crate = &HighlightCrate;
  FlashCrate.type[0] = (char)current_selected_crate;
  flash_crate = &FlashCrate;
  locked_crate = 0;
  triggerorigin_crate = 0;
  triggerdest_crate = 0;
  highlighted_crate = 0;
  return;
}

//NGC MATCH
void CloseCrates(void) {
  if (crates != NULL) {
    NuLstDestroy(crates);
    crates = NULL;
  }
  return;
}

//NGC MATCH
s32 ReadCrateData(void) {
  s32 handle;
  s32 i;
  s32 iVar7;
  s32 version;
  void * fbuff;
  
  CRATECOUNT = 0;
  sprintf(tbuf,"%s.crt",LevelFileName);
  if (NuFileExists(tbuf) != 0) {
    fbuff = Chase;
    handle = NuMemFileOpen(fbuff,NuFileLoadBuffer(tbuf,fbuff,0x7fffffff),NUFILE_READ);
    if (handle != 0) {
      version = NuFileReadInt(handle);
      if (version > 5) {
          NuFileClose(handle);
          return 0;
      }
      CRATEGROUPCOUNT = (s32)NuFileReadShort(handle);
      for(i = 0; i < CRATEGROUPCOUNT; i++) {
            CrateGroup[i].origin.x = NuFileReadFloat(handle);
            CrateGroup[i].origin.y = NuFileReadFloat(handle);
            CrateGroup[i].origin.z = NuFileReadFloat(handle);
            CrateGroup[i].radius = 0.0f;
            CrateGroup[i].iCrate = NuFileReadShort(handle);
            CrateGroup[i].nCrates = NuFileReadShort(handle);
            CRATECOUNT = CRATECOUNT + CrateGroup[i].nCrates;
            CrateGroup[i].angle = NuFileReadShort(handle);
            for (iVar7 = (s32)CrateGroup[i].iCrate; iVar7 < (s32)CrateGroup[i].iCrate + (s32)CrateGroup[i].nCrates; iVar7++) {
                Crate[iVar7].pos0.x = NuFileReadFloat(handle);
                Crate[iVar7].pos0.y = NuFileReadFloat(handle);
                Crate[iVar7].pos0.z = NuFileReadFloat(handle);
                Crate[iVar7].shadow = NuFileReadFloat(handle);
                Crate[iVar7].dx = NuFileReadShort(handle);
                Crate[iVar7].dy = NuFileReadShort(handle);
                Crate[iVar7].dz = NuFileReadShort(handle);
                Crate[iVar7].type1 = NuFileReadChar(handle);
                if (version > 2) {
                  Crate[iVar7].type2 = NuFileReadChar(handle);
                  Crate[iVar7].type3 = NuFileReadChar(handle);
                  Crate[iVar7].type4 = NuFileReadChar(handle);
                }
                else {
                  Crate[iVar7].type2 = -1;
                  Crate[iVar7].type3 = -1;
                  Crate[iVar7].type4 = -1;
                }
                Crate[iVar7].timer = 0.0f;
                Crate[iVar7].on = 1;
                Crate[iVar7].iU = NuFileReadShort(handle);
                Crate[iVar7].iD = NuFileReadShort(handle);
                Crate[iVar7].iN = NuFileReadShort(handle);
                Crate[iVar7].iS = NuFileReadShort(handle);
                Crate[iVar7].iE = NuFileReadShort(handle);
                Crate[iVar7].iW = NuFileReadShort(handle);
                if (version > 3) {
                  Crate[iVar7].trigger = NuFileReadShort(handle);
                }
                else {
                  Crate[iVar7].trigger = -1;
                }
            }
      }
      NuFileClose(handle);
      return 1;        
    }
  }
  return 0;
}

//NGC MATCH
s32 NewCrateAnimation(CrateCube* crate, s32 type, s32 action, s32 random) {
    struct CharacterModel* model;
    s32 i;
    s32 character;

    if ((u32)type > 0x14) {
        return 0;
    }
    
    character = crate_list[type].character;
    crate->character = -1;
    crate->action = -1;
    
    if (((u32)character > 0xbe) || ((u32)action > 0x75)) {
        return 0;
    }

    i = CRemap[character];
    if (i == -1) {
        return 0;
    }
    
    model = &CModel[i];
    if (model->anmdata[action] != NULL) {
        i = 1;
        crate->anim_duration = model->anmdata[action]->time;
        crate->anim_cycle = model->animlist[action]->flags & 1;
        crate->anim_speed = model->animlist[action]->speed;
    } else if (model->fanmdata[action] != NULL) {
        i = 1;
        crate->anim_duration = model->fanmdata[action]->time;
        crate->anim_cycle = model->fanimlist[action]->flags & 1;
        crate->anim_speed = model->fanimlist[action]->speed;
    } else {
        i = 0;
    }

    if (i != 0) {
        crate->anim_time = 1.0f;
        if (random != 0) {
            crate->anim_time = (s32)qrand() * 0.000015259022f * (crate->anim_duration - 1.0f) + crate->anim_time;
            if (crate->anim_time >= crate->anim_duration) {
                crate->anim_time = 1.0f;
            }
        }
        crate->character = character;
        crate->action = action;
    }
    return i;
}

//NGC MATCH
void OpenPreviousCheckpoints(s32 iRAIL,s32 iALONG,float fALONG) {
  CrateCubeGroup *group;
  CrateCube *crate;
  s32 i;
  s32 j;

  group = CrateGroup;
  for(i = 0; i < CRATEGROUPCOUNT; i++, group++) {
      crate = &Crate[group->iCrate];
      for(j = 0; j < group->nCrates; j++, crate++) {
          if ((((crate->on != 0) && (crate->type1 == 7)) && (crate->iRAIL != -1)) &&
             (FurtherBEHIND(crate->iRAIL,crate->iALONG,crate->fALONG,iRAIL,iALONG,fALONG) != 0)) {
            crate->on = 0;
            if (crate->model != NULL) {
              crate->model->draw = 0;
            }
            NewCrateAnimation(crate,7,0x34,0);
          }
      }
  }
  return;
}

//NGC MATCH
void HeightSortCrateData(void) {
  s32 i;
  s32 j;
  s32 l;
  s32 k;
  CrateCube tcr;
  CrateCubeGroup* crg;

  for(i = 0; i < CRATEGROUPCOUNT; i++) {
   crg = &CrateGroup[i];
      for(j = crg->iCrate; j < (s32)crg->iCrate + (s32)crg->nCrates - 1; j++) {
          for(k = j + 1; k < (crg->iCrate + crg->nCrates); k++) {
              if (Crate[j].dy > Crate[k].dy) {
                tcr = Crate[k];
                Crate[k] = Crate[j];
                Crate[j] = tcr;
                for(l = 0; l < CRATECOUNT; l++) {
                    if (Crate[l].trigger == j) {
                      Crate[l].trigger = k;
                    }
                    else if (Crate[l].trigger == k) {
                      Crate[l].trigger = j;
                    }
                }
              }
          }
      }
  }
  return;
}

//NGC MATCH
void StartExclamationCrateSequence(CrateCubeGroup *group,CrateCube *crate) {
  struct nuvec_s pos;
  
  pos.x = (crate->pos).x;
  pos.y = (crate->pos).y + 0.25f;
  pos.z = (crate->pos).z;
  temp_pGroup = group;
  temp_pCrate = crate;
  AddKaboom(0x20,&pos,0.0f);
  crate->newtype = 0xf;
  crate->metal_count = 1;
  GameSfx(0x35,&temp_pCrate->pos);
  return;
}

//NGC MATCH
void SaveCrateTypeData(CrateCube *crate) {
  CRATETYPEDATA *data;
  
  if (i_cratetypedata > 0x1f) {
    return;
  }
  data = &CrateTypeData[i_cratetypedata];
  data->crate = crate;
  data->type1 = crate->type1;
  data->type2 = crate->type2;
  data->type3 = crate->type3;
  data->type4 = crate->type4;
  i_cratetypedata++;
  return;
}

//NGC MATCH
void RestoreCrateTypeData(void) {
  s32 i;
  CRATETYPEDATA *data;
  
  data = CrateTypeData;
  for(i = 0; i < i_cratetypedata; i++, data++) {
      data->crate->type1 = data->type1;
      data->crate->type2 = data->type2;
      data->crate->type3 = data->type3;
      data->crate->type4 = data->type4;
  }
  i_cratetypedata = 0;
  return;
}

//NGC MATCH
void AddExtraLife(struct nuvec_s *pos,int pdeb) {
  struct nuvec_s scr_pos;
  struct nuvec_s cV[2];
  float scale;
  
  NuCameraTransformScreenClip(&scr_pos,pos,1,NULL);
  cV[0].x = GameCam[0].vX.x * 0.1f + pos->x;
  cV[0].y = GameCam[0].vX.y * 0.1f + pos->y;
  cV[0].z = GameCam[0].vX.z * 0.1f + pos->z;
  NuCameraTransformScreenClip(&cV[1],&cV[0],1,NULL);
  scale = NuFabs((scr_pos.x - cV[1].x));
  AddPanelDebris(scr_pos.x,scr_pos.y,pdeb,(scale * 3.6363637f),1);
  return;
}

//NGC MATCH
s32 GetCrateType(CrateCube* crate, s32 flags) {
    u32 type;

    if (TimeTrial != 0) {
        type = crate->type2;
    } else {
        type = crate->type1;
    }
    
    if (type == 0 && crate->newtype != -1) {
        type = crate->newtype;
    }
    
    if ((flags & 2) && type == 8) {
        if (crate->newtype != -1) {
            goto block_12;
        }
    }
    else if (crate->newtype != -1) {
block_12:
        type = (TimeTrial == 0 || type == 9) ? (crate->subtype != -1 ? crate->subtype : crate->newtype) : crate->newtype;
    } else if (TimeTrial == 0 || type == 9) {
        type = (crate->subtype != -1) ? crate->subtype : type;
    }

    if (!(flags & 1)) {
        if (type == 22 || type == 23 || type == 24) {
            type = 9;
        }
    }
        
    else if (type == 2) {
        if (LDATA->character == 1) {
            type = 25;
        }
    }
    
    return type;
}

//NGC MATCH
s32 CrateAbove(struct obj_s *obj,CrateCubeGroup *group,CrateCube *crate) {
  CrateCube *crate2;
  s32 i;
  
  crate2 = Crate + group->iCrate;
  for(i = 0; i < group->nCrates; i++, crate2++) {
        if ( ( ((crate2 != crate) && (crate2->on != 0)) && (GetCrateType(crate2,0) != 0) ) &&
              ((crate2->dx == crate->dx) && (crate2->dz == crate->dz)) ) {
            if (crate2->pos.y > crate->pos.y && ( !(obj->objtop < crate2->pos.y) 
                && !(obj->objbot > (crate2->pos.y + 0.5f)) )) {
                 return 1;
              }
        }
  }
  return 0;
}

//NGC MATCH
s32 CrateBelow(struct obj_s *obj,CrateCubeGroup *group,CrateCube *crate) {
  s32 i;
  CrateCube *crate2;
  
  crate2 = Crate + group->iCrate;
  for(i = 0; i < group->nCrates; i++, crate2++) {
      if (((((crate2 != crate) && (crate2->on != 0)) && (GetCrateType(crate2,0) != 0)) &&
          ((crate2->dx == crate->dx && (crate2->dz == crate->dz)))) && ((crate2->pos.y < crate->pos.y &&
          (!(obj->objtop < crate2->pos.y || (obj->objbot > (crate2->pos.y + 0.5f) ) ) ) )) ) {
        return 1;
      }
  }
  return 0;
}

//NGC MATCH
s32 CrateOff(CrateCubeGroup *group,CrateCube *crate,s32 kaboom,s32 chase) {
  struct nuvec_s pos;
  s32 type;
  s32 i;
  s32 sfx;
  float fVar2;
  
  type = GetCrateType(crate,0);
  if ((((type == -1) || (type - 0xdU < 2)) || (type == 0)) || ((type == 0xf || (type == 0x11) ))) {
    return 0;
  }
  crate->on = 0;
  if (crate->model != NULL) {
    crate->model->draw = 0;
  }
  pos.x = (crate->pos).x;
  pos.y = (crate->pos).y + 0.25f;
  pos.z = (crate->pos).z;
  if ((type != 7) && (crate->in_range != 0)) {
    AddCrateExplosion(&crate->pos,type,group->angle,crate->colbox);
  }
  sfx = 0x25;
  switch (type) {
      case 2:
          if ((((kaboom & 3U) == 0) && ((player->obj).dead == 0)) && (chase == 0)) {
            i = 2;
            if ((crate->flags & 0x40) != 0) {
              i = 3;
            }
            AddExtraLife(&pos,i);
            if ((TimeTrial == 0) && ((crate->flags & 0x40) == 0)) {
              if ((crate->type1 == '\b') && (0 < crate->i)) {
                SaveCrateTypeData(crate);
                if (crate->i == 1) {
                  crate->type3 = crate->type4;
                }
                crate->type4 = -1;
              }
              else if (crate->type1 == 2) {
                SaveCrateTypeData(crate);
                crate->type1 = 5;
              }
              else if ((crate->type1 == 0) && (crate->type3 == 2)) {
                SaveCrateTypeData(crate);
                crate->type3 = 5;
              }
            }
          }
      break;
      case 7:
          sfx = 0x17;
          ResetCheckpoint((s32)crate->iRAIL,(s32)crate->iALONG,crate->fALONG,&crate->pos);
          NewCrateAnimation(crate,7,0x34,0);
      break;
      case 3:
          if ((((kaboom & 3U) == 0) && ((player->obj).dead == 0)) && (chase == 0)) {
            mask_crates++;
            vNEWMASK = crate->model->pos;
            newmask_advise = 0;
          }
      break;
      case 6:
      break;
      case 8:
      break;
      case 0x10:
          AddKaboom(2,&pos,0.0f);
          i = 6;
          sfx = 0x3b;
          AddGameDebris(i,&pos);
      break;
      case 9:
            AddKaboom(1,&pos,0.0f);
            i = 5;
            sfx = 0x3b;
            AddGameDebris(i,&pos);
            
      break;
      case 0xc:
          if (chase != 0) break;
          fVar2 = 3.0f;
            TimeTrialWait += fVar2;
      break;
      case 0xb:
        if (chase != 0) break;
        fVar2 = 2.0f;
        TimeTrialWait += fVar2;
      break;
      case 10:
        if (chase != 0) break;
        fVar2 = 1.0f;
        TimeTrialWait += fVar2;
      break;
      case 0x14:
          if ((chase == 0) && ((kaboom == 0 || ((kaboom & 0xcU) != 0)))) {
            plr_invisibility_time = 0.0f;
            GameSfx(0x1e,NULL);
          } 
      break;
      default:
          if ((((kaboom & 3U) == 0) && (chase == 0)) &&
             ((TimeTrial == 0 && (((player->obj).dead == 0 && ((crate->flags & 0x400) == 0)))))) {
              if (type == 5) {
                if (last_questionmark_extralife == 0) {
                    i = last_questionmark_extralife;
                  if (((Game.lives < 10) && ((crate->flags & 0x40) == 0)) &&
                     (qrand() < (s32)(0x4000 - (Game.lives << 0xe) / 10))) {
                    AddExtraLife(&pos,2);
                    last_questionmark_extralife = 1;
                    break;
                  }
                }
              }
              if (last_questionmark_extralife != 0) last_questionmark_extralife--;
              if (CrateOnTop(group,crate) != 0) {
                i = 1;
                AddScreenWumpa(pos.x,pos.y,pos.z,i);
              }
              else {
                if (type != 0x13) {
                  i = qrand() / 0x3334 + 1;
                }
                else {
                  i = 1;
                }
                if (((crate->flags & 0x1000) != 0) || ((VEHICLECONTROL == 1 && ((player->obj).vehicle != -1))) ) {
                  AddScreenWumpa(pos.x,pos.y,pos.z,i);
                } else {
                    AddTempWumpa(pos.x,pos.y,pos.z,crate,i);
                }
              }
        }
      break;
  }
  GameSfx(sfx,((crate->flags & 0x400) == 0) ? &pos : NULL);
  return 1;
}

//NGC MATCH
CrateCube * CrateInSlot(CrateCubeGroup *group,s32 x,s32 y,s32 z) {
  CrateCube *crate;
  s32 i;
  
  crate = &Crate[group->iCrate];
  for(i = 0; i < group->nCrates; i++, crate++) {
      if (((crate->dx == x) && (crate->dy == y)) && (crate->dz == z)) {
        return crate;
      }
  }
  return NULL;
}

//NGC MATCH
void ResetInvisibility(void) {
  plr_invisibility_time = 5.0f;
  glass_mix = (Level != 0x17) ? 0.0f : WATERBOSSGLASSMIX;
  glass_col_mix = 0;
  glass_col_enabled = 0;
  glass_enabled = 0;
  return;
}

//NGC MATCH
void ResetCrate(CrateCube *crt) {
  crt->mom = 0.0f;
  crt->oldy = crt->pos0.y;
  crt->pos.y = crt->pos0.y;
  crt->newtype = -1;
  crt->subtype = -1;
  if (((crt->type1 == 6) || (crt->type2 == 6)) || ((crt->type1 == 0 && (crt->type3 == 6)))) {
    crt->counter = 0xa;
  }
  else {
    crt->counter = 0;
  }
  crt->metal_count = 0;
  crt->action = -1;
  crt->appeared = 0;
  return;
}

//NGC MATCH
void InitCrateExplosions(void) {
  s32 i;
  
  iBOXEXP = 0;
  for(i = 0; i < 0x10; i++) {
    BoxExpList[i].time = 0;
  }
  return;
}

//NGC MATCH
void UpdateCrateExplosions(void) {
  BoxExpType *box;
  BoxPolType *face;
  s32 i;
  s32 j;
  s32 k;
  s32 bVar6;
  
  box = BoxExpList;
  for(i = 0; i < 0x10; i++, box++) {
    if (box->time != 0) {
      box->time--;
      if (box->time != 0) {
          face = BoxExpList[i].BoxPol;
          for(j = 0; j < 5; j++) {
                for(k = 0; k < 6; k++, face++) {
                  bVar6 = 0;
                  face->pos.x = face->pos.x + face->mom.x;
                  face->pos.y = face->pos.y + face->mom.y;
                  face->mom.y = face->mom.y - 0.004f;
                  face->pos.z = face->pos.z + face->mom.z;
                  (face->ang).x = (face->ang).x + face->angmom.x;
                  (face->ang).y = (face->ang).y + face->angmom.y;
                  (face->ang).z = (face->ang).z + face->angmom.z;
                  if ((face->pos.x < box->colbox[0].x) && (face->mom.x < 0.0f)) {
                    face->pos.x = box->colbox[0].x;
                    bVar6 = 1;
                    face->mom.x = -face->mom.x * 0.8f;
                  }
                  if ((face->pos.y < box->colbox[0].y) && (face->mom.y < 0.0f)) {
                    face->pos.y = box->colbox[0].y;
                    bVar6 = 1;
                    face->mom.y = -face->mom.y * 0.8f;
                  }
                  if ((face->pos.z < box->colbox[0].z) && (face->mom.z < 0.0f)) {
                    face->pos.z = box->colbox[0].z;
                    bVar6 = 1;
                    face->mom.z = -face->mom.z * 0.8f;
                  }
                  if (face->pos.x > (box->colbox[1].x) && (0.0f < face->mom.x)) {
                    face->pos.x = box->colbox[1].x;
                    bVar6 = 1;
                    face->mom.x = -face->mom.x * 0.8f;
                  }
                  if (face->pos.z > (box->colbox[1].z) && (0.0f < face->mom.z)) {
                    face->pos.z = box->colbox[1].z;
                    bVar6 = 1;
                    face->mom.z = -face->mom.z * 0.8f;
                  }
                  if (bVar6) {
                    face->angmom.x = -face->angmom.x;
                    face->angmom.y = -face->angmom.y;
                    face->angmom.z = -face->angmom.z;
                  }
                }
          }
      }
    }
  }
  return;
}

//NGC MATCH
float CrateTopBelow(struct nuvec_s *pos) {
  CrateCubeGroup *group;
  CrateCube *crate;
  s32 i;
  s32 j;
  float top;
  float y;
  struct nuvec_s vNew;
  
  y = 2000000.0f;
  group = CrateGroup;
  for(i = 0; i < CRATEGROUPCOUNT; i++, group++) {
      if (((!(pos->x < group->minclip.x) && !(pos->x > (group->maxclip).x)) &&
          !(pos->z < group->minclip.z)) && !(pos->z > (group->maxclip).z)) {
        vNew.x = pos->x - (group->origin).x;
        vNew.z = pos->z - (group->origin).z;
        NuVecRotateY(&vNew,&vNew,-(u32)group->angle);
        crate = &Crate[group->iCrate];
        for (j = 0; j < group->nCrates; j++, crate++) {
            if (((((crate->on != 0) && ( GetCrateType(crate,0) + 1U > 1)) &&
                 ((top = (crate->pos.y + 0.5f), !(pos->y < top) &&
                  ( !(vNew.x < (crate->dx * 0.5f) || (vNew.x > (crate->dx * 0.5f) + 0.5f))))))
                && !(vNew.z < crate->dz * 0.5f)) &&
               (!(vNew.z > (crate->dz * 0.5f) + 0.5f || ((y != 2000000.0f && !(top > y)))))) {
              y = top;
            }
        }
      }
  }
  return y;
}

//NGC MATCH
void DestroyAllNitroCrates(CrateCubeGroup *group,CrateCube *crate) {
  CrateCubeGroup *group2;
  CrateCube *crate2;
  struct nuvec_s pos;
  s32 i;
  s32 j;
  
  group2 = CrateGroup;
  for(i = 0; i < CRATEGROUPCOUNT; i++, group2++) {
      crate2 = Crate + group2->iCrate;
      for(j = 0; j < group2->nCrates; j++, crate2++) {
          if ((crate2->on != 0) && (GetCrateType(crate2,0) == 0x10)) {
            CrateOff(group2,crate2,0,0);
          }
      }
  }
  pos.x = (crate->pos).x;
  pos.y = (crate->pos).y + 0.25f;
  pos.z = (crate->pos).z;
  temp_pGroup = group;
  temp_pCrate = crate;
  AddKaboom(0x20,&pos,0.0f);
  crate->newtype = 0xf;
  crate->metal_count = 1;
  JudderGameCamera(GameCam,0.5f,NULL);
  GameSfx(0x33,&crate->pos);
  return;
}

//NGC MATCH
s32 AttackCrate(struct obj_s *obj,CrateCubeGroup *group,CrateCube *crate) {
  s32 type;
  
  type = GetCrateType(crate,0);
  if (type == -1) {
    return 1;
  }
    if (type == 0xe) {
      if ((crate->action == -1) && (NewCrateAnimation(crate,0xe,0x35,0) == 0)) {
        StartExclamationCrateSequence(group,crate);
      }
      return 2;
    }
    if (type == 0x11) {
        if ((crate->action == -1) && (NewCrateAnimation(crate,0x11,0x35,0) == 0)) {
            DestroyAllNitroCrates(group,crate);
        }
        return 2;
    }
    if (CrateOff(group,crate,0,obj->attack >> 9 & 1) != 0) {
        if ((type == 9) || (type == 0x10)) {
                KillPlayer(obj,GetDieAnim(obj,6));   
        }
        return 1;
    }
  return 0;
}

//NGC MATCH
s32 LowestCrate(CrateCubeGroup *group,CrateCube *crate) {
  CrateCube *crate2;
  s32 i;
  s32 dx;
  s32 dz;
  
  crate2 = &Crate[group->iCrate];
  dx = crate->dx;
  dz = crate->dz;
  for(i = 0; i < group->nCrates; i++, crate2++) {
      if ((((crate2 != crate) && (crate2->dx == dx)) && (crate2->dz == dz)) &&
         ((crate2->pos0).y < (crate->pos0).y)) {
        return 0;
      }
  }
  return 1;
}

//NGC MATCH
s32 LowestActiveCrate(CrateCubeGroup *group,CrateCube *crate) {
  CrateCube *crate2;
  s32 i;
  s32 dx;
  s32 dz;
  
  crate2 = &Crate[group->iCrate];
  dx = crate->dx;
  dz = crate->dz;
  for(i = 0; i < group->nCrates; i++, crate2++) {
      if ((((crate2 != crate) && (crate2->on != 0)) && (crate2->dx == dx)) &&
         ((crate2->dz == dz && ((crate2->pos).y < (crate->pos).y)))) {
        return 0;
      }
  }
  return 1;
}

//NGC MATCH
s32 CrateInTheWay(struct obj_s *obj,CrateCubeGroup *group,CrateCube *crate,s32 dx,s32 dz,char *hit) {
  CrateCube *crate2;
  s32 i;
  
  crate2 = &Crate[group->iCrate];
  for(i = 0; i < group->nCrates; i++, crate2++) {
      if ((((crate2 != crate) && (hit[i] == 1)) && (crate2->dx == dx)) &&
         (crate2->dz == dz)) {
        return 1;
      }
  }
  return 0;
}

//NGC MATCH
s32 CrateOnTop(CrateCubeGroup *group,CrateCube *crate) {
  s32 i;
  CrateCube *crate2;
  
  crate2 = &Crate[group->iCrate];
  for(i = 0; i < group->nCrates; i++, crate2++) {
      if ((((crate2 != crate) && (crate2->on != 0)) && (GetCrateType(crate2,0) != 0)) &&
         (((crate2->dx == crate->dx && (crate2->dz == crate->dz)) &&
          (crate2->pos.y == crate->pos.y + 0.5f)))) {
        return 1;
      }
  }
  return 0;
}

//NGC MATCH
void BreakCrate(CrateCubeGroup *group,CrateCube *crate,s32 type,s32 attack) {
  if ((type == 0x13) && ((attack & 0x200U) == 0)) {
    if (crate->action == -1) {
      NewCrateAnimation(crate,0x13,0x58,0);
      GameSfx(0x39,&crate->pos);
    }
  }
  else if (type == 0xe) {
    if ((crate->action == -1) && (NewCrateAnimation(crate,0xe,0x35,0) == 0)) {
      StartExclamationCrateSequence(group,crate);
    }
  }
  else if (type == 0x11) {
    if ((crate->action == -1) && (NewCrateAnimation(crate,0x11,0x35,0) == 0)) {
      DestroyAllNitroCrates(group,crate);
    }
  }
  else {
    if (CrateOff(group,crate,0,(attack >> 9) & 1) != 0) {
      HopCratesAbove(CRATEHOPSPEED,group,crate);
    }
  }
  return;
}

//NGC MATCH
s32 HitCrates(struct obj_s *obj,s32 destroy) {
  if ((level_part_2 == 0) &&
     (InCrate(obj->pos.x,obj->pos.z,(obj->top * obj->SCALE + obj->pos.y),(obj->bot * obj->SCALE + obj->pos.y),obj->RADIUS) != 0)) {
        if ((destroy == 1) || ((((destroy == 2 && (temp_crate_type != 7)) && (temp_crate_type != 0xe)) && (temp_crate_type != 0x11)))) {
          BreakCrate(temp_pGroup,temp_pCrate,temp_crate_type,(uint)obj->attack);
        }
        return 1;
  }
  else {
    return 0;
  }
}

//NGC MATCH
s32 WipeCrates(s32 iRAIL0,s32 iALONG0,float fALONG0,s32 iRAIL1,s32 iALONG1,float fALONG1,s32 destroy) {
  CrateCubeGroup *group;
  CrateCube *crate;
  s32 i;
  s32 j;
  s32 type;

  group = CrateGroup;
  for(i = 0; i < CRATEGROUPCOUNT; i++, group++) {
      crate = &Crate[group->iCrate];
      for(j = 0; j < group->nCrates; j++, crate++) {
        if (crate->on != 0) {
        type = GetCrateType(crate,0);
            if((u32)(type + 1) > 1 && 
                (FurtherALONG(crate->iRAIL,crate->iALONG,crate->fALONG,iRAIL0,iALONG0,fALONG0) != 0) &&
                  (FurtherALONG(iRAIL1,iALONG1,fALONG1,crate->iRAIL,crate->iALONG,crate->fALONG) != 0) &&
                 ((destroy == 1) || ((destroy == 2 && (type != 7)) && (type != 0xe && (type != 0x11))))) {
                BreakCrate(group,crate,type,0x200);
            }
        }
      }
  }
  return 0;
}

//NGC MATCH
CrateCube * HitCrateBalloons(struct nuvec_s *pos,float radius) {
  CrateCubeGroup *group;
  CrateCube *crate;
  float r2;
  s32 iVar1;
  s32 iVar3;
  struct nuvec_s v;
  struct nuvec_s d;
  
  temp_pGroup = NULL;
  temp_pCrate = NULL;
  if (level_part_2 != 0) {
    return temp_pCrate;
  }
    group = CrateGroup;
    r2 = (radius + CRATEBALLOONRADIUS);
    r2 *= r2;
    for(iVar1 = 0; iVar1 < CRATEGROUPCOUNT; iVar1++, group++) {
        crate = Crate + group->iCrate;
        for(iVar3 = 0; iVar3 < group->nCrates; iVar3++, crate++) {
            if ((crate->on != 0) && ((crate->flags & 0x400) != 0)) {
              v.x = (crate->pos).x;
              v.y = (crate->pos).y + CRATEBALLOONOFFSET;
              v.z = (crate->pos).z;
              NuVecSub(&d,pos,&v);
              if ((d.x * d.x + d.y * d.y + d.z * d.z) < r2) {
                temp_crate_type = GetCrateType(crate,0);
                temp_pGroup = group;
                temp_pCrate = crate;
                GameSfx(0x50,&v);
                return temp_pCrate;
              }
            }
        }
    }
  return temp_pCrate;
}

//NGC MATCH
s32 RayIntersectCuboid(struct nuvec_s *p0,struct nuvec_s *p1,struct nuvec_s *min,struct nuvec_s *max) {
  float new_ratio;
  float dx;
  float dy;
  float dz;
  s32 face;
  struct nuvec_s v;
  
  dx = (p1->x - p0->x);
  dy = (p1->y - p0->y);
  dz = (p1->z - p0->z);
  if ((p0->x <= min->x) && (p1->x >= min->x)) {
    new_ratio = ((min->x - p0->x) / dx);
    face = 8;
    v.x = min->x;
  }
  else {
    if ((p0->x >= max->x) && (p1->x <= max->x)) {
      new_ratio = ((max->x - p0->x) / dx);
      face = 4;
      v.x = max->x;
    }
    else {
      face = 0;
    }
  }
  if (face != 0) {
    v.z = (dz * new_ratio + p0->z);
    if ((v.z >= min->z) && (v.z <= max->z)) {
        v.y = (dy * new_ratio + p0->y);
         if ((v.y >= min->y) && (v.y <= max->y)) {
            temp_ratio = new_ratio;
            temp_face = face;
            vTEMP = v;
            return 1;
        }
    }
  }
    if ((p0->z <= min->z) && (p1->z >= min->z)) {
      new_ratio = ((min->z - p0->z) / dz);
      face = 2;
      v.z = min->z;
    }
    else {
      if ((p0->z >= max->z) && (p1->z <= max->z)) {
        new_ratio = ((max->z - p0->z) / dz);
        face = 1;
        v.z = max->z;
      }
      else {
        face = 0;
      }
    }
    if (face != 0) {
      v.x = (dx * new_ratio + p0->x);
      if ((v.x >= min->x) && (v.x <= max->x)) {
        v.y = (dy * new_ratio + p0->y);
        if ((v.y >= min->y) && (v.y <= max->y)) {
            temp_ratio = new_ratio;
            temp_face = face;
            vTEMP = v;
            return 1;
        }
      }
    }
    if ((p0->y <= min->y) && (p1->y >= min->y)) {
      new_ratio = ((min->y - p0->y) / dy);
      face = 0x20;
      v.y = min->y;
    }
    else {
      if ((p0->y >= max->y) && (p1->y <= max->y)) {
        new_ratio = ((max->y - p0->y) / dy);
        face = 0x10;
        v.y = max->y;
      }
      else {
        face = 0;
      }
    }
    if (face != 0) {
      v.x = (dx * new_ratio + p0->x);
      if ((v.x >= min->x) && (v.x <= max->x)) {
        v.z = (dz * new_ratio + p0->z);
        if ((v.z >= min->z) && (v.z <= max->z)) {
            temp_ratio = new_ratio;
            temp_face = face;
            vTEMP = v;
            return 1;
        }
      }
    }
  return 0;
}

//NGC MATCH
s32 CrateRayCast(struct nuvec_s *p0,struct nuvec_s *p1) {
  struct nuvec_s vMIN;
  struct nuvec_s vMAX;
  struct nuvec_s v0;
  struct nuvec_s v1;
  struct nuvec_s min;
  struct nuvec_s max;
  s32 i;
  s32 j;
  s32 face;
  s32 type;
  float ratio;
  CrateCubeGroup *group;
  CrateCube *crate;
  
  ratio = 1.0f;
  vMIN.x = (p0->x < p1->x) ? p0->x : p1->x;
  vMIN.z = (p0->z < p1->z) ? p0->z : p1->z;
  vMAX.x = (p0->x > p1->x) ? p0->x : p1->x;
  vMAX.z = (p0->z > p1->z) ? p0->z : p1->z;
  group = CrateGroup;
  for(i = 0; i < CRATEGROUPCOUNT; i++, group++) {
      if ((((vMAX.x >= group->minclip.x) && (vMIN.x <= group->maxclip.x)) &&
          (vMAX.z >= group->minclip.z )) && (vMIN.z <= group->maxclip.z)) {
        v0.x = p0->x - (group->origin).x;
        v0.y = p0->y;
        v0.z = p0->z - (group->origin).z;
        NuVecRotateY(&v0,&v0,-(uint)group->angle);
        v1.x = p1->x - (group->origin).x;
        v1.y = p1->y;
        v1.z = p1->z - (group->origin).z;
        NuVecRotateY(&v1,&v1,-(uint)group->angle);
        crate = &Crate[group->iCrate];
        for(j = 0; j < group->nCrates; j++, crate++) {
           if ((crate->on != 0) && (crate->in_range != 0)) {
                type = GetCrateType(crate,0) + 1;
                 if ((u32)type > 1) {
                  min.x = ((s32)crate->dx * 0.5f);
                  min.y = crate->pos.y;
                  min.z = (crate->dz * 0.5f);
                  max.x = (min.x + 0.5f);
                  max.y = min.y + 0.5f;
                  max.z = (min.z + 0.5f);
                  if ((RayIntersectCuboid(&v0,&v1,&min,&max) != 0) && (temp_ratio < ratio)) {
                    face = temp_face;
                    ratio = temp_ratio;
                  }
                }
            }
        }
      }
  }
  temp_face = face;
  temp_ratio = (float)ratio;
  if (ratio < 1.0f) {
      return 1;
  }
  return 0;
}

//NGC MATCH
s32 GotoCheckpoint(struct nuvec_s *pos,s32 direction) {
  CrateCubeGroup *group;
  CrateCube *crate;
  s32 i;
  s32 j;
  s32 iRAIL;
  s32 iALONG;
  float fALONG;
  
  cp_goto = -1;
  if ((best_cRPos == NULL) || ((u32)direction > 1)) {
      return 0;
  }
    group = CrateGroup;
    iRAIL = -1;
    for(i = 0; i < CRATEGROUPCOUNT; i++, group++) {
        crate = Crate + group->iCrate;
        for(j = 0; j < group->nCrates; j++, crate++) {
            if ((((crate->type1 == 7) && (NuVecDist(pos,&crate->pos0,NULL) > 5.0f)) && ((((direction == 0) &&
                  (FurtherALONG(crate->iRAIL,crate->iALONG,crate->fALONG,
                                        best_cRPos->iRAIL,best_cRPos->iALONG,
                                        best_cRPos->fALONG) != 0)) || ((direction == 1 &&
                  (FurtherBEHIND(crate->iRAIL,crate->iALONG,crate->fALONG,
                                         best_cRPos->iRAIL,best_cRPos->iALONG,
                                         best_cRPos->fALONG) != 0))))))
                && ((iRAIL == -1 || (((direction == 0 &&
                  (FurtherBEHIND(crate->iRAIL,crate->iALONG,crate->fALONG,iRAIL,iALONG,fALONG) != 0)) ||
                 ((direction == 1 &&
                   (FurtherALONG(crate->iRAIL,crate->iALONG,crate->fALONG,iRAIL,iALONG,fALONG) != 0)))))))) {
              cpGOTO.x = crate->pos.x;
              cpGOTO.y = (crate->pos0.y + 0.5f) + 1.0f;
              cpGOTO.z = crate->pos.z;
              iRAIL = crate->iRAIL;
              iALONG = crate->iALONG;
              fALONG = crate->fALONG;
            }
        }
    }
    if (iRAIL != -1) {
        cp_goto = direction;
        return 1;
    }
    return 0;
}

//NGC MATCH (temp var)
s32 CrateBounceReaction(CrateCubeGroup *group, CrateCube *crate, s32 type, s32 hit) {
    s32 gone;
    s32 sfx;
    s32 bounce;

    bounce = 0;
    gone = 0;
    sfx = -1;

    if (type != 0xf) {
        if (type == 9) {
            if ((crate->timer > 0.0f) && (crate->newtype != -1)) {
                goto LAB_80013a4c;
            }
            if (((TimeTrial == 0) ? crate->type1 : crate->type2) == 8) {
                if ((crate->newtype == -1) && (crate->subtype == 0x09)) {
                    sfx = 2;
                    bounce |= 1;
                    crate->newtype = 9;
                    crate->timer = 0.01666667f;
                    goto LAB_80013a4c;
                }
            }
            if (type == 9) {
                if (crate->timer == 0.0f) {
                    sfx = 2;
                    bounce |= 1;
                    crate->timer += 0.01666667f;
                }
                goto LAB_80013a4c;
            }
        }
        
        if (type == 0xe) {
            if (crate->action == -1) {
                sfx = 0xe;
                bounce |= 1;
                if (NewCrateAnimation(crate, 0xe, 0x35, 0) == 0) {
                    StartExclamationCrateSequence(group, crate);
                }
            }
        } else if (type == 0x11) {
            if (crate->action == -1) {
                sfx = 0xe;
                bounce |= 1;
                if (NewCrateAnimation(crate, 0x11, 0x35, 0) == 0) {
                    DestroyAllNitroCrates(group, crate);
                }
            }
        } else if (type == 0x13) {
            NewCrateAnimation(crate, 0x13, 0x58, 0);
            GameSfx(0x38, &crate->pos);
        } else if (type == 6) {
            sfx = 2;
            if (crate->timer == 0.0f) {
                crate->timer += 0.01666667f;
            }
            if (crate->counter > 1) {
                crate->counter--;
                if ((TimeTrial == 0) && ((player->obj).dead == 0)) {
                    AddScreenWumpa((crate->pos).x, (crate->pos).y + 0.25f, (crate->pos).z, 2);
                }
                NewCrateAnimation(crate, type, (hit == 2) ? 0x58 : 0x16, 0);
            } else {
                float f = crate->timer;
                if (((crate->flags & 0x20) != 0 && (f < 2.5f)) ||
                    ((crate->flags & 0x20) == 0 && (f < 5.0f))
                   ) {
                    if ((TimeTrial == 0) && ((player->obj).dead == 0)) {
                        AddScreenWumpa((crate->pos).x, (crate->pos).y + 0.25f, (crate->pos).z, 2);
                    }
                }
                gone = CrateOff(group, crate, 0, 0);
            }
            bounce |= 1;
        } else {
            if ((type == 4) || (type == 0xd)) {
                NewCrateAnimation(crate, type, 0x58, 0);
                bounce |= 3;
                sfx = 0xe;
                if (type == 4) {
                    sfx = 2;
                }
            } else {
                gone = CrateOff(group, crate, 0, 0);
                sfx = 2;
                bounce |= 1;
            }
        }
    }

LAB_80013a4c:
    if (VEHICLECONTROL != 2) {
        temp_crate_bounce = temp_crate_bounce | bounce;
        if (bounce != 0) {
            NewRumble(&player->rumble, 0x7f);
            NewBuzz(&player->rumble, 0xc);
        }
    }

    if (sfx != -1) {
        GameSfx(sfx, &crate->pos);
    }

    return gone;
}

//NGC MATCH
void CrateSafety(CrateCubeGroup *group,CrateCube *crate,struct obj_s *obj) {
  CrateCube *crate2;
  CrateCube *crate3;
  float size;
  s32 i;
  
  if ((Level == 7) && (Bonus == 2)) {
    obj->pos.y = crate->pos.y - obj->top * obj->SCALE;
  }
  else {
    size = (obj->top - obj->bot) * obj->SCALE;
    if ((obj->bot + obj->top) * obj->SCALE * 0.5f + obj->pos.y < crate->pos.y + 0.25f) {
      crate2 = &Crate[group->iCrate];
      crate3 = crate;
      for(i = 0; i < group->nCrates; i++, crate2++) {
          if (((crate2->on != 0) && (crate2->dx == crate3->dx)) && (crate2->dz == crate3->dz)) {
            if (crate2->pos.y < crate3->pos.y) {
                if (crate3->pos.y - (crate2->pos.y + 0.5f) > size) {
                    break;
                }
                crate3 = crate2;
            }
          }
      }
      obj->pos.y = crate3->pos.y - obj->top * obj->SCALE;
      if ((crate3->shadow != 2000000.0f) && (crate3->pos.y - crate3->shadow > size))
      goto SetBot;
    }
    crate2 = &Crate[group->iCrate];
    crate3 = crate;
    for(i = 0; i < group->nCrates; i++, crate2++) {
        if (((crate2->on != 0) && (crate2->dx == crate3->dx)) && (crate2->dz == crate3->dz)) {
            if (crate2->pos.y > crate3->pos.y) {
                if (crate2->pos.y - (crate3->pos.y + 0.5f) > size) {
                    break;
                }
                crate3 = crate2;
            }
        }
    }
    obj->pos.y = (crate3->pos.y + 0.5f) - obj->bot * obj->SCALE;
  }
SetBot:
  NewTopBot(obj);
  OldTopBot(obj);
  obj->mom.y = 0.0f;
}

//NGC MATCH
float CrateBottomAbove(struct nuvec_s *pos) {
  CrateCubeGroup *group;
  CrateCube *crate;
  s32 i;
  s32 j;
  s32 type;
  float bot;
  float y;
  struct nuvec_s vNew;
  
  y = 2000000.0f;
  group = CrateGroup;
  for (i = 0; i < CRATEGROUPCOUNT; i++, group++) {
      if ((( !(pos->x < group->minclip.x) && !(pos->x > group->maxclip.x)) &&
          !(pos->z < group->minclip.z)) && !(pos->z > group->maxclip.z)) {
        vNew.x = pos->x - group->origin.x;
        vNew.z = pos->z - group->origin.z;
        NuVecRotateY(&vNew,&vNew,-(u32)group->angle);
        crate = &Crate[group->iCrate];
        for (j = 0; j < group->nCrates; j++, crate++) {
            if ((crate->on != 0)) {
                type = GetCrateType(crate,0);
                if ((type == 0 || type == -1)) continue;
                bot = crate->pos.y;
                if (!(pos->y > bot)) {
                    if ( ( (vNew.x < crate->dx  * 0.5f) || (vNew.x > crate->dx  * 0.5f + 0.5f))) continue;
                    if ((vNew.z < crate->dz  * 0.5f) || (vNew.z > crate->dz  * 0.5f + 0.5f)) continue;
                    if (y != 2000000.0f && !(crate->pos.y < y)) continue;
                    y = crate->pos.y;
                }

            }
        }
      }
  }
  return y;
}

//NGC MATCH
void DrawCrates(void) {
  s32 i;
  s32 j;
  s32 k;
  s32 type;
  s32 shadow;
  CrateCubeGroup *group;
  CrateCube *crate;
  struct nuspecial_s *obj;
  struct nuvec_s pos;
  struct numtx_s *m;
  float dx;
  float dz;
  float d;
  float r2;
  float shadow_r2;
  struct numtx_s mGROUP;
  struct numtx_s mCRATE;
  struct CharacterModel *model;
  float **dwa;
  
  shadow = 0;
  if (((((LDATA->flags & 0x200) == 0) && (Level != 0x1d)) || (VEHICLECONTROL != 1)) &&
     ((Level != 0x1c && (Level != 3)))) {
    r2 = 45.0f;
    if ((float)((s32)LDATA->farclip) < 45.0f) {
      r2 = (float)LDATA->farclip;
    }
    r2 *= r2;
  }
  else {
    r2 = (float)(s32)(LDATA->farclip * LDATA->farclip);
  }
  if (((((DRAWCRATESHADOWS != 0) && (VEHICLECONTROL != 2)) &&
       ((VEHICLECONTROL != 1 || ((player->obj).vehicle != 0x20)))) && ((LDATA->flags & 0x1000) == 0)
      ) && (((((LDATA->flags & 0x200) == 0 && (Level != 0x1d)) || (VEHICLECONTROL != 1)) &&
            (Level != 0xb)))) {
    shadow = 1;
    shadow_r2 = r2;
  }
  group = CrateGroup;
  for(i = 0; i < CRATEGROUPCOUNT; i++, group++) {
      NuMtxSetRotationY(&mGROUP,(uint)group->angle);
      crate = &Crate[group->iCrate];
      for(j = 0; j < group->nCrates; j++, crate++) {
          crate->in_range = 0;
          type = GetCrateType(crate,1);
          if (((type != -1) && ((crate->on != 0 || (type == 7)))) &&
             (((((LDATA->flags & 0x200) != 0 || (Level == 0x1d)) && (VEHICLECONTROL == 1)) ||
              (dx = pCam->pos.x - crate->pos.x, dz = pCam->pos.z - crate->pos.z,
              d = (dx * dx + dz * dz), !(d > r2))))) {
                crate->in_range = 1;
                pos.x = crate->pos.x;
                pos.y = crate->pos.y + 0.25f;
                pos.z = crate->pos.z;
                if ((crate->xrot != 0) || (crate->zrot != 0)) {
                  m = &mCRATE;
                  NuMtxSetRotationY(&mCRATE,(uint)group->angle);
                  NuMtxRotateZ(&mCRATE,(uint)crate->zrot);
                  NuMtxRotateX(&mCRATE,(uint)crate->xrot);
                } else {
                   m = &mGROUP;
                }
                m->_30 = pos.x;
                m->_31 = pos.y;
                m->_32 = pos.z;
                if ((crate->flags & 0x400) != 0) {
                  j = (type == 5) ? 0x43 : 0x41;
                  if (ObjTab[j].obj.special != NULL) {
                    obj = ObjTab[j].obj.special;
                    NuRndrGScnObj((ObjTab[j].obj.scene)->gobjs[obj->instance->objid],m);
                  }
                }
                k = crate_list[type].character;
                if (((u32)k < 0xbf) && (CRemap[k] != -1)) {
                  model = &CModel[CRemap[k]];
                  if ((crate->action != -1) && ((s32)crate->character == k)) {
                    if (model->fanmdata[crate->action] != NULL) {
                      dwa = NuHGobjEvalDwa(1,NULL,model->fanmdata[crate->action],crate->anim_time);
                    }
                    else {
                      dwa = NULL;
                    }
                    if (model->anmdata[crate->action] != NULL) {
                      NuHGobjEvalAnim(model->hobj,model->anmdata[crate->action],crate->anim_time,0,NULL,tmtx);
                    }
                    else {
                      NuHGobjEval(model->hobj,0,NULL,tmtx);
                    }
                  }
                  else {
                    NuHGobjEval(model->hobj,0,NULL,tmtx);
                    dwa = NULL;
                  }
                  NuHGobjRndrMtxDwa(model->hobj,m,1,NULL,tmtx,dwa);
                  if ((crate->flags & 0x200) != 0) {
                    mCRATE = *m;
                    mCRATE._01 = -mCRATE._01;
                    mCRATE._11 = -mCRATE._11;
                    mCRATE._21 = -mCRATE._21;
                    mCRATE._31 = crate->shadow - (mCRATE._31 - crate->shadow);
                    NuHGobjRndrMtxDwa(model->hobj,&mCRATE,1,NULL,tmtx,dwa);
                  }
                }
                else {
                  obj = crate_list[type].obj.special;
                  if ((obj != NULL) && (NuRndrGScnObj(crate_scene->gobjs[obj->instance->objid],m),
                     (crate->flags & 0x200) != 0)) {
                    mCRATE = *m;
                    mCRATE._01 = -mCRATE._01;
                    mCRATE._11 = -mCRATE._11;
                    mCRATE._21 = -mCRATE._21;
                    mCRATE._31 = crate->shadow - (mCRATE._31 - crate->shadow);
                    NuRndrGScnObj(crate_scene->gobjs[obj->instance->objid],&mCRATE);
                  }
                }
                if (((((shadow) && (type != 7)) && (type != 0)) &&
                    ((((crate->flags & 0x2000) != 0 && (crate->shadow != 2000000.0f)) &&
                     ((crate->pos.y > crate->shadow &&
                      ((d < shadow_r2 && (ObjTab[21].obj.special != NULL)))))))) &&
                   (LowestActiveCrate(group,crate) != 0)) {
                  NuMtxSetRotationY(&mCRATE,(uint)group->angle);
                  NuMtxRotateZ(&mCRATE,(uint)crate->surface_zrot);
                  NuMtxRotateX(&mCRATE,(uint)crate->surface_xrot);
                  mCRATE._30 = crate->pos.x;
                  mCRATE._31 = crate->shadow + 0.025f;
                  mCRATE._32 = crate->pos.z;
                  NuRndrGScnObj((ObjTab[21].obj.scene)->gobjs[(ObjTab[21].obj.special)->instance->objid],&mCRATE);
                }
          }
      }
  }
}